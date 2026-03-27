module;

#include "stb_image.h"
#include "stb_image_write.h"

export module stb_image;
export import std;

namespace stb
{
    export template <typename T>
    using stb_result = std::expected<T, std::string>;

    export using image_data_t = std::uint8_t;

    export struct image_metadata
    {
        int width;
        int height;
        int channels;
    };

    static void image_data_free(image_data_t* data)
    {
        stbi_image_free(data);
    }

    export class image
    {
    public:
        image(image_data_t* data, const image_metadata& metadata) noexcept
            : image_data(data, image_data_free)
            , metadata(metadata)
        {}

        [[nodiscard]] image_data_t* data() const { return image_data.get(); }

        [[nodiscard]] std::span<image_data_t> as_span() const
        {
            return std::span{
                data(),
                static_cast<size_t>(metadata.channels) * (metadata.width * metadata.height)
            };
        }

    private:
        std::unique_ptr<image_data_t, decltype(&image_data_free)> image_data;
        image_metadata metadata;
    };

    export [[nodiscard]] stb_result<image> load_image(const std::filesystem::path& path, const std::uint8_t desired_channels = 0)
    {
        if (!std::filesystem::exists(path))
        {
            return std::unexpected("Path does not exist");
        }

        image_metadata metadata {};
        image_data_t* data = stbi_load(
            path.string().c_str(),
            &metadata.width,
            &metadata.height,
            &metadata.channels,
            desired_channels);

        if (data == nullptr)
        {
            return std::unexpected("Could not load image");
        }

        if (desired_channels != 0)
        {
            metadata.channels = desired_channels;
        }

        return image{data, metadata};
    }

    export [[nodiscard]] stb_result<void> write_png(
        const std::filesystem::path& path,
        const image_data_t* data,
        const image_metadata& metadata)
    {
        const int error = stbi_write_png(
            path.string().c_str(),
            metadata.width,
            metadata.height,
            metadata.channels,
            data,
            metadata.width * metadata.channels);

        return error == 0
            ? std::unexpected(std::format("Could not write png."))
            : stb_result<void>{};
    }

}
