module;
#include <QtCore>

export module qt:core;

import std;
import :internal;

namespace qt
{
    export class object
    {
        explicit object(QObject* qt_object)
            : handle{qt_object}
        {}

        [[nodiscard]] QObject* qt_object() const { return handle; }
        friend internal;

        QObject* handle;
    };
}