#include "ghost.h"
#include "utils.h"   // Para Position, Direction, manhattan_distance, get_opposite_direction, etc.
#include "config.h"  // Para MAX_GHOSTS, GHOST_SYMBOLS, SCATTER_TARGETS, SYMBOL_WALL, POINTS_PER_GHOST_EATEN
#include "player.h"  // Para Player struct (necessário para player->score)
#include "logger.h"  // Para LOG_I, LOG_D
#include <stdlib.h>  // Para rand()


// Define símbolos e posições de scatter default.
static const char GHOST_SYMBOLS[MAX_GHOSTS] = {SYMBOL_GHOST_RED, SYMBOL_GHOST_GREEN, SYMBOL_GHOST_BLUE, SYMBOL_GHOST_PINK};
// SCATTER_TARGETS são as posições dos cantos para o modo scatter.
// Ajustar estas posições para ficarem dentro dos limites de MAX_MAP_WIDTH e MAX_MAP_HEIGHT de config.h
static const Position SCATTER_TARGETS[MAX_GHOSTS] = {
    {1, 1},                           // Canto superior esquerdo (exemplo)
    {MAX_MAP_WIDTH - 2, 1},           // Canto superior direito (exemplo)
    {1, MAX_MAP_HEIGHT - 2},          // Canto inferior esquerdo (exemplo)
    {MAX_MAP_WIDTH - 2, MAX_MAP_HEIGHT - 2} // Canto inferior direito (exemplo)
};

void init_ghosts(Ghost ghosts[], int count) {
    for (int i = 0; i < count; i++) {
        ghosts[i].ghost_id = i;
        ghosts[i].symbol = GHOST_SYMBOLS[i % MAX_GHOSTS];
        ghosts[i].state = GHOST_NORMAL;
        ghosts[i].is_active = false; // Começam inativos; main.c ativa os do mapa.
        ghosts[i].difficulty = DIFFICULTY_MEDIUM; // Dificuldade padrão
        ghosts[i].scatter_mode = true; // Começam em modo scatter
        ghosts[i].timer = 0;
        ghosts[i].path_start = 0;
        ghosts[i].path_end = 0;
        
        // Posições iniciais e initial_pos são definidas em main.c ao carregar o mapa.
        // Define uma posição inicial segura padrão para initial_pos caso não seja sobrescrita.
        ghosts[i].pos = (Position){1,1};
        ghosts[i].initial_pos = ghosts[i].pos; // Garante que initial_pos tenha um valor padrão
        ghosts[i].direction = NORTH; // Direção inicial padrão
        
        ghosts[i].target = SCATTER_TARGETS[i % MAX_GHOSTS]; // Alvo inicial para scatter
        
        // O Log de criação efetiva com posição é feito em main.c após carregar do mapa.
        // LOG_I("Fantasma %d (%c) template criado.", i, ghosts[i].symbol);
    }
}

// Esta é a versão que usa Maze* (anteriormente calculate_next_direction_maze)
// A versão antiga que usava const char* maze foi removida.
Direction calculate_next_direction(const Ghost* ghost, Position pacman_pos, const Maze* maze_data) {
    if (!maze_data) {
        LOG_E("calculate_next_direction chamado com maze_data NULL para ghost %d", ghost->ghost_id);
        return ghost->direction; // Retorna a direção atual como fallback seguro
    }
    if (!ghost) {
        LOG_E("calculate_next_direction chamado com ghost NULL");
        return DIR_INVALID; // Ou alguma direção padrão/inválida
    }

    Direction best_dir = ghost->direction;
    long min_distance = -1; // Usar long e -1 para indicar que nenhuma direção válida foi encontrada ainda.

    Position target = calculate_target_position(ghost, pacman_pos); // Alvo depende do estado do fantasma
    
    for (Direction dir = NORTH; dir <= WEST; dir++) { // Itera sobre todas as direções válidas
        if (!is_valid_direction(dir)) continue;

        Position next_pos = get_next_position(ghost->pos, dir);
        
        // Fantasmas comidos podem ignorar a regra de inversão de 180 graus para voltar para casa.
        bool allow_reverse = (ghost->state == GHOST_EATEN || ghost->state == GHOST_FRIGHTENED);
        if (!is_valid_move_ghost(next_pos, maze_data) ||
            (!allow_reverse && dir == get_opposite_direction(ghost->direction))) {
            continue;
        }
        
        long distance; // Usar long para consistência com min_distance
        if (ghost->state == GHOST_FRIGHTENED) {
            // Tenta se afastar do Pacman (maximizar distância)
            distance = -manhattan_distance(next_pos, pacman_pos);
            distance += (rand() % 50) - 25; // Adiciona aleatoriedade
        } else {
            // Persegue o alvo (que pode ser pacman_pos, scatter_target, ou initial_pos para GHOST_EATEN)
            distance = manhattan_distance(next_pos, target);
        }
        
        if (min_distance == -1 || distance < min_distance) {
            min_distance = distance;
            best_dir = dir;
        }
    }
    
    if (min_distance == -1) { // Se nenhuma direção válida foi encontrada
        Position next_pos_current_dir = get_next_position(ghost->pos, ghost->direction);
        if (is_valid_move_ghost(next_pos_current_dir, maze_data)) {
            return ghost->direction; // Mantém direção atual se válida
        }
        return DIR_INVALID; // Indica que não pode se mover
    }
    return best_dir;
}

Position calculate_target_position(const Ghost* ghost, const Position pacman_pos) {
    // Se estiver comido, o alvo é a sua posição inicial (casa dos fantasmas)
    if (ghost->state == GHOST_EATEN) {
        return ghost->initial_pos;
    }
    // Se assustado, o "alvo" é irrelevante para perseguição direta,
    // pois a lógica de movimento em calculate_next_direction é de fuga.
    // Retornar pacman_pos aqui é um placeholder.
    if (ghost->state == GHOST_FRIGHTENED) {
        return pacman_pos;
    }

    // Em modo scatter (e não assustado/comido), o alvo é o canto designado.
    if (ghost->scatter_mode) {
        return SCATTER_TARGETS[ghost->ghost_id % MAX_GHOSTS];
    }
    
    // Em modo de perseguição (CHASE)
    Position target = pacman_pos;
    
    switch (ghost->ghost_id % MAX_GHOSTS) {
        case 0: // Red ghost (Blinky) - Perseguidor direto
            if (ghost->difficulty == DIFFICULTY_EASY) { // Menos preciso se fácil
                target.x += (rand() % 5) - 2;
                target.y += (rand() % 5) - 2;
            }
            // Em HARD ou MEDIUM, persegue diretamente (target = pacman_pos já definido)
            break;
            
        case 1: // Green ghost (substituindo Pinky) - Perseguidor com variabilidade
            if (ghost->difficulty == DIFFICULTY_HARD) {
                target = pacman_pos; // Persegue diretamente
            } else if (ghost->difficulty == DIFFICULTY_MEDIUM) {
                target.x = pacman_pos.x + (rand() % 5) - 2;
                target.y = pacman_pos.y + (rand() % 5) - 2;
            } else { // DIFFICULTY_EASY
                target.x = pacman_pos.x + (rand() % 9) - 4;
                target.y = pacman_pos.y + (rand() % 9) - 4;
            }
            break;
            
        case 2: // Blue ghost (Inky) - Lógica de flanco simplificada ou perseguição
             if (ghost->difficulty == DIFFICULTY_EASY) {
                target = SCATTER_TARGETS[ghost->ghost_id % MAX_GHOSTS]; // Vai para o canto se fácil
            } else { // Senão, persegue diretamente (lógica de flanco mais complexa removida para simplificar)
                target = pacman_pos;
            }
            break;
            
        case 3: // Pink/Orange ghost (Clyde) - Persegue se perto, scatter se longe
            if (manhattan_distance(ghost->pos, pacman_pos) > 8) { // Se longe
                target = SCATTER_TARGETS[ghost->ghost_id % MAX_GHOSTS];
            }
            // Se perto, persegue diretamente (target = pacman_pos já definido)
            break;
    }
    return target;
}



// Esta é a versão que usa Maze* (anteriormente is_valid_move_ghost_maze)
// A versão antiga que usava const char* maze foi removida.
bool is_valid_move_ghost(Position pos, const Maze* maze_data) {
    if (!maze_data) return false;
    if (pos.x < 0 || pos.x >= maze_data->width || pos.y < 0 || pos.y >= maze_data->height) {
        return false; // Fora dos limites
    }

    return maze_data->grid[pos.y][pos.x] != SYMBOL_WALL;
}

void update_ghost_state(Ghost* ghost, int timer_value) { // Renomeado current_time para timer_value para clareza
    (void)timer_value; // Marca como usado para evitar warning
    
    // Lógica de Scatter/Chase
    if (ghost->state == GHOST_NORMAL || ghost->state == GHOST_FRIGHTENED) {
        // A cada SCATTER_CHASE_INTERVAL ticks, alterna o modo.
        if (ghost->timer % SCATTER_CHASE_INTERVAL == 0 && ghost->timer > 0) { // ghost->timer > 0 para não alternar no tick 0
            ghost->scatter_mode = !ghost->scatter_mode;
            LOG_D("Fantasma %d (%c) modo: %s.", ghost->ghost_id, ghost->symbol, ghost->scatter_mode ? "SCATTER" : "CHASE");
        }
    }

    if (ghost->state == GHOST_FRIGHTENED) {
        if (ghost->timer > FRIGHTENED_MODE_DURATION) {
            ghost->state = GHOST_NORMAL;
            ghost->timer = 0;
            LOG_D("Fantasma %d (%c) não está mais FRIGHTENED.", ghost->ghost_id, ghost->symbol);
        }
    } else if (ghost->state == GHOST_EATEN) {

    }
}

void set_ghost_difficulty(Ghost* ghost, DifficultyLevel difficulty) {
    if (!ghost) return;
    ghost->difficulty = difficulty;
    LOG_D("Fantasma %d: dificuldade alterada para %d", ghost->ghost_id, difficulty);
}

void reset_ghost(Ghost* ghost) {
    if (!ghost) return;
    
    ghost->state = GHOST_NORMAL;
    ghost->scatter_mode = true; // Padrão para scatter ao resetar
    ghost->timer = 0;
    ghost->is_active = true; // Torna o fantasma ativo ao resetar
    ghost->path_start = 0;
    ghost->path_end = 0;
    ghost->pos = ghost->initial_pos; // Reseta para sua posição inicial específica
    ghost->direction = NORTH;        // Direção padrão ao resetar
    LOG_D("Fantasma %d (%c) resetado para pos (%d,%d), estado NORMAL.", ghost->ghost_id, ghost->symbol, ghost->pos.x, ghost->pos.y);
}

// Assinatura atualizada para incluir Player* player
bool check_collision_with_pacman(Player* player, Ghost ghosts[], int count, Position pacman_pos) {
    for (int i = 0; i < count; i++) {
        // Só considera colisão com fantasmas ativos e não comidos.
        // Fantasmas no estado GHOST_EATEN (is_active=false) não causam colisão.
        if (!ghosts[i].is_active || ghosts[i].state == GHOST_EATEN) continue;

        if (positions_equal(ghosts[i].pos, pacman_pos)) {
            if (ghosts[i].state == GHOST_FRIGHTENED) {
                // Fantasma foi comido
                ghosts[i].state = GHOST_EATEN;
                ghosts[i].is_active = false; // Fica inativo até chegar na base e ser resetado
                ghosts[i].timer = 0;

                if (player) {
                    player->score += POINTS_PER_GHOST_EATEN; // POINTS_PER_GHOST_EATEN de config.h
                    LOG_I("Fantasma %d (%c) comido! +%d pontos. Score: %d",
                          ghosts[i].ghost_id, ghosts[i].symbol, POINTS_PER_GHOST_EATEN, player->score);
                } else {
                    LOG_W("Player é NULL em check_collision_with_pacman ao dar score.");
                }
                return false; // Não é colisão fatal
            }
            // Colisão com fantasma normal ou scatter
            LOG_I("Colisão fatal! Pacman (%d,%d) vs Fantasma %d (%c) (%d,%d)",
                  pacman_pos.x, pacman_pos.y, ghosts[i].ghost_id, ghosts[i].symbol, ghosts[i].pos.x, ghosts[i].pos.y);
            return true; // Colisão fatal
        }
    }
    return false; // Sem colisão
}

// ===== IMPLEMENTAÇÃO DA FILA DE CAMINHO INTERNA =====

bool ghost_path_enqueue(Ghost* ghost, Position pos) {
    if (!ghost || ghost_path_is_full(ghost)) {
        return false;
    }
    
    ghost->path[ghost->path_end] = pos;
    ghost->path_end = (ghost->path_end + 1) % MAX_PATH_LENGTH;
    return true;
}

Position ghost_path_dequeue(Ghost* ghost) {
    Position invalid_pos = {-1, -1};
    
    if (!ghost || ghost_path_is_empty(ghost)) {
        return invalid_pos;
    }
    
    Position pos = ghost->path[ghost->path_start];
    ghost->path_start = (ghost->path_start + 1) % MAX_PATH_LENGTH;
    return pos;
}

bool ghost_path_is_empty(const Ghost* ghost) {
    return ghost && (ghost->path_start == ghost->path_end);
}

bool ghost_path_is_full(const Ghost* ghost) {
    if (!ghost) return true;
    return ((ghost->path_end + 1) % MAX_PATH_LENGTH) == ghost->path_start;
}

void ghost_path_clear(Ghost* ghost) {
    if (ghost) {
        ghost->path_start = 0;
        ghost->path_end = 0;
    }
}

int ghost_path_size(const Ghost* ghost) {
    if (!ghost) return 0;
    
    if (ghost->path_end >= ghost->path_start) {
        return ghost->path_end - ghost->path_start;
    } else {
        return (MAX_PATH_LENGTH - ghost->path_start) + ghost->path_end;
    }
}

Position ghost_path_peek(const Ghost* ghost) {
    Position invalid_pos = {-1, -1};
    
    if (!ghost || ghost_path_is_empty(ghost)) {
        return invalid_pos;
    }
    
    return ghost->path[ghost->path_start];
}

// ===== EXEMPLO DE USO DA FILA DE CAMINHO =====

void ghost_plan_path_to_target(Ghost* ghost, Position target, const Maze* maze_data) {
    if (!ghost || !maze_data) return;
    
    // Limpa o caminho atual
    ghost_path_clear(ghost);
    
    // Algoritmo simples de pathfinding (pode ser melhorado)
    Position current = ghost->pos;
    int max_steps = 10; // Evita loops infinitos
    
    while (max_steps > 0 && !positions_equal(current, target)) {
        Direction best_dir = NORTH;
        int best_distance = manhattan_distance(current, target);
        
        // Testa todas as direções
        Direction directions[] = {NORTH, EAST, SOUTH, WEST};
        for (int i = 0; i < 4; i++) {
            Position next_pos = get_next_position(current, directions[i]);
            
            if (is_valid_move_ghost(next_pos, maze_data)) {
                int distance = manhattan_distance(next_pos, target);
                if (distance < best_distance) {
                    best_distance = distance;
                    best_dir = directions[i];
                }
            }
        }
        
        // Adiciona a próxima posição ao caminho
        Position next_pos = get_next_position(current, best_dir);
        if (!ghost_path_enqueue(ghost, next_pos)) {
            break; // Fila cheia
        }
        
        current = next_pos;
        max_steps--;
    }
}

bool ghost_follow_path(Ghost* ghost, const Maze* maze_data) {
    if (!ghost || !maze_data || ghost_path_is_empty(ghost)) {
        return false;
    }
    
    Position next_pos = ghost_path_dequeue(ghost);
    
    // Verifica se a posição ainda é válida
    if (is_valid_move_ghost(next_pos, maze_data)) {
        ghost->pos = next_pos;
        return true;
    }
    
    // Se a posição não é mais válida, limpa o caminho
    ghost_path_clear(ghost);
    return false;
}