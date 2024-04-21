#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

typedef struct { int i, j; } Point;
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

/* thread routine */
void *multiple_all(void* arg) {
    Point data = *((Point*)arg);

    mat3[data.i][data.j] = 0;
    for (int k = 0; k < n; ++k) mat3[data.i][data.j] += mat1[data.i][k] * mat2[k][data.j];

    pthread_exit(0);
}

int main(int argc, char* argv[]) {
    initData();

    pthread_t **tid = (pthread_t**) malloc(sizeof(pthread_t*) * n);
    Point **data = (Point**) malloc(sizeof(Point*) * n);

    for (int i = 0; i < n; ++i) {
        tid[i] = (pthread_t*) malloc(sizeof(pthread_t) * n);
        data[i] = (Point*) malloc(sizeof(Point) * n);
    }

    // 시간 측정 시작
    clock_t start = clock();

    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) {
        data[i][j].i = i, data[i][j].j = j;
        pthread_create(&tid[i][j], NULL, multiple_all, (void*)(&data[i][j]));
    }

    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) {
        pthread_join(tid[i][j], NULL);
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