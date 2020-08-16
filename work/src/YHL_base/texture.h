#ifndef TEXTURE_H
#define TEXTURE_H

class Texture
{
public:
    Texture(char* path);

    ~Texture();

    inline unsigned char* data() {
        return _data;
    }

    inline bool isValid() {
        return _valid;
    }

    inline int getWidth() {
        return _width;
    }

    inline int getHeight() {
        return _height;
    }

    void reloadGraph(char* path);

    void free();

private:
    bool loadGraph(char* path);

    unsigned char* _data;
    int _width;
    int _height;
    bool _valid;
};

#endif // TEXTURE_H