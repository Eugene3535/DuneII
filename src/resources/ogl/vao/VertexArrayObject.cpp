#include "resources/ogl/vao/VertexArrayObject.hpp"


VertexArrayObject::VertexArrayObject() noexcept:
	m_handle(0),
	m_attributeCount(0),
	m_indexCount(0)
{
	
}


VertexArrayObject::VertexArrayObject(GLuint handle) noexcept:
	m_handle(handle),
	m_attributeCount(0),
	m_indexCount(0)
{
	
}


void VertexArrayObject::addVertexBuffer(const GLBuffer& buffer, std::span<const VertexBufferLayout::Attribute> attributes) noexcept
{
	if(m_handle)
	{
		if(buffer.getTarget() == GL_ARRAY_BUFFER)
		{
			const VertexBufferLayout layout(attributes);
	
			glBindVertexArray(m_handle);
	
			for (const auto& attribute : layout.getAttributes())
			{
				glEnableVertexAttribArray(m_attributeCount);
				glBindVertexBuffer(m_attributeCount, buffer.getHandle(), attribute.offset, static_cast<GLuint>(layout.getStride()));
				glVertexAttribFormat(m_attributeCount, static_cast<GLint>(attribute.componentsCount), attribute.componentType, attribute.isNormalized, 0);
				glVertexAttribBinding(m_attributeCount, m_attributeCount);
	
				++m_attributeCount;
			}
	
			glBindVertexArray(0);
		}
	}
}


void VertexArrayObject::setElementBuffer(const GLBuffer& buffer) noexcept
{
	if(m_handle)
	{
		if(buffer.getTarget() == GL_ELEMENT_ARRAY_BUFFER)
		{
			if(GLuint count = buffer.getCount(); count > 0)
			{
				glBindVertexArray(m_handle);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.getHandle());
				m_indexCount = count;
				glBindVertexArray(0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}
		}
	}
}


GLuint VertexArrayObject::getHandle() const noexcept
{
	return m_handle;
}


GLuint VertexArrayObject::getIndexCount() const noexcept
{
	return m_indexCount;
}