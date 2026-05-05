
export module sdl:internal;
export import :common;

namespace sdl::internal
{
    export struct native
    {
        template<typename T>
        static auto* get_handle(T&& input)
        {
            return input.native_handle();
        }
    };

    export struct wrapper
    {
        template<typename T, typename... Args>
        static T create(Args&&... args)
        {
            return T{ std::forward<Args>(args)... };
        }
    };

    export template<typename Wrapper, typename Handle>
    concept WrapperFor = requires(Wrapper& wrapper)
    {
        { native::get_handle(wrapper) } -> std::convertible_to<Handle*>;
    };

}
