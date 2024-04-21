#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *out_func(void* arg) {
    int i = *((int*)arg);
    printf("[out] I recive %d.\n", i);
    pthread_exit(0);
}

void *in_func(void* arg) {
    int i = *((int*)arg);
    printf("[in] I recive %d.\n", i);
    pthread_exit(0);
}

int main() {
    pthread_t out_tid[5], in_tid[5];

    // for문 밖에서 정의
    int out_data[5];
    for (int i = 0; i < 5; ++i) {
        out_data[i] = i;
        pthread_create(&out_tid[i], NULL, out_func, (void*)(&out_data[i]));
    }
    for (int i = 0; i < 5; ++i) {
        pthread_join(out_tid[i], NULL);
    }

    sleep(3);

    // for문 안에서 정의
    for (int i = 0; i < 5; ++i) {
        int in_data = i;
        pthread_create(&in_tid[i], NULL, in_func, (void*)(&in_data));
    }

    for (int i = 0; i < 5; ++i) {
        pthread_join(in_tid[i], NULL);
    }

    printf("Main is End.\n");
    return 0;
}