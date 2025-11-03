#ifndef SHADER_HPP
#define SHADER_HPP

#include <filesystem>

#include <glad/glad.h>

class Shader final
{
public:
    Shader() noexcept;
    Shader(const Shader&) noexcept = delete;
    Shader(Shader&&) noexcept = delete;
    Shader& operator = (const Shader&) noexcept = delete;
    Shader& operator = (Shader&&) noexcept = delete;
    ~Shader() noexcept;

    GLuint loadFromFile(const std::filesystem::path& filepath, GLenum shaderType) noexcept;
    GLuint getHandle() const noexcept;
    GLenum getType()   const noexcept;

private:
    GLuint m_handle;
    GLenum m_type;
};

#endif // !SHADER_HPP