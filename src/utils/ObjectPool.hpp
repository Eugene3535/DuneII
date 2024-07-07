#ifndef OBJECT_POOL_HPP
#define OBJECT_POOL_HPP

#include <cstring>
#include <array>

template<class T, size_t U>
class ObjectPool
{
	struct Object
	{
		T    object;
		bool isEnabled = false;
	};

public:
	ObjectPool() noexcept;

	T*   findUnusedObject() noexcept;
	void returnObjectBack(const T* object) noexcept;

private:
	std::array<Object, U> m_objects;
	std::size_t           m_lastUsedIndex;
};

#include "ObjectPool.inl"

#endif