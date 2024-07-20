template<class T, size_t U>
ObjectPool<T, U>::ObjectPool() noexcept:
	m_lastUsedIndex(0)
{
	
}

template<class T, size_t U>
T* ObjectPool<T, U>::findUnusedObject() noexcept
{
	if(m_pool.all())
		return nullptr;

//  Search from last used object, this will usually return almost instantly
	for (size_t i = m_lastUsedIndex; i < m_pool.size(); ++i)
	{
		if(!m_pool[i])
		{
			m_pool.set(i);
			m_lastUsedIndex = i;

			return (m_objects.data() + i);
		}	
	}

//  Otherwise, do a linear search
	for (size_t i = 0; i < m_lastUsedIndex; ++i)
	{
		if(!m_pool[i])
		{
			m_pool.set(i);
			m_lastUsedIndex = i;

			return (m_objects.data() + i);
		}
	}

//  No free objects	
	m_lastUsedIndex = 0;

	return nullptr;
}

template<class T, size_t U>
void ObjectPool<T, U>::returnObjectBack(const T* objectPtr) noexcept
{
	const auto beginPtr = m_objects.data();
	size_t index = static_cast<size_t>(objectPtr - beginPtr);

	if(index < m_objects.size())
	{
		if(objectPtr == (beginPtr + index))
		{
			m_pool.reset(index);
		}
	}
}

template<class T, size_t U>
std::vector<T*> ObjectPool<T, U>::getOccupiedObjects() noexcept
{
	std::vector<T*> objects;
	objects.reserve(m_pool.count());
	auto beginPtr = m_objects.data();

	for (size_t i = 0; i < m_pool.size(); ++i)
	{
		if(m_pool[i])
			objects.push_back(beginPtr + i);
	}
	
	return objects;
}