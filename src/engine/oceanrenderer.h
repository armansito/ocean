#ifndef OCEANRENDERER_H
#define OCEANRENDERER_H

#include <QGLShaderProgram>

class ProjectedGrid;

class OceanRenderer
{
public:

    OceanRenderer();
    ~OceanRenderer();

    void update(float seconds);
    void draw();

    void cameraMoved();

private:

    ProjectedGrid *m_grid;
    QGLShaderProgram *m_prog;
};

#endif // OCEANRENDERER_H
