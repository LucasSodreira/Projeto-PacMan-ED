# 📖 Manual do Usuário - Pac-Man Terminal

## 🎮 Introdução

Bem-vindo ao **Pac-Man Terminal**! Esta é uma implementação do clássico jogo Pac-Man, desenvolvido em linguagem C para ser executado diretamente no terminal. O jogo é funcional e passou por refatorações para melhorar sua estrutura e jogabilidade, incluindo o uso de uma Fila para gerenciar a movimentação dos fantasmas.

## 🚀 Como Começar

### Compilação e Execução Rápida
A forma mais simples de compilar e executar o jogo é usando o Makefile fornecido:
```bash
make run-game
```
Isso deve compilar o código e iniciar o jogo. Alternativamente, use `make all` para compilar e depois execute `./bin/pacman` (ou `bin\pacman.exe` no Windows).

### Primeiro Jogo
1.  **Abra seu terminal.**
2.  **Navegue até o diretório do projeto.**
3.  **Execute `make run-game`.**
4.  **Pressione ENTER** na tela de título para começar.
5.  **Use as teclas WASD** para mover o Pac-Man (símbolo 'P').
6.  **O objetivo é coletar todos os pontos e power pellets!**

## 🎯 Objetivo do Jogo

**Meta Principal:** Controle o Pac-Man para coletar todos os pontos (`.`) e power pellets (`O`) espalhados pelo labirinto, evitando ser capturado pelos fantasmas.

### Como Vencer um Nível 🏆
- Colete **TODOS** os pontos e power pellets do mapa.
- Ao limpar o nível, você avançará para o próximo (se houver).

### Como Perder 💀
- Ser tocado por um fantasma quando ele não está no estado "Assustado".
- Perder todas as suas vidas.

## 🕹️ Controles

### Movimentação Principal
| Tecla | Ação             |
|-------|------------------|
| **W** | Mover para Cima  |
| **S** | Mover para Baixo |
| **A** | Mover para Esquerda|
| **D** | Mover para Direita|

(Nota: As teclas de seta do teclado podem não ser universalmente suportadas em todos os terminais. WASD são os controles primários definidos em `config.h`.)

### Comandos do Jogo
| Tecla | Ação          |
|-------|---------------|
| **P** | Pausar/Retomar|
| **Q** | Sair do Jogo  |

(As teclas exatas são definidas em `config.h` como `KEY_PAUSE` e `KEY_QUIT`.)

## 🗺️ Elementos do Jogo

### Símbolos no Mapa
| Símbolo | Elemento             | Descrição                                                                 |
|---------|----------------------|---------------------------------------------------------------------------|
| **P**   | Pac-Man              | O personagem que você controla.                                             |
| **F,G,B,R** | Fantasmas (Normal)   | Inimigos que perseguem o Pac-Man. Cada um (`SYMBOL_GHOST_RED`, etc.) pode ter uma estratégia básica. |
| **A**   | Fantasma Assustado   | (`SYMBOL_GHOST_FRIGHTENED`) Após Pac-Man comer um Power Pellet. Vulnerável. |
| **e**   | Fantasma Comido      | (`SYMBOL_GHOST_EATEN`) Olhos do fantasma retornando à base para regenerar.  |
| **#**   | Parede (`SYMBOL_WALL`) | Obstáculo intransponível.                                                 |
| **.**   | Ponto (`SYMBOL_DOT`)   | Colete para ganhar pontos.                                                |
| **O**   | Power Pellet (`SYMBOL_POWER_PELLET`)| Permite ao Pac-Man comer fantasmas e ganhar mais pontos.   |
| (espaço)| Caminho Livre (`SYMBOL_EMPTY_SPACE`)| Área onde Pac-Man e fantasmas podem se mover.                |

### Interface do Jogo (HUD)
Durante o jogo, informações importantes são exibidas na tela, geralmente na parte superior ou inferior, incluindo:
- **Score:** Sua pontuação atual.
- **Vidas:** Número de vidas restantes.
- **Estado:** Estado atual do jogo (Ex: Jogando, Pausado), convertido para texto.
- **Nível:** O nível atual em que você está (exibido em `main.c`).

## 🎲 Como Jogar Detalhado

1.  **Iniciando:** Após compilar e executar, uma tela de título será exibida. Pressione Enter para começar.
2.  **Movimentação:** Use as teclas WASD para guiar o Pac-Man pelo labirinto. O Pac-Man continuará se movendo na direção selecionada até encontrar uma parede ou uma nova direção ser inserida.
3.  **Coletando Pontos:** Passe sobre os `.` (pontos) e `O` (power pellets) para coletá-los. Todos devem ser coletados para passar de nível.
4.  **Fantasmas:**
    *   Evite contato com os fantasmas (F, G, B, R) quando estão em seu estado normal/perseguição.
    *   Coma um Power Pellet (`O`) para mudar o estado dos fantasmas para "Assustado" (símbolo 'A', geralmente cor azul/ciano). Neste estado, você pode tocar nos fantasmas para "comê-los".
    *   Fantasmas comidos (símbolo 'e', geralmente cor branca) se tornam inativos e retornam à sua base no centro do labirinto para se regenerar, após o qual voltam ao jogo em seu estado normal.
5.  **Progressão:** Limpe todos os pontos de um nível para avançar automaticamente para o próximo. O jogo possui múltiplos níveis que são carregados de arquivos de mapa. Seu score e o número de vidas são mantidos entre os níveis.
6.  **Fim de Jogo:** O jogo termina se você perder todas as vidas (Game Over) ou completar todos os níveis disponíveis (Vitória Geral!).

## 📊 Sistema de Pontuação

| Ação                       | Pontos (Valor de `config.h`) |
|----------------------------|-------------------------------|
| Coletar um Ponto (`.`)       | `POINTS_PER_DOT` (ex: +10)    |
| Coletar um Power Pellet (`O`)| `POINTS_PER_POWER_PELLET` (ex: +50) |
| Comer um Fantasma Assustado| `POINTS_PER_GHOST_EATEN` (ex: +200) |
| Vida Extra                 | A cada `POINTS_FOR_EXTRA_LIFE` (ex: 1000) |
| Bônus por Nível (Opcional) | `BONUS_LEVEL_COMPLETE` (ex: +100) |

A pontuação é acumulada ao longo dos níveis.

## 👻 Comportamento dos Fantasmas

- **Movimentação:** A ordem em que os fantasmas têm a chance de se mover a cada ciclo do jogo é gerenciada por uma **Fila (FIFO)**. Isso significa que um fantasma é processado por vez.
- **IA Individual:** Cada fantasma (identificado por seu símbolo/ID) possui uma lógica de IA para decidir sua próxima direção, que pode variar dependendo do seu estado (normal, scatter, frightened, eaten) e dificuldade.
- **Estados dos Fantasmas:**
    - **Normal (Perseguição/Chase):** O objetivo principal é perseguir o Pac-Man, usando diferentes táticas dependendo do fantasma.
    - **Dispersão (Scatter):** Periodicamente, os fantasmas podem entrar em modo "scatter", onde se dirigem para seus respectivos cantos do labirinto por um breve período antes de retomar a perseguição.
    - **Assustado (Frightened):** Após o Pac-Man consumir um Power Pellet, os fantasmas ficam temporariamente vulneráveis (símbolo 'A', cor diferente) e tentam fugir do Pac-Man. Se tocados neste estado, são "comidos".
    - **Comido (Eaten):** Após serem comidos, os fantasmas (símbolo 'e', cor diferente) se tornam inofensivos e se dirigem à sua posição inicial (base) para serem reativados e voltarem ao estado Normal.

## 🛠️ Configurações e Logging

- **Configurações do Jogo:** Muitas constantes do jogo, como pontuações (`POINTS_PER_DOT`, `POINTS_PER_GHOST_EATEN`), número de vidas (`DEFAULT_LIVES`), velocidade (`GAME_SPEED_MS`), e o número máximo de níveis (`MAX_LEVELS`), podem ser encontradas e ajustadas no arquivo `src/config.h` por usuários com conhecimento técnico.
- **Logging:** O jogo gera um arquivo de log chamado `game.log` que registra eventos importantes, avisos e erros. Isso pode ser útil para depuração ou para entender o comportamento do jogo em retrospecto. O nível de detalhe do log (DEBUG, INFO, etc.) é definido em `logger.c`.
- **Comandos de Debug (se ativos):** Durante o jogo, podem existir teclas de debug (como D, S, L) que, se implementadas e ativas, podem exibir informações internas sobre o estado do jogo ou do logger.

## 🏆 Boa Sorte!

Divirta-se jogando Pac-Man Terminal e tente completar todos os níveis!
