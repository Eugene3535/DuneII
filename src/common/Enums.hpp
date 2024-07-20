#ifndef ENUMS_HPP
#define ENUMS_HPP

enum class House : unsigned int
{
    Atreides = 1 << 1,
    Ordos = 1 << 2,
    Harkonnen = 1 << 3,
    Fremen = 1 << 4
};

#endif // !ENUMS_HPP