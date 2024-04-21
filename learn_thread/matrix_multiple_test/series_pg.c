#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int n, **mat1, **mat2, **mat3;

void initData() {
    FILE* fp = fopen("input.txt", "r");
    if (fp == NULL) {
        perror("Failed to open file");
        exit(1);
    }
    
    // 행렬 크기
    fscanf(fp, "%d", &n);

    // 배열 동적할당
    mat1 = (int**) malloc(sizeof(int*) * n);
    mat2 = (int**) malloc(sizeof(int*) * n);
    mat3 = (int**) malloc(sizeof(int*) * n);
    for (int i = 0; i < n; ++i) {
        mat1[i] = (int*) malloc(sizeof(int) * n);
        mat2[i] = (int*) malloc(sizeof(int) * n);
        mat3[i] = (int*) malloc(sizeof(int) * n);
    }

    // 입력받기
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) fscanf(fp, "%d", &mat1[i][j]);
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) fscanf(fp, "%d", &mat2[i][j]);

    fclose(fp);
}

int main(int argc, char* argv[]) {
    initData();

    // 시간 측정 시작
    clock_t start = clock();

    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) {
        mat3[i][j] = 0;
        for (int k = 0; k < n; ++k) mat3[i][j] += mat1[i][k] * mat2[k][j];
    }

    // 시간 측정 종료
    clock_t end = clock();

    // 인자가 더 들어오면 답 보기
    if (argc > 1) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) printf("%d ", mat3[i][j]);
            printf("\n");
        }
    }
    printf("Time: %lf\n", (double)(end - start)/CLOCKS_PER_SEC);
}