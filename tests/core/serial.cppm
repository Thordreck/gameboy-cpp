
export module tests.core:serial;
import std;

namespace tests
{
    export class dummy_serial
    {
    public:
        [[nodiscard]] static std::uint8_t transfer_bit(const std::uint8_t) { return 0x00; }
    };

    export class memory_serial
    {
    public:
        [[nodiscard]] std::string result() const
        {
            return { buffer.begin(), buffer.end() };
        }

        [[nodiscard]] std::uint8_t transfer_bit(const std::uint8_t bit)
        {
            current_byte = current_byte << 1  | (bit & 0b1);

            if (++bits_received >= 8)
            {
                buffer.push_back(current_byte);
                bits_received = 0;
            }

            return 0x00;
        }

    private:
        std::vector<std::uint8_t> buffer {};
        std::uint8_t current_byte {};
        std::uint8_t bits_received {};
    };


}