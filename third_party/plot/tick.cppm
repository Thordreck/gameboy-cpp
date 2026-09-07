module;
#include <signalsmith-plot/plot.h>

export module plot:tick;
import :internal;

namespace plot
{
    export class tick
    {
    public:
        explicit tick(const double value)
            : imp { value }
        {}

        template<typename Name>
        requires std::is_convertible_v<Name, std::string_view>
        tick(const double value, const Name name)
            : imp { value, std::string { name } }
        {}

        [[nodiscard]] double value() const { return imp.value; }
        [[nodiscard]] std::string_view name() const { return imp.name; }

        void set_value(const double value) { imp.value = value; }

        template<typename Name>
        requires std::is_convertible_v<Name, std::string_view>
        void set_name(const Name name) { imp.name = std::string { name }; }

    private:
        signalsmith::plot::Tick imp;

        friend struct internal;
        auto* get_imp() { return &imp; }
        auto const* get_imp() const { return &imp; }
    };

}
