
export module utilities:io;
export import std;

namespace utils
{
    export using binary_data_t = std::vector<std::uint8_t>;

    export template <typename T>
    using read_file_result = std::expected<T, std::string>;

    export [[nodiscard]] read_file_result<binary_data_t> read_binary_file(const std::filesystem::path& filepath)
    {
        if (!std::filesystem::exists(filepath))
        {
            return std::unexpected{ "File does not exist" };
        }

        std::ifstream file {};
        file.exceptions(std::ifstream::goodbit);
        file.open(filepath, std::ios::in | std::ios::binary);

        if (!file)
        {
            return std::unexpected{ "Could not open file" };
        }

        const std::size_t file_size = std::filesystem::file_size(filepath);
        binary_data_t file_contents (file_size);

        file.read(reinterpret_cast<char*>(file_contents.data()), static_cast<std::streamsize>(file_contents.size()));

        if (file.bad())
        {
            return std::unexpected{ "I/O error" };
        }

        return file_contents;
    }
}