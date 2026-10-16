# Desafio: Buffer de Logs do Sistema (Deep Free)

## 1. Cenário e Objetivo
No Linux e em servidores, os programas geram mensagens de texto contínuas para registrar eventos e erros (logs do sistema). 

Cada registro de log possui:
1. Um nível de **severidade** (número inteiro: ex.: `1` = Informativo, `2` = Aviso, `3` = Erro grave).
2. Uma **mensagem de texto** de tamanho variável (ex.: `"falha_no_disco"`, `"ok"`, `"timeout_banco"`).

O objetivo deste exercício é praticar **alocação dinâmica em dois níveis** (uma `struct` com um ponteiro para string dentro dela) e dominar o conceito de **Deep Free** (liberação em camadas na memória heap).

---

## 2. A Mecânica de Memória (O Grande Aprendizado)

Pense nisso como um **armário com gavetas**:
* O seu **vetor de logs** é o armário alocado dinamicamente na heap (`malloc` / `realloc`).
* Cada gaveta é uma `struct Log`.
* Dentro de cada gaveta, a mensagem não tem tamanho fixo: há apenas um **ponteiro (`char *mensagem`)** apontando para o bloco exato de texto alocado à parte na heap.
* **O Perigo do *Shallow Free*:** Se você der apenas `free(vetorLogs)`, você apaga o armário, mas todas as mensagens de texto continuam presas na memória para sempre (vazamento grave no Valgrind).
* **A Missão do *Deep Free*:** Aprender a limpar **de dentro para fora**:
  1. Primeiro, percorrer o vetor e dar `free` na mensagem de texto de cada log.
  2. Só depois dar `free` no vetor de logs principal.

---

## 3. Ordem de Entrada dos Dados

1. **Configuração Inicial (um único número inteiro):**
   * A **severidade mínima** que você deseja monitorar (ex.: `2`). Qualquer log com severidade maior ou igual a esse número é considerado **crítico**.
2. **Os Registros de Log (em repetição, até EOF ou dado inválido):**
   * Cada log contém 2 valores:
     1. `severidade` (número inteiro).
     2. `mensagem` (string de uma palavra sem espaços, ex.: `"conexao_perdida"`).

---

## 4. Formato da Saída

* Se a entrada for vazia (ou se houver apenas a severidade mínima sem nenhum log):
  ```text
  sem dados
  ```
* Se houver logs, mas nenhum atingir a severidade mínima:
  ```text
  total: X logs, nenhum log com severidade >= S
  ```
* Se houver logs críticos:
  ```text
  total: X logs, criticos: Y, maior_mensagem: TEXTO (tam T)
  ```
  * `X`: total de logs válidos lidos.
  * `Y`: quantidade de logs críticos (severidade $\ge S$).
  * `TEXTO`: a mensagem com mais caracteres entre os logs críticos.
  * `T`: a quantidade de caracteres dessa maior mensagem.

---

## 5. Casos de Teste (Entrada em 1 linha)

### Teste 1 (Caso Normal: Mistura de logs)
* **Entrada:**
  ```text
  2 1 sistema_iniciado 3 falha_no_disco 2 memoria_alta 1 ping_ok
  ```
* **Saída esperada:**
  ```text
  total: 4 logs, criticos: 2, maior_mensagem: falha_no_disco (tam 14)
  ```

### Teste 2 (Nenhum log atinge a severidade)
* **Entrada:**
  ```text
  3 1 servico_ativo 2 cpu_em_50_pct 1 porta_aberta
  ```
* **Saída esperada:**
  ```text
  total: 3 logs, nenhum log com severidade >= 3
  ```

### Teste 3 (Todos são críticos)
* **Entrada:**
  ```text
  1 2 aviso_rapido 3 kernel_panic 1 ok
  ```
* **Saída esperada:**
  ```text
  total: 3 logs, criticos: 3, maior_mensagem: aviso_rapido (tam 12)
  ```

### Teste 4 (Apenas o filtro, sem logs)
* **Entrada:**
  ```text
  2
  ```
* **Saída esperada:**
  ```text
  sem dados
  ```

---

## 6. Dicas Conceituais de Arquitetura

1. **A `struct`:**
   Deve agrupar um `int severidade` e um `char *mensagem`.
2. **Como ler palavras de tamanho desconhecido com segurança:**
   * Use um buffer estático temporário para a leitura (ex.: `char temp[256]`).
   * Leia a palavra com `scanf("%255s", temp)`.
   * Meça o tamanho real com `strlen(temp)`.
   * Aloque na heap o tamanho exato: `strlen(temp) + 1` (lembrando do caractere nulo `\0` no final).
   * Copie o conteúdo para a heap com `strcpy`.
3. **Crescimento Dinâmico do Vetor:**
   * O vetor de `struct`s deve crescer dinamicamente conforme chegam novos logs (usando duplicação com `realloc`).
4. **Higiene de Memória:**
   * Crie uma função dedicada para desalocar o buffer de logs, garantindo que cada `mensagem` seja liberada antes do vetor que as contém.
