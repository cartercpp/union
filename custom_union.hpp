#pragma once

#include <stdexcept>
#include <algorithm>
#include <utility>
#include <new>
#include <cstddef>
#include "type_set.hpp"
#include "type_traits.hpp"

template <typename... Ts> requires (sizeof...(Ts) > 0)
class custom_union
{
public:

	//CONSTRUCTORS & STUFF

	custom_union(const custom_union<Ts...>&) = delete;
	custom_union(custom_union<Ts...>&&) = delete;
	auto& operator=(const custom_union<Ts...>&) = delete;
	auto& operator=(custom_union<Ts...>&&) = delete;

	custom_union() noexcept = default;

	template <typename ValueType> requires contains_v<ValueType, Ts...>
	custom_union(const ValueType& value)
	{
		ConstructImpl(value);
	}

	template <typename ValueType> requires contains_v<ValueType, Ts...>
	auto& operator=(const ValueType& value)
	{
		if (m_typeSet.index_in_use() != index_of_v<ValueType, Ts...>)
		{
			DestroyImpl<0>();
			ConstructImpl(value);
		}
		else
			AssignImpl(value);

		return *this;
	}

	~custom_union()
	{
		DestroyImpl<0>();
	}

	//METHODS & STUFF

	template <typename ValueType>
	ValueType& get()
	{
		if (m_typeSet.index_in_use() != index_of_v<ValueType, Ts...>)
			throw std::runtime_error{ "Type requested is not in use" };

		return *GetPtr<ValueType>();
	}

	template <typename ValueType>
	const ValueType& get() const
	{
		if (m_typeSet.index_in_use() != index_of_v<ValueType, Ts...>)
			throw std::runtime_error{ "Type requested is not in use" };

		return *GetPtr<ValueType>();
	}

private:

	template <typename ValueType>
	ValueType* GetPtr()
	{
		return std::launder(reinterpret_cast<ValueType*>(m_bytes));
	}

	template <typename ValueType>
	const ValueType* GetPtr() const
	{
		return std::launder(reinterpret_cast<const ValueType*>(m_bytes));
	}

	template <typename ValueType>
	void ConstructImpl(const ValueType& value)
	{
		ValueType* ptr = GetPtr<ValueType>();
		new (ptr) ValueType(value);
		m_typeSet.mark_used<ValueType>();
	}

	template <typename ValueType>
	void AssignImpl(const ValueType& value)
	{
		ValueType* ptr = GetPtr<ValueType>();
		*ptr = value;
	}

	template <std::size_t Index>
	void DestroyImpl()
	{
		if constexpr (Index < sizeof...(Ts))
		{
			if (m_typeSet.index_in_use() == Index)
			{
				using ValueType = nth_index_t<Index, Ts...>;
				ValueType* ptr = GetPtr<ValueType>();
				ptr->~ValueType();
			}
			else
				DestroyImpl<Index + 1>();
		}
	}

	static constexpr std::size_t s_align = std::max({ alignof(Ts)... });
	static constexpr std::size_t s_byteCount = std::max({ sizeof(Ts)... });

	alignas(s_align) std::byte m_bytes[s_byteCount];
	type_set<Ts...> m_typeSet;
};
