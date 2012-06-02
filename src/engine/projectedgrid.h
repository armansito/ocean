#ifndef PROJECTEDGRID_H
#define PROJECTEDGRID_H

#include <qgl.h>
#include "vector.h"

class ProjectedGrid
{
public:

    ProjectedGrid();
    ~ProjectedGrid();

    void updateGrid();
    void draw();

private:

    void computeVertices();

    GLuint m_vbo;
    Vector3 *m_verts;
    unsigned m_vcount;
    bool m_visible;
};

#endif // PROJECTEDGRID_H
