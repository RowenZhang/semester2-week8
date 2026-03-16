/**
 * @file activity3-pgm_tools_refactored.c
 * @brief Tools for manipulating PGM (Portable Gray Map) image files
 *
 * COMP1850 Structures Activity 3
 *
 * This program reads, displays, inverts, rotates, and saves PGM images.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int width;
    int height;
    int max_gray;
    unsigned char **pixels;
} PGMImage;

unsigned char **allocate_image_array(int height, int width);
void free_image_array(unsigned char **image, int height);
PGMImage *read_pgm_image(const char *filename);
void print_image_info(PGMImage *image);
void print_image_values(PGMImage *image);
PGMImage *invert_image_colors(PGMImage *original_image);
PGMImage *rotate_image(PGMImage *original_image, int degrees);
unsigned char **rotate_image_array(unsigned char **original_image, int height, int width, int degrees);
int save_pgm_image(const char *filename, PGMImage *image);
void free_pgm_image(PGMImage *image);
int get_user_menu_choice(const char *message);
void display_menu(void);

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <image_path>\n", argv[0]);
        return 1;
    }

    PGMImage *image = read_pgm_image(argv[1]);
    if (image == NULL) {
        return 1;
    }

    print_image_info(image);

    int choice;
    char output_filename[100];
    int rotation_degrees;
    PGMImage *processed_image = NULL;

    do {
        choice = -1;
        display_menu();
        while (choice < 1)
            choice = get_user_menu_choice("Enter choice");

        switch (choice) {
        case 1:
            print_image_values(image);
            break;
        case 2:
            printf("Inverting image colours...\n");
            processed_image = invert_image_colors(image);
            if (processed_image == NULL) {
                printf("Failed to invert image\n");
                break;
            }

            printf("Enter output filename: ");
            scanf("%99s", output_filename);
            getchar();

            if (save_pgm_image(output_filename, processed_image)) {
                printf("Inverted image saved to %s\n", output_filename);
            } else {
                printf("Failed to save inverted image\n");
            }

            free_pgm_image(processed_image);
            processed_image = NULL;
            break;
        case 3:
            printf("Enter rotation in degrees (90, 180, or 270): ");
            scanf("%d", &rotation_degrees);
            getchar();

            if (rotation_degrees != 90 && rotation_degrees != 180 && rotation_degrees != 270) {
                printf("Invalid rotation angle. Please use 90, 180, or 270 degrees.\n");
                break;
            }

            printf("Rotating image by %d degrees...\n", rotation_degrees);
            processed_image = rotate_image(image, rotation_degrees);
            if (processed_image == NULL) {
                printf("Rotation failed\n");
                break;
            }

            printf("Enter output filename: ");
            scanf("%99s", output_filename);
            getchar();

            if (save_pgm_image(output_filename, processed_image)) {
                printf("Rotated image saved to %s\n", output_filename);
            } else {
                printf("Failed to save rotated image\n");
            }

            free_pgm_image(processed_image);
            processed_image = NULL;
            break;
        case 4:
            printf("Exiting program...\n");
            free_pgm_image(image);
            return 0;
        default:
            printf("Invalid choice, please try again\n");
            break;
        }
    } while (1);
}

/**
 * @brief Gets user menu choice from standard input
 * 
 * @param message The prompt message to display
 * @return int The user's choice, or -1 if invalid input
 */
int get_user_menu_choice(const char *message) {
    char buffer[100];
    int choice;
    printf("%s: ", message);
    fgets(buffer, 100, stdin);
    int check = sscanf(buffer, "%d", &choice);
    if (!check) {
        return -1;
    }
    return choice;
}

/**
 * @brief Displays the main program menu
 */
void display_menu(void) {
    printf("\n----- PGM Image Tool Menu -----\n");
    printf("1 - View PGM Image\n");
    printf("2 - Invert Image\n");
    printf("3 - Rotate Image\n");
    printf("4 - Quit\n");
}

/**
 * @brief Allocates a 2D array for storing the image pixels
 * 
 * @param height Number of rows in the image
 * @param width Number of columns in the image
 * @return unsigned char** 2D array of image pixels, or NULL if allocation fails
 */
unsigned char **allocate_image_array(int height, int width) {
    /* TODO: Refactor this to use your PGMImage structure */
    unsigned char **array = calloc(height, sizeof(unsigned char *));
    if (array == NULL) {
        printf("Error: Memory allocation failed for image array\n");
        return NULL;
    }
    
    for (int i = 0; i < height; i++) {
        array[i] = calloc(width, sizeof(unsigned char));
        if (array[i] == NULL) {
            /* Free previously allocated memory if this allocation fails */
            for (int j = 0; j < i; j++) {
                free(array[j]);
            }
            free(array);
            printf("Error: Memory allocation failed for image row %d\n", i);
            return NULL;
        }
    }

    return array;
}

/**
 * @brief Frees memory allocated for a 2D image array
 * 
 * @param image The 2D image array to free
 * @param height Number of rows in the image
 */
void free_image_array(unsigned char **image, int height) {
    /* TODO: Refactor this to use your PGMImage structure */
    if (image == NULL) return;
    
    for (int i = 0; i < height; i++) {
        free(image[i]);
    }
    free(image);
}

/**
 * @brief Reads a PGM image from a file
 * 
 * @param filename Path to the PGM file
 * @param height Pointer to store the image height
 * @param width Pointer to store the image width
 * @param max_gray Pointer to store the max gray value
 * @return unsigned char** 2D array of image pixels, or NULL if reading fails
 */
PGMImage *read_pgm_image(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", filename);
        return NULL;
    }

    char magic_number[3];
    if (fscanf(file, "%2s", magic_number) != 1) {
        printf("Error: Invalid PGM file format\n");
        fclose(file);
        return NULL;
    }

    if (strcmp(magic_number, "P2") != 0) {
        printf("Error: File is not a valid PGM file (P2 format)\n");
        fclose(file);
        return NULL;
    }

    int width, height, max_gray;
    if (fscanf(file, "%d %d", &width, &height) != 2) {
        printf("Error: Could not read image dimensions\n");
        fclose(file);
        return NULL;
    }

    if (fscanf(file, "%d", &max_gray) != 1) {
        printf("Error: Could not read max gray value\n");
        fclose(file);
        return NULL;
    }

    unsigned char **pixels = allocate_image_array(height, width);
    if (pixels == NULL) {
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pixel_value;
            if (fscanf(file, "%d", &pixel_value) != 1) {
                printf("Error: Could not read pixel value at position (%d,%d)\n", j, i);
                free_image_array(pixels, height);
                fclose(file);
                return NULL;
            }
            pixels[i][j] = (unsigned char)pixel_value;
        }
    }

    fclose(file);

    PGMImage *image = malloc(sizeof(PGMImage));
    if (image == NULL) {
        printf("Error: Memory allocation failed for PGMImage\n");
        free_image_array(pixels, height);
        return NULL;
    }

    image->width = width;
    image->height = height;
    image->max_gray = max_gray;
    image->pixels = pixels;

    return image;
}

void print_image_info(PGMImage *image) {
    if (image == NULL) return;
    printf("Image dimensions: %d x %d pixels\n", image->width, image->height);
    printf("Max Gray Value: %d\n", image->max_gray);
}

void print_image_values(PGMImage *image) {
    if (image == NULL || image->pixels == NULL) return;
    printf("\nImage Pixel Values (%dx%d):\n", image->width, image->height);
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            printf("%3d ", image->pixels[i][j]);
        }
        printf("\n");
    }
}

PGMImage *invert_image_colors(PGMImage *original_image) {
    if (original_image == NULL) return NULL;

    PGMImage *inverted = malloc(sizeof(PGMImage));
    if (inverted == NULL) {
        printf("Error: Memory allocation failed for inverted image\n");
        return NULL;
    }

    inverted->width = original_image->width;
    inverted->height = original_image->height;
    inverted->max_gray = original_image->max_gray;
    inverted->pixels = allocate_image_array(inverted->height, inverted->width);
    if (inverted->pixels == NULL) {
        free(inverted);
        return NULL;
    }

    for (int i = 0; i < inverted->height; i++) {
        for (int j = 0; j < inverted->width; j++) {
            inverted->pixels[i][j] = original_image->max_gray - original_image->pixels[i][j];
        }
    }

    return inverted;
}

unsigned char **rotate_image_array(unsigned char **original_image, int height, int width, int degrees) {
    unsigned char **rotated_image = NULL;

    switch (degrees) {
        case 90:
        case 270:
            rotated_image = allocate_image_array(width, height);
            break;
        case 180:
            rotated_image = allocate_image_array(height, width);
            break;
        default:
            printf("Error: Invalid rotation angle\n");
            return NULL;
    }

    if (rotated_image == NULL) {
        return NULL;
    }

    if (degrees == 90) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                rotated_image[j][height - 1 - i] = original_image[i][j];
            }
        }
    } else if (degrees == 180) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                rotated_image[height - 1 - i][width - 1 - j] = original_image[i][j];
            }
        }
    } else { // 270
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                rotated_image[width - 1 - j][i] = original_image[i][j];
            }
        }
    }

    return rotated_image;
}

PGMImage *rotate_image(PGMImage *original_image, int degrees) {
    if (original_image == NULL) return NULL;
    PGMImage *rotated = malloc(sizeof(PGMImage));
    if (rotated == NULL) {
        printf("Error: Memory allocation failed for rotated image\n");
        return NULL;
    }

    if (degrees == 90 || degrees == 270) {
        rotated->width = original_image->height;
        rotated->height = original_image->width;
    } else {
        rotated->width = original_image->width;
        rotated->height = original_image->height;
    }

    rotated->max_gray = original_image->max_gray;
    rotated->pixels = rotate_image_array(original_image->pixels, original_image->height, original_image->width, degrees);
    if (rotated->pixels == NULL) {
        free(rotated);
        return NULL;
    }

    return rotated;
}

int save_pgm_image(const char *filename, PGMImage *image) {
    if (image == NULL) return 0;

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Could not create output file %s\n", filename);
        return 0;
    }

    fprintf(file, "P2\n");
    fprintf(file, "%d %d\n", image->width, image->height);
    fprintf(file, "%d\n", image->max_gray);

    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            fprintf(file, "%d ", image->pixels[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    return 1;
}

void free_pgm_image(PGMImage *image) {
    if (image == NULL) return;
    free_image_array(image->pixels, image->height);
    free(image);
}

