#pragma once

#include <QtCore>
#include <span>

namespace emulator
{
    class emulator_joypad_filter : public QObject
    {
        Q_OBJECT

    public:
        explicit emulator_joypad_filter(std::span<bool,8> keyboard_state, QObject *parent = nullptr);
        bool eventFilter(QObject* obj, QEvent* event) override;

    private:
        std::span<bool, 8> state;
    };

}
