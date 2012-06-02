#ifndef PROJECTEDGRID_H
#define PROJECTEDGRID_H

#include <qgl.h>
#include "vector.h"
#include "matrix.h"

class ProjectedGrid
{
public:

    ProjectedGrid();
    ~ProjectedGrid();

    void updateGrid();
    void draw();

    inline const Matrix4x4 &rangeConversionMatrix() const { return m_rangec; }

private:

    void computeRange();

    GLuint m_vbo;
    Vector3 *m_verts;
    unsigned m_vcount;
    bool m_visible;

    Matrix4x4 m_rangec;
};

#endif // PROJECTEDGRID_H
