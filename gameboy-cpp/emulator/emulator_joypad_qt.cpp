#include "emulator_joypad_qt.hpp"
#include <QKeyEvent>

namespace emulator
{
    emulator_joypad_filter::emulator_joypad_filter(const std::span<bool,8> keyboard_state, QObject *parent)
        : QObject(parent)
        , state { keyboard_state }
    {}

    bool emulator_joypad_filter::eventFilter(QObject* obj, QEvent* event)
    {
        if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease)
        {
            const auto* keyEvent = dynamic_cast<QKeyEvent*>(event);
            const bool pressed = event->type() == QEvent::KeyPress;

            switch (keyEvent->key())
            {
            case Qt::Key_Return:
                state[0] = pressed;
                break;
            case Qt::Key_Backspace:
                state[1] = pressed;
                break;
            case Qt::Key_Up:
                state[2] = pressed;
                break;
            case Qt::Key_Down:
                state[3] = pressed;
                break;
            case Qt::Key_Left:
                state[4] = pressed;
                break;
            case Qt::Key_Right:
                state[5] = pressed;
                break;
            case Qt::Key_X:
                state[6] = pressed;
                break;
            case Qt::Key_Z:
                state[7] = pressed;
                break;
            default: break;
            }
        }

        return QObject::eventFilter(obj, event);
    }
}
