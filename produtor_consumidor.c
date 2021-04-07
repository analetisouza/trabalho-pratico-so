/*
Implementação do problema do Produtor e Consumidor com o uso de semáforos
Alunas: Ana Letícia Souza e Rita Chen
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

void imprime(void);
void* produz(void* n);
void* consome(void* n);
int num_random(int min, int max);

//threads do produtor (thread[0]) e consumidor (thread[1])
pthread_t thread[2];

//semáforos
sem_t mutex; //semáforo binário que controla se alguma thread está acessando a região crítica (exclusão mutua)
sem_t full; //semáforo que indica quantos espaços do buffer estão ocupados
sem_t empty; //semáforo que indica quantos espaços do buffer estão vazios

//dados coletados na entrada (são dados constantes depois de declarados)
int tam_buffer; //tamanho do buffer
int itens_prod; //quantidade de itens produzidos pelo produtor de uma única vez
int itens_cons; //quantidade de itens consumidos pelo consumidor de uma única vez
int contador = 0;

//região crítica (recursos compartilhados entre as threads)
int* buffer; //vetor do buffer

int main(void) {
	int i;

	//pega os dados da entrada
	printf("Informe o tamanho do buffer: ");
	scanf("%d", &tam_buffer);
	printf("Informe a quantidade de itens produzidos pelo produtor de uma única vez: ");
	scanf("%d", &itens_prod);
	printf("Informe a quantidade de itens consumidos pelo consumidor de uma única vez: ");
	scanf("%d", &itens_cons);
	
	//aloca o buffer com o tamanho dado
	buffer = (int*)malloc(tam_buffer*sizeof(int));

	//inicialização dos semáforos
	sem_init(&mutex, 0, 1); //mutex é binário (0 ou 1) e inicializada com 1, indicando que não há thread utilizando a região crítica
	sem_init(&full, 0, 0); //indica quantas posições estão ocupadas, inicializada com 0
	sem_init(&empty, 0, tam_buffer);//indica quantas posições estão livres, inicializada com o tamanho do buffer

	//criando e inicializando as threads do produtor na pos. 0 e consumidor na pos. 1
	pthread_create(&thread[0], NULL, produz, NULL);
	pthread_create(&thread[1], NULL, consome, NULL);

	for(i = 0; i < 2; i++)
        pthread_join(thread[i], NULL);

	return 0;
}

void* produz(void* n)
{
/*
	sem_wait é equivalente a operação de DOWN: 
- decrementa os valores dos semáforos empty e mutex em 1 quando seus valores são > 0, 
para permitir a produção de um item dentro da região crítica, garantindo a exclusão mútua.

	sem_post é equivalente a operação de UP:
- incrementa os valores dos semáforos mutex e full em 1 após a utilização da região crítica pelo produtor, 
liberando assim o uso do buffer para a thread do consumidor que está esperando, acordando esta thread.
*/
	int i;
	while(1){
		sem_wait(&empty);
		sem_wait(&mutex);
		printf("\n\t    EMPTY E MUTEX > 0 => PRODUÇÃO!\n");

		buffer[contador] = num_random(1,10);
		printf("Produzi o buffer[%d] = %d\n",contador, buffer[contador]);
		contador = (contador + 1) % tam_buffer;
		printf("Contador atual: %d\n", contador);
		printf("Saí da RC\n");

		sem_post(&mutex);
		sem_post(&full);

		imprime();
		sleep(1);
	}
}


void* consome(void* n)
{
/*
	sem_wait é equivalente a operação de DOWN: 
- decrementa os valores dos semáforos full e mutex em 1 quando seus valores são > 0, 
para permitir o consumo de um item dentro da região crítica, garantindo a exclusão mútua.

	sem_post é equivalente a operação de UP:
- incrementa os valores dos semáforos mutex e empty em 1 após a utilização da região crítica pelo consumidor, 
liberando assim o uso do buffer para a thread do produtor que está esperando, acordando esta thread.
*/
	while(1){
		sem_wait(&full);
		sem_wait(&mutex);
		printf("\n\t    FULL E MUTEX > 0 => CONSUMO!\n");

		int item = buffer[--contador];
		printf("Consumi o item da posição[%d] = %d\n",contador, buffer[contador]);
		buffer[contador] = -1;
		printf("Contador atual: %d\n", contador);
		printf("Saí da RC\n");

		sem_post(&mutex);
		sem_post(&empty);

		imprime();
		sleep(1);
	}
}


void imprime(void)
{
	int i;

	printf("\n\t    Informações:\n\tQntd de elementos = %d\n\tBuffer: ", contador);
	for(i = 0; i < tam_buffer; i++) {
		printf("%d   ", buffer[i]);
	}
	printf("\n----------------------------------------\n");
	//system("clear || cls");
}

int num_random(int min, int max){
   return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}
