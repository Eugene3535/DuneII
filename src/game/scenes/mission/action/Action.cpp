#include "game/scenes/mission/action/ActionData.hpp"
#include "game/scenes/mission/action/Action.hpp"


namespace Action
{

size_t construct(void* actionData, float deltaTime) noexcept
{
    auto* data = static_cast<Construction*>(actionData);

    data->duration -= deltaTime;

    if (data->duration < 0.f)
        return sizeof(Construction);

    return 0;
}

}