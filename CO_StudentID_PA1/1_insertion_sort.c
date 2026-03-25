#include <stdio.h>
#include <string.h>

/* TODO: Implement your code below */
int insertionSort(float *p_a, int arr_size)
{
    int shift_cnt = 0;  /* shift count */

    for (int i = 1; i < arr_size; i++) {
        float *p_i = p_a + i;  /* index of i-th element */
        int j = i - 1;
        
        asm volatile(
            "flw f0, 0(%[p_i])\n\t" // f0 = key
            "while_loop:\n\t"
                "bltz %[j], end\n\t"
                "slli t0, %[j], 2\n\t" // t0 = j * 4
                "add t0, t0, %[p_a]\n\t" 
                "flw f1, 0(t0)\n\t" // f1 = A[j]
                "fle.s t1, f1, f0\n\t"
                "bnez t1, end\n\t"
                "fsw f1, 4(t0)\n\t"
                "addi %[j], %[j], -1\n\t"
                "addi %[shift_cnt], %[shift_cnt], 1\n\t"
                "j while_loop\n\t"
            "end:\n\t"
                "slli t0, %[j], 2\n\t" // t0 = j * 4
                "add t0, t0, %[p_a]\n\t" 
                "fsw f0, 4(t0)\n\t"
            : [p_i] "+r" (p_i), [j] "+r" (j), [p_a] "+r" (p_a), [shift_cnt] "+r" (shift_cnt)
            : 
            : "t0", "t1", "f0", "f1", "memory"
        );
    }
    
    return shift_cnt;
}

int main(int argc, char *argv[])
{
    FILE *input = stdin;
    FILE *output_file = NULL;
    float target = -1;
    
    if (argc >= 2) {
        input = fopen(argv[1], "r");
        if (!input) {
            fprintf(stderr, "Error opening file: %s\n", argv[1]);
            return 1;
        }
    }
    
    // Read 'target' from the input file of problem 2 (2_*.txt) first
    if (argc >= 3) {
        output_file = fopen(argv[2], "r");
        if (output_file) {
            fscanf(output_file, "%f", &target);
            fclose(output_file);
        }
    }
    
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
    
    // Call insertion sort function
    int shift_cnt = insertionSort(p_a, arr_size);
    
    p_a = &arr[0];
    
    // Write to output file
    if (argc >= 3 && argv[2]) {
        output_file = fopen(argv[2], "w");
        if (output_file) {
            // target
            fprintf(output_file, "%.1f\n", target);
            // arr_size
            fprintf(output_file, "%d\n", arr_size);
            // sorted array
            for (int i = 0; i < arr_size; i++) {
                fprintf(output_file, "%.1f ", arr[i]);
            }
            fprintf(output_file, "\n");
            fclose(output_file);
        }
    }
    
    // Print the result
    for (int i = 0; i < arr_size; i++) {
        printf("%.1f ", *p_a++);
    }
    printf("\n");
    printf("%d\n", shift_cnt);
    return 0;
}
