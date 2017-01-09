


#include "NVRTC/error.h"
#include "NVRTC/program.h"


namespace NVRTC
{
	unique_program createProgram(const char* source, const char* name, const char** headers, int num_headers, const char** include_names)
	{
		nvrtcProgram prog;
		checkError(nvrtcCreateProgram(&prog, source, name, num_headers, headers, include_names));
		return unique_program(prog);
	}

	std::string getProgramLog(nvrtcProgram prog)
	{
		std::size_t log_size;
		checkError(nvrtcGetProgramLogSize(prog, &log_size));

		std::string log(log_size, char());
		checkError(nvrtcGetProgramLog(prog, &log[0]));
		return log;
	}

	PTXObject getPTX(nvrtcProgram prog)
	{
		std::size_t size;
		checkError(nvrtcGetPTXSize(prog, &size));

		PTXObject obj = {
			std::make_unique<char[]>(size),
			size
		};

		checkError(nvrtcGetPTX(prog, &obj.data[0]));

		return obj;
	}
}
