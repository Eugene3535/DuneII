#include "common/tasks/TaskData.hpp"
#include "common/tasks/Task.hpp"


BEGIN_NAMESPACE_TASK

uint32_t construct(void* taskData, float deltaTime) noexcept
{
    auto* data = static_cast<Construction*>(taskData);

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