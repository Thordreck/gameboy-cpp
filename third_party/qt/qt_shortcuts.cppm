module;
#include <QKeySequence>
#include <QShortcut>

export module qt:shortcut;

import std;

import :internal;
import :keys;

namespace qt
{
    export enum class shortcut_context : std::uint8_t
    {
        widget = Qt::WidgetShortcut,
        widget_with_children = Qt::WidgetWithChildrenShortcut,
        window = Qt::WindowShortcut,
        application = Qt::ApplicationShortcut,
    };

    export template <QtObject Parent, std::invocable Functor>
    void register_shortcut(const standard_key key, Functor&& functor, const shortcut_context context, Parent& parent)
    {
        QObject* qt_parent { internal::get_qt_object(parent) };
        QKeySequence::StandardKey qt_key { static_cast<QKeySequence::StandardKey>(std::to_underlying(key)) };
        Qt::ShortcutContext qt_context { static_cast<Qt::ShortcutContext>(std::to_underlying(context)) };

        std::ignore = new QShortcut(qt_key, qt_parent, functor, qt_context);
    }
}
