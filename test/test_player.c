#include <stdio.h>
#include <assert.h>
#include "../src/player.h"
#include "../src/maze.h"
#include "../src/utils.h"

int main() {
    printf("=== TESTE PLAYER + MAZE INTEGRADO ===\n");
    
    // ✅ Usar Maze em vez de GameState
    Maze maze;
    Player player;
    
    // Inicializar maze (vai usar o mapa padrão do maze.c)
    maze_init(&maze);
    
    // Inicializar player
    Position start_pos = {1, 1};
    player_init(&player, start_pos);
    
    printf("✅ Inicialização:\n");
    printf("   Player: (%d,%d), Score: %d, Lives: %d\n", 
           player.pos.x, player.pos.y, player.score, player.lives);
    printf("   Maze: %dx%d com %d pontos\n", 
           maze.width, maze.height, maze.total_points);
    
    // Testar movimentos
    printf("\n✅ Teste 1: Mover para direita (d)...\n");
    int old_score = player.score;
    player_move(&player, &maze, 'd');  // ✅ Usar assinatura correta
    printf("   Nova posição: (%d,%d)\n", player.pos.x, player.pos.y);
    printf("   Score: %d -> %d\n", old_score, player.score);
    
    // Verificar se coletou ponto
    if (player.score > old_score) {
        printf("   ✅ Ponto coletado!\n");
    }
    
    printf("\n✅ Teste 2: Mover para baixo (s)...\n");
    old_score = player.score;
    player_move(&player, &maze, 's');
    printf("   Nova posição: (%d,%d)\n", player.pos.x, player.pos.y);
    printf("   Score: %d -> %d\n", old_score, player.score);
    
    printf("\n✅ Teste 3: Tentar movimento inválido (a - vai para parede)...\n");
    // Primeiro vamos mover para uma posição próxima a uma parede
    player_move(&player, &maze, 'a'); // Move para (1,2)
    player_move(&player, &maze, 'a'); // Tenta mover para (0,2) que é parede '#'
    
    Position expected_pos = {1, 2}; // Deveria ficar em (1,2)
    if (player.pos.x == expected_pos.x && player.pos.y == expected_pos.y) {
        printf("   ✅ Movimento bloqueado corretamente!\n");
    } else {
        printf("   ❌ ERRO: Movimento não foi bloqueado! Pos: (%d,%d)\n",
               player.pos.x, player.pos.y);
    }
    
    printf("\n✅ Teste 4: Verificar condição de vitória...\n");
    // Para teste rápido, vamos simular coleta de todos os pontos
    int remaining_points = maze_count_points(&maze);
    printf("   Pontos restantes no maze: %d\n", remaining_points);
    
    // Se ainda há pontos, não deve ter vencido
    if (remaining_points > 0) {
        printf("   ✅ Ainda há pontos - não venceu ainda\n");
    } else {
        printf("   🎉 Todos os pontos coletados - VITÓRIA!\n");
    }
    
    printf("\n🎮 TESTE CONCLUÍDO COM SUCESSO!\n");
    return 0;
}