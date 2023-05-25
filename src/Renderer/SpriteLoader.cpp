#include "SpriteLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace sng {

Sprite SpriteLoader::load(const std::string& filename) {
    Sprite sprite;
    int n = 0;
    sprite.data = stbi_load(filename.c_str(), &sprite.width, &sprite.height, &n, 4);
    return sprite;
}

}