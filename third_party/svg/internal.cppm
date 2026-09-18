
export module svg:internal;
import std;

namespace svg
{
    export struct internal
    {
        template<typename T, typename... Args>
        static T create(Args&&... args)
        {
            return T(std::forward<Args>(args)...);
        }
    };

}
