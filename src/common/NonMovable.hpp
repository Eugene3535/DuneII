#ifndef NON_MOVABLE_HPP
#define NON_MOVABLE_HPP

class NonMovable 
{
protected:
    NonMovable() = default;
    ~NonMovable() = default;

    NonMovable(NonMovable&&) = delete;
    NonMovable& operator=(NonMovable&&) = delete;
};

#endif