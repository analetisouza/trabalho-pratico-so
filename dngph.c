/*
Assumptions about the problem:
- Philosophers have one chopstick each
- Philosophers start the dinner hungry
- Philosophers spend a total time eating and a total time thinking
- Philosophers can think at the same time
- The dinner ends when they finish eating and thinking

Issues found:
- Consecutive philosophers eating at the same time

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define LEFT_PHILOSOPHER (n + (total_philosophers - 1)) % total_philosophers
#define RIGHT_PHILOSOPHER (n + 1) % total_philosophers
#define LEFT_CHOPSTICK n
#define RIGHT_CHOPSTICK (n + 1) % total_philosophers

pthread_t* philosopher;
sem_t* chopstick;
sem_t mutex;
sem_t print;
int* state;
int total_philosophers;
int total_eating;
int total_thinking;

void print_status(){
    sem_wait(&print);
    printf("\n");
    for(int i = 0; i < total_philosophers; i++){
        if (state[i] == HUNGRY){
            printf("Philosopher %d is hungry\n", i);
        }
        else if (state[i] == EATING){
            printf("Philosopher %d is eating\n", i);
        }
        else if (state[i] == THINKING){
            printf("Philosopher %d is thinking\n", i);
        }
    }
    printf("\n");
    sem_post(&print);
}

void hungry(int n){
    state[n] = HUNGRY;
}

void take_chops(int n){
    sem_wait(&mutex);
    if (state[LEFT_PHILOSOPHER] != EATING && state[RIGHT_PHILOSOPHER] != EATING){
        sem_wait(&chopstick[LEFT_CHOPSTICK]);
        printf("Philosopher %d took left chopstick (Nº %d)\n", n, LEFT_CHOPSTICK);
        sem_wait(&chopstick[RIGHT_CHOPSTICK]);
        printf("Philosopher %d took right chopstick (Nº %d)\n", n, RIGHT_CHOPSTICK);
    }
}

void eat(int n){
    state[n] = EATING;
    print_status();
    sleep(total_eating);
}

void put_chops(int n){
    sem_post(&chopstick[RIGHT_CHOPSTICK]);
    sem_post(&chopstick[LEFT_CHOPSTICK]);
    sem_post(&mutex);
}

void think(int n){
    state[n] = THINKING;
    sleep(total_thinking);
}

void* philosopher_actions(void* n){
    int* aux = n;
    int i = (int)aux;

    hungry(i);
    take_chops(i);
    eat(i);
    put_chops(i);
    think(i);
    pthread_exit(NULL);
}

int main (void){
    printf("Type the amount of philosophers joining the dinner: ");
    scanf("%d", &total_philosophers);
    printf("Type the amount of time each philosopher will spend eating: ");
    scanf("%d", &total_eating);
    printf("Type the amount of time each philosopher will spend thinking: ");
    scanf("%d", &total_thinking);

    philosopher = malloc(total_philosophers * sizeof(pthread_t));
    chopstick = malloc(total_philosophers * sizeof(sem_t));
    state = malloc(total_philosophers * sizeof(int));

    for(int i = 0; i < total_philosophers; i++)
        sem_init(&chopstick[i], 0, 1);

    sem_init(&mutex, 0, 2);
    sem_init(&print, 0, 1);

    for (int i = 0; i < total_philosophers; i++){
        pthread_create(&philosopher[i], NULL, philosopher_actions, (void*)i);
        state[i] = THINKING;
    }

    for(int i = 0; i < total_philosophers; i++)
        pthread_join(philosopher[i], NULL);

    free(state);
    free(chopstick);
    free(philosopher);
    return 0;
}