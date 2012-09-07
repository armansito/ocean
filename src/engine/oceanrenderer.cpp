#include "oceanrenderer.h"

#include <math.h>

#include "projectedgrid.h"

// texture ids
enum { kWaveTexture };

static long lrandom(long *seed)
{
    *seed = (*seed * 1103515245 + 12345) & 0x7FFFFFFF;
    return *seed;
}

static float frandom(long *seed)
{
    long r = lrandom(seed) >> (31 - 24);
    return r / (float)(1 << 24);
}

static float grandom(float mean, float std_deviation, long *seed)
{
    float x1, x2, w, y1;
    static float y2;
    static int use_last = 0;

    if (use_last) {
        y1 = y2;
        use_last = 0;
    } else {
        do {
            x1 = 2.0f * frandom(seed) - 1.0f;
            x2 = 2.0f * frandom(seed) - 1.0f;
            w  = x1 * x1 + x2 * x2;
        } while (w >= 1.0f);
        w  = sqrt((-2.0f * log(w)) / w);
        y1 = x1 * w;
        y2 = x2 * w;
        use_last = 1;
    }
	return mean + y1 * std_deviation;
}

OceanRenderer::OceanRenderer()
{
    m_grid = new ProjectedGrid();
    m_prog = new QGLShaderProgram();
    m_prog->addShaderFromSourceFile(QGLShader::Vertex, ":/shaders/ocean.vert");
    m_prog->addShaderFromSourceFile(QGLShader::Fragment, ":/shaders/ocean.frag");
    m_prog->link();

    initializeTextures();

    m_wavebuffer = new Wave[m_params.number_of_waves];
    generateWaves();
}

OceanRenderer::~OceanRenderer()
{
    delete m_grid;
    delete m_prog;
    delete[] m_wavebuffer;
    foreach (GLuint tex, m_textures)
        glDeleteTextures(1, &tex);
}

void OceanRenderer::initializeTextures()
{
    GLuint tex;
    glActiveTexture(GL_TEXTURE0 + kWaveTexture);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_1D, tex);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_textures["wave"] = tex;
}

void OceanRenderer::generateWaves()
{
    long seed = 1234567;
    float min = log(m_params.lambda_min) / log(2.0f);
    float max = log(m_params.lambda_max) / log(2.0f);

    #define srnd() (2*frandom(&seed) - 1)
    #define num_angles 5
    #define angle(i) (1.5f*(((i)%num_angles)/(float)(num_angles/2)-1))
    #define dangle() (1.5f/(float)(num_angles/2))

    float Wa[num_angles];
    int index[num_angles];
    float s = 0;
    for (int i = 0; i < num_angles; ++i) {
        index[i] = i;
        float a = angle(i);
        s += Wa[i] = exp(-0.5f*a*a);
    }
    for (int i = 0; i < num_angles; ++i) {
        Wa[i] /= s;
    }

    for (int i = 0; i < m_params.number_of_waves; ++i) {
        float x = i / (m_params.number_of_waves - 1.0f);

        float lambda = pow(2.0f, (1.0f - x) * min + x *max);
        float ktheta = grandom(0.0f, 1.0f, &seed) * m_params.wave_dispersion;
        float knorm = 2.0f * M_PI / lambda;
        float omega = sqrt(9.81f * knorm);
        float amplitude;
        
        float step = (max-min) / (m_params.number_of_waves - 1);
        float omega0 = 9.81 / m_params.U0;
        if ((i%num_angles) == 0) {
            for (int k = 0; k < num_angles; ++k) {
                int n1 = lrandom(&seed)%num_angles,
                    n2 = lrandom(&seed)%num_angles,
                    n = index[n1];
                    index[n1] = index[n2];
                    index[n2] = n;
            }
        }
        ktheta = m_params.wave_dispersion*(angle(index[(i)%num_angles])+0.4f*srnd()*dangle());
        ktheta *= 1.f/(1.f+40.f*pow(omega0/omega,4));
        amplitude = (8.1e-3*9.81f*9.81f)/pow(omega,5)*exp(-0.74*pow(omega0/omega,4));
        amplitude *= 0.5f*sqrt(2.f*M_PI*9.81f/lambda)*num_angles*step;
        amplitude = 3*m_params.height_max*sqrt(amplitude);
        if (amplitude > 1.f / knorm)
            amplitude = 1.f / knorm;
        else if (amplitude < -1.f / knorm)
            amplitude = 1.f / knorm;

        m_wavebuffer[i].amplitude = amplitude;
        m_wavebuffer[i].omega = omega;
        m_wavebuffer[i].kx = knorm * cos(ktheta);
        m_wavebuffer[i].kz = knorm * sin(ktheta);
    }

    #undef srnd()
    #undef num_angles
    #undef angle(i)
    #undef dangle()

    glActiveTexture(GL_TEXTURE0 + kWaveTexture);
    glBindTexture(GL_TEXTURE_1D, m_textures["wave"]);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F_ARB, m_params.number_of_waves,
                 0, GL_RGBA, GL_FLOAT, (const GLvoid *)m_wavebuffer);
}

static float t = 0.f;
void OceanRenderer::update(float seconds)
{
    t += seconds;
}

void OceanRenderer::draw()
{
    const Matrix4x4 &rc = m_grid->rangeConversionMatrix();
    GLfloat rcm[4][4];
    rc.toOpenGLMatrix((GLfloat *)rcm);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    m_prog->bind();
    m_prog->setUniformValue("rangec", rcm);
    glActiveTexture(GL_TEXTURE0 + kWaveTexture);
    glBindTexture(GL_TEXTURE_1D, m_textures["wave"]);
    m_prog->setUniformValue("wave_texture", kWaveTexture);
    m_prog->setUniformValue("num_waves", (float)m_params.number_of_waves);
    m_prog->setUniformValue("time", t);
    m_grid->draw();
    m_prog->release();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void OceanRenderer::cameraMoved()
{
    m_grid->updateGrid();
}
