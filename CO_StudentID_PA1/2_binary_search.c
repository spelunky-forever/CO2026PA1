#include <stdio.h>

/* TODO: Implement your code below */
int binarySearch(float *p_a, int arr_size, float target)
{
    int result = -1;

    asm volatile(
        "li t0, 0\n\t" // t0 = L
        "addi t1, %[arr_size], -1\n\t" // t1 = R
        "while_loop:\n\t"
            "bgt t0, t1, end\n\t"
            "add t2, t0, t1\n\t" // t2 = mid
            "srli t2, t2, 1\n\t"
            "if1:\n\t"
                "slli t3, t2, 2\n\t" // t3 = mid * 4
                "add t3, t3, %[p_a]\n\t"
                "flw f0, 0(t3)\n\t" // f0 = A[mid]
                "feq.s t4, f0, %[target]\n\t"
                "beqz t4, if2\n\t"
                "mv %[result], t2\n\t"
                "j end\n\t"
            "if2:\n\t"
                "fge.s t4, f0, %[target]\n\t"
                "bnez t4, if3\n\t"
                "addi t0, t2, 1\n\t"
                "j while_loop\n\t"
            "if3:\n\t"
                "addi t1, t2, -1\n\t"
                "j while_loop\n\t"
        "end:\n\t"
        : [arr_size] "+r" (arr_size), [p_a] "+r" (p_a), [target] "+f" (target), [result] "+r" (result)
        :
        : "t0", "t1", "t2", "t3", "t4", "f0", "memory"
    );

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
