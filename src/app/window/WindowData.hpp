#pragma once

#include <cglm/struct/vec2.h>

#include "graphics/camera/OrthoMatrix.hpp"


struct WindowData
{
    vec2s cursor;

    struct
    {
        int button;
        int action;
        int mods;
    } mouse;

    struct
    {
        int key;
        int scancode;
        int action;
        int mods;
    } keyboard;

    OrthoMatrix* view = nullptr;
    class Scene* scene = nullptr;
};