#ifndef OCEANRENDERER_H
#define OCEANRENDERER_H

#include <qgl.h>
#include <QGLShaderProgram>
#include <QHash>

class ProjectedGrid;

class OceanRenderer
{
public:

    struct Parameters
    {
        Parameters()
        {
            number_of_waves = 60;
            lambda_min = 0.02f;
            lambda_max = 30.0f;
            height_max = 0.32f;
            wave_direction = 2.4f;
            U0 = 10.0f;
            wave_dispersion = 1.25f;
        }

        int number_of_waves;
        float lambda_min,
              lambda_max,
              height_max,
              wave_direction,
              U0,
              wave_dispersion;
    };

    OceanRenderer();
    ~OceanRenderer();

    void update(float seconds);
    void draw();

    void cameraMoved();

private:

    struct Wave
    {
        float amplitude,
              omega,
              kx,
              kz;
    };

    void initializeTextures();
    void generateWaves();

    ProjectedGrid *m_grid;
    QGLShaderProgram *m_prog;
    Parameters m_params;
    Wave *m_wavebuffer;
    QHash<QString, GLuint> m_textures;
};

#endif // OCEANRENDERER_H
