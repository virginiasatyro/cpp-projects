# Hints

## Formula para acessar um array 1D como se fosse 2D

`y * mapWidth + x` é uma fórmula para transformar uma posição 2D `(x, y)` em um índice de array 1D.

Imagine um mapa com largura `mapWidth`:

```text
(x, y)
x = coluna
y = linha
mapWidth = quantidade de colunas por linha
```

Se o mapa é guardado em um array simples, ele fica assim na memória:

```text
linha 0:  [0] [1] [2] [3]
linha 1:  [4] [5] [6] [7]
linha 2:  [8] [9] [10][11]
```

Para `mapWidth = 4`, a posição `(2, 1)` significa:

```cpp
index = y * mapWidth + x;
index = 1 * 4 + 2;
index = 6;
```

Passo a passo:

1. `y * mapWidth` pula todas as linhas anteriores.
   
   Se `y = 1` e `mapWidth = 4`, você pula `4` posições.

2. `+ x` anda dentro da linha atual.
   
   Se `x = 2`, você anda mais `2` posições.

3. Resultado: índice `6`.

Então:

```cpp
array[y * mapWidth + x]
```

significa: “pegue o elemento que está na coluna `x` e linha `y` dentro de um mapa 2D armazenado como array 1D.”

---