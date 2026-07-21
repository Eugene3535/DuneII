#include "common/action/ActionData.hpp"
#include "common/action/Action.hpp"


BEGIN_NAMESPACE_ACTION

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

END_NAMESPACE