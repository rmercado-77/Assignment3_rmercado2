#include <stdio.h>
#include <stdlib.h>
#include "array.h"

// Function declarations. Each function receives a pointer to an Array.
void output_array(Array *a);
void shift_array(Array *a);
Array *average_adjacent(Array *a);

int main(int argc, char *argv[])
{
    Array *original;
    Array *averages;
    int requested_size;
    int i;

    // Check that the user entered exactly one command-line argument -- argv[1] will contain the requested array size
    if (argc != 2)
    {
        printf("Usage: %s <array size>\n", argv[0]);
        return 1;
    }

    // Convert the command-line argument from text to an integer. The array size must be greater than zero.
    requested_size = atoi(argv[1]);

    if (requested_size <= 0)
    {
        printf("Error: array size must be a positive integer.\n");
        return 1;
    }

    //  Dynamically allocate the Array structure itself, then assign the user-selected size into the structure.
    original = (Array *) malloc(sizeof(Array));

    if (original == NULL)
    {
        printf("Unable to allocate Array structure.\n");
        return 1;
    }

    original->size = requested_size;

    // Dynamically allocate the double array stored in original->data. This is separate from allocation of the structure.
    original->data =
        (double *) malloc(sizeof(double) * original->size);

    if (original->data == NULL)
    {
        printf("Unable to allocate array data.\n");
        free(original);
        return 1;
    }

    // Fill the original array with values
    for (i = 0; i < original->size; i++)
    {
        original->data[i] = i + 1;
    }

    // Displays the original values before changing the array.
    printf("Original array:\n");
    output_array(original);

    // Now to display the shifted array of every value left by one position.
    shift_array(original);

    printf("\nShifted array:\n");
    output_array(original);

    // Then we can create a second dynamically allocated Array. It will contain averages of neighboring pairs from the shifted original array.
    averages = average_adjacent(original);

    if (averages == NULL)
    {
        printf("Unable to allocate average array.\n");

        free(original->data);
        free(original);

        return 1;
    }

    // To display the new array returned by average_adjacent.
    printf("\nAdjacent averages:\n");
    output_array(averages);

    //Then to free the data arrays before freeing their structures. Each malloc allocation must have one matching free
    free(averages->data);
    free(averages);

    free(original->data);
    free(original);

    return 0;
}

// Here we can print every element in the Array structure. The function uses a pointer so it can access a->size and a->data.
void output_array(Array *a)
{
    int i;

    for (i = 0; i < a->size; i++)
    {
        printf("array[%d] = %.2f\n", i, a->data[i]);
    }
}

/* In this function we rotate the array left by one position. Save index 0,
 shift all other elements left, then place the saved first value into the last index. */
void shift_array(Array *a)
{
    double first_value;
    int i;

    first_value = a->data[0];

    for (i = 0; i < a->size - 1; i++)
    {
        a->data[i] = a->data[i + 1];
    }

    a->data[a->size - 1] = first_value;
}

/* For the average adjacent function  we create and return a new Array containing averages of adjacent pairs.
 If the source array has an odd size, its last unpaired element is ignored.*/
Array *average_adjacent(Array *a)
{
    Array *new_array;
    int i;

    // Allocates the new Array structure
    new_array = (Array *) malloc(sizeof(Array));

    if (new_array == NULL)
    {
        return NULL;
    }

    // Integer division makes the new array half the original size, ie: 5 becomes 2 and so forth
    new_array->size = a->size / 2;

    // Allocates the storage for the averaged values.
    new_array->data =
        (double *) malloc(sizeof(double) * new_array->size);

    if (new_array->data == NULL)
    {
        free(new_array);
        return NULL;
    }

    // Average the pairs (0,1), (2,3), (4,5), and so forth. The expression 2 * i selects the first index of each pair.
    for (i = 0; i < new_array->size; i++)
    {
        new_array->data[i] =
            (a->data[2 * i] + a->data[2 * i + 1]) / 2.0;
    }

    return new_array;
}
