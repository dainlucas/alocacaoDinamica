# Desafio: Detector de Violações de SLA (Vetor Dinâmico de Structs)

## 1. Cenário e Objetivo
Em infraestrutura de produção e engenharia de confiabilidade (SRE), agentes de telemetria interceptam e analisam métricas de requisições de rede para monitorar o cumprimento de acordos de nível de serviço (SLA - *Service Level Agreement*).

Como o volume de requisições que chegam ao servidor é indeterminado e flutuante, o programa precisa armazenar os eventos dinamicamente na memória heap, identificar quais requisições violaram as regras operacionais, gerar uma coleção filtrada de incidentes e exibir um resumo estatístico das violações.

O objetivo deste exercício é praticar **vetores dinâmicos de estruturas (`struct`)** e gerenciar o ciclo de vida de **múltiplos blocos de memória independentes na heap**.

---

## 2. A Mecânica de Memória (O Grande Aprendizado)

Neste problema, o programa gerencia dois blocos dinâmicos na heap:
1. **Buffer Principal de Eventos:** Um vetor dinâmico de `struct`s que armazena todos os eventos lidos da entrada, crescendo sob demanda através de realocações geométricas (`realloc`).
2. **Buffer Filtrado de Incidentes (SLA):** Uma segunda lista alocada dinamicamente contendo apenas as cópias dos eventos que violaram o SLA.
3. **Gerenciamento de Múltiplos Ponteiros (*Ownership*):** 
   * Ambos os ponteiros (`listaEventos` e `listaSLA`) devem ter seu ciclo de vida monitorado com rigor.
   * Se a alocação da segunda lista falhar, a primeira deve ser liberada antes do encerramento para evitar vazamento (*memory leak*).
   * Ao final da execução, ambos os blocos devem ser explicitamente liberados via `free`.

---

## 3. Ordem de Entrada dos Dados

A entrada dos dados ocorre em duas fases:

1. **Configuração Inicial (1º dado):**
   * Um único número real (`double`) representando o **limite máximo de latência tolerável** em milissegundos (ex.: `200.0`).
2. **Registros de Eventos (em fluxo contínuo até EOF ou entrada inválida):**
   * Cada evento é composto por uma trinca de valores:
     1. `id`: Identificador numérico do serviço (número inteiro).
     2. `status`: Código de resposta HTTP (número inteiro, ex.: `200`, `404`, `500`).
     3. `latencia`: Tempo de resposta em milissegundos (número real / `double`).

---

## 4. Regra de Negócio e Violação de SLA

Um evento é classificado como **Violação de SLA** se atender a **pelo menos uma** das seguintes condições:
* Sua latência for **estritamente maior** que o limite tolerável (`latencia > limite`); **OU**
* Seu código de status for um **erro interno de servidor** (`status >= 500`).

---

## 5. Formato da Saída

* Se a entrada for vazia (ou contiver apenas o limite sem nenhum evento válido):
  ```text
  sem dados
  ```
* Se houver eventos válidos, mas **nenhuma** violação de SLA:
  ```text
  total: X eventos, sla intacto
  ```
* Se houver **uma ou mais** violações de SLA:
  ```text
  total: X eventos, violacoes: Y (P%), pior_latencia: Z ms (servico W)
  ```
  * `X`: Quantidade total de eventos válidos lidos.
  * `Y`: Quantidade de eventos que violaram o SLA.
  * `P%`: Percentual de violações em relação ao total (ex.: `50.0%`).
  * `Z`: A **maior latência** registrada **entre as violações**, formatada com 2 casas decimais.
  * `W`: O `id` do serviço responsável por essa pior latência.

---

## 6. Casos de Teste (Entrada em 1 linha)

### Teste 1 (Mistura de Sucessos e Violações)
* **Entrada:**
  ```text
  100.0 10 200 45.5 20 500 80.0 30 200 150.2 10 200 60.0
  ```
* **O que acontece:** Limite é `100.0`. O serviço 20 violou por status 500 (80.0 ms). O serviço 30 violou por latência alta (150.2 ms). O pior incidente foi do serviço 30.
* **Saída esperada:**
  ```text
  total: 4 eventos, violacoes: 2 (50.0%), pior_latencia: 150.20 ms (servico 30)
  ```

### Teste 2 (SLA Intacto)
* **Entrada:**
  ```text
  250.0 1 200 12.5 2 201 100.0 3 404 45.0 1 200 250.0
  ```
* **O que acontece:** Todos os tempos estão dentro de 250.0 ms e nenhum status é $\ge 500$ (status 404 é erro de cliente, não de servidor).
* **Saída esperada:**
  ```text
  total: 4 eventos, sla intacto
  ```

### Teste 3 (100% de Violações)
* **Entrada:**
  ```text
  50.0 101 503 30.0 102 200 95.5 103 500 120.0
  ```
* **Saída esperada:**
  ```text
  total: 3 eventos, violacoes: 3 (100.0%), pior_latencia: 120.00 ms (servico 103)
  ```

### Teste 4 (Apenas 1 Evento Violado)
* **Entrada:**
  ```text
  75.0 5 200 120.8
  ```
* **Saída esperada:**
  ```text
  total: 1 eventos, violacoes: 1 (100.0%), pior_latencia: 120.80 ms (servico 5)
  ```

### Teste 5 (Apenas o Limite, Sem Eventos)
* **Entrada:**
  ```text
  100.0
  ```
* **Saída esperada:**
  ```text
  sem dados
  ```

---

## 7. Dicas Conceituais de Arquitetura

1. **Definição da `struct`:**
   * Agrupe os campos de forma coesa (ex.: `id`, `statusHTTP` e `latencia`).
2. **Cálculo de Memória:**
   * Lembre-se de calcular os bytes de alocação utilizando `sizeof(Evento)` ou `sizeof(*ponteiro)`.
3. **Atenção na Busca da Pior Latência:**
   * A função que encontra a maior latência deve pesquisar exclusivamente na coleção filtrada de **violações** (`listaSLA`), e não na lista geral com todos os eventos saudáveis.
4. **Higiene e Liberação:**
   * Verifique se toda alocação bem-sucedida possui uma instrução `free` correspondente ao final do fluxo.
