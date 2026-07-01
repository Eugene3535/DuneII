#include "graphics/vao/VertexBufferLayout.hpp"


static constexpr GLuint shaderAttributeTypeToComponentCount(const VertexBufferLayout::Attribute::Type type) noexcept
{
    switch (type)
    {
        case VertexBufferLayout::Attribute::Type::Float:
        case VertexBufferLayout::Attribute::Type::Int:
            return 1;

        case VertexBufferLayout::Attribute::Type::Float2:
        case VertexBufferLayout::Attribute::Type::Int2:
            return 2;

        case VertexBufferLayout::Attribute::Type::Float3:
        case VertexBufferLayout::Attribute::Type::Int3:
            return 3;

        case VertexBufferLayout::Attribute::Type::Float4:
        case VertexBufferLayout::Attribute::Type::Int4:
            return 4;
    }

    return 0;
}


static constexpr size_t shaderAttributeTypeSizeOf(const VertexBufferLayout::Attribute::Type type) noexcept
{
    switch (type)
    {
        case VertexBufferLayout::Attribute::Type::Float:
        case VertexBufferLayout::Attribute::Type::Float2:
        case VertexBufferLayout::Attribute::Type::Float3:
        case VertexBufferLayout::Attribute::Type::Float4:
            return sizeof(GLfloat) * shaderAttributeTypeToComponentCount(type);

        case VertexBufferLayout::Attribute::Type::Int:
        case VertexBufferLayout::Attribute::Type::Int2:
        case VertexBufferLayout::Attribute::Type::Int3:
        case VertexBufferLayout::Attribute::Type::Int4:
            return sizeof(GLint) * shaderAttributeTypeToComponentCount(type);
    }

    return 0;
}


static constexpr GLuint shaderAttributeTypeToComponentType(const VertexBufferLayout::Attribute::Type type) noexcept
{
    switch (type)
    {
        case VertexBufferLayout::Attribute::Type::Float:
        case VertexBufferLayout::Attribute::Type::Float2:
        case VertexBufferLayout::Attribute::Type::Float3:
        case VertexBufferLayout::Attribute::Type::Float4:
            return GL_FLOAT;

        case VertexBufferLayout::Attribute::Type::Int:
        case VertexBufferLayout::Attribute::Type::Int2:
        case VertexBufferLayout::Attribute::Type::Int3:
        case VertexBufferLayout::Attribute::Type::Int4:
            return GL_INT;

        case VertexBufferLayout::Attribute::Type::Byte:
        case VertexBufferLayout::Attribute::Type::Byte2:
        case VertexBufferLayout::Attribute::Type::Byte3:
        case VertexBufferLayout::Attribute::Type::Byte4:
            return GL_UNSIGNED_BYTE;
    }

    return GL_FLOAT;
}


VertexBufferLayout::Attribute::Attribute(const Attribute::Type attrType, GLboolean normalized) noexcept:
    type(attrType),
    componentType(shaderAttributeTypeToComponentType(attrType)),
    componentsCount(shaderAttributeTypeToComponentCount(attrType)),
    sizeInBytes(shaderAttributeTypeSizeOf(attrType)),
    offset(0),
    isNormalized(normalized)
{

}


VertexBufferLayout::VertexBufferLayout(std::span<const Attribute> attributes) noexcept :
    m_attributes(attributes.begin(), attributes.end()),
    m_stride(0)
{
    GLuint offset = 0;

    for (auto& attribute : m_attributes)
    {
        attribute.offset = offset;
        offset += attribute.sizeInBytes;
        m_stride += attribute.sizeInBytes;
    }
}


void VertexBufferLayout::createVertexInputState(GLuint vao, GLuint vbo) const noexcept
{
    if (m_attributes.empty())
        return;

	GLuint attributeCount = 0;

	glBindVertexArray(vao);

	for (const auto& attribute : m_attributes)
	{
		glEnableVertexAttribArray(attributeCount);
		glBindVertexBuffer(attributeCount, vbo, attribute.offset, m_stride);
		glVertexAttribFormat(attributeCount, static_cast<GLint>(attribute.componentsCount), attribute.componentType, attribute.isNormalized, 0);
		glVertexAttribBinding(attributeCount, attributeCount);

		++attributeCount;
	}

	glBindVertexArray(0);	
}


std::span<const VertexBufferLayout::Attribute> VertexBufferLayout::getAttributes() const noexcept
{
    return m_attributes;
}


GLuint VertexBufferLayout::getStride() const noexcept
{
    return m_stride;
}