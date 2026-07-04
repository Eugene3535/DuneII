#pragma once

#include <string>

#include <cglm/struct/vec2.h>

#include "graphics/camera/OrthoMatrix.hpp"


struct WindowData
{
    std::string title;
    ivec2s size;

    OrthoMatrix viewport;

    void* scene = nullptr;
};