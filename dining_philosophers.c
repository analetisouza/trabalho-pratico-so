/*

Assumptions about the problem:

- Philosophers start the dinner hungry
- Philosophers spend a total time eating and a total time thinking
- The dinner ends when they finish eating and thinking

*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define LEFT_PHILOSOPHER (n + 4) % 5
#define RIGHT_PHILOSOPHER (n + 1) % 5
#define LEFT_CHOPSTICK n
#define RIGHT_CHOPSTICK (n + 1) % 5

sem_t chopstick[5];
sem_t mutex;
int state[5];
int total_eating;
int total_thinking; 

void hungry(int n){
    state[n] = HUNGRY;
    printf("Philosopher %d is hungry\n", n);
}

void take_chops(int n){
    if (state[n] == HUNGRY && state[LEFT_PHILOSOPHER] != EATING && state[RIGHT_PHILOSOPHER] != EATING){
        if (LEFT_CHOPSTICK < RIGHT_CHOPSTICK){
            sem_wait(&chopstick[LEFT_CHOPSTICK]);
            printf("Philosopher %d took left chopstick\n", n);
            sleep(1);
            sem_wait(&chopstick[RIGHT_CHOPSTICK]);
            printf("Philosopher %d took right chopstick\n", n);
        }
        else {
            sem_wait(&mutex);
            sem_wait(&chopstick[LEFT_CHOPSTICK]);
            printf("Philosopher %d took left chopstick\n", n);
            sleep(1);
            sem_wait(&chopstick[RIGHT_CHOPSTICK]);
            printf("Philosopher %d took right chopstick\n", n);
        }
    }
} //add busy chopsticks mechanism

void eat(int n, int* eating){
    int check_eating = *eating;
    if (check_eating > 0){
        state[n] = EATING;
        printf("Philosopher %d is eating\n", n);
        sleep(1);
        *eating = *eating - 1;
        printf("Philosopher %d remaining eating time: %d\n", n, check_eating);
    }
}

void put_chops(int n){
    sem_post(&chopstick[RIGHT_CHOPSTICK]);
    sem_post(&chopstick[LEFT_CHOPSTICK]);
    sem_post(&mutex); //not sure if this should be here
}

void think(int n, int* thinking){
    int check_thinking = *thinking;
    if (check_thinking > 0){
        state[n] = THINKING;
        printf("Philosopher %d is thinking\n", n);
        sleep(1);
        *thinking = *thinking - 1;
        printf("Philosopher %d remaining thinking time: %d\n", n, check_thinking);
    } 
}

void* philosopher_actions(void* n){
    int local_eating = total_eating;
    int local_thinking = total_thinking;

    while(local_eating > 0 || local_thinking > 0){
        int* i = n;

        hungry(i);
        take_chops(i);

        if (local_eating > 0)
            eat(i, &local_eating);

        put_chops(i);

        if (local_thinking > 0)
            think(i, &local_thinking);
    }
    pthread_exit(NULL);
}

int main (void){
    pthread_t philosopher[5];

    printf("Type the amount of time each philosopher will spend eating: ");
    scanf("%d", &total_eating);
    printf("Type the amount of time each philosopher will spend thinking: ");
    scanf("%d", &total_thinking);

    // initiating chopsticks
    for(int i = 0; i < 5; i++)
        sem_init(&chopstick[i], 0, 1);

    // initiating philosophers
    for (int i = 0; i < 5; i++){
        pthread_create(&philosopher[i], NULL, philosopher_actions, (void*)i);
        state[i] = THINKING;
    }

    // joining philosophers threads
    for(int i = 0; i < 5; i++)
        pthread_join(philosopher[i], NULL);
    return 0;
}