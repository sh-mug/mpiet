#ifndef _COLOR_HPP_
#define _COLOR_HPP_

#include <iostream>
#include <png++/png.hpp>

bool operator== (const png::rgb_pixel &a, const png::rgb_pixel &b) {
    return a.red == b.red && a.green == b.green && a.blue == b.blue;
}
std::ostream &operator<< (std::ostream &os, const png::rgb_pixel &pixel) {
    os << "R:" << static_cast<int>(static_cast<int>(pixel.red))
      << " G:" << static_cast<int>(static_cast<int>(pixel.green))
      << " B:" << static_cast<int>(static_cast<int>(pixel.blue));
    return os;
}

struct Color {
    bool isblack = false;
    bool iswhite = false;
    int hue = -1;
    int lightness = -1;
    bool operator== (Color other) {
        return this->isblack == other.isblack && this->iswhite == other.iswhite
            && this->hue == other.hue && this->lightness == other.lightness;
    }
    bool operator!= (Color other) { return !(*this == other); }
    void operator=  (png::rgb_pixel pixel) {
        if (pixel == png::rgb_pixel(0x00, 0x00, 0x00)) { isblack = true; } else
        if (pixel == png::rgb_pixel(0xFF, 0xFF, 0xFF)) { iswhite = true; } else
        if (pixel == png::rgb_pixel(0xFF, 0xC0, 0xC0)) { hue = 0; lightness = 0; } else
        if (pixel == png::rgb_pixel(0xFF, 0x00, 0x00)) { hue = 0; lightness = 1; } else
        if (pixel == png::rgb_pixel(0xC0, 0x00, 0x00)) { hue = 0; lightness = 2; } else
        if (pixel == png::rgb_pixel(0xFF, 0xFF, 0xC0)) { hue = 1; lightness = 0; } else
        if (pixel == png::rgb_pixel(0xFF, 0xFF, 0x00)) { hue = 1; lightness = 1; } else
        if (pixel == png::rgb_pixel(0xC0, 0xC0, 0x00)) { hue = 1; lightness = 2; } else
        if (pixel == png::rgb_pixel(0xC0, 0xFF, 0xC0)) { hue = 2; lightness = 0; } else
        if (pixel == png::rgb_pixel(0x00, 0xFF, 0x00)) { hue = 2; lightness = 1; } else
        if (pixel == png::rgb_pixel(0x00, 0xC0, 0x00)) { hue = 2; lightness = 2; } else
        if (pixel == png::rgb_pixel(0xC0, 0xFF, 0xFF)) { hue = 3; lightness = 0; } else
        if (pixel == png::rgb_pixel(0x00, 0xFF, 0xFF)) { hue = 3; lightness = 1; } else
        if (pixel == png::rgb_pixel(0x00, 0xC0, 0xC0)) { hue = 3; lightness = 2; } else
        if (pixel == png::rgb_pixel(0xC0, 0xC0, 0xFF)) { hue = 4; lightness = 0; } else
        if (pixel == png::rgb_pixel(0x00, 0x00, 0xFF)) { hue = 4; lightness = 1; } else
        if (pixel == png::rgb_pixel(0x00, 0x00, 0xC0)) { hue = 4; lightness = 2; } else
        if (pixel == png::rgb_pixel(0xFF, 0xC0, 0xFF)) { hue = 5; lightness = 0; } else
        if (pixel == png::rgb_pixel(0xFF, 0x00, 0xFF)) { hue = 5; lightness = 1; } else
        if (pixel == png::rgb_pixel(0xC0, 0x00, 0xC0)) { hue = 5; lightness = 2; } else
        throw std::runtime_error("invalid color");
    }
};

#endif
