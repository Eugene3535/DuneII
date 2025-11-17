#ifndef DESTINY_HPP
#define DESTINY_HPP

#include "game/scenes/Scene.hpp"


class Destiny:
    public Scene
{
public:
    explicit Destiny(class DuneII* game) noexcept;

    bool load(std::string_view info)   noexcept override;
    void update(float dt)              noexcept override;
    void draw()                        noexcept override;
    void resize(int width, int height) noexcept override;

private:

};

#endif // !DESTINY_HPP