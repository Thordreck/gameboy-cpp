
export module cartridge:header;

import :common;
import std;

namespace cartridge
{
    export struct header
    {
        std::string title;
        std::optional<std::string> manufacturer;
        std::optional<cgb_mode> cgb_flag;
        std::string licensee;
        bool sgb_support;
        std::uint64_t rom_size;
        std::uint64_t ram_size;
        destination_code destination;
        std::uint8_t version;
        hardware_type hardware;
    };

    export [[nodiscard]] std::string pretty_print(const hardware_type type)
    {
        using enum hardware_type;

        switch (type)
        {
        case rom_only: return "ROM Only";
        case mbc1: return "MBC1";
        case mbc1_ram: return "MBC1 (ram)";
        case mbc1_ram_battery: return "MBC1 (ram + battery)";
        case mbc2: return "MBC2";
        case mbc2_battery: return "MBC2 (battery)";
        case rom_ram: return "ROM RAM";
        case rom_ram_battery: return "ROM RAM (battery)";
        case mmm_01: return "MMM01";
        case mmm_01_ram: return "MMM01 (ram)";
        case mmm_01_ram_battery: return "MMM01 (ram + battery)";
        case mbc3_timer_battery: return "MBC3 (timer + battery)";
        case mbc3_timer_ram_battery: return "MBC3 (timer + ram + battery)";
        case mbc3: return "MBC3";
        case mbc3_ram: return "MBC3 (ram)";
        case mbc3_ram_battery: return "MBC3 (ram + battery)";
        case mbc5: return "MBC5";
        case mbc5_ram: return "MBC5 (ram)";
        case mbc5_ram_battery: return "MBC5 (ram + battery)";
        case mbc5_rumble: return "MBC5 (rumble)";
        case mbc5_rumble_ram: return "MBC5 (rumble + ram)";
        case mbc5_rumble_ram_battery: return "MBC5 (rumble + ram + battery)";
        case mbc6: return "MBC6";
        case mbc7_sensor_rumble_ram_battery: return "MBC6 (rumble + ram + battery)";
        case pocket_camera: return "Pocket Camera";
        case bandai_tama5: return "Bandai Tama 5";
        case huc3: return "HUC3";
        case huc1_ram_battery: return "HUC1 (ram + battery)";
        case unknown: return "Unknown";
        default: std::unreachable();
        }
    }

    export [[nodiscard]] std::string pretty_print(const header& header)
    {
        return std::format(
            "Title: {}\n"
            "Manufacturer: {}\n"
            "DMG support: {}\n"
            "Licensee: {}\n"
            "SGB support: {}\n"
            "Rom size: {}\n"
            "Ram size: {}\n"
            "Destination: {}\n"
            "Version: {}\n"
            "Hardware: {}",
            header.title,
            header.manufacturer.value_or("Unknown"),
            header.cgb_flag.transform([](const auto flag) { return flag == cgb_mode::dmg_compatible; }).value_or(true) ? "Yes" : "No",
            header.licensee,
            header.sgb_support ? "Yes" : "No",
            header.rom_size,
            header.ram_size,
            header.destination == destination_code::japan ? "Japan" : "Overseas",
            header.version,
            pretty_print(header.hardware));
    }
}