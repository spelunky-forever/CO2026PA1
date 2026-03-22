#include <stdio.h>

/* TODO: Implement your code below */
int binarySearch(float *p_a, int arr_size, float target)
{
    int result = -1;

    asm volatile(/*Assembly code*/);

    return result;
}

int main(int argc, char *argv[])
{
    FILE *input = stdin;
    
    if (argc >= 2) {
        input = fopen(argv[1], "r");
        if (!input) {
            fprintf(stderr, "Error opening file: %s\n", argv[1]);
            return 1;
        }
    }
    
    // Read 'target'
    float target;
    fscanf(input, "%f", &target);
    
    // Read 'arr_size'
    int arr_size;
    fscanf(input, "%d", &arr_size);
    float arr[arr_size];

    // Read 'floats' from input into the array
    for (int i = 0; i < arr_size; i++) {
        float data;
        fscanf(input, "%f", &data);
        arr[i] = data;
    }
    
    if (argc >= 2) {
        fclose(input);
    }

    float *p_a = &arr[0];

    int index = binarySearch(p_a, arr_size, target);

    // Print the result
    printf("%d ", index);
    printf("\n");

    return 0;
}
