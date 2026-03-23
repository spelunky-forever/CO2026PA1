#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct ListNode
{
    int val;
    struct ListNode *next;
};

/* TODO: Implement your code below */
bool hasCycle(struct ListNode *head)
{
    struct ListNode *slow = head;
    struct ListNode *fast = head;
    struct ListNode *meet = NULL;
    bool result = false;

    asm volatile(
        "beqz %[head], end\n\t"
        "ld t0, 8(%[slow])\n\t" // t0 = slow.next
        "beqz t0, end\n\t"
        "while_loop:\n\t"
            "ld %[fast], 8(%[fast])\n\t" // fast = fast.next
            "beqz %[fast], end\n\t"
            "ld %[slow], 8(%[slow])\n\t" // slow = slow.next
            "ld %[fast], 8(%[fast])\n\t" // fast = fast.next
            "beqz %[fast], end\n\t"
            "beq %[slow], %[fast], meet\n\t"
            "j while_loop\n\t"
                
        "meet:\n\t"
            "mv %[meet], %[slow]\n\t"
            "li %[result], 1\n\t"    
        "end:\n\t"
        : [head] "+r" (head), [slow] "+r" (slow), [fast] "+r" (fast), [meet] "+r" (meet), [result] "+r" (result)
        :
        : "t0"
    );

    if (result) 
        printf("%d\n", meet->val);
    else
        printf("NULL\n");
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

    int node_count, edge_count;
    
    // Read first line: node_count and edge_count
    if (fscanf(input, "%d %d", &node_count, &edge_count) != 2) {
        fprintf(stderr, "Error reading node and edge count\n");
        if (argc >= 2) fclose(input);
        return 1;
    }

    if (node_count == 0) {
        printf("NULL\n");
        printf("false\n");
        if (argc >= 2) fclose(input);
        return 0;
    }

    // Allocate nodes array
    struct ListNode **nodes = (struct ListNode**)malloc(node_count * sizeof(struct ListNode*));
    
    // Read second line: node values and create nodes
    for (int i = 0; i < node_count; i++) {
        int val;
        if (fscanf(input, "%d", &val) != 1) {
            fprintf(stderr, "Error reading node value\n");
            if (argc >= 2) fclose(input);
            return 1;
        }
        nodes[i] = (struct ListNode*)malloc(sizeof(struct ListNode));
        nodes[i]->val = val;
        nodes[i]->next = NULL;
    }

    // Read edges and set next pointers
    for (int i = 0; i < edge_count; i++) {
        int from_val, to_val;
        if (fscanf(input, "%d %d", &from_val, &to_val) != 2) {
            fprintf(stderr, "Error reading edge\n");
            if (argc >= 2) fclose(input);
            return 1;
        }

        // Find 'from' and 'to' nodes by value
        struct ListNode *from_node = NULL;
        struct ListNode *to_node = NULL;

        for (int j = 0; j < node_count; j++) {
            if (nodes[j]->val == from_val) {
                from_node = nodes[j];
            }
            if (nodes[j]->val == to_val) {
                to_node = nodes[j];
            }
        }

        if (from_node && to_node) {
            from_node->next = to_node;
        }
    }

    if (argc >= 2) {
        fclose(input);
    }

    struct ListNode *head = (node_count > 0) ? nodes[0] : NULL;

    bool has_cycle = hasCycle(head);

    // Print the result
    if (has_cycle) {
        printf("true\n");
    } else {
        printf("false\n");
    }

    for (int i = 0; i < node_count; i++) {
        free(nodes[i]);
    }
    free(nodes);

    return 0;
}