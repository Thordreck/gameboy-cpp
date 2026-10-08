
export module tests.core:memory;

import std;
import memory;

namespace tests
{
    export [[nodiscard]] std::string read_memory_as_string(
        const memory::memory_address_t start,
        const memory::memory_address_t end,
        memory::ReadOnlyMemory auto const& memory)
    {
        std::string result {};

        for (auto address = start; address < end; address++)
        {
            const auto value = memory.read(address);
            if (value == '\0') { break; }

            result += value;
        }

        return result;
    }

}