


#ifndef INCLUDED_FRAMEWORK_PNG_FILE_FORMAT
#define INCLUDED_FRAMEWORK_PNG_FILE_FORMAT

#pragma once

#include <cstdint>

#include "image.h"

namespace PGA
{
  namespace PNG
  {
	image<std::uint32_t> loadImage2D(const char* filename);
	void saveImage(const char* filename, const image<std::uint32_t>& surface);
  } // namespace PNG
} // namespace PGA

#endif  // INCLUDED_FRAMEWORK_PNG_FILE_FORMAT
