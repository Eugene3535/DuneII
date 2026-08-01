#pragma once

class Context final
{
public:
    Context() noexcept;
    ~Context();

    bool load(struct Game& game) noexcept;
    void save(struct Game& game) noexcept;
};