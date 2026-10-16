# Desafio: Leitor de Temperaturas (Vetor Dinâmico 1D)

## 1. Cenário e Objetivo
Em sistemas embarcados e estações meteorológicas, sensores transmitem medições contínuas de temperatura. A quantidade total de dados transmitidos em uma sessão não é conhecida com antecedência: o fluxo de dados pode conter desde uma única medição até centenas de milhares de valores.

O objetivo deste exercício é construir um **vetor dinâmico redimensionável em C** para armazenar uma quantidade indeterminada de números reais (`double`) lidos da entrada padrão, calcular estatísticas básicas (quantidade, média e valor máximo) e garantir a liberação completa da memória.

---

## 2. A Mecânica de Memória (O Grande Aprendizado)

Como o hardware possui memória limitada e não sabemos quantas temperaturas serão enviadas:
* **Alocação Inicial:** O programa deve iniciar com uma capacidade inicial na heap (ex.: 100 posições de `double` com `malloc`).
* **Crescimento Geométrico (Amortizado $O(1)$):** Toda vez que o vetor atingir a capacidade máxima, deve-se expandir o espaço com `realloc`. A boa prática recomendada é a duplicação multiplicativa ($\times 2$) ou incremento planejado, evitando realocar de 1 em 1 para poupar cópias desnecessárias de memória e chamadas ao sistema operacional.
* **Sincronização de Ponteiros:** O ponteiro retornado pelo `realloc` deve ser armazenado com segurança (usando ponteiro temporário para evitar perda de referência em caso de falha de alocação).
* **Desalocação:** Ao término do processamento, toda a memória alocada deve ser devolvida ao sistema operacional via `free` (zero vazamentos).

---

## 3. Ordem de Entrada dos Dados

O programa lê continuamente valores reais (`double`) da entrada padrão (`stdin`), separados por espaços ou quebras de linha, até o final da entrada (`EOF`) ou até encontrar um dado não numérico.

Exemplo de entrada contínua:
```text
20.5 22.0 19.5 24.3
```

---

## 4. Formato da Saída

* Se nenhuma leitura válida for fornecida (entrada vazia ou primeiro dado inválido):
  ```text
  sem dados
  ```
* Se houver uma ou mais leituras válidas:
  ```text
  X leituras, média M, máximo MAX
  ```
  * `X`: quantidade total de leituras válidas (número inteiro).
  * `M`: média aritmética das temperaturas, formatada com 2 casas decimais (`%.2f`).
  * `MAX`: a maior temperatura registrada, formatada com 2 casas decimais (`%.2f`).

> **Atenção:** Se houver valores negativos, o maior valor deve ser calculado corretamente (cuidado para não assumir que o maior valor inicial é zero).

---

## 5. Casos de Teste (Entrada em 1 linha)

### Teste 1 (Valores Positivos Normais)
* **Entrada:**
  ```text
  20.5 22 19.5
  ```
* **Saída esperada:**
  ```text
  3 leituras, média 20.67, máximo 22.00
  ```

### Teste 2 (Todos Negativos)
* **Entrada:**
  ```text
  -5 -1 -3
  ```
* **Saída esperada:**
  ```text
  3 leituras, média -3.00, máximo -1.00
  ```

### Teste 3 (Dado Inválido no Meio)
* **Entrada:**
  ```text
  7.5 abc 9.0
  ```
* **O que acontece:** O programa lê `7.5`, encontra o texto inválido `abc` e encerra a leitura imediatamente, processando apenas os dados lidos antes do erro.
* **Saída esperada:**
  ```text
  1 leituras, média 7.50, máximo 7.50
  ```

### Teste 4 (Entrada Vazia)
* **Entrada:**
  ```text
  
  ```
* **Saída esperada:**
  ```text
  sem dados
  ```

### Teste 5 (Único Valor Zero)
* **Entrada:**
  ```text
  0
  ```
* **Saída esperada:**
  ```text
  1 leituras, média 0.00, máximo 0.00
  ```

---

## 6. Dicas Conceituais de Arquitetura e Boas Práticas

1. **Laço Idiomático de Leitura:**
   * Em C, a forma limpa de ler dados até o fim é checar diretamente o retorno do `scanf`:
     ```c
     while (scanf("%lf", &ponteiro[indice]) == 1)
     ```
2. **Tamanho Baseado no Ponteiro:**
   * Prefira `sizeof(*ponteiro)` em vez de `sizeof(double)`. Isso desacopla o código do tipo de dado em caso de futuras refatorações.
3. **Tratamento Seguro de `realloc`:**
   * Nunca faça `ponteiro = realloc(ponteiro, novo_tamanho)` diretamente. Se o `realloc` falhar e retornar `NULL`, você sobrescreve o endereço antigo e perde o acesso à memória anterior, gerando vazamento (*memory leak*).
4. **Modularidade:**
   * Divida o código em funções pequenas e com responsabilidades únicas: entrada e redimensionamento, cálculo de média e busca do valor máximo.
