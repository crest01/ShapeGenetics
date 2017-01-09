


#include <sstream>
#include <iostream>

#include <NVRTC/program.h>


std::string compile(const char* source, int cc_major, int cc_minor)
{
	std::ostringstream arch_builder;
	arch_builder << "-arch=compute_" << cc_major << cc_minor;
	auto arch = arch_builder.str();

	auto program = NVRTC::createProgram(source, "@", nullptr, 0, nullptr);

	NVRTC::compileProgram(program, { "-default-device", "-std=c++11", "-lineinfo", "-dc", arch.c_str() });

	std::cout << NVRTC::getProgramLog(program) << std::endl;

	auto ptx = NVRTC::getPTX(program);
	return { &ptx.data[0], ptx.size - 1 };
}
