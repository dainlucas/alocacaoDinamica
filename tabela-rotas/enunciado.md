# Exercício: tabela dinâmica de rotas em C

## 1. Cenário e objetivo

Um gateway é um programa que recebe requisições e as encaminha para outros serviços. Neste exercício, cada rota cadastrada é representada apenas por um caminho textual, como `/api`, `/metrics` ou `/api/v1/checkout`.

Escreva um programa em C que leia esses caminhos, armazene-os em memória dinâmica e apresente uma listagem ordenada. A quantidade de rotas não é informada antecipadamente, e os caminhos podem ter comprimentos diferentes.

O foco é praticar **vetor de ponteiros, strings e gerenciamento de memória dinâmica**. O cenário é simplificado: não é necessário conhecer HTTP, configurar um gateway, acessar a rede ou implementar a escolha de uma rota para uma requisição. Ordenar caminhos por comprimento, por si só, não realiza essa escolha.

## 2. Entrada

O programa deve ler da entrada padrão (`stdin`) até o final da entrada (`EOF`). Cada trecho não vazio separado por espaços em branco representa uma rota. Espaços, tabulações e quebras de linha podem aparecer em qualquer quantidade entre as rotas.

Garantias e convenções:

- Cada rota começa com `/` e contém apenas caracteres ASCII visíveis, sem espaços em branco. A rota `/` é válida.
- Todos os caminhos recebidos respeitam esse formato. Não é necessário validar a sintaxe de URLs nem interpretar os caminhos.
- Rotas repetidas devem ser mantidas e contadas separadamente.
- Cada rota recebida tem de **1 a 255 caracteres**, incluindo as barras e excluindo o terminador `\0`. A entrada garante esse limite; não é necessário ler nem tratar caminhos maiores.
- Não há um limite fixo de quantidade de rotas definido pelo exercício. A tabela deve crescer conforme necessário, sem descartar rotas silenciosamente por causa de uma capacidade fixa.
- A entrada pode estar vazia ou conter somente espaços em branco. Nos dois casos, nenhuma rota foi fornecida.
- O comprimento de uma rota inclui as barras e exclui o terminador de string `\0`. Com os caracteres garantidos aqui, a quantidade de caracteres também corresponde ao número de bytes do texto.

Exemplo de entrada:

```text
/api /healthz /api/v1/checkout /metrics /auth
```

## 3. Requisitos de memória

- A tabela deve ser um **vetor dinâmico de ponteiros para strings**, representado por `char **`.
- Cada ocorrência de rota deve ter armazenamento próprio na memória dinâmica, dimensionado para seu texto e seu terminador.
- A capacidade do vetor deve crescer sob demanda com `realloc`, usando crescimento geométrico: aumentos por um fator constante maior que 1. A capacidade inicial e o fator ficam a seu critério.
- Durante a ordenação, a posição das rotas deve mudar pela movimentação dos ponteiros no vetor. O texto das strings deve permanecer inalterado; não copie seu conteúdo para reordená-las.
- A troca de dois ponteiros deve ter custo constante, independente do comprimento das strings. Isso não significa que comparar duas rotas ou ordenar a tabela inteira tenha custo constante.
- Toda memória alocada pelo programa deve ser liberada, tanto no encerramento normal quanto se uma alocação falhar. Não pode haver acesso fora dos limites, uso de memória já liberada ou liberação duplicada.

Se uma alocação falhar, o programa deve encerrar de forma controlada, liberar os recursos que já possui e indicar falha por um código de saída diferente de zero. Nesse caso, não deve apresentar uma listagem parcial como resultado completo. O texto de uma eventual mensagem de erro não é fixado; mensagens de diagnóstico devem ir para a saída de erros (`stderr`).

## 4. Regra de ordenação

Apresente todas as rotas segundo estes critérios, nesta prioridade:

1. **Comprimento decrescente:** caminhos mais longos aparecem antes dos mais curtos.
2. **Ordem lexicográfica crescente:** entre caminhos de mesmo comprimento, vem primeiro o menor segundo a comparação de strings de C (`strcmp`). A comparação distingue letras maiúsculas de minúsculas.

Por exemplo, `/auth` e `/ping` têm comprimento 5; nesse empate, `/auth` vem antes de `/ping`. Rotas idênticas continuam aparecendo tantas vezes quantas foram fornecidas.

Você pode escolher o algoritmo de ordenação. Não é exigido um algoritmo avançado.

## 5. Saída

Se nenhuma rota tiver sido fornecida, imprima apenas:

```text
sem dados
```

Caso contrário, imprima duas linhas:

```text
total: X rotas, maior: ROTA (tam T)
ordem: ROTA_1 ROTA_2 ROTA_3 ...
```

- `X` é a quantidade de rotas recebidas, incluindo repetições.
- `ROTA` é a primeira rota na ordem definida na seção 4, e `T` é seu comprimento.
- Após `ordem:`, apresente todas as rotas ordenadas, separadas por um único espaço, sem espaço ao final.
- Termine cada linha com uma quebra de linha. Não inclua menus ou pedidos de entrada na saída padrão (`stdout`).

Para manter o formato uniforme, use o texto literal `rotas` mesmo quando a quantidade for 1. No encerramento normal, inclusive com entrada vazia, o código de saída deve ser zero.

## 6. Exemplos verificáveis

### Exemplo 1 — comprimentos diferentes

Entrada:

```text
/api /healthz /api/v1/checkout /metrics /auth
```

Saída esperada:

```text
total: 5 rotas, maior: /api/v1/checkout (tam 16)
ordem: /api/v1/checkout /healthz /metrics /auth /api
```

Nesse exemplo, `/healthz` e `/metrics` têm comprimento 8. No desempate lexicográfico, `/healthz` vem primeiro.

### Exemplo 2 — desempate entre rotas de mesmo comprimento

Entrada:

```text
/status /metrics /healthz
```

Saída esperada:

```text
total: 3 rotas, maior: /healthz (tam 8)
ordem: /healthz /metrics /status
```

O empate ocorre entre `/healthz` e `/metrics`, ambas com comprimento 8. `/status` tem comprimento 7.

### Exemplo 3 — uma única rota

Entrada:

```text
/ping
```

Saída esperada:

```text
total: 1 rotas, maior: /ping (tam 5)
ordem: /ping
```

### Exemplo 4 — rotas repetidas

Entrada:

```text
/api /v1 /api
```

Saída esperada:

```text
total: 3 rotas, maior: /api (tam 4)
ordem: /api /api /v1
```

### Exemplo 5 — nenhuma rota

Para uma entrada sem caracteres, ou composta somente por espaços em branco, a saída esperada é:

```text
sem dados
```

## 7. Entrega e critérios de avaliação

Escreva sua solução em `main.c`. Além do comportamento definido acima, você deverá conseguir explicar:

- A diferença entre a quantidade de rotas armazenadas e a capacidade do vetor.
- O que cada ponteiro representa e quem é responsável pela memória alocada.
- Como justificou o tamanho das alocações e como preserva os dados durante o crescimento da tabela.
- O comportamento do programa quando uma alocação falha.
- O custo de movimentar ponteiros, de comparar strings e do algoritmo de ordenação escolhido, distinguindo essas operações.

Verifique sua solução com entrada vazia, uma única rota, repetições, empates de comprimento, separadores variados, caminhos de até 255 caracteres e quantidades que provoquem vários redimensionamentos. A avaliação considera corretude, preservação dos dados, segurança de memória e sua justificativa das decisões.

## 8. Arena de Testes disponível

Dentro da pasta `exercicios/alocacaoDinamica/tabela-rotas`, execute:

```bash
python3 testes/testar.py
```

A bateria existente inclui casos de saída, bordas, variações de espaços em branco, caminhos de até 255 caracteres, escala, verificações de memória e injeção de falhas de alocação. Espaços extras e tabulações são entradas válidas segundo este enunciado; não há exigência de tratar caminhos malformados ou maiores que o limite garantido.

Consulte o relatório para saber quais verificações foram efetivamente executadas e quais ficaram pendentes. O placar complementa a revisão da solução e a explicação do seu raciocínio.
