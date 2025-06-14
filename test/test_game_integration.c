#include "../src/game.h"
#include "../src/ghost.h"
#include "../src/logger.h"
#include "../src/config.h"
#include "../src/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// Mapa de teste simples (20x20)
const char TEST_MAP[MAX_MAP_SIZE * MAX_MAP_SIZE] = 
    "####################"
    "#........##........#"
    "#.##.##..##..##.##.#"
    "#.##.##..##..##.##.#"
    "#.##.##..##..##.##.#"
    "#..................#"
    "#.##.##.####.##.##.#"
    "#.##.##.####.##.##.#"
    "#......########....#"
    "####.##########.####"
    "####.##########.####"
    "#......########....#"
    "#.##.##.####.##.##.#"
    "#.##.##.####.##.##.#"
    "#..................#"
    "#.##.##..##..##.##.#"
    "#.##.##..##..##.##.#"
    "#.##.##..##..##.##.#"
    "#........##........#"
    "####################";

void verify_ghost_state(Ghost* ghost) {
    assert(ghost->is_active >= 0 && ghost->is_active <= 1);
    assert(ghost->ghost_id >= 0 && ghost->ghost_id < MAX_GHOSTS);
    assert(ghost->state >= GHOST_NORMAL && ghost->state <= GHOST_EATEN);
    assert(ghost->difficulty >= DIFFICULTY_EASY && ghost->difficulty <= DIFFICULTY_HARD);
    assert(ghost->path_start >= 0 && ghost->path_start <= MAX_PATH_LENGTH);
    assert(ghost->path_end >= 0 && ghost->path_end <= MAX_PATH_LENGTH);
    assert(ghost->timer >= 0);
}

void test_ghost_initialization() {
    printf("\n=== Testando inicialização dos fantasmas ===\n");
    Ghost ghosts[MAX_GHOSTS];
    init_ghosts(ghosts, MAX_GHOSTS);
    
    for (int i = 0; i < MAX_GHOSTS; i++) {
        verify_ghost_state(&ghosts[i]);
        assert(ghosts[i].is_active == 1);
        assert(ghosts[i].ghost_id == i);
        assert(ghosts[i].state == GHOST_NORMAL);
        
        // Verificar posição inicial válida
        assert(ghosts[i].pos.x >= 0 && ghosts[i].pos.x < MAX_MAP_SIZE);
        assert(ghosts[i].pos.y >= 0 && ghosts[i].pos.y < MAX_MAP_SIZE);
        printf("✓ Fantasma %d inicializado corretamente\n", i);
    }
}

void test_ghost_movement() {
    printf("\n=== Testando movimentação dos fantasmas ===\n");
    Ghost ghosts[MAX_GHOSTS];
    init_ghosts(ghosts, MAX_GHOSTS);
    Position pacman = {5, 5};
    
    // Testar movimento por 5 frames
    for (int frame = 0; frame < 5; frame++) {
        Position old_positions[MAX_GHOSTS];
        for (int i = 0; i < MAX_GHOSTS; i++) {
            old_positions[i] = ghosts[i].pos;
        }
        
        move_ghosts(ghosts, MAX_GHOSTS, pacman, TEST_MAP);
        
        for (int i = 0; i < MAX_GHOSTS; i++) {
            verify_ghost_state(&ghosts[i]);
            assert(is_valid_move(ghosts[i].pos, TEST_MAP));
            
            // Verificar se o movimento foi válido
            int dx = abs(ghosts[i].pos.x - old_positions[i].x);
            int dy = abs(ghosts[i].pos.y - old_positions[i].y);
            assert(dx <= 1 && dy <= 1); // Só pode mover 1 casa por vez
            
            if (!positions_equal(ghosts[i].pos, old_positions[i])) {
                printf("✓ Fantasma %d moveu de (%d,%d) para (%d,%d)\n",
                    i, old_positions[i].x, old_positions[i].y,
                    ghosts[i].pos.x, ghosts[i].pos.y);
            }
        }
    }
}

void test_collision_detection() {
    printf("\n=== Testando detecção de colisão ===\n");
    Ghost ghosts[MAX_GHOSTS];
    init_ghosts(ghosts, MAX_GHOSTS);
    Position pacman = {5, 5};
    
    // Testar colisão normal
    ghosts[0].pos = pacman;
    ghosts[0].state = GHOST_NORMAL;
    assert(check_collision_with_pacman(ghosts, MAX_GHOSTS, pacman));
    printf("✓ Colisão com fantasma normal detectada\n");
    
    // Testar colisão com fantasma assustado
    ghosts[0].state = GHOST_FRIGHTENED;
    assert(!check_collision_with_pacman(ghosts, MAX_GHOSTS, pacman)); // Deve retornar false após comer o fantasma
    printf("✓ Colisão com fantasma assustado detectada (fantasma foi comido)\n");
    
    // Testar não-colisão
    pacman.x += 2;
    assert(!check_collision_with_pacman(ghosts, MAX_GHOSTS, pacman));
    printf("✓ Não-colisão detectada corretamente\n");
}

void test_score_system() {
    printf("\n=== Testando sistema de pontuação ===\n");
    int score = 0;
    
    update_score(&score, SYMBOL_DOT);
    assert(score == POINTS_PER_DOT);
    printf("✓ Pontuação por ponto comum: %d\n", POINTS_PER_DOT);
    
    update_score(&score, SYMBOL_POWER_PELLET);
    assert(score == POINTS_PER_DOT + POINTS_PER_POWER_PELLET);
    printf("✓ Pontuação por power pellet: %d\n", POINTS_PER_POWER_PELLET);
    
    update_score(&score, SYMBOL_GHOST_RED);
    assert(score == POINTS_PER_DOT + POINTS_PER_POWER_PELLET + POINTS_FOR_EXTRA_LIFE);
    printf("✓ Pontuação por comer fantasma: %d\n", POINTS_FOR_EXTRA_LIFE);
}

void test_logger_integration() {
    printf("\n=== Testando integração com logger ===\n");
    
    // Inicializar logger em modo debug
    assert(logger_init("test_game.log", LOG_DEBUG));
    printf("✓ Logger inicializado\n");
    
    // Testar diferentes tipos de log
    Position pos = {3, 4};
    logger_log_player_action("moveu para", pos.x, pos.y);
    logger_dot_collected(pos.x, pos.y, POINTS_PER_DOT);
    logger_collision_detected(pos.x, pos.y, 1);
    logger_game_started();
    logger_game_ended(1000, 300);
    printf("✓ Eventos registrados no log\n");
    
    // Verificar estatísticas
    logger_print_stats();
    logger_shutdown();
    printf("✓ Sistema de log finalizado corretamente\n");
}

void test_game_rendering() {
    printf("\n=== Testando renderização do jogo ===\n");
    Ghost ghosts[MAX_GHOSTS];
    init_ghosts(ghosts, MAX_GHOSTS);
    Position pacman = {5, 5};
    
    // Verificar se o mapa está completo
    int dots = 0, walls = 0;
    for (int i = 0; i < MAX_MAP_SIZE * MAX_MAP_SIZE; i++) {
        if (TEST_MAP[i] == SYMBOL_DOT) dots++;
        if (TEST_MAP[i] == SYMBOL_WALL) walls++;
    }
    assert(dots > 0);
    assert(walls > 0);
    printf("✓ Mapa válido (contém %d pontos e %d paredes)\n", dots, walls);
    
    printf("\nMapa de teste:\n");
    draw_game(ghosts, MAX_GHOSTS, pacman, TEST_MAP);
    printf("✓ Renderização concluída\n");
}

int main() {
    srand(time(NULL)); // Inicializar gerador de números aleatórios
    printf("\n🎮 Iniciando testes de integração do jogo...\n");
    printf("Data: %s\n", __DATE__);
    printf("MAX_GHOSTS: %d\n", MAX_GHOSTS);
    printf("Tamanho do mapa: %dx%d\n\n", MAX_MAP_SIZE, MAX_MAP_SIZE);
    
    test_ghost_initialization();
    test_ghost_movement();
    test_collision_detection();
    test_score_system();
    test_logger_integration();
    test_game_rendering();
    
    printf("\n✨ TODOS OS TESTES CONCLUÍDOS COM SUCESSO! ✨\n");
    return 0;
}
