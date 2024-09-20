#include <cassert>

#include "assets/AssetManager.hpp"

AssetManager* AssetManager::m_instance;

AssetManager::AssetManager() noexcept
{
    assert((m_instance == nullptr) && "AssetManager must be unique!");
    m_instance = this;
}

AssetManager* AssetManager::instance() noexcept
{
    return m_instance;
}