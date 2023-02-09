#ifndef TETRIS_COLOR_HPP
#define TETRIS_COLOR_HPP

#define RED Color(0xFF595EFF)
#define YELLOW Color(0xFFCA3AFF)
#define GREEN Color(0x8AC926FF)
#define BLUE Color(0X1982C4FF)
#define PURPLE Color(0x6A4C93FF)
#define CYAN Color(0x00C1C9FF)
#define ORANGE Color(0xFC6115FF)
#define BLACK Color(0x000000FF)
#include <stdint.h>

class Color
{
public:
    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
    Color(uint32_t hex);
    Color();
    uint8_t getRed();
    uint8_t getGreen();
    uint8_t getBlue();
    uint8_t getAlpha();
    void set(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

private:
    uint8_t m_alpha;
    uint8_t m_red;
    uint8_t m_green;
    uint8_t m_blue;
};

#endif