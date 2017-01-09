#ifndef INCLUDED_PARAMETERS_PARAMETER_CONVERSION
#define INCLUDED_PARAMETERS_PARAMETER_CONVERSION


#pragma once


namespace _paramConversion {
	constexpr int sign(long double v) {
		return v < 0.0 ? -1 : 1;
	}

	constexpr std::pair<int, long double> expDown(long double v, int ex = 126, long double comp = 0.5)
	{
		return (ex == 0 || v >= comp) ? std::pair<int, long double>(ex, v / comp) : expDown(v, ex - 1, 0.5*comp);
	}
	constexpr std::pair<int, long double> expUp(long double v, int ex = 127, long double comp = 2.0)
	{
		return (ex == 256 || v < comp) ? std::pair<int, long double>(ex, v / (0.5*comp)) : expUp(v, ex + 1, 2.0*comp);
	}

	constexpr std::pair<int, long double> exp(long double v, int ex = 0, long double comp = 1.1754943508222875e-38)
	{
		return v < 1.0 ? expDown(v) : expUp(v);
		//return (ex == 0xFF || v < comp) ? std::pair<int, long double>(ex, v / (0.5*comp)) : exp(v, ex+1, 2.0*comp);
	}
	constexpr int fract(long double v, int res = 0x0, long double comp = 1.5, int resbit = 0x400000)
	{
		return resbit == 0 ? (v >= comp ? res + 1 : res) :
			fract(v >= comp ? v - (comp - 1.0) : v, res | (v >= comp ? resbit : 0), 0.5*(comp - 1.0) + 1.0, resbit >> 1);
	}
	constexpr int combine(int s, std::pair<int, long double> exp_cleaned)
	{
		return ((s < 0) << 31) | (exp_cleaned.first << 23) | fract(exp_cleaned.second);
	}
	constexpr int convertToInt(long double v) {
		return combine(sign(v), exp(sign(v)*v));
	}
}


class FloatRep
{
	int value;
public:
	constexpr FloatRep(int v) : value(v) {}
	constexpr FloatRep operator - () const
	{
		return FloatRep(value ^ 0x80000000);
	}
	constexpr operator int() const
	{
		return value;
	}
};

constexpr FloatRep operator "" _p(long double v)
{
	return FloatRep(_paramConversion::convertToInt(v));
}

constexpr FloatRep operator "" _r(long double v)
{
	return FloatRep(_paramConversion::convertToInt(-v));
}

constexpr FloatRep operator "" _p(unsigned long long v)
{
	return FloatRep(_paramConversion::convertToInt(static_cast<long double>(v)));
}

constexpr FloatRep operator "" _r(unsigned long long v)
{
	return -FloatRep(_paramConversion::convertToInt(static_cast<long double>(v)));
}


#endif