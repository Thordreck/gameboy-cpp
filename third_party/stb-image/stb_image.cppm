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

    export class image
    {
    public:
        image(image_data_t* data, const image_metadata& metadata)
            : data(data)
            , metadata(metadata)
        {}

        ~image()
        {
            stbi_image_free(data);
            data = nullptr;
        }

    private:
        image_data_t* data;
        const image_metadata metadata;
    };

    export [[nodiscard]] stb_result<image> load_image(const std::filesystem::path& path)
    {
        if (!std::filesystem::exists(path))
        {
            return std::unexpected("Path does not exist");
        }

        image_metadata metadata {};
        std::uint8_t* data = stbi_load(
            path.string().c_str(),
            &metadata.width,
            &metadata.height,
            &metadata.channels,
            0);

        if (data == nullptr)
        {
            return std::unexpected("Could not load image");
        }

        return image{data, metadata};
    }

    export stb_result<void> write_png(const std::filesystem::path& path, const image_data_t* data, const image_metadata& metadata)
    {
        const int error = stbi_write_png(
            path.string().c_str(),
            metadata.width,
            metadata.height,
            metadata.channels, data,
            metadata.width * metadata.channels);

        return error != 0
            ? std::unexpected("Could not write png")
            : stb_result<void>{};
    }

}
