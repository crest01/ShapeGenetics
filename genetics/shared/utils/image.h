


#ifndef INCLUDED_FRAMEWORK_IMAGE
#define INCLUDED_FRAMEWORK_IMAGE

#pragma once

#include <memory>
#include <algorithm>


namespace PGA {

namespace PNG {




template <typename T>
class image
{
private:
	std::unique_ptr<T[]> m;

	size_t w;
	size_t h;

public:
	image(size_t width, size_t height)
		: m(new T[width * height]),
		  w(width),
		  h(height)
	{
	}

	image(const image& s)
		: m(new T[s.w * s.h]),
		  w(s.w),
		  h(s.h)
	{
		std::copy(&s.m[0], &s.m[0] + w * h, &m[0]);
	}

	image(image&& s)
		: m(move(s.m)),
		  w(s.w),
		  h(s.h)
	{
	}

	image& operator =(const image& s)
	{
		w = s.w;
		h = s.h;
		std::unique_ptr<T[]> buffer(new T[w * h]);
		std::copy(&s.m[0], &s.m[0] + w * h, &buffer[0]);
		m = move(buffer);
		return *this;
	}

	image& operator =(image&& s)
	{
		w = s.w;
		h = s.h;
		m = move(s.data);
		return *this;
	}

	T operator ()(size_t x, size_t y) const { return m[y * w + x]; }
	T& operator ()(size_t x, size_t y) { return m[y * w + x]; }

	size_t getWidth()
	{
		return w;
	}
	size_t getHeight()
	{
		return h;
	}

	T* getData()
	{
		return &m[0];
	}

	friend size_t width(const image& img)
	{
		return img.w;
	}

	friend size_t height(const image& img)
	{
		return img.h;
	}

	friend const T* data(const image& img)
	{
		return &img.m[0];
	}

	friend T* data(image& img)
	{
		return &img.m[0];
	}
};

}  // namespace PNG
}  // namespace PGA
#endif  // INCLUDED_FRAMEWORK_IMAGE

