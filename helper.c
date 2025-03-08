// 312CA Chirila Tiberiu Andrei

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "helper.h"

image *load(char *filename)
{
	FILE *file = fopen(filename, "rb"); // Opens the file in binary mode
	if (!file) {
		printf("Failed to load %s\n", filename); // Return an error
		return NULL;
	}
	image *matrix = NULL;
	int v, x; // The max value and an aux value for binary
	char format[3];
	matrix = malloc(sizeof(image)); // Allocate memory for the image structure
	fscanf(file, "%3s ", format); // Read the image format and dimensions
	fscanf(file, "%d %d %d ", &matrix->width, &matrix->height, &v);
	/// Allocate memory for the image
	matrix->data = (u8 ***)malloc(matrix->width * sizeof(u8 **));
	for (int i = 0; i < matrix->width; i++)
		matrix->data[i] =
			(u8 **)malloc(matrix->height * sizeof(u8 *));
	/// Load the image data
	switch (format[1]) {
	case '2': // ASCII grayscale
		matrix->channels = 1;
		for (int i = 0; i < matrix->width; i++)
			for (int j = 0; j < matrix->height; j++)
				matrix->data[i][j] =
					(u8 *)malloc(matrix->channels * sizeof(u8));
		for (int i = 0; i < matrix->height; i++) {
			for (int j = 0; j < matrix->width; j++) {
				fscanf(file, "%d ", &x); // Read the pixel value
				matrix->data[j][i][0] = x; // Store the pixel value
			}
		}
		break;
	case '3': // P3 format (ASCII RGB)
		matrix->channels = 3;
		for (int i = 0; i < matrix->width; i++)
			for (int j = 0; j < matrix->height; j++)
				matrix->data[i][j] =
					(u8 *)malloc(matrix->channels * sizeof(u8));
		for (int i = 0; i < matrix->height; i++) {
			for (int j = 0; j < matrix->width; j++) { // Read the pixel
				fscanf(file, "%d ", &x); matrix->data[j][i][0] = x;
				fscanf(file, "%d ", &x); matrix->data[j][i][1] = x;
				fscanf(file, "%d ", &x); matrix->data[j][i][2] = x;
			}
		}
		break;

	case '5': // P5 format (binary grayscale)
		matrix->channels = 1;
		for (int i = 0; i < matrix->width; i++)
			for (int j = 0; j < matrix->height; j++)
				matrix->data[i][j] =
					(u8 *)malloc(matrix->channels * sizeof(u8));
		for (int i = 0; i < matrix->height; i++) {
			for (int j = 0; j < matrix->width; j++) {
				fread(&matrix->data[j][i][0], 1, 1, file); // Read the pixel
			}
		}
		break;

	case '6': // P6 format (binary RGB)
		matrix->channels = 3;
		for (int i = 0; i < matrix->width; i++)
			for (int j = 0; j < matrix->height; j++)
				matrix->data[i][j] =
					(u8 *)malloc(matrix->channels * sizeof(u8));
		for (int i = 0; i < matrix->height; i++) {
			for (int j = 0; j < matrix->width; j++) {
				fread(&matrix->data[j][i][0], 1, 1, file); // Read the pixel
				fread(&matrix->data[j][i][1], 1, 1, file);
				fread(&matrix->data[j][i][2], 1, 1, file);
			}
		}
		break;
	}
	fclose(file); // Close the file
	printf("Loaded %s\n", filename); // Return success
	return matrix;
}

void displace_image(image *matrix)
{
	if (!matrix) { // Check if the image is loaded
		return;
	}
	for (int i = 0; i < matrix->width; i++) {
		for (int j = 0; j < matrix->height; j++) {
			free(matrix->data[i][j]); // Free the pixel data
		}
		free(matrix->data[i]); // Free the row
	}
	free(matrix->data); // Free the image data
	free(matrix); // Free the image structure
}

void save(image *matrix, char *filename, bool ascii)
{
	if (!matrix) {
		printf("No image loaded\n"); // Check if the image is loaded
		return;
	}

	if (ascii) { // Check if the image is saved in ASCII format
		FILE *f = fopen(filename, "w"); // Open the file in write mode
		if (matrix->channels == 1) // Write the image format and dimensions
			fprintf(f, "P2\n");
		else if (matrix->channels == 3)
			fprintf(f, "P3\n");
		fprintf(f, "%d %d\n", matrix->width, matrix->height);
		fprintf(f, "%d\n", 255);
		for (int i = 0; i < matrix->height; i++) {
			for (int j = 0; j < matrix->width; j++)
				for (int k = 0; k < matrix->channels; k++)
					fprintf(f, "%d ", matrix->data[j][i][k]);
					// Write the image
			fprintf(f, "\n");
		}
		fclose(f); // Close the file
	} else { // Save the image in binary format
		FILE *f = fopen(filename, "wb"); // Open the file in write binary mode
		if (matrix->channels == 1) // Write the image format and dimensions
			fprintf(f, "P5\n");
		else
			fprintf(f, "P6\n");
		fprintf(f, "%d %d\n", matrix->width, matrix->height);
		fprintf(f, "%d\n", 255);
		for (int i = 0; i < matrix->height; i++) {
			for (int j = 0; j < matrix->width; j++)
				for (int k = 0; k < matrix->channels; k++)
					fwrite(&matrix->data[j][i][k], sizeof(u8), 1, f);
		}	// Write the image
		fclose(f); // Close the file
	}
	printf("Saved %s\n", filename);
}

void swap_int(int *a, int *b)
{
	int temp = *a; // Swap two integers
	*a = *b; // Using a temporary variable
	*b = temp;
}

void select_(int x1, int y1, int x2, int y2, select_t *matrix, image *m)
{
	if (!m) { // Check if the image is loaded
		printf("No image loaded\n");
		return;
	}
	if (x2 < x1) { // Swap the coordinates if needed
		swap_int(&x1, &x2);
	}
	if (y2 < y1) {
		swap_int(&y1, &y2);
	}
	if (x1 < 0 || y1 < 0 || x2 > m->width || y2 > m->height || y1 == y2) {
		// Check if the coordinates are valid
		printf("Invalid set of coordinates\n");
		return;
	}
	matrix->x1 = x1; // Set the selection coordinates
	matrix->x2 = x2;
	matrix->y1 = y1;
	matrix->y2 = y2;
	matrix->select = true; // Set the selection flag

	printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
}

void select_all(select_t *matrix, image *m)
{
	if (!m) { // Check if the image is loaded
		printf("No image loaded\n");
		return;
	}
	matrix->x1 = 0; // Set the selection to the entire image
	matrix->x2 = m->width;
	matrix->y1 = 0;
	matrix->y2 = m->height;
	matrix->select = true; // Set the selection flag
	printf("Selected ALL\n");
}

int verify_y(int y)
{
	if (y < 2 || y > 256) // Check if the number of bins is valid
		return 1;

	return !((y & (y - 1)) == 0); // Check if the number is a power of 2
}

u8 clamp(int x)
{
	if (x < 0) // Clamp the value to 0 - 255
		return 0;
	else if (x > 255)
		return 255;

	return x;
}

void histogram(image *matrix, int x, int y)
{
	if (!matrix) { // Check if the image is loaded
		printf("No image loaded\n");
		return;
	}

	if (matrix->channels != 1) { // Check if the image is grayscale
		printf("Black and white image needed\n");
		return;
	}

	if (verify_y(y)) // Check if the number of bins is valid
		return;

	int *bins = calloc(y, sizeof(int)); // Allocate memory for the bins
	int bin_size = 256 / y; // Calculate the bin size

	for (int i = 0; i < matrix->width; i++) { // Calculate the histogram
		for (int j = 0; j < matrix->height; j++) {
			unsigned char pix_val = matrix->data[i][j][0]; // Get the pixel
			int bin_index = pix_val / bin_size; // Calculate the bin index
			if (bin_index >= y) // Clamp the bin index
				bin_index = y - 1;

			bins[bin_index]++; // Update the bin count
		}
	}

	int max_count = 0; // Find the maximum count
	for (int i = 0; i < y; i++) {
		if (bins[i] > max_count)
			max_count = bins[i]; // Update the maximum count
	}

	for (int i = 0; i < y; i++) {
		int star_count = (bins[i] * x) / max_count; //The number of stars
		printf("%d\t|\t", star_count);

		for (int s = 0; s < star_count; s++) // Print the histogram
			printf("*");

		printf("\n");
	}

	free(bins);
}

void equalize(image *matrix)
{
	if (!matrix) { // Check if the image is loaded
		printf("No image loaded\n");
		return;
	}

	if (matrix->channels != 1) { // Check if the image is grayscale
		printf("Black and white image needed\n");
		return;
	}

	int H[256] = {0}; // Calculate the histogram
	int area = matrix->width * matrix->height; //Calculate the area of the image
	for (int i = 0; i < matrix->height; i++) {
		for (int j = 0; j < matrix->width; j++) {
			unsigned char val = matrix->data[j][i][0]; // Get the pixel value
			H[val]++; // Update the histogram
		}
	}

	int cdf[256]; // Calculate the CDF
	cdf[0] = H[0];
	for (int i = 1; i < 256; i++) {
		cdf[i] = cdf[i - 1] + H[i]; // Calculate the CDF
	}

	u8 lut[256];
	float area_d = (float)area;
	for (int i = 0; i < 256; i++) {
		float val = 255.0 * ((float)cdf[i] / area_d); // Calculate the LUT
		val = round(val); // Round the value
		val = clamp((u8)val); // Clamp the value
		lut[i] = val; // Update the LUT
	}

	for (int i = 0; i < matrix->height; i++) {
		for (int j = 0; j < matrix->width; j++) {
			matrix->data[j][i][0] = lut[matrix->data[j][i][0]]; // Update image
		}
	}

	printf("Equalize done\n");
}

void alloc_temp(u8 ****temp, int width, int height, int channels)
{
	*temp = (u8 ***)malloc(width * sizeof(u8 **)); // Allocate memory
	for (int i = 0; i < width; i++)
		(*temp)[i] = (u8 **)malloc(height * sizeof(u8 *));
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
			(*temp)[i][j] = (u8 *)malloc(channels * sizeof(u8));
}

void free_temp(u8 ***temp, int width, int height)
{
	for (int i = 0; i < width; i++) { // Free memory
		for (int j = 0; j < height; j++)
			free(temp[i][j]);
		free(temp[i]);
	}
	free(temp);
}

void rotate_clockwise(image *matrix, select_t *m, bool select)
{
	int width, length;
	if (select || !m) { // Check if a selection is made
		width = matrix->width;
		length = matrix->height;
	} else { // Calculate the dimensions of the selection
		width = m->x2 - m->x1;
		length = m->y2 - m->y1;
	}
	u8 ***temp = NULL; // Allocate memory for the rotated image
	alloc_temp(&temp, length, width, matrix->channels);
	// Perform -90 degrees rotation
	for (int i = 0; i < width; i++)
		for (int j = 0; j < length; j++) {
			for (int k = 0; k < matrix->channels; k++) {
				temp[length - j - 1][i][k] =
					matrix->data[i + m->x1][j + m->y1][k];
			}
		}
	// Copy rotated data back into selection
	if (select) { // Check if a selection is made
		for (int i = 0; i < matrix->width; i++) {
			for (int j = 0; j < matrix->height; j++) {
				free(matrix->data[i][j]);
			}
			free(matrix->data[i]);
		}
		free(matrix->data);
		matrix->width = length;
		matrix->height = width;
		matrix->data = temp;
	} else { // Copy rotated data back into the image
		for (int i = 0; i < width; i++)
			for (int j = 0; j < length; j++)
				for (int k = 0; k < matrix->channels; k++)
					matrix->data[i + m->x1][j + m->y1][k] = temp[i][j][k];

		free_temp(temp, length, width); // Free memory
		if (m->x1 == 0 && m->y1 == 0 && m->x2 == matrix->width &&
			m->y2 == matrix->height) { // Update the image dimensions
			matrix->width = length;
			matrix->height = width;
		}
	}
}

void crop(select_t *sel, image *matrix)
{
	if (!matrix) { // Check if the image is loaded
		printf("No image loaded\n");
		return;
	}
	// Calculate new dimensions
	int new_width = sel->x2 - sel->x1;
	int new_height = sel->y2 - sel->y1;
	if (new_width == matrix->width && new_height == matrix->height) {
		printf("Image cropped\n");
		return;
	}
	// Allocate memory for cropped image
	u8 ***new_data = NULL;
	alloc_temp(&new_data, new_width, new_height, matrix->channels);
	// Copy pixel data from the selected region
	for (int i = 0; i < new_width; i++) {
		for (int j = 0; j < new_height; j++) {
			for (int k = 0; k < matrix->channels; k++) {
				new_data[i][j][k] = matrix->data[sel->x1 + i][sel->y1 + j][k];
			}
		}
	}

	// Free the data
	free_temp(matrix->data, matrix->width, matrix->height);

	// Update the image structure with the cropped image
	matrix->data = new_data;
	matrix->width = new_width;
	matrix->height = new_height;
	// Reset selection to the entire new image
	sel->x1 = 0;
	sel->y1 = 0;
	sel->x2 = new_width;
	sel->y2 = new_height;
	sel->select = true;
	printf("Image cropped\n");

}

int calculate1(image *matrix, int i, int j, int z)
{
	int x = 0;
	int edge[3][3] = {
		{-1, -1, -1},
		{-1, 8, -1},
		{-1, -1, -1}}; // edge matrix

	for (int k = 0; k < 3; k++) {
		for (int l = 0; l < 3; l++) {
			x = x + edge[k][l] *
				matrix->data[i + k - 1][j + l - 1][z];
				// Calculate the edge
		}
	}

	return x ; // Return the edge
}

int calculate2(image *matrix, int i, int j, int z)
{
	int x = 0;
	int sha[3][3] = {
		{0, -1, 0},
		{-1, 5, -1},
		{0, -1, 0}}; // Sharpen matrix

	for (int k = 0; k < 3; k++) {
		for (int l = 0; l < 3; l++) {
			x = x + sha[k][l] *
				matrix->data[i + k - 1][j + l - 1][z];
				// Calculate the sharpen
		}
	}

	return x ; // Return the sharpen
}

float calculate3(image *matrix, int i, int j, int z)
{
	float x = 0;
	int blur[3][3] = {
		{1, 1, 1},
		{1, 1, 1},
		{1, 1, 1}}; // Blur matrix

	for (int k = 0; k < 3; k++) {
		for (int l = 0; l < 3; l++) {
			x = x + blur[k][l] *
				matrix->data[i + k - 1][j + l - 1][z];
				// Calculate the blur
		}
	}

	return x ; // Return the blur
}

float calculate4(image *matrix, int i, int j, int z)
{
	float x = 0;
	int gaus[3][3] = {
		{1, 2, 1},
		{2, 4, 2},
		{1, 2, 1}}; // Gausian-blur matrix

	for (int k = 0; k < 3; k++) {
		for (int l = 0; l < 3; l++) {
			x = x + gaus[k][l] *
				matrix->data[i + k - 1][j + l - 1][z];
				// Calculate the edge
		}
	}

	return x ; // Return the edge
}

void edge(select_t *m, image *matrix)
{
	u8 ***temp = NULL; // Allocate memory for the result
	alloc_temp(&temp, matrix->width, matrix->height, matrix->channels);
	// Perform Sharpen detection
	for (int z = 0; z < matrix->channels; z++) {
		for (int i = m->x1 ; i < m->x2 ; i++) {
			for (int j = m->y1; j < m->y2 ; j++) {
				if (i == 0 || j == 0 || i == matrix->width - 1 ||
					j == matrix->height - 1) {
					temp[i][j][z] = matrix->data[i][j][z]; // Copy the pixel
				} else {
					int x = 0;
					x = calculate1(matrix, i, j, z); // Calculate the edge
					temp[i][j][z] = clamp(x); // Clamp values to 0 - 255
				}
			}
		}
	}

	for (int i = m->x1 ; i < m->x2 ; i++)
		for (int j = m->y1 ; j < m->y2; j++)
			for (int k = 0; k < matrix->channels; k++)
				matrix->data[i][j][k] = temp[i][j][k]; // Update the matrix

	free_temp(temp, matrix->width, matrix->height); // Free memory
	printf("APPLY EDGE done\n");
}

void sharpen(select_t *m, image *matrix)
{
	if (m->select == 0) { // Check if a selection is made
		m->x1 = 0; m->y1 = 0;
		m->x2 = matrix->width; m->y2 = matrix->height;
	}

	// Allocate memory for the result
	u8 ***temp = NULL;
	alloc_temp(&temp, matrix->width, matrix->height, matrix->channels);
	// Perform sharpen
	for (int z = 0; z < matrix->channels; z++) {
		for (int i = m->x1 ; i < m->x2 ; i++) {
			for (int j = m->y1; j < m->y2 ; j++) {
				if (i == 0 || j == 0 || i == matrix->width - 1 ||
					j == matrix->height - 1) {
					temp[i][j][z] = matrix->data[i][j][z];
				} else {
					int x = 0;
					x = calculate2(matrix, i, j, z); // Calculate the sharpen
					temp[i][j][z] = clamp(x); // Clamp values to 0 - 255
				}
			}
		}
	}

	for (int i = m->x1 ; i < m->x2 ; i++)
		for (int j = m->y1 ; j < m->y2; j++)
			for (int k = 0; k < matrix->channels; k++)
				matrix->data[i][j][k] = temp[i][j][k]; // Update the matrix

	free_temp(temp, matrix->width, matrix->height); // Free memory
	printf("APPLY SHARPEN done\n");
}

void blur(select_t *m, image *matrix)
{
	// Allocate memory for the result
	u8 ***temp = NULL;
	alloc_temp(&temp, matrix->width, matrix->height, matrix->channels);
	// Perform blur detection
	for (int z = 0; z < matrix->channels; z++) {
		for (int i = m->x1 ; i < m->x2 ; i++) {
			for (int j = m->y1; j < m->y2 ; j++) {
				if (i == 0 || j == 0 || i == matrix->width - 1 ||
					j == matrix->height - 1) {
					temp[i][j][z] = matrix->data[i][j][z];
				} else {
					float x = 0;
					x = calculate3(matrix, i, j, z); // Calculate the blur
					temp[i][j][z] = clamp(round((float)x / 9.0)); //Clamp value
				}
			}
		}
	}

	for (int i = m->x1 ; i < m->x2 ; i++)
		for (int j = m->y1 ; j < m->y2; j++)
			for (int k = 0; k < matrix->channels; k++)
				matrix->data[i][j][k] = temp[i][j][k]; // Update the matrix

	free_temp(temp, matrix->width, matrix->height); // Free memory

	printf("APPLY BLUR done\n");
}

void gausian_blur(select_t *m, image *matrix)
{
	// Allocate memory for the result
	u8 ***temp = NULL;
	alloc_temp(&temp, matrix->width, matrix->height, matrix->channels);

	// Perform gausian-blur detection
	for (int z = 0; z < matrix->channels; z++) {
		for (int i = m->x1 ; i < m->x2 ; i++) {
			for (int j = m->y1; j < m->y2 ; j++) {
				if (i == 0 || j == 0 || i == matrix->width - 1 ||
					j == matrix->height - 1) {
					temp[i][j][z] = matrix->data[i][j][z];
				} else {
					float x = 0;
					x = calculate4(matrix, i, j, z); // Calculate the blur
					temp[i][j][z] = clamp(round((float)x / 16.0)); //Clamp value
				}
			}
		}
	}

	for (int i = m->x1 ; i < m->x2 ; i++)
		for (int j = m->y1 ; j < m->y2; j++)
			for (int k = 0; k < matrix->channels; k++)
				matrix->data[i][j][k] = temp[i][j][k]; // Update the matrix

	free_temp(temp, matrix->width, matrix->height); // Free memory

	printf("APPLY GAUSSIAN_BLUR done\n");
}
