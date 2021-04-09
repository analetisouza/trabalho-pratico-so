Disponível no repositório: https://github.com/analetisouza/trabalho-pratico-so

## Para compilar os arquivos código-fonte dining_philosophers.c e produtor_consumidor.c:

- Acesse o diretório em que os arquivos estão localizados pelo terminal

- Digite make no próximo comando do terminal


## Para executar o programa dining_philosophers:

- Escreva no terminal: ./dining_philosophers

- ex: ./dining_philosophers


## Para executar o programa produtor_consumidor:

- Escreva no terminal: ./produtor_consumidor (tamanho do buffer) (quantidade de itens produzidos pelo produtor de uma única vez) (quantidade de itens consumidos pelo consumidor de uma única vez)

- ex: ./produtor_consumidor 10 3 2

### Atenção, existem duas versões do problema do jantar dos filósofos

- Na primeira versão, as instruções do problema foram levadas a risca, então os filósofos usam seu todo o tempo comendo e pensando de uma vez.

- Na segunda versão, foi considerado um quantum de tempo de 1 segundo, esse tempo é decrescido do total de tempo que eles gastarão comendo e pensando a cada iteração. O programa termina quando ambos os tempos acabam. Essa versão possui alguns problemas de sincronização ainda não solucionados portanto apenas um filósofo come por vez.
