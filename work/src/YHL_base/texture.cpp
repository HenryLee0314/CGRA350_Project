#include "texture.h"
#include "cgra_log.h"

// #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Texture::Texture(char* graphPath)
: _data(nullptr)
, _width(0)
, _height(0)
, _valid(false)
{
    CGRA_LOGD("graphPath(%s)", graphPath);
    _valid = loadGraph(graphPath);
}

void Texture::reloadGraph(char* graphPath) {
    CGRA_LOGD("graphPath(%s)", graphPath);

    free();

    _valid = loadGraph(graphPath);
}

bool Texture::loadGraph(char* graphPath) {
    int channels;

    do {
        if (graphPath == nullptr) {
            CGRA_LOGE("return false, graphPath == nullptr");
            break;
        }

        if (_data != nullptr) {
            CGRA_LOGE("return false, _data != nullptr");
            break;
        }

        _data = stbi_load(graphPath, &_width, &_height, &channels, 0);

        if (_data == nullptr) {
            CGRA_LOGE("return false, _data == nullptr");
            break;
        }

        CGRA_LOGD("return ture, _width(%d), _height(%d)", _width, _height);
        return true;
    } while(0);
    return false;
}

Texture::~Texture() {
    if (_data != nullptr) {
        stbi_image_free(_data);
    }
}

void Texture::free() {
    CGRA_LOGD();
    if (_data != nullptr) {
        stbi_image_free(_data);
        _data = nullptr;
    }
    _valid = false;
    _width = 0;
    _height = 0;
}