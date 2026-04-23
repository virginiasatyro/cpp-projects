# Maze Creator - Recursive Backtracking Algorithm

Uma visualização animada de geração de labirintos usando o algoritmo de **Recursive Backtracking** (Backtracking Recursivo).

![Maze Generator Animation](../../images/maze-creator.gif)

## 📋 Visão Geral

Este projeto implementa um gerador de labirintos perfeito (sem loops) que cria mazes de forma visual e animada. O algoritmo usa uma técnica de **profundidade primeiro (DFS - Depth-First Search)** com **backtracking** para garantir que todos os cômodos sejam visitados exatamente uma vez, gerando labirintos válidos e sem isolas isoladas.

### O que você aprenderá

1. **Algoritmos de Geração de Labirintos**
   - Recursive Backtracking (Profundidade Primeiro com Backtrack)
   - Conceito de células visitadas vs. não visitadas
   - Caminhos direcionais (N, S, E, W)

2. **Estruturas de Dados**
   - Uso de `std::stack` para manter histórico de posições (backtracking)
   - Manipulação de bits (bitwise operations) para armazenar múltiplas flags em um único `int`
   - Alocação dinâmica de memória para grid 2D (representado como array 1D)

3. **Programação Orientada a Objetos**
   - Herança de classes (Mazes herda de `olc::PixelGameEngine`)
   - Sobrescrita de métodos (override)
   - Encapsulamento de dados (privado, protegido)

4. **Conceitos de Rendering**
   - Loop de game (OnUserCreate, OnUserUpdate)
   - Desenho de pixels em tela
   - Animação em tempo real

5. **Técnicas de Otimização**
   - Representação compacta de dados usando operações bitwise
   - Indexação linear de arrays 2D
   - Sleep/delay para controlar velocidade de animação

---

## 🎨 Como Funciona

### Inicialização (OnUserCreate)

1. Define dimensões do labirinto: **40x25 células**
2. Aloca memória dinâmica para grid (`maze = new int[width * height]`)
3. Escolhe uma célula inicial aleatória
4. Marca como visitada e coloca na pilha (stack)

### Algoritmo Principal (OnUserUpdate - a cada frame)

```
Enquanto não visitou todas as células:
  1. Obtenha vizinhos não-visitados da célula atual
  2. Se houver vizinhos disponíveis:
     - Escolha um vizinho aleatório
     - Crie um caminho entre célula atual e vizinho
     - Marque vizinho como visitado
     - Empilhe vizinho (move para vizinho)
  3. Se não houver vizinhos:
     - Faça backtrack: desempilhe (volta à célula anterior)
```

### Representação de Dados

Cada célula é um `int` (32 bits) que armazena múltiplas informações usando **bitwise flags**:

```
Bit 0 (0x01):   CELL_PATH_N  - Caminho para NORTE
Bit 1 (0x02):   CELL_PATH_E  - Caminho para LESTE
Bit 2 (0x04):   CELL_PATH_S  - Caminho para SUL
Bit 3 (0x08):   CELL_PATH_W  - Caminho para OESTE
Bit 4 (0x10):   CELL_VISITED - Célula foi visitada
```

Exemplo: `maze[i] = 0x15` significa:
- Bit 0: 1 (caminho NORTE ✓)
- Bit 4: 1 (visitada ✓)
- Resto: 0

### Visualização

- **Branco**: célula visitada (parte do labirinto)
- **Azul**: célula não visitada (parede)
- **Verde**: célula atual (sendo explorada)

---

## 🔑 Conceitos-Chave Explicados

### 1. Bitwise Operations (Operações Bitwise)

```cpp
// Usar bitwise OR (|=) para SET flag
maze[i] |= CELL_PATH_N;  // "Liga" o bit de CELL_PATH_N

// Usar bitwise AND (&) para CHECK flag
if (maze[i] & CELL_VISITED) { /* célula visitada */ }

// Combinar múltiplas flags
maze[i] |= CELL_VISITED | CELL_PATH_S;  // Marca visitada E cria caminho sul
```

**Por que usar?** Eficiência: economiza memória (5 flags em 5 bits vs. 5 bools inteiros).

### 2. Stack (Pilha) e Backtracking

```cpp
std::stack<std::pair<int, int>> stack;  // Pilha de posições (x, y)
```

- `push()`: adiciona posição quando se move para nova célula
- `top()`: obtém célula atual
- `pop()`: volta à célula anterior (backtrack)

### 3. Indexação Linear de Arrays 2D

Para converter coordenada 2D em índice linear:

```cpp
int index = y * width + x;
```

Vantagem: aloca uma vez (`new int[width*height]`) vs. alocação dupla.

### 4. Lambda Functions

```cpp
auto offset = [&](int x, int y)
{ return (stack.top().second + y) * mazeWidth + (stack.top().first + x); };
```

Calcula índice relativo à célula atual. `[&]` captura tudo por referência.

---

## 📊 Complexidade

- **Tempo**: O(W × H) — visita cada célula uma vez
- **Espaço**: O(W × H) — pilha pode crescer até W × H no pior caso (caminho muito longo)
- **Garantias**: Labirinto perfeito (apenas um caminho entre dois pontos)

---

## 🚀 Compilação e Execução

### Pré-requisitos

- **Compilador**: g++ (MSYS2/MinGW-w64 recomendado no Windows)
- **C++ Standard**: C++17 ou superior
- **Biblioteca**: olcPixelGameEngine (incluída em `../commonlib/`)

### Compilar

```bash
cd maze-creator
g++ -std=c++17 -O2 -I../commonlib -o maze.exe main.cpp \
    -lgdi32 -lgdiplus -lopengl32 -luser32 -lShlwapi -ldwmapi
```

### Executar

```bash
.\maze.exe
```

A janela abrirá mostrando a geração animada do labirinto em tempo real.

---

## 🎓 Extensões e Melhorias

1. **Algoritmos Alternativos**
   - Prim's Algorithm (randomizado)
   - Kruskal's Algorithm
   - Eller's Algorithm (gera linha por linha)

2. **Visualizações Avançadas**
   - Mostrar "distância" da entrada (gradiente de cores)
   - Resolver labirinto visualmente (A* algorithm)
   - Múltiplos labirintos gerando simultaneamente

3. **Interatividade**
   - Pausar/retomar geração
   - Controlar velocidade com teclas
   - Salvar labirinto em arquivo
   - Carregar e resolver labirinto existente

4. **Conceitos a Explorar**
   - Flood Fill Algorithm (preenchimento de áreas)
   - Path Finding (encontrar caminho mais curto)
   - Maze Solving (resolver labirinto automaticamente)

---

## 📚 Referências

- [Video Tutorial - OneLoneCoder](https://www.youtube.com/watch?v=Y37-gB83HKE)
- [OneLoneCoder GitHub - Maze Implementation](https://github.com/OneLoneCoder/Javidx9/blob/master/ConsoleGameEngine/SmallerProjects/OneLoneCoder_Mazes.cpp)
- [olcPixelGameEngine Documentation](https://github.com/OneLoneCoder/olcPixelGameEngine)

---

## 💡 Dicas de Aprendizado

1. **Breakpoints**: Adicione pontos de parada no algoritmo de geração para ver passo-a-passo
2. **Modificações**:
   - Altere `mazeWidth` e `mazeHeight` para diferentes tamanhos
   - Mude cores de `CELL_VISITED` e `CELL_UNVISITED`
   - Aumente/diminua delay para controlar velocidade
3. **Visualização**: Desenhe as operações bitwise em papel para entender
4. **Comparação**: Compare com outros algoritmos para ver diferenças

---

## 📝 Notas Técnicas

### Por que `new` e `delete` (alocação dinâmica)?

O tamanho da grid é conhecido apenas em tempo de execução (`OnUserCreate`), então precisa alocação dinâmica em heap. Em código moderno, considere usar `std::vector` para segurança automática.

### Thread Safety (Sleep)

```cpp
std::this_thread::sleep_for(std::chrono::milliseconds(10));
```

Pausa 10ms a cada frame, criando animação visual. Sem isso, labirinto seria gerado instantaneamente.

### Macro `#define OLC_PGE_APPLICATION`

Necessário para compilar com `olcPixelGameEngine.h`. Ativa implementação da classe.

---

**Autor**: Baseado em OneLoneCoder (Javidx9)  
**Propósito**: Aprendizado de algoritmos, estruturas de dados e game loops
