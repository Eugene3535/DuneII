#include "common/tasks/TaskData.hpp"
#include "common/tasks/Task.hpp"


BEGIN_NAMESPACE_TASK

uint32_t construct(void* taskData, float deltaTime) noexcept
{
    auto* data = static_cast<Construction*>(taskData);

    (*data->progress) -= data->duration * deltaTime;

    if ((*data->progress) < 1.f)
    {
        return sizeof(Construction);
    }

    return 0;
}

END_NAMESPACE