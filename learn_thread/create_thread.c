#include <stdio.h>
#include <pthread.h>

#define REPEAT 10

/* thread routine 1 */
void *func1(void *args) {
    int i = *((int*)(args));
    printf("Phrase %d: 0. (%li)\n", i, pthread_self());
    pthread_exit(0);
}

/* thread routine 2 */
void *func2(void *args) {
    int i = *((int*)(args));
    printf("Phrase %d: 1. (%li)\n", i, pthread_self());
    pthread_exit(0);
}

/* thread routine 3 */
void *func3(void *args) {
    int i = *((int*)(args));
    printf("Phrase %d: 2. (%li)\n", i, pthread_self());
    pthread_exit(0);
}

/* thread routine 4 */
void *func4(void *args) {
    int i = *((int*)(args));
    printf("Phrase %d: 3. (%li)\n", i, pthread_self());
    pthread_exit(0);
}

/* thread routine 5 */
void *func5(void *args) {
    int i = *((int*)(args));
    printf("Phrase %d: 4. (%li)\n", i, pthread_self());
    pthread_exit(0);
}

/* thread routine 6 */
void *func6(void *args) {
    int i = *((int*)(args));
    printf("Phrase %d: 5. (%li)\n", i, pthread_self());
    pthread_exit(0);
}

/* thread routine 7 */
void *func7(void *args) {
    int i = *((int*)(args));
    printf("Phrase %d: 6. (%li)\n", i, pthread_self());
    pthread_exit(0);
}

/* thread routine 8 */
void *func8(void *args) {
    int i = *((int*)(args));
    printf("Phrase %d: 7. (%li)\n", i, pthread_self());
    pthread_exit(0);
}

/* thread routine 9 */
void *func9(void *args) {
    int i = *((int*)(args));
    printf("Phrase %d: 8. (%li)\n", i, pthread_self());
    pthread_exit(0);
}

/* thread routine 10 */
void *func10(void *args) {
    int i = *((int*)(args));
    printf("Phrase %d: 9. (%li)\n", i, pthread_self());
    pthread_exit(0);
}

int main() {
    void* funcs[10] = {func1, func2, func3, func4, func5, func6, func7, func8, func9, func10};
    pthread_t tid[10];

    int temps[10];
    for (int i = 0; i < 10; ++i) {
        temps[i] = i;
        if (pthread_create(&tid[i], NULL, funcs[i], (void*)(&temps[i]))) {
            printf("Error in pthread creation.\n");
            return 0;
        }
    }

    int x = 10;
    for (int i = 1; i <= 1000000; ++i) {
        int y = x;
        x = y;
    }

    // for (int i = 0; i < 10; ++i) {
    //     pthread_join(tid[i], NULL);
    // }
    
    return 0;
}