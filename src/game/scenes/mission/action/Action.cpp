#include "game/scenes/mission/action/Action.hpp"


BEGIN_NAMESPACE_ACTION

size_t construct(void* actionData) noexcept
{
    auto* data = static_cast<Construction*>(actionData);

    if (data->duration < 0.f)
        return sizeof(Construction);

    return 0;
}

END_NAMESPACE_ACTION


