#pragma once

class Context
{
public:
    Context() noexcept;
    virtual ~Context();

    bool initialize() noexcept;

private:
    void initializeDebug() noexcept;
};