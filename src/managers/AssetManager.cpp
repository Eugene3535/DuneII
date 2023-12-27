#include "managers/AssetManager.hpp"

AssetManager* AssetManager::m_instance;

AssetManager::AssetManager() noexcept
{
    if (m_instance == nullptr)
        m_instance = this;
}