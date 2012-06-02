#include "oceanrenderer.h"
#include "projectedgrid.h"

OceanRenderer::OceanRenderer()
{
    m_grid = new ProjectedGrid();
    m_prog = new QGLShaderProgram();
    m_prog->addShaderFromSourceFile(QGLShader::Vertex, ":/shaders/ocean.vert");
    m_prog->addShaderFromSourceFile(QGLShader::Fragment, ":/shaders/ocean.frag");
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
    const Matrix4x4 &rc = m_grid->rangeConversionMatrix();
    GLfloat rcm[4][4];
    rc.toOpenGLMatrix((GLfloat *)rcm);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    m_prog->bind();
    m_prog->setUniformValue("rangec", rcm);
    m_grid->draw();
    m_prog->release();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void OceanRenderer::cameraMoved()
{
    m_grid->updateGrid();
}
