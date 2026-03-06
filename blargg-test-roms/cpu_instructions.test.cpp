
import blargg;
#include "doctest.h"

TEST_CASE("blargg.cpu_instrs.01-special")
{
	blargg::run_test("01-special.gb", "01-special\n\n\nPassed\n", 30e5);
}

TEST_CASE("blargg.cpu_instrs.02-interrupts")
{
	blargg::run_test("02-interrupts.gb", "02-interrupts\n\n\nPassed\n", 20e6);
}

TEST_CASE("blargg.cpu_instrs.03-op sp,hl")
{
	blargg::run_test("03-op sp,hl.gb", "03-op sp,hl\n\n\nPassed\n", 30e5);
}

TEST_CASE("blargg.cpu_instrs.04-op r,imm")
{
	blargg::run_test("04-op r,imm.gb", "04-op r,imm\n\n\nPassed\n", 30e5);
}

TEST_CASE("blargg.cpu_instrs.05-op rp")
{
	blargg::run_test("05-op rp.gb", "05-op rp\n\n\nPassed\n", 60e5);
}

TEST_CASE("blargg.cpu_instrs.06-ld r,r")
{
	blargg::run_test("06-ld r,r.gb", "06-ld r,r\n\n\nPassed\n", 30e5);
}

TEST_CASE("blargg.cpu_instrs.07-jr,jp,call,ret,rst")
{
	blargg::run_test("07-jr,jp,call,ret,rst.gb", "07-jr,jp,call,ret,rst\n\n\nPassed\n", 60e5);
}

TEST_CASE("blargg.cpu_instrs.08-misc instrs")
{
	blargg::run_test("08-misc instrs.gb", "08-misc instrs\n\n\nPassed\n", 30e5);
}

TEST_CASE("blargg.cpu_instrs.09-op r,r")
{
	blargg::run_test("09-op r,r.gb", "09-op r,r\n\n\nPassed\n", 30e6);
}

TEST_CASE("blargg.cpu_instrs.10-bit ops")
{
	blargg::run_test("10-bit ops.gb", "10-bit ops\n\n\nPassed\n",  30e6);
}

TEST_CASE("blargg.cpu_instrs.11-op a,(hl)")
{
	blargg::run_test("11-op a,(hl).gb", "11-op a,(hl)\n\n\nPassed\n", 30e6);
}
