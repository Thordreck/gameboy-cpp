
export module graphics:object_buffer;
export import :oam;
export import std;

import utilities;

namespace graphics
{
    using object_buffer = utils::fixed_capacity_vector<object, max_num_objects_per_scanline>;

    export std::optional<object> get_object_at_x(const object_buffer& objects, const std::uint8_t x)
    {
        const auto result_it = std::ranges::find_if(
            objects,
            [x] (const auto& candidate) { return static_cast<int>(candidate.x()) - 8 == x; });

        return result_it == objects.end()
            ? std::nullopt
            : std::make_optional(*result_it);

    }
}