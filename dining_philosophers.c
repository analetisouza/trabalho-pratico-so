/*

Assumptions about the problem:

- Philosophers start the dinner hungry
- Philosophers spend a total time eating and a total time thinking
- The dinner ends when they finish eating and thinking

*/

#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <pthread.h>
#include <semaphore.h>

#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define LEFT_PHILOSOPHER (n + 4) % 5
#define RIGHT_PHILOSOPHER (n + 1) % 5
#define LEFT_CHOPSTICK n
#define RIGHT_CHOPSTICK (n + 1) % 5

pthread_t philosopher[5];
sem_t chopstick[5];
sem_t mutex;
int state[5];
int total_eating;
int total_thinking; 

WINDOW* philosopher_status[5];

void update_status(int n) {
    wclear(philosopher_status[n]);
    move(n, 0);

    if (state[n] == HUNGRY){
        wprintw(philosopher_status[n], "Philosopher %d is hungry", n);
        wrefresh(philosopher_status[n]);
    }
    else if (state[n] == EATING){
        wprintw(philosopher_status[n], "Philosopher %d is eating", n);
        wrefresh(philosopher_status[n]);
    }
    else if (state[n] == THINKING){
        wprintw(philosopher_status[n], "Philosopher %d is thinking", n);
        wrefresh(philosopher_status[n]);
    }
}

void hungry(int n){
    state[n] = HUNGRY;
    update_status(n);
}

void take_chops(int n){
    if (state[n] == HUNGRY && state[LEFT_PHILOSOPHER] != EATING && state[RIGHT_PHILOSOPHER] != EATING){
        if (LEFT_CHOPSTICK < RIGHT_CHOPSTICK){
            sem_wait(&chopstick[LEFT_CHOPSTICK]);
            //printf("Philosopher %d took left chopstick\n", n);
            sleep(1);
            sem_wait(&chopstick[RIGHT_CHOPSTICK]);
            //printf("Philosopher %d took right chopstick\n", n);
        }
        else {
            sem_wait(&mutex);
            sem_wait(&chopstick[LEFT_CHOPSTICK]);
            //printf("Philosopher %d took left chopstick\n", n);
            sleep(1);
            sem_wait(&chopstick[RIGHT_CHOPSTICK]);
            //printf("Philosopher %d took right chopstick\n", n);
        }
    }
}

void eat(int n, int* eating){
    int check_eating = *eating;
    if (check_eating > 0){
        state[n] = EATING;
        update_status(n);
        sleep(1);
        *eating = *eating - 1;
        //printf("Philosopher %d remaining eating time: %d\n", n, check_eating - 1);
    }
}

void put_chops(int n){
    sem_post(&chopstick[RIGHT_CHOPSTICK]);
    sem_post(&chopstick[LEFT_CHOPSTICK]);
    sem_post(&mutex);
}

void think(int n, int* thinking){
    int check_thinking = *thinking;
    if (check_thinking > 0){
        state[n] = THINKING;
        update_status(n);
        sleep(1);
        *thinking = *thinking - 1;
        //printf("Philosopher %d remaining thinking time: %d\n", n, check_thinking - 1);
    } 
}

void* philosopher_actions(void* n){
    int local_eating = total_eating;
    int local_thinking = total_thinking;
    int* i = n;
    int index = (int)i;

    philosopher_status[index] = newwin(1, 50, index, 0); //change 50 to total string length

    while(local_eating > 0 || local_thinking > 0){
        hungry(index);
        take_chops(index);
        eat(index, &local_eating);
        put_chops(index);
        think(index, &local_thinking);
    }
    pthread_exit(NULL);
}

int main (void){
    initscr();
    printw("Type the amount of time each philosopher will spend eating: ");
    refresh();
    scanw("%d", &total_eating);
    clear();
    printw("Type the amount of time each philosopher will spend thinking: ");
    refresh();
    scanw("%d", &total_thinking);
    clear();
    refresh();

    for(int i = 0; i < 5; i++)
        sem_init(&chopstick[i], 0, 1);


    for (int i = 0; i < 5; i++){
        pthread_create(&philosopher[i], NULL, philosopher_actions, (void *)i);
        state[i] = THINKING;
    }

    for(int i = 0; i < 5; i++)
        pthread_join(philosopher[i], NULL);

    endwin();
    return 0;
}