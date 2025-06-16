# ===== CONFIGURAÇÃO DO MAKEFILE =====
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -I$(SRCDIR)
SRCDIR = src
TESTDIR = test
OBJDIR = obj
BINDIR = bin

# ===== DETECÇÃO DE SISTEMA =====
ifeq ($(OS),Windows_NT)
    EXE_EXT = .exe
    RM = del /Q
    RMDIR = rmdir /S /Q
    MKDIR = mkdir
else
    EXE_EXT = 
    RM = rm -f
    RMDIR = rm -rf
    MKDIR = mkdir -p
endif

# ===== ARQUIVOS FONTE ORGANIZADOS =====
# Módulos básicos
CORE_SOURCES = $(SRCDIR)/utils.c $(SRCDIR)/queue.c

# Sistemas avançados  
SYSTEM_SOURCES = $(CORE_SOURCES) $(SRCDIR)/logger.c $(SRCDIR)/stats.c

# Componentes do jogo
GAME_SOURCES = $(SYSTEM_SOURCES) $(SRCDIR)/maze.c $(SRCDIR)/player.c

# Jogo completo
ALL_SOURCES = $(GAME_SOURCES) $(SRCDIR)/ghost.c $(SRCDIR)/game.c $(SRCDIR)/main.c

# ===== OBJETOS =====
ALL_OBJECTS = $(ALL_SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# ===== EXECUTÁVEIS =====
PACMAN_GAME = $(BINDIR)/pacman$(EXE_EXT)
TEST_BASIC = $(BINDIR)/test_structs$(EXE_EXT)
TEST_ADVANCED = $(BINDIR)/test_advanced$(EXE_EXT)
TEST_COMPLETE = $(BINDIR)/test_complete$(EXE_EXT)
TEST_PLAYER = $(BINDIR)/test_player$(EXE_EXT)

# ===== TARGETS PRINCIPAIS =====

# Target padrão
all: dirs $(PACMAN_GAME) tests

# Jogo principal
game: $(PACMAN_GAME)

# Todos os testes
tests: $(TEST_BASIC) $(TEST_ADVANCED) $(TEST_COMPLETE) $(TEST_PLAYER)

# ===== COMPILAÇÃO =====

# Criar diretórios
dirs:
	@echo "📁 Criando diretórios..."
ifeq ($(OS),Windows_NT)
	@if not exist $(OBJDIR) $(MKDIR) $(OBJDIR)
	@if not exist $(BINDIR) $(MKDIR) $(BINDIR)
else
	@$(MKDIR) $(OBJDIR) $(BINDIR)
endif

# Compilar objetos
$(OBJDIR)/%.o: $(SRCDIR)/%.c | dirs
	@echo "🔨 Compilando $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

# Jogo principal
$(PACMAN_GAME): $(ALL_OBJECTS)
	@echo "🎮 Linkando jogo Pac-Man..."
	@$(CC) $(CFLAGS) $(ALL_OBJECTS) -o $@
	@echo "✅ Jogo compilado: $@"

# ===== TESTES =====

$(TEST_BASIC): $(OBJDIR)/utils.o $(OBJDIR)/queue.o $(TESTDIR)/test_structs.c | dirs
	@echo "🧪 Compilando teste básico..."
	@$(CC) $(CFLAGS) $^ -o $@

$(TEST_ADVANCED): $(OBJDIR)/utils.o $(OBJDIR)/queue.o $(TESTDIR)/test_advanced.c | dirs
	@echo "🧪 Compilando teste avançado..."
	@$(CC) $(CFLAGS) $^ -o $@

$(TEST_COMPLETE): $(OBJDIR)/utils.o $(OBJDIR)/queue.o $(OBJDIR)/logger.o $(OBJDIR)/stats.o $(TESTDIR)/test_complete.c | dirs
	@echo "🧪 Compilando teste completo..."
	@$(CC) $(CFLAGS) $^ -o $@

$(TEST_PLAYER): $(filter-out $(OBJDIR)/main.o, $(ALL_OBJECTS)) $(TESTDIR)/test_player.c | dirs
	@echo "🧪 Compilando teste do player..."
	@$(CC) $(CFLAGS) $^ -o $@

# ===== EXECUÇÃO =====

# Executar o jogo
run: $(PACMAN_GAME)
	@echo "🚀 Iniciando Pac-Man..."
	@$(PACMAN_GAME)

# Executar todos os testes
test: tests
	@echo "🧪 Executando todos os testes..."
	@echo "=== Teste Básico ==="
	@$(TEST_BASIC)
	@echo "=== Teste Avançado ==="  
	@$(TEST_ADVANCED)
	@echo "=== Teste Completo ==="
	@$(TEST_COMPLETE)
	@echo "=== Teste Player ==="
	@$(TEST_PLAYER)
	@echo "✅ Todos os testes concluídos!"

# Testes individuais
test-basic: $(TEST_BASIC)
	@$(TEST_BASIC)

test-advanced: $(TEST_ADVANCED)
	@$(TEST_ADVANCED)

test-complete: $(TEST_COMPLETE)
	@$(TEST_COMPLETE)

test-player: $(TEST_PLAYER)
	@$(TEST_PLAYER)

# ===== VARIAÇÕES DE COMPILAÇÃO =====

# Debug
debug: CFLAGS += -DDEBUG -O0 -g3
debug: clean all
	@echo "🐛 Versão debug compilada"

# Release
release: CFLAGS += -O2 -DNDEBUG
release: clean all
	@echo "🚀 Versão release compilada"

# Profiling
profile: CFLAGS += -pg -O2
profile: clean all
	@echo "📊 Versão profiling compilada"

# ===== LIMPEZA =====

clean:
	@echo "🧹 Limpando arquivos..."
ifeq ($(OS),Windows_NT)
	@if exist $(OBJDIR) $(RMDIR) $(OBJDIR)
	@if exist $(BINDIR) $(RMDIR) $(BINDIR)
	@if exist *.log $(RM) *.log
	@if exist *.dat $(RM) *.dat
else
	@$(RMDIR) $(OBJDIR) $(BINDIR) 2>/dev/null || true
	@$(RM) *.log *.dat 2>/dev/null || true
endif
	@echo "✅ Limpeza concluída"

# Limpeza completa
distclean: clean
	@echo "🗑️  Limpeza completa..."
	@$(RM) tags cscope.* 2>/dev/null || true

# ===== INFORMAÇÕES =====

info:
	@echo "╔════════════════════════════════════╗"
	@echo "║       INFORMAÇÕES DO PROJETO       ║"
	@echo "╚════════════════════════════════════╝"
	@echo "📦 Projeto: Pac-Man Terminal"
	@echo "🔧 Compilador: $(CC)"
	@echo "⚙️  Flags: $(CFLAGS)"
	@echo "💻 Sistema: $(if $(OS),$(OS),Unix-like)"
	@echo "📁 Diretórios: src/, test/, obj/, bin/"
	@echo "🎯 Executáveis: $(words $(ALL_SOURCES)) módulos"
	@echo "✅ Status: 100% implementado"

status:
	@echo "📊 STATUS DOS MÓDULOS:"
	@echo "  ✅ utils.c/h     - Funções utilitárias"
	@echo "  ✅ queue.c/h     - Fila FIFO" 
	@echo "  ✅ logger.c/h    - Sistema de logging"
	@echo "  ✅ stats.c/h     - Estatísticas"
	@echo "  ✅ maze.c/h      - Labirinto"
	@echo "  ✅ player.c/h    - Jogador"
	@echo "  ✅ ghost.c/h     - IA dos fantasmas"
	@echo "  ✅ game.c/h      - Lógica do jogo"
	@echo "  ✅ main.c/h      - Loop principal"
	@echo "  ✅ config.h      - Configurações"

help:
	@echo "🎮 MAKEFILE DO PAC-MAN TERMINAL"
	@echo ""
	@echo "📋 TARGETS PRINCIPAIS:"
	@echo "  make run         - Compilar e executar o jogo"
	@echo "  make game        - Apenas compilar o jogo"  
	@echo "  make test        - Executar todos os testes"
	@echo "  make all         - Compilar tudo"
	@echo ""
	@echo "🧪 TESTES:"
	@echo "  make test-basic    - Teste das estruturas básicas"
	@echo "  make test-advanced - Teste das funcionalidades avançadas"
	@echo "  make test-complete - Teste de integração completa"
	@echo "  make test-player   - Teste do sistema de jogador"
	@echo ""
	@echo "🔧 COMPILAÇÃO:"
	@echo "  make debug       - Versão com debug"
	@echo "  make release     - Versão otimizada"
	@echo "  make profile     - Versão com profiling"
	@echo ""
	@echo "🧹 LIMPEZA:"
	@echo "  make clean       - Limpar arquivos compilados"
	@echo "  make distclean   - Limpeza completa"
	@echo ""
	@echo "ℹ️  INFORMAÇÕES:"
	@echo "  make info        - Informações do projeto"
	@echo "  make status      - Status dos módulos"
	@echo "  make help        - Esta ajuda"

# ===== PHONY TARGETS =====
.PHONY: all game tests dirs run test test-basic test-advanced test-complete test-player
.PHONY: debug release profile clean distclean info status help

# Target padrão quando chamado sem argumentos
.DEFAULT_GOAL := run
