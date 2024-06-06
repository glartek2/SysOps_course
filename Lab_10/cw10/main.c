#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_REINDEERS 9
#define DELIVERIES 3

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t santa_condition = PTHREAD_COND_INITIALIZER;

int waiting_reindeers = 0;
int deliveries = 0;

void *santa(void *arg) {
    while (deliveries < DELIVERIES) {
        pthread_mutex_lock(&mutex);
        while (waiting_reindeers < NUM_REINDEERS) {
            printf("Mikołaj: zasypiam\n");
            pthread_cond_wait(&santa_condition, &mutex);
        }

        printf("Mikołaj: budzę się\n");
        waiting_reindeers = 0;

        printf("Mikołaj: dostarczam zabawki\n");
        pthread_mutex_unlock(&mutex);

        sleep(rand() % 3 + 2);

        deliveries++;
        pthread_mutex_lock(&mutex);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *reindeer(void *arg) {
    int id = *(int *)arg;
    while (deliveries < DELIVERIES) {
        sleep(rand() % 6 + 5);

        pthread_mutex_lock(&mutex);
        waiting_reindeers++;
        printf("Renifer: czeka %d reniferów na Mikołaja, ID: %d\n", waiting_reindeers, id);

        if (waiting_reindeers == NUM_REINDEERS) {
            printf("Renifer: wybudzam Mikołaja, ID: %d\n", id);
            pthread_cond_signal(&santa_condition);
        }
        pthread_mutex_unlock(&mutex);

        sleep(rand() % 3 + 2);
    }
    return NULL;
}

int main() {
    pthread_t santa_thread;
    pthread_t reindeer_threads[NUM_REINDEERS];
    int reindeer_ids[NUM_REINDEERS];

    srand(time(NULL));

    pthread_create(&santa_thread, NULL, santa, NULL);

    for (int i = 0; i < NUM_REINDEERS; i++) {
        reindeer_ids[i] = i + 1;
        pthread_create(&reindeer_threads[i], NULL, reindeer, &reindeer_ids[i]);
    }

    pthread_join(santa_thread, NULL);

    for (int i = 0; i < NUM_REINDEERS; i++) {
        pthread_join(reindeer_threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&santa_condition);

    return 0;
}
