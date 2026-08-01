#ifdef DEBUG
#include <cstdio>
#endif

#include <array>
#include <type_traits>
#include <string>
#include <unordered_map>

#include "files/FileProvider.hpp"
#include "files/Shader.hpp"
#include "app/game/Game.hpp"
#include "app/context/Context.hpp"


static GLuint create_program(std::span<const GLuint> shaders) noexcept
{
    GLint program = glCreateProgram();

    for(const auto& shader : shaders)
        glAttachShader(program, shader);
    
    glLinkProgram(program);

    int32_t success = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (success == GL_FALSE)
    {
        GLchar infoLog[1024]{};
        glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
#ifdef DEBUG
        printf("SHADER PROGRAM: Link-time error:\n%s\n", infoLog);
#endif
        glDeleteProgram(program);

        return 0;
    }
    else
    {
        GLuint uniformBlockIndex = glGetUniformBlockIndex(program, "Matrices");

        if (uniformBlockIndex != GL_INVALID_INDEX)
            glUniformBlockBinding(program, uniformBlockIndex, 0);
    }

    for(const auto& shader : shaders)
        glDetachShader(program, shader);

    return program;
}



Context::Context() noexcept
{

}


Context::~Context() = default;


bool Context::load(Game& game) noexcept
{
    static_assert(std::is_same_v<GLbyte, int8_t>, "GLbyte mismatch");
    static_assert(std::is_same_v<GLubyte, uint8_t>, "GLubyte mismatch");
    static_assert(std::is_same_v<GLshort, int16_t>, "GLshort mismatch");
    static_assert(std::is_same_v<GLushort, uint16_t>, "GLushort mismatch");
    static_assert(std::is_same_v<GLint, int32_t>, "GLint mismatch");
    static_assert(std::is_same_v<GLuint, uint32_t>, "GLuint mismatch");
    static_assert(std::is_same_v<GLfloat, float>, "GLfloat mismatch");
    static_assert(std::is_same_v<GLdouble, double>, "GLdouble mismatch");

    std::unordered_map<std::string, Shader> shaderMap;

    for (const auto filename : 
    {
        "color_outline.frag",
        "color_outline.vert",
        "color_sprite.frag",
        "entity_view.frag",
        "selection.frag",
        "selection.vert",
        "sprite.frag",
        "sprite.vert"
    }) shaderMap[filename] = Shader();

    for (auto& [filename, shader] : shaderMap)
    {
        GLenum target;

        if (filename.ends_with(".vert"))
            target = GL_VERTEX_SHADER;
        else if (filename.ends_with(".frag"))
            target = GL_FRAGMENT_SHADER;
        else if (filename.ends_with(".geom"))
            target = GL_GEOMETRY_SHADER;

        if (!shader.loadFromFile(FileProvider::findPathToFile(filename), target))
            return false;
    }

    std::unordered_map<std::string, uint32_t> programs;

    {// Color outline
        std::array<GLuint, 2> shaders;
        shaders[0] = shaderMap["color_outline.vert"].getHandle();
        shaders[1] = shaderMap["color_outline.frag"].getHandle();

        if (const GLuint program = create_program(shaders))
            programs.emplace("color_outline", program);    
    }

    {// Sprite
        std::array<GLuint, 2> shaders;
        shaders[0] = shaderMap["sprite.vert"].getHandle();
        shaders[1] = shaderMap["sprite.frag"].getHandle();

        if (const GLuint program = create_program(shaders))
            programs.emplace("sprite", program); 
    }

    {// Color sprite
        std::array<GLuint, 2> shaders;
        shaders[0] = shaderMap["sprite.vert"].getHandle();
        shaders[1] = shaderMap["color_sprite.frag"].getHandle();

        if (const GLuint program = create_program(shaders))
            programs.emplace("color_sprite", program); 
    }

    {// Entity preview sidebar
        std::array<GLuint, 2> shaders;
        shaders[0] = shaderMap["sprite.vert"].getHandle();
        shaders[1] = shaderMap["entity_view.frag"].getHandle();

        if (const GLuint program = create_program(shaders))
            programs.emplace("entity_view", program);   
    }

    {// Selection frame
        std::array<GLuint, 2> shaders;
        shaders[0] = shaderMap["selection.vert"].getHandle();
        shaders[1] = shaderMap["selection.frag"].getHandle();

        if (const GLuint program = create_program(shaders))
            programs.emplace("selection", program);   
    }

    game.glResources.appendPrograms(programs);

    return true;
}


void Context::save(Game& game) noexcept
{
    game.glResources.reset();
}