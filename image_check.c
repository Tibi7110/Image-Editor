// 312CA Chirila Tiberiu Andrei

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"

void rotate(image *matrix, int angle, select_t *sel)
{
	if (!matrix) {
		printf("No image loaded\n"); // Check if the image is loaded
		return;
	}
	bool select = false; // Check if a selection is made
	if (sel->x1 == 0 && sel->y1 == 0 &&
		sel->x2 == matrix->width && sel->y2 == matrix->height) {
		select = true; // Check if the selection is the entire image
	}
	if (angle % 90 != 0 || angle < -360 || angle > 360) {
		printf("Unsupported rotation angle\n");
		return; // Check if the angle is valid
	}
	int width = sel->x2 - sel->x1, length = sel->y2 - sel->y1;  // the sizes
	if (!sel->select) {
		select = true; // Check if a selection is made
	} else if (length != width &&
		 (sel->x1 != 0 ||
		sel->y1 != 0 ||
		sel->x2 != matrix->width ||
		sel->y2 != matrix->height)) {
		printf("The selection must be square\n");
		return; // Check if the selection is square
	}
	if (width == matrix->width && length == matrix->height)
		select = true;
	if (angle < 0) { // Check if the angle is negative
		switch (angle) {
		case -90:
			rotate_clockwise(matrix, sel, select);
			rotate_clockwise(matrix, sel, select);
			rotate_clockwise(matrix, sel, select);
			printf("Rotated -90\n");
			break; // Perform -90 degrees rotation

		case -180:
			rotate_clockwise(matrix, sel, select);
			rotate_clockwise(matrix, sel, select);
			printf("Rotated -180\n");
			break; // Perform -180 degrees rotation

		case -270:
			rotate_clockwise(matrix, sel, select);
			printf("Rotated -270\n");
			break; // Perform -270 degrees rotation
		case -360:
			printf("Rotated -360\n");
			break; // Perform -360 degrees rotation

		default:
			break;
		}
	} else {
		switch (angle) { // Check if the angle is positive
		case 90:
			rotate_clockwise(matrix, sel, select);
			printf("Rotated 90\n");
			break; // Perform 90 degrees rotation
		case 180:
			rotate_clockwise(matrix, sel, select);
			rotate_clockwise(matrix, sel, select);
			printf("Rotated 180\n");
			break; // Perform 180 degrees rotation
		case 270:
			rotate_clockwise(matrix, sel, select);
			rotate_clockwise(matrix, sel, select);
			rotate_clockwise(matrix, sel, select);
			printf("Rotated 270\n");
			break; // Perform 270 degrees rotation
		case 360:
			printf("Rotated 360\n");
			break; // Perform 360 degrees rotation
		case 0:
			printf("Rotated 0\n");
			break; // Perform 0 degrees rotation
		default:
			break;
		}
	}
}

void apply(select_t *m, image *matrix, char format[])
{
	if (matrix->channels != 3) {
		printf("Easy, Charlie Chaplin\n");
		return; // Check if the image is RGB
	}
	if (strcmp(format, "EDGE") == 0) {
		edge(m, matrix); // Apply edge detection
	} else if (strcmp(format, "BLUR") == 0) {
		blur(m, matrix); // Apply blur detection
	} else if (strcmp(format, "SHARPEN") == 0) {
		sharpen(m, matrix); // Apply sharpen
	} else if (strcmp(format, "GAUSSIAN_BLUR") == 0) {
		gausian_blur(m, matrix); // Apply Gaussian blur
	} else {
		printf("APPLY parameter invalid\n"); // Check if the filter is valid
	}
}

int main(void)
{
	image *matrix = NULL; // Initialize the image
	select_t selection; // Initialize the selection
	selection.select = false; // Set the selection flag
	char filename[40], buffer[256], command[20];
	int ok = 1, z = 0; z++;
	while (ok != 0) {
		fgets(buffer, 256, stdin); // Read the command
		z = sscanf(buffer, "%s", command);
		if (strcmp(command, "LOAD") == 0) {
			if (matrix) { // Check if an image is already loaded
				displace_image(matrix); // Free the memory
			}
			sscanf(buffer, "LOAD %s", filename); // Read the filename
			selection.select = false; // Reset the selection flag
			matrix = load(filename); // Load the image
		} else if (strcmp(command, "SAVE") == 0) {
			char format[6]; // Initialize the format
			int res = sscanf(buffer, "SAVE %s %s", filename, format);//filename
			if (res == 2 && strcmp(format, "ascii") == 0) { // Check the format
				save(matrix, filename, true); // Save the image in ASCII format
			} else if (res == 1) { // Check the format
				save(matrix, filename, false); //Save the image in binary
			}
		} else if (strcmp(command, "EXIT") == 0) {
			ok = 0; // Exit the program
			if (matrix) // Check if an image is loaded
				displace_image(matrix); // Free the memory
			else
				printf("No image loaded\n"); // Returns an error
		} else if (strcmp(command, "ROTATE") == 0) {
			int angle; // Initialize the angle
			z = sscanf(buffer, "ROTATE %d", &angle); // Read the angle
			rotate(matrix, angle, &selection); // Rotate the image
		} else if (strcmp(command, "APPLY") == 0) {
			if (!matrix) { // Check if an image is loaded
				printf("No image loaded\n"); // Returns an error
			} else {
				char filter[20];
				int ret = sscanf(buffer, "APPLY %s", filter); // Read the filter
				if (ret == 1) // Check the filter
					apply(&selection, matrix, filter); // Apply the filter
				else
					printf("Invalid command\n"); // Returns an error
			}
		} else if (strcmp(command, "EQUALIZE") == 0) {
			equalize(matrix); // Equalize the image
		} else if (strcmp(command, "HISTOGRAM") == 0) {
			if (!matrix) { // Check if an image is loaded
				printf("No image loaded\n");
		}	else {
			int x, y, z, res; // Initialize the parameters
			res = sscanf(buffer, "HISTOGRAM %d %d %d", &x, &y, &z); // Read
				if (res == 2)
					histogram(matrix, x, y); // Calculate the histogram
			else
				printf("Invalid command\n"); // Returns an error
			}
		} else if (strcmp(command, "CROP") == 0) {
			crop(&selection, matrix); // Crop the image
		} else if (strcmp(command, "SELECT") == 0) {
			char filter[20];
			z = sscanf(buffer, "SELECT %s", filter); // Read the select type
			if (strcmp(filter, "ALL") == 0) { // Check the select type
				select_all(&selection, matrix); // Select the entire image
			} else {
				int x1, y1, x2, y2, res; // Initialize the coordinates
				res = sscanf(buffer, "SELECT %d %d %d %d", &x1, &y1, &x2, &y2);
				if (res == 4)
					select_(x1, y1, x2, y2, &selection, matrix); // Select
				else
					printf("Invalid command\n"); // Returns an error
			}
		} else {
			printf("Invalid command\n"); // Returns an error
		}
	}

	return 0;
}
