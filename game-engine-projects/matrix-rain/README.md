# Matrix

# Matrix Rain

## Visão Geral

Este projeto recria o efeito visual conhecido como **Matrix Rain**: colunas de caracteres descendo pela tela em velocidades diferentes, com a ponta mais brilhante e rastros em tons de verde. A implementação foi feita em **C++** usando a **olcPixelGameEngine** para desenhar cada caractere em tempo real.

O resultado é uma animação simples, mas boa para estudar:

- renderização 2D baseada em texto;
- atualização por frame com `fElapsedTime`;
- uso de listas e estruturas simples para controlar várias entidades;
- geração procedural com aleatoriedade.

## Como a animação funciona

<images src="../images/matrix-rain.gif" alt="Matrix Rain" width="50%">

Cada "chuva" é representada por uma struct `Streamer`, que contém:

- `column`: coluna horizontal onde a sequência será desenhada;
- `position`: posição vertical atual da cabeça do fluxo;
- `speed`: velocidade de descida;
- `text`: sequência de caracteres exibida.

Na inicialização, o programa cria até `250` streamers. Para cada um deles:

1. escolhe uma coluna aleatória;
2. define uma velocidade aleatória;
3. gera uma string com tamanho entre `10` e `89` caracteres;
4. preenche essa string com caracteres ASCII visíveis.

Durante a execução:

1. a posição vertical de cada streamer é atualizada com base no tempo de frame;
2. cada caractere da sequência é desenhado na mesma coluna, em linhas sucessivas;
3. a cabeça da sequência fica em branco, os primeiros caracteres do rastro ficam em cinza e o restante em verde;
4. alguns caracteres mudam aleatoriamente ao longo do tempo para dar sensação de fluxo dinâmico;
5. quando o streamer sai completamente da tela, ele é reinicializado com novos valores.

## Detalhes visuais atuais

- Resolução da janela: `1000 x 600`
- Escala de pixel: `1 x 1`
- Quantidade máxima de streamers: `250`
- Faixa de velocidade: `5` a `44`
- Caracteres usados: ASCII visível (`33` a `125`)

Observação: o código usa `DrawString()` e multiplica a posição vertical por `8`, porque a fonte padrão da engine desenha os caracteres em uma grade vertical baseada nesse tamanho.

## Estrutura do projeto

```text
matrix-rain/
|-- main.cpp
|-- main.exe
|-- notes.coffee
`-- README.md
```

Arquivo principal:

- [main.cpp](/c:/Users/vivi_/github/cpp-projects/src/game-engine-projects/matrix-rain/main.cpp)

## Compilação

### Pré-requisitos

- `g++` com suporte a C++17
- biblioteca `olcPixelGameEngine.h`
- dependências gráficas do Windows (`gdi32`, `gdiplus`, `opengl32`, `user32`, `Shlwapi`, `dwmapi`)

### Comando

No diretório `src/game-engine-projects/matrix-rain`:

```powershell
g++ -std=c++17 -O2 -I../../../commonlib -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi
```

Se quiser compilar a partir da raiz do repositório:

```powershell
g++ -std=c++17 -O2 -Icommonlib -o src/game-engine-projects/matrix-rain/main.exe src/game-engine-projects/matrix-rain/main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi
```

## Execução

```powershell
.\main.exe
```

Ao executar, uma janela abrirá exibindo a chuva de caracteres em movimento contínuo.

## Pontos do código que valem estudo

- `prepareStreamer(Streamer *s)`: reinicializa uma coluna com novos dados aleatórios.
- `randomCharacter()`: gera os símbolos usados na animação.
- `OnUserCreate()`: cria o conjunto inicial de streamers.
- `OnUserUpdate(float fElapsedTime)`: atualiza posições, redesenha os caracteres e recicla streamers que saíram da tela.

## Limitações atuais

O código já funciona, mas ainda tem alguns pontos observados nas notas do projeto:

- as colunas podem se sobrepor;
- os caracteres usados ainda não simulam o visual de katakana típico do filme;
- não há controles de teclado para variar densidade, velocidade ou pausar;
- o efeito visual é baseado apenas em texto ASCII.

## Possíveis melhorias

- substituir ASCII por um conjunto customizado de símbolos;
- evitar colisão visual entre streamers na mesma coluna;
- adicionar controles para pausar, reiniciar e alterar velocidade;
- variar brilho e comprimento das caudas;
- permitir fullscreen ou resolução configurável;
- adicionar efeito de fade mais suave no rastro.

## Referências

- OneLoneCoder: https://github.com/OneLoneCoder/Javidx9
- Vídeo de referência: https://www.youtube.com/watch?v=s7EbrvA188A
- Implementação relacionada: https://github.com/Joseph21-6147/From-ConsoleGameEngine-to-PixelGameEngine/blob/main/OneLoneCoder_Matrix.PGE.cpp
