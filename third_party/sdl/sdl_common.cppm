module;
#include <SDL3/SDL.h>

export module sdl:common;
export import std;

namespace sdl
{
    export template <typename T>
    using result = std::expected<T, std::string>;

    export struct color
    {
        std::uint8_t r{};
        std::uint8_t g{};
        std::uint8_t b{};
        std::uint8_t a{};
    };

    export constexpr color red = {0xFF, 0x00, 0x00, 0xFF};

    export enum class pixel_format : std::uint64_t
    {
        unknown = SDL_PIXELFORMAT_UNKNOWN,
        index_1_lsb = SDL_PIXELFORMAT_INDEX1LSB,
        index_1_msb = SDL_PIXELFORMAT_INDEX1MSB,
        index_2_lsb = SDL_PIXELFORMAT_INDEX2LSB,
        index_2_msb = SDL_PIXELFORMAT_INDEX2MSB,
        index_4_lsb = SDL_PIXELFORMAT_INDEX4LSB,
        index_4_msb = SDL_PIXELFORMAT_INDEX4MSB,
        index_8 = SDL_PIXELFORMAT_INDEX8,
        rgb_332 = SDL_PIXELFORMAT_RGB332,
        xrgb_4444 = SDL_PIXELFORMAT_XRGB4444,
        xbgr_4444 = SDL_PIXELFORMAT_XBGR4444,
        xrgb_1555 = SDL_PIXELFORMAT_XRGB1555,
        xbgr_1555 = SDL_PIXELFORMAT_XBGR1555,
        argb_4444 = SDL_PIXELFORMAT_ARGB4444,
        rgba_4444 = SDL_PIXELFORMAT_RGBA4444,
        abgr_4444 = SDL_PIXELFORMAT_ABGR4444,
        bgra_4444 = SDL_PIXELFORMAT_BGRA4444,
        argb_1555 = SDL_PIXELFORMAT_ARGB1555,
        rgba_5551 = SDL_PIXELFORMAT_RGBA5551,
        abgr_1555 = SDL_PIXELFORMAT_ABGR1555,
        bgra_5551 = SDL_PIXELFORMAT_BGRA5551,
        rgb_565 = SDL_PIXELFORMAT_RGB565,
        bgr_565 = SDL_PIXELFORMAT_BGR565,
        rgb_24 = SDL_PIXELFORMAT_RGB24,
        bgr_24 = SDL_PIXELFORMAT_BGR24,
        xrgb_8888 = SDL_PIXELFORMAT_XRGB8888,
        rgbx_8888 = SDL_PIXELFORMAT_RGBX8888,
        xbgr_8888 = SDL_PIXELFORMAT_XBGR8888,
        bgrx_8888 = SDL_PIXELFORMAT_BGRX8888,
        argb_8888 = SDL_PIXELFORMAT_ARGB8888,
        rgba_8888 = SDL_PIXELFORMAT_RGBA8888,
        abgr_8888 = SDL_PIXELFORMAT_ABGR8888,
        bgra_8888 = SDL_PIXELFORMAT_BGRA8888,
        xrgb_2101010 = SDL_PIXELFORMAT_XRGB2101010,
        xbgr_2101010 = SDL_PIXELFORMAT_XBGR2101010,
        argb_2101010 = SDL_PIXELFORMAT_ARGB2101010,
        abgr_2101010 = SDL_PIXELFORMAT_ABGR2101010,
        rgb_48 = SDL_PIXELFORMAT_RGB48,
        bgr_48 = SDL_PIXELFORMAT_BGR48,
        rgba_64 = SDL_PIXELFORMAT_RGBA64,
        argb_64 = SDL_PIXELFORMAT_ARGB64,
        bgra_64 = SDL_PIXELFORMAT_BGRA64,
        abgr_64 = SDL_PIXELFORMAT_ABGR64,
        rgb_48_float = SDL_PIXELFORMAT_RGB48_FLOAT,
        bgr_48_float = SDL_PIXELFORMAT_BGR48_FLOAT,
        rgba_64_float = SDL_PIXELFORMAT_RGBA64_FLOAT,
        argb_64_float = SDL_PIXELFORMAT_ARGB64_FLOAT,
        bgra_64_float = SDL_PIXELFORMAT_BGRA64_FLOAT,
        abgr_64_float = SDL_PIXELFORMAT_ABGR64_FLOAT,
        rgb_96_float = SDL_PIXELFORMAT_RGB96_FLOAT,
        bgr_96_float = SDL_PIXELFORMAT_BGR96_FLOAT,
        rgba_128 = SDL_PIXELFORMAT_RGBA128_FLOAT,
        argb_128_float = SDL_PIXELFORMAT_ARGB128_FLOAT,
        bgra_128_float = SDL_PIXELFORMAT_BGRA128_FLOAT,
        abgr_128_float = SDL_PIXELFORMAT_ABGR128_FLOAT,
        yv12 = SDL_PIXELFORMAT_YV12,
        iyuv = SDL_PIXELFORMAT_IYUV,
        yuy2 = SDL_PIXELFORMAT_YUY2,
        uyvy = SDL_PIXELFORMAT_UYVY,
        yvyu = SDL_PIXELFORMAT_YVYU,
        nv12 = SDL_PIXELFORMAT_NV12,
        nv21 = SDL_PIXELFORMAT_NV21,
        p010 = SDL_PIXELFORMAT_P010,
        external_oes = SDL_PIXELFORMAT_EXTERNAL_OES,
        mjpg = SDL_PIXELFORMAT_MJPG,

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rgba_32= SDL_PIXELFORMAT_RGBA8888,
        argb_32= SDL_PIXELFORMAT_ARGB8888,
        bgra_32= SDL_PIXELFORMAT_BGRA8888,
        abgr_32= SDL_PIXELFORMAT_ABGR8888,
        rgbx_32= SDL_PIXELFORMAT_RGBX8888,
        xrgb_32= SDL_PIXELFORMAT_XRGB8888,
        bgrx_32= SDL_PIXELFORMAT_BGRX8888,
        xbgr_32= SDL_PIXELFORMAT_XBGR8888,
#else
        rgba_32 = SDL_PIXELFORMAT_ABGR8888,
        argb_32 = SDL_PIXELFORMAT_BGRA8888,
        bgra_32 = SDL_PIXELFORMAT_ARGB8888,
        abgr_32 = SDL_PIXELFORMAT_RGBA8888,
        rgbx_32 = SDL_PIXELFORMAT_XBGR8888,
        xrgb_32 = SDL_PIXELFORMAT_BGRX8888,
        bgrx_32 = SDL_PIXELFORMAT_XRGB8888,
        xbgr_32 = SDL_PIXELFORMAT_RGBX8888,
#endif
    };

    export class version
    {
    public:
        [[nodiscard]] static version get()
        {
            return {SDL_GetVersion(), SDL_GetRevision()};
        }

        [[nodiscard]] int number() const { return number_; }
        [[nodiscard]] std::string_view revision() const { return revision_; }

    private:
        version(const int number, const std::string_view revision)
            : number_(number)
            , revision_(revision)
        {}

        int number_;
        std::string_view revision_;
    };

}

namespace std
{
    export template <>
    struct formatter<sdl::version>
    {
        constexpr auto parse(std::format_parse_context& ctx)
        {
            auto it = ctx.begin();

            if (it != ctx.end() && (*it == 'n' || *it == 'r' || *it == 'f'))
            {
                presentation_mode = *it++;
            }

            return it;
        }

        template <typename FormatContext>
        auto format(const sdl::version& v, FormatContext& ctx) const
        {
            switch (presentation_mode)
            {
            case 'n':
                return std::format_to(ctx.out(), "{}", v.number());
            case 'r':
                return std::format_to(ctx.out(), "{}", v.revision());
            default:
                return std::format_to(ctx.out(), "{} ({})", v.number(), v.revision());
            }
        }

    private:
        char presentation_mode = 'f';
    };
}
