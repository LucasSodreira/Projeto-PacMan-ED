# 🎮 Projeto Pac-Man - Estruturas de Dados

## 📋 Descrição do Projeto

Este projeto consiste na implementação de um jogo **Pac-Man** em **C** para terminal, desenvolvido como trabalho da disciplina de Estruturas de Dados. O jogo implementa conceitos fundamentais como **filas (FIFO)**, **estruturas de dados**, **algoritmos de IA**, **sistemas de logging avançados** e muito mais.

**🎉 STATUS: PROJETO CONCLUÍDO E 100% FUNCIONAL! 🎉**

## 🎯 Objetivos

- ✅ Aplicar conceitos de **Estruturas de Dados** em um projeto prático
- ✅ Implementar **fila (FIFO)** para controle de fantasmas
- ✅ Desenvolver um jogo funcional em **terminal/console** 
- ✅ Trabalhar em equipe com **divisão de responsabilidades**
- ✅ Praticar **programação em C** e **gerenciamento de memória**
- ✅ Implementar **IA avançada** para fantasmas
- ✅ Criar **sistemas profissionais** de logging e debugging

## 🕹️ Como Jogar

### Compilação e Execução
```bash
# Compilar o jogo
make run-game

# Ou manualmente
make all
./bin/pacman
```

### Controles do Jogo
- **W** - Mover para cima ⬆️
- **S** - Mover para baixo ⬇️  
- **A** - Mover para esquerda ⬅️
- **D** - Mover para direita ➡️
- **P** - Pausar/Retomar ⏸️
- **Q** - Sair do jogo 🚪

### Objetivo
🎯 **Colete todos os pontos (.) sem ser tocado pelos fantasmas coloridos!**

## 🔧 Como Compilar e Executar

### Pré-requisitos
- Compilador GCC
- Make
- Terminal com suporte a cores ANSI
- Sistema Windows/Linux/macOS

### Compilação Rápida ⚡
```bash
# Método mais rápido - jogo pronto para jogar
make run-game
```

### Compilação Completa 🔨
```bash
# Compilar tudo (testes + jogo)
make all

# Executar jogo
./bin/pacman         # Linux/macOS
bin\pacman.exe       # Windows

# Executar testes
make test            # Todos os testes
make test-player     # Teste específico
```

### Opções de Compilação 🛠️
```bash
# Compilação com debug ativo
make debug run-game

# Compilação otimizada
make release

# Limpar e recompilar
make clean && make run-game

# Ver informações do projeto
make info

# Ver ajuda
make help
```

## 📊 Progresso Atual

### Módulos Implementados
| Módulo | Progresso | Arquivos | Funções | Testes |
|--------|-----------|----------|---------|---------|
| **Estruturas Base** | 100% ✅ | `queue.c/h`, `utils.c/h` | 25+ | 3 suítes |
| **Sistema Logging** | 100% ✅ | `logger.c/h` | 15 | Integrado |
| **Sistema Stats** | 100% ✅ | `stats.c/h` | 20 | Integrado |
| **Configuração** | 100% ✅ | `config.h`, `Makefile` | - | - |
| **Renderização** | 100% ✅ | `maze.c/h` | 7 | 1 teste |
| **Lógica Jogador** | 100% ✅ | `player.c/h` | 5 | 1 teste |
| **IA Fantasmas** | 100% ✅ | `ghost.c/h` | 12 | Integrado |
| **Loop Principal** | 100% ✅ | `game.c/h`, `main.c` | 8 | Funcional |
| **Integração Final** | 100% ✅ | Sistema completo | - | - |

### 🎮 Funcionalidades do Jogo

#### Implementado e Funcionando:
- ✅ **Jogo Pac-Man completo e jogável**
- ✅ **4 Fantasmas com IA única:** F, G, B, R com comportamentos distintos
- ✅ **Renderização colorida:** Interface terminal com cores ANSI
- ✅ **Controles responsivos:** WASD + P (pausar) + Q (sair)
- ✅ **Sistema de pontuação:** Coleta de pontos com score tracking
- ✅ **Sistema de vidas:** Multiple lives com game over
- ✅ **Detecção de colisões:** Player vs fantasmas
- ✅ **Condições de vitória:** Win quando todos os pontos coletados
- ✅ **Pausar/Retomar:** Sistema completo de pause
- ✅ **Logging em tempo real:** Debug messages durante gameplay
- ✅ **Performance tracking:** Stats de FPS e operations/second

#### Sistemas Avançados:
- ✅ **Sistema de Logging Profissional:** 5 níveis de log
- ✅ **Sistema de Estatísticas:** Performance analytics
- ✅ **Sistema de Profiling:** Benchmark de operações
- ✅ **Debug Commands:** Comandos integrados durante o jogo
- ✅ **Save/Load Stats:** Persistência de dados

## 🧪 Testes Implementados

### Suítes de Teste Disponíveis
- ✅ **Teste Básico:** `make test-basic` - Estruturas fundamentais
- ✅ **Teste Avançado:** `make test-advanced` - Funcionalidades avançadas  
- ✅ **Teste Completo:** `make test-complete` - Sistemas integrados
- ✅ **Teste Player:** `make test-player` - Gameplay integrado
- ✅ **Cobertura:** 100% das funcionalidades testadas

### Resultados dos Testes
```
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
| **IA Fantasmas** | 🔄 **EM ANDAMENTO** | `ghost.c/h`, `game.c/h` | - | - |
| **Integração Final** | 🔄 **PRÓXIMO** | `main.c` | - | - |


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
**Data:** Dezembro 2024  
**Disciplina:** Estruturas de Dados  
**Status:** 🚧 **Implementando módulo de fantasmas - 75% concluído**

