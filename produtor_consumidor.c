/*
Implementação do problema do Produtor e Consumidor com o uso de semáforos
*utilizando vetor circular
Alunas: Ana Letícia Souza e Rita Chen
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

void* produz(void* n);
void* consome(void* n);
void imprime_buffer(void);

//threads do produtor (thread[0]) e do consumidor (thread[1])
pthread_t thread[2];

//semáforos
sem_t mutex; //semáforo binário que controla se alguma thread está acessando a região crítica
sem_t full; //semáforo contador que indica quantos espaços do buffer estão ocupados
sem_t empty; //semáforo contador que indica quantos espaços do buffer estão vazios

//dados coletados na entrada (são dados constantes depois de declarados)
int tam_buffer; //tamanho do buffer
int itens_prod; //quantidade de itens produzidos pelo produtor de uma única vez
int itens_cons; //quantidade de itens consumidos pelo consumidor de uma única vez
int ind_prod = 0, ind_cons = 0, qntd_itens = 0; //indices de onde vão realizar a produção e a consumação e o contador de itens no buffer

//região crítica (recursos compartilhados entre as threads do produtor e consumidor)
int* buffer; //vetor do buffer

int main(int argc, char* argv[]) {
	int i;

	//pega os dados da linha de comando e atribui nas variáveis
	tam_buffer = atoi(argv[1]);
	itens_prod = atoi(argv[2]);
	itens_cons = atoi(argv[3]);

	//aloca o buffer com o tamanho dado
	buffer = (int*)malloc(tam_buffer*sizeof(int));

	//inicialização dos semáforos
	sem_init(&mutex, 0, 1); //mutex é binário (0 ou 1) e inicializada com 1, indicando que não há thread utilizando a região crítica
	sem_init(&full, 0, 0); //indica quantas posições estão ocupadas, inicializada com 0
	sem_init(&empty, 0, tam_buffer);//indica quantas posições estão livres, inicializada com o tamanho do buffer

	//criando e inicializando as threads do produtor na pos. 0 e consumidor na pos. 1 do vetor thread
	pthread_create(&thread[0], NULL, produz, NULL);
	pthread_create(&thread[1], NULL, consome, NULL);

	for(i = 0; i < 2; i++)
        pthread_join(thread[i], NULL);

	return 0;
}

void* produz(void* n)
{
	int i;

	while(1){
		for(i = 0; i < itens_prod; i++) {
			//vai produzir quando empty e mutex > 0:
			sem_wait(&empty); //decrementa em 1 o semáforo de posições vazias do buffer
			sem_wait(&mutex); //a thread do produtor está dentro da região crítica, mutex = 0

			//produz o item com id entre 1 a 99 e o coloca dentro do buffer
			buffer[ind_prod] = (rand()%99 + 1);
			printf("\n\t\t* %dº PRODUÇÃO DO ID %d!\n", i+1, buffer[ind_prod]);
			ind_prod = (ind_prod + 1) % tam_buffer;
			qntd_itens++;

			imprime_buffer();

			sem_post(&mutex); //a thread do produtor sai da região crítica, mutex = 1
			sem_post(&full); //incrementa em 1 o semáforo de posições ocupadas do buffer
		}
		printf("\n\n-------------------------------------------------------------\n\n");
		sleep(2);
	}

	pthread_exit(NULL);
}


void* consome(void* n)
{
	int i;

	while(1){
		for(i = 0; i < itens_cons; i++) {
			//vai consumir quando full e mutex > 0:
			sem_wait(&full); //decrementa em 1 o semáforo de posições ocupadas do buffer
			sem_wait(&mutex); //a thread do consumidor está dentro da região crítica, mutex = 0

			//consome o item do mais antigo para o mais recente, considerando o vetor circular
			//int item = buffer[ind_cons];
			printf("\n\t\t* %dº CONSUMO DO ID %d!\n", i+1, buffer[ind_cons]);
			buffer[ind_cons] = 0;
			ind_cons = (ind_cons + 1) % tam_buffer;
			qntd_itens--;

			imprime_buffer();

			sem_post(&mutex); //a thread do consumidor sai dentro da região crítica, mutex = 1
			sem_post(&empty); //incrementa em 1 o semáforo de posições vazias do buffer

		}
		printf("\n\n-------------------------------------------------------------\n\n");
		sleep(2);
	}

	pthread_exit(NULL);
}


void imprime_buffer(void)
{
	int i;

	printf("\n\tBuffer: ");
	for(i = 0; i < qntd_itens; i++) {
		printf("%d   ", buffer[(ind_cons + i) % tam_buffer]);
	}
	printf("\n");
}
