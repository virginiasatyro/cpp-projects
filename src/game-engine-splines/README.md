# Splines — Curvas de Interpolação Catmull-Rom

## Visão Geral

Este projeto implementa um visualizador interativo de **Splines Catmull-Rom**, um tipo de curva matemática que passa por um conjunto de pontos de controle, criando um caminho suave e contínuo. O programa permite manipular pontos de controle em tempo real, visualizar a curva resultante, e demonstrar como calcular a **derivada (gradiente)** da spline — útil para aplicações como caminhos de IA, animações e trajetórias de objetos.

![Splines](../../img/splines.gif)

## O Que São Splines?

### Definição Histórica

A palavra "spline" vem de réguas flexíveis de madeira ou plástico (chamadas "spline" em inglês) usadas por desenhistas e engenheiros. Essas réguas podiam ser curvadas para passar por um conjunto de pontos, criando curvas suaves e naturais sem kinks ou descontinuidades.

### Definição Matemática

Uma **spline** é uma função contínua definida por dois ou mais pontos de controle, construída como um conjunto de polinômios conectados em intervalos (segmentos). Características principais:

1. **Interpolação**: A curva passa exatamente pelos pontos de controle (ou perto deles, dependendo do tipo).
2. **Suavidade**: A curva é contínua em posição e geralmente em derivadas (tangente, curvatura).
3. **Controle Local**: Mover um ponto afeta principalmente a região próxima a ele (não toda a curva).
4. **Eficiência Computacional**: Fácil de calcular usando polinômios de baixo grau (típico: cúbicos).

### Tipos Principais de Splines

| Tipo | Características | Uso Comum |
|------|-----------------|-----------|
| **Linear** | Conecta pontos com retas | Traços simples, baixo custo |
| **Quadrática** | Polinômios de grau 2 | Menos comum; balanço entre velocidade e suavidade |
| **Cúbica** | Polinômios de grau 3 | Padrão em gráficos, animação; bom balanço |
| **Bézier** | Usa 4 pontos por segmento | Design (Illustrator, fontes), animação |
| **B-Spline** | Generalização mais flexível | CAD, modelagem 3D |
| **Catmull-Rom** | **Cúbica interpoladora** | Animação de câmera, caminhos de IA (Este projeto!) |

## Catmull-Rom Splines

### O Que É?

**Catmull-Rom spline** é um tipo de spline cúbica (polinômio de grau 3) que:
- **Passa exatamente** pelos pontos de controle (interpoladora)
- Usa 4 pontos por segmento: $P_0, P_1, P_2, P_3$
- Calcula a curva **entre** $P_1$ e $P_2$ considerando a "direção" dos pontos $P_0$ e $P_3$
- Resulta em curva suave com C² continuidade (contínua em posição e primeira derivada)

### Formulação Matemática

Para um parâmetro $t \in [0,1)$ no segmento entre $P_1$ e $P_2$:

$$P(t) = \frac{1}{2} \begin{pmatrix} 1 & t & t^2 & t^3 \end{pmatrix} \begin{pmatrix} 0 & 2 & 0 & 0 \\ -1 & 0 & 1 & 0 \\ 2 & -5 & 4 & -1 \\ -1 & 3 & -3 & 1 \end{pmatrix} \begin{pmatrix} P_0 \\ P_1 \\ P_2 \\ P_3 \end{pmatrix}$$

Alternativamente, em forma de pesos de blending:

$$P(t) = 0.5 \left( P_0 \cdot q_1(t) + P_1 \cdot q_2(t) + P_2 \cdot q_3(t) + P_3 \cdot q_4(t) \right)$$

Onde:
- $q_1(t) = -t^3 + 2t^2 - t$
- $q_2(t) = 3t^3 - 5t^2 + 2$
- $q_3(t) = -3t^3 + 4t^2 + t$
- $q_4(t) = t^3 - t^2$

### Derivada (Gradiente)

A **tangente** à spline em qualquer ponto é calculada diferenciando $P(t)$:

$$P'(t) = 0.5 \left( P_0 \cdot q_1'(t) + P_1 \cdot q_2'(t) + P_2 \cdot q_3'(t) + P_3 \cdot q_4'(t) \right)$$

Onde:
- $q_1'(t) = -3t^2 + 4t - 1$
- $q_2'(t) = 9t^2 - 10t$
- $q_3'(t) = -9t^2 + 8t + 1$
- $q_4'(t) = 3t^2 - 2t$

A derivada é útil para:
- Determinar a **direção** de movimento (ângulo da câmera, rumo do agente)
- Calcular **velocidade** (magnitude da tangente)
- Aplicar **física** (rotação para acompanhar o caminho)

## Histórico e Origem

### Desenvolvimento da Spline Cúbica

1. **1940s-1950s**: Criação de splines por desenhistas e engenheiros (réguas flexíveis).
2. **1956**: Primeiros trabalhos matemáticos formais — Schoenberg em splines polinômicas.
3. **1974**: **Edwin Catmull** e **Raphael Rom** propõem a **Catmull-Rom spline** como solução elegante para interpolação de pontos de controle.
4. **1980s-1990s**: Adoção generalizada em CG, animação de câmera, sistemas de efeitos visuais.
5. **Hoje**: Ubíqua em engines de jogos, software de design, animação 3D.

### Por Que Catmull-Rom?

- **Intuitivo**: Curva passa pelos pontos (controle visual direto)
- **Suave**: Derivadas contínuas (movimento natural)
- **Eficiente**: Apenas 4 pontos necessários por segmento
- **Local**: Mover um ponto afeta principalmente vizinhos próximos

## Estrutura do Código

### Estrutura de Dados

```cpp
struct point2D {
    float x, y;
};

struct Spline {
    std::vector<point2D> points;  // Pontos de controle
    
    point2D getSplinePoint(float t, bool looped = false);     // Calcula posição
    point2D getSplineGradient(float t, bool looped = false);  // Calcula tangente
};
```

### Métodos Principais

#### `getSplinePoint(float t, bool looped)`

- **Entrada**: Parâmetro $t$ (posição ao longo da spline) e flag `looped`.
- **Processo**:
  1. Determina os 4 índices de pontos: $P_0, P_1, P_2, P_3$
  2. Se `looped=true`, usa aritmética modular para envolver (trata como loop)
  3. Calcula pesos de blending ($q_1, q_2, q_3, q_4$)
  4. Interpola X e Y separadamente
- **Saída**: Ponto $(x, y)$ na spline

#### `getSplineGradient(float t, bool looped)`

- Calcula a **derivada** (tangente) em qualquer ponto $t$
- Usa fórmulas derivadas dos pesos de blending
- Retorna vetor de direção (não normalizado)
- Usado para calcular ângulo de rotação do "agente" (nave, câmera, etc.)

### Controles Interativos

| Tecla | Ação |
|-------|------|
| **X** | Próximo ponto de controle |
| **Z** | Ponto anterior |
| **← / →** | Mover ponto esquerda/direita |
| **↑ / ↓** | Mover ponto cima/baixo |
| **A / S** | Mover marcador ao longo spline (retroativo/avanço) |

## Compilação

### Pré-requisitos

- Compilador: `g++` (MSYS2/MinGW-w64 no Windows)
- C++17 ou superior
- olcPixelGameEngine (em `../../commonlib/`)

### Comando

```powershell
cd 'C:\Users\vivi_\github\cpp-projects\src\splines'
g++ -std=c++17 -O2 -I../../commonlib -o splines.exe main.cpp -lgdi32 -lgdiplus -lopengl32 -luser32 -lShlwapi -ldwmapi
.\splines.exe
```

## Execução

```powershell
.\splines.exe
```

Abrirá uma janela com:
- **Pontos vermelhos**: Pontos de controle (nós)
- **Ponto amarelo**: Ponto selecionado (número ao lado)
- **Curva branca**: Spline Catmull-Rom interpolando os pontos
- **Linha azul**: Vetor de gradiente (tangente) — indica direção

## Teste e Experimentação

### 1. Manipular Pontos de Controle

- Use **X/Z** para selecionar ponto
- Use **← / → / ↑ / ↓** para mover
- Observe como a curva se adapta suavemente
- Teste: Coloque pontos em linha reta vs. em padrão complexo

**Esperado**: A curva passa por todos os pontos; mover um ponto afeta principalmente vizinhos próximos.

### 2. Rastrear o Marcador (Agente)

- Use **A/S** para mover o marcador ao longo da spline
- Observe a **linha azul** (gradiente) — indica direção de movimento
- A linha azul **sempre aponta tangente** à curva

**Esperado**: O agente segue a curva suavemente; a direção muda continuamente e suavemente.

### 3. Comparar Splines Abertas vs. Fechadas (Looped)

**Atual**: `looped = true` — a spline se fecha em loop (última volta para primeira).

Teste **aberta**:
```cpp
// Na função getSplinePoint, troque:
point2D pos = path.getSplinePoint(t, false);  // Mude para false
```

**Diferenças**:
- **Looped**: Curva contínua (boa para caminhos circulares)
- **Aberta**: Começo e fim explícitos (melhor para trajetórias)

### 4. Densidade da Curva

Na função de desenho, ajuste `t += 0.005`:

```cpp
for (float t = 0.0; t < (float)path.points.size(); t += 0.005)  // Menor = mais denso
```

- **0.01**: Menos pontos, curva "pixelada"
- **0.005**: Padrão (suave)
- **0.001**: Muito denso (mais lento)

### 5. Ângulo do Agente

O código calcula o ângulo usando:
```cpp
float r = atan2(-g1.y, g1.x);
```

Experimente **visualizar o ângulo**:
```cpp
printf("Ângulo: %f radianos = %f graus\n", r, r * 180.0 / M_PI);
```

### 6. Adicionar Múltiplas Splines

Crie várias splines e renderize todas:

```cpp
std::vector<Spline> paths;  // Múltiplas splines

// Desenhe todas
for (auto& sp : paths) {
    for (float t = 0.0; t < sp.points.size(); t += 0.005) {
        point2D pos = sp.getSplinePoint(t, true);
        Draw(pos.x, pos.y, olc::CYAN);  // Cores diferentes
    }
}
```

### 7. Análise de Curvatura

Implemente a **segunda derivada** (aceleração) para medir curvatura:

```cpp
point2D getSplineAccel(float t, bool looped = false) {
    // Diferenciar getSplineGradient
    // Resultado: mede quanto a curva está "curvando"
}
```

**Uso**: Velocidade deveria **diminuir em curvas apertadas** (mais realista).

## Melhorias e Próximos Passos

### Curto Prazo

1. **Exportar/Importar Caminhos**
   - Salvar spline em arquivo (JSON ou texto simples)
   - Carregar padrões pré-definidos
   ```cpp
   void saveSpline(const std::string& filename, const Spline& sp);
   Spline loadSpline(const std::string& filename);
   ```

2. **Criar Padrões Predefinidos**
   ```cpp
   Spline createCircle(float centerX, float centerY, float radius, int numPoints);
   Spline createWave(float amplitude, float frequency);
   ```

3. **Interatividade Melhorada**
   - Clicar para adicionar novos pontos
   - Arrastar pontos com mouse
   - Deletar pontos (tecla DEL)
   - Inserir ponto entre dois existentes (tecla INS)

### Médio Prazo

4. **Visualização Avançada**
   - Desenhar **pontos de controle conectados** (mostra estrutura)
   - Mostrar **caixa envolvente** (bounding box)
   - Animação de agente seguindo caminho com velocidade variável
   - Mostrar **tangentes** e **normais** em todos os pontos

5. **Física Aplicada**
   - Agente tem **inércia** (acelera/desacelera naturalmente)
   - Força **centrípeta** em curvas (agente desvia do caminho)
   - **Fricção** afeta velocidade
   ```cpp
   velocity += gradient * acceleration;
   position += velocity * dt;
   velocity *= damping;  // Fricção
   ```

6. **Detecção de Colisão**
   - Agente colide com **obstáculos**
   - Spline **desvia automaticamente** (geração procedural)
   ```cpp
   if (distanceToObstacle(pos) < radius) {
       // Recompute path
   }
   ```

### Longo Prazo

7. **3D e Câmera**
   - Estender para **splines 3D** (3 coordenadas: x, y, z)
   - Usar spline como **trilho de câmera** em jogo/filme
   ```cpp
   struct point3D { float x, y, z; };  // Adicionar z
   ```
   - Calcular **look-ahead** (câmera aponta para adiante na curva)

8. **Otimização e Parametrização**
   - **Reparametrização por comprimento de arco**: $t$ baseado em distância real (não paramétrica)
   - **Adaptive sampling**: Mais pontos em curvas apertadas, menos em retas
   - **Level of Detail (LOD)**: Diferentes resoluções para diferentes distâncias

9. **Ferramentas de Design**
   - **GUI interativa**: Arrastar, escalar, rotacionar splines
   - **Modo de edição**: Adicionar/remover/ajustar pontos
   - **Preview em tempo real**: Ver resultado enquanto edita
   - **Biblioteca de splines**: Salvar/carregar padrões

10. **Geração Procedural**
    - Gerar **splines aleatórias** com certas propriedades (suavidade, comprimento)
    - **Splines evolucionárias**: Otimizar baseado em critérios (menor curvatura, comprimento)
    - Usar para criar **mapas procedurais** (caminhos em jogos)

## Casos de Uso Práticos

### 1. Animação e VFX
- **Trilhos de câmera**: Suavemente seguir ação em cena
- **Trajetórias de partículas**: Caminhos fluidos para efeitos visuais
- **Animação de personagem**: Movimento suave de ponto A a B

### 2. Game Development
- **Caminhos de IA**: Inimigos seguem rotas suaves
- **Spline de corrida**: Trajetória ótima em jogos de corrida
- **Cutscenes**: Movimento de câmera cinemático

### 3. Robótica e Controle
- **Planejamento de trajetória**: Robô segue caminho suave
- **Controle de movimento**: Transições suaves entre pontos
- **Navegação**: Paths para drones, veículos autônomos

### 4. Design e CAD
- **Modelagem de curvas**: Desenhar formas orgânicas
- **Tipografia**: Design de fontes (contornos de letras)
- **Engenharia**: Superfícies suaves em design industrial

## Referências

### Wikipedia
- [Spline (matemática)](https://pt.wikipedia.org/wiki/Spline)
- [Catmull-Rom Spline (English)](https://en.wikipedia.org/wiki/Centripetal_Catmull%E2%80%93Rom_spline)
- [Interpolation](https://en.wikipedia.org/wiki/Interpolation)
- [Bezier Curve](https://en.wikipedia.org/wiki/B%C3%A9zier_curve)

### Acadêmicas
- [Parametric Curves (Math World)](https://mathworld.wolfram.com/ParametricCurves.html)
- [Catmull-Rom Spline Matematics](https://en.wikibooks.org/wiki/Cg_Programming/Rasterization/Interpolation)

### Tutoriais
- [OneLoneCoder - Splines (YouTube)](https://www.youtube.com/watch?v=9_aJGUTePYo)
- [Interactive Spline Visualization](https://www.desmos.com/) — use para visualizar equações

### Implementações Relacionadas
- [Pix Engine (olcPixelGameEngine GitHub)](https://github.com/OneLoneCoder/olcPixelGameEngine)
- [Game Engine Architecture - Spline Systems](https://www.gameenginebook.com/)

## Conclusão

Splines Catmull-Rom são ferramentas poderosas e versáteis para criar caminhos suaves, animações fluidas e interfaces elegantes. Este projeto fornece uma base sólida para entender a matemática e a implementação prática. Os próximos passos exploram otimizações, extensões 3D e aplicações práticas em games e animação.

---

**Autor**: Baseado em OneLoneCoder (Javidx9)  
**Implementação**: olcPixelGameEngine  
**Propósito**: Educacional — aprender splines, interpolação e cálculo de tangentes  
**Dificuldade**: Intermediário (conceito matemático moderadamente complexo)
