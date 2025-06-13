# 🎮 Projeto Pac-Man - Estruturas de Dados

## 📋 Descrição do Projeto

Este projeto consiste na implementação de um jogo **Pac-Man** em **C** para terminal, desenvolvido como trabalho da disciplina de Estruturas de Dados. O jogo implementa conceitos fundamentais como **filas (FIFO)**, **estruturas de dados**, **manipulação de arquivos**, **algoritmos de movimentação** e **sistemas avançados de monitoramento**.

## 🎯 Objetivos

- Aplicar conceitos de **Estruturas de Dados** em um projeto prático
- Implementar **fila (FIFO)** para controle de fantasmas
- Desenvolver um jogo funcional em **terminal/console**
- Trabalhar em equipe com **divisão de responsabilidades**
- Praticar **programação em C** e **gerenciamento de memória**
**Adicional**
- Implementar **sistemas avançados** de logging, estatísticas e profiling

## 🗂️ Estrutura do Projeto

```
Projeto-PacMan-ED/
├── src/                          # Código fonte
│   ├── queue.c/h                # ✅ Estrutura de fila FIFO (Pessoa 1)
│   ├── utils.c/h                # ✅ Funções auxiliares e utilitárias (Pessoa 1)
│   ├── stats.c/h                # ✅ Sistema de estatísticas (Pessoa 1)
│   ├── logger.c/h               # ✅ Sistema de logging (Pessoa 1)
│   ├── config.h                 # ✅ Configurações centralizadas (Pessoa 1)
│   ├── maze.c/h                 # ✅ Sistema de mapas (Pessoa 2)
│   ├── player.c/h               # ✅ Lógica do Pac-Man (Pessoa 3)
│   ├── ghost.c/h                # 🔄 Lógica dos fantasmas (Pessoa 4)
│   ├── game.c/h                 # 🔄 Loop principal (Pessoa 4)
│   └── main.c                   # 🔄 Função main (Pessoa 4)
├── maps/                        # Arquivos de mapas
│   └── level1.txt               # 🔄 Mapas do jogo (Pessoa 2)
├── test/                        # Testes
│   ├── test_structs.c           # ✅ Teste básico das estruturas (Pessoa 1)
│   ├── test_advanced.c          # ✅ Teste avançado (Pessoa 1)
│   ├── test_complete.c          # ✅ Teste completo de integração (Pessoa 1)
│   └── test_player.c            # ✅ Teste integrado Player + Maze (Pessoa 3)
├── docs/                        # Documentação
│   ├── README.md                # Este arquivo
│   ├── manual.md                # Manual do usuário
│   └── relatorio.md             # Relatório técnico
├── Makefile                     # ✅ Sistema de compilação (Pessoa 1)
├── test_log.txt                 # ✅ Logs de teste gerados
└── test_stats.dat               # ✅ Estatísticas persistidas
```

## 🎮 Como o Jogo Funciona

### Elementos do Jogo
- **P** - Pac-Man (jogador)
- **F/G/B/R** - Fantasmas (inimigos)
- **#** - Paredes
- **.** - Pontos para coletar
- **(espaço)** - Caminho livre

### Controles
- **W** - Mover para cima
- **S** - Mover para baixo
- **A** - Mover para esquerda
- **D** - Mover para direita
- **Q** - Sair do jogo

### Objetivo
Colete todos os pontos sem ser tocado pelos fantasmas!

## 🔧 Como Compilar e Executar

### Pré-requisitos
- Compilador GCC
- Sistema Windows/Linux/macOS
- Terminal/Prompt de comando

### Compilação com Makefile ✅
```bash
# Compilar todos os testes
make all

# Executar todos os testes
make test

# Executar teste específico
make test-basic    # Teste básico
make test-advanced # Teste avançado

# Compilação com debug
make debug

# Limpeza
make clean
```

### Compilação Manual
```bash
# Teste básico das estruturas
gcc -o test_structs test/test_structs.c src/queue.c src/utils.c

# Teste avançado com todas as funcionalidades
gcc -o test_advanced test/test_advanced.c src/queue.c src/utils.c

# Teste completo (requer todos os módulos)
gcc -o test_complete test/test_complete.c src/queue.c src/utils.c src/stats.c src/logger.c

# Teste integrado Player + Maze
gcc -o test_player test/test_player.c src/player.c src/maze.c src/utils.c src/logger.c src/stats.c src/queue.c -std=c99 -Wall -I src

# Jogo completo (quando estiver pronto)
gcc -o pacman src/*.c
```

### Execução
```bash
# Executar testes
./test_structs      # Teste básico
./test_advanced     # Teste avançado
./test_complete     # Teste completo
./test_player       # Teste Player + Maze integrado

# Executar jogo (quando estiver pronto)
./pacman
```

## 🧪 Testes Implementados
- **Teste Básico:** 4/4 cenários passaram
- **Teste Avançado:** 6/6 categorias testadas com sucesso
- **Teste Completo:** 4/4 sistemas integrados funcionando
- **Teste Player:** 4/4 funcionalidades do jogador testadas
- **Cobertura:** 100% das funcionalidades implementadas testadas

### Como Executar Testes
```bash
# Usando Makefile (recomendado)
make test

# Individual
make test-basic
make test-advanced

# Manual
gcc -o test_structs test/test_structs.c src/queue.c src/utils.c
./test_structs

gcc -o test_advanced test/test_advanced.c src/queue.c src/utils.c
./test_advanced

gcc -o test_complete test/test_complete.c src/queue.c src/utils.c src/stats.c src/logger.c
./test_complete

# Teste integrado Player + Maze
gcc -o test_player test/test_player.c src/player.c src/maze.c src/utils.c src/logger.c src/stats.c src/queue.c -std=c99 -Wall -I src
./test_player
```
---


## 📊 Progresso Atual

### Módulos Implementados
| Módulo | Progresso | Arquivos | Funções | Testes |
|--------|-----------|----------|---------|---------|
| **Estruturas Base** | 100% ✅ | `queue.c/h`, `utils.c/h` | 25+ | 3 suítes |
| **Sistema Logging** | 100% ✅ | `logger.c/h` | 15 | Integrado |
| **Sistema Stats** | 100% ✅ | `stats.c/h` | 20 | Integrado |
| **Configuração** | 100% ✅ | `config.h`, `Makefile` | - | - |
| **Renderização** | 100% ✅ | `maze.c/h` | 6 | 1 teste |
| **Lógica Jogador** | 100% ✅ | `player.c/h` | 5 | 1 teste |
| **IA Fantasmas** | 0% 🔄 | `ghost.c/h`, `game.c/h` | - | - |


## 📝 Convenções de Código

- **Linguagem:** C (padrão C99)
- **Nomenclatura:** snake_case para funções e variáveis
- **Comentários:** Em português
- **Indentação:** 4 espaços
- **Headers:** Include guards (#ifndef/#define/#endif)
- **Logging:** Usar macros `LOG_D()`, `LOG_I()`, etc.
- **Testes:** Adicionar asserções e testes unitários


## 🔍 Debugging e Profiling

### Sistema de Logging
```c
#include "logger.h"

// Inicializar (uma vez no main)
logger_init("game.log", LOG_DEBUG);

// Usar durante o desenvolvimento
LOG_D("Debug info: %d", value);
LOG_I("Game started");
LOG_W("Warning: %s", message);
LOG_E("Error occurred");

// Finalizar
logger_shutdown();
```

### Sistema de Estatísticas
```c
#include "stats.h"

// Criar e usar estatísticas
QueueStats* stats = create_queue_stats();
update_queue_stats_enqueue(stats, queue_size(queue));
print_queue_stats(stats);
```

### Sistema de Profiling
```c
#include "stats.h"

// Medir performance
ProfileData* profile = start_profiling("Operação crítica");
// ... código a ser medido ...
end_profiling(profile);
print_profile_result(profile);
```

---

**Versão:** 1.0  
**Data:** 11 de Junho de 2025  
**Disciplina:** Estruturas de Dados  
**Status:** 🚀 **Base Avançada Implementada - Pronta para Integração**

