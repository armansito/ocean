#ifndef ENGINE_H
#define ENGINE_H

#include "inputhandler.h"

class Camera;
class OceanRenderer;

class Engine : public MouseDelegate
{
public:

    /** Initializes the rendering engine.
      * Should be called once an OpenGL context
      * is present.
      */
    Engine();
    ~Engine();

    /** Called when the screen size changes.
      */
    void resize(int width, int height);

    /** Updates the simulation.
      *
      * @param seconds  - total elapsed time since
      *                   the last update 
      */
    void update(float seconds);
    
    /**
      * Renders the scene.
      */
    void draw();


    /** MouseDelegate callback */
    void mouseMoved(const Vector2 &);

private:

    Camera *m_camera;
    OceanRenderer *m_ocean;
};

#endif // ENGINE_H
