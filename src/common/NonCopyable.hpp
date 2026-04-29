#ifndef NON_COPYABLE_HPP
#define NON_COPYABLE_HPP

class NonCopyable 
{
protected:
    NonCopyable() = default;
    ~NonCopyable() = default;

    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

#endif