#pragma once

#include <concepts>
#include <cstddef>

template <std::size_t Index, typename FirstType, typename... OtherTypes>
struct nth_index
{
	using type = nth_index<Index - 1, OtherTypes...>::type;
};

template <typename FirstType, typename... OtherTypes>
struct nth_index<0, FirstType, OtherTypes...>
{
	using type = FirstType;
};

template <std::size_t Index, typename... Ts>
using nth_index_t = nth_index<Index, Ts...>::type;

template <typename SearchType, typename FirstType, typename... OtherTypes>
struct index_of
{
	static constexpr std::size_t value = std::same_as<SearchType, FirstType>
										? 0
										: (1 + index_of<SearchType, OtherTypes...>::value);
};

template <typename SearchType, typename FirstType>
struct index_of<SearchType, FirstType>
{
	static constexpr std::size_t value = std::same_as<SearchType, FirstType> ? 0 : 1;
};

template <typename SearchType, typename... OtherTypes>
constexpr std::size_t index_of_v = index_of<SearchType, OtherTypes...>::value;

template <typename SearchType, typename FirstType, typename... OtherTypes>
struct contains
{
	static constexpr bool value = std::same_as<SearchType, FirstType>
								|| contains<SearchType, OtherTypes...>::value;
};

template <typename SearchType, typename FirstType>
struct contains<SearchType, FirstType>
{
	static constexpr bool value = std::same_as<SearchType, FirstType>;
};

template <typename SearchType, typename... Ts>
constexpr bool contains_v = contains<SearchType, Ts...>::value;
