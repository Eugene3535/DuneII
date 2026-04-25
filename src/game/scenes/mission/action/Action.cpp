#include "game/scenes/mission/action/ActionData.hpp"
#include "game/scenes/mission/action/Action.hpp"


namespace Action
{

size_t construct(void* actionData, float deltaTime) noexcept
{
    auto* data = static_cast<Construction*>(actionData);

    data->countdown -= data->duration * deltaTime;
    (*data->progress) = data->countdown;

    if (data->countdown < 1.f)
    {
        (*data->progress) = 0;

        return sizeof(Construction);
    }

    return 0;
}

}