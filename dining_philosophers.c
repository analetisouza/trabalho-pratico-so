#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define HUNGRY 0
#define EATING 1
#define THINKING 2

pthread_t* philosopher; //philosophers threads
sem_t* chopstick; //chopsticks semaphores

int* state; //verifies if the philosopher is THINKING, HUNGRY or EATING
int* current_philosopher; //returns the id of the current philosopher
int total_philosophers; //total number of philosophers
int time_eating; //total time eating (eating sleep time)
int time_thinking; //total time thinking (thinking sleep time)

void hungry(int n){
    printf("Philosopher %d is hungry...\n\n", n);
    state[n] = HUNGRY;
}

void eating(int n){
    printf("Philosopher %d is eating...\n\n", n);
    state[n] = EATING;
    sleep(time_eating);
}

void thinking(int n){
    printf("Philosopher %d is thinking...\n\n", n);
    state[n] = THINKING;
    sleep(time_thinking);
}

void print_status(){
    printf("--------------------------------------\n");
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
    printf("--------------------------------------\n");
}

void take_chops(int* pos_current_philosopher){
    sem_wait(&chopstick[*pos_current_philosopher]);
    sem_wait(&chopstick[((*pos_current_philosopher) + 1) % total_philosophers]);
    printf("Philosopher %d got the %d chopstick\n", *pos_current_philosopher, *pos_current_philosopher);
    printf("Philosopher %d got the %d chopstick\n", *pos_current_philosopher, ((*pos_current_philosopher) + 1) % total_philosophers);
}

void put_chops(int* pos_current_philosopher){
    printf("Philosopher %d finished eating\n", *pos_current_philosopher);
    printf("Philosopher %d put the %d chopstick down\n", *pos_current_philosopher, *pos_current_philosopher);
    printf("Philosopher %d put the %d chopstick down\n\n", *pos_current_philosopher, ((*pos_current_philosopher) + 1) % total_philosophers);
    sem_post(&chopstick[*pos_current_philosopher]);
    sem_post(&chopstick[((*pos_current_philosopher) + 1) % total_philosophers]);
}

void* philosopher_actions(void* n){
    int* pos_current_philosopher = (int*) n;

    while(1){
        print_status();
        hungry(*pos_current_philosopher);
        take_chops(pos_current_philosopher);
        eating(*pos_current_philosopher);
        put_chops(pos_current_philosopher);
        thinking(*pos_current_philosopher);
    }
}

int main(int argc, char* argv[])
{
    int i;
    // parameters: number of philosophers, time eating, time thinking
    total_philosophers = atoi(argv[1]);
    time_eating = atoi(argv[2]);
    time_thinking = atoi(argv[3]);  

    philosopher = (pthread_t*) malloc(total_philosophers * sizeof(pthread_t*));
    current_philosopher = (int*) malloc(total_philosophers * sizeof(int));
    state = (int*) malloc(total_philosophers * sizeof(int));
    chopstick = (sem_t*) malloc(total_philosophers * sizeof(sem_t));

    // initiating chopsticks
    for(i = 0; i < total_philosophers; i++)
        sem_init(&chopstick[i], 0, 1);

    // initiating philosophers
    for (i = 0; i < total_philosophers; i++){
        current_philosopher[i] = i;
        state[i] = HUNGRY;
        pthread_create(&philosopher[i], NULL, philosopher_actions, (void*)&current_philosopher[i]);
    }

    // joining philosophers threads
    for(i = 0; i < total_philosophers; i++)
        pthread_join(philosopher[i], NULL);

    return 0;
}