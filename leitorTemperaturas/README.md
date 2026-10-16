Como não sei quanto de memória o meu hardware possui, decidi por criar um local no código para decidir a memória inicial de espaço de alocação, exemplo:
a. Inicio com 100
b. Atingi 100? -> Aloca mais 100
Com isso evito ter que alocar 1 espaço por vez, economizando tempo e processamento.

O meu código funcionará para milhares de entradas pois ele é dinâmico, se houver mais que a quantidade inicial alocada, ele aloca mais X unidades