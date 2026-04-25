export module cartridge:parser;

import :rom;
import :common;
import :header;

import std;
import utilities;

namespace cartridge
{
    constexpr std::uint16_t header_start_address = 0x100;
    constexpr std::uint16_t header_end_address = 0x14F;
    constexpr std::uint16_t cgb_flag_address = 0x143;
    constexpr std::uint16_t title_start_address = 0x134;
    constexpr std::uint16_t manufacturer_start_address = 0x13F;
    constexpr std::uint16_t manufacturer_end_address = 0x142;
    constexpr std::uint16_t sgb_flag_address = 0x146;
    constexpr std::uint16_t hardware_type_address = 0x147;
    constexpr std::uint16_t rom_size_address = 0x148;
    constexpr std::uint16_t ram_size_address = 0x149;
    constexpr std::uint16_t destination_code_address = 0x14A;
    constexpr std::uint16_t old_licensee_code_address = 0x14B;
    constexpr std::uint16_t new_licensee_code_start_address = 0x144;
    constexpr std::uint16_t new_licensee_code_end_address = 0x145;
    constexpr std::uint16_t version_address = 0x14C;

    export using const_rom_data_t = std::span<const std::uint8_t>;

    [[nodiscard]] std::string parse_title(const const_rom_data_t rom, const bool cgb_cartridge)
    {
        const std::uint8_t title_size = cgb_cartridge ? 11 : 15;
        const std::uint16_t title_end_address = title_start_address + title_size;

        std::string title{
            std::next(rom.begin(), title_start_address),
            std::next(rom.begin(), title_end_address)
        };

        title.erase(title.find_last_not_of('\0') + 1, std::string::npos);
        return title;
    }

    [[nodiscard]] std::string parse_manufacturer(const const_rom_data_t rom)
    {
        return {
            std::next(rom.begin(), manufacturer_start_address),
            std::next(rom.begin(), manufacturer_end_address)
        };
    }

    [[nodiscard]] cgb_mode parse_cgb_flag(const const_rom_data_t rom)
    {
        using enum cgb_mode;
        return rom[cgb_flag_address] == 0x80 ? dmg_compatible : cgb_only;
    }

    [[nodiscard]] std::string parse_old_licensee(const const_rom_data_t rom)
    {
        switch (rom[old_licensee_code_address])
        {
        case 0x00: return "None";
        case 0x01: return "Nintendo";
        case 0x08: return "Capcom";
        case 0x09: return "HOT-B";
        case 0x0A: return "Jaleco";
        case 0x0B: return "Coconuts Japan";
        case 0x0C: return "Elite Systems";
        case 0x13: return "Electronics Arts";
        case 0x18: return "Hudson Soft";
        case 0x19: return "ITC Entertainment";
        case 0x1A: return "Yanoman";
        case 0x1D: return "Japan Clary";
        case 0x1F: return "Virgin Games Ltd";
        case 0x24: return "PCM Complete";
        case 0x25: return "San-X";
        case 0x28: return "Kemco";
        case 0x29: return "SETA Corporation";
        case 0x30: return "Infogrames";
        case 0x31: return "Nintendo";
        case 0x32: return "Bandai";
        case 0x34: return "Konami";
        case 0x35: return "HectorSoft";
        case 0x38: return "Capcom";
        case 0x39: return "Banpresto";
        case 0x3C: return "Entertainment Interactive";
        case 0x3E: return "Gremlin";
        case 0x41: return "Ubi Soft";
        case 0x42: return "Atlus";
        case 0x44: return "Malibu Interactive";
        case 0x46: return "Angel";
        case 0x47: return "Spectrum HoloByte";
        case 0x49: return "Irem";
        case 0x4A: return "virgin Games Ltd";
        case 0x4D: return "Malibu Interactive";
        case 0x4F: return "U.S. Gold";
        case 0x50: return "Absolute";
        case 0x51: return "Entertainment";
        case 0x52: return "Activision";
        case 0x53: return "Sammy USA Corporation";
        case 0x54: return "GameTek";
        case 0x55: return "Park Place";
        case 0x56: return "LJN";
        case 0x57: return "Matchbox";
        case 0x59: return "Milton Bradley Company";
        case 0x5A: return "Mindscape";
        case 0x5B: return "Romstar";
        case 0x5C: return "Naxat Soft";
        case 0x5D: return "Tradewest";
        case 0x60: return "Titus Interactive";
        case 0x61: return "Virgin Games Ltd";
        case 0x67: return "Ocean Software";
        case 0x69: return "EA (Electronic Arts)";
        case 0x6E: return "Elite Systems";
        case 0x6F: return "Electro Brain";
        case 0x70: return "Infogrames";
        case 0x71: return "Interplay Entertainment";
        case 0x72: return "Broderbund";
        case 0x73: return "Sculptured Software";
        case 0x75: return "The Sales Curve Limited";
        case 0x78: return "THQ";
        case 0x79: return "Accolade";
        case 0x7A: return "Triffix Entertainment";
        case 0x7C: return "MicroProse";
        case 0x7F: return "Kemco";
        case 0x80: return "Misawa Entertainment";
        case 0x83: return "LOZC G.";
        case 0x86: return "Tokuma Shoten";
        case 0x8B: return "Bullet-Proof Software";
        case 0x8C: return "Vic Tokai Corp.";
        case 0x8E: return "Ape Inc.";
        case 0x8F: return "I’Max";
        case 0x91: return "Chunsoft Co.";
        case 0x92: return "Video System";
        case 0x93: return "Tsubaraya Productions";
        case 0x95: return "Varie";
        case 0x96: return "Yonezawa/S’Pal";
        case 0x97: return "Kemco";
        case 0x99: return "Arc";
        case 0x9A: return "Nihon Bussan";
        case 0x9B: return "Tecmo";
        case 0x9C: return "Imagineer";
        case 0x9D: return "Banpresto";
        case 0x9F: return "Nova";
        case 0xA1: return "Hori Electric";
        case 0xA2: return "Bandai";
        case 0xA4: return "Konami";
        case 0xA6: return "Kawada";
        case 0xA7: return "Takara";
        case 0xA9: return "Technos Japan";
        case 0xAA: return "Broderbund";
        case 0xAC: return "Toei Animation";
        case 0xAD: return "Toho";
        case 0xAF: return "Namco";
        case 0xB0: return "Acclaim Entertainment";
        case 0xB1: return "ASCII Corporation or Nexsoft";
        case 0xB2: return "Bandai";
        case 0xB4: return "Square Enix";
        case 0xB6: return "HAL Laboratory";
        case 0xB7: return "SNK";
        case 0xB9: return "Pony Canyon";
        case 0xBA: return "Culture Brain";
        case 0xBB: return "Sunsoft";
        case 0xBD: return "Sony Imagesoft";
        case 0xBF: return "Sammy Corporation";
        case 0xC0: return "Taito";
        case 0xC2: return "Kemco";
        case 0xC3: return "Square";
        case 0xC4: return "Tokuma Shoten";
        case 0xC5: return "Data East";
        case 0xC6: return "Tonkin House";
        case 0xC8: return "Koei";
        case 0xC9: return "UFL";
        case 0xCA: return "Ultra Games";
        case 0xCB: return "VAP, Inc.";
        case 0xCC: return "Use Corporation";
        case 0xCD: return "Meldac";
        case 0xCE: return "Pony Canyon";
        case 0xCF: return "Angel";
        case 0xD0: return "Taito";
        case 0xD1: return "SOFEL (Software Engineering Lab)";
        case 0xD2: return "Quest";
        case 0xD3: return "Sigma Enterprises";
        case 0xD4: return "ASK Kodansha Co.";
        case 0xD6: return "Naxat Soft16";
        case 0xD7: return "Copya System";
        case 0xD9: return "Banpresto";
        case 0xDA: return "Tomy";
        case 0xDB: return "LJN";
        case 0xDD: return "Nippon Computer Systems";
        case 0xDE: return "Human Ent.";
        case 0xDF: return "Altron";
        case 0xE0: return "Jaleco";
        case 0xE1: return "Towa Chiki";
        case 0xE2: return "Yutaka";
        case 0xE3: return "Varie";
        case 0xE5: return "Epoch";
        case 0xE7: return "Athena";
        case 0xE8: return "Asmik Ace Entertainment";
        case 0xE9: return "Natsume";
        case 0xEA: return "King Records";
        case 0xEB: return "Atlus";
        case 0xEC: return "Epic/Sony Records";
        case 0xEE: return "IGS";
        case 0xF0: return "A Wave";
        case 0xF3: return "Extreme Entertainment";
        case 0xFF: return "LJN";
        default: return "Unknown";
        }
    }

    [[nodiscard]] std::string parse_new_licensee(const const_rom_data_t rom)
    {
        constexpr auto make_code = [](const std::uint8_t a, const std::uint8_t b) -> std::uint16_t
        {
            return (static_cast<std::uint16_t>(a) << 8) | b;
        };

        const std::uint16_t licensee_code = make_code(
            rom[new_licensee_code_start_address],
            rom[new_licensee_code_end_address]);

        switch (licensee_code)
        {
        case make_code('0', '0'): return "None";
        case make_code('0', '1'): return "Nintendo Research & Development 1";
        case make_code('0', '8'): return "Capcom";
        case make_code('1', '3'): return "EA (Electronic Arts)";
        case make_code('1', '8'): return "Hudson Soft";
        case make_code('1', '9'): return "B-AI";
        case make_code('2', '0'): return "KSS";
        case make_code('2', '2'): return "Planning Office WADA";
        case make_code('2', '4'): return "PCM Complete";
        case make_code('2', '5'): return "San-X";
        case make_code('2', '8'): return "Kemco";
        case make_code('2', '9'): return "SETA Corporation";
        case make_code('3', '0'): return "Viacom";
        case make_code('3', '1'): return "Nintendo";
        case make_code('3', '2'): return "Bandai";
        case make_code('3', '3'): return "Ocean Software/Acclaim Entertainment";
        case make_code('3', '4'): return "Konami";
        case make_code('3', '5'): return "HectorSoft";
        case make_code('3', '7'): return "Taito";
        case make_code('3', '8'): return "Hudson Soft";
        case make_code('3', '9'): return "Banpresto";
        case make_code('4', '1'): return "Ubi Soft1";
        case make_code('4', '2'): return "Atlus";
        case make_code('4', '4'): return "Malibu Interactive";
        case make_code('4', '6'): return "Angel";
        case make_code('4', '7'): return "Bullet-Proof Software";
        case make_code('4', '9'): return "Irem";
        case make_code('5', '0'): return "Absolute";
        case make_code('5', '1'): return "Acclaim Entertainment";
        case make_code('5', '2'): return "Activision";
        case make_code('5', '3'): return "Sammy USA Corporation";
        case make_code('5', '4'): return "Konami";
        case make_code('5', '5'): return "Hi Tech Expressions";
        case make_code('5', '6'): return "LJN";
        case make_code('5', '7'): return "Matchbox";
        case make_code('5', '8'): return "Mattel";
        case make_code('5', '9'): return "Milton Bradley Company";
        case make_code('6', '0'): return "Titus Interactive";
        case make_code('6', '1'): return "Virgin Games Ltd.";
        case make_code('6', '4'): return "Lucasfilm Games";
        case make_code('6', '7'): return "Ocean Software";
        case make_code('6', '9'): return "EA (Electronic Arts)";
        case make_code('7', '0'): return "Infogrames";
        case make_code('7', '1'): return "Interplay Entertainment";
        case make_code('7', '2'): return "Broderbund";
        case make_code('7', '3'): return "Sculptured Software";
        case make_code('7', '5'): return "The Sales Curve Limited";
        case make_code('7', '8'): return "THQ";
        case make_code('7', '9'): return "Accolade";
        case make_code('8', '0'): return "Misawa Entertainment";
        case make_code('8', '3'): return "LOZC G.";
        case make_code('8', '6'): return "Tokuma Shoten";
        case make_code('8', '7'): return "Tsukuda Original";
        case make_code('9', '1'): return "Chunsoft Co.";
        case make_code('9', '2'): return "Video System";
        case make_code('9', '3'): return "Ocean Software/Acclaim Entertainment";
        case make_code('9', '5'): return "Varie";
        case make_code('9', '6'): return "Yonezawa10/S’Pal";
        case make_code('9', '7'): return "Kaneko";
        case make_code('9', '9'): return "Pack-In-Video";
        case make_code('9', 'H'): return "Bottom Up";
        case make_code('A', '4'): return "Konami (Yu-Gi-Oh!)";
        case make_code('B', 'L'): return "MTO";
        case make_code('D', 'K'): return "Kodansha";
        default: return "Unknown";
        }
    }

    [[nodiscard]] std::string parse_licensee(const const_rom_data_t rom)
    {
        return rom[old_licensee_code_address] == 0x33
                   ? parse_new_licensee(rom)
                   : parse_old_licensee(rom);
    }

    [[nodiscard]] bool parse_sgb_support_flag(const const_rom_data_t rom)
    {
        return rom[sgb_flag_address] == 0x03;
    }

    [[nodiscard]] std::uint64_t parse_rom_size(const const_rom_data_t rom)
    {
        return 32 * 1024 * (1 << rom[rom_size_address]);
    }

    [[nodiscard]] std::uint64_t parse_ram_size(const const_rom_data_t rom)
    {
        switch (rom[ram_size_address])
        {
        case 0x02: return 8 * 1024;
        case 0x03: return 32 * 1024;
        case 0x04: return 128 * 1024;
        case 0x05: return 64 * 1024;
        default: return 0;
        }
    }

    [[nodiscard]] destination_code parse_destination_code(const const_rom_data_t rom)
    {
        using enum destination_code;
        return rom[destination_code_address] == 0x0 ? japan : overseas;
    }

    [[nodiscard]] std::uint8_t parse_version(const const_rom_data_t rom)
    {
        return rom[version_address];
    }

    [[nodiscard]] hardware_type parse_hardware_type(const const_rom_data_t rom)
    {
        using enum hardware_type;

        switch (rom[hardware_type_address])
        {
        case 0x00: return rom_only;
        case 0x01: return mbc1;
        case 0x02: return mbc1_ram;
        case 0x03: return mbc1_ram_battery;
        case 0x05: return mbc2;
        case 0x06: return mbc2_battery;
        case 0x08: return rom_ram;
        case 0x09: return rom_ram_battery;
        case 0x0B: return mmm_01;
        case 0x0C: return mmm_01_ram;
        case 0x0D: return mmm_01_ram_battery;
        case 0x0F: return mbc3_timer_battery;
        case 0x10: return mbc3_timer_ram_battery;
        case 0x11: return mbc3;
        case 0x12: return mbc3_ram;
        case 0x13: return mbc3_ram_battery;
        case 0x19: return mbc5;
        case 0x1A: return mbc5_ram;
        case 0x1B: return mbc5_ram_battery;
        case 0x1C: return mbc5_rumble;
        case 0x1D: return mbc5_rumble_ram;
        case 0x1E: return mbc5_rumble_ram_battery;
        case 0x20: return mbc6;
        case 0x22: return mbc7_sensor_rumble_ram_battery;
        case 0xFC: return pocket_camera;
        case 0xFD: return bandai_tama5;
        case 0xFE: return huc3;
        case 0xFF: return huc1_ram_battery;
        default: return unknown;
        }
    }

    export [[nodiscard]] std::expected<header, std::string> parse_header(const const_rom_data_t rom)
    {
        if (rom.size() < header_end_address)
        {
            return std::unexpected{"Unexpected rom header end"};
        }

        const bool cgb_cartridge = rom[cgb_flag_address] & 0x80 != 0;

        return header
        {
            parse_title(rom, cgb_cartridge),
            cgb_cartridge ? std::make_optional(parse_manufacturer(rom)) : std::nullopt,
            cgb_cartridge ? std::make_optional(parse_cgb_flag(rom)) : std::nullopt,
            parse_licensee(rom),
            parse_sgb_support_flag(rom),
            parse_rom_size(rom),
            parse_ram_size(rom),
            parse_destination_code(rom),
            parse_version(rom),
            parse_hardware_type(rom),
        };
    }

    export [[nodiscard]] std::expected<rom, std::string> load_rom_file(const std::filesystem::path& path)
    {
        const auto read_file = utils::read_binary_file(path);
        const auto read_header = read_file.and_then(parse_header);

        if (!read_header.has_value())
        {
            return std::unexpected{ read_file.error() };
        }

        return rom { read_header.value(), read_file.value() };
    }
}
