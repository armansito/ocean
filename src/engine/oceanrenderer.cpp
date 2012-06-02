#include "oceanrenderer.h"
#include "projectedgrid.h"

OceanRenderer::OceanRenderer()
{
    m_grid = new ProjectedGrid();
    m_prog = new QGLShaderProgram();
    m_prog->addShaderFromSourceCode(QGLShader::Vertex,
            "void main(void)"
            "{"
            "   gl_Position = ftransform();"
            "}");
    m_prog->addShaderFromSourceCode(QGLShader::Fragment,
            "void main(void)"
            "{"
            "   gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);"
            "}");
    m_prog->link();
}

OceanRenderer::~OceanRenderer()
{
    delete m_grid;
    delete m_prog;
}

void OceanRenderer::update(float seconds)
{
}

void OceanRenderer::draw()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    m_prog->bind();
    m_grid->draw();
    m_prog->release();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void OceanRenderer::cameraMoved()
{
    m_grid->updateGrid();
}
