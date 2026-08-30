#pragma once

#include "common/Enums.hpp"


struct Sprite2D
{
	uint32_t texture;
	uint32_t frame;
	uint32_t width;
	uint32_t height;
};


struct Background
{
    uint32_t    vertexArrayObject;
    Sprite2D    sprite;
    uint32_t    program;
};


struct Outline
{
    uint32_t vertexArrayObject;
    uint32_t program;
    uint32_t count;
};


struct Landscape
{
    uint32_t texture;
    uint32_t vertexArrayObject;
    uint32_t vertexBufferObject;
    uint32_t indexBufferObject;
    uint32_t indexCount;
    uint32_t program;
};


struct StructureStorage
{
    uint32_t texture;
    uint32_t vertexBufferObject;
    uint32_t vertexArrayObject;
    void*    mappedStorage;
};


struct Widget
{
    uint32_t background;
    uint32_t outline;
};


struct ConstructionMenuFrames
{
    uint32_t vertexBufferObject;
    uint32_t vertexArrayObject;
    uint32_t program;
    int32_t  uniformColor;

    Widget rootWidget;
    Widget entityWidget;
    Widget entityWidgetLabel;
    Widget entityWidgetParams[3];
};


struct ConstructionMenuPreviewCells
{
    uint32_t program;
    uint32_t texture;
    uint32_t vertexBufferObject;
    uint32_t vertexArrayObject;
    uint32_t cellCount;
    uint32_t cellWidth;
    uint32_t cellHeight;
};


struct ConstructionMenuElements
{        
    uint32_t program;
    uint32_t textures[3];
    uint32_t vertexBufferObject;
    uint32_t vertexArrayObject;
    Sprite2D buttonExit;
    Sprite2D buttonRepair;
    Sprite2D buttonStop;
    EntityIcon lastSelectedPreview;
    
    struct
    {
        uint32_t vertexArrayObject;
        uint32_t program;
        uint32_t count;
        int32_t row;
        int32_t column;
    } selectionFrame;
};