#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

/* thread routine */
void *func(void* arg) {
    int i = *((int*)arg);
    printf("Thread %d start.\n", i);
    if (i % 2 == 1) {
        sleep(3);
        printf("Thread %d wake up.\n", i);
    }
    else printf("Thread %d end.\n", i);
    pthread_exit(0);
}

int main() {
    pthread_t tid[3];

    int data[3];
    for (int i = 0; i < 3; ++i) {
        data[i] = i+1;
        pthread_create(&tid[i], NULL, func, (void*)(&data[i]));
    }
    for (int i = 0; i < 3; ++i) {
        pthread_join(tid[i], NULL);
    }
    printf("Ending of Main...\n");
}