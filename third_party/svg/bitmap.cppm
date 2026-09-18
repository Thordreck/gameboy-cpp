module;
#include <lunasvg.h>

export module svg:bitmap;

import std;

import :common;
import :internal;

namespace svg
{
    using upstream_bitmap = lunasvg::Bitmap;

    export class bitmap
    {
    public:
        [[nodiscard]] outcome save_as_png(const std::filesystem::path& path)
        {
            if (!imp.writeToPng(path.string()))
            {
                return std::unexpected { std::format("Could not save svg bitmap at {}", path.string()) };
            }

            return {};
        }

    private:
        explicit bitmap(upstream_bitmap&& imp)
            : imp { std::move(imp) }
        {}

        explicit bitmap(const upstream_bitmap& imp)
            : imp { imp }
        {}

        upstream_bitmap imp;
        friend struct internal;
    };

}