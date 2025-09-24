<p align="center">Image editor</p>

****This is a basic CLI image editor.****

Technologies used: dynamic memory allocation, file operations, pointers.
It is composed of two programs:

**convert.c** – This program converts a JPEG image to PNG format, making it compatible for further operations in the next program.

**image_check.c** – This program performs multiple operations on an image, including:

 - Reading the image.
 - Saving the image.
 - Zooming in on the image.
 - Applying multiple filters.
 - Rotating the image.
 - Performing the read, zoom-in, and filter operations on a selected region of the image only.

## How to use:
 - git clone https://github.com/Tibi7110/Image-Editor.git
 - cd Image-Editor
 - make

**Old README:**
      
// 312CA Chirila Tiberiu Andrei
    
    The program is given an image and a variety of functions from stdin
    like: load, save, crop, apply, equalize, histogram, rotate,
    select, select all and exit. Each function is applied on the image or
    is exiting with an error code corresponding to the error appeared.

    The images are readed from a file (text or binary) and are stored in
    a dinamically allocated matrix type of data and deallocated once the program is
    finished.

    The functions are applied on the image and the result is stored in a new
    image. The new image is stored in a file (text or binary) and the memory is deallocated.

  How the program works:
  -The program reads the input from stdin.
  -The program reads the image from a file.
  -The program applies the functions given.
  -The program stores the new image in a file.
  -The program deallocates the memory.

    The program is structured in functions and each function has a specific role
    and is called in the main function. The functions are described in the following 
    lines.

    The program is structured in the following way:

    - The image structure contains the image data.
    - The selection structure contains the selection data.
    - The load function loads an image from a file.
    - The save function saves an image to a file.
    - The displace_image function frees the memory for the image data.
    - The swap_int function swaps two int types of data.
    - The select_ function verifies if the x1, x2, y1, y2 variables are within the image matrix borders.
    - The select_all function applies the change into the selection matrix.
    - The verify_y function verifies if y is between 2 and 255 and if it is a power of 2.
    - The clamp function clamps the x value.
    - The histogram function prints the histogram of the image.
    - The equalize function equalizes the image.
    - The rotate_clockwise function rotates the image clockwise.
    - The rotate function rotates the image with a given angle.
    - The crop function crops the image.
    - The apply function applies the value to the selection matrix.
    - The edge function applies the edge function on the image.
    - The blur function applies the blur function on the image.
    - The sharpen function applies the sharpen function on the image.
    - The gausian_blur function applies the gausian blur function on the image.
    - The main function is the main function of the program.

    The functions are described in the following lines.

///

The image *load(char *filename) function :

    This program contains a function 'load' to load image files in PGM/PPM formats.
    Supported formats:
    - P2: Grayscale (Text-based)
    - P5: Grayscale (Binary-based)
    - P3: RGB Color (Text-based)
    - P6: RGB Color (Binary-based)

**Function Overview**
image *load(char *filename)
    - Loads an image from a given file.
    - Parses the PGM/PPM header to determine the format, dimensions, and max value.
    - Allocates memory for the image data dynamically.
    - Populates the image structure with pixel values.

**Parameters**
  - filename: The path to the image file to be loaded.

**Return Value**
  - Returns a pointer to an image structure containing the loaded data.
  - If the file cannot be loaded, it prints an error message and returns NULL.

**File Format Details**
  - The first line specifies the format .
  - The second line specifies the image dimensions: width and height.
  - The third line specifies the maximum pixel value (e.g., 255).
  - The rest of the file contains pixel data:
      - For P2 and P5: Single grayscale values for each pixel.
      - For P3 and P6: Three values (R, G, B) for each pixel.

**How It Works**
  1. Opens the file in binary mode.
  2. Reads the format from the header.
  3. Reads the dimensions (width, height) and maximum pixel value (max_value).
  4. Allocates memory for the pixel data dynamically based on dimensions.
  5. Loads pixel values into the 'matrix->data' structure.
  6. Closes the file and returns the loaded image.

**Error Handling**
  - Prints an error if the file cannot be opened.
  - Returns NULL if the file is unsupported or corrupted.

**Note**
  The current implementation only supports text-based PGM/PPM formats.

///

The void save(image *matrix, char *filename, bool ascii) :

    This program contains a function 'save' to save image files in PGM/PPM formats.
    Supported formats:
    - P2: Grayscale (Text-based)
    - P5: Grayscale (Binary-based)
    - P3: RGB Color (Text-based)
    - P6: RGB Color (Binary-based)

**Function Overview**
void save(image *matrix, char *filename, bool ascii)
    - saves an image in a given file.
    - Parses the image *matrix parameter to determine the format, dimensions, and max value.
    - Parses the char *filename parameter for the file.
    - Saves the image structure with pixel values.

**Parameters**
  - image *matrix: The image to be saved.
  - filename: The path to the image file to be loaded.
  - ascii: The format of the file.

**File Format Details**
  - The first line specifies the format .
  - The second line specifies the image dimensions: width and height.
  - The third line specifies the maximum pixel value (e.g., 255).
  - The rest of the file contains pixel data:
      - For P2 and P5: Single grayscale values for each pixel.
      - For P3 and P6: Three values (R, G, B) for each pixel.

**How It Works**
  1. Opens the file in text or binary mode.
  2. Saves the format from the header.
  3. Saves the dimensions (width, height) and maximum pixel value (max_value).
  5. Saves pixel values into the file.
  6. Closes the file.

**Note**
  The current implementation only supports text-based PGM/PPM formats.

///

The void displace_image(image *matrix) function:

**Function Overview**
void displace_image(image *matrix)
    - Frees the memory for the image data.

**Parameters**
  - filename: The path to the image file to be freed.

**How It Works**
  1. Iterates through the 3D data array and frees each allocated pixel
  2. After the itaretaion is done it frees the entire matrix

///

The void swap_int(int *a, int *b) function:

**Function Overview**
void swap_int(int *a, int *b)
    - swaps two int types of data.

**Parameters**
  - int *a, int *b : the variables to be swaped.

**How It Works**
  1. It swaps the values of the variables using a temp variable

///

The void select_(int x1, int y1, int x2, int y2, select_t *matrix, image *m) function:

**Function Overview**
void select_(int x1, int y1, int x2, int y2,select_t *matrix, image *m)
    - Verifies if the x1, x2, y1, y2 variables are within the image matrix borders.
    - Applies the change into the selection matrix.

**Parameters**
  - int x1, x2, y1, y2 : the variables to be used.
  - select_t *matrix : the place where to populate the variables.
  - image *m : the borders of the matrix.

**Return Value**
  - Returns a pointer to an selection structure containing the loaded data.
  - If the file cannot be loaded, it prints an error message and returns NULL.

**How It Works**
  1. It swaps the values of the variables to respect the order x1 < x2, y1 < y2.
  2. It populates the selection matrix with the data.

**Error Handling**
  - Prints an error if the values are not within the border.

///

The void select_all(select_t *matrix, image *m) function:

**Function Overview**
void select_all(select_t *matrix, image *m)
    - Applies the change into the selection matrix.

**Parameters**
  - select_t *matrix : the place where to populate the variables.
  - image *m : the borders of the matrix.

**Return Value**
  - Returns a pointer to an selection structure containing the loaded data.

**How It Works**
  1. It populates the selection matrix with the data.

///

The int verify_y(int y) function:

**Function Overview**
int verify_y(int y)
    - Verifies if y is between 2 and 255 and if it is a power of 2.

**Parameters**
  - int y : the data to be evaluated.

**Return Value**
  - Returns 1 if the conditions are not met.
  - Returns 0 if the conditons are met.

///

The int clamp(int x) function:

**Function Overview**
int int clamp(int x)
    - clamps the x value.

**Parameters**
  - int x : the data to be evaluated.

**Return Value**
  - Returns x if x is between 0 and 255.
  - Returns 0 if x < 0.
  - Returns 255 if x > 255.

///

The histogram(image *matrix) function:

**Function Overview**
void histogram(image *matrix)
    - Prints the histogram of the image.

**Parameters**
  - image *matrix : the image to be evaluated.

**Return Value**
  - Returns the histogram of the image.
  - Returns an error if the image is not loaded or it is not black and white format.

**How It Works**
  1. It creates an array of 256 elements and populates it with 0.
  2. It iterates through the image and increments the value of the array at the position of the pixel value.
  3. It prints the histogram.

**Error Handling**
  - Prints an error if the image is not loaded.
  - Prints an error if the image is not black and white format.

///

The voi equalize(image *matrix) function:

**Function Overview**
void equalize(image *matrix)
    - Equalizes the image.

**Parameters**
  - image *matrix : the image to be equalized.

**Return Value**
  - Returns the equalized image.
  - Returns an error if the image is not loaded or it is not black and white format.

**How It Works**
  1. It creates an array of 256 elements and populates it with 0.
  2. It iterates through the image and increments the value of the array at the position of the pixel value.
  3. It creates an array of 256 elements and populates it with the sum of the previous array.
  4. It iterates through the image and calculates the new pixel value.
  5. It loads the equalized image.

**Error Handling**
  - Prints an error if the image is not loaded.
  - Prints an error if the image is not black and white format.

///

The void rotate_clockwise(image *matrix) function:

**Function Overview**
void rotate_clockwise(image *matrix)
    - Rotates the image clockwise.

**Parameters**
  - image *matrix : the image to be rotated.

**Return Value**
  - Returns the rotated image.

**How It Works**
  1. It creates a new image.
  2. It iterates through the image and populates the new image with the rotated pixels.
  3. It loads the rotated image.

///

The void rotate(image *matrix, int angle) function:

**Function Overview**
void rotate(image *matrix, int angle)
    - Rotates the image with a given angle.

**Parameters**
  - image *matrix : the image to be rotated.
  - int angle : the angle to be rotated.

**Return Value**
  - Returns the rotated image.
  - Returns an error if the conditions are not met.

**How It Works**
  1. It verifies the initial conditions.
  2. It calls the functions needed to rotate the image with the given angle.

**Error Handling**
  - Prints an error if the image is not loaded.
  - Prints an error if the angle is not 0, +-90, +-180, +-270, +- 360.

///

The void crop(image *matrix, select_t *s) function:

**Function Overview**
void crop(image *matrix, Selection *s)
    - Crops the image.

**Parameters**
  - image *matrix : the image to be cropped.
  - select_t *s : the selection matrix.

**Return Value**
  - Returns the cropped image.
  - Returns an error if the conditions are not met.

**How It Works**
  1. It verifies the initial conditions.
  2. It creates a new image.
  3. It iterates through the image and populates the new image with the cropped pixels.
  4. It loads the cropped image.

**Error Handling**
  - Prints an error if the image is not loaded.
  - Prints an error if the selection matrix is not populated.

///

The void apply(image *matrix, select_t *s, int value) function:

**Function Overview**
void apply(image *matrix, select_t *s, int value)
    - Applies the value to the selection matrix.

**Parameters**
  - image *matrix : the image to be applied.
  - select_t *s : the selection matrix.
  - int value : the value to be applied.

**Return Value**
  - Returns the applied image.
  - Returns an error if the conditions are not met.

**How It Works**
  1. It verifies the initial conditions.
  2. It calls the function needed to apply the change.
  3. It loads the changed image.

**Error Handling**
  - Prints an error if the image is not loaded.
  - Prints an error if the image is not RGB.
  - Prints an error if the paramet is invalid.

///

The edge(select_t *m, image *matrix) function:

**Function Overview**
void edge(select_t *m, image *matrix)
    - Applies the edge function on the image.

**Parameters**
  - select_t *m : the selection matrix.
  - image *matrix : the image to be applied.

**Return Value**
  - Returns the image with the edge applied.

**How It Works**
  1. It creates a new image.
  2. It iterates through the image and applies the edge function.
  3. It loads the changed image.

///

The void blur(select_t *m, image *matrix) function:

**Function Overview**
void blur(select_t *m, image *matrix)
    - Applies the blur function on the image.

**Parameters**
  - select_t *m : the selection matrix.
  - image *matrix : the image to be applied.

**Return Value**
  - Returns the image with the blur applied.

**How It Works**
  1. It creates a new image.
  2. It iterates through the image and applies the blur function.
  3. It loads the changed image.

///

The void sharpen(select_t *m, image *matrix) function:

**Function Overview**
void sharpen(select_t *m, image *matrix)
    - Applies the sharpen function on the image.

**Parameters**
  - select_t *m : the selection matrix.
  - image *matrix : the image to be applied.

**Return Value**
  - Returns the image with the sharpen applied.

**How It Works**
  1. It creates a new image.
  2. It iterates through the image and applies the sharpen function.
  3. It loads the changed image.

///

The void gausian_blur(select_t *m, image *matrix) function:

**Function Overview**
void gausian_blur(select_t *m, image *matrix)
    - Applies the gausian blur function on the image.

**Parameters**
  - select_t *m : the selection matrix.
  - image *matrix : the image to be applied.

**Return Value**
  - Returns the image with the gausian blur applied.

**How It Works**
  1. It creates a new image.
  2. It iterates through the image and applies the gausian blur function.
  3. It loads the changed image.

///

The int main() function:

**Function Overview**
int main()
    - The main function of the program.

**Parameters**
  - None.

**Return Value**
  - Returns 0 if the program runs successfully.
  - Returns an error code if the program fails.

**How It Works**
  1. It reads the input from stdin.
  2. It calls the functions given.
