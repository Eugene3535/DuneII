#include "resources/gl_interfaces/vao/VertexArrayObject.hpp"


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


void VertexArrayObject::setup(GLuint handle) noexcept
{
	m_handle = handle;
	m_attributeCount = 0;
	m_indexCount = 0;
}


void VertexArrayObject::createVertexInputState(GLuint vao, GLuint vbo, std::span<const VertexBufferLayout::Attribute> attributes) noexcept
{
	const VertexBufferLayout layout(attributes);
	GLuint attributeCount = 0;

	glBindVertexArray(vao);

	for (const auto& attribute : layout.getAttributes())
	{
		glEnableVertexAttribArray(attributeCount);
		glBindVertexBuffer(attributeCount, vbo, attribute.offset, static_cast<GLuint>(layout.getStride()));
		glVertexAttribFormat(attributeCount, static_cast<GLint>(attribute.componentsCount), attribute.componentType, attribute.isNormalized, 0);
		glVertexAttribBinding(attributeCount, attributeCount);

		++attributeCount;
	}

	glBindVertexArray(0);	
}


GLuint VertexArrayObject::getHandle() const noexcept
{
	return m_handle;
}


GLuint VertexArrayObject::getIndexCount() const noexcept
{
	return m_indexCount;
}