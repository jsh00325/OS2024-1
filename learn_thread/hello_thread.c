#include <stdio.h>
#include <pthread.h>

/* thread routine */
void *hello(void *vargp) {
    printf("Hello World!\n");
    return NULL;
}

int main() {
    pthread_t tid1, tid2;

    pthread_create(&tid1, NULL, hello, NULL);
    pthread_create(&tid2, NULL, hello, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;    
}