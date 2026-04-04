#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <filesystem>
#include <memory>

struct StbImage
{
    struct StbImageDeleter 
    {
        void operator()(uint8_t* src) noexcept;
    };

    bool loadFromFile(const std::filesystem::path& filepath) noexcept;

    std::unique_ptr<uint8_t[], StbImageDeleter> pixels;
    int32_t width = 0;
    int32_t height = 0;
    int32_t bytePerPixel = 0;
};

#endif // !IMAGE_HPP