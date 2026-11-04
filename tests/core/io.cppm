export module tests.core:io;

import std;
import utilities;

namespace tests
{
    export [[nodiscard]] std::filesystem::path get_temp_file(const std::string_view extension)
    {
        using namespace utils;
        namespace files = std::filesystem;

        return (files::temp_directory_path() / generate_uuid()).replace_extension(extension);
    }

}
