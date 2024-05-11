#include <stdio.h>
#include <pthread.h>

#define NUM_THREAD 5

void *routine(void *arg) {
    int i = *((int*)arg);

    printf("Hello, I'm thread %d.\n", i);

    pthread_exit(0);
}

int main() {
    int scope;

    pthread_t tid[NUM_THREAD];
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    if (pthread_attr_getscope(&attr, &scope)) {
        fprintf(stderr, "Unable to get scheduling scope.\n");
        return 0;
    }

    if (scope == PTHREAD_SCOPE_PROCESS) {
        printf("PTHREAD_SCOPE_PROCESS\n");
    }
    else if (scope == PTHREAD_SCOPE_SYSTEM) {
        printf("PTHREAD_SCOPE_SYSTEM\n");
    }
    else {
        fprintf(stderr, "Illegal scope value.\n");
        return 0;
    }

    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    int temp_data[NUM_THREAD];
    for (int i = 0; i < NUM_THREAD; ++i) {
        temp_data[i] = i+1;
        if (pthread_create(&tid[i], NULL, routine, (void*)(&temp_data[i]))) {
            fprintf(stderr, "Unable to create thread %d.\n", (i+1));
            return 0;
        }
    }

    for (int i = 0; i < NUM_THREAD; ++i) {
        if (pthread_join(tid[i], NULL)) {
            fprintf(stderr, "Fail to join thread %d.\n", (i+1));
            return 0;
        }
    }

    printf("Main thread is End.\n");
    return 0;
}