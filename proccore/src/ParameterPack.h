#ifndef INCLUDED_PARAMETER_PACK
#define INCLUDED_PARAMETER_PACK

#pragma once

#include "DualBuild.h"

//splits off parameters from the parameter pack and calls the ::call function from Caller with that parameters
// a positive SplitOff value takes parameters from the front
// a negative SplitOff value takes parameters from the back
template<int SplitOff, class ... AllParams>
struct SplitParameterPack
{
	template<class Caller>
	DUAL_BUILD_FUNCTION static void call(Caller& caller);

	template<class Caller>
	DUAL_BUILD_FUNCTION static void callWithParams(Caller& caller, AllParams&... params);
};



template<bool SplitMore, bool Front, int SplitOff, class ... AllParams>
struct SplitParameterPackImpl;

//{
//	template<class Caller, class ... Splitoff >
//	DUAL_BUILD_FUNCTION static void call(Caller& caller);
//
//	template<class Caller, class ... Splitoff>
//	DUAL_BUILD_FUNCTION static void callWithParams(Caller& caller, Splitoff&... splitparams, AllParams&... remainingparams);
//};

template<bool Front, int SplitOff, class OneParam, class ... RemainingParams>
struct SplitParameterPackImpl<true, Front, SplitOff, OneParam, RemainingParams...>
{
	template<class Caller, class ... Splitoff >
	DUAL_BUILD_FUNCTION static void call(Caller& caller)
	{
		SplitParameterPackImpl<(SplitOff - 1 > 0), Front, SplitOff - 1, RemainingParams...> :: template call <Caller, Splitoff..., OneParam>(caller);
	}

	template<class Caller, class ... Splitoff>
	DUAL_BUILD_FUNCTION static void callWithParams(Caller& caller, Splitoff&... splitparams, OneParam& onparam, RemainingParams&... remainingparams)
	{
		SplitParameterPackImpl<(SplitOff - 1 > 0), Front, SplitOff - 1, RemainingParams...> :: template callWithParams <Caller, Splitoff..., OneParam>(caller, splitparams..., onparam, remainingparams...);
	}
};

template<class ... RemainingParams>
struct SplitParameterPackImpl<false, true, 0, RemainingParams...>
{
	template<class Caller, class ... Splitoff >
	DUAL_BUILD_FUNCTION static void call(Caller& caller)
	{
		caller.template call<Splitoff...>();
	}

	template<class Caller, class ... Splitoff>
	DUAL_BUILD_FUNCTION static void callWithParams(Caller& caller, Splitoff&... splitparams, RemainingParams&... remainingparams)
	{
		caller.template call<Splitoff...>(splitparams...);
	}
};

template<class ... RemainingParams>
struct SplitParameterPackImpl<false, false, 0, RemainingParams...>
{
	template<class Caller, class ... Splitoff >
	DUAL_BUILD_FUNCTION static void call(Caller& caller)
	{
		caller.template call<RemainingParams...>();
	}

	template<class Caller, class ... Splitoff>
	DUAL_BUILD_FUNCTION static void callWithParams(Caller& caller, Splitoff&... splitparams, RemainingParams&... remainingparams)
	{
		caller.template call<RemainingParams...>(remainingparams...);
	}
};




template<int SplitOff, class ... AllParams>
template<class Caller>
DUAL_BUILD_FUNCTION void SplitParameterPack<SplitOff, AllParams...>::call(Caller& caller)
{
	SplitParameterPackImpl<SplitOff != 0, (SplitOff >= 0 ? SplitOff : -SplitOff), (SplitOff >= 0 ? SplitOff : (static_cast<int>(sizeof...(AllParams)) + SplitOff)), AllParams...>::call(caller);
}


template<int SplitOff, class ... AllParams>
template<class Caller>
DUAL_BUILD_FUNCTION void SplitParameterPack<SplitOff, AllParams...>::callWithParams(Caller& caller, AllParams&... params)
{
	SplitParameterPackImpl<SplitOff != 0, (SplitOff >= 0 ? SplitOff : -SplitOff), (SplitOff >= 0 ? SplitOff : sizeof...(AllParams)+SplitOff), AllParams...>::call(caller, params...);
}


#endif 