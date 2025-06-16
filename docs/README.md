# 🎮 Projeto Pac-Man em C para Terminal

## 📋 Descrição do Projeto

Este projeto é uma implementação do clássico jogo Pac-Man, desenvolvido em linguagem C para ser executado no terminal. O desenvolvimento focou na aplicação de conceitos de estruturas de dados, incluindo o uso de uma **Fila (FIFO)** para o gerenciamento e movimentação dos fantasmas, um requisito chave do projeto. O jogo passou por refatorações significativas para melhorar a organização do código, a lógica de jogo e a robustez.

**Status:** Projeto funcional com mecânicas principais implementadas.

## 🎯 Objetivos Originais (Exemplos)

- Aplicar conceitos de Estruturas de Dados em um projeto prático.
- Implementar Fila (FIFO) para controle de entidades (fantasmas).
- Desenvolver um jogo funcional em terminal/console.
- Praticar programação em C e gerenciamento de memória.
- Implementar IA básica para os fantasmas.

## 🕹️ Como Jogar

### Compilação e Execução

Recomenda-se compilar usando o Makefile fornecido:
```bash
# Compilar e executar o jogo (opção preferida)
make run-game

# Alternativamente, para compilar tudo (incluindo possíveis testes):
make all
# E então executar:
./bin/pacman
```
(No Windows, o executável pode estar em `bin\pacman.exe`)

### Controles do Jogo
- **W** ou **Seta para Cima** - Mover para cima ⬆️
- **S** ou **Seta para Baixo** - Mover para baixo ⬇️
- **A** ou **Seta para Esquerda** - Mover para esquerda ⬅️
- **D** ou **Seta para Direita** - Mover para direita ➡️
- **P** - Pausar/Retomar o jogo ⏸️
- **Q** - Sair do jogo 🚪

(Nota: As teclas de seta podem depender da configuração do terminal. WASD são os controles primários.)

### Objetivo
O objetivo é controlar o Pac-Man para coletar todos os pontos (`.`) e power pellets (`O`) em cada nível, evitando ser capturado pelos fantasmas. Comer um power pellet deixa os fantasmas temporariamente vulneráveis, permitindo que o Pac-Man os coma para ganhar pontos extras.

## ✨ Funcionalidades Implementadas

- Jogo Pac-Man jogável em interface de terminal.
- Movimentação do jogador controlada pelo usuário (WASD).
- Coleta de pontos e power pellets, com atualização de score em tempo real.
- Múltiplos níveis carregados a partir de arquivos de mapa (localizados na pasta `maps/`).
- Fantasmas com movimentação gerenciada por uma Fila (FIFO), processando um fantasma por ciclo de jogo para permitir diferentes velocidades relativas.
- Inteligência Artificial (IA) para os fantasmas com diferentes comportamentos:
    - Perseguição direta ao Pac-Man.
    - Comportamento de "scatter" para dispersar pelos cantos do mapa.
    - Estado "Frightened" (assustado) após o Pac-Man consumir um power pellet, durante o qual podem ser comidos.
    - Estado "Eaten" (comido), no qual o fantasma retorna à sua base para ser reativado.
- Sistema de pontuação, incluindo pontos por coleta de itens e por comer fantasmas assustados.
- Sistema de vidas para o jogador, com tela de "Game Over" ao esgotarem.
- Condição de vitória ao limpar todos os pontos de um nível, com progressão para o próximo nível.
- Funcionalidade de pausar (`P`) e sair (`Q`) do jogo a qualquer momento.
- Interface de terminal com uso de cores ANSI para distinguir elementos do jogo (jogador, fantasmas, paredes, pontos).
- Logging básico de eventos importantes do jogo e erros em um arquivo (`game.log`), configurável através de `logger.c/h`.

## 🛠️ Estrutura do Projeto (Principais Módulos)

O projeto é organizado nos seguintes módulos principais:
- `main.c`: Contém o loop principal do jogo (`game_loop`) e a função `main`, gerenciando a inicialização geral e a progressão de níveis.
- `game.c/h`: Orquestra as funcionalidades centrais do jogo, como atualização do estado do jogo (`update_game`), processamento de input (`process_player_input`), e renderização da tela (`draw_game`).
- `player.c/h`: Define a estrutura `Player` e gerencia a lógica relacionada ao jogador, incluindo movimento (`player_move`), inicialização (`player_init`), e gerenciamento de vidas (`player_lose_life`).
- `ghost.c/h`: Define a estrutura `Ghost` e implementa a IA dos fantasmas, incluindo cálculo de direção (`calculate_next_direction`), definição de alvos (`calculate_target_position`), e gerenciamento de estados (`update_ghost_state`, `reset_ghost`).
- `maze.c/h`: Define a estrutura `Maze` e lida com o carregamento de mapas de arquivos (`load_maze`), inicialização do labirinto (`maze_init`), e verificação de colisões com paredes ou coleta de pontos.
- `queue.c/h`: Implementação da estrutura de dados Fila genérica, utilizada para gerenciar a ordem de movimento dos fantasmas.
- `utils.c/h`: Fornece funções utilitárias diversas, como manipulação de posições e direções, conversão de status para string, e outras ferramentas auxiliares.
- `config.h`: Arquivo central de configuração, contendo constantes globais como símbolos do jogo, cores, pontuações, configurações de dificuldade (se aplicável), e parâmetros de jogabilidade.
- `logger.c/h`: Sistema de logging simples para registrar mensagens de debug, informação, avisos e erros em um arquivo.
- `stats.c/h`: Estruturas e funções relacionadas a estatísticas de jogo e profiling de performance (uso atual pode variar).

## 🧪 Testes

O projeto original incluía uma pasta `test` com arquivos como `test_structs.c`, indicando uma estrutura para testes unitários e de integração. Durante as fases de desenvolvimento e refatoração, foram realizados testes manuais e revisões de código para assegurar as funcionalidades principais. Para garantir maior robustez e cobertura, a suíte de testes pode ser expandida e atualizada para refletir o estado atual do código.

## 📝 Convenções de Código e Ferramentas

- **Linguagem:** C (compilado com padrão C99 ou similar).
- **Nomenclatura:** Predominantemente snake_case para funções e variáveis.
- **Comentários:** Em português, com o objetivo de explicar lógicas importantes e decisões de design.
- **Logging:** O sistema de logging (`logger.c/h`) é utilizado para registrar informações de debug (`LOG_D`), eventos de jogo (`LOG_I`), avisos (`LOG_W`) e erros (`LOG_E`) em um arquivo (padrão: `game.log`). O nível de logging pode ser ajustado.
- **Estatísticas/Profiling:** O código contém referências a um sistema de estatísticas e profiling (`stats.c/h`). Se estas funcionalidades forem mantidas e atualizadas, podem ser usadas para análise de performance do jogo.

---
**Versão:** 2.0 (Pós-Refatoração Significativa)
**Disciplina:** Estruturas de Dados (Contexto Original)
```
