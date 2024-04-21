#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]) {
    if (argc == 1) {
        printf("No parameter.\n");
        return 0;
    }

    FILE* fp = fopen("input.txt", "w");
    srand(time(NULL));

    int n = atoi(argv[1]);    

    fprintf(fp, "%d\n", n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            fprintf(fp, "%d ", rand()%2);
        }
        fprintf(fp, "\n");
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            fprintf(fp, "%d ", rand()%2);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}