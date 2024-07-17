#ifndef OBJECT_POOL_HPP
#define OBJECT_POOL_HPP

#include <bitset>
#include <array>
#include <iterator>

template<class T, size_t U>
class ObjectPool
{
public:
	ObjectPool() noexcept;

	T*   findUnusedObject() noexcept;
	void returnObjectBack(const T* object) noexcept;

private:
	std::array<T, U> m_objects;
	std::bitset<U>   m_pool;
	size_t           m_lastUsedIndex;
};

#include "ObjectPool.inl"

#endif