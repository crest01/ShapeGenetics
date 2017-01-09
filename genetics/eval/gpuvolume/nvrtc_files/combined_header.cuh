
#ifndef INCLUDED_MATH
#define INCLUDED_MATH

#define MATH_FUNCTION __host__ __device__


namespace math
{

	template <typename T>
	struct constants;

	template <>
	struct constants<float>
	{
		MATH_FUNCTION static float one() { return 1.0f; }
		MATH_FUNCTION static float zero() { return 0.0f; }
		MATH_FUNCTION static float pi() { return 3.1415926535897932384626434f; }
		MATH_FUNCTION static float e() { return 2.7182818284590452353602875f; }
		MATH_FUNCTION static float sqrtHalf() { return 0.70710678118654752440084436210485f; }
		MATH_FUNCTION static float sqrtTwo() { return 1.4142135623730950488016887242097f; }
		MATH_FUNCTION static float epsilon() { return 0.00000001f; }
	};

	template <>
	struct constants<double>
	{
		MATH_FUNCTION static double one() { return 1.0; }
		MATH_FUNCTION static double zero() { return 0.0; }
		MATH_FUNCTION static double pi() { return 3.1415926535897932384626434; }
		MATH_FUNCTION static double e() { return 2.7182818284590452353602875; }
		MATH_FUNCTION static double sqrtHalf() { return 0.70710678118654752440084436210485; }
		MATH_FUNCTION static double sqrtTwo() { return 1.4142135623730950488016887242097; }
		MATH_FUNCTION static double epsilon() { return 0.00000000001; }
	};

	template <>
	struct constants<long double>
	{
		MATH_FUNCTION static long double one() { return 1.0l; }
		MATH_FUNCTION static long double zero() { return 0.0l; }
		MATH_FUNCTION static long double pi() { return 3.1415926535897932384626434l; }
		MATH_FUNCTION static long double e() { return 2.7182818284590452353602875l; }
		MATH_FUNCTION static long double sqrtHalf() { return 0.70710678118654752440084436210485l; }
		MATH_FUNCTION static long double sqrtTwo() { return 1.4142135623730950488016887242097l; }
		MATH_FUNCTION static long double epsilon() { return 0.0000000000001l; }
	};


#ifdef ALREADY_DEFINED
	MATH_FUNCTION inline float min(float a, float b)
	{
		return fminf(a, b);
	}
	MATH_FUNCTION inline float max(float a, float b)
	{
		return fmaxf(a, b);
	}
	MATH_FUNCTION inline float abs(float a)
	{
		return fabsf(a);
	}

	MATH_FUNCTION inline float exp(float a)
	{
		return expf(a);
	}
	MATH_FUNCTION inline float frexp(float a, int* b)
	{
		return frexpf(a, b);
	}
	MATH_FUNCTION inline float ldexp(float a, int b)
	{
		return ldexpf(a, b);
	}
	MATH_FUNCTION inline float log(float a)
	{
		return logf(a);
	}
	MATH_FUNCTION inline float log10(float a)
	{
		return log10f(a);
	}
	MATH_FUNCTION inline float modf(float a, float* b)
	{
		return modff(a, b);
	}

	MATH_FUNCTION inline float cos(float a)
	{
		return cosf(a);
	}
	MATH_FUNCTION inline float sin(float a)
	{
		return sinf(a);
	}
	MATH_FUNCTION inline float tan(float a)
	{
		return tanf(a);
	}
	MATH_FUNCTION inline float acos(float a)
	{
		return acosf(a);
	}
	MATH_FUNCTION inline float asin(float a)
	{
		return asinf(a);
	}
	MATH_FUNCTION inline float atan(float a)
	{
		return atanf(a);
	}
	MATH_FUNCTION inline float atan2(float a)
	{
		return expf(a);
	}
	MATH_FUNCTION inline float cosh(float a)
	{
		return coshf(a);
	}
	MATH_FUNCTION inline float sinh(float a)
	{
		return sinhf(a);
	}
	MATH_FUNCTION inline float tanh(float a)
	{
		return expf(a);
	}

	MATH_FUNCTION inline float pow(float a, float b)
	{
		return powf(a, b);
	}
	MATH_FUNCTION inline float sqrt(float a)
	{
		return sqrtf(a);
	}

	MATH_FUNCTION inline float floor(float a)
	{
		return floorf(a);
	}
	MATH_FUNCTION inline float ceil(float a)
	{
		return ceilf(a);
	}

	MATH_FUNCTION inline float fmod(float a, float b)
	{
		return fmodf(a, b);
	}

	template <typename T>
	MATH_FUNCTION inline T clamp(T v, T min = constants<T>::zero(), T max = constants<T>::one())
	{
		return static_cast<T>(min(max(v, min), max));
	}

	MATH_FUNCTION inline float saturate(float v)
	{
		return clamp(v, 0.0f, 1.0f);
	}

	MATH_FUNCTION inline double saturate(double v)
	{
		return clamp(v, 0.0, 1.0);
	}

	MATH_FUNCTION inline long double saturate(long double v)
	{
		return clamp(v, 0.0l, 1.0l);
	}

	MATH_FUNCTION inline float rcp(float v)
	{
		return 1.0f / v;
	}

	MATH_FUNCTION inline double rcp(double v)
	{
		return 1.0 / v;
	}

	MATH_FUNCTION inline long double rcp(long double v)
	{
		return 1.0l / v;
	}

	MATH_FUNCTION inline float frac(float v)
	{
		return v - floor(v);
	}

	MATH_FUNCTION inline double frac(double v)
	{
		return v - floor(v);
	}

	MATH_FUNCTION inline long double frac(long double v)
	{
		return v - floor(v);
	}

#endif /* ALREADY_DEFINED */

	MATH_FUNCTION inline float half(float v)
	{
		return v * 0.5f;
	}

	MATH_FUNCTION inline double half(double v)
	{
		return v * 0.5;
	}

	MATH_FUNCTION inline long double half(long double v)
	{
		return v * 0.5l;
	}

	MATH_FUNCTION inline float lerp(float a, float b, float t)
	{
		return (1.0f - t) * a + t * b;
	}

	MATH_FUNCTION inline double lerp(double a, double b, double t)
	{
		return (1.0 - t) * a + t * b;
	}

	MATH_FUNCTION inline long double lerp(long double a, long double b, long double t)
	{
		return (1.0l - t) * a + t * b;
	}

	MATH_FUNCTION inline float smoothstep(float t)
	{
		return t * t * (3.0f - 2.0f * t);
	}

	MATH_FUNCTION inline double smoothstep(double t)
	{
		return t * t * (3.0 - 2.0 * t);
	}

	MATH_FUNCTION inline long double smoothstep(long double t)
	{
		return t * t * (3.0l - 2.0l * t);
	}

	MATH_FUNCTION inline float smootherstep(float t)
	{
		return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
	}

	MATH_FUNCTION inline double smootherstep(double t)
	{
		return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
	}

	MATH_FUNCTION inline long double smootherstep(long double t)
	{
		return t * t * t * (t * (t * 6.0l - 15.0l) + 10.0l);
	}

}

#endif /* INCLUDED_MATH */

#ifndef INCLUDED_MATH_VECTOR
#define INCLUDED_MATH_VECTOR

namespace math
{
	template <typename T, unsigned int D>
	class vector;

	template <typename T>
	class vector<T, 2U>
	{
	public:
		static const unsigned int dim = 2U;
		typedef T field_type;

		T x;
		T y;

		MATH_FUNCTION vector() = default;

		MATH_FUNCTION explicit vector(T a)
		    : x(a), y(a)
		{
		}

		MATH_FUNCTION vector(T x, T y)
		    : x(x), y(y)
		{
		}

		template <typename U>
		MATH_FUNCTION vector(const vector<U, 2U>& v)
		    : x(v.x), y(v.y)
		{
		}

		MATH_FUNCTION vector yx() const
		{
			return vector(y, x);
		}

		MATH_FUNCTION vector operator-() const
		{
			return vector(-x, -y);
		}

		MATH_FUNCTION vector& operator+=(const vector& v)
		{
			x += v.x;
			y += v.y;
			return *this;
		}

		MATH_FUNCTION vector& operator-=(const vector& v)
		{
			x -= v.x;
			y -= v.y;
			return *this;
		}

		MATH_FUNCTION vector& operator-=(T a)
		{
			x -= a;
			y -= a;
			return *this;
		}

		MATH_FUNCTION vector& operator*=(T a)
		{
			x *= a;
			y *= a;
			return *this;
		}

		MATH_FUNCTION vector& operator*=(const vector& v)
		{
			x *= v.x;
			y *= v.y;
			return *this;
		}

		MATH_FUNCTION vector& operator/=(T a)
		{
			x /= a;
			y /= a;
			return *this;
		}

		MATH_FUNCTION friend const vector operator+(const vector& a, const vector& b)
		{
			return vector(a.x + b.x, a.y + b.y);
		}

		MATH_FUNCTION friend const vector operator+(const vector& a, T b)
		{
			return vector(a.x + b, a.y + b);
		}

		MATH_FUNCTION friend const vector operator+(T a, const vector& b)
		{
			return vector(a + b.x, a + b.y);
		}

		MATH_FUNCTION friend const vector operator-(const vector& a, const vector& b)
		{
			return vector(a.x - b.x, a.y - b.y);
		}

		MATH_FUNCTION friend const vector operator-(const vector& a, T b)
		{
			return vector(a.x - b, a.y - b);
		}
		MATH_FUNCTION friend const vector operator-(T b, const vector& a)
		{
			return vector(b - a.x, b - a.y);
		}

		MATH_FUNCTION friend const vector operator*(T a, const vector& v)
		{
			return vector(a * v.x, a * v.y);
		}

		MATH_FUNCTION friend const vector operator*(const vector& v, T a)
		{
			return a * v;
		}

		MATH_FUNCTION friend const vector operator*(const vector& a, const vector& b)
		{
			return vector(a.x * b.x, a.y * b.y);
		}

		MATH_FUNCTION friend const vector operator/(const vector& v, T a)
		{
			return vector(v.x / a, v.y / a);
		}

		MATH_FUNCTION friend T dot(const vector& a, const vector& b)
		{
			return a.x * b.x + a.y * b.y;
		}

		MATH_FUNCTION friend vector abs(const vector& v)
		{
			return vector(abs(v.x), abs(v.y));
		}

		MATH_FUNCTION friend vector floor(const vector& v)
		{
			return vector(floor(v.x), floor(v.y));
		}

		MATH_FUNCTION friend vector ceil(const vector& v)
		{
			return vector(ceil(v.x), ceil(v.y));
		}

		MATH_FUNCTION friend vector max(const vector& v, T c)
		{
			return vector(max(v.x, c), max(v.y, c));
		}
		MATH_FUNCTION friend vector max(T c, const vector& v)
		{
			return vector(max(v.x, c), max(v.y, c));
		}
		MATH_FUNCTION friend vector min(const vector& v, T c)
		{
			return vector(min(v.x, c), min(v.y, c));
		}
		MATH_FUNCTION friend vector min(T c, const vector& v)
		{
			return vector(min(v.x, c), min(v.y, c));
		}

		MATH_FUNCTION friend vector min(const vector& a, const vector& b)
		{
			return vector(min(a.x, b.x), min(a.y, b.y));
		}
		MATH_FUNCTION friend vector max(const vector& a, const vector& b)
		{
			return vector(max(a.x, b.x), max(a.y, b.y));
		}
		MATH_FUNCTION friend T min(const vector& a)
		{
			return min(a.x, a.y);
		}
		MATH_FUNCTION friend T max(const vector& a)
		{
			return max(a.x, a.y);
		}

		MATH_FUNCTION friend T length(const vector& v)
		{
			return sqrt(dot(v, v));
		}

		MATH_FUNCTION friend vector normalize(const vector& v)
		{
			return v * rcp(length(v));
		}

		MATH_FUNCTION friend vector pow(const vector& v, T exponent)
		{
			return vector(pow(v.x, exponent), pow(v.y, exponent));
		}

		MATH_FUNCTION friend vector lerp(const vector& a, const vector& b, T t)
		{
			return vector(lerp(a.x, b.x, t), lerp(a.y, b.y, t));
		}

		MATH_FUNCTION friend vector rcp(const vector& v)
		{
			return vector(rcp(v.x), rcp(v.y));
		}

		MATH_FUNCTION bool operator==(const vector& v) const
		{
			return x == v.x && y == v.y;
		}
		MATH_FUNCTION bool operator!=(const vector& v) const
		{
			return x != v.x || y == v.y;
		}
	};

	template <typename T>
	class vector<T, 3U>
	{
	public:
		static const unsigned int dim = 3U;
		typedef T field_type;

		T x;
		T y;
		T z;

		MATH_FUNCTION vector() = default;

		MATH_FUNCTION explicit vector(T a)
		    : x(a), y(a), z(a)
		{
		}

		MATH_FUNCTION vector(T x, T y, T z)
		    : x(x), y(y), z(z)
		{
		}

		template <typename U>
		MATH_FUNCTION vector(const vector<U, 3U>& v)
		    : x(v.x), y(v.y), z(v.z)
		{
		}

		template <typename U>
		MATH_FUNCTION vector(const vector<U, 2U>& v, T z)
		    : x(v.x), y(v.y), z(z)
		{
		}

		template <typename U>
		MATH_FUNCTION vector(T _x, const vector<U, 2U>& v)
		    : x(_x), y(v.x), z(v.y)
		{
		}

		MATH_FUNCTION vector<T, 2U> xy() const
		{
			return vector<T, 2U>(x, y);
		}
		MATH_FUNCTION vector<T, 2U> yx() const
		{
			return vector<T, 2U>(y, x);
		}
		MATH_FUNCTION vector<T, 2U> xz() const
		{
			return vector<T, 2U>(x, z);
		}
		MATH_FUNCTION vector<T, 2U> zx() const
		{
			return vector<T, 2U>(z, x);
		}
		MATH_FUNCTION vector<T, 2U> yz() const
		{
			return vector<T, 2U>(y, z);
		}
		MATH_FUNCTION vector<T, 2U> zy() const
		{
			return vector<T, 2U>(z, y);
		}
		MATH_FUNCTION vector xzy() const
		{
			return vector(x, y, x);
		}
		MATH_FUNCTION vector yxz() const
		{
			return vector(y, x, z);
		}
		MATH_FUNCTION vector yzx() const
		{
			return vector(y, z, z);
		}
		MATH_FUNCTION vector zxy() const
		{
			return vector(z, x, y);
		}
		MATH_FUNCTION vector zyx() const
		{
			return vector(z, y, x);
		}

		MATH_FUNCTION vector operator-() const
		{
			return vector(-x, -y, -z);
		}

		MATH_FUNCTION vector& operator+=(const vector& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}

		MATH_FUNCTION vector& operator-=(const vector& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}

		MATH_FUNCTION vector& operator-=(T a)
		{
			x -= a;
			y -= a;
			z -= a;
			return *this;
		}

		MATH_FUNCTION vector& operator*=(T a)
		{
			x *= a;
			y *= a;
			z *= a;
			return *this;
		}

		MATH_FUNCTION vector& operator/=(T a)
		{
			x /= a;
			y /= a;
			z /= a;
			return *this;
		}

		MATH_FUNCTION vector& operator*=(const vector& v)
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;
			return *this;
		}

		MATH_FUNCTION T& operator[](size_t pos)
		{
			return (*(&(this->x) + pos));
		}

		MATH_FUNCTION T operator[](size_t pos) const
		{
			return (*(&(this->x) + pos));
		}

		MATH_FUNCTION friend  const vector operator+(const vector& a, const vector& b)
		{
			return vector(a.x + b.x, a.y + b.y, a.z + b.z);
		}

		MATH_FUNCTION friend const vector operator+(const vector& a, T b)
		{
			return vector(a.x + b, a.y + b, a.z + b);
		}

		MATH_FUNCTION friend const vector operator+(T a, const vector& b)
		{
			return vector(a + b.x, a + b.y, a + b.z);
		}

		MATH_FUNCTION friend const vector operator-(const vector& a, const vector& b)
		{
			return vector(a.x - b.x, a.y - b.y, a.z - b.z);
		}

		MATH_FUNCTION friend const vector operator-(const vector& a, T b)
		{
			return vector(a.x - b, a.y - b, a.z - b);
		}

		MATH_FUNCTION friend const vector operator-(T b, const vector& a)
		{
			return vector(b - a.x, b - a.y, b - a.z);
		}

		MATH_FUNCTION friend const vector operator*(T a, const vector& v)
		{
			return vector(a * v.x, a * v.y, a * v.z);
		}

		MATH_FUNCTION friend const vector operator/(const vector& v, T a)
		{
			return vector(v.x / a, v.y / a, v.z / a);
		}

		MATH_FUNCTION friend const vector operator*(const vector& v, T a)
		{
			return a * v;
		}

		MATH_FUNCTION friend const vector operator*(const vector& a, const vector& b)
		{
			return vector(a.x * b.x, a.y * b.y, a.z * b.z);
		}

		MATH_FUNCTION friend T dot(const vector& a, const vector& b)
		{
			return a.x * b.x + a.y * b.y + a.z * b.z;
		}

		MATH_FUNCTION friend vector cross(const vector& a, const vector& b)
		{
			return vector(a.y * b.z - a.z * b.y,
			              a.z * b.x - a.x * b.z,
			              a.x * b.y - a.y * b.x);
		}

		MATH_FUNCTION friend vector abs(const vector& v)
		{
			return vector(abs(v.x), abs(v.y), abs(v.z));
		}

		MATH_FUNCTION friend vector floor(const vector& v)
		{
			return vector(floor(v.x), floor(v.y));
		}

		MATH_FUNCTION friend vector ceil(const vector& v)
		{
			return vector(ceil(v.x), ceil(v.y));
		}

		MATH_FUNCTION friend vector max(const vector& v, T c)
		{
			return vector(max(v.x, c), max(v.y, c), max(v.z, c));
		}
		MATH_FUNCTION friend vector max(T c, const vector& v)
		{
			return vector(max(v.x, c), max(v.y, c), max(v.z, c));
		}
		MATH_FUNCTION friend vector min(const vector& v, T c)
		{
			return vector(min(v.x, c), min(v.y, c), min(v.z, c));
		}
		MATH_FUNCTION friend vector min(T c, const vector& v)
		{
			return vector(min(v.x, c), min(v.y, c), min(v.z, c));
		}

		MATH_FUNCTION friend vector min(const vector& a, const vector& b)
		{
			return vector(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
		}
		MATH_FUNCTION friend vector max(const vector& a, const vector& b)
		{
			return vector(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
		}
		MATH_FUNCTION friend T min(const vector& a)
		{
			return min(min(a.x, a.y), a.z);
		}
		MATH_FUNCTION friend T max(const vector& a)
		{
			return max(max(a.x, a.y), a.z);
		}

		MATH_FUNCTION friend T length(const vector& v)
		{
			return sqrt(dot(v, v));
		}

		MATH_FUNCTION friend vector normalize(const vector& v)
		{
			return v * rcp(length(v));
		}

		MATH_FUNCTION friend vector pow(const vector& v, T exponent)
		{
			return vector(pow(v.x, exponent), pow(v.y, exponent), pow(v.z, exponent));
		}

		MATH_FUNCTION friend vector lerp(const vector& a, const vector& b, T t)
		{
			return vector(lerp(a.x, b.x, t), lerp(a.y, b.y, t), lerp(a.z, b.z, t));
		}

		MATH_FUNCTION friend vector rcp(const vector& v)
		{
			return vector(rcp(v.x), rcp(v.y), rcp(v.z));
		}

		MATH_FUNCTION bool operator==(const vector& v) const
		{
			return x == v.x && y == v.y && z == v.z;
		}
		MATH_FUNCTION bool operator!=(const vector& v) const
		{
			return x != v.x || y == v.y || z == v.z;
		}
	};

	template <typename T>
	class vector<T, 4U>
	{
	public:
		static const unsigned int dim = 4U;
		typedef T field_type;

		T x;
		T y;
		T z;
		T w;

		MATH_FUNCTION vector() = default;

		MATH_FUNCTION explicit vector(T a)
		    : x(a), y(a), z(a), w(a)
		{
		}

		MATH_FUNCTION vector(T x, T y, T z, T w)
		    : x(x), y(y), z(z), w(w)
		{
		}

		template <typename U>
		MATH_FUNCTION vector(const vector<U, 4>& v)
		    : x(v.x), y(v.y), z(v.z), w(v.w)
		{
		}

		template <typename U>
		MATH_FUNCTION vector(const vector<U, 3>& v, T w)
		    : x(v.x), y(v.y), z(v.z), w(w)
		{
		}

		template <typename U>
		MATH_FUNCTION vector(T x, const vector<U, 3>& v)
		    : x(x), y(v.x), z(v.y), w(v.z)
		{
		}

		template <typename U>
		MATH_FUNCTION vector(T x, T y, const vector<U, 2>& v)
		    : x(x), y(y), z(v.x), w(v.y)
		{
		}

		template <typename U>
		MATH_FUNCTION vector(const vector<U, 2>& v2, T z, T w)
		    : x(v2.x), y(v2.y), z(z), w(w)
		{
		}

		template <typename U>
		MATH_FUNCTION vector(const vector<U, 2>& v1, const vector<U, 2>& v2)
		    : x(v1.x), y(v1.y), z(v2.x), w(v2.y)
		{
		}

		MATH_FUNCTION vector<T, 3U> xyz() const
		{
			return vector<T, 3U>(x, y, z);
		}
		MATH_FUNCTION vector<T, 3U> xyw() const
		{
			return vector<T, 3U>(x, y, w);
		}
		MATH_FUNCTION vector<T, 2U> xy() const
		{
			return vector<T, 2U>(x, y);
		}

		MATH_FUNCTION vector operator-() const
		{
			return vector(-x, -y, -z, -w);
		}

		MATH_FUNCTION vector& operator+=(const vector& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;
			return *this;
		}

		MATH_FUNCTION vector& operator-=(const vector& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;
			return *this;
		}

		MATH_FUNCTION vector& operator-=(T a)
		{
			x -= a;
			y -= a;
			z -= a;
			w -= a;
			return *this;
		}

		MATH_FUNCTION vector& operator*=(T a)
		{
			x *= a;
			y *= a;
			z *= a;
			w *= a;
			return *this;
		}

		MATH_FUNCTION vector& operator*=(const vector& v)
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;
			w *= v.w;
			return *this;
		}

		MATH_FUNCTION vector& operator/=(T a)
		{
			x /= a;
			y /= a;
			z /= a;
			w /= a;
			return *this;
		}

		MATH_FUNCTION friend const vector operator+(const vector& a, const vector& b)
		{
			return vector(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
		}

		MATH_FUNCTION friend const vector operator+(const vector& a, T b)
		{
			return vector(a.x + b, a.y + b, a.z + b, a.w + b);
		}

		MATH_FUNCTION friend const vector operator+(T a, const vector& b)
		{
			return vector(a + b.x, a + b.y, a + b.z, a + b.w);
		}

		MATH_FUNCTION friend const vector operator-(const vector& a, const vector& b)
		{
			return vector(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
		}

		MATH_FUNCTION friend const vector operator-(const vector& a, T b)
		{
			return vector(a.x - b, a.y - b, a.z - b, a.w - b);
		}

		MATH_FUNCTION friend const vector operator-(T b, const vector& a)
		{
			return vector(b - a.x, b - a.y, b - a.z, b - a.w);
		}

		MATH_FUNCTION friend const vector operator*(T a, const vector& v)
		{
			return vector(a * v.x, a * v.y, a * v.z, a * v.w);
		}

		MATH_FUNCTION friend const vector operator/(const vector& v, T a)
		{
			return vector(v.x / a, v.y / a, v.z / a, v.w / a);
		}

		MATH_FUNCTION friend const vector operator*(const vector& v, T a)
		{
			return a * v;
		}

		MATH_FUNCTION friend const vector operator*(const vector& a, const vector& b)
		{
			return vector(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
		}

		MATH_FUNCTION friend T dot(const vector& a, const vector& b)
		{
			return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
		}

		MATH_FUNCTION friend vector abs(const vector& v)
		{
			return vector(abs(v.x), abs(v.y), abs(v.z), abs(v.w));
		}

		MATH_FUNCTION friend vector floor(const vector& v)
		{
			return vector(floor(v.x), floor(v.y), floor(v.z), floor(v.w));
		}

		MATH_FUNCTION friend vector ceil(const vector& v)
		{
			return vector(ceil(v.x), ceil(v.y), ceil(v.z), ceil(v.w));
		}

		MATH_FUNCTION friend vector max(const vector& v, T c)
		{
			return vector(max(v.x, c), max(v.y, c), max(v.z, c), max(v.w, c));
		}
		MATH_FUNCTION friend vector max(T c, const vector& v)
		{
			return vector(max(v.x, c), max(v.y, c), max(v.z, c), max(v.w, c));
		}
		MATH_FUNCTION friend vector min(const vector& v, T c)
		{
			return vector(min(v.x, c), min(v.y, c), min(v.z, c), min(v.w, c));
		}
		MATH_FUNCTION friend vector min(T c, const vector& v)
		{
			return vector(min(v.x, c), min(v.y, c), min(v.z, c), min(v.w, c));
		}

		MATH_FUNCTION friend vector min(const vector& a, const vector& b)
		{
			return vector(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z), min(a.w, b.w));
		}
		MATH_FUNCTION friend vector max(const vector& a, const vector& b)
		{
			return vector(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z), max(a.w, b.w));
		}

		MATH_FUNCTION friend T min(const vector& a)
		{
			return min(min(a.x, a.y), min(a.z, a.w));
		}
		MATH_FUNCTION friend T max(const vector& a)
		{
			return max(max(a.x, a.y), max(a.z, a.w));
		}

		MATH_FUNCTION friend T length(const vector& v)
		{
			return sqrt(dot(v, v));
		}

		MATH_FUNCTION friend vector normalize(const vector& v)
		{
			return v * rcp(length(v));
		}

		MATH_FUNCTION friend vector pow(const vector& v, T exponent)
		{
			return vector(pow(v.x, exponent), pow(v.y, exponent), pow(v.z, exponent), pow(v.w, exponent));
		}

		MATH_FUNCTION friend vector rcp(const vector& v)
		{
			return vector(rcp(v.x), rcp(v.y), rcp(v.z), rcp(v.w));
		}

		MATH_FUNCTION friend vector lerp(const vector& a, const vector& b, T t)
		{
			return vector(lerp(a.x, b.x, t), lerp(a.y, b.y, t), lerp(a.z, b.z, t), lerp(a.w, b.w, t));
		}

		MATH_FUNCTION bool operator==(const vector& v) const
		{
			return x == v.x && y == v.y && z == v.z && w == v.w;
		}
		MATH_FUNCTION bool operator!=(const vector& v) const
		{
			return x != v.x || y == v.y || z == v.z || w == v.w;
		}
	};

	template <typename T, unsigned int D>
	MATH_FUNCTION inline vector<T, D> clamp(const vector<T, D>& v, T lower, T upper)
	{
		return min(max(v, lower), upper);
	}

	template <typename T, unsigned int D>
	MATH_FUNCTION inline T length2(const vector<T, D>& v)
	{
		return dot(v, v);
	}

	typedef vector<float, 2U> float2;
	typedef vector<float, 3U> float3;
	typedef vector<float, 4U> float4;

	typedef vector<short, 2U> short2;
	typedef vector<short, 3U> short3;
	typedef vector<short, 4U> short4;

	typedef vector<unsigned short, 2U> ushort2;
	typedef vector<unsigned short, 3U> ushort3;
	typedef vector<unsigned short, 4U> ushort4;

	typedef vector<int, 2U> int2;
	typedef vector<int, 3U> int3;
	typedef vector<int, 4U> int4;

	typedef vector<unsigned int, 2U> uint2;
	typedef vector<unsigned int, 3U> uint3;
	typedef vector<unsigned int, 4U> uint4;
}

#endif // INCLUDED_MATH_VECTOR

#ifndef INCLUDED_MATH_MATRIX
#define INCLUDED_MATH_MATRIX

namespace math
{
	template <typename T, unsigned int M, unsigned int N>
	class matrix;

	template <typename T>
	class matrix<T, 2U, 2U>;

	template <typename T>
	class matrix<T, 2U, 3U>;

	template <typename T>
	class matrix<T, 3U, 3U>;

	template <typename T>
	class matrix<T, 3U, 4U>;

	template <typename T>
	class matrix<T, 4U, 3U>;

	template <typename T>
	class matrix<T, 4U, 4U>;

	template <typename T, unsigned int D>
	class affine_matrix;

	template <typename T>
	class affine_matrix<T, 3>;

	template <typename T>
	class affine_matrix<T, 3>;


	template <typename T>
	class matrix<T, 2U, 2U>
	{
	public:
		static const unsigned int M = 2U;
		static const unsigned int N = 2U;
		typedef T field_type;

		T _11, _12;
		T _21, _22;

		MATH_FUNCTION matrix() = default;

		MATH_FUNCTION explicit matrix(T a)
		    : _11(a), _12(a), _21(a), _22(a)
		{
		}

		MATH_FUNCTION matrix(T m11, T m12, T m21, T m22)
		    : _11(m11), _12(m12), _21(m21), _22(m22)
		{
		}

		static MATH_FUNCTION matrix from_rows(const vector<T, 2U>& r1, const vector<T, 2U>& r2)
		{
			return matrix(r1.x, r1.y, r2.x, r2.y);
		}
		static MATH_FUNCTION matrix from_cols(const vector<T, 2U>& c1, const vector<T, 2U>& c2)
		{
			return matrix(c1.x, c2.x, c1.y, c2.y);
		}

		MATH_FUNCTION const vector<T, 2U> row1() const
		{
			return vector<T, 3U>(_11, _12);
		}
		MATH_FUNCTION const vector<T, 2U> row2() const
		{
			return vector<T, 3U>(_21, _22);
		}

		MATH_FUNCTION const vector<T, 2U> column1() const
		{
			return vector<T, 2U>(_11, _21);
		}
		MATH_FUNCTION const vector<T, 2U> column2() const
		{
			return vector<T, 2U>(_12, _22);
		}

		MATH_FUNCTION friend matrix transpose(const matrix& m)
		{
			return matrix(m._11, m._21, m._12, m._22);
		}

		MATH_FUNCTION static matrix scale(const vector<T, 2>& angle)
		{
			matrix M(angle.x, 0.0f, 0.0f, angle.y);
			return M;
		}

		MATH_FUNCTION friend matrix operator*(const matrix& a, const matrix& b)
		{
			return matrix(a._11 * b._11 + a._12 * b._21, a._11 * b._12 + a._12 * b._22, a._21 * b._11 + a._22 * b._21, a._21 * b._12 + a._22 * b._22);
		}

		MATH_FUNCTION friend vector<T, 2u> operator*(const matrix& a, const vector<T, 2u>& b)
		{
			return vector<T, 2u>(a._11 * b.x + a._12 * b.y,
			                     a._21 * b.x + a._22 * b.y);
		}

		MATH_FUNCTION friend matrix operator+(const matrix& a, const matrix& b)
		{
			return matrix(a._11 + b._11, a._12 + b._12, a._21 + b._21, a._22 + b._22);
		}

		MATH_FUNCTION friend matrix operator*(T f, const matrix& m)
		{
			return matrix(f * m._11, f * m._12, f * m._21, f * m._22);
		}

		MATH_FUNCTION friend matrix operator*(const matrix& m, T f)
		{
			return f * m;
		}

		MATH_FUNCTION friend T trace(const matrix& M)
		{
			return M._11 + M._22;
		}
	};



	template <typename T>
	class matrix<T, 2U, 3U>
	{
	public:
		static const unsigned int M = 2U;
		static const unsigned int N = 3U;
		typedef T field_type;

		T _11, _12, _13;
		T _21, _22, _23;

		MATH_FUNCTION matrix() = default;

		MATH_FUNCTION explicit matrix(T a)
			: _11(a), _12(a), _13(a), _21(a), _22(a), _23(a)
		{
		}

		MATH_FUNCTION matrix(T m11, T m12, T m13, T m21, T m22, T m23)
			: _11(m11), _12(m12), _13(m13), _21(m21), _22(m22), _23(m23)
		{
		}

		MATH_FUNCTION matrix(const affine_matrix<T, 2U>& M)
			: _11(M._11), _12(M._12), _13(M._13), _21(M._21), _22(M._22), _23(M._23)
		{
		}

		static MATH_FUNCTION matrix from_rows(const vector<T, 3U>& r1, const vector<T, 3U>& r2)
		{
			return matrix(r1.x, r1.y, r1.z, r2.x, r2.y, r2.z);
		}

		static MATH_FUNCTION matrix from_cols(const vector<T, 2U>& c1, const vector<T, 2U>& c2, const vector<T, 2U>& c3)
		{
			return matrix(c1.x, c2.x, c3.x, c1.y, c2.y, c3.y);
		}

		MATH_FUNCTION const vector<T, 3U> row1() const
		{
			return vector<T, 3U>(_11, _12, _13);
		}
		MATH_FUNCTION const vector<T, 3U> row2() const
		{
			return vector<T, 3U>(_21, _22, _23);
		}

		MATH_FUNCTION const vector<T, 2U> column1() const
		{
			return vector<T, 2U>(_11, _21);
		}
		MATH_FUNCTION const vector<T, 2U> column2() const
		{
			return vector<T, 2U>(_12, _22);
		}
		MATH_FUNCTION const vector<T, 2U> column3() const
		{
			return vector<T, 2U>(_13, _23);
		}

		MATH_FUNCTION friend matrix<T, 2U, 3U> transpose(const matrix& m)
		{
			return matrix<T, 2U, 3U>(m._11, m._21, m._12, m._22, m._13, m._23);
		}

		MATH_FUNCTION friend matrix operator+(const matrix& a, const matrix& b)
		{
			return matrix(a._11 + b._11, a._12 + b._12, a._13 + b._13, a._21 + b._21, a._22 + b._22, a._23 + b._23);
		}

		MATH_FUNCTION friend matrix operator*(float f, const matrix& m)
		{
			return matrix(f * m._11, f * m._12, f * m._13, f * m._21, f * m._22, f * m._23);
		}

		MATH_FUNCTION friend matrix operator*(const matrix& m, float f)
		{
			return f * m;
		}
	};


	template <typename T>
	class matrix<T, 3U, 3U>
	{
	public:
		static const unsigned int M = 3U;
		static const unsigned int N = 3U;
		typedef T field_type;

		T _11, _12, _13;
		T _21, _22, _23;
		T _31, _32, _33;

		MATH_FUNCTION matrix() = default;

		MATH_FUNCTION explicit matrix(T a)
		    : _11(a), _12(a), _13(a), _21(a), _22(a), _23(a), _31(a), _32(a), _33(a)
		{
		}

		MATH_FUNCTION matrix(T m11, T m12, T m13, T m21, T m22, T m23, T m31, T m32, T m33)
		    : _11(m11), _12(m12), _13(m13), _21(m21), _22(m22), _23(m23), _31(m31), _32(m32), _33(m33)
		{
		}

		MATH_FUNCTION matrix(const affine_matrix<T, 2U>& M)
		    : _11(M._11), _12(M._12), _13(M._13), _21(M._21), _22(M._22), _23(M._23), _31(0.0f), _32(0.0f), _33(1.0f)
		{
		}

		static MATH_FUNCTION matrix from_rows(const vector<T, 3U>& r1, const vector<T, 3U>& r2, const vector<T, 3U>& r3)
		{
			return matrix(r1.x, r1.y, r1.z, r2.x, r2.y, r2.z, r3.x, r3.y, r3.z);
		}
		static MATH_FUNCTION matrix from_cols(const vector<T, 3U>& c1, const vector<T, 3U>& c2, const vector<T, 3U>& c3)
		{
			return matrix(c1.x, c2.x, c3.x, c1.y, c2.y, c3.y, c1.z, c2.z, c3.z);
		}

		MATH_FUNCTION const vector<T, 3U> row1() const
		{
			return vector<T, 3U>(_11, _12, _13);
		}
		MATH_FUNCTION const vector<T, 3U> row2() const
		{
			return vector<T, 3U>(_21, _22, _23);
		}
		MATH_FUNCTION const vector<T, 3U> row3() const
		{
			return vector<T, 3U>(_31, _32, _33);
		}

		MATH_FUNCTION const vector<T, 3U> column1() const
		{
			return vector<T, 3U>(_11, _21, _31);
		}
		MATH_FUNCTION const vector<T, 3U> column2() const
		{
			return vector<T, 3U>(_12, _22, _32);
		}
		MATH_FUNCTION const vector<T, 3U> column3() const
		{
			return vector<T, 3U>(_13, _23, _33);
		}

		MATH_FUNCTION friend matrix transpose(const matrix& m)
		{
			return matrix(m._11, m._21, m._31, m._12, m._22, m._32, m._13, m._23, m._33);
		}

		MATH_FUNCTION friend T determinant(const matrix& m)
		{
			return m._11 * m._22 * m._33 + m._12 * m._23 * m._31 + m._13 * m._21 * m._32 - m._13 * m._22 * m._31 - m._12 * m._21 * m._33 - m._11 * m._23 * m._32;
		}

		MATH_FUNCTION friend matrix operator+(const matrix& a, const matrix& b)
		{
			return matrix(a._11 + b._11, a._12 + b._12, a._13 + b._13, a._21 + b._21, a._22 + b._22, a._23 + b._23, a._31 + b._31, a._32 + b._32, a._33 + b._33);
		}

		MATH_FUNCTION friend matrix operator*(float f, const matrix& m)
		{
			return matrix(f * m._11, f * m._12, f * m._13, f * m._21, f * m._22, f * m._23, f * m._31, f * m._32, f * m._33);
		}

		MATH_FUNCTION friend matrix operator*(const matrix& m, float f)
		{
			return f * m;
		}

		MATH_FUNCTION friend matrix operator*(const matrix& a, const matrix& b)
		{
			return matrix(a._11 * b._11 + a._12 * b._21 + a._13 * b._31, a._11 * b._12 + a._12 * b._22 + a._13 * b._32, a._11 * b._13 + a._12 * b._23 + a._13 * b._33, a._21 * b._11 + a._22 * b._21 + a._23 * b._31, a._21 * b._12 + a._22 * b._22 + a._23 * b._32, a._21 * b._13 + a._22 * b._23 + a._23 * b._33, a._31 * b._11 + a._32 * b._21 + a._33 * b._31, a._31 * b._12 + a._32 * b._22 + a._33 * b._32, a._31 * b._13 + a._32 * b._23 + a._33 * b._33);
		}

		MATH_FUNCTION friend vector<T, 3U> operator*(const vector<T, 3U>& v, const matrix& m)
		{
			return vector<T, 3U>(v.x * m._11 + v.y * m._21 + v.z * m._31,
			                     v.x * m._12 + v.y * m._22 + v.z * m._32,
			                     v.x * m._13 + v.y * m._23 + v.z * m._33);
		}

		MATH_FUNCTION friend vector<T, 3U> operator*(const matrix& m, const vector<T, 3U>& v)
		{
			return vector<T, 3U>(m._11 * v.x + m._12 * v.y + m._13 * v.z,
			                     m._21 * v.x + m._22 * v.y + m._23 * v.z,
			                     m._31 * v.x + m._32 * v.y + m._33 * v.z);
		}

		MATH_FUNCTION friend T trace(const matrix& M)
		{
			return M._11 + M._22 + M._33;
		}
	};

	template <typename T>
	class matrix<T, 3U, 4U>
	{
	public:
		static const unsigned int M = 3U;
		static const unsigned int N = 4U;
		typedef T field_type;

		T _11, _12, _13, _14;
		T _21, _22, _23, _24;
		T _31, _32, _33, _34;

		MATH_FUNCTION matrix() = default;

		MATH_FUNCTION explicit matrix(T a)
		    : _11(a), _12(a), _13(a), _14(a), _21(a), _22(a), _23(a), _24(a), _31(a), _32(a), _33(a), _34(a)
		{
		}

		MATH_FUNCTION matrix(T m11, T m12, T m13, T m14, T m21, T m22, T m23, T m24, T m31, T m32, T m33, T m34)
		    : _11(m11), _12(m12), _13(m13), _14(m14), _21(m21), _22(m22), _23(m23), _24(m24), _31(m31), _32(m32), _33(m33), _34(m34)
		{
		}

		MATH_FUNCTION matrix(const affine_matrix<T, 3U>& M)
		    : _11(M._11), _12(M._12), _13(M._13), _14(M._14), _21(M._21), _22(M._22), _23(M._23), _24(M._24), _31(M._31), _32(M._32), _33(M._33), _34(M._34)
		{
		}

		static MATH_FUNCTION matrix from_rows(const vector<T, 4U>& r1, const vector<T, 4U>& r2, const vector<T, 4U>& r3)
		{
			return matrix(r1.x, r1.y, r1.z, r1.w, r2.x, r2.y, r2.z, r2.w, r3.x, r3.y, r3.z, r3.w);
		}
		static MATH_FUNCTION matrix from_cols(const vector<T, 3U>& c1, const vector<T, 3U>& c2, const vector<T, 3U>& c3, const vector<T, 3U>& c4)
		{
			return matrix(c1.x, c2.x, c3.x, c4.x, c1.y, c2.y, c3.y, c4.y, c1.z, c2.z, c3.z, c4.z);
		}

		MATH_FUNCTION const vector<T, 4U> row1() const
		{
			return vector<T, 4U>(_11, _12, _13, _14);
		}
		MATH_FUNCTION const vector<T, 4U> row2() const
		{
			return vector<T, 4U>(_21, _22, _23, _24);
		}
		MATH_FUNCTION const vector<T, 4U> row3() const
		{
			return vector<T, 4U>(_31, _32, _33, _34);
		}

		MATH_FUNCTION const vector<T, 3U> column1() const
		{
			return vector<T, 3U>(_11, _21, _31);
		}
		MATH_FUNCTION const vector<T, 3U> column2() const
		{
			return vector<T, 3U>(_12, _22, _32);
		}
		MATH_FUNCTION const vector<T, 3U> column3() const
		{
			return vector<T, 3U>(_13, _23, _33);
		}
		MATH_FUNCTION const vector<T, 3U> column4() const
		{
			return vector<T, 3U>(_14, _24, _34);
		}

		MATH_FUNCTION friend matrix<T, 4U, 3U> transpose(const matrix& m)
		{
			return matrix<T, 4U, 3U>(m._11, m._21, m._31, m._12, m._22, m._32, m._13, m._23, m._33, m._14, m._24, m._34);
		}

		MATH_FUNCTION friend matrix operator+(const matrix& a, const matrix& b)
		{
			return matrix(a._11 + b._11, a._12 + b._12, a._13 + b._13, a._14 + b._14, a._21 + b._21, a._22 + b._22, a._23 + b._23, a._24 + b._24, a._31 + b._31, a._32 + b._32, a._33 + b._33, a._34 + b._34);
		}

		MATH_FUNCTION friend matrix operator*(float f, const matrix& m)
		{
			return matrix(f * m._11, f * m._12, f * m._13, f * m._14, f * m._21, f * m._22, f * m._23, f * m._24, f * m._31, f * m._32, f * m._33, f * m._34);
		}

		MATH_FUNCTION friend matrix operator*(const matrix& m, float f)
		{
			return f * m;
		}

		MATH_FUNCTION friend vector<T, 4U> operator*(const vector<T, 3U>& v, const matrix& m)
		{
			return vector<T, 4U>(v.x * m._11 + v.y * m._21 + v.z * m._31,
			                     v.x * m._12 + v.y * m._22 + v.z * m._32,
			                     v.x * m._13 + v.y * m._23 + v.z * m._33,
			                     v.x * m._14 + v.y * m._24 + v.z * m._34);
		}

		MATH_FUNCTION friend vector<T, 3U> operator*(const matrix& m, const vector<T, 4>& v)
		{
			return vector<T, 3U>(m._11 * v.x + m._12 * v.y + m._13 * v.z + m._14 * v.w,
			                     m._21 * v.x + m._22 * v.y + m._23 * v.z + m._24 * v.w,
			                     m._31 * v.x + m._32 * v.y + m._33 * v.z + m._34 * v.w);
		}

		MATH_FUNCTION friend matrix operator*(const matrix& a, const matrix<T, 4U, 4U>& b)
		{
			return matrix(a._11 * b._11 + a._12 * b._21 + a._13 * b._31 + a._14 * b._41, a._11 * b._12 + a._12 * b._22 + a._13 * b._32 + a._14 * b._42, a._11 * b._13 + a._12 * b._23 + a._13 * b._33 + a._14 * b._43, a._11 * b._14 + a._12 * b._24 + a._13 * b._34 + a._14 * b._44,
				a._21 * b._11 + a._22 * b._21 + a._23 * b._31 + a._24 * b._41, a._21 * b._12 + a._22 * b._22 + a._23 * b._32 + a._24 * b._42, a._21 * b._13 + a._22 * b._23 + a._23 * b._33 + a._24 * b._43, a._21 * b._14 + a._22 * b._24 + a._23 * b._34 + a._24 * b._44,
				a._31 * b._11 + a._32 * b._21 + a._33 * b._31 + a._34 * b._41, a._31 * b._12 + a._32 * b._22 + a._33 * b._32 + a._34 * b._42, a._31 * b._13 + a._32 * b._23 + a._33 * b._33 + a._34 * b._43, a._31 * b._14 + a._32 * b._24 + a._33 * b._34 + a._34 * b._44);
		}
	};

	template <typename T>
	class matrix<T, 4U, 3U>
	{
	public:
		static const unsigned int M = 4U;
		static const unsigned int N = 3U;
		typedef T field_type;

		T _11, _12, _13;
		T _21, _22, _23;
		T _31, _32, _33;
		T _41, _42, _43;

		MATH_FUNCTION matrix() = default;

		MATH_FUNCTION explicit matrix(T a)
		    : _11(a), _12(a), _13(a), _21(a), _22(a), _23(a), _31(a), _32(a), _33(a), _41(a), _42(a), _43(a)
		{
		}

		MATH_FUNCTION matrix(T m11, T m12, T m13, T m21, T m22, T m23, T m31, T m32, T m33, T m41, T m42, T m43)
		    : _11(m11), _12(m12), _13(m13), _21(m21), _22(m22), _23(m23), _31(m31), _32(m32), _33(m33), _41(m41), _42(m42), _43(m43)
		{
		}

		static MATH_FUNCTION matrix from_rows(const vector<T, 3U>& r1, const vector<T, 3U>& r2, const vector<T, 3U>& r3, const vector<T, 3U>& r4)
		{
			return matrix(r1.x, r1.y, r1.z, r2.x, r2.y, r2.z, r3.x, r3.y, r3.z, r4.x, r4.y, r4.z);
		}
		static MATH_FUNCTION matrix from_cols(const vector<T, 4U>& c1, const vector<T, 4U>& c2, const vector<T, 4U>& c3)
		{
			return matrix(c1.x, c2.x, c3.x, c1.y, c2.y, c3.y, c1.z, c2.z, c3.z, c1.w, c2.w, c3.w);
		}

		MATH_FUNCTION const vector<T, 3U> row1() const
		{
			return vector<T, 3U>(_11, _12, _13);
		}
		MATH_FUNCTION const vector<T, 3U> row2() const
		{
			return vector<T, 3U>(_21, _22, _23);
		}
		MATH_FUNCTION const vector<T, 3U> row3() const
		{
			return vector<T, 3U>(_31, _32, _33);
		}
		MATH_FUNCTION const vector<T, 3U> row4() const
		{
			return vector<T, 3U>(_41, _42, _43);
		}

		MATH_FUNCTION const vector<T, 3U> column1() const
		{
			return vector<T, 4U>(_11, _21, _31, _41);
		}
		MATH_FUNCTION const vector<T, 3U> column2() const
		{
			return vector<T, 4U>(_12, _22, _32, _42);
		}
		MATH_FUNCTION const vector<T, 3U> column3() const
		{
			return vector<T, 4U>(_13, _23, _33, _43);
		}

		MATH_FUNCTION friend matrix<T, 3U, 4U> transpose(const matrix& m)
		{
			return matrix<T, 3U, 4U>(m._11, m._21, m._31, m._41, m._12, m._22, m._32, m._42, m._13, m._23, m._33, m._43);
		}

		MATH_FUNCTION friend matrix operator+(const matrix& a, const matrix& b)
		{
			return matrix(a._11 + b._11, a._12 + b._12, a._13 + b._13, a._21 + b._21, a._22 + b._22, a._23 + b._23, a._31 + b._31, a._32 + b._32, a._33 + b._33, a._41 + b._41, a._42 + b._42, a._43 + b._43);
		}

		MATH_FUNCTION friend matrix operator*(float f, const matrix& m)
		{
			return matrix(f * m._11, f * m._12, f * m._13, f * m._21, f * m._22, f * m._23, f * m._31, f * m._32, f * m._33, f * m._41, f * m._42, f * m._43);
		}

		MATH_FUNCTION friend matrix operator*(const matrix& m, float f)
		{
			return f * m;
		}

		MATH_FUNCTION friend vector<T, 3U> operator*(const vector<T, 4U>& v, const matrix& m)
		{
			return vector<T, 3U>(v.x * m._11 + v.y * m._21 + v.z * m._31 + v.w * m._41,
			                     v.x * m._12 + v.y * m._22 + v.z * m._32 + v.w * m._42,
			                     v.x * m._13 + v.y * m._23 + v.z * m._33 + v.w * m._43);
		}

		MATH_FUNCTION friend vector<T, 4U> operator*(const matrix& m, const vector<T, 3>& v)
		{
			return vector<T, 4U>(m._11 * v.x + m._12 * v.y + m._13 * v.z,
			                     m._21 * v.x + m._22 * v.y + m._23 * v.z,
			                     m._31 * v.x + m._32 * v.y + m._33 * v.z,
			                     m._41 * v.x + m._42 * v.y + m._43 * v.z);
		}

	};

	template <typename T>
	class matrix<T, 4U, 4U>
	{
	public:
		static const unsigned int M = 4U;
		static const unsigned int N = 4U;
		typedef T field_type;

		T _11, _12, _13, _14;
		T _21, _22, _23, _24;
		T _31, _32, _33, _34;
		T _41, _42, _43, _44;

		MATH_FUNCTION matrix() = default;

		MATH_FUNCTION explicit matrix(T a)
		    : _11(a), _12(a), _13(a), _14(a), _21(a), _22(a), _23(a), _24(a), _31(a), _32(a), _33(a), _34(a), _41(a), _42(a), _43(a), _44(a)
		{
		}

		MATH_FUNCTION matrix(T m11, T m12, T m13, T m14, T m21, T m22, T m23, T m24, T m31, T m32, T m33, T m34, T m41, T m42, T m43, T m44)
		    : _11(m11), _12(m12), _13(m13), _14(m14), _21(m21), _22(m22), _23(m23), _24(m24), _31(m31), _32(m32), _33(m33), _34(m34), _41(m41), _42(m42), _43(m43), _44(m44)
		{
		}

		MATH_FUNCTION matrix(const affine_matrix<T, 3U>& M)
		    : _11(M._11), _12(M._12), _13(M._13), _14(M._14), _21(M._21), _22(M._22), _23(M._23), _24(M._24), _31(M._31), _32(M._32), _33(M._33), _34(M._34), _41(0.0f), _42(0.0f), _43(0.0f), _44(1.0f)
		{
		}

		static MATH_FUNCTION matrix from_rows(const vector<T, 4U>& r1, const vector<T, 4U>& r2, const vector<T, 4U>& r3, const vector<T, 4U>& r4)
		{
			return matrix(r1.x, r1.y, r1.z, r1.w, r2.x, r2.y, r2.z, r2.w, r3.x, r3.y, r3.z, r3.w, r4.x, r4.y, r4.z, r4.w);
		}
		static MATH_FUNCTION matrix from_cols(const vector<T, 4U>& c1, const vector<T, 4U>& c2, const vector<T, 4U>& c3, const vector<T, 4U>& c4)
		{
			return matrix(c1.x, c2.x, c3.x, c4.x, c1.y, c2.y, c3.y, c4.y, c1.z, c2.z, c3.z, c4.z, c1.w, c2.w, c3.w, c4.w);
		}

		MATH_FUNCTION const vector<T, 4U> row1() const
		{
			return vector<T, 4U>(_11, _12, _13, _14);
		}
		MATH_FUNCTION const vector<T, 4U> row2() const
		{
			return vector<T, 4U>(_21, _22, _23, _24);
		}
		MATH_FUNCTION const vector<T, 4U> row3() const
		{
			return vector<T, 4U>(_31, _32, _33, _34);
		}
		MATH_FUNCTION const vector<T, 4U> row4() const
		{
			return vector<T, 4U>(_41, _42, _43, _44);
		}

		MATH_FUNCTION const vector<T, 4U> column1() const
		{
			return vector<T, 4U>(_11, _21, _31, _41);
		}
		MATH_FUNCTION const vector<T, 4U> column2() const
		{
			return vector<T, 4U>(_12, _22, _32, _42);
		}
		MATH_FUNCTION const vector<T, 4U> column3() const
		{
			return vector<T, 4U>(_13, _23, _33, _43);
		}
		MATH_FUNCTION const vector<T, 4U> column4() const
		{
			return vector<T, 4U>(_14, _24, _34, _44);
		}

		MATH_FUNCTION friend matrix transpose(const matrix& m)
		{
			return matrix(m._11, m._21, m._31, m._41, m._12, m._22, m._32, m._42, m._13, m._23, m._33, m._43, m._14, m._24, m._34, m._44);
		}

		MATH_FUNCTION friend matrix operator+(const matrix& a, const matrix& b)
		{
			return matrix(a._11 + b._11, a._12 + b._12, a._13 + b._13, a._14 + b._14, a._21 + b._21, a._22 + b._22, a._23 + b._23, a._24 + b._24, a._31 + b._31, a._32 + b._32, a._33 + b._33, a._34 + b._34, a._41 + b._41, a._42 + b._42, a._43 + b._43, a._44 + b._44);
		}

		MATH_FUNCTION friend matrix operator*(float f, const matrix& m)
		{
			return matrix(f * m._11, f * m._12, f * m._13, f * m._14, f * m._21, f * m._22, f * m._23, f * m._24, f * m._31, f * m._32, f * m._33, f * m._34, f * m._41, f * m._42, f * m._43, f * m._44);
		}

		MATH_FUNCTION friend matrix operator*(const matrix& m, float f)
		{
			return f * m;
		}

		MATH_FUNCTION friend matrix operator*(const matrix& a, const matrix& b)
		{
			return matrix(a._11 * b._11 + a._12 * b._21 + a._13 * b._31 + a._14 * b._41, a._11 * b._12 + a._12 * b._22 + a._13 * b._32 + a._14 * b._42, a._11 * b._13 + a._12 * b._23 + a._13 * b._33 + a._14 * b._43, a._11 * b._14 + a._12 * b._24 + a._13 * b._34 + a._14 * b._44, a._21 * b._11 + a._22 * b._21 + a._23 * b._31 + a._24 * b._41, a._21 * b._12 + a._22 * b._22 + a._23 * b._32 + a._24 * b._42, a._21 * b._13 + a._22 * b._23 + a._23 * b._33 + a._24 * b._43, a._21 * b._14 + a._22 * b._24 + a._23 * b._34 + a._24 * b._44, a._31 * b._11 + a._32 * b._21 + a._33 * b._31 + a._34 * b._41, a._31 * b._12 + a._32 * b._22 + a._33 * b._32 + a._34 * b._42, a._31 * b._13 + a._32 * b._23 + a._33 * b._33 + a._34 * b._43, a._31 * b._14 + a._32 * b._24 + a._33 * b._34 + a._34 * b._44, a._41 * b._11 + a._42 * b._21 + a._43 * b._31 + a._44 * b._41, a._41 * b._12 + a._42 * b._22 + a._43 * b._32 + a._44 * b._42, a._41 * b._13 + a._42 * b._23 + a._43 * b._33 + a._44 * b._43, a._41 * b._14 + a._42 * b._24 + a._43 * b._34 + a._44 * b._44);
		}

		MATH_FUNCTION friend vector<T, 4U> operator*(const vector<T, 4U>& v, const matrix& m)
		{
			return vector<T, 4U>(v.x * m._11 + v.y * m._21 + v.z * m._31 + v.w * m._41,
			                     v.x * m._12 + v.y * m._22 + v.z * m._32 + v.w * m._42,
			                     v.x * m._13 + v.y * m._23 + v.z * m._33 + v.w * m._43,
			                     v.x * m._14 + v.y * m._24 + v.z * m._34 + v.w * m._44);
		}

		MATH_FUNCTION friend vector<T, 4U> operator*(const matrix& m, const vector<T, 4>& v)
		{
			return vector<T, 4U>(m._11 * v.x + m._12 * v.y + m._13 * v.z + m._14 * v.w,
			                     m._21 * v.x + m._22 * v.y + m._23 * v.z + m._24 * v.w,
			                     m._31 * v.x + m._32 * v.y + m._33 * v.z + m._34 * v.w,
			                     m._41 * v.x + m._42 * v.y + m._43 * v.z + m._44 * v.w);
		}

		MATH_FUNCTION T operator[](unsigned int i) const
		{
			return _m[i];
		}

		MATH_FUNCTION friend T trace(const matrix& M)
		{
			return M._11 + M._22 + M._33 + M._44;
		}
	};

	template <typename T>
	class affine_matrix<T, 2U>
	{
	public:
		typedef T field_type;

		T _11, _12, _13;
		T _21, _22, _23;

		MATH_FUNCTION affine_matrix() = default;

		MATH_FUNCTION explicit affine_matrix(T a)
		    : _11(a), _12(a), _13(a), _21(a), _22(a), _23(a)
		{
		}

		MATH_FUNCTION affine_matrix(T m11, T m12, T m13, T m21, T m22, T m23)
		    : _11(m11), _12(m12), _13(m13), _21(m21), _22(m22), _23(m23)
		{
		}

		MATH_FUNCTION affine_matrix(const matrix<T, 2U, 3U>& M)
		    : _11(M._11), _12(M._12), _13(M._13), _21(M._21), _22(M._22), _23(M._23)
		{
		}

		MATH_FUNCTION affine_matrix(const matrix<T, 3U, 3U>& M)
		    : _11(M._11), _12(M._12), _13(M._13), _21(M._21), _22(M._22), _23(M._23)
		{
		}

		MATH_FUNCTION friend affine_matrix operator+(const affine_matrix& a, const affine_matrix& b)
		{
			return matrix<T, 3U, 3U>(a) + matrix<T, 3U, 3U>(b);
		}

		MATH_FUNCTION friend affine_matrix operator+(const affine_matrix& a, const matrix<T, 3U, 3U>& b)
		{
			return matrix<T, 3U, 3U>(a) + b;
		}

		MATH_FUNCTION friend affine_matrix operator+(const matrix<T, 3U, 3U>& a, const affine_matrix& b)
		{
			return a + matrix<T, 3U, 3U>(b);
		}

		MATH_FUNCTION friend affine_matrix operator*(const affine_matrix& a, const affine_matrix& b)
		{
			return matrix<T, 3U, 3U>(a) * matrix<T, 3U, 3U>(b);
		}

		MATH_FUNCTION friend const matrix<T, 3U, 3U> operator*(const affine_matrix& a, const matrix<T, 3U, 3U>& b)
		{
			return matrix<T, 3U, 3U>(a) * b;
		}

		MATH_FUNCTION friend const matrix<T, 3U, 3U> operator*(const matrix<T, 3U, 3U>& a, const affine_matrix& b)
		{
			return a * matrix<T, 3U, 3U>(b);
		}

		MATH_FUNCTION friend vector<T, 3U> operator*(const vector<T, 3U>& v, const affine_matrix& m)
		{
			return vector<T, 3U>(v.x * m._11 + v.y * m._21,
			                     v.x * m._12 + v.y * m._22,
			                     v.x * m._13 + v.y * m._23 + v.z);
		}

		MATH_FUNCTION friend vector<T, 3U> operator*(const affine_matrix& m, const vector<T, 3>& v)
		{
			return vector<T, 4U>(m._11 * v.x + m._12 * v.y + m._13 * v.z,
			                     m._21 * v.x + m._22 * v.y + m._23 * v.z,
			                     v.z);
		}
	};

	template <typename T>
	class affine_matrix<T, 3U>
	{
	public:
		typedef T field_type;

		T _11, _12, _13, _14;
		T _21, _22, _23, _24;
		T _31, _32, _33, _34;

		MATH_FUNCTION affine_matrix() = default;

		MATH_FUNCTION explicit affine_matrix(T a)
		    : _11(a), _12(a), _13(a), _14(a), _21(a), _22(a), _23(a), _24(a), _31(a), _32(a), _33(a), _34(a)
		{
		}

		MATH_FUNCTION affine_matrix(T m11, T m12, T m13, T m14, T m21, T m22, T m23, T m24, T m31, T m32, T m33, T m34)
		    : _11(m11), _12(m12), _13(m13), _14(m14), _21(m21), _22(m22), _23(m23), _24(m24), _31(m31), _32(m32), _33(m33), _34(m34)
		{
		}

		MATH_FUNCTION affine_matrix(const matrix<T, 3U, 4U>& M)
		    : _11(M._11), _12(M._12), _13(M._13), _14(M._14), _21(M._21), _22(M._22), _23(M._23), _24(M._24), _31(M._31), _32(M._32), _33(M._33), _34(M._34)
		{
		}

		MATH_FUNCTION affine_matrix(const matrix<T, 4U, 4U>& M)
		    : _11(M._11), _12(M._12), _13(M._13), _14(M._14), _21(M._21), _22(M._22), _23(M._23), _24(M._24), _31(M._31), _32(M._32), _33(M._33), _34(M._34)
		{
		}

		MATH_FUNCTION friend affine_matrix operator+(const affine_matrix& a, const affine_matrix& b)
		{
			return matrix<T, 4U, 4U>(a) + matrix<T, 4U, 4U>(b);
		}

		MATH_FUNCTION friend affine_matrix operator+(const affine_matrix& a, const matrix<T, 4U, 4U>& b)
		{
			return matrix<T, 4U, 4U>(a) + b;
		}

		MATH_FUNCTION friend affine_matrix operator+(const matrix<T, 4U, 4U>& a, const affine_matrix& b)
		{
			return a + matrix<T, 4U, 4U>(b);
		}

		MATH_FUNCTION friend affine_matrix operator*(const affine_matrix& a, const affine_matrix& b)
		{
			return matrix<T, 4U, 4U>(a) * matrix<T, 4U, 4U>(b);
		}

		MATH_FUNCTION friend const matrix<T, 4U, 4U> operator*(const affine_matrix& a, const matrix<T, 4U, 4U>& b)
		{
			return matrix<T, 4U, 4U>(a) * b;
		}

		MATH_FUNCTION friend const matrix<T, 4U, 4U> operator*(const matrix<T, 4U, 4U>& a, const affine_matrix& b)
		{
			return a * matrix<T, 4U, 4U>(b);
		}

		MATH_FUNCTION friend vector<T, 4U> operator*(const vector<T, 4U>& v, const affine_matrix& m)
		{
			return vector<T, 4U>(v.x * m._11 + v.y * m._21 + v.z * m._31,
			                     v.x * m._12 + v.y * m._22 + v.z * m._32,
			                     v.x * m._13 + v.y * m._23 + v.z * m._33,
			                     v.x * m._14 + v.y * m._24 + v.z * m._34 + v.w);
		}

		MATH_FUNCTION friend vector<T, 4U> operator*(const affine_matrix& m, const vector<T, 4>& v)
		{
			return vector<T, 4U>(m._11 * v.x + m._12 * v.y + m._13 * v.z + m._14 * v.w,
			                     m._21 * v.x + m._22 * v.y + m._23 * v.z + m._24 * v.w,
			                     m._31 * v.x + m._32 * v.y + m._33 * v.z + m._34 * v.w,
			                     v.w);
		}
	};

	template <typename T>
	MATH_FUNCTION inline T det(const matrix<T, 2U, 2U>& m)
	{
		return m._11 * m._22 - m._21 * m._12;
	}

	template <typename T>
	MATH_FUNCTION inline T det(const matrix<T, 3U, 3U>& m)
	{
		return m._11 * det(matrix<T, 2U, 2U>(m._22, m._23, m._32, m._33)) -
		       m._12 * det(matrix<T, 2U, 2U>(m._21, m._23, m._31, m._33)) +
		       m._13 * det(matrix<T, 2U, 2U>(m._21, m._22, m._31, m._32));
	}

	template <typename T>
	MATH_FUNCTION inline T det(const matrix<T, 4U, 4U>& m)
	{
		return m._11 * det(matrix<T, 3U, 3U>(m._22, m._23, m._24, m._32, m._33, m._34, m._42, m._43, m._44)) -
		       m._12 * det(matrix<T, 3U, 3U>(m._21, m._23, m._24, m._31, m._33, m._34, m._41, m._43, m._44)) +
		       m._13 * det(matrix<T, 3U, 3U>(m._21, m._22, m._24, m._31, m._32, m._34, m._41, m._42, m._44)) -
		       m._14 * det(matrix<T, 3U, 3U>(m._21, m._22, m._23, m._31, m._32, m._33, m._41, m._42, m._43));
	}

	template <typename T>
	MATH_FUNCTION inline matrix<T, 3U, 3U> adj(const matrix<T, 3U, 3U>& m)
	{
		return transpose(matrix<T, 3U, 3U>(
		    det(matrix<T, 2U, 2U>(m._22, m._23, m._32, m._33)),
		    -det(matrix<T, 2U, 2U>(m._21, m._23, m._31, m._33)),
		    det(matrix<T, 2U, 2U>(m._21, m._22, m._31, m._32)),

		    -det(matrix<T, 2U, 2U>(m._12, m._13, m._32, m._33)),
		    det(matrix<T, 2U, 2U>(m._11, m._13, m._31, m._33)),
		    -det(matrix<T, 2U, 2U>(m._11, m._12, m._31, m._32)),

		    det(matrix<T, 2U, 2U>(m._12, m._13, m._22, m._23)),
		    -det(matrix<T, 2U, 2U>(m._11, m._13, m._21, m._23)),
		    det(matrix<T, 2U, 2U>(m._11, m._12, m._21, m._22))));
	}

	template <typename T>
	MATH_FUNCTION inline matrix<T, 4U, 4U> adj(const matrix<T, 4U, 4U>& m)
	{
		return transpose(matrix<T, 4U, 4U>(
		    det(matrix<T, 3U, 3U>(m._22, m._23, m._24, m._32, m._33, m._34, m._42, m._43, m._44)),
		    -det(matrix<T, 3U, 3U>(m._21, m._23, m._24, m._31, m._33, m._34, m._41, m._43, m._44)),
		    det(matrix<T, 3U, 3U>(m._21, m._22, m._24, m._31, m._32, m._34, m._41, m._42, m._44)),
		    -det(matrix<T, 3U, 3U>(m._21, m._22, m._23, m._31, m._32, m._33, m._41, m._42, m._43)),

		    -det(matrix<T, 3U, 3U>(m._12, m._13, m._14, m._32, m._33, m._34, m._42, m._43, m._44)),
		    det(matrix<T, 3U, 3U>(m._11, m._13, m._14, m._31, m._33, m._34, m._41, m._43, m._44)),
		    -det(matrix<T, 3U, 3U>(m._11, m._12, m._14, m._31, m._32, m._34, m._41, m._42, m._44)),
		    det(matrix<T, 3U, 3U>(m._11, m._12, m._13, m._31, m._32, m._33, m._41, m._42, m._43)),

		    det(matrix<T, 3U, 3U>(m._12, m._13, m._14, m._22, m._23, m._24, m._42, m._43, m._44)),
		    -det(matrix<T, 3U, 3U>(m._11, m._13, m._14, m._21, m._23, m._24, m._41, m._43, m._44)),
		    det(matrix<T, 3U, 3U>(m._11, m._12, m._14, m._21, m._22, m._24, m._41, m._42, m._44)),
		    -det(matrix<T, 3U, 3U>(m._11, m._12, m._13, m._21, m._22, m._23, m._41, m._42, m._43)),

		    -det(matrix<T, 3U, 3U>(m._12, m._13, m._14, m._22, m._23, m._24, m._32, m._33, m._34)),
		    det(matrix<T, 3U, 3U>(m._11, m._13, m._14, m._21, m._23, m._24, m._31, m._33, m._34)),
		    -det(matrix<T, 3U, 3U>(m._11, m._12, m._14, m._21, m._22, m._24, m._31, m._32, m._34)),
		    det(matrix<T, 3U, 3U>(m._11, m._12, m._13, m._21, m._22, m._23, m._31, m._32, m._33))));
	}

	template <typename T, unsigned int N>
	MATH_FUNCTION inline matrix<T, N, N> inverse(const matrix<T, N, N>& M)
	{
		// TODO: optimize; compute det using adj
		return rcp(det(M)) * adj(M);
	}

	template <typename T, unsigned int D>
	MATH_FUNCTION inline affine_matrix<T, D> inverse(const affine_matrix<T, D>& M)
	{
		return affine_matrix<T, D>(inverse(matrix<T, D + 1, D + 1>(M)));
	}

	typedef matrix<float, 2U, 2U> float2x2;
	typedef matrix<float, 2U, 3U> float2x3;
	typedef matrix<float, 3U, 3U> float3x3;
	typedef matrix<float, 3U, 4U> float3x4;
	typedef matrix<float, 4U, 3U> float4x3;
	typedef matrix<float, 4U, 4U> float4x4;

	typedef affine_matrix<float, 2U> affine_float3x3;
	typedef affine_matrix<float, 3U> affine_float4x4;

	template <typename T>
	MATH_FUNCTION inline T identity();

	template <>
	MATH_FUNCTION inline float2x2 identity<float2x2>()
	{
		return float2x2(1.0f, 0.0f, 0.0f, 1.0f);
	}

	template <>
	MATH_FUNCTION inline float3x3 identity<float3x3>()
	{
		return float3x3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	}

	template <>
	MATH_FUNCTION inline float3x4 identity<float3x4>()
	{
		return float3x4(1.0f, 0.0f, 0.0f, 0.0f,
		                0.0f, 1.0f, 0.0f, 0.0f,
		                0.0f, 0.0f, 1.0f, 0.0f);
	}

	template <>
	MATH_FUNCTION inline float4x3 identity<float4x3>()
	{
		return float4x3(1.0f, 0.0f, 0.0f,
		                0.0f, 1.0f, 0.0f,
		                0.0f, 0.0f, 1.0f,
		                0.0f, 0.0f, 0.0f);
	}

	template <>
	MATH_FUNCTION inline affine_float3x3 identity<math::affine_float3x3>()
	{
		return affine_float3x3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	}

	template <>
	MATH_FUNCTION inline float4x4 identity<math::float4x4>()
	{
		return float4x4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	}

	template <>
	MATH_FUNCTION inline affine_float4x4 identity<affine_float4x4>()
	{
		return affine_float4x4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	}
}

#endif // INCLUDED_MATH_MATRIX

/*
 * Copyright 1993-2012 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 *
 */

/*
    This file implements common mathematical operations on vector types
    (float3, float4 etc.) since these are not provided as standard by CUDA.

    The syntax is modelled on the Cg standard library.

    This is part of the CUTIL library and is not supported by NVIDIA.

    Thanks to Linh Hah for additions and fixes.
*/

#ifndef CUTIL_MATH_H
#define CUTIL_MATH_H

typedef unsigned int uint;
typedef unsigned short ushort;

////////////////////////////////////////////////////////////////////////////////
// constructors
////////////////////////////////////////////////////////////////////////////////

inline __host__ __device__ float2 make_float2(float s)
{
    return make_float2(s, s);
}
inline __host__ __device__ float2 make_float2(float3 a)
{
    return make_float2(a.x, a.y);
}
inline __host__ __device__ float2 make_float2(int2 a)
{
    return make_float2(float(a.x), float(a.y));
}
inline __host__ __device__ float2 make_float2(uint2 a)
{
    return make_float2(float(a.x), float(a.y));
}

inline __host__ __device__ int2 make_int2(int s)
{
    return make_int2(s, s);
}
inline __host__ __device__ int2 make_int2(int3 a)
{
    return make_int2(a.x, a.y);
}
inline __host__ __device__ int2 make_int2(uint2 a)
{
    return make_int2(int(a.x), int(a.y));
}
inline __host__ __device__ int2 make_int2(float2 a)
{
    return make_int2(int(a.x), int(a.y));
}

inline __host__ __device__ uint2 make_uint2(uint s)
{
    return make_uint2(s, s);
}
inline __host__ __device__ uint2 make_uint2(uint3 a)
{
    return make_uint2(a.x, a.y);
}
inline __host__ __device__ uint2 make_uint2(int2 a)
{
    return make_uint2(uint(a.x), uint(a.y));
}

inline __host__ __device__ float3 make_float3(float s)
{
    return make_float3(s, s, s);
}
inline __host__ __device__ float3 make_float3(float2 a)
{
    return make_float3(a.x, a.y, 0.0f);
}
inline __host__ __device__ float3 make_float3(float2 a, float s)
{
    return make_float3(a.x, a.y, s);
}
inline __host__ __device__ float3 make_float3(float4 a)
{
    return make_float3(a.x, a.y, a.z);
}
inline __host__ __device__ float3 make_float3(int3 a)
{
    return make_float3(float(a.x), float(a.y), float(a.z));
}
inline __host__ __device__ float3 make_float3(uint3 a)
{
    return make_float3(float(a.x), float(a.y), float(a.z));
}

inline __host__ __device__ int3 make_int3(int s)
{
    return make_int3(s, s, s);
}
inline __host__ __device__ int3 make_int3(int2 a)
{
    return make_int3(a.x, a.y, 0);
}
inline __host__ __device__ int3 make_int3(int2 a, int s)
{
    return make_int3(a.x, a.y, s);
}
inline __host__ __device__ int3 make_int3(uint3 a)
{
    return make_int3(int(a.x), int(a.y), int(a.z));
}
inline __host__ __device__ int3 make_int3(float3 a)
{
    return make_int3(int(a.x), int(a.y), int(a.z));
}

inline __host__ __device__ uint3 make_uint3(uint s)
{
    return make_uint3(s, s, s);
}
inline __host__ __device__ uint3 make_uint3(uint2 a)
{
    return make_uint3(a.x, a.y, 0);
}
inline __host__ __device__ uint3 make_uint3(uint2 a, uint s)
{
    return make_uint3(a.x, a.y, s);
}
inline __host__ __device__ uint3 make_uint3(uint4 a)
{
    return make_uint3(a.x, a.y, a.z);
}
inline __host__ __device__ uint3 make_uint3(int3 a)
{
    return make_uint3(uint(a.x), uint(a.y), uint(a.z));
}

inline __host__ __device__ float4 make_float4(float s)
{
    return make_float4(s, s, s, s);
}
inline __host__ __device__ float4 make_float4(float3 a)
{
    return make_float4(a.x, a.y, a.z, 0.0f);
}
inline __host__ __device__ float4 make_float4(float3 a, float w)
{
    return make_float4(a.x, a.y, a.z, w);
}
inline __host__ __device__ float4 make_float4(int4 a)
{
    return make_float4(float(a.x), float(a.y), float(a.z), float(a.w));
}
inline __host__ __device__ float4 make_float4(uint4 a)
{
    return make_float4(float(a.x), float(a.y), float(a.z), float(a.w));
}

inline __host__ __device__ int4 make_int4(int s)
{
    return make_int4(s, s, s, s);
}
inline __host__ __device__ int4 make_int4(int3 a)
{
    return make_int4(a.x, a.y, a.z, 0);
}
inline __host__ __device__ int4 make_int4(int3 a, int w)
{
    return make_int4(a.x, a.y, a.z, w);
}
inline __host__ __device__ int4 make_int4(uint4 a)
{
    return make_int4(int(a.x), int(a.y), int(a.z), int(a.w));
}
inline __host__ __device__ int4 make_int4(float4 a)
{
    return make_int4(int(a.x), int(a.y), int(a.z), int(a.w));
}


inline __host__ __device__ uint4 make_uint4(uint s)
{
    return make_uint4(s, s, s, s);
}
inline __host__ __device__ uint4 make_uint4(uint3 a)
{
    return make_uint4(a.x, a.y, a.z, 0);
}
inline __host__ __device__ uint4 make_uint4(uint3 a, uint w)
{
    return make_uint4(a.x, a.y, a.z, w);
}
inline __host__ __device__ uint4 make_uint4(int4 a)
{
    return make_uint4(uint(a.x), uint(a.y), uint(a.z), uint(a.w));
}

////////////////////////////////////////////////////////////////////////////////
// negate
////////////////////////////////////////////////////////////////////////////////

inline __host__ __device__ float2 operator-(float2 &a)
{
    return make_float2(-a.x, -a.y);
}
inline __host__ __device__ int2 operator-(int2 &a)
{
    return make_int2(-a.x, -a.y);
}
inline __host__ __device__ float3 operator-(float3 &a)
{
    return make_float3(-a.x, -a.y, -a.z);
}
inline __host__ __device__ int3 operator-(int3 &a)
{
    return make_int3(-a.x, -a.y, -a.z);
}
inline __host__ __device__ float4 operator-(float4 &a)
{
    return make_float4(-a.x, -a.y, -a.z, -a.w);
}
inline __host__ __device__ int4 operator-(int4 &a)
{
    return make_int4(-a.x, -a.y, -a.z, -a.w);
}

////////////////////////////////////////////////////////////////////////////////
// addition
////////////////////////////////////////////////////////////////////////////////

inline __host__ __device__ float2 operator+(float2 a, float2 b)
{
    return make_float2(a.x + b.x, a.y + b.y);
}
inline __host__ __device__ void operator+=(float2 &a, float2 b)
{
    a.x += b.x; a.y += b.y;
}
inline __host__ __device__ float2 operator+(float2 a, float b)
{
    return make_float2(a.x + b, a.y + b);
}
inline __host__ __device__ float2 operator+(float b, float2 a)
{
    return make_float2(a.x + b, a.y + b);
}
inline __host__ __device__ void operator+=(float2 &a, float b)
{
    a.x += b; a.y += b;
}

inline __host__ __device__ int2 operator+(int2 a, int2 b)
{
    return make_int2(a.x + b.x, a.y + b.y);
}
inline __host__ __device__ void operator+=(int2 &a, int2 b)
{
    a.x += b.x; a.y += b.y;
}
inline __host__ __device__ int2 operator+(int2 a, int b)
{
    return make_int2(a.x + b, a.y + b);
}
inline __host__ __device__ int2 operator+(int b, int2 a)
{
    return make_int2(a.x + b, a.y + b);
}
inline __host__ __device__ void operator+=(int2 &a, int b)
{
    a.x += b; a.y += b;
}

inline __host__ __device__ uint2 operator+(uint2 a, uint2 b)
{
    return make_uint2(a.x + b.x, a.y + b.y);
}
inline __host__ __device__ void operator+=(uint2 &a, uint2 b)
{
    a.x += b.x; a.y += b.y;
}
inline __host__ __device__ uint2 operator+(uint2 a, uint b)
{
    return make_uint2(a.x + b, a.y + b);
}
inline __host__ __device__ uint2 operator+(uint b, uint2 a)
{
    return make_uint2(a.x + b, a.y + b);
}
inline __host__ __device__ void operator+=(uint2 &a, uint b)
{
    a.x += b; a.y += b;
}


inline __host__ __device__ float3 operator+(float3 a, float3 b)
{
    return make_float3(a.x + b.x, a.y + b.y, a.z + b.z);
}
inline __host__ __device__ void operator+=(float3 &a, float3 b)
{
    a.x += b.x; a.y += b.y; a.z += b.z;
}
inline __host__ __device__ float3 operator+(float3 a, float b)
{
    return make_float3(a.x + b, a.y + b, a.z + b);
}
inline __host__ __device__ void operator+=(float3 &a, float b)
{
    a.x += b; a.y += b; a.z += b;
}

inline __host__ __device__ int3 operator+(int3 a, int3 b)
{
    return make_int3(a.x + b.x, a.y + b.y, a.z + b.z);
}
inline __host__ __device__ void operator+=(int3 &a, int3 b)
{
    a.x += b.x; a.y += b.y; a.z += b.z;
}
inline __host__ __device__ int3 operator+(int3 a, int b)
{
    return make_int3(a.x + b, a.y + b, a.z + b);
}
inline __host__ __device__ void operator+=(int3 &a, int b)
{
    a.x += b; a.y += b; a.z += b;
}

inline __host__ __device__ uint3 operator+(uint3 a, uint3 b)
{
    return make_uint3(a.x + b.x, a.y + b.y, a.z + b.z);
}
inline __host__ __device__ void operator+=(uint3 &a, uint3 b)
{
    a.x += b.x; a.y += b.y; a.z += b.z;
}
inline __host__ __device__ uint3 operator+(uint3 a, uint b)
{
    return make_uint3(a.x + b, a.y + b, a.z + b);
}
inline __host__ __device__ void operator+=(uint3 &a, uint b)
{
    a.x += b; a.y += b; a.z += b;
}

inline __host__ __device__ int3 operator+(int b, int3 a)
{
    return make_int3(a.x + b, a.y + b, a.z + b);
}
inline __host__ __device__ uint3 operator+(uint b, uint3 a)
{
    return make_uint3(a.x + b, a.y + b, a.z + b);
}
inline __host__ __device__ float3 operator+(float b, float3 a)
{
    return make_float3(a.x + b, a.y + b, a.z + b);
}

inline __host__ __device__ float4 operator+(float4 a, float4 b)
{
    return make_float4(a.x + b.x, a.y + b.y, a.z + b.z,  a.w + b.w);
}
inline __host__ __device__ void operator+=(float4 &a, float4 b)
{
    a.x += b.x; a.y += b.y; a.z += b.z; a.w += b.w;
}
inline __host__ __device__ float4 operator+(float4 a, float b)
{
    return make_float4(a.x + b, a.y + b, a.z + b, a.w + b);
}
inline __host__ __device__ float4 operator+(float b, float4 a)
{
    return make_float4(a.x + b, a.y + b, a.z + b, a.w + b);
}
inline __host__ __device__ void operator+=(float4 &a, float b)
{
    a.x += b; a.y += b; a.z += b; a.w += b;
}

inline __host__ __device__ int4 operator+(int4 a, int4 b)
{
    return make_int4(a.x + b.x, a.y + b.y, a.z + b.z,  a.w + b.w);
}
inline __host__ __device__ void operator+=(int4 &a, int4 b)
{
    a.x += b.x; a.y += b.y; a.z += b.z; a.w += b.w;
}
inline __host__ __device__ int4 operator+(int4 a, int b)
{
    return make_int4(a.x + b, a.y + b, a.z + b,  a.w + b);
}
inline __host__ __device__ int4 operator+(int b, int4 a)
{
    return make_int4(a.x + b, a.y + b, a.z + b,  a.w + b);
}
inline __host__ __device__ void operator+=(int4 &a, int b)
{
    a.x += b; a.y += b; a.z += b; a.w += b;
}

inline __host__ __device__ uint4 operator+(uint4 a, uint4 b)
{
    return make_uint4(a.x + b.x, a.y + b.y, a.z + b.z,  a.w + b.w);
}
inline __host__ __device__ void operator+=(uint4 &a, uint4 b)
{
    a.x += b.x; a.y += b.y; a.z += b.z; a.w += b.w;
}
inline __host__ __device__ uint4 operator+(uint4 a, uint b)
{
    return make_uint4(a.x + b, a.y + b, a.z + b,  a.w + b);
}
inline __host__ __device__ uint4 operator+(uint b, uint4 a)
{
    return make_uint4(a.x + b, a.y + b, a.z + b,  a.w + b);
}
inline __host__ __device__ void operator+=(uint4 &a, uint b)
{
    a.x += b; a.y += b; a.z += b; a.w += b;
}

////////////////////////////////////////////////////////////////////////////////
// subtract
////////////////////////////////////////////////////////////////////////////////

inline __host__ __device__ float2 operator-(float2 a, float2 b)
{
    return make_float2(a.x - b.x, a.y - b.y);
}
inline __host__ __device__ void operator-=(float2 &a, float2 b)
{
    a.x -= b.x; a.y -= b.y;
}
inline __host__ __device__ float2 operator-(float2 a, float b)
{
    return make_float2(a.x - b, a.y - b);
}
inline __host__ __device__ float2 operator-(float b, float2 a)
{
    return make_float2(b - a.x, b - a.y);
}
inline __host__ __device__ void operator-=(float2 &a, float b)
{
    a.x -= b; a.y -= b;
}

inline __host__ __device__ int2 operator-(int2 a, int2 b)
{
    return make_int2(a.x - b.x, a.y - b.y);
}
inline __host__ __device__ void operator-=(int2 &a, int2 b)
{
    a.x -= b.x; a.y -= b.y;
}
inline __host__ __device__ int2 operator-(int2 a, int b)
{
    return make_int2(a.x - b, a.y - b);
}
inline __host__ __device__ int2 operator-(int b, int2 a)
{
    return make_int2(b - a.x, b - a.y);
}
inline __host__ __device__ void operator-=(int2 &a, int b)
{
    a.x -= b; a.y -= b;
}

inline __host__ __device__ uint2 operator-(uint2 a, uint2 b)
{
    return make_uint2(a.x - b.x, a.y - b.y);
}
inline __host__ __device__ void operator-=(uint2 &a, uint2 b)
{
    a.x -= b.x; a.y -= b.y;
}
inline __host__ __device__ uint2 operator-(uint2 a, uint b)
{
    return make_uint2(a.x - b, a.y - b);
}
inline __host__ __device__ uint2 operator-(uint b, uint2 a)
{
    return make_uint2(b - a.x, b - a.y);
}
inline __host__ __device__ void operator-=(uint2 &a, uint b)
{
    a.x -= b; a.y -= b;
}

inline __host__ __device__ float3 operator-(float3 a, float3 b)
{
    return make_float3(a.x - b.x, a.y - b.y, a.z - b.z);
}
inline __host__ __device__ void operator-=(float3 &a, float3 b)
{
    a.x -= b.x; a.y -= b.y; a.z -= b.z;
}
inline __host__ __device__ float3 operator-(float3 a, float b)
{
    return make_float3(a.x - b, a.y - b, a.z - b);
}
inline __host__ __device__ float3 operator-(float b, float3 a)
{
    return make_float3(b - a.x, b - a.y, b - a.z);
}
inline __host__ __device__ void operator-=(float3 &a, float b)
{
    a.x -= b; a.y -= b; a.z -= b;
}

inline __host__ __device__ int3 operator-(int3 a, int3 b)
{
    return make_int3(a.x - b.x, a.y - b.y, a.z - b.z);
}
inline __host__ __device__ void operator-=(int3 &a, int3 b)
{
    a.x -= b.x; a.y -= b.y; a.z -= b.z;
}
inline __host__ __device__ int3 operator-(int3 a, int b)
{
    return make_int3(a.x - b, a.y - b, a.z - b);
}
inline __host__ __device__ int3 operator-(int b, int3 a)
{
    return make_int3(b - a.x, b - a.y, b - a.z);
}
inline __host__ __device__ void operator-=(int3 &a, int b)
{
    a.x -= b; a.y -= b; a.z -= b;
}

inline __host__ __device__ uint3 operator-(uint3 a, uint3 b)
{
    return make_uint3(a.x - b.x, a.y - b.y, a.z - b.z);
}
inline __host__ __device__ void operator-=(uint3 &a, uint3 b)
{
    a.x -= b.x; a.y -= b.y; a.z -= b.z;
}
inline __host__ __device__ uint3 operator-(uint3 a, uint b)
{
    return make_uint3(a.x - b, a.y - b, a.z - b);
}
inline __host__ __device__ uint3 operator-(uint b, uint3 a)
{
    return make_uint3(b - a.x, b - a.y, b - a.z);
}
inline __host__ __device__ void operator-=(uint3 &a, uint b)
{
    a.x -= b; a.y -= b; a.z -= b;
}

inline __host__ __device__ float4 operator-(float4 a, float4 b)
{
    return make_float4(a.x - b.x, a.y - b.y, a.z - b.z,  a.w - b.w);
}
inline __host__ __device__ void operator-=(float4 &a, float4 b)
{
    a.x -= b.x; a.y -= b.y; a.z -= b.z; a.w -= b.w;
}
inline __host__ __device__ float4 operator-(float4 a, float b)
{
    return make_float4(a.x - b, a.y - b, a.z - b,  a.w - b);
}
inline __host__ __device__ void operator-=(float4 &a, float b)
{
    a.x -= b; a.y -= b; a.z -= b; a.w -= b;
}

inline __host__ __device__ int4 operator-(int4 a, int4 b)
{
    return make_int4(a.x - b.x, a.y - b.y, a.z - b.z,  a.w - b.w);
}
inline __host__ __device__ void operator-=(int4 &a, int4 b)
{
    a.x -= b.x; a.y -= b.y; a.z -= b.z; a.w -= b.w;
}
inline __host__ __device__ int4 operator-(int4 a, int b)
{
    return make_int4(a.x - b, a.y - b, a.z - b,  a.w - b);
}
inline __host__ __device__ int4 operator-(int b, int4 a)
{
    return make_int4(b - a.x, b - a.y, b - a.z, b - a.w);
}
inline __host__ __device__ void operator-=(int4 &a, int b)
{
    a.x -= b; a.y -= b; a.z -= b; a.w -= b;
}

inline __host__ __device__ uint4 operator-(uint4 a, uint4 b)
{
    return make_uint4(a.x - b.x, a.y - b.y, a.z - b.z,  a.w - b.w);
}
inline __host__ __device__ void operator-=(uint4 &a, uint4 b)
{
    a.x -= b.x; a.y -= b.y; a.z -= b.z; a.w -= b.w;
}
inline __host__ __device__ uint4 operator-(uint4 a, uint b)
{
    return make_uint4(a.x - b, a.y - b, a.z - b,  a.w - b);
}
inline __host__ __device__ uint4 operator-(uint b, uint4 a)
{
    return make_uint4(b - a.x, b - a.y, b - a.z, b - a.w);
}
inline __host__ __device__ void operator-=(uint4 &a, uint b)
{
    a.x -= b; a.y -= b; a.z -= b; a.w -= b;
}

////////////////////////////////////////////////////////////////////////////////
// multiply
////////////////////////////////////////////////////////////////////////////////

inline __host__ __device__ float2 operator*(float2 a, float2 b)
{
    return make_float2(a.x * b.x, a.y * b.y);
}
inline __host__ __device__ void operator*=(float2 &a, float2 b)
{
    a.x *= b.x; a.y *= b.y;
}
inline __host__ __device__ float2 operator*(float2 a, float b)
{
    return make_float2(a.x * b, a.y * b);
}
inline __host__ __device__ float2 operator*(float b, float2 a)
{
    return make_float2(b * a.x, b * a.y);
}
inline __host__ __device__ void operator*=(float2 &a, float b)
{
    a.x *= b; a.y *= b;
}

inline __host__ __device__ int2 operator*(int2 a, int2 b)
{
    return make_int2(a.x * b.x, a.y * b.y);
}
inline __host__ __device__ void operator*=(int2 &a, int2 b)
{
    a.x *= b.x; a.y *= b.y;
}
inline __host__ __device__ int2 operator*(int2 a, int b)
{
    return make_int2(a.x * b, a.y * b);
}
inline __host__ __device__ int2 operator*(int b, int2 a)
{
    return make_int2(b * a.x, b * a.y);
}
inline __host__ __device__ void operator*=(int2 &a, int b)
{
    a.x *= b; a.y *= b;
}

inline __host__ __device__ uint2 operator*(uint2 a, uint2 b)
{
    return make_uint2(a.x * b.x, a.y * b.y);
}
inline __host__ __device__ void operator*=(uint2 &a, uint2 b)
{
    a.x *= b.x; a.y *= b.y;
}
inline __host__ __device__ uint2 operator*(uint2 a, uint b)
{
    return make_uint2(a.x * b, a.y * b);
}
inline __host__ __device__ uint2 operator*(uint b, uint2 a)
{
    return make_uint2(b * a.x, b * a.y);
}
inline __host__ __device__ void operator*=(uint2 &a, uint b)
{
    a.x *= b; a.y *= b;
}

inline __host__ __device__ float3 operator*(float3 a, float3 b)
{
    return make_float3(a.x * b.x, a.y * b.y, a.z * b.z);
}
inline __host__ __device__ void operator*=(float3 &a, float3 b)
{
    a.x *= b.x; a.y *= b.y; a.z *= b.z;
}
inline __host__ __device__ float3 operator*(float3 a, float b)
{
    return make_float3(a.x * b, a.y * b, a.z * b);
}
inline __host__ __device__ float3 operator*(float b, float3 a)
{
    return make_float3(b * a.x, b * a.y, b * a.z);
}
inline __host__ __device__ void operator*=(float3 &a, float b)
{
    a.x *= b; a.y *= b; a.z *= b;
}

inline __host__ __device__ int3 operator*(int3 a, int3 b)
{
    return make_int3(a.x * b.x, a.y * b.y, a.z * b.z);
}
inline __host__ __device__ void operator*=(int3 &a, int3 b)
{
    a.x *= b.x; a.y *= b.y; a.z *= b.z;
}
inline __host__ __device__ int3 operator*(int3 a, int b)
{
    return make_int3(a.x * b, a.y * b, a.z * b);
}
inline __host__ __device__ int3 operator*(int b, int3 a)
{
    return make_int3(b * a.x, b * a.y, b * a.z);
}
inline __host__ __device__ void operator*=(int3 &a, int b)
{
    a.x *= b; a.y *= b; a.z *= b;
}

inline __host__ __device__ uint3 operator*(uint3 a, uint3 b)
{
    return make_uint3(a.x * b.x, a.y * b.y, a.z * b.z);
}
inline __host__ __device__ void operator*=(uint3 &a, uint3 b)
{
    a.x *= b.x; a.y *= b.y; a.z *= b.z;
}
inline __host__ __device__ uint3 operator*(uint3 a, uint b)
{
    return make_uint3(a.x * b, a.y * b, a.z * b);
}
inline __host__ __device__ uint3 operator*(uint b, uint3 a)
{
    return make_uint3(b * a.x, b * a.y, b * a.z);
}
inline __host__ __device__ void operator*=(uint3 &a, uint b)
{
    a.x *= b; a.y *= b; a.z *= b;
}

inline __host__ __device__ float4 operator*(float4 a, float4 b)
{
    return make_float4(a.x * b.x, a.y * b.y, a.z * b.z,  a.w * b.w);
}
inline __host__ __device__ void operator*=(float4 &a, float4 b)
{
    a.x *= b.x; a.y *= b.y; a.z *= b.z; a.w *= b.w;
}
inline __host__ __device__ float4 operator*(float4 a, float b)
{
    return make_float4(a.x * b, a.y * b, a.z * b,  a.w * b);
}
inline __host__ __device__ float4 operator*(float b, float4 a)
{
    return make_float4(b * a.x, b * a.y, b * a.z, b * a.w);
}
inline __host__ __device__ void operator*=(float4 &a, float b)
{
    a.x *= b; a.y *= b; a.z *= b; a.w *= b;
}

inline __host__ __device__ int4 operator*(int4 a, int4 b)
{
    return make_int4(a.x * b.x, a.y * b.y, a.z * b.z,  a.w * b.w);
}
inline __host__ __device__ void operator*=(int4 &a, int4 b)
{
    a.x *= b.x; a.y *= b.y; a.z *= b.z; a.w *= b.w;
}
inline __host__ __device__ int4 operator*(int4 a, int b)
{
    return make_int4(a.x * b, a.y * b, a.z * b,  a.w * b);
}
inline __host__ __device__ int4 operator*(int b, int4 a)
{
    return make_int4(b * a.x, b * a.y, b * a.z, b * a.w);
}
inline __host__ __device__ void operator*=(int4 &a, int b)
{
    a.x *= b; a.y *= b; a.z *= b; a.w *= b;
}

inline __host__ __device__ uint4 operator*(uint4 a, uint4 b)
{
    return make_uint4(a.x * b.x, a.y * b.y, a.z * b.z,  a.w * b.w);
}
inline __host__ __device__ void operator*=(uint4 &a, uint4 b)
{
    a.x *= b.x; a.y *= b.y; a.z *= b.z; a.w *= b.w;
}
inline __host__ __device__ uint4 operator*(uint4 a, uint b)
{
    return make_uint4(a.x * b, a.y * b, a.z * b,  a.w * b);
}
inline __host__ __device__ uint4 operator*(uint b, uint4 a)
{
    return make_uint4(b * a.x, b * a.y, b * a.z, b * a.w);
}
inline __host__ __device__ void operator*=(uint4 &a, uint b)
{
    a.x *= b; a.y *= b; a.z *= b; a.w *= b;
}

////////////////////////////////////////////////////////////////////////////////
// divide
////////////////////////////////////////////////////////////////////////////////

inline __host__ __device__ float2 operator/(float2 a, float2 b)
{
    return make_float2(a.x / b.x, a.y / b.y);
}
inline __host__ __device__ void operator/=(float2 &a, float2 b)
{
    a.x /= b.x; a.y /= b.y;
}
inline __host__ __device__ float2 operator/(float2 a, float b)
{
    return make_float2(a.x / b, a.y / b);
}
inline __host__ __device__ void operator/=(float2 &a, float b)
{
    a.x /= b; a.y /= b;
}
inline __host__ __device__ float2 operator/(float b, float2 a)
{
    return make_float2(b / a.x, b / a.y);
}

inline __host__ __device__ float3 operator/(float3 a, float3 b)
{
    return make_float3(a.x / b.x, a.y / b.y, a.z / b.z);
}
inline __host__ __device__ void operator/=(float3 &a, float3 b)
{
    a.x /= b.x; a.y /= b.y; a.z /= b.z;
}
inline __host__ __device__ float3 operator/(float3 a, float b)
{
    return make_float3(a.x / b, a.y / b, a.z / b);
}
inline __host__ __device__ void operator/=(float3 &a, float b)
{
    a.x /= b; a.y /= b; a.z /= b;
}
inline __host__ __device__ float3 operator/(float b, float3 a)
{
    return make_float3(b / a.x, b / a.y, b / a.z);
}

inline __host__ __device__ float4 operator/(float4 a, float4 b)
{
    return make_float4(a.x / b.x, a.y / b.y, a.z / b.z,  a.w / b.w);
}
inline __host__ __device__ void operator/=(float4 &a, float4 b)
{
    a.x /= b.x; a.y /= b.y; a.z /= b.z; a.w /= b.w;
}
inline __host__ __device__ float4 operator/(float4 a, float b)
{
    return make_float4(a.x / b, a.y / b, a.z / b,  a.w / b);
}
inline __host__ __device__ void operator/=(float4 &a, float b)
{
    a.x /= b; a.y /= b; a.z /= b; a.w /= b;
}
inline __host__ __device__ float4 operator/(float b, float4 a){
    return make_float4(b / a.x, b / a.y, b / a.z, b / a.w);
}

////////////////////////////////////////////////////////////////////////////////
// min
////////////////////////////////////////////////////////////////////////////////

inline  __host__ __device__ float2 fminf(float2 a, float2 b)
{
	return make_float2(fminf(a.x,b.x), fminf(a.y,b.y));
}
inline __host__ __device__ float3 fminf(float3 a, float3 b)
{
	return make_float3(fminf(a.x,b.x), fminf(a.y,b.y), fminf(a.z,b.z));
}
inline  __host__ __device__ float4 fminf(float4 a, float4 b)
{
	return make_float4(fminf(a.x,b.x), fminf(a.y,b.y), fminf(a.z,b.z), fminf(a.w,b.w));
}

inline __host__ __device__ int2 min(int2 a, int2 b)
{
    return make_int2(min(a.x,b.x), min(a.y,b.y));
}
inline __host__ __device__ int3 min(int3 a, int3 b)
{
    return make_int3(min(a.x,b.x), min(a.y,b.y), min(a.z,b.z));
}
inline __host__ __device__ int4 min(int4 a, int4 b)
{
    return make_int4(min(a.x,b.x), min(a.y,b.y), min(a.z,b.z), min(a.w,b.w));
}

inline __host__ __device__ uint2 min(uint2 a, uint2 b)
{
    return make_uint2(min(a.x,b.x), min(a.y,b.y));
}
inline __host__ __device__ uint3 min(uint3 a, uint3 b)
{
    return make_uint3(min(a.x,b.x), min(a.y,b.y), min(a.z,b.z));
}
inline __host__ __device__ uint4 min(uint4 a, uint4 b)
{
    return make_uint4(min(a.x,b.x), min(a.y,b.y), min(a.z,b.z), min(a.w,b.w));
}

////////////////////////////////////////////////////////////////////////////////
// max
////////////////////////////////////////////////////////////////////////////////

inline __host__ __device__ float2 fmaxf(float2 a, float2 b)
{
	return make_float2(fmaxf(a.x,b.x), fmaxf(a.y,b.y));
}
inline __host__ __device__ float3 fmaxf(float3 a, float3 b)
{
	return make_float3(fmaxf(a.x,b.x), fmaxf(a.y,b.y), fmaxf(a.z,b.z));
}
inline __host__ __device__ float4 fmaxf(float4 a, float4 b)
{
	return make_float4(fmaxf(a.x,b.x), fmaxf(a.y,b.y), fmaxf(a.z,b.z), fmaxf(a.w,b.w));
}

inline __host__ __device__ int2 max(int2 a, int2 b)
{
    return make_int2(max(a.x,b.x), max(a.y,b.y));
}
inline __host__ __device__ int3 max(int3 a, int3 b)
{
    return make_int3(max(a.x,b.x), max(a.y,b.y), max(a.z,b.z));
}
inline __host__ __device__ int4 max(int4 a, int4 b)
{
    return make_int4(max(a.x,b.x), max(a.y,b.y), max(a.z,b.z), max(a.w,b.w));
}

inline __host__ __device__ uint2 max(uint2 a, uint2 b)
{
    return make_uint2(max(a.x,b.x), max(a.y,b.y));
}
inline __host__ __device__ uint3 max(uint3 a, uint3 b)
{
    return make_uint3(max(a.x,b.x), max(a.y,b.y), max(a.z,b.z));
}
inline __host__ __device__ uint4 max(uint4 a, uint4 b)
{
    return make_uint4(max(a.x,b.x), max(a.y,b.y), max(a.z,b.z), max(a.w,b.w));
}

////////////////////////////////////////////////////////////////////////////////
// lerp
// - linear interpolation between a and b, based on value t in [0, 1] range
////////////////////////////////////////////////////////////////////////////////

inline __device__ __host__ float lerp(float a, float b, float t)
{
    return a + t*(b-a);
}
inline __device__ __host__ float2 lerp(float2 a, float2 b, float t)
{
    return a + t*(b-a);
}
inline __device__ __host__ float3 lerp(float3 a, float3 b, float t)
{
    return a + t*(b-a);
}
inline __device__ __host__ float4 lerp(float4 a, float4 b, float t)
{
    return a + t*(b-a);
}

////////////////////////////////////////////////////////////////////////////////
// clamp
// - clamp the value v to be in the range [a, b]
////////////////////////////////////////////////////////////////////////////////

inline __device__ __host__ float clamp(float f, float a, float b)
{
    return fmaxf(a, fminf(f, b));
}
inline __device__ __host__ int clamp(int f, int a, int b)
{
    return max(a, min(f, b));
}
inline __device__ __host__ uint clamp(uint f, uint a, uint b)
{
    return max(a, min(f, b));
}

inline __device__ __host__ float2 clamp(float2 v, float a, float b)
{
    return make_float2(clamp(v.x, a, b), clamp(v.y, a, b));
}
inline __device__ __host__ float2 clamp(float2 v, float2 a, float2 b)
{
    return make_float2(clamp(v.x, a.x, b.x), clamp(v.y, a.y, b.y));
}
inline __device__ __host__ float3 clamp(float3 v, float a, float b)
{
    return make_float3(clamp(v.x, a, b), clamp(v.y, a, b), clamp(v.z, a, b));
}
inline __device__ __host__ float3 clamp(float3 v, float3 a, float3 b)
{
    return make_float3(clamp(v.x, a.x, b.x), clamp(v.y, a.y, b.y), clamp(v.z, a.z, b.z));
}
inline __device__ __host__ float4 clamp(float4 v, float a, float b)
{
    return make_float4(clamp(v.x, a, b), clamp(v.y, a, b), clamp(v.z, a, b), clamp(v.w, a, b));
}
inline __device__ __host__ float4 clamp(float4 v, float4 a, float4 b)
{
    return make_float4(clamp(v.x, a.x, b.x), clamp(v.y, a.y, b.y), clamp(v.z, a.z, b.z), clamp(v.w, a.w, b.w));
}

inline __device__ __host__ int2 clamp(int2 v, int a, int b)
{
    return make_int2(clamp(v.x, a, b), clamp(v.y, a, b));
}
inline __device__ __host__ int2 clamp(int2 v, int2 a, int2 b)
{
    return make_int2(clamp(v.x, a.x, b.x), clamp(v.y, a.y, b.y));
}
inline __device__ __host__ int3 clamp(int3 v, int a, int b)
{
    return make_int3(clamp(v.x, a, b), clamp(v.y, a, b), clamp(v.z, a, b));
}
inline __device__ __host__ int3 clamp(int3 v, int3 a, int3 b)
{
    return make_int3(clamp(v.x, a.x, b.x), clamp(v.y, a.y, b.y), clamp(v.z, a.z, b.z));
}
inline __device__ __host__ int4 clamp(int4 v, int a, int b)
{
    return make_int4(clamp(v.x, a, b), clamp(v.y, a, b), clamp(v.z, a, b), clamp(v.w, a, b));
}
inline __device__ __host__ int4 clamp(int4 v, int4 a, int4 b)
{
    return make_int4(clamp(v.x, a.x, b.x), clamp(v.y, a.y, b.y), clamp(v.z, a.z, b.z), clamp(v.w, a.w, b.w));
}

inline __device__ __host__ uint2 clamp(uint2 v, uint a, uint b)
{
    return make_uint2(clamp(v.x, a, b), clamp(v.y, a, b));
}
inline __device__ __host__ uint2 clamp(uint2 v, uint2 a, uint2 b)
{
    return make_uint2(clamp(v.x, a.x, b.x), clamp(v.y, a.y, b.y));
}
inline __device__ __host__ uint3 clamp(uint3 v, uint a, uint b)
{
    return make_uint3(clamp(v.x, a, b), clamp(v.y, a, b), clamp(v.z, a, b));
}
inline __device__ __host__ uint3 clamp(uint3 v, uint3 a, uint3 b)
{
    return make_uint3(clamp(v.x, a.x, b.x), clamp(v.y, a.y, b.y), clamp(v.z, a.z, b.z));
}
inline __device__ __host__ uint4 clamp(uint4 v, uint a, uint b)
{
    return make_uint4(clamp(v.x, a, b), clamp(v.y, a, b), clamp(v.z, a, b), clamp(v.w, a, b));
}
inline __device__ __host__ uint4 clamp(uint4 v, uint4 a, uint4 b)
{
    return make_uint4(clamp(v.x, a.x, b.x), clamp(v.y, a.y, b.y), clamp(v.z, a.z, b.z), clamp(v.w, a.w, b.w));
}

////////////////////////////////////////////////////////////////////////////////
// dot product
////////////////////////////////////////////////////////////////////////////////

inline __host__ __device__ float dot(float2 a, float2 b)
{
    return a.x * b.x + a.y * b.y;
}
inline __host__ __device__ float dot(float3 a, float3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
inline __host__ __device__ float dot(float4 a, float4 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline __host__ __device__ int dot(int2 a, int2 b)
{
    return a.x * b.x + a.y * b.y;
}
inline __host__ __device__ int dot(int3 a, int3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
inline __host__ __device__ int dot(int4 a, int4 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline __host__ __device__ uint dot(uint2 a, uint2 b)
{
    return a.x * b.x + a.y * b.y;
}
inline __host__ __device__ uint dot(uint3 a, uint3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
inline __host__ __device__ uint dot(uint4 a, uint4 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

////////////////////////////////////////////////////////////////////////////////
// length
////////////////////////////////////////////////////////////////////////////////

inline __host__ __device__ float length(float2 v)
{
    return sqrtf(dot(v, v));
}
inline __host__ __device__ float length(float3 v)
{
    return sqrtf(dot(v, v));
}
inline __host__ __device__ float length(float4 v)
{
    return sqrtf(dot(v, v));
}

////////////////////////////////////////////////////////////////////////////////
// normalize
////////////////////////////////////////////////////////////////////////////////

inline __host__ __device__ float2 normalize(float2 v)
{
    float invLen = rsqrtf(dot(v, v));
    return v * invLen;
}
inline __host__ __device__ float3 normalize(float3 v)
{
    float invLen = rsqrtf(dot(v, v));
    return v * invLen;
}
inline __host__ __device__ float4 normalize(float4 v)
{
    float invLen = rsqrtf(dot(v, v));
    return v * invLen;
}

////////////////////////////////////////////////////////////////////////////////
// floor
////////////////////////////////////////////////////////////////////////////////

inline __host__ __device__ float2 floorf(float2 v)
{
    return make_float2(floorf(v.x), floorf(v.y));
}
inline __host__ __device__ float3 floorf(float3 v)
{
    return make_float3(floorf(v.x), floorf(v.y), floorf(v.z));
}
inline __host__ __device__ float4 floorf(float4 v)
{
    return make_float4(floorf(v.x), floorf(v.y), floorf(v.z), floorf(v.w));
}

////////////////////////////////////////////////////////////////////////////////
// frac - returns the fractional portion of a scalar or each vector component
////////////////////////////////////////////////////////////////////////////////

inline __host__ __device__ float fracf(float v)
{
    return v - floorf(v);
}
inline __host__ __device__ float2 fracf(float2 v)
{
    return make_float2(fracf(v.x), fracf(v.y));
}
inline __host__ __device__ float3 fracf(float3 v)
{
    return make_float3(fracf(v.x), fracf(v.y), fracf(v.z));
}
inline __host__ __device__ float4 fracf(float4 v)
{
    return make_float4(fracf(v.x), fracf(v.y), fracf(v.z), fracf(v.w));
}

////////////////////////////////////////////////////////////////////////////////
// fmod
////////////////////////////////////////////////////////////////////////////////

inline __host__ __device__ float2 fmodf(float2 a, float2 b)
{
    return make_float2(fmodf(a.x, b.x), fmodf(a.y, b.y));
}
inline __host__ __device__ float3 fmodf(float3 a, float3 b)
{
    return make_float3(fmodf(a.x, b.x), fmodf(a.y, b.y), fmodf(a.z, b.z));
}
inline __host__ __device__ float4 fmodf(float4 a, float4 b)
{
    return make_float4(fmodf(a.x, b.x), fmodf(a.y, b.y), fmodf(a.z, b.z), fmodf(a.w, b.w));
}

////////////////////////////////////////////////////////////////////////////////
// absolute value
////////////////////////////////////////////////////////////////////////////////

inline __host__ __device__ float2 fabs(float2 v)
{
	return make_float2(fabs(v.x), fabs(v.y));
}
inline __host__ __device__ float3 fabs(float3 v)
{
	return make_float3(fabs(v.x), fabs(v.y), fabs(v.z));
}
inline __host__ __device__ float4 fabs(float4 v)
{
	return make_float4(fabs(v.x), fabs(v.y), fabs(v.z), fabs(v.w));
}

inline __host__ __device__ int2 abs(int2 v)
{
	return make_int2(abs(v.x), abs(v.y));
}
inline __host__ __device__ int3 abs(int3 v)
{
	return make_int3(abs(v.x), abs(v.y), abs(v.z));
}
inline __host__ __device__ int4 abs(int4 v)
{
	return make_int4(abs(v.x), abs(v.y), abs(v.z), abs(v.w));
}

////////////////////////////////////////////////////////////////////////////////
// reflect
// - returns reflection of incident ray I around surface normal N
// - N should be normalized, reflected vector's length is equal to length of I
////////////////////////////////////////////////////////////////////////////////

inline __host__ __device__ float3 reflect(float3 i, float3 n)
{
	return i - 2.0f * n * dot(n,i);
}

////////////////////////////////////////////////////////////////////////////////
// cross product
////////////////////////////////////////////////////////////////////////////////

inline __host__ __device__ float3 cross(float3 a, float3 b)
{
    return make_float3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}

////////////////////////////////////////////////////////////////////////////////
// smoothstep
// - returns 0 if x < a
// - returns 1 if x > b
// - otherwise returns smooth interpolation between 0 and 1 based on x
////////////////////////////////////////////////////////////////////////////////

inline __device__ __host__ float smoothstep(float a, float b, float x)
{
	float y = clamp((x - a) / (b - a), 0.0f, 1.0f);
	return (y*y*(3.0f - (2.0f*y)));
}
inline __device__ __host__ float2 smoothstep(float2 a, float2 b, float2 x)
{
	float2 y = clamp((x - a) / (b - a), 0.0f, 1.0f);
	return (y*y*(make_float2(3.0f) - (make_float2(2.0f)*y)));
}
inline __device__ __host__ float3 smoothstep(float3 a, float3 b, float3 x)
{
	float3 y = clamp((x - a) / (b - a), 0.0f, 1.0f);
	return (y*y*(make_float3(3.0f) - (make_float3(2.0f)*y)));
}
inline __device__ __host__ float4 smoothstep(float4 a, float4 b, float4 x)
{
	float4 y = clamp((x - a) / (b - a), 0.0f, 1.0f);
	return (y*y*(make_float4(3.0f) - (make_float4(2.0f)*y)));
}

#endif /* CUTIL_MATH_H */


#ifndef VOXELCOORDS_H_
#define VOXELCOORDS_H_

namespace math {

MATH_FUNCTION static inline float4x4 translationMatrix(const float3& dst)
{
	return float4x4(1.0f, 0.0f, 0.0f, dst.x,
			0.0f, 1.0f, 0.0f, dst.y,
			0.0f, 0.0f, 1.0f, dst.z,
			0.0f, 0.0f, 0.0f, 1.0f);
}

MATH_FUNCTION static inline float4x4 scale(const float3& scale)
{
	float4x4 M(scale.x, 0.0f, 0.0f, 0.0f,
			0.0f, scale.y, 0.0f, 0.0f,
			0.0f, 0.0f, scale.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	return M;
}

struct VoxelCoords {
private:
	int3	  		_voxels;
	float3  		_bb_min;
	float3  		_bb_max;
	float3			_voxel_size;
	float4x4		_m;
	float4x4		_m_inv;
	float3 			_translation;
	float3 			_scale;

public:

	MATH_FUNCTION bool check(const int3& coords) {
		if (coords.x < 0 || coords.x >= _voxels.x || coords.y < 0 || coords.y >= _voxels.y || coords.z < 0 || coords.z >= _voxels.z) {
			return false;
		}
		return true;
	}

	MATH_FUNCTION int toIndex(const int3& coords)
	{
		return coords.x + _voxels.x * coords.y + _voxels.y * _voxels.x * coords.z;
	}

	MATH_FUNCTION int calcNumVoxels(const int3& size)
	{
		return ((size.x) * (size.y) * (size.z));
	}

	MATH_FUNCTION int3 getVoxelCoordsInsideBox(const unsigned int& index, const int3& size)
	{
		int3 coord(0, 0, 0);

		coord.x = index % (size.x);
		coord.y = (index / (size.x)) % (size.y);
		coord.z = (index / ((size.x) * (size.y)));

		return coord;
	}


	MATH_FUNCTION int3 toVoxel(const float3& coords)
	{
		float4 x(coords.x, coords.y, coords.z, 1.0f);
		x = _m * x;
		return int3(floor((x.x / x.w) + 0.5f), floor((x.y / x.w) + 0.5f), floor((x.z / x.w) + 0.5f));
	}

	MATH_FUNCTION float3 toWorld(const int3& coords)
	{
		float4 x(coords.x, coords.y, coords.z, 1.0f);
		x = _m_inv * x;
		return float3(x.x / x.w, x.y / x.w, x.z / x.w);
	}

	MATH_FUNCTION float3 voxelSize() {
		return _voxel_size;
	}


	MATH_FUNCTION int3& voxelPerDimension() {
		return _voxels;
	}

	MATH_FUNCTION int numVoxels() {
		return _voxels.x * _voxels.y * _voxels.z;
	}

	MATH_FUNCTION void init(const float3& bb_min, const float3& bb_max, const int3& voxel_per_dimension)
	{
		_bb_min = bb_min;
		_bb_max = bb_max;
		_voxels = voxel_per_dimension;

		_voxel_size = float3(	(bb_max.x - bb_min.x) / static_cast<float>(voxel_per_dimension.x),
									(bb_max.y - bb_min.y) / static_cast<float>(voxel_per_dimension.y),
									(bb_max.z - bb_min.z) / static_cast<float>(voxel_per_dimension.z));

		float3 translation = (bb_min * -1.0f) - _voxel_size / 2.0f;
		float3 scaling(1.0f/_voxel_size.x, 1.0f/_voxel_size.y, 1.0f/_voxel_size.z);


		float4x4 scale_w2v(scale(scaling));
		float4x4 translate_w2v(translationMatrix(translation));

		_m = scale_w2v * translate_w2v;

		float4x4 scale_v2w(scale(_voxel_size));
		float4x4 translate_v2w(translationMatrix(-1.0f * translation));

		_m_inv = translate_v2w * scale_v2w;
	}
};

}  // namespace PGA



#endif /* GPUPROCGENETICS_SRC_EVALUATION_GPU_VOXELCOORDS_H_ */


