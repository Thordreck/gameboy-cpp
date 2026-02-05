
import std;
import cpu;

int main()
{
	cpu::registers registers{};
	registers.af() = 1;

	std::cout << std::format("CPU register size in bytes: {}", sizeof(cpu::register_8));
	std::getchar();
}
