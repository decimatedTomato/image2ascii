#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image/stb_image_resize.h"

// #define TARGET_IMAGE "blue.jpeg"
// #define TARGET_IMAGE "Black.jpg"
// #define TARGET_IMAGE "wide checker.jpeg"
// #define TARGET_IMAGE "rina.jpeg"
#define TARGET_IMAGE "rina.jpeg"
#define PALETTE_SIZE 13
#define PALETTE " .,-~:;=!*#$@"

// #define CANVAS_WIDTH 160
// #define CANVAS_HEIGHT 45

// #define CANVAS_WIDTH 300
#define CANVAS_WIDTH 280
#define CANVAS_HEIGHT 70

int main() {
    char palette[PALETTE_SIZE] = PALETTE;

    int width, height, channels;
    unsigned char *img = stbi_load(TARGET_IMAGE, &width, &height, &channels, 0);
    if (img == NULL) {
        printf("Error loading the image\n");
        exit(1);
    }
    unsigned char *resized_img = malloc(width * height * channels);
    if (resized_img == NULL) {
        printf("Out of memory error\n");
        exit(1);
    }

    char output[CANVAS_WIDTH * CANVAS_HEIGHT];
    char *current_output = output;

    // Resize image
    stbir_resize_uint8(img, width, height, 0, resized_img, CANVAS_WIDTH, CANVAS_HEIGHT, 0, channels);
    
    for(unsigned char *p = resized_img; p != resized_img + CANVAS_WIDTH * CANVAS_HEIGHT * channels; p += channels, current_output++) {
        *current_output = palette[(unsigned int)((*p + *(p + 1) + *(p + 2))/3.0 * PALETTE_SIZE / UINT8_MAX)];
    }

    printf("image w:%d h:%d ch:%d\n", CANVAS_WIDTH, CANVAS_HEIGHT, channels);
    for (size_t i = 0; i < CANVAS_HEIGHT * CANVAS_WIDTH; i++) {
        if (i % CANVAS_WIDTH == 0) putchar('\n');
        putchar(output[i]);
    }
    
    // for (size_t h = 0; h < CANVAS_HEIGHT; h++) {
    //     for (size_t w = 0; w < CANVAS_WIDTH; w++) {
    //         putchar(output[w + CANVAS_WIDTH * h]);
    //     }
    //     putchar('\n');
    // }

    stbi_image_free(img);
    stbi_image_free(resized_img);

    return 0;
}