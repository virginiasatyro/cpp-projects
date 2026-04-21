# ⭐ RESUMO COMPLETO DO SISTEMA AUTOMÁTICO DE TESTES E BENCHMARK PARA SOLUÇÕES C++ DE LEETCODE

## 🎯 OBJETIVO

Criar um ambiente em que você:

* escreve apenas **as soluções** (`solutionX.cpp`)
* escreve apenas **os testes** (`tests.json`)
* não precisa de CMake
* não precisa escrever `main()` em cada solução
* não precisa compilar manualmente
* não precisa mexer no projeto quando adicionar novos problemas

E basta rodar:

```sh
python run.py <nome_do_problema>
```

E o sistema faz:

✔ compila cada solução
✔ gera automaticamente um executável
✔ injeta entrada
✔ captura saída
✔ executa os testes
✔ mede performance
✔ mostra barra de progresso
✔ gera ranking de velocidade

---

# 🧱 ESTRUTURA COMPLETA DO PROJETO

```
leet-code/
│
├── run.py
│
└── problems/
    └── problem_1_two_sum/
         ├── solution1.cpp
         ├── solution2.cpp
         ├── solution_opt.cpp
         └── tests.json
```

Você só adiciona novas pastas dentro de `problems/`.

---

# ⚙️ COMO O `run.py` FUNCIONA (passo a passo)

## 1️⃣ Identificação automática do problema

Quando você executa:

```sh
python run.py problem_1_two_sum
```

O runner procura a pasta:

```
problems/problem_1_two_sum/
```

Se encontra, ele lê:

* todos os arquivos `solution*.cpp`
* o arquivo `tests.json`

---

## 2️⃣ Leitura do arquivo `tests.json`

Formato:

```json
{
  "tests": [
    {
      "input": { "nums": [2,7,11,15], "target": 9 },
      "expected": [0,1]
    }
  ]
}
```

O runner extrai:

* lista de testes
* entradas
* saídas esperadas

---

## 3️⃣ Para cada solução (solution1.cpp, solution2.cpp, ...):

O runner gera automaticamente um arquivo **wrapper temporário** contendo:

* um `#include` apontando para a solução
* um `main()` automático que:

  * lê os dados do stdin
  * chama a classe correta:
    `Solution1().run(nums, target)`
  * imprime a resposta

**Você NÃO escreve nenhum `main()` manual.**

---

## 4️⃣ Compilação automática

O runner chama:

```
g++ -std=c++20 -O2 wrapper.cpp solutionX.cpp -o solutionX.exe
```

✔ Compila a solução
✔ Evita erros de WinMain
✔ Funciona no Windows + MinGW
✔ Funciona no Linux/Mac também

---

## 5️⃣ Execução dos testes

Para cada caso de teste:

O runner injeta a entrada no stdin do executável:

```
<quantidade> <números> <target>
```

Exemplo:

```
4 2 7 11 15 9
```

O executável gera saída:

```
0 1
```

O runner compara com:

```json
"expected": [0,1]
```

Se bate → PASS
Se não bate → FAIL

---

## 6️⃣ Barra de progresso

Durante os testes:

```
[####################......................] 50.0%
```

Esto aparece mesmo no Windows (ASCII safe).

---

## 7️⃣ Medição de tempo

A cada solução:

* tempo total de execução é medido via `time.perf_counter()`
* isso permite comparar performance real

---

## 8️⃣ Ranking final

Ao final:

```
=== Ranking ===
1. solution_opt.cpp     0.00012s   (PASS)
2. solution2.cpp        0.00021s   (PASS)
3. solution1.cpp        0.00181s   (PASS)
```

Permite ver:

* correção
* velocidade
* impacto de otimizações

---

# 🧠 INTELIGÊNCIA DO RUNNER

## ✔ 1. Identifica automaticamente os arquivos de solução

```
solution*.cpp
```

Então você pode criar:

```
solution1.cpp
solution_hash.cpp
solution_mem_opt.cpp
solution_fastest.cpp
```

Sem mudar nada no runner.

---

## ✔ 2. Descobre automaticamente o nome da classe C++

Para:

```
solution_opt.cpp
```

Ele gera:

```
SolutionOpt
```

Pela função:

```python
def infer_classname(src):
    base = os.path.splitext(os.path.basename(src))[0]
    parts = base.split('_')
    parts = [p.capitalize() for p in parts]
    return "".join(parts)
```

---

## ✔ 3. Wrapper universal com main()

O runner cria um arquivo temporário contendo:

```cpp
#include "solution_opt.cpp"
#include <iostream>

int main() {
    ...
    auto out = SolutionOpt().run(nums, target);
    ...
}
```

Você **não precisa escrever main() em nenhuma solução**.

---

## ✔ 4. Caminho absoluto no include

Para garantir que o compilador encontra o arquivo mesmo no Windows com MinGW:

```
#include "C:/Users/.../solution_opt.cpp"
```

Isso evita:

* problemas de include
* erros de diretório atual
* diferenças entre Windows/Linux

---

# 💡 O que você precisa fazer?

Apenas 2 coisas:

## ✔ 1. Criar soluções C++ (apenas a classe)

```cpp
#include <vector>
class Solution1 {
public:
    std::vector<int> run(const std::vector<int>& nums, int target) {
        ...
    }
};
```

Sem main, sem includes adicionais.

---

## ✔ 2. Criar testes.json

Pronto.

---

# 💥 O que você GANHA com esse sistema?

### ✔ Automação total

### ✔ Não toca em CMake

### ✔ Não escreve main()

### ✔ Sistema portátil (Windows/Linux/Mac)

### ✔ Separação limpa de soluções por problema

### ✔ Benchmarks automáticos

### ✔ Compatível com centenas de problemas

### ✔ Ultra simples de usar

---

# 🚀 Exemplo de uso real:

```sh
python run.py problem_1_two_sum
python run.py problem_3_longest_substring
python run.py problem_4_median_two_sorted_arrays
```

Cada pasta segue:

```
problems/<nome>/
    solution*.cpp
    tests.json
```

---
