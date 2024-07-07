template<class T, size_t U>
ObjectPool<T, U>::ObjectPool() noexcept
{
	std::memset(m_objects.data(), 0, sizeof(m_objects));
}

template<class T, size_t U>
T* ObjectPool<T, U>::findUnusedObject() noexcept
{
//  Search from last used object, this will usually return almost instantly
	for (size_t i = m_lastUsedIndex; i < m_objects.size(); ++i)
	{
		if( ! m_objects[i].isEnabled)
		{
			m_lastUsedIndex = i;

			return &m_objects[i].object; 
		}
	}
//  Otherwise, do a linear search
	for (size_t i = 0; i < m_lastUsedIndex; ++i)
	{
		if( ! m_objects[i].isEnabled)
		{
			m_lastUsedIndex = i;

			return &m_objects[i].object; 
		}
	}
//  No free objects	
	m_lastUsedIndex = 0;

	return nullptr;
}

template<class T, size_t U>
void ObjectPool<T, U>::returnObjectBack(const T* object) noexcept
{
	for (auto& element : m_objects)
	{
		if(object == &element.object)
		{
			element.isEnabled = false;
			break;
		}
	}
}