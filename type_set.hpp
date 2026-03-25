#pragma once

#include <concepts>
#include <cstddef>

template <typename FirstType, typename... OtherTypes>
class type_set
{
public:

	constexpr type_set() noexcept
		: m_inUse{false}
	{
	}

	template <typename T>
	constexpr void mark_used() noexcept
	{
		if constexpr (std::same_as<T, FirstType>)
		{
			m_inUse = true;
		}
		else
		{
			m_inUse = false;
		}

		m_next.mark_used<T>();
	}

	constexpr std::size_t index_in_use() const noexcept
	{
		return m_inUse ? 0 : (1 + m_next.index_in_use());
	}

private:

	bool m_inUse;
	type_set<OtherTypes...> m_next;
};

template <typename FirstType>
class type_set<FirstType>
{
public:

	constexpr type_set() noexcept
		: m_inUse{ false }
	{
	}

	template <typename T>
	constexpr void mark_used() noexcept
	{
		if constexpr (std::same_as<FirstType, T>)
		{
			m_inUse = true;
		}
		else
		{
			m_inUse = false;
		}
	}

	constexpr std::size_t index_in_use() const noexcept
	{
		return m_inUse ? 0 : 1;
	}

private:

	bool m_inUse;
};
