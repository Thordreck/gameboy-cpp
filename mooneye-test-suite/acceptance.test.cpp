import mooneye;
#include "doctest.h"

TEST_CASE("mooneye.acceptance.bits.mem_oam")
{
    mooneye::run_test("mem_oam.gb");
}

TEST_CASE("mooneye.acceptance.bits.reg_f")
{
    mooneye::run_test("reg_f.gb");
}

TEST_CASE("mooneye.acceptance.bits.unused_hwio")
{
    mooneye::run_test("unused_hwio-GS.gb");
}

TEST_CASE("mooneye.acceptance.instr.daa")
{
    mooneye::run_test("daa.gb");
}

TEST_CASE("mooneye.acceptance.interrupts.ie_push")
{
    mooneye::run_test("ie_push.gb");
}

TEST_CASE("mooneye.acceptance.oam_dma.basic")
{
    mooneye::run_test("basic.gb");
}

TEST_CASE("mooneye.acceptance.oam_dma.reg_read")
{
    mooneye::run_test("reg_read.gb");
}

TEST_CASE("mooneye.acceptance.oam_dma.sources")
{
    mooneye::run_test("sources-GS.gb");
}

TEST_CASE("mooneye.acceptance.ppu.hblank_ly_scx_timing")
{
    mooneye::run_test("hblank_ly_scx_timing-GS.gb");
}

TEST_CASE("mooneye.acceptance.ppu.intr_1_2_timing")
{
    mooneye::run_test("intr_1_2_timing-GS.gb");
}

TEST_CASE("mooneye.acceptance.ppu.intr_2_0_timing")
{
    mooneye::run_test("intr_2_0_timing.gb");
}

TEST_CASE("mooneye.acceptance.ppu.intr_2_mode0_timing")
{
    mooneye::run_test("intr_2_mode0_timing.gb");
}

TEST_CASE("mooneye.acceptance.ppu.intr_2_mode0_timing_sprites")
{
    mooneye::run_test("intr_2_mode0_timing_sprites.gb");
}

TEST_CASE("mooneye.acceptance.ppu.intr_2_mode3_timing")
{
    mooneye::run_test("intr_2_mode3_timing.gb");
}

TEST_CASE("mooneye.acceptance.ppu.intr_2_oam_ok_timing")
{
    mooneye::run_test("intr_2_oam_ok_timing.gb");
}

TEST_CASE("mooneye.acceptance.ppu.lcdon_timing")
{
    mooneye::run_test("lcdon_timing-GS.gb");
}

TEST_CASE("mooneye.acceptance.ppu.lcdon_write_timing")
{
    mooneye::run_test("lcdon_write_timing-GS.gb");
}

TEST_CASE("mooneye.acceptance.ppu.stat_irq_blocking")
{
    mooneye::run_test("stat_irq_blocking.gb");
}

TEST_CASE("mooneye.acceptance.ppu.stat_lyc_onoff")
{
    mooneye::run_test("stat_lyc_onoff.gb");
}

TEST_CASE("mooneye.acceptance.ppu.vblank_stat_intr")
{
    mooneye::run_test("vblank_stat_intr-GS.gb");
}

TEST_CASE("mooneye.acceptance.timer.div_write")
{
    mooneye::run_test("div_write.gb");
}

TEST_CASE("mooneye.acceptance.timer.rapid_toggle")
{
    mooneye::run_test("rapid_toggle.gb");
}

TEST_CASE("mooneye.acceptance.timer.tim00")
{
    mooneye::run_test("tim00.gb");
}

TEST_CASE("mooneye.acceptance.timer.tim00_div_trigger")
{
    mooneye::run_test("tim00_div_trigger.gb");
}

TEST_CASE("mooneye.acceptance.timer.tim01")
{
    mooneye::run_test("tim01.gb");
}

TEST_CASE("mooneye.acceptance.timer.tim01_div_trigger")
{
    mooneye::run_test("tim01_div_trigger.gb");
}

TEST_CASE("mooneye.acceptance.timer.tim10")
{
    mooneye::run_test("tim10.gb");
}

TEST_CASE("mooneye.acceptance.timer.tim10_div_trigger")
{
    mooneye::run_test("tim10_div_trigger.gb");
}

TEST_CASE("mooneye.acceptance.timer.tim11")
{
    mooneye::run_test("tim11.gb");
}

TEST_CASE("mooneye.acceptance.timer.tim11_div_trigger")
{
    mooneye::run_test("tim11_div_trigger.gb");
}

TEST_CASE("mooneye.acceptance.timer.tima_reload")
{
    mooneye::run_test("tima_reload.gb");
}

TEST_CASE("mooneye.acceptance.timer.tima_write_reloading")
{
    mooneye::run_test("tima_write_reloading.gb");
}

TEST_CASE("mooneye.acceptance.timer.tma_write_reloading")
{
    mooneye::run_test("tma_write_reloading.gb");
}

TEST_CASE("mooneye.acceptance.add_sp_e_timing")
{
    mooneye::run_test("add_sp_e_timing.gb");
}

TEST_CASE("mooneye.acceptance.boot_div-dmg0")
{
    mooneye::run_test("boot_div-dmg0.gb");
}

TEST_CASE("mooneye.acceptance.boot_div-dmgABCmgb")
{
    mooneye::run_test("boot_div-dmgABCmgb.gb");
}

TEST_CASE("mooneye.acceptance.boot_hwio-dmg0")
{
    mooneye::run_test("boot_hwio-dmg0.gb");
}

TEST_CASE("mooneye.acceptance.boot_hwio-dmgABCmgb")
{
    mooneye::run_test("boot_hwio-dmgABCmgb.gb");
}

TEST_CASE("mooneye.acceptance.boot_regs-dmg0")
{
    mooneye::run_test("boot_regs-dmg0.gb");
}

TEST_CASE("mooneye.acceptance.boot_regs-dmgABC")
{
    mooneye::run_test("boot_regs-dmgABC.gb");
}

TEST_CASE("mooneye.acceptance.call_cc_timing")
{
    mooneye::run_test("call_cc_timing.gb");
}

TEST_CASE("mooneye.acceptance.call_cc_timing2")
{
    mooneye::run_test("call_cc_timing2.gb");
}

TEST_CASE("mooneye.acceptance.call_timing")
{
    mooneye::run_test("call_timing.gb");
}

TEST_CASE("mooneye.acceptance.call_cc_timing2")
{
    mooneye::run_test("call_timing2.gb");
}

TEST_CASE("mooneye.acceptance.di_timing-GS")
{
    mooneye::run_test("di_timing-GS.gb");
}

TEST_CASE("mooneye.acceptance.div_timing")
{
    mooneye::run_test("div_timing.gb");
}

TEST_CASE("mooneye.acceptance.ei_sequence")
{
    mooneye::run_test("ei_sequence.gb");
}

TEST_CASE("mooneye.acceptance.ei_timing")
{
    mooneye::run_test("ei_timing.gb");
}

TEST_CASE("mooneye.acceptance.halt_ime0_ei")
{
    mooneye::run_test("halt_ime0_ei.gb");
}

TEST_CASE("mooneye.acceptance.halt_ime0_nointr_timing")
{
    mooneye::run_test("halt_ime0_nointr_timing.gb");
}

TEST_CASE("mooneye.acceptance.halt_ime1_timing")
{
    mooneye::run_test("halt_ime1_timing.gb");
}

TEST_CASE("mooneye.acceptance.halt_ime1_timing2")
{
    mooneye::run_test("halt_ime1_timing2-GS.gb");
}

TEST_CASE("mooneye.acceptance.if_ie_registers")
{
    mooneye::run_test("if_ie_registers.gb");
}

TEST_CASE("mooneye.acceptance.intr_timing")
{
    mooneye::run_test("intr_timing.gb");
}

TEST_CASE("mooneye.acceptance.jp_cc_timing")
{
    mooneye::run_test("jp_cc_timing.gb");
}

TEST_CASE("mooneye.acceptance.jp_timing")
{
    mooneye::run_test("jp_timing.gb");
}

TEST_CASE("mooneye.acceptance.ld_hl_sp_e_timing")
{
    mooneye::run_test("ld_hl_sp_e_timing.gb");
}

TEST_CASE("mooneye.acceptance.oam_dma_restart")
{
    mooneye::run_test("oam_dma_restart.gb");
}

TEST_CASE("mooneye.acceptance.oam_dma_start")
{
    mooneye::run_test("oam_dma_start.gb");
}

TEST_CASE("mooneye.acceptance.oam_dma_timing")
{
    mooneye::run_test("oam_dma_timing.gb");
}

TEST_CASE("mooneye.acceptance.pop_timing")
{
    mooneye::run_test("pop_timing.gb");
}

TEST_CASE("mooneye.acceptance.push_timing")
{
    mooneye::run_test("push_timing.gb");
}

TEST_CASE("mooneye.acceptance.rapid_di_ei")
{
    mooneye::run_test("rapid_di_ei.gb");
}

TEST_CASE("mooneye.acceptance.ret_cc_timing")
{
    mooneye::run_test("ret_cc_timing.gb");
}

TEST_CASE("mooneye.acceptance.ret_timing")
{
    mooneye::run_test("ret_timing.gb");
}

TEST_CASE("mooneye.acceptance.reti_intr_timing")
{
    mooneye::run_test("reti_intr_timing.gb");
}

TEST_CASE("mooneye.acceptance.reti_timing")
{
    mooneye::run_test("reti_timing.gb");
}

TEST_CASE("mooneye.acceptance.rst_timing")
{
    mooneye::run_test("rst_timing.gb");
}
