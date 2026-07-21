template<size_t N>
MemoryAllocator<N>::MemoryAllocator() noexcept:
    m_stride(0)
{
    static_assert(N >= 1024, "The memory size must be equal to or greater than 1 kilobyte.");
}


template<size_t N>
MemoryAllocator<N>::~MemoryAllocator() = default;


template<size_t N>
template<class T, class... Args>
T* MemoryAllocator<N>::allocate(Args&&... args) noexcept
{
    auto& freeBlocks = m_freeBlocks[sizeof(T)];

    if(!freeBlocks.empty())
    {
        void* block = freeBlocks.back();
        freeBlocks.pop_back();

        return new (block) T(std::forward<Args>(args)...);
    }

    if (m_stride + sizeof(T) < m_memoryPool.size())
    {
        const size_t offset = m_stride;
        m_stride += sizeof(T);

        return new (m_memoryPool.data() + offset) T(std::forward<Args>(args)...);
    }

    return nullptr;
}


template<size_t N>
template<class T>
void MemoryAllocator<N>::release(T* data) noexcept
{
    auto& freeBlocks = m_freeBlocks[sizeof(T)];
    freeBlocks.push_back(static_cast<void*>(data));
}


template<size_t N>
void MemoryAllocator<N>::release(void* data, size_t size) noexcept
{
    auto& freeBlocks = m_freeBlocks[size];
    freeBlocks.push_back(data);
}