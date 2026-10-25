# avl-rb-trees

Implementação de **Árvore AVL** e **Árvore Rubro-Negra Caída para a Esquerda (Left-Leaning Red-Black Tree - LLRB 2-3)** em C++17 para a disciplina de Estruturas de Dados Básicas (EDB).

---

## Funcionalidades Implementadas

Tanto a **AVLTree** quanto a **LLRBTree** possuem suporte completo às operações fundamentais e instrumentação para análise empírica:

1. **Inserção de Elementos (`insert(int id)`)**:
   - `AVLTree`: Inserção recursiva com cálculo de altura e balanceamento por rotações simples e duplas (LL, RR, LR, RL).
   - `LLRBTree`: Inserção 2-3 LLRB garantindo que links vermelhos inclinem à esquerda, corrigindo links vermelhos à direita com rotação à esquerda, links vermelhos consecutivos com rotação à direita, e divisão de 4-nós com inversão de cores (`flip_colors`). A raiz é sempre mantida na cor PRETA (`BLACK`). Lança exceção `std::invalid_argument` em caso de chaves duplicadas.

2. **Remoção de Elementos (`remove(int id)`)**:
   - `AVLTree`: Remoção com substituição pelo menor elemento da subárvore direita (sucessor) e rebalanceamento da árvore.
   - `LLRBTree`: Remoção 2-3 LLRB completa utilizando os métodos auxiliares `move_red_left`, `move_red_right`, `delete_min` e `balance`, preservando perfeitamente o equilíbrio negro e as propriedades de árvore rubro-negra.

3. **Busca de Elementos (`search(int id)`)**:
   - Busca binária eficiente retornando o ponteiro para o nó encontrado ou `nullptr`.

4. **Percurso Em Ordem (`inorder()`)**:
   - Visita em ordem simétrica (in-order) exibindo todos os elementos ordenados de forma crescente no terminal.

5. **Impressão da Estrutura no Terminal (`print_tree()`)**:
   - Impressão "deitada": a raiz fica à esquerda, os filhos **direitos aparecem acima** e os **esquerdos abaixo** de cada nó, ligados por `┌──` e `└──`.
   - Na **LLRBTree**, os nós são identificados com **`(R)`** para vermelho e **`(B)`** para preto ao lado do valor do nó.
   - Na **AVLTree**, cada nó exibe a altura (`h`) e o fator de balanceamento com sinal (`fb=+1`, `fb=0`, `fb=-1`).

6. **Estatísticas**:
   - Medição de tempo com precisão de microssegundos via `<chrono>`.
   - Contagem precisa de **rotações** e **balanceamentos** (e de **inversões de cores** na LLRB, via `flip_colors`).

---

## Menu Interativo e Testes Automáticos

A `main.cpp` oferece um sistema interativo completo:

1. **Interagir com as árvores**:
   - Escolha entre interagir com a **AVL** ou com a **Rubro-Negra (LLRB)**.
   - Operações interativas:
     - Inserir elemento
     - Remover elemento
     - Buscar elemento
     - Imprimir percurso em ordem
     - Imprimir estrutura da árvore no console
     - Voltar

2. **Realizar testes automáticos e de estresse**:
   - **Opções disponíveis**:
     - **Bateria completa**: executa sequencialmente para **1000, 2000, 4000 e 8000 elementos** gerando a tabela consolidada de escalabilidade.
     - **Quantidade personalizada**: valor seguro definido pelo usuário no intervalo validado de **10 a 50.000 elementos**.
     - **Voltar ao menu principal**.
   - **Cenários testados em cada tamanho**:
     - Inserção em ordem **Crescente** ($1 \dots N$)
     - Inserção em ordem **Decrescente** ($N \dots 1$)
     - Inserção em ordem **Não Ordenada / Aleatória**
     - Busca de $N$ elementos existentes
     - Busca de $N$ elementos inexistentes
     - Remoção em massa ($N/2$ elementos)
   - **Saída do teste**: o terminal exibe apenas a **tabela consolidada** de escalabilidade e, logo abaixo, uma seção separada de **casos de borda** (árvore vazia, árvore com 1 elemento e remoções de folha, 1 filho, 2 filhos, raiz e chave inexistente), com resultado `OK`/`FALHA` para cada árvore.
   - **Tabela Consolidada de Escalabilidade**:
     - Comparação de tempo, altura final, rotações e balanceamentos lado a lado para cada $N$, comprovando a escala logarítmica ($O(\log N)$).

3. **Sair**

---

## Estrutura do Projeto

```
avl-rb-trees/
├── CMakeLists.txt
├── README.md
├── main.cpp
├── include/
│   ├── AVLNode.h
│   ├── AVLTree.h
│   ├── LLRBNode.h
│   └── LLRBTree.h
└── src/
    ├── AVLNode.cpp
    ├── AVLTree.cpp
    ├── LLRBNode.cpp
    └── LLRBTree.cpp
```

---

## Como Compilar e Executar

### Usando CMake

```bash
cmake --build build
./build/avl_rb_trees
```

### Compilando diretamente via g++ ou clang++

```bash
g++ -std=c++17 -Iinclude main.cpp src/*.cpp -o avl_rb_trees
./avl_rb_trees
```
