#ifndef EXTERNAL_MATH_HPP
#define EXTERNAL_MATH_HPP

#include <array>

// #include <glm/gtc/constants.hpp> // glm::pi() and other

class TrigTable
{
public:
    TrigTable() noexcept;

    float getSin(int degrees) const noexcept;
    float getCos(int degrees) const noexcept;

private:
    static constexpr size_t TABLE_SIZE = 360;

    std::array<float, TABLE_SIZE> m_sinTable;
    std::array<float, TABLE_SIZE> m_cosTable;
};

// int32_t get_distance(const sf::Vector2i& from, const sf::Vector2i& to) noexcept;

#endif // !EXTERNAL_MATH_HPP