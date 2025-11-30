

template<class T, size_t N>
std::array<GLuint, N> GlResourceManager::create() noexcept
{
    if constexpr(std::is_same_v<T, GLBuffer>)
    {
        return createResources<N>(m_buffers, glGenBuffers);
    }
    else if constexpr(std::is_same_v<T, VertexArrayObject>)
    {
        return createResources<N>(m_arrays, glGenVertexArrays);
    }
    else if constexpr(std::is_same_v<T, Texture>)
    {
        return createResources<N>(m_textures, glGenTextures);
    }
}


template<class T>
void GlResourceManager::destroy(const std::span<GLuint> objects) noexcept
{
    if constexpr(std::is_same_v<T, GLBuffer>)
    {
        destroyResources(objects, m_buffers, glDeleteBuffers);
    }
    else if constexpr(std::is_same_v<T, VertexArrayObject>)
    {
        destroyResources(objects, m_arrays, glDeleteVertexArrays);
    }
    else if constexpr(std::is_same_v<T, Texture>)
    {
        destroyResources(objects, m_textures, glDeleteTextures);
    }
}


template<size_t N>
std::array<GLuint, N> GlResourceManager::createResources(std::vector<GLuint>& handles, void(*func)(GLint, GLuint*)) noexcept
{
    std::array<GLuint, N> objects;

    func(static_cast<GLint>(N), objects.data());
    handles.insert(handles.end(), objects.begin(), objects.end());

    return objects;
}