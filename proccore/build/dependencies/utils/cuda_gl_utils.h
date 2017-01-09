


#ifndef INCLUDED_GPUTOOLS_CUDA_GL_UTILS_H
#define INCLUDED_GPUTOOLS_CUDA_GL_UTILS_H

#pragma once

#include <cuda_gl_interop.h>

#include <algorithm>

#include <tools/utils.h>


namespace CUDA
{
  namespace GL
  {
    class Buffer
    {
    private:
      cudaGraphicsResource* res;
    public:
      Buffer(cudaGraphicsResource* resource = nullptr)
        : res(resource)
      {
      }

      Buffer(const Buffer& other) : res(other.res)
      {
        const_cast<Buffer*>(&other)->res = nullptr;
      }
      const Buffer& operator =(const Buffer& other)
      {
        res = other.res;
        const_cast<Buffer*>(&other)->res = nullptr;
        return *this;
      }

      Buffer(GLuint buffer)
      {
        CUDA_CHECKED_CALL(cudaGraphicsGLRegisterBuffer(&res, buffer, cudaGraphicsMapFlagsNone));
      }

      Buffer(Buffer&& b)
        : res(b.res)
      {
        b.res = nullptr;
      }

      Buffer& operator =(Buffer&& b)
      {
        std::swap(res, b.res);
        return *this;
      }

      ~Buffer()
      {
        if (res)
          cudaGraphicsUnregisterResource(res);
      }

      template <typename T>
      T* map()
      {
        T* ptr;
        size_t size;
        CUDA_CHECKED_CALL(cudaGraphicsMapResources(1, &res, 0));
        CUDA_CHECKED_CALL(cudaGraphicsResourceGetMappedPointer(reinterpret_cast<void**>(&ptr), &size, res));
        return ptr;
      }

      void unmap()
      {
        cudaGraphicsUnmapResources(1, &res, 0);
      }
    };

    class Image
    {
    private:
      cudaGraphicsResource* res;
    public:
      Image(cudaGraphicsResource* resource = nullptr)
        : res(resource)
      {
      }

      Image(const Image& other) : res(other.res)
      {
        const_cast<Image*>(&other)->res = nullptr;
      }
      const Image& operator =(const Image& other)
      {
        res = other.res;
        const_cast<Image*>(&other)->res = nullptr;
        return *this;
      }

      Image(GLuint texture, GLenum target = GL_TEXTURE_2D)
      {
        CUDA_CHECKED_CALL(cudaGraphicsGLRegisterImage(&res, texture, target, cudaGraphicsMapFlagsNone));
      }

      Image(Image&& b)
        : res(b.res)
      {
        b.res = nullptr;
      }

      Image& operator =(Image&& b)
      {
        std::swap(res, b.res);
        return *this;
      }

      ~Image()
      {
        if (res)
          cudaGraphicsUnregisterResource(res);
      }

      template <typename T>
      T* map()
      {
        T* ptr;
        size_t size;
        CUDA_CHECKED_CALL(cudaGraphicsMapResources(1, &res, 0));
        CUDA_CHECKED_CALL(cudaGraphicsResourceGetMappedPointer(reinterpret_cast<void**>(&ptr), &size, res));
        return ptr;
      }

      void unmap()
      {
        cudaGraphicsUnmapResources(1, &res, 0);
      }
    };

    template <typename T>
    class mapped_buffer
    {
    private:
      mapped_buffer(const mapped_buffer&);
      mapped_buffer& operator =(const mapped_buffer&);
      Buffer& buffer;
      T* ptr;
    public:
      mapped_buffer(Buffer& buffer)
        : buffer(buffer),
          ptr(buffer.map<T>())
      {
      }

      ~mapped_buffer()
      {
        buffer.unmap();
      }

      const  T*& get() const { return ptr; }
      T*& get() { return ptr; }

      operator const T*() const { return ptr; }
      operator T*() { return ptr; }
    };

    template <typename T>
    class mapped_image
    {
    private:
      mapped_image(const mapped_image&);
      mapped_image& operator =(const mapped_image&);
      Image& image;
      T* ptr;
    public:
      mapped_image(Image& image)
        : image(image),
          ptr(image.map<T>())
      {
      }

      ~mapped_image()
      {
        image.unmap();
      }

      const  T*& get() const { return ptr; }
      T*& get() { return ptr; }

      operator const T*() const { return ptr; }
      operator T*() { return ptr; }
    };
  }
}

#endif  // INCLUDED_GPUTOOLS_CUDA_GL_UTILS_H
