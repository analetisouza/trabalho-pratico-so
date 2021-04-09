Desenvolvido pelas alunas Ana Letícia Souza e Rita Zhen Chen para a disciplina de Sistemas Operacionais do curso de Ciência da Computação da Universidade Federal Rural do Rio de Janeiro
Disponível no repositório: https://github.com/analetisouza/trabalho-pratico-so

## Para compilar os arquivos código-fonte dining_philosophers.c, produtor_consumidor.c e experimental_dining_philosophers.c:

- Acesse o diretório em que os arquivos estão localizados pelo terminal

- Digite make no próximo comando do terminal


## Para executar o programa do código dining_philosophers.c:

- Escreva no terminal: ./dp (número de filósofos) (tempo de pensamento) (tempo comendo)

- ex: ./dp 5 2 2


## Para executar o programa do código produtor_consumidor.c:

- Escreva no terminal: ./pc (tamanho do buffer) (quantidade de itens produzidos pelo produtor de uma única vez) (quantidade de itens consumidos pelo consumidor de uma única vez)

- ex: ./pc 10 3 2


## Para executar o programa do código experimental_dining_philosophers.c:

- Escreva no terminal: ./edp

- ex: ./edp


### Existem duas versões do problema do jantar dos filósofos

- Na primeira versão (dining_philosophers.c), as instruções do problema foram levadas a risca, então os filósofos usam seu todo o tempo comendo e pensando de uma vez.

- Na segunda versão (experimental_dining_philosophers.c), foi considerado um quantum de tempo de 1 segundo, esse tempo é decrescido do total de tempo que eles gastarão comendo e pensando a cada iteração. O programa termina quando ambos os tempos acabam. Essa versão possui alguns problemas de sincronização ainda não solucionados portanto apenas um filósofo come por vez.
