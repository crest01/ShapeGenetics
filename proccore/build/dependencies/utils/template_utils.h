#ifndef TEMPLATE_UTILS_INCLUDED
#define TEMPLATE_UTILS_INCLUDED

#pragma once

#include <type_traits>

using std::enable_if;
using std::result_of;
using std::declval;

template<class Value>
struct not_value
{
	static const bool value = Value::value ? false : true;
};


template<class ... Conditions>
struct static_logical;

template<class FinalCondition>
struct static_logical<FinalCondition>
{
	static const bool all = FinalCondition::value;
	static const bool any = FinalCondition::value;
};

template<class OneCondition, class ... OtherConditions>
struct static_logical<OneCondition, OtherConditions...>
{
	static const bool all = OneCondition::value && static_logical<OtherConditions...>::all;
	static const bool any = OneCondition::value || static_logical<OtherConditions...>::all;
};

template<class ... Values>
struct static_equal_values;

template<class OneValue, class AnotherValue,  class ... OtherValues>
struct static_equal_values<OneValue, AnotherValue, OtherValues...>
{
	static const bool value = OneValue::value == AnotherValue::value && static_equal_values<AnotherValue, OtherValues...>::value;
};

template<class OneValue, class AnotherValue>
struct static_equal_values<OneValue, AnotherValue>
{
	static const bool value = OneValue::value == AnotherValue::value;
};


template<typename T, T ... Values>
struct static_equals;

template<typename T, T OneValue, T AnotherValue, T ... OtherValues>
struct static_equals<T, OneValue, AnotherValue, OtherValues...>
{
	static const bool value = OneValue == AnotherValue && static_equals<T, AnotherValue, OtherValues...>::value;
};

template<typename T, T OneValue, T AnotherValue>
struct static_equals<T, OneValue, AnotherValue>
{
	static const bool value = OneValue == AnotherValue;
};


template<class ... Types>
struct type_equals
{
	static const bool value = false;
};

template<class OneType, class ... OtherTypes>
struct type_equals<OneType, OneType, OtherTypes...>
{
	static const bool value = type_equals<OneType, OtherTypes...>::value;
};

template<class OneType>
struct type_equals<OneType, OneType>
{
	static const bool value = true;
};







#endif //TEMPLATE_UTILS_INCLUDED
