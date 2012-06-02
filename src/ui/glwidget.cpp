#include "glwidget.h"
#include <qgl.h>
#include "camera.h"
#include "engine.h"
#include "inputhandler.h"
#include <QFont>

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    m_engine = NULL;
}

GLWidget::~GLWidget()
{
    delete m_engine;
}

void GLWidget::initializeGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.f);

    m_engine = new Engine();

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
    m_time.start();
    m_timer.start(1000/60);
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    m_engine->resize(w, h);
}

static int fps = 0;
void GLWidget::paintGL()
{
    m_engine->draw();
    glColor3f(1.f, 1.f, 1.f);
    glDisable(GL_DEPTH_TEST);
    renderText(5, 20, QString("%1 FPS").arg(fps), QFont("Monaco", 16));
    glEnable(GL_DEPTH_TEST);
}

static int count = 0;
static float timeacc = 0.f;
void GLWidget::tick()
{
    float dt = m_time.restart() * 0.001f;
    timeacc += dt;
    if (count++ % 60 == 0) {
        count = 1;
        fps = (60/timeacc);
        timeacc = 0.f;
    }
    m_engine->update(dt);
    updateGL();
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    InputHandler::globalInstance()->keyPressEvent(event);
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    InputHandler::globalInstance()->keyReleaseEvent(event);
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    InputHandler::globalInstance()->mouseMoveEvent(event);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    InputHandler::globalInstance()->mousePressEvent(event);
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    InputHandler::globalInstance()->mouseReleaseEvent(event);
}

