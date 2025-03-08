#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE *image = fopen("pics.jpeg", "rb");  // Open in binary read mode
    if (!image) {
        fprintf(stderr, "Error: could not open input file.\n");
        return 1;
    }

    // Open for append in binary mode so we don't overwrite existing data
    FILE *binary = fopen("binary_image_data.txt", "wb");
    if (!binary) {
        fprintf(stderr, "Error: could not open output file.\n");
        fclose(image);
        return 1;
    }

    int ch;
    // Read until EOF
    while ((ch = fgetc(image)) != EOF) {
        fputc(ch, binary);
    }

    printf("Image successfully converted into binary file.\n");

    fclose(image);
    fclose(binary);

    return 0;
}
