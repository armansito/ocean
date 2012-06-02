#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <QKeyEvent>
#include <QMouseEvent>
#include <QHash>
#include "vector.h"

class MouseDelegate
{
public:

    virtual void mouseMoved(const Vector2 &) = 0;
};

class InputHandler
{
public:

    static InputHandler *globalInstance();

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    inline const Vector2 &mousePos() const { return m_mousep; }

    inline void setMouseDelegate(MouseDelegate *md) { m_md = md; }

    bool isKeyPressed(const QString &key);
    bool isMousePressed(bool left);

private:

    Vector2 m_mousep;
    bool m_lmdown, m_rmdown;
    QHash<QString, bool> m_keymap;
    MouseDelegate *m_md;
};

#endif // INPUTHANDLER_H
