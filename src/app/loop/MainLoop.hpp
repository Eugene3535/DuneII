#pragma once

class MainLoop
{
public:
    MainLoop(class MainWindow& window) noexcept;
    ~MainLoop();

    void operator()(struct Game& game) noexcept;

private:
    class MainWindow& m_window;
};