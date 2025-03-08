// 312CA Chirila Tiberiu Andrei

#ifndef HELPER_H
#define HELPER_H
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

typedef unsigned char u8;

typedef struct {
	int width;      // Width of the image
	int height;     // Height of the image
	int channels;   // Number of channels (e.g., 1 for grayscale, 3 for RGB)
	u8 ***data; // 3D array to hold pixel data
} image;

// Struct definition for a selection within an image
typedef struct {
	int x1, x2; // x-coordinate of the selection
	int y1, y2; // y-coordinate of the selection
	bool select; // true if it contains a selection, false otherwise
} select_t;

//LOAD
image *load(char *filename);
//FREE
void displace_image(image *matrix);
//SAVE
void save(image *matrix, char *filename, bool ascii);
//SWAP
void swap_int(int *a, int *b);
//SELECT
void select_(int x1, int y1, int x2, int y2, select_t *matrix, image *m);
//SELECT ALL
void select_all(select_t *matrix, image *m);
//Verify_y
int verify_y(int y);
//Histogram
void histogram(image *matrix, int x, int y);
//Equalize
void equalize(image *matrix);
//Rotate
void rotate_clockwise(image *matrix, select_t *m, bool select);
//Crop
void crop(select_t *sel, image *matrix);
//Clamp
uint8_t clamp(int x);
//Edge
void edge(select_t *m, image *matrix);
//Blur
void blur(select_t *m, image *matrix);
//Sharpen
void sharpen(select_t *m, image *matrix);
//Gausian-blur
void gausian_blur(select_t *m, image *matrix);

#endif
