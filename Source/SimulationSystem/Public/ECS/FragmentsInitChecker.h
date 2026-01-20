#pragma once

template<typename... Ts>
struct TTypeList {};

template<typename T, typename List>
struct TIsInTypeList;
 
template<typename T, typename... Ts>
struct TIsInTypeList<T, TTypeList<Ts...>>
{
	static constexpr bool Value = (std::is_same_v<T, Ts> || ...);
};