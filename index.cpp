#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

typedef struct {
    int thread_id;
    int num_steps;
    double step_size;
    double a;
    double b;
    double result;
} ThreadData;

double f(double x) {
    return x * x * sin(x) + log(x + 1);  // Ваша функція
}

void* integrate(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    double sum = 0.0;
    double x;

    for (int i = data->thread_id; i < data->num_steps; i += data->num_steps) {
        x = data->a + (i + 0.5) * data->step_size;  // Використовуємо центр прямокутника
        sum += f(x);
    }
    data->result = sum * data->step_size; // Помножити на розмір кроку
    pthread_exit(NULL);
}

int main() {
    srand(time(NULL)); // Ініціалізація генератора випадкових чисел

    // Генерація випадкових вхідних даних
    int num_steps = rand() % 10000 + 1000; // Випадкова кількість кроків від 1000 до 10000
    int num_threads = rand() % 8 + 1;      // Випадкова кількість потоків від 1 до 8
    double step_size = (double)(rand() % 100 + 1) / 1000; // Випадкова ширина кроку від 0.001 до 0.1
    double a = (double)(rand() % 100);      // Випадкова ліва межа інтегрування
    double b = a + num_steps * step_size;   // Права межа інтегрування

    // Вивід випадкових параметрів англійською
    printf("Random parameters:\n");
    printf("Number of steps: %d\n", num_steps);
    printf("Number of threads: %d\n", num_threads);
    printf("Step size: %lf\n", step_size);
    printf("Left bound: %lf\n", a);
    printf("Right bound: %lf\n", b);

    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];
    double total_sum = 0.0;

    clock_t start = clock();

    for (int i = 0; i < num_threads; i++) {
        thread_data[i].thread_id = i;
        thread_data[i].num_steps = num_steps; // Передаємо кількість кроків
        thread_data[i].step_size = step_size;
        thread_data[i].a = a;
        thread_data[i].b = b;
        pthread_create(&threads[i], NULL, integrate, (void*)&thread_data[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        total_sum += thread_data[i].result;
    }

    clock_t end = clock();
    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;

    // Вивід результатів англійською
    printf("Total Integral: %lf\n", total_sum);
    printf("Time taken: %lf seconds\n", time_taken);

    return EXIT_SUCCESS;
}
