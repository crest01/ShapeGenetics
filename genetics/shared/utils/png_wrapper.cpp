


#include <memory>
#include <string>
#include <stdexcept>
#include <cstdio>

#include <png.h>

#include "png_wrapper.h"

namespace PGA
{

namespace PNG
{
	image<std::uint32_t> loadImage2D(const char* filename)
	{
		std::FILE* fp;
		png_structp png_ptr;
		png_infop info_ptr;

		fp = std::fopen(filename, "rb");
		if (fp == nullptr)
			throw std::runtime_error(std::string("unable to open '") + filename + "'");

		png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

		if (png_ptr == nullptr)
		{
			std::fclose(fp);
			throw std::runtime_error("png_create_read_struct() failed");
		}

		info_ptr = png_create_info_struct(png_ptr);
		if (info_ptr == nullptr)
		{
			std::fclose(fp);
			png_destroy_read_struct(&png_ptr, nullptr, nullptr);
			throw std::runtime_error("png_create_info_struct() failed");
		}

		if (setjmp(png_jmpbuf(png_ptr)))
		{
			std::fclose(fp);
			png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
			throw std::runtime_error(std::string("error reading '") + filename + "'");
		}

		png_init_io(png_ptr, fp);

		png_read_info(png_ptr, info_ptr);

		png_uint_32 w, h;
		int bit_depth, color_type, interlace_method, compression_method, filter_method;
		png_get_IHDR(png_ptr, info_ptr, &w, &h, &bit_depth, &color_type, &interlace_method, &compression_method, &filter_method);

		if (bit_depth > 8)
		{
			png_set_scale_16(png_ptr);
		}
		else if (bit_depth < 8)
		{
			png_set_packing(png_ptr);
		}

		if (color_type == PNG_COLOR_TYPE_RGB)
		{
			png_set_add_alpha(png_ptr, 0xFF, PNG_FILLER_AFTER);
		}
		else if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		{
			png_set_gray_to_rgb(png_ptr);
			png_set_add_alpha(png_ptr, 0xFF, PNG_FILLER_AFTER);
		}
		else if (color_type == PNG_COLOR_TYPE_PALETTE)
		{
			png_set_palette_to_rgb(png_ptr);
			png_set_add_alpha(png_ptr, 0xFF, PNG_FILLER_AFTER);
		}
		else if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
		{
			// do nothing
		}
		else
		{
			std::fclose(fp);
			png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
			throw std::runtime_error(std::string("color type not supported: '") + filename + "'");
		}

		png_read_update_info(png_ptr, info_ptr);

		image<std::uint32_t> img(w, h);

		std::unique_ptr<png_byte*[]> rows(new png_byte*[h]);
		for (size_t y = 0; y < h; ++y)
			rows[y] = reinterpret_cast<png_byte*>(data(img) + (h - 1 - y) * w);

		png_read_image(png_ptr, &rows[0]);

		png_read_end(png_ptr, info_ptr);

		png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

		std::fclose(fp);

		return img;
	}

	void saveImage(const char* filename, const image<std::uint32_t>& img)
	{
		std::FILE* fp;
		png_structp png_ptr;
		png_infop info_ptr;

		fp = std::fopen(filename, "wb");
		if (fp == nullptr)
			throw std::runtime_error(std::string("unable to open '") + filename + "'");

		png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

		if (png_ptr == nullptr)
		{
			std::fclose(fp);
			throw std::runtime_error("png_create_write_struct() failed");
		}

		info_ptr = png_create_info_struct(png_ptr);
		if (info_ptr == nullptr)
		{
			std::fclose(fp);
			png_destroy_write_struct(&png_ptr, nullptr);
			throw std::runtime_error("png_create_info_struct() failed");
		}

		if (setjmp(png_jmpbuf(png_ptr)))
		{
			std::fclose(fp);
			png_destroy_write_struct(&png_ptr, &info_ptr);
			throw std::runtime_error(std::string("error writing '") + filename + "'");
		}

		png_init_io(png_ptr, fp);


		int w = static_cast<int>(width(img));
		int h = static_cast<int>(height(img));

		png_set_IHDR(png_ptr, info_ptr, w, h, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

		png_write_info(png_ptr, info_ptr);

		std::unique_ptr<png_byte*[]> rows(new png_byte*[h]);
		for (int y = 0; y < h; ++y)
			rows[y] = const_cast<png_byte*>(reinterpret_cast<const png_byte*>(data(img) + (h - 1 - y) * w));

		png_write_image(png_ptr, rows.get());

		png_write_end(png_ptr, info_ptr);

		png_destroy_write_struct(&png_ptr, &info_ptr);

		std::fclose(fp);
	}
} // namespace PNG
} // namespace PGA
