module;
#include <QtCore>

export module qt:internal;

namespace qt
{
    export struct internal
    {
        template<typename T>
        static QObject* get_qt_object(T& input)
        {
            return input.qt_object();
        }

        template<typename T, typename... Args>
        static T create(Args&&... args)
        {
            return T{ std::forward<Args>(args)... };
        }

        template<typename T>
        using qt_type = T::qt_type;
    };

    export template<typename Wrapper>
    concept QtWrapper =
    requires { typename internal::qt_type<Wrapper>; }
    && requires(internal::qt_type<Wrapper> arg) { { internal::create<Wrapper>(arg) } -> std::convertible_to<Wrapper>; };

}