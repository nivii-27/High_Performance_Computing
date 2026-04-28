#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 5

typedef struct {
    int num1, num2;
    char op;
} Task;

Task buffer[BUFFER_SIZE];
int in = 0, out = 0;
int total_tasks = 0;
int tasks_produced = 0;
int tasks_consumed = 0;

sem_t empty, full, mutex;
FILE *result_file;

double compute(Task t) {
    if (t.op == '+') return t.num1 + t.num2;
    if (t.op == '-') return t.num1 - t.num2;
    if (t.op == '*') return t.num1 * t.num2;
    if (t.op == '/') return (t.num2 != 0) ? (double)t.num1 / t.num2 : 0;
    return 0;
}

void* producer(void* arg) {
    char ops[] = {'+', '-', '*', '/'};
    while (1) {
        sem_wait(&mutex);
        if (tasks_produced >= total_tasks) {
            sem_post(&mutex);
            break;
        }
        sem_post(&mutex);

        int empty_count;
        sem_getvalue(&empty, &empty_count);
        if (empty_count == 0) {
            printf("Buffer is FULL. Producer is waiting...\n");
            usleep(500000);
            continue;
        }

        sem_wait(&empty);
        sem_wait(&mutex);

        Task newTask = {rand() % 100, rand() % 100, ops[rand() % 4]};
        buffer[in] = newTask;
        in = (in + 1) % BUFFER_SIZE;
        tasks_produced++;

        sem_post(&mutex);
        sem_post(&full);

        usleep(400000); // Small delay to see transitions
    }
    pthread_exit(NULL);
}

void* consumer(void* arg) {
    char myOp = *(char*)arg;
    while (1) {
        sem_wait(&mutex);
        if (tasks_consumed >= total_tasks) {
            sem_post(&mutex);
            break;
        }
        sem_post(&mutex);

        int full_count;
        sem_getvalue(&full, &full_count);
        if (full_count == 0) {
            usleep(1000000);
            continue;
        }

        sem_wait(&full);
        sem_wait(&mutex);

        Task t = buffer[out];
        if (t.op == myOp) {
            out = (out + 1) % BUFFER_SIZE;
            tasks_consumed++;
            sem_post(&mutex);
            sem_post(&empty);

            double res = compute(t);
            fprintf(result_file, "%d %c %d = %.2lf\n", t.num1, t.op, t.num2, res);
            fflush(result_file);
            printf("Thread %c processed: %d %c %d = %.2lf\n", myOp, t.num1, t.op, t.num2, res);
        } else {
            // Not my task, put it back
            sem_post(&mutex);
            sem_post(&full);
            usleep(200000);
        }
    }
    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));
    pthread_t prod, add_t, sub_t, mul_t, div_t;
    char add = '+', sub = '-', mul = '*', div = '/';

    printf("Enter total number of tasks: ");
    if(scanf("%d", &total_tasks) != 1) return 1;

    result_file = fopen("result.txt", "w");
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&add_t, NULL, consumer, &add);
    pthread_create(&sub_t, NULL, consumer, &sub);
    pthread_create(&mul_t, NULL, consumer, &mul);
    pthread_create(&div_t, NULL, consumer, &div);

    pthread_join(prod, NULL);
    pthread_join(add_t, NULL);
    pthread_join(sub_t, NULL);
    pthread_join(mul_t, NULL);
    pthread_join(div_t, NULL);

    printf("\n--- All Tasks Completed ---\n");
    fclose(result_file);
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);
    return 0;
}