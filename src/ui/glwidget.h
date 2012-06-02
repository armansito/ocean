#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTimer>
#include <QTime>

class Camera;
class Engine;

class GLWidget : public QGLWidget
{
Q_OBJECT
public:
    
    explicit GLWidget(QWidget *parent);
    ~GLWidget();

private:

    void initializeGL();
    void resizeGL(int, int);
    void paintGL();

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    QTimer m_timer;
    QTime m_time;

    Engine *m_engine;

private slots:

    void tick();
};

#endif // GLWIDGET_H
