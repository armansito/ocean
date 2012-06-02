#include "inputhandler.h"

InputHandler inputhandler_global;

InputHandler *InputHandler::globalInstance()
{
    return &inputhandler_global;
}
    
void InputHandler::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    switch (key)
    {
    case Qt::Key_W:
        m_keymap["W"] = true;
        break;
    case Qt::Key_A:
        m_keymap["A"] = true;
        break;
    case Qt::Key_S:
        m_keymap["S"] = true;
        break;
    case Qt::Key_D:
        m_keymap["D"] = true;
        break;
    case Qt::Key_Space:
        m_keymap["Space"] = true;
        break;
    case Qt::Key_C:
        m_keymap["C"] = true;
        break;
    case Qt::Key_Escape:
        m_keymap["Esc"] = true;
        break;
    default:
        break;
    }
}

void InputHandler::keyReleaseEvent(QKeyEvent *event)
{
    int key = event->key();
    switch (key)
    {
    case Qt::Key_W:
        m_keymap["W"] = false;
        break;
    case Qt::Key_A:
        m_keymap["A"] = false;
        break;
    case Qt::Key_S:
        m_keymap["S"] = false;
        break;
    case Qt::Key_D:
        m_keymap["D"] = false;
        break;
    case Qt::Key_Space:
        m_keymap["Space"] = false;
        break;
    case Qt::Key_C:
        m_keymap["C"] = false;
        break;
    case Qt::Key_Escape:
        m_keymap["Esc"] = false;
        break;
    default:
        break;
    }
}

void InputHandler::mouseMoveEvent(QMouseEvent *event)
{
    Vector2 pos(event->x(), event->y());
    m_md->mouseMoved(pos - m_mousep);
    m_mousep = pos;
}

void InputHandler::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_lmdown = true;
        if (!m_rmdown) {
            m_mousep = Vector2(event->x(), event->y());
        }
    }
    else if (event->button() == Qt::RightButton) {
        m_rmdown = true;
        if (!m_lmdown) {
            m_mousep = Vector2(event->x(), event->y());
        }
    }
}

void InputHandler::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        m_lmdown = false;
    else if (event->button() == Qt::RightButton)
        m_rmdown = false;
}

bool InputHandler::isKeyPressed(const QString &key)
{
    return m_keymap.value(key, false);
}

bool InputHandler::isMousePressed(bool left)
{
    return left ? m_lmdown : m_rmdown;
}
