
export module graphics:object_buffer;
export import :oam;
export import std;

import utilities;

namespace graphics
{
    using object_buffer = utils::fixed_capacity_vector<object, max_num_objects_per_scanline>;

    export std::optional<object> pop_object_at_x(object_buffer& objects, const std::uint8_t x)
    {
        const auto result_it = std::ranges::find_if(
            objects,
            [x] (const auto& candidate) { return static_cast<int>(candidate.x() - 8) <= x; });

        if (result_it == objects.end())
        {
            return std::nullopt;
        }

        object result = *result_it;
        objects.erase(result_it);

        return std::make_optional(result);
    }

}