/*

Assumptions about the problemvgzbvvazaz\ccc\zzcczdjmjbbbbnkeekwsk ekkkdjdmdmcfvvgffff  ddddz sss:

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
#define LEFT (n + 4) % 5
#define RIGHT (n + 1) % 5

sem_t chopstick[5];
sem_t mutex;
int state[5];
int total_eating;
int total_thinking; 

void hungry(){}

void take_chops(){} //test here

void eat(){}

void put_chops(){}

void think(){}

void* philosopher_actions(){} //copy total values

int main (void) {

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