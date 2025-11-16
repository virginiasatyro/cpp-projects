# Maze Creator — Randomized Prim (C++ / olcPixelGameEngine)

Este diretório contém uma implementação em C++ de um gerador de labirintos "perfeito" usando uma variação do algoritmo Randomized Prim (também conhecido como algoritmo de fronteira). O projeto usa a biblioteca `olcPixelGameEngine` (arquivo `../commonlib/olcPixelGameEngine.h`) para renderizar o labirinto em uma janela gráfica simples.

![Maze Generator Animation](../../img/maze-creator-random-prim.gif)

## Visão geral

- Arquivo principal: `main.cpp`
- Biblioteca gráfica: `olcPixelGameEngine` (incluída localmente em `commonlib`)
- Objetivo: gerar e visualizar um labirinto perfeito, mostrando o processo de crescimento passo a passo.

## O que você vai aprender com este código

1. Algoritmos de geração de labirintos — Randomized Prim / Frontier method.
2. Estruturas de dados usadas em geração procedural: vetores, pilha (stack) e lista de fronteira.
3. Técnicas de marcação usando operações bitwise para armazenar múltiplas flags em um único inteiro (economia de memória e operações rápidas).
4. Representação de uma grade 2D como um array 1D com indexação linear: `index = y * width + x`.
5. Arquitetura de jogo simples: ciclo de inicialização (`OnUserCreate`) e atualização por frame (`OnUserUpdate`).
6. Boas práticas para visualização passo-a-passo (pausas, passo de tempo, visualização do "fronteira/atual").

## Algoritmo (em palavras)

Este código implementa uma variação de Randomized Prim (fronteira):

1. Começa escolhendo uma célula inicial aleatória e a marca como visitada.
2. Adiciona as células vizinhas dessa célula à lista de "fronteira" (frontier).
3. Repetidamente: enquanto a lista de fronteira não estiver vazia:
   - Escolhe aleatoriamente uma célula `f` da fronteira.
   - Procura os vizinhos de `f` que já estejam marcados como visitados.
   - Conecta `f` a um desses vizinhos visitados (abrindo paredes correspondentes) — isso integra `f` à área já gerada.
   - Marca `f` como visitada e adiciona seus vizinhos não-visitados à fronteira.
   - Remove `f` da lista de fronteira.

O resultado é um labirinto perfeito: existe exatamente um caminho entre quaisquer duas células (sem loops).

> Notas: O conjunto "fronteira" aqui é armazenado como `std::vector<std::pair<int,int>> frontier` e o algoritmo escolhe aleatoriamente índices desta lista.

## Estruturas de dados e flags

Cada célula é um `int` com flags bitwise:

- `CELL_PATH_N = 0x01` — abertura ao norte
- `CELL_PATH_E = 0x02` — abertura ao leste
- `CELL_PATH_S = 0x04` — abertura ao sul
- `CELL_PATH_W = 0x08` — abertura ao oeste
- `CELL_VISITED = 0x10` — célula já visitada / parte do labirinto

Operações comuns:
- Marcar visita: `maze[index] |= CELL_VISITED;`
- Verificar: `if (maze[index] & CELL_VISITED) {...}`
- Abrir caminho entre células: setar os bits correspondentes em ambas as células (ex.: abrir norte em A e sul em B).

Representação 2D→1D: `index = y * mazeWidth + x`.

## Como compilar (Windows, via MinGW/MSYS2)

1. Instale MSYS2 e o toolchain `mingw-w64-x86_64-toolchain` (ver README geral do seu sistema).
2. Abra `MSYS2 MinGW 64-bit` ou garanta que um `g++` do MinGW64 esteja no PATH.
3. Do diretório `maze-creator-random-prim`, rode:

```powershell
# No PowerShell (com g++ disponível no PATH)
cd 'C:\Users\vivi_\github\cpp-projects\maze-creator-random-prim'
# Compilar (ajuste -I se commonlib estiver em outro local)
g++ -std=c++17 -O2 -I..\commonlib -o maze.exe main.cpp -lgdi32 -lgdiplus -lopengl32 -luser32 -lShlwapi -ldwmapi
.# Execute
.\maze.exe
```

## Estrutura do código (`main.cpp`) — guia de leitura

- Cabeçalho (comentários do arquivo): resumo, objetivos e instruções de compilação.
- Includes e macro `OLC_PGE_APPLICATION` para habilitar implementação no header da olc.
- Classe `Mazes : olc::PixelGameEngine`:
  - Membros privados: dimensões (`mazeWidth`, `mazeHeight`), array `maze`, `visitedCells`, `stack` para backtracking, `pathWidth` para render.
  - Enum de flags das células (descrição acima).
  - `OnUserCreate()` — função chamada uma vez na inicialização:
    - Inicializa parâmetros, aloca `maze` com `new int[width*height]` e zera.
    - Escolhe célula inicial aleatória e a marca como visitada, empurra na pilha.
  - `OnUserUpdate(float fElapsedTime)` — chamado a cada frame:
    - Pausa curta para desacelerar a animação (`sleep_for(10ms)`).
    - Implementa a lógica do Randomized Prim: se não visitou todas as células, gerencia a lista `frontier`, escolhe uma célula da fronteira, conecta-a a um vizinho visitado e expande a fronteira.
    - Renderiza o labirinto: loop que desenha células visitadas em branco, não visitadas em azul, o "current" em verde e ligações (path flags) como aberturas.
- `main()` — cria uma instância `Mazes` e chama `Construct` com resolução e tamanho do pixel para iniciar o motor.

## Pontos para estudo e experimentação

- Troque a semente RNG (usar `srand(time(nullptr))`) para obter reprodutibilidade/aleatoriedade controlada.
- Ajuste `mazeWidth` / `mazeHeight` e `pathWidth` para ver como muda a aparência e desempenho.
- Compare com o algoritmo de "Recursive Backtracker" (pilha) — comente/coletem o código e compare visualmente os labirintos gerados (backtracker tende a produzir longos corredores, Prim produz labirintos mais entrelaçados).
- Experimente implementar peso nas fronteiras ou heurísticas para produzir labirintos com características específicas (p.ex. mais loops, zonas abertas).

## Possíveis melhorias e notas de implementação

- Gerenciamento de memória: atualmente `maze` é alocado com `new` e não é liberado — adicionar `delete[] maze;` no destrutor (ou usar `std::vector<int>` para RAII).
- Evitar vetor `frontier` com remoções por índice O(n) (erase(begin()+i)): usar `std::deque` ou trocar por troca com último elemento e pop_back para remover em O(1) quando a ordem não importa.
- Visualização: desenhar as paredes explicitamente com linhas/pixels em vez de preencher células para melhorar legibilidade.
- Thread-safety: o código está desenhado para um único thread; cuidado se integrar com threads externas.

## Referências e leitura

- OneLoneCoder (Javidx9): exemplos e vídeos sobre pequenos projetos gráficos e algoritmos.
- Wikipedia: Maze generation algorithms — Randomized Prim, Recursive Backtracker.

---

Se quiser, eu também posso:
- Inserir comentários adicionais linha-a-linha no `main.cpp` explicando cada bloco com mais detalhe (posso editar o arquivo diretamente).
- Adicionar um `Makefile` ou `tasks.json` para compilar automaticamente.
- Substituir a alocação `new` por `std::vector<int>` e aplicar a melhoria de `frontier` para remover O(n).

