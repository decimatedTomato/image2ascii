#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

#define TARGET_IMAGE "wide checker.jpeg"
#define PALETTE_SIZE 13
#define PALETTE " .,-~:;=!*#$@"

#define CANVAS_WIDTH 80 * 2
#define CANVAS_HEIGHT 45

int main() {
    char palette[] = PALETTE;

    int width, height, channels;
    unsigned char *img = stbi_load(TARGET_IMAGE, &width, &height, &channels, 0);
    if (img == NULL) {
        printf("Error loading the image\n");
        exit(1);
    }

    printf("image w:%d h:%d ch:%d\n", width, height, channels);
    char output[(CANVAS_WIDTH + 1) * CANVAS_HEIGHT + 1];
    
    const int pixel_group_height = height / CANVAS_HEIGHT;
    const int pixel_group_width = width / CANVAS_WIDTH;

    char *current_output = output;
    unsigned char *current_img = img;

    //TODO change to use resize image
    
    // for (size_t h = 0; h < CANVAS_HEIGHT; h++) {
    //     for (size_t w = 0; w < CANVAS_WIDTH; w++) {
    for (size_t h = 0; h < CANVAS_HEIGHT; h++) {
        for (size_t w = 0; w < CANVAS_WIDTH; w++) {
            // Calculate average brightness of pixel region
            float brightness_sum = 0;
            unsigned char *pixel_group_ptr;
            for (size_t j = 0; j < pixel_group_height; j++) {
                for (size_t i = 0; i < pixel_group_width; i++) {
                    pixel_group_ptr = img + i + j * 3 * CANVAS_WIDTH;
                    brightness_sum += (*pixel_group_ptr + *(pixel_group_ptr + 1) + *(pixel_group_ptr + 2)) / (3.0 * UCHAR_MAX);
                }
            }
            // Map to palette char
            *current_output = palette[(int)(PALETTE_SIZE * brightness_sum / (pixel_group_height * pixel_group_width))];

            current_output++;
            current_img += 3 * pixel_group_width;
        }
        *current_output = '\n';
        current_output++;
        current_img += 3 * CANVAS_WIDTH * 3 * pixel_group_height;
    }
    *current_output = '\0';

    stbi_image_free(img);

    // Print output string
    printf("%s", output);
    return 0;
}