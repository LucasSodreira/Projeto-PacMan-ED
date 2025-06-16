# 📊 Relatório Técnico - Projeto Pac-Man em C

## 📋 Informações do Projeto

- **Nome:** Pac-Man Terminal em C
- **Disciplina:** Estruturas de Dados (Contexto Original do Projeto)
- **Linguagem:** C (Padrão C99)
- **Status Atual:** Funcional após refatorações significativas, com mecânicas principais de jogo implementadas e melhorias de robustez.
- **Foco da Refatoração:** Melhorar a modularidade, clareza do código, robustez e implementar funcionalidades chave como carregamento de mapas e progressão de níveis.

## 👥 Equipe de Desenvolvimento (Original)

| Pessoa     | Responsabilidade Original (Exemplo)                                      |
|------------|--------------------------------------------------------------------------|
| Pessoa 1   | Estruturas de Dados (Fila), Sistemas Auxiliares (Logging, Stats, Utils)  |
| Pessoa 2   | Labirinto (Maze) e Renderização                                          |
| Pessoa 3   | Lógica do Pac-Man (Player)                                               |
| Pessoa 4   | IA dos Fantasmas (Ghost) e Integração (Game, Main)                       |

*(Nota: As responsabilidades e o estado detalhado de cada módulo original foram alterados durante o processo de desenvolvimento e subsequentes refatorações.)*

## 🎯 Objetivos Técnicos e Pedagógicos Atingidos (Pós-Refatoração)

### Objetivos Técnicos:
- ✅ **Implementação de Fila (FIFO):** Utilizada para gerenciar a ordem de movimento dos fantasmas.
- ✅ **Sistema de Renderização para Terminal:** Jogo visualmente representado no terminal com cores ANSI.
- ✅ **Controles Responsivos:** Jogador controlado por teclas (WASD).
- ✅ **IA Básica para Fantasmas:** Fantasmas com diferentes estados (perseguição, scatter, assustado, comido) e lógica de movimentação.
- ✅ **Carregamento de Mapas:** Níveis são carregados de arquivos de texto, permitindo flexibilidade.
- ✅ **Múltiplos Níveis:** Implementada a progressão entre níveis, com manutenção de score e vidas.
- ✅ **Gerenciamento de Estado do Jogo:** Lógica para pontuação (incluindo comer fantasmas), vidas, game over, vitória e pausa.
- ✅ **Modularização do Código:** Esforço para separar responsabilidades em diferentes módulos.
- ✅ **Melhorias de Robustez:** Aumento da segurança no tratamento de erros de alocação e manipulação de strings.

### Objetivos Pedagógicos:
- ✅ Aplicação de conceitos de Estruturas de Dados (Fila).
- ✅ Prática de programação em C.
- ✅ Experiência com gerenciamento de memória e prevenção de erros comuns.
- ✅ Desenvolvimento de lógica de jogo e algoritmos de movimentação/IA simples.
- ✅ Organização e refatoração de um projeto de software de médio porte.

## 🏗️ Arquitetura do Sistema e Módulos

O jogo é estruturado em módulos com responsabilidades distintas, promovendo organização e manutenibilidade:

-   **`main.c`**: Ponto de entrada do programa. Contém o loop principal do jogo (`game_loop`), gerencia a progressão entre os níveis, e orquestra a inicialização (chamando `initialize_level`) e finalização dos componentes do jogo.
-   **`game.c/h`**: Responsável pela lógica central de um nível. `update_game` processa colisões, perda de vidas e condições de vitória. `draw_game` renderiza o estado atual do jogo no terminal. `process_player_input` trata a entrada do usuário e delega ações.
-   **`player.c/h`**: Gerencia a estrutura de dados do jogador (`Player`), sua movimentação (`player_move`), inicialização (`player_init`), pontuação e sistema de vidas (`player_lose_life`).
-   **`ghost.c/h`**: Define a estrutura `Ghost`. Implementa a lógica de IA dos fantasmas, incluindo o cálculo da próxima direção (`calculate_next_direction`), a definição de posições alvo (`calculate_target_position`), e o gerenciamento dos diferentes estados dos fantasmas (Normal, Scatter, Frightened, Eaten) através de `update_ghost_state` e `reset_ghost`.
-   **`maze.c/h`**: Define a estrutura `Maze`. Responsável pelo carregamento dos labirintos de arquivos de texto (`load_maze`), inicialização do mapa para um nível (`maze_init`), e fornece funções para verificar colisões com paredes (`maze_is_wall`) e a presença/remoção de pontos (`maze_has_point`, `maze_remove_point`, `maze_count_points`).
-   **`queue.c/h`**: Implementa uma estrutura de dados Fila (FIFO) genérica, utilizada especificamente para gerenciar a ordem em que os fantasmas são processados e se movem a cada ciclo do jogo.
-   **`utils.c/h`**: Contém funções utilitárias diversas usadas em todo o projeto, como manipulação de `Position` e `Direction`, conversão de `GameStatus` para string, e possivelmente funções de entrada do usuário e controle de tempo (`sleep_ms`).
-   **`config.h`**: Arquivo central para constantes globais, como símbolos do jogo, cores ANSI, valores de pontuação, número de vidas, velocidade do jogo, `MAX_LEVELS`, etc. Facilita a configuração e ajuste de parâmetros do jogo.
-   **`logger.c/h`**: Sistema de logging para registrar mensagens de debug, informações, avisos e erros em um arquivo (`game.log`).
-   **`stats.c/h`**: (Estrutura original) Destinado a coletar e exibir estatísticas de jogo e performance. Sua integração e uso podem variar após refatorações.

*(O diagrama Mermaid e a divisão detalhada por "Pessoa" foram removidos para focar na arquitetura funcional atual.)*

## 💾 Estruturas de Dados Implementadas

### `Ghost` (em `ghost.h`)
Representa um fantasma no jogo.
```c
struct Ghost {
    Position pos;          // Posição atual (x,y)
    Direction direction;   // Direção atual de movimento
    int ghost_id;         // Identificador único
    char symbol;          // Caractere que representa o fantasma no mapa (F, G, B, R)
    GhostState state;     // Estado atual (NORMAL, FRIGHTENED, EATEN)
    int is_active;        // Flag indicando se o fantasma está ativo no jogo
    Position target;      // Posição alvo para a IA de movimento
    DifficultyLevel difficulty; // Nível de dificuldade da IA
    int scatter_mode;    // Flag para indicar se está no modo de dispersão
    int timer;          // Temporizador para estados (frightened, eaten) e outros usos
    Position initial_pos; // Posição inicial para onde o fantasma retorna ao ser comido/resetado
};
```

### `Player` (em `player.h` ou `utils.h`)
Representa o jogador (Pac-Man).
```c
typedef struct {
    Position pos;           // Posição atual (x,y)
    int score;             // Pontuação acumulada
    int lives;             // Número de vidas restantes
    char symbol;           // Caractere que representa o jogador (P)
} Player;
```

### `Maze` (em `maze.h`)
Representa o labirinto do jogo.
```c
typedef struct {
    char grid[MAZE_HEIGHT][MAZE_WIDTH]; // Matriz 2D que armazena o layout do labirinto
    int width;                          // Largura efetiva do labirinto (geralmente MAZE_WIDTH)
    int height;                         // Altura efetiva do labirinto (geralmente MAZE_HEIGHT)
    int total_points;                   // Número total de pontos e power pellets no início do nível
    Position player_start_pos_from_map; // Posição inicial do jogador lida do mapa
} Maze;
```

### `Queue` e `QueueNode` (em `queue.h`)
Usada para gerenciar a movimentação dos fantasmas.
```c
typedef struct QueueNode {
    Ghost ghost; // Armazena uma cópia da struct Ghost
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode* front;
    QueueNode* rear;
    int size;
} Queue;
```
A fila garante que cada fantasma ativo seja processado sequencialmente a cada ciclo do jogo.

### Gerenciamento de Estado do Jogo
A struct `GameState` definida em `utils.h` parece ser uma estrutura mais antiga e abrangente. No fluxo de jogo atual implementado em `main.c`, o estado do jogo é gerenciado por variáveis separadas (`Player player`, `Maze maze`, `Ghost ghosts[]`, `GameStatus game_status`, `int current_level`), em vez de uma única struct `GameState` centralizada que contém tudo. A enum `GameStatus` (PLAYING, GAME_OVER, etc.) é usada ativamente para controlar o fluxo principal.

## ✨ Funcionalidades Chave e Decisões de Design (Pós-Refatoração)

-   **Carregamento Dinâmico de Mapas:** Os níveis do jogo são definidos em arquivos de texto (`maps/levelX.txt`). A função `load_maze` em `maze.c` é responsável por ler esses arquivos, interpretar os símbolos (paredes, pontos, power pellets, posições iniciais do jogador e fantasmas) e popular a estrutura `Maze`. Isso permite fácil adição ou modificação de níveis sem recompilar o jogo. Um mapa padrão é usado como fallback se um arquivo de nível não for encontrado.
-   **Movimentação de Fantasmas por Fila:** Para controlar a ordem e a frequência de movimento dos fantasmas, uma Fila (FIFO) foi implementada (`queue.c`). Em cada ciclo do loop principal do jogo, um fantasma é retirado da fila, suas ações são processadas (atualização de estado, cálculo de nova direção, movimento), e então ele é reinserido na fila. Isso permite que os fantasmas se movam em "turnos", facilitando o balanceamento da velocidade deles em relação ao jogador.
-   **Inteligência Artificial (IA) dos Fantasmas:**
    -   **Estados:** Os fantasmas operam em diferentes estados:
        -   `GHOST_NORMAL`: Pode alternar entre modo `CHASE` (perseguir o Pac-Man) e `SCATTER` (ir para um canto específico do labirinto).
        -   `GHOST_FRIGHTENED`: Após o Pac-Man consumir um "Power Pellet", os fantasmas ficam vulneráveis, mudam de aparência e tentam fugir do Pac-Man.
        -   `GHOST_EATEN`: Se comidos no estado "Frightened", os fantasmas se tornam inativos (apenas "olhos") e retornam à sua posição inicial (`initial_pos`) para serem reativados.
    -   **Cálculo de Direção:** A função `calculate_next_direction` determina o próximo movimento com base no estado atual do fantasma e na posição do Pac-Man (ou outros alvos, como cantos ou a base).
-   **Sistema de Pontuação e Vidas:** O jogador ganha pontos ao coletar `SYMBOL_DOT` e `SYMBOL_POWER_PELLET`. Pontos adicionais (`POINTS_PER_GHOST_EATEN`) são concedidos ao comer fantasmas no estado "Frightened". O jogador recebe vidas extras ao atingir limiares de pontuação (`POINTS_FOR_EXTRA_LIFE`). O jogo termina se todas as vidas (`DEFAULT_LIVES`, `MAX_LIVES`) forem perdidas.
-   **Progressão de Múltiplos Níveis:** O jogo suporta múltiplos níveis (`MAX_LEVELS`). Após o jogador limpar todos os pontos de um nível, o `current_level` é incrementado, e um novo mapa é carregado. O score e as vidas do jogador são mantidos entre os níveis. O jogo é vencido ao completar todos os níveis.
-   **Melhorias de Robustez:** Foram implementadas verificações de retorno para funções críticas (como criação de fila e adição de elementos à fila) e `snprintf` foi usado para prevenir buffer overflows na formatação de nomes de arquivo. As funções da Fila também foram revisadas para melhor tratamento de ponteiros nulos.

## 🧪 Testes

Durante o desenvolvimento e o extenso processo de refatoração, foram realizados testes funcionais manuais e revisões de código contínuas para validar a lógica dos módulos principais. Isso incluiu a verificação da movimentação do jogador e dos fantasmas, colisões, sistema de pontuação, transição de estados dos fantasmas, carregamento de mapas e o ciclo de vida do jogo (início, pausa, game over, vitória por nível, vitória final).

O projeto também contém uma estrutura inicial para testes unitários na pasta `test/` (ex: `test_structs.c`), que pode ser expandida para fornecer uma cobertura de testes automatizada mais abrangente e sistemática para os diversos módulos e funções.

## 📈 Análise de Qualidade Pós-Refatoração

-   **Funcionalidade:** O jogo Pac-Man possui suas mecânicas centrais implementadas e é completamente jogável do início ao fim, incluindo a progressão por múltiplos níveis.
-   **Modularidade:** As refatorações resultaram em uma estrutura de código mais clara e organizada, com responsabilidades melhor distribuídas entre os módulos.
-   **Lógica dos Fantasmas:** A IA dos fantasmas, embora não excessivamente complexa, implementa os estados essenciais (Chase, Scatter, Frightened, Eaten) e utiliza uma Fila para gerenciar a ordem de movimento, cumprindo um requisito central do projeto.
-   **Flexibilidade de Níveis:** O sistema de carregamento de mapas a partir de arquivos de texto oferece boa flexibilidade para design e expansão de níveis.
-   **Gerenciamento de Estado:** O estado do jogo, incluindo pontuação, vidas, status atual e progressão de níveis, é gerenciado de forma consistente.
-   **Robustez:** Foram aplicadas melhorias significativas para tratar erros de alocação de memória (especialmente na Fila) e para usar funções mais seguras de manipulação de strings, aumentando a estabilidade geral do programa.

## 📦 Entregáveis (Conforme Estrutura do Projeto)

-   **Código Fonte:** Todos os arquivos `.c` e `.h` localizados na pasta `src/`.
-   **Mapas:** Arquivos de nível de exemplo na pasta `maps/` (necessário criar para testar).
-   **Documentação:** Inclui `docs/README.md` (visão geral do projeto), `docs/manual.md` (guia do usuário) e este `docs/relatorio.md` (detalhes técnicos da implementação).
-   **Makefile:** Para compilação e execução do projeto em ambientes compatíveis.
-   **Testes:** Estrutura de testes básicos em `test/`.

## 🔮 Próximos Passos (Sugestões)

-   **Expandir Suíte de Testes:** Desenvolver mais testes unitários e de integração automatizados para garantir a estabilidade e corretude de todos os módulos.
-   **Refinar IA dos Fantasmas:** Implementar comportamentos mais distintos e estratégicos para cada tipo de fantasma (ex: Pinky mirando à frente do Pac-Man, Inky usando uma lógica de flanco mais complexa que envolva a posição de outro fantasma).
-   **Melhorar Interface do Usuário:** Considerar o uso de bibliotecas como `ncurses` (ou `PDCurses` para Windows) para uma interface de terminal mais rica, com controle de cursor aprimorado e menos "flickering" na renderização.
-   **Adicionar Elementos de Jogo:** Introduzir frutas bônus com pontuações variadas, passagens secretas laterais no labirinto, ou outros power-ups.
-   **Persistência de High Scores:** Implementar um sistema para salvar e carregar as melhores pontuações em arquivo.
-   **Portabilidade de Input:** Melhorar a função `get_user_input` para ser não-bloqueante e mais portável entre diferentes sistemas operacionais e terminais.
-   **Revisão de `stats.c/h`:** Avaliar a utilidade atual do módulo de estatísticas e profiling, integrando-o melhor ou removendo-o se não for essencial.

---
*(Este relatório foi atualizado para refletir o estado do projeto após um significativo processo de refatoração.)*