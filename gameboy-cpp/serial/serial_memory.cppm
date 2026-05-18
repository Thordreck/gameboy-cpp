export module serial:memory;

import memory;
import :link;

namespace serial
{
    export constexpr memory::memory_address_t serial_transfer_data_address = 0xFF01;
    export constexpr memory::memory_address_t serial_transfer_control_address = 0xFF02;

    export [[nodiscard]] memory::memory_data_t read_serial_transfer_data_address(const link& serial)
    {
        return serial.get_transfer_data();
    }

    export [[nodiscard]] memory::memory_data_t read_serial_transfer_control_address(const link& serial)
    {
        return serial.is_transfer_enabled() << 7 | serial.is_master();
    }

    export void write_serial_transfer_data_address(link& serial, const memory::memory_data_t value)
    {
        serial.set_transfer_data(value);
    }

    export void write_serial_transfer_control_address(link& serial, const memory::memory_data_t value)
    {
        serial.set_transfer_enabled(value >> 7);
        serial.set_master(value & 0b1);
    }

}
