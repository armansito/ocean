#include "engine.h"
#include "camera.h"
#include "inputhandler.h"
#include "oceanrenderer.h"
#include <QApplication>
#include <qgl.h>

Engine::Engine()
{
    glClearColor(0.321f, 0.482f, 0.607f, 1.f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);

    m_camera = new Camera(45.f, 1.f, 0.1f, 10000.f);
    m_camera->setZoom(0.f);
    m_camera->move(Vector3(0.f, 15.f, 0.f));

    m_ocean = new OceanRenderer();

    InputHandler::globalInstance()->setMouseDelegate(this);
}

Engine::~Engine()
{
    delete m_camera;
}

void Engine::resize(int w, int h)
{
    m_camera->setAspect((float)w/(float)h);
    m_ocean->cameraMoved();
}

void Engine::mouseMoved(const Vector2 &delta)
{
    if (InputHandler::globalInstance()->isMousePressed(true)) {
        m_camera->rotate(0.003f * delta.x, 0.003f * delta.y);
        m_ocean->cameraMoved();
    }
}

void Engine::update(float seconds)
{
    // handle key events
    bool cammoved = false;
    const float spd = 10.f;
    InputHandler *ih = InputHandler::globalInstance();
    if (ih->isKeyPressed("Esc")) {
        QApplication::quit();
    } 
    if (ih->isKeyPressed("W")) {
        m_camera->move(m_camera->look() * seconds * spd);
        cammoved = true;
    }
    if (ih->isKeyPressed("S")) {
        m_camera->move(-m_camera->look() * seconds * spd);
        cammoved = true;
    }
    if (ih->isKeyPressed("A")) {
        Vector3 dir(m_camera->look().z, 0.f, -m_camera->look().x);
        m_camera->move(dir * seconds * spd);
        cammoved = true;
    }
    if (ih->isKeyPressed("D")) {
        Vector3 dir(-m_camera->look().z, 0.f, m_camera->look().x);
        m_camera->move(dir * seconds * spd);
        cammoved = true;
    }
    if (ih->isKeyPressed("Space")) {
        m_camera->move(Vector3(0.f, 1.f, 0.f) * seconds * spd);
        cammoved = true;
    }
    if (ih->isKeyPressed("C")) {
        m_camera->move(Vector3(0.f, -1.f, 0.f) * seconds * spd);
        cammoved = true;
    }

    m_ocean->update(seconds);
    if (cammoved) m_ocean->cameraMoved();
}

void Engine::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_camera->loadModelviewMatrix();
    m_ocean->draw();
}

