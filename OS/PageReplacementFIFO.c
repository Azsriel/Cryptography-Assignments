#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, slen;
    printf("Enter the number of frames: ");
    scanf("%d", &n);
    printf("Enter the length of working set window of processes: ");
    scanf("%d", &slen);

    // For Printing
    int** output = (int**) malloc(sizeof(int*) * slen);

    int* input = (int*) malloc(sizeof(int) * slen);
    for (int i = 0; i<slen; ++i) {
        scanf("%d", &input[i]);
    }

    int* pages = (int*) malloc(sizeof(int) * n);
    for (int i = 0; i<n; ++i) {
        pages[i] = -1;
    }
    int nextToReplace = 0;

    int pageHits = 0;

    for (int i = 0; i<slen; ++i) {
        int next = input[i];
        int found = 0;
        for (int j = 0; j<n; ++j) {
            if (pages[j] == next) {
                pageHits++;
                found = 1;
            }
        }
        if (found == 0) {
            pages[nextToReplace] = next;
            nextToReplace = (nextToReplace + 1) % n; // FIFO
        }

        output[i] = (int*) malloc (sizeof(int) * n);
        for (int j = 0; j<n; ++j) {
            output[i][j] = pages[j];
        }
        // for (int j = 0; j<n; ++j ){
        //     printf("%d ", pages[j]);
        // }
        // printf("\n");
    }

    // Print output:
    printf("| ");
    for (int i = 0; i<slen; ++i) {
        printf("%d | ", input[i]);
    }
    printf("\n");
    for (int i = 0; i<n; ++i) {
        printf("\n| "); 
        for (int j = 0; j<slen; ++j) {
            if (output[j][i] != -1)
                printf("%d | ", output[j][i]);
            else 
                printf("E | " );
        }
    } 
    printf("\nPage Faults: %d\n", slen - pageHits);
    printf("Page Hits: %d\n", pageHits);
    printf("Page Replacements: %d\n", slen - pageHits - 4);
} 