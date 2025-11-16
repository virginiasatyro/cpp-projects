# Conway's Game of Life — Autômato Celular

## Visão Geral

Este projeto implementa o famoso **Jogo da Vida de Conway** (Conway's Game of Life), um autômato celular que simula a evolução de uma população de células em uma grade bidimensional. Apesar de suas regras simples, o Jogo da Vida demonstra emergência complexa: padrões simples evoluem para estruturas intrincadas, osciladores, naves espaciais e comportamentos caóticos, oferecendo insights sobre a vida, a complexidade e sistemas autorregulatórios.

![Game of Life](../../img/game-of-life.gif)

## O Que É um Autômato Celular?

Um **autômato celular** é um modelo computacional discreto que consiste em:

1. **Grade (Grid)**: Uma matriz de células, geralmente infinita (ou com bordas envolventes).
2. **Estados**: Cada célula pode estar em um de vários estados (no Jogo da Vida: viva ou morta; 1 ou 0).
3. **Vizinhança**: Um conjunto de células próximas que influenciam a evolução de uma célula (no Jogo da Vida: os 8 vizinhos adjacentes).
4. **Regras Determinísticas**: Um conjunto de regras que determinam o próximo estado de uma célula com base em seu estado atual e no estado de seus vizinhos.
5. **Tempo Discreto**: O sistema evolui em passos discretos (gerações); todas as células atualizam simultaneamente.

Autômatos celulares são usados para modelar:
- Sistemas biológicos (crescimento, reprodução, morte)
- Sistemas físicos (difusão, cristalização)
- Sistemas sociais (propagação de rumores, padrões urbanos)
- Computação teórica (máquinas de Turing, fenômenos computáveis)

## História: Como o Jogo da Vida Surgiu

### Contexto Histórico

Em **1970**, o matemático britânico **John Horton Conway** criou o Jogo da Vida enquanto estudava autômatos celulares, particularmente inspirado pelo trabalho anterior de **John von Neumann** e **Stanisław Ulam** nos anos 1950.

### Motivação

Conway buscava criar um autômato celular que satisfizesse três critérios:

1. Não deveria haver padrão inicial levando a crescimento infinito (explodindo indefinidamente).
2. Deveria haver padrões iniciais que levassem a crescimento ou morte lentos (interessantes de observar).
3. Deveria ser simples de compreender e implementar, mas capaz de exibir comportamento complexo.

### Descoberta Revolucionária

O Jogo da Vida surpreendeu a comunidade científica por sua capacidade de:
- Gerar **padrões estáveis** (blocos, colmeias, tocas)
- Produzir **osciladores** (padrões que repetem periodicamente)
- Criar **naves espaciais** (padrões que se movem pela grade)
- Demonstrar **universalidade computacional** (pode simular uma máquina de Turing)

O jogo foi popularizado pela coluna "Mathematical Games" de **Martin Gardner** em *Scientific American* em 1970, captando a imaginação de cientistas, programadores e entusiastas.

## As Regras do Jogo da Vida

O Jogo da Vida opera em uma grade infinita (ou toroidal/envolvente). Cada célula tem dois estados: **viva (1)** ou **morta (0)**.

A cada geração, as seguintes regras são aplicadas simultaneamente a **todas as células**:

### Regra 1: Subpopulação (Morte por Isolamento)
- Uma célula viva com **menos de 2 vizinhos vivos** morre.

### Regra 2: Sobrevivência
- Uma célula viva com **2 ou 3 vizinhos vivos** sobrevive.

### Regra 3: Superpopulação (Morte por Superlotação)
- Uma célula viva com **mais de 3 vizinhos vivos** morre.

### Regra 4: Reprodução (Nascimento)
- Uma célula morta com **exatamente 3 vizinhos vivos** torna-se viva (nascimento).

Essas regras simples geram dinâmica fascinante e imprevisível.

## Algoritmo

```
1. Inicialize a grade com um estado inicial (aleatório ou padrão escolhido)
2. Para cada geração:
   a. Para cada célula (x, y):
      - Conte o número de vizinhos vivos (8 adjacências)
      - Aplique as regras de sobrevivência/morte/nascimento
      - Armazene o novo estado
   b. Atualize toda a grade com os novos estados simultaneamente
   c. Renderize a grade (vivas = verde, mortas = preto)
   d. Repita
```

**Importante**: Os novos estados são calculados com base no estado anterior (geração atual), não no estado já atualizado. Isso exige dois buffers: `state` (estado atual) e `output` (estado anterior para leitura).
dições envolventes** (toroidal) para simular uma grade sem bordas.

## Compilação

### Pré-requisitos

- Compilador: `g++` (MSYS2/MinGW-w64 no Windows)
- C++11 ou superior
- olcPixelGameEngine (em `../../commonlib/`)

### Comando de Compilação

```powershell
cd 'C:\Users\vivi_\github\cpp-projects\src\game-of-life'
g++ -std=c++17 -O2 -I../../commonlib -o life.exe main.cpp -lgdi32 -lgdiplus -lopengl32 -luser32 -lShlwapi -ldwmapi
.\life.exe
```

## Execução

```powershell
.\life.exe
```

Uma janela abrirá com a simulação:
- **Verde**: Células vivas
- **Preto**: Células mortas
- **Espaço**: Pausa/retoma a simulação

## Teste e Experimentação

### 1. Estado Inicial das Células

O estado inicial é crítico para o comportamento emergente. Experimente:

#### A) Aleatório (Padrão Atual)
```cpp
for (int i = 0; i < ScreenWidth() * ScreenHeight(); i++)
{
    state[i] = rand() % 2;  // 50% chance viva/morta
}
```
Resultado: Caótico no início; padrões estáveis emergem ao longo do tempo.

#### B) Padrão: "Glider" (Nave Espacial)
```cpp
// Limpe tudo
memset(state, 0, ScreenWidth() * ScreenHeight() * sizeof(int));

// Criar glider no canto superior esquerdo
state[1 * ScreenWidth() + 2] = 1;
state[2 * ScreenWidth() + 3] = 1;
state[3 * ScreenWidth() + 1] = 1;
state[3 * ScreenWidth() + 2] = 1;
state[3 * ScreenWidth() + 3] = 1;
```
Resultado: Padrão que se move na diagonal (famoso "glider").

#### C) Padrão: "Block" (Bloco Estável)
```cpp
memset(state, 0, ScreenWidth() * ScreenHeight() * sizeof(int));

// Bloco 2x2 no centro
int cx = ScreenWidth() / 2;
int cy = ScreenHeight() / 2;
state[cy * ScreenWidth() + cx] = 1;
state[cy * ScreenWidth() + cx + 1] = 1;
state[(cy + 1) * ScreenWidth() + cx] = 1;
state[(cy + 1) * ScreenWidth() + cx + 1] = 1;
```
Resultado: Bloco estável; nunca muda (oscilador de período 1).

#### D) Padrão: "Blinker" (Oscilador de Período 2)
```cpp
memset(state, 0, ScreenWidth() * ScreenHeight() * sizeof(int));

// 3 células vivas em linha horizontal
int cx = ScreenWidth() / 2;
int cy = ScreenHeight() / 2;
state[cy * ScreenWidth() + cx - 1] = 1;
state[cy * ScreenWidth() + cx] = 1;
state[cy * ScreenWidth() + cx + 1] = 1;
```
Resultado: Alterna entre horizontal e vertical a cada geração.

#### E) Padrão: "Beacon" (Oscilador de Período 2)
```cpp
memset(state, 0, ScreenWidth() * ScreenHeight() * sizeof(int));

int cx = ScreenWidth() / 2;
int cy = ScreenHeight() / 2;

// Dois blocos 2x2 sobrepostos
state[cy * ScreenWidth() + cx] = 1;
state[cy * ScreenWidth() + cx + 1] = 1;
state[(cy + 1) * ScreenWidth() + cx] = 1;
state[(cy + 1) * ScreenWidth() + cx + 1] = 1;

state[(cy + 1) * ScreenWidth() + cx + 1] = 1;
state[(cy + 1) * ScreenWidth() + cx + 2] = 1;
state[(cy + 2) * ScreenWidth() + cx + 1] = 1;
state[(cy + 2) * ScreenWidth() + cx + 2] = 1;
```
Resultado: Padrão que oscila entre dois estados.

#### F) Padrão: "Gosper Glider Gun" (Gerador de Gliders)
Padrão complexo que gera gliders continuamente — não trivial de implementar manualmente, mas pode ser copiado de referências online.

### 2. Taxa de Aleatoriedade

Altere a probabilidade inicial de células vivas:

```cpp
// Aumentar aleatoriedade (mais células vivas inicialmente)
for (int i = 0; i < ScreenWidth() * ScreenHeight(); i++)
{
    state[i] = (rand() % 100) < 70 ? 1 : 0;  // 70% vivas
}

// Diminuir aleatoriedade (menos células vivas)
for (int i = 0; i < ScreenWidth() * ScreenHeight(); i++)
{
    state[i] = (rand() % 100) < 20 ? 1 : 0;  // 20% vivas
}
```

Experimente valores entre 0% e 100% para ver como a população evolui.

### 3. Vizinhança Diferente

O Jogo da Vida usa vizinhança de **Moore** (8 vizinhos). Experimente vizinhança de **Von Neumann** (4 vizinhos orthogonais):

```cpp
int neighbors = 0;
neighbors += cell(x, y - 1);  // Norte
neighbors += cell(x + 1, y);  // Leste
neighbors += cell(x - 1, y);  // Oeste
neighbors += cell(x, y + 1);  // Sul
// Omita as 4 diagonais
```

Resultado: Comportamento significativamente diferente; padrões mais simples/previsíveis.

### 4. Regras Alternativas

Modifique as regras de sobrevivência/nascimento (notação B/S):
- **Jogo da Vida padrão**: B3/S23 (nasce com 3, sobrevive com 2-3)
- **Variant "HighLife"**: B36/S23 (nasce com 3 ou 6, sobrevive com 2-3)
- **Variant "Seeds"**: B2/S (nasce com 2, nunca sobrevive)

```cpp
// Exemplo: HighLife
if (cell(x, y) == 1)
{
    state[y * ScreenWidth() + x] = (neighbors == 2 || neighbors == 3) ? 1 : 0;
}
else
{
    state[y * ScreenWidth() + x] = (neighbors == 3 || neighbors == 6) ? 1 : 0;
}
```

### 5. Análise de Padrões

Mantenha registro de:
- **Tempo até estabilização**: Quantas gerações até que a população alcança um estado estável?
- **Padrões recorrentes**: Quais são os osciladores mais comuns?
- **Naves espaciais**: Com que frequência emergem padrões móveis?
- **População máxima/mínima**: Qual é a maior/menor população observada?

### 6. Condições Envolventes (Toroidal Wrapping)

Modifique a vizinhança para suportar wraparound (simule uma grade envolvente):

```cpp
auto cell_wrap = [&](int x, int y) 
{ 
    x = (x + ScreenWidth()) % ScreenWidth();
    y = (y + ScreenHeight()) % ScreenHeight();
    return output[y * ScreenWidth() + x]; 
};

// Use cell_wrap em vez de cell para permitir padrões evoluerem nas bordas
```

### 7. Rastreamento de Gerações

Adicione um contador de gerações para estudar dinâmicas de longo prazo:

```cpp
static int generation = 0;
generation++;

// Imprima a cada N gerações
if (generation % 100 == 0)
{
    printf("Geração: %d\n", generation);
    // Conte células vivas
    int alive = 0;
    for (int i = 0; i < ScreenWidth() * ScreenHeight(); i++)
    {
        alive += state[i];
    }
    printf("Vivas: %d\n", alive);
}
```

## Conceitos Matemáticos e Computacionais

### Universalidade de Turing

O Jogo da Vida é **Turing-completo**: pode simular uma máquina de Turing universal. Isso significa que:
- Qualquer algoritmo computável pode, em princípio, ser implementado no Jogo da Vida.
- Existem configurações iniciais que funcionam como "computadores" (ex.: Gosper Glider Gun).

### Emergência

O Jogo da Vida exemplifica **emergência**: comportamento complexo emerge de regras simples. Propriedades globais (padrões, osciladores) não são codificadas nas regras locais, mas surgem naturalmente.

### Caos e Estrutura

Dependendo do estado inicial:
- **Caos**: Evolução aparentemente aleatória (inicial aleatória).
- **Estrutura**: Padrões estáveis/periódicos emergem.
- **Transição de fase**: Existe uma densidade de população crítica (~20-30% para aleatório) que separa ordem e caos.

## Referências

- **Wikipedia - Conway's Game of Life**: https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
- **Wikipedia - Cellular Automaton**: https://en.wikipedia.org/wiki/Cellular_automaton
- **Game of Life (Site Oficial)**: https://www.conwaylife.com
- **OneLoneCoder**: https://www.youtube.com/c/javidx9

## Possíveis Extensões

1. **Visualização melhorada**: Colorir células por "idade" (quanto tempo estão vivas).
2. **Exportar padrão**: Salvar o estado atual em arquivo (formato RLE — Run Length Encoding).
3. **Importar padrão**: Carregar padrões famosos de arquivo.
4. **Controle de velocidade**: Variar a velocidade da simulação com teclas.
5. **Desenho interativo**: Permitir usuário "pintar" células vivas/mortas ao clicar.
6. **Estatísticas**: Gráficos de população ao longo do tempo.

## Conclusão

O Jogo da Vida é uma ferramenta poderosa para entender sistemas autorregulatórios, emergência e complexidade computacional. Suas regras simples contêm profundidade matemática, tornando-a relevante em biologia, física, ciência da computação e filosofia da vida.

---

**Autor**: Baseado em Conway's Game of Life (1970) de John Horton Conway  
**Implementação**: olcPixelGameEngine  
**Propósito**: Educacional — aprender sobre autômatos celulares, emergência e dinâmicas populacionais  
**Dificuldade**: Iniciante (conceito) a Intermediário (extensões)
