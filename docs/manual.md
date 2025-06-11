# 📖 Manual do Usuário - Pac-Man Terminal

## 🎮 Introdução

Bem-vindo ao **Pac-Man Terminal**! Este é um jogo clássico implementado em C para ser jogado diretamente no terminal/prompt de comando. O objetivo é simples: colete todos os pontos espalhados pelo labirinto sem ser tocado pelos fantasmas.

## 🎯 Objetivo do Jogo

**Meta Principal:** Colete todos os pontos (.) espalhados pelo labirinto sem ser capturado pelos fantasmas.

### Como Ganhar
- ✅ Colete **TODOS** os pontos do mapa
- ✅ Evite tocar nos fantasmas
- ✅ Complete o nível para avançar

### Como Perder
- ❌ Tocar em qualquer fantasma
- ❌ Ficar sem vidas restantes

## 🕹️ Controles

### Movimentação
| Tecla | Ação |
|-------|------|
| **W** | Mover para **CIMA** ⬆️ |
| **S** | Mover para **BAIXO** ⬇️ |
| **A** | Mover para **ESQUERDA** ⬅️ |
| **D** | Mover para **DIREITA** ➡️ |

### Comandos Especiais
| Tecla | Ação |
|-------|------|
| **Q** | **SAIR** do jogo |
| **P** | **PAUSAR** o jogo (se implementado) |
| **D** | **DEBUG** - Mostrar estado da fila (modo debug) |
| **S** | **STATS** - Mostrar estatísticas (modo debug) |
| **L** | **LOG** - Alternar nível de log (modo debug) |

### Comandos de Debug (Modo Debug Ativo)
| Combinação | Ação |
|------------|------|
| **Ctrl+D** | Dump completo do estado do jogo |
| **Ctrl+S** | Salvar estatísticas atuais |
| **Ctrl+L** | Mostrar últimas 10 mensagens de log |
| **Ctrl+M** | Mostrar uso atual de memória |
| **Ctrl+P** | Mostrar informações de performance |

> **💡 Dica:** As teclas são **case-insensitive** - tanto maiúsculas quanto minúsculas funcionam!

### Modos Especiais de Execução
```cmd
# Modo verbose com todas as informações
pacman --verbose

# Modo silencioso (apenas erros críticos)
pacman --quiet

# Modo de demonstração (se implementado)
pacman --demo

# Modo benchmark para testar performance
pacman --benchmark
```

## 🗺️ Elementos do Jogo

### Símbolos no Mapa

| Símbolo | Elemento | Descrição |
|---------|----------|-----------|
| **P** | **Pac-Man** | Você! O personagem controlado pelo jogador |
| **F** | **Fantasma Vermelho** | Inimigo - evite tocar! |
| **G** | **Fantasma Verde** | Inimigo - evite tocar! |
| **B** | **Fantasma Azul** | Inimigo - evite tocar! |
| **R** | **Fantasma Rosa** | Inimigo - evite tocar! |
| **#** | **Parede** | Bloqueio - não é possível atravessar |
| **.** | **Ponto** | Colete para ganhar pontos! |
| **(espaço)** | **Caminho Livre** | Área onde você pode se mover |

### Exemplo de Mapa
```
######################
#........#.........#
#.##.###.#.###.##.#
#.................#
#.##.#.#####.#.##.#
#....#...P...#....#
####.###.#.###.####
#........#........#
#.##.###.#.###.##.#
#..................#
#F.##.#######.##.G#
#..................#
######################
```

## 🎲 Como Jogar

### 1. **Iniciando o Jogo**
```bash
# Executar o jogo
./pacman

# Ou no Windows
pacman.exe
```

### 2. **Durante o Jogo**
1. **Movimente o Pac-Man** usando as teclas W, A, S, D
2. **Colete pontos** passando por cima dos símbolos "."
3. **Evite os fantasmas** - eles se movem automaticamente
4. **Observe sua pontuação** na parte superior/inferior da tela
5. **Complete o nível** coletando todos os pontos

### 3. **Finalizando**
- Pressione **Q** para sair a qualquer momento
- O jogo termina automaticamente se você:
  - Coletar todos os pontos (VITÓRIA!)
  - Perder todas as vidas (GAME OVER)

## 📊 Sistema de Pontuação

### Pontos por Ação
| Ação | Pontos |
|------|--------|
| Coletar um ponto (.) | +10 pontos |
| Completar um nível | +100 pontos |
| Vida extra | A cada 1000 pontos |

### Informações na Tela
Durante o jogo, você verá:
- **Pontuação atual**
- **Vidas restantes**
- **Nível atual**
- **Pontos coletados / Total de pontos**

## 👻 Comportamento dos Fantasmas

### Como se Movem
- Os fantasmas se movem **automaticamente**
- Utilizam sistema **FIFO (First In, First Out)**
- Cada fantasma tem sua própria **direção inicial**
- Mudam de direção ao encontrar paredes

### Tipos de Fantasmas
1. **F (Vermelho)** - Movimento agressivo
2. **G (Verde)** - Movimento em patrulha
3. **B (Azul)** - Movimento defensivo
4. **R (Rosa)** - Movimento aleatório

> **📝 Nota:** O comportamento específico pode variar dependendo da implementação da Pessoa 4.

## 🎮 Estratégias e Dicas

### Dicas Básicas
1. **📍 Planeje sua rota** - Observe onde estão os fantasmas
2. **🔄 Use os cantos** - Fantasmas podem ter dificuldade em cantos
3. **⚡ Seja rápido** - Não fique parado muito tempo
4. **🎯 Priorize pontos isolados** - Colete pontos em áreas seguras primeiro

### Estratégias Avançadas
1. **🔍 Observe padrões** - Fantasmas podem ter rotas previsíveis
2. **🚪 Use "corredores"** - Movimente-se por caminhos estreitos
3. **⏰ Timing é tudo** - Espere o momento certo para passar por fantasmas
4. **🎪 Bait and switch** - Atraia fantasmas para uma direção e vá para outra

## ⚠️ Solução de Problemas

### Problemas Comuns e Soluções

#### O jogo não inicia
```cmd
# 1. Verifique se existe
dir pacman.exe

# 2. Recompile com debug
make clean && make debug

# 3. Execute com logging ativo
pacman.exe --debug

# 4. Verifique logs de erro
type error_log.txt
```

#### Controles não respondem
```cmd
# 1. Teste com logging ativo
pacman --log-level=DEBUG

# 2. Procure por mensagens como:
# [DEBUG] Input recebido: 'w'
# [WARNING] Input inválido ignorado

# 3. Verifique configurações
# Certifique-se de usar W,A,S,D (não setas)
```

#### Jogo muito rápido/lento
- Velocidade do jogo depende da implementação
- Pode variar entre diferentes sistemas operacionais

#### Caracteres estranhos na tela
- Use terminal compatível com UTF-8
- No Windows, considere usar Command Prompt ou PowerShell

### Requisitos do Sistema
- **SO:** Windows, Linux, macOS
- **Terminal:** Qualquer terminal moderno
- **Memória:** Mínimo 1MB RAM
- **Espaço:** ~50KB de espaço em disco

## 🔧 Comandos de Compilação

### Para Usuários Avançados
```bash
# Compilação simples
gcc -o pacman src/*.c

# Compilação com debug
gcc -g -DDEBUG -o pacman src/*.c

# Compilação otimizada
gcc -O2 -DNDEBUG -o pacman src/*.c

# Compilação com sistema de logging ativo
gcc -DLOG_LEVEL=0 -o pacman src/*.c
```

## 🐛 Sistema de Debug e Logging

### Ativando Modo Debug
```cmd
# Compilar com debug ativo
make debug

# Ou manualmente
gcc -g -DDEBUG -DLOG_LEVEL=0 src/*.c -o pacman
```

### Interpretando Mensagens de Log

O jogo utiliza um sistema de logging profissional com 5 níveis:

| Nível | Cor | Descrição | Quando Aparece |
|-------|-----|-----------|----------------|
| **DEBUG** | 🔍 Cyan | Informações detalhadas | Modo debug ativo |
| **INFO** | ℹ️ Branco | Informações gerais | Eventos normais |
| **WARNING** | ⚠️ Amarelo | Avisos importantes | Situações anômalas |
| **ERROR** | ❌ Vermelho | Erros recuperáveis | Problemas detectados |
| **CRITICAL** | 🚨 Magenta | Erros críticos | Falhas graves |

### Exemplo de Saída com Logging
```
[2025-06-11 14:30:15] [INFO] Iniciando Pac-Man Terminal v1.0
[2025-06-11 14:30:15] [DEBUG] Sistema de fila inicializado
[2025-06-11 14:30:15] [INFO] Mapa carregado: level1.txt (23x12)
[2025-06-11 14:30:15] [DEBUG] 4 fantasmas adicionados à fila
[2025-06-11 14:30:16] [WARNING] Fantasma colidiu com parede
[2025-06-11 14:30:20] [INFO] Pontuação: 150 pontos
```

### Arquivos de Log
- **game_log.txt** - Log completo do jogo
- **game_stats.txt** - Estatísticas de performance
- **error_log.txt** - Apenas erros e warnings

## 📊 Sistema de Estatísticas

### Visualizando Estatísticas em Tempo Real
Durante o jogo, estatísticas são coletadas automaticamente:

```
=== ESTATÍSTICAS DO JOGO ===
Tempo de jogo: 02:45
Pontos coletados: 150/200
Fantasmas ativos: 4/4
Operações de fila: 1,247
Colisões detectadas: 3
FPS médio: 28.5
```

### Estatísticas Salvas
```cmd
# Ver estatísticas salvas
type game_stats.txt    # Windows
cat game_stats.txt     # Linux/macOS
```

### Análise de Performance
```
=== PERFORMANCE DA FILA ===
Enqueue: 0.001ms (média)
Dequeue: 0.001ms (média)  
Search: 0.025ms (média)
Uso de memória: 2.1KB
```

## ⚙️ Configurações Avançadas

### Arquivo de Configuração (config.h)
Você pode ajustar configurações editando `src/config.h`:

```c
// Configurações de jogo
#define MAX_GHOSTS 256          // Máximo de fantasmas
#define GAME_SPEED 100          // Velocidade (ms entre frames)
#define POINTS_PER_DOT 10       // Pontos por bolinha
#define EXTRA_LIFE_SCORE 1000   // Pontos para vida extra

// Configurações de logging
#define LOG_LEVEL LOG_INFO      // Nível mínimo de log
#define LOG_TO_FILE 1           // Salvar logs em arquivo
#define LOG_WITH_COLOR 1        // Usar cores no terminal

// Configurações de debug
#define SHOW_PERFORMANCE 1      // Mostrar stats de performance
#define SHOW_GHOST_INFO 1       // Mostrar info dos fantasmas
```

### Configurações por Linha de Comando
```cmd
# Ativar debug verbose
pacman --debug

# Definir nível de log
pacman --log-level=DEBUG

# Desativar cores
pacman --no-color

# Modo de teste (se implementado)
pacman --test-mode
```

## 🧪 Modos de Teste

### Executando Testes
```cmd
# Teste básico das estruturas
make test-basic
./test_structs.exe

# Teste das funcionalidades avançadas
make test-advanced  
./test_advanced.exe

# Teste completo integrado
make test-complete
./test_complete.exe
```

## 🏆 Boa Sorte!

Agora você está pronto para enfrentar o labirinto com todas as ferramentas avançadas à sua disposição! Lembre-se: 

**Que a força esteja com você, Pac-Man!** 🎮👾

---

**Manual versão 1.0**  
**Última atualização:** Junho 2025  
**Criado por:** Equipe Pac-Man ED