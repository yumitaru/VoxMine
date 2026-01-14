#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_STDIO
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#define STBI_NO_LINEAR
#define STBI_NO_HDR

#include <fstream>
#include <vector>
#include <cstring>
#include <cstdlib>

#include "stb_image.h"


#define STBI_ASSERT(x)

static unsigned char* fake_load(
    const char* filename, int* x, int* y, int* ch, int desired_ch
) {
    std::ifstream f(filename, std::ios::binary);
    if (!f) return nullptr;

    std::vector<unsigned char> data(
        (std::istreambuf_iterator<char>(f)),
        std::istreambuf_iterator<char>()
    );

    *x = 1;
    *y = 1;
    *ch = 4;

    unsigned char* img = (unsigned char*)malloc(4);
    img[0] = 255;
    img[1] = 0;
    img[2] = 255;
    img[3] = 255;
    return img;
}

unsigned char* stbi_load(
    char const* filename,
    int* x, int* y,
    int* channels_in_file,
    int desired_channels
) {
    return fake_load(filename, x, y, channels_in_file, desired_channels);
}

void stbi_image_free(void* data) {
    free(data);
}
