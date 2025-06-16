#include "ghost.h"
#include "utils.h"
#include "logger.h"
#include "maze.h"
#include <stdlib.h>
#include <limits.h>


// ===== CONSTANTES PRIVADAS =====
static const char GHOST_SYMBOLS[MAX_GHOSTS] = {
    SYMBOL_GHOST_RED, SYMBOL_GHOST_GREEN, SYMBOL_GHOST_BLUE, SYMBOL_GHOST_PINK
};

static const Position SCATTER_TARGETS[MAX_GHOSTS] = {
    {2, 2},                                    // Red ghost - canto superior esquerdo
    {MAX_MAP_WIDTH-3, 2},                     // Green ghost - canto superior direito  
    {2, MAX_MAP_HEIGHT-3},                    // Blue ghost - canto inferior esquerdo
    {MAX_MAP_WIDTH-3, MAX_MAP_HEIGHT-3}       // Pink ghost - canto inferior direito
};

// ===== FUNÇÕES AUXILIARES PRIVADAS =====

static void initialize_single_ghost(Ghost* ghost, int id) {
    ghost->ghost_id = id;
    ghost->symbol = GHOST_SYMBOLS[id % MAX_GHOSTS];
    ghost->state = GHOST_NORMAL;
    ghost->is_active = 1;
    ghost->difficulty = DIFFICULTY_MEDIUM;
    ghost->scatter_mode = 1;
    ghost->timer = 0;
    ghost->path_start = 0;
    ghost->path_end = 0;
    ghost->direction = NORTH;
    ghost->target = SCATTER_TARGETS[id % MAX_GHOSTS];
    
    // Posição inicial padrão (será ajustada depois)
    ghost->pos.x = 5 + (id * 4);
    ghost->pos.y = 5 + (id % 2);
}

// ===== FUNÇÕES DE VALIDAÇÃO DE MOVIMENTO =====

bool is_valid_move_ghost_maze(Position pos, const Maze* maze) {
    // Verificar se o maze é válido
    if (!maze) {
        LOG_E("Maze é NULL em is_valid_move_ghost_maze");
        return false;
    }
    
    // Verificar limites básicos
    if (pos.x < 0 || pos.y < 0) {
        LOG_D("Posição inválida: coordenadas negativas (%d,%d)", pos.x, pos.y);
        return false;
    }
    
    if (pos.x >= maze->width || pos.y >= maze->height) {
        LOG_D("Posição fora dos limites: (%d,%d) vs maze (%d,%d)", 
              pos.x, pos.y, maze->width, maze->height);
        return false;
    }
    

    char cell = maze->grid[pos.y][pos.x];
    if (cell == '#') {
        LOG_D("Posição é parede: (%d,%d)", pos.x, pos.y);
        return false;
    }
    
    return true;
}

// Função auxiliar para contar direções válidas
static int count_valid_directions(const Ghost* ghost, const Maze* maze) {
    if (!ghost || !maze) return 0;
    
    int count = 0;
    
    for (Direction dir = NORTH; dir <= WEST; dir++) {
        Position next_pos = get_next_position(ghost->pos, dir);
        
        // Não contar direção oposta (fantasmas não fazem 180°)
        if (dir == get_opposite_direction(ghost->direction) && 
            ghost->state != GHOST_FRIGHTENED) {
            continue;
        }
        
        if (is_valid_move_ghost_maze(next_pos, maze)) {
            count++;
        }
    }
    
    return count;
}

// Função para obter direção aleatória válida
static Direction get_random_valid_direction(const Ghost* ghost, const Maze* maze) {
    if (!ghost || !maze) return NORTH;
    
    Direction valid_directions[4];
    int valid_count = 0;
    
    for (Direction dir = NORTH; dir <= WEST; dir++) {
        Position next_pos = get_next_position(ghost->pos, dir);
        
        // Evitar direção oposta
        if (dir != get_opposite_direction(ghost->direction) && 
            is_valid_move_ghost_maze(next_pos, maze)) {
            valid_directions[valid_count++] = dir;
        }
    }
    
    if (valid_count > 0) {
        return valid_directions[rand() % valid_count];
    }
    
    // Se não há direções válidas, manter direção atual
    LOG_W("Fantasma %d sem direções válidas na posição (%d,%d)", 
          ghost->ghost_id, ghost->pos.x, ghost->pos.y);
    return ghost->direction;
}

// ===== IMPLEMENTAÇÃO DA IA POR TIPO DE FANTASMA =====

static Direction ai_red_ghost(const Ghost* ghost, const Position pacman_pos, const Maze* maze) {
    // Fantasma Vermelho: Perseguição direta agressiva
    Position target = pacman_pos;
    
    // Adicionar erro baseado na dificuldade
    if (ghost->difficulty == DIFFICULTY_EASY) {
        target.x += random_range(-2, 2);
        target.y += random_range(-2, 2);
        
        // Garantir que target está dentro dos limites
        if (target.x < 0) target.x = 0;
        if (target.y < 0) target.y = 0;
        if (target.x >= maze->width) target.x = maze->width - 1;
        if (target.y >= maze->height) target.y = maze->height - 1;
    }
    
    return calculate_best_direction_to_target(ghost, target, maze);
}

static Direction ai_green_ghost(const Ghost* ghost, const Position pacman_pos, const Maze* maze) {
    // Fantasma Verde: Predição de movimento
    Position target = pacman_pos;
    
    // Tentar prever onde o player estará
    int prediction_steps = (ghost->difficulty == DIFFICULTY_HARD) ? 4 : 2;
    
    for (int i = 0; i < prediction_steps; i++) {
        Position next_target = get_next_position(target, NORTH);
        
        // Verificar se posição predita é válida
        if (next_target.x >= 0 && next_target.x < maze->width &&
            next_target.y >= 0 && next_target.y < maze->height) {
            target = next_target;
        } else {
            break; // Parar se sair dos limites
        }
    }
    
    return calculate_best_direction_to_target(ghost, target, maze);
}

static Direction ai_blue_ghost(const Ghost* ghost, const Position pacman_pos, const Maze* maze) {
    // Fantasma Azul: Estratégia de flanqueamento
    Position target;
    int distance = manhattan_distance(ghost->pos, pacman_pos);
    
    if (distance > 8) {
        // Se longe, ir direto
        target = pacman_pos;
    } else {
        // Se perto, flanquear
        target.x = pacman_pos.x + (ghost->pos.x - pacman_pos.x) / 2;
        target.y = pacman_pos.y + (ghost->pos.y - pacman_pos.y) / 2;
        
        // Garantir que target está dentro dos limites
        if (target.x < 0) target.x = 0;
        if (target.y < 0) target.y = 0;
        if (target.x >= maze->width) target.x = maze->width - 1;
        if (target.y >= maze->height) target.y = maze->height - 1;
    }
    
    return calculate_best_direction_to_target(ghost, target, maze);
}

static Direction ai_pink_ghost(const Ghost* ghost, const Position pacman_pos, const Maze* maze) {
    // Fantasma Rosa: Comportamento híbrido
    int distance = manhattan_distance(ghost->pos, pacman_pos);
    
    if (distance > 10) {
        // Se muito longe, comportamento aleatório
        return get_random_valid_direction(ghost, maze);
    } else if (distance > 5) {
        // Se média distância, ir para scatter target
        return calculate_best_direction_to_target(ghost, ghost->target, maze);
    } else {
        // Se perto, perseguição direta
        return calculate_best_direction_to_target(ghost, pacman_pos, maze);
    }
}

// ===== FUNÇÕES PÚBLICAS =====

void init_ghosts(Ghost ghosts[], int count) {
    if (!ghosts) {
        LOG_E("Array de fantasmas é NULL");
        return;
    }
    
    for (int i = 0; i < count && i < MAX_GHOSTS; i++) {
        initialize_single_ghost(&ghosts[i], i);
        LOG_I("Fantasma %d (%c) inicializado", i, ghosts[i].symbol);
    }
    
    LOG_I("Total de %d fantasmas inicializados", count);
}

Direction calculate_next_direction_maze(const Ghost* ghost, const Position pacman_pos, const Maze* maze) {
    if (!ghost) {
        LOG_E("Ghost é NULL em calculate_next_direction_maze");
        return NORTH;
    }
    
    if (!maze) {
        LOG_E("Maze é NULL em calculate_next_direction_maze");
        return NORTH;
    }
    
    // Se em modo scatter, ir para target de scatter
    if (ghost->scatter_mode) {
        return calculate_best_direction_to_target(ghost, ghost->target, maze);
    }
    
    // Se assustado, movimento aleatório
    if (ghost->state == GHOST_FRIGHTENED) {
        return get_random_valid_direction(ghost, maze);
    }
    
    // IA específica por tipo de fantasma
    switch (ghost->ghost_id % MAX_GHOSTS) {
        case 0: return ai_red_ghost(ghost, pacman_pos, maze);
        case 1: return ai_green_ghost(ghost, pacman_pos, maze);
        case 2: return ai_blue_ghost(ghost, pacman_pos, maze);
        case 3: return ai_pink_ghost(ghost, pacman_pos, maze);
        default: return ai_red_ghost(ghost, pacman_pos, maze);
    }
}

Direction calculate_best_direction_to_target(const Ghost* ghost, Position target, const Maze* maze) {
    if (!ghost || !maze) {
        LOG_E("Parâmetros inválidos em calculate_best_direction_to_target");
        return NORTH;
    }
    
    Direction best_dir = ghost->direction;
    int min_distance = 1000000;
    bool found_valid_direction = false;
    
    for (Direction dir = NORTH; dir <= WEST; dir++) {
        Position next_pos = get_next_position(ghost->pos, dir);
        
        // Verificar se movimento é válido
        if (!is_valid_move_ghost_maze(next_pos, maze)) {
            continue;
        }
        
        // Evitar reversão (180°) exceto se necessário
        if (dir == get_opposite_direction(ghost->direction) && 
            count_valid_directions(ghost, maze) > 1) {
            continue;
        }
        
        // Calcular distância até o target
        int distance = manhattan_distance(next_pos, target);
        
        if (distance < min_distance) {
            min_distance = distance;
            best_dir = dir;
            found_valid_direction = true;
        }
    }
    
    // Se não encontrou direção válida, tentar qualquer direção válida
    if (!found_valid_direction) {
        LOG_W("Fantasma %d: Nenhuma direção ótima encontrada, usando direção aleatória", 
              ghost->ghost_id);
        return get_random_valid_direction(ghost, maze);
    }
    
    return best_dir;
}

void update_ghost_state(Ghost* ghost, int current_time) {
    if (!ghost) return;
    
    // Alternar entre modos scatter e chase periodicamente
    if (current_time % 300 == 0) { // A cada ~10 segundos
        ghost->scatter_mode = !ghost->scatter_mode;
        LOG_D("Fantasma %d mudou para modo %s", ghost->ghost_id, 
              ghost->scatter_mode ? "scatter" : "chase");
    }
    
    // Gerenciar estado frightened
    if (ghost->state == GHOST_FRIGHTENED) {
        if (ghost->timer > 200) { // ~7 segundos
            ghost->state = GHOST_NORMAL;
            ghost->timer = 0;
            LOG_D("Fantasma %d saiu do modo frightened", ghost->ghost_id);
        }
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
    ghost->scatter_mode = 1;
    ghost->timer = 0;
    ghost->is_active = 1;
    ghost->path_start = 0;
    ghost->path_end = 0;
    ghost->direction = NORTH;
    
    // Reset para posição inicial
    ghost->pos.x = 5 + (ghost->ghost_id * 4);
    ghost->pos.y = 5 + (ghost->ghost_id % 2);
}

bool check_collision_with_pacman(Ghost ghosts[], int count, Position pacman_pos) {
    if (!ghosts) {
        LOG_E("Array de fantasmas é NULL em check_collision_with_pacman");
        return false;
    }
    
    for (int i = 0; i < count; i++) {
        if (!ghosts[i].is_active) continue;
        
        if (positions_equal(ghosts[i].pos, pacman_pos)) {
            if (ghosts[i].state == GHOST_FRIGHTENED) {
                // Fantasma foi comido
                ghosts[i].state = GHOST_EATEN;
                ghosts[i].is_active = 0;
                LOG_I("Fantasma %d foi comido!", ghosts[i].ghost_id);
                return false; // Não é colisão fatal
            }
            
            LOG_W("Colisão detectada com fantasma %d", ghosts[i].ghost_id);
            return true; // Colisão fatal
        }
    }
    
    return false;
}

// ===== FUNÇÕES AUXILIARES PARA COMPATIBILIDADE =====

Direction calculate_next_direction(const Ghost* ghost, const Position pacman_pos, const char* maze) {
    // Esta função é mantida para compatibilidade, mas não deveria ser usada
    LOG_W("Usando função obsoleta calculate_next_direction - use calculate_next_direction_maze");
    
    // Suprimir warnings de parâmetros não utilizados
    (void)pacman_pos;
    (void)maze;
    
    if (!ghost) return NORTH;
    
    // Implementação simples sem maze structure
    Direction directions[] = {NORTH, EAST, SOUTH, WEST};
    return directions[rand() % 4];
}

Position calculate_target_position(const Ghost* ghost, const Position pacman_pos) {
    if (!ghost) {
        Position invalid = {-1, -1};
        return invalid;
    }
    
    // Retornar target baseado no modo
    if (ghost->scatter_mode) {
        return ghost->target;
    } else {
        return pacman_pos;
    }
}

bool is_valid_move_ghost(Position pos, const char* maze) {
    // Função mantida para compatibilidade
    LOG_W("Usando função obsoleta is_valid_move_ghost - use is_valid_move_ghost_maze");
    
    if (!maze) return false;
    
    // Verificação básica
    if (pos.x < 0 || pos.x >= MAX_MAP_WIDTH || pos.y < 0 || pos.y >= MAX_MAP_HEIGHT) {
        return false;
    }
    
    int index = pos.y * MAX_MAP_WIDTH + pos.x;
    return maze[index] != '#';
}

void move_ghosts(Ghost ghosts[], int count, const Position pacman_pos, const GameState* game_state) {
    // Esta função não é mais usada, mas mantida para compatibilidade
    LOG_W("Função move_ghosts obsoleta - use update_ghosts_ai diretamente no main");
    (void)ghosts;
    (void)count;
    (void)pacman_pos;
    (void)game_state;
}

// ===== FUNÇÕES DE INICIALIZAÇÃO DE SISTEMA =====

void initialize_ghosts_safely(Ghost ghosts[], Maze* maze) {
    init_ghosts(ghosts, MAX_GHOSTS);
    
    // Posicionar fantasmas em locais seguros
    for (int i = 0; i < MAX_GHOSTS; i++) {
        set_ghost_difficulty(&ghosts[i], DIFFICULTY_MEDIUM);
        
        // Calcular posição inicial segura
        int start_x = 5 + (i * 6);  // Distribuir horizontalmente
        int start_y = 3 + (i % 2);  // Alternar entre duas linhas
        
        // ✅ CORRIGIR: Verificação de limites adequada
        if (start_x >= maze->width) start_x = maze->width - 2;
        if (start_y >= maze->height) start_y = maze->height - 2;
        
        // Verificar se posição é válida
        Position test_pos = {start_x, start_y};
        
        // Usar verificação manual para evitar dependência de maze_is_wall
        while (start_x < maze->width - 1 && maze->grid[test_pos.y][test_pos.x] == '#') {
            start_x++;
            test_pos.x = start_x;
            if (start_x >= maze->width - 1) {
                start_x = 1;
                start_y++;
                if (start_y >= maze->height - 1) start_y = 1;
                test_pos.x = start_x;
                test_pos.y = start_y;
            }
        }
        
        ghosts[i].pos = test_pos;
        
        LOG_I("Fantasma %d (%c) posicionado em (%d,%d)", 
              i, ghosts[i].symbol, ghosts[i].pos.x, ghosts[i].pos.y);
    }
}