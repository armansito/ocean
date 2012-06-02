#include "projectedgrid.h"
#include <vector>
#include <float.h>
#include <iostream>

#define DIM 100

static bool hitTestPlane(const Vector3 &p, const Vector3 &n, const Vector3 &o, const Vector3 &r, Vector3 &result)
{
    float ndotr = n.dot(r);
    if (ndotr == 0.f) return false;
    float t = -n.dot(o - p) / ndotr;
    if (t < -0.f || t > 1.f) return false;
    result = o + r * t;
    return true;
}

ProjectedGrid::ProjectedGrid()
{
    glGenBuffers(1, &m_vbo);
    m_visible = false;
    m_vcount = DIM*DIM*4;
    m_verts = new Vector3[m_vcount];
    float x = 0.f, y = 0.f, xincr = 1.f/DIM, yincr = 1.f/DIM;
    int index = 0;
    for (int i = 0; i < DIM; i++) {
        x = 0.f;
        for (int j = 0; j < DIM; j++) {
            m_verts[index++] = Vector3(x, y, 0.f);
            m_verts[index++] = Vector3(x, y-yincr, 0.f);
            m_verts[index++] = Vector3(x+xincr, y-yincr, 0.f);
            m_verts[index++] = Vector3(x+xincr, y, 0.f);
            x += xincr;
        }
        y -= yincr;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vcount * sizeof(Vector3), (GLvoid *)m_verts, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    delete[] m_verts;
    computeRange();
}

ProjectedGrid::~ProjectedGrid()
{
    glDeleteBuffers(1, &m_vbo);
}

void ProjectedGrid::updateGrid()
{
    computeRange();
}

void ProjectedGrid::computeRange()
{
    // obtain the camera transforms
    float modelview[16];
    float projection[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
    glGetFloatv(GL_PROJECTION_MATRIX, projection); 
 
    Matrix4x4 mv, proj;
    mv.setOpenGLMatrix(modelview);
    proj.setOpenGLMatrix(projection);

    Matrix4x4 mvp = proj * mv;
    Matrix4x4 mvpinv(mvp);
    mvpinv.invert();

    // obtain frustum corners
    Vector3 frust[] = { Vector3(-1.f,-1.f,-1.f),
                        Vector3( 1.f,-1.f,-1.f),
                        Vector3( 1.f, 1.f,-1.f),
                        Vector3(-1.f, 1.f,-1.f),
                        Vector3(-1.f,-1.f, 1.f),
                        Vector3( 1.f,-1.f, 1.f),
                        Vector3( 1.f, 1.f, 1.f),
                        Vector3(-1.f, 1.f, 1.f) };
    
    for (int i = 0; i < 8; i++) {
        Vector4 v = mvpinv * Vector4(frust[i], 1.f);
        v /= v.w;
        frust[i] = v.xyz();
    }

    // find the intersection points of the frustum edges with the
    // displaceable volume
    int ind[] = { 0, 1,
                  0, 3,
                  3, 2,
                  1, 2,

                  4, 5,
                  4, 7,
                  7, 6,
                  5, 6,

                  0, 4,
                  1, 5,
                  2, 6,
                  3, 7 };
    
    std::vector<Vector3> points;
    Vector3 v0,
            no(0.f, 1.f, 0.f);
    for (int i = 0; i < 24; i += 2) {
        const Vector3 &o = frust[ind[i]];
        const Vector3 &r = frust[ind[i + 1]] - o;
        Vector3 result;
        if (hitTestPlane(v0, no, o, r, result)) points.push_back(result);
    }

    // if no intersection was found, water is not visible
    if (points.size() == 0) {
        m_visible = false;
        return;
    }
    m_visible = true;

    // project all points and find the range of the grid
    float xmin, xmax, ymin, ymax;
    xmin = ymin = FLT_MAX;
    xmax = ymax = -FLT_MAX;
    
    for (unsigned i = 0; i < points.size(); i++) {
        Vector4 p = mvp * Vector4(points[i], 1.f);
        p /= p.w;
        if (p.x > xmax) xmax = p.x;
        if (p.x < xmin) xmin = p.x;
        if (p.y > ymax) ymax = p.y;
        if (p.y < ymin) ymin = p.y;
    }
/*
    Matrix4x4 rc(1.2f*(xmax - xmin),     0.f    , 0.f, 1.2f*xmin,
                      0.f   , 1.2f*(ymax - ymin), 0.f, 1.2f*ymax,
                      0.f   ,     0.f    , 1.f,  0.f,
                      0.f   ,     0.f    , 0.f,  1.f);
*/
    Matrix4x4 rc((xmax - xmin),     0.f    , 0.f, xmin,
                      0.f   , (ymax - ymin), 0.f, ymax,
                      0.f   ,     0.f    , 1.f,  0.f,
                      0.f   ,     0.f    , 0.f,  1.f);
    m_rangec = rc;

}

void ProjectedGrid::draw()
{
    if (!m_visible) return;
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_QUADS, 0, m_vcount);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
