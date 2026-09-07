export module plot:internal;
import std;

namespace plot
{
    export struct internal
    {
        template<typename Wrapper>
        static auto* get_imp(Wrapper& wrapper)
        {
            return wrapper.get_imp();
        }

        template<typename Wrapper>
        static auto const* get_imp(const Wrapper& wrapper)
        {
            return wrapper.get_imp();
        }
    };

    export template <typename Imp, typename Wrapper>
    concept WrapperFor = requires(Wrapper& wrapper)
    {
        { internal::get_imp(wrapper) } -> std::convertible_to<Imp*>;
    };

}