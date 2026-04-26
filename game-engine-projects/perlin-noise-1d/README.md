# Perlin Noise 1D

Este projeto demonstra uma versão 1D de ruído Perlin usando `olcPixelGameEngine`. Ruído Perlin é uma técnica de geração procedural que cria variações suaves a partir de valores aleatórios; em vez de cada ponto mudar de forma totalmente independente, os valores são interpolados para formar curvas naturais. Esse tipo de ruído é muito usado em jogos e computação gráfica para gerar terrenos, ondas, vento, texturas, movimento orgânico e qualquer efeito que precise parecer aleatório, mas ainda contínuo.

## Como funciona

O programa começa criando um array `noiseSeed1D` com valores aleatórios entre `0` e `1`. A função `PerlinNoise1D()` percorre cada posição horizontal da tela e calcula um valor suavizado usando múltiplas oitavas. Cada oitava usa um `pitch` menor, pegando duas amostras da semente original e misturando essas amostras com interpolação linear. O resultado de todas as oitavas é acumulado em `noise`, normalizado por `scaleAccumulate` e salvo em `perlinNoise1D`.

Na renderização, cada valor de `perlinNoise1D[x]` é convertido em uma posição vertical `y`, e o programa desenha um ponto branco nessa coordenada. Assim, o ruído aparece como uma linha ondulada que muda de forma conforme a quantidade de oitavas, a semente e o `scalingBias`.

## Controles

- `SPACE`: aumenta a quantidade de oitavas; depois de `8`, volta para `1`
- `S`: gera uma nova semente aleatória
- `UP`: aumenta o `scalingBias`
- `DOWN`: diminui o `scalingBias`

## Build

Compile com:

```bash
g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17
```

## Run

```powershell
.\main.exe
```

## Notes

- Usa uma resolução interna de `256x256`
- O gráfico mostra o ruído como uma linha 1D desenhada na tela
- O código é útil para entender oitavas, interpolação e suavização antes de passar para ruído 2D

![Perlin Noise 1D](../images/perlin-noise-1d.png)
