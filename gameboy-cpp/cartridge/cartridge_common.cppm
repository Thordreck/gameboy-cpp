
export module cartridge:common;
import std;

namespace cartridge
{
    export enum class cgb_mode
    {
        dmg_compatible,
        cgb_only
    };

    export enum class destination_code : std::uint8_t
    {
        japan,
        overseas
    };

    export enum class hardware_type
    {
        unknown,
        rom_only,
        mbc1,
        mbc1_ram,
        mbc1_ram_battery,
        mbc2,
        mbc2_battery,
        rom_ram,
        rom_ram_battery,
        mmm_01,
        mmm_01_ram,
        mmm_01_ram_battery,
        mbc3,
        mbc3_ram,
        mbc3_ram_battery,
        mbc3_timer_battery,
        mbc3_timer_ram_battery,
        mbc5,
        mbc5_ram,
        mbc5_ram_battery,
        mbc5_rumble,
        mbc5_rumble_ram,
        mbc5_rumble_ram_battery,
        mbc6,
        mbc7_sensor_rumble_ram_battery,
        pocket_camera,
        bandai_tama5,
        huc3,
        huc1_ram_battery,
    };

}