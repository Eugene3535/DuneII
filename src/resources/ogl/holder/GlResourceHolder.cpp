#include "resources/ogl/holder/GlResourceHolder.hpp"


namespace
{
    std::span<const GLuint> create_resources(GLsizei amount, std::vector<GLuint>* handles, void(*func)(GLsizei, GLuint*)) noexcept
    {
        size_t oldSize = handles->size();
        handles->resize(oldSize + static_cast<size_t>(amount));
        func(amount, handles->data() + oldSize);

        return std::span(handles->data() + oldSize, static_cast<size_t>(amount));
    }

    void destroy_resources(std::span<const GLuint> objects, std::vector<GLuint>* handles, void(*func)(GLsizei, const GLuint*)) noexcept
    {
        if(func)
        {
            if(!objects.empty())
            {
//  Avoid deleting non-existent objects
                std::vector<GLuint> tmp;
                tmp.reserve(objects.size());

                for(GLuint object : objects)
                {
                    for(size_t i = 0; i < handles->size(); ++i)
                    {
                        if(auto& handle = (*handles)[i]; handle == object)
                        {
                            tmp.push_back(object);
                            std::swap(handle, handles->back());
                            handles->pop_back();
                            break;
                        }
                    }
                }

                if(!tmp.empty())
                    func(static_cast<GLsizei>(tmp.size()), tmp.data());
            }
        }
    }
} 


GlResourceHolder::~GlResourceHolder() noexcept
{
    if(!m_buffers.empty())
        glDeleteBuffers(static_cast<GLsizei>(m_buffers.size()), m_buffers.data());

    if(!m_arrays.empty())
        glDeleteVertexArrays(static_cast<GLsizei>(m_arrays.size()), m_arrays.data());

    if(!m_textures.empty())
        glDeleteTextures(static_cast<GLsizei>(m_textures.size()), m_textures.data());
}


std::span<const GLuint> GlResourceHolder::create(GLsizei amount, void(*func)(GLsizei, GLuint*)) noexcept
{
    if(func && amount > 0)
    {
        std::vector<GLuint>* handles = nullptr;

        if(func == glGenBuffers)
            handles = &m_buffers;
            
        if(func == glGenVertexArrays)
            handles = &m_arrays;

        if(func == glGenTextures)
            handles = &m_textures;

        if(handles)
            return create_resources(amount, handles, func);
    }

    return {};
}


void GlResourceHolder::destroy(std::span<const GLuint> buffers, void(*func)(GLsizei, const GLuint*)) noexcept
{
    if( func && ( ! buffers.empty() ) )
    {
        std::vector<GLuint>* handles = nullptr;

        if(func == glDeleteBuffers)
            handles = &m_buffers;
            
        if(func == glDeleteVertexArrays)
            handles = &m_arrays;

        if(func == glDeleteTextures)
            handles = &m_textures;

        if(handles)
            return destroy_resources(buffers, handles, func);
    }
}