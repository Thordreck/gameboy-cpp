
import cpu;
import sdl;
import timer;
import memory;
import graphics;
import utilities;
import emulator;

namespace
{
    std::vector<std::uint8_t> read_rom(const std::filesystem::path& filepath)
    {
        std::ifstream file(filepath, std::ios::in | std::ios::binary);
        return { std::istreambuf_iterator(file), std::istreambuf_iterator<char>() };
    }
}

// TODO: command line arguments parser interface
int main(int argc, char* argv[])
{
    using namespace sdl;
    using namespace utils;
    using namespace std::chrono_literals;

    // SDL initialization
    std::cout << std::format("Using sdl version {}", version::get()) << std::endl;

    const auto session = value_or_panic(session::create(init_flags::video));
    auto window = value_or_panic(window::create("Gameboy-cpp", graphics::lcd_width, graphics::lcd_height, window_flags::resizable));

    std::ranges::for_each(render_driver::get_available(), [](const auto& driver)
    {
        std::cout << std::format("Available render driver: {}", driver.name()) << std::endl;
    });

    auto renderer = value_or_panic(renderer::create(window));
    std::cout << std::format("Selected driver: {}", value_or_panic(renderer.driver()).name()) << std::endl;

    auto texture = value_or_panic(texture::create(renderer, pixel_format::rgb_24, texture_access::streaming, graphics::lcd_width, graphics::lcd_height));

    // cpu
    cpu::cpu cpu{ };
    cpu.pc() = 0x100;
    cpu.sp() = 0xFFFE;

    // timer
    timer::timer_system timers{};
    timers.divider() = 0xAB;

    // interrupts
    interrupts::interrupt_registers interrupts{};

    // graphics
    std::array<memory::memory_data_t, graphics::vram_size> vram {};
    std::array<memory::memory_data_t, graphics::lcd_memory_size> framebuffer {};

    graphics::raw_memory_lcd lcd_imp { framebuffer };
    graphics::lcd lcd(lcd_imp);

    graphics::oam_dma oam_dma {};
    graphics::pixel_processing_unit ppu(lcd);

    // memory
    emulator::vram_memory_page vram_page { vram };
    emulator::io_hram_interrupt_memory_page io_hram_interrupt_page{ timers, interrupts, ppu };
    emulator::oam_memory_page oam_memory_page { oam_dma };

    std::array<memory::memory_data_t, 0x4000> rom_bank_0{};
    std::array<memory::memory_data_t, 0x4000> rom_bank_n{};
    std::array<memory::memory_data_t, 0x2000> external_ram{};
    std::array<memory::memory_data_t, 0x1000> work_ram_0{};
    std::array<memory::memory_data_t, 0x1000> work_ram_1{};
    std::array<memory::memory_data_t, 0x1E00> echo_ram{};

    if (argc <= 1)
    {
        panic("Missing rom file argument");
    }

    const std::filesystem::path rom_file{ argv[1] };

    if (!std::filesystem::exists(rom_file))
    {
        panic("Cannot read rom file at {}", rom_file.string());
    }

    const std::vector<std::uint8_t> rom_data = read_rom(rom_file);
    std::ranges::copy(rom_data | std::views::take(rom_bank_0.size()), rom_bank_0.begin());
    std::ranges::copy(rom_data | std::views::drop(rom_bank_0.size()), rom_bank_n.begin());

    auto rom_bank_0_page = memory::map(rom_bank_0);
    auto rom_bank_n_page = memory::map<0x4000, 0x4000, 0x7FFF>(rom_bank_n);
    auto external_ram_page = memory::map<0x2000, 0xA000, 0xBFFF>(external_ram);
    auto work_ram_0_page = memory::map<0x1000, 0xC000, 0xCFFF>(work_ram_0);
    auto work_ram_1_page = memory::map<0x1000, 0xD000, 0xDFFF>(work_ram_1);
    auto echo_ram_page = memory::map<0x1E00, 0xE000, 0xFDFF>(echo_ram);

    auto memory_map = memory::build_memory_map(
        rom_bank_0_page,
        rom_bank_n_page,
        vram_page,
        external_ram_page,
        work_ram_0_page,
        work_ram_1_page,
        echo_ram_page,
        oam_memory_page,
        io_hram_interrupt_page);

    graphics::vram_access_policy vram_policy { ppu };
    graphics::oam_dma_access_policy oam_dma_policy { oam_dma };
    graphics::oam_ppu_access_policy oam_ppu_policy { ppu, oam_dma };

    memory::memory_bus raw_memory_bus { memory_map };
    memory::memory_bus cpu_memory_bus { memory_map, vram_policy, oam_dma_policy, oam_ppu_policy };
    memory::memory_bus ppu_memory_bus { memory_map, oam_ppu_policy };

    memory::connect(cpu_memory_bus, cpu, timers);
    memory::connect(ppu_memory_bus, ppu);
    memory::connect(raw_memory_bus, oam_dma);

    // cpu runner
    emulator::default_instructions_provider instructions{};
    emulator::default_interrupt_handler interrupts_handler{};
    emulator::cpu_runner runner{ cpu, instructions, interrupts_handler };

    // Main loop
    // TODO: main clock
    while (true)
    {
        // TODO: other events
        const auto event = poll_event();
        const bool should_quit = event
            .transform([] (const auto& e) { return std::holds_alternative<quit_event>(e); })
            .value_or(false);

        if (should_quit)
        {
            break;
        }

        runner.tick();
        timers.tick();
        ppu.tick();
        oam_dma.tick();

        // TODO: proper way to detect whe frame should be drawn
        if (ppu.scanline() == 144)
        {
            auto [lock, surface] = value_or_panic(lock_to_surface(texture));
            std::ranges::copy(framebuffer, static_cast<std::uint8_t*>(surface.pixels()));

            panic_on_error(renderer.clear());
            panic_on_error(render(renderer, texture));
            panic_on_error(renderer.present());
        }
    }

    return 0;
}