# Makefile para o Projeto Pac-Man
# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -I$(SRCDIR)
SRCDIR = src
TESTDIR = test
OBJDIR = obj
BINDIR = bin

# Detectar sistema operacional
ifeq ($(OS),Windows_NT)
    EXE_EXT = .exe
    RM = del /Q
    MKDIR = if not exist $(1) mkdir $(1)
    RMDIR = if exist $(1) rmdir /S /Q $(1)
    # Função para criar diretório no Windows
    define create_dir
    	if not exist $(1) mkdir $(1)
    endef
else
    EXE_EXT = 
    RM = rm -f
    MKDIR = mkdir -p
    RMDIR = rm -rf
    # Função para criar diretório no Linux/macOS
    define create_dir
    	mkdir -p $(1)
    endef
endif

# Arquivos fonte implementados - INCLUIR TODOS OS MÓDULOS
CORE_SOURCES = $(SRCDIR)/queue.c $(SRCDIR)/utils.c
ADVANCED_SOURCES = $(CORE_SOURCES) $(SRCDIR)/stats.c $(SRCDIR)/logger.c
GAME_SOURCES = $(ADVANCED_SOURCES) $(SRCDIR)/player.c $(SRCDIR)/maze.c
ALL_SOURCES = $(GAME_SOURCES) $(SRCDIR)/ghost.c $(SRCDIR)/game.c $(SRCDIR)/main.c

# Objetos correspondentes
CORE_OBJECTS = $(CORE_SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
ADVANCED_OBJECTS = $(ADVANCED_SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
GAME_OBJECTS = $(GAME_SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
ALL_OBJECTS = $(ALL_SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Executáveis de teste (apenas os que podem ser compilados)
TEST_BASIC = $(BINDIR)/test_structs$(EXE_EXT)
TEST_ADVANCED = $(BINDIR)/test_advanced$(EXE_EXT)
TEST_COMPLETE = $(BINDIR)/test_complete$(EXE_EXT)
TEST_PLAYER = $(BINDIR)/test_player$(EXE_EXT)

# Executáveis
PACMAN_GAME = $(BINDIR)/pacman$(EXE_EXT)

# Alvo padrão - agora incluindo o jogo completo
all: dirs $(TEST_BASIC) $(TEST_ADVANCED) $(TEST_COMPLETE) $(TEST_PLAYER) $(PACMAN_GAME)

# Criar diretórios necessários - CORRIGIDO
dirs:
	@echo "Criando diretórios..."
ifeq ($(OS),Windows_NT)
	@if not exist $(OBJDIR) mkdir $(OBJDIR)
	@if not exist $(BINDIR) mkdir $(BINDIR)
else
	@$(MKDIR) $(OBJDIR) $(BINDIR)
endif

# Compilar objetos individuais - CORRIGIR DEPENDÊNCIA
$(OBJDIR)/%.o: $(SRCDIR)/%.c | dirs
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Teste básico das estruturas (queue + utils)
$(TEST_BASIC): $(CORE_OBJECTS) $(TESTDIR)/test_structs.c
	@echo "Linkando teste básico..."
	$(CC) $(CFLAGS) $(CORE_OBJECTS) $(TESTDIR)/test_structs.c -o $@

# Teste avançado das estruturas (queue + utils)
$(TEST_ADVANCED): $(CORE_OBJECTS) $(TESTDIR)/test_advanced.c
	@echo "Linkando teste avançado..."
	$(CC) $(CFLAGS) $(CORE_OBJECTS) $(TESTDIR)/test_advanced.c -o $@

# Teste completo do sistema (todos os módulos avançados)
$(TEST_COMPLETE): $(ADVANCED_OBJECTS) $(TESTDIR)/test_complete.c
	@echo "Linkando teste completo..."
	$(CC) $(CFLAGS) $(ADVANCED_OBJECTS) $(TESTDIR)/test_complete.c -o $@

# Teste integrado Player + Maze
$(TEST_PLAYER): $(GAME_OBJECTS) $(TESTDIR)/test_player.c
	@echo "Linkando teste do player..."
	$(CC) $(CFLAGS) $(GAME_OBJECTS) $(TESTDIR)/test_player.c -o $@

# Jogo completo
$(PACMAN_GAME): $(ALL_OBJECTS)
	@echo "Linkando jogo completo..."
	$(CC) $(CFLAGS) $(ALL_OBJECTS) -o $@

# Executar testes disponíveis
test: all
	@echo "=== Executando Teste Básico ==="
	$(TEST_BASIC)
	@echo ""
	@echo "=== Executando Teste Avançado ==="
	$(TEST_ADVANCED)
	@echo ""
	@echo "=== Executando Teste Completo ==="
	$(TEST_COMPLETE)
	@echo ""
	@echo "=== Executando Teste do Player ==="
	$(TEST_PLAYER)

# Executar jogo
run-game: $(PACMAN_GAME)
	@echo "Executando Pac-Man..."
	$(PACMAN_GAME)

# Testes individuais
test-basic: $(TEST_BASIC)
	@echo "Executando teste básico..."
	$(TEST_BASIC)

test-advanced: $(TEST_ADVANCED)
	@echo "Executando teste avançado..."
	$(TEST_ADVANCED)

test-complete: $(TEST_COMPLETE)
	@echo "Executando teste completo..."
	$(TEST_COMPLETE)

test-player: $(TEST_PLAYER)
	@echo "Executando teste do player..."
	$(TEST_PLAYER)

# Compilação com debug
debug: CFLAGS += -DDEBUG -O0
debug: all

# Compilação otimizada
release: CFLAGS += -O2 -DNDEBUG
release: all

# Verificar arquivos necessários - ATUALIZADO
check:
	@echo "Verificando arquivos fonte..."
	@echo "Implementados:"
	@for file in $(ALL_SOURCES); do \
		if [ -f $$file ]; then echo "  ✓ $$file"; else echo "  ✗ $$file (faltando)"; fi; \
	done
	@echo ""
	@echo "Pendentes para jogo completo:"
	@echo "  ✗ $(SRCDIR)/ghost.c"
	@echo "  ✗ $(SRCDIR)/game.c" 
	@echo "  ✗ $(SRCDIR)/main.c"

# Limpar arquivos compilados - CORRIGIDO
clean:
	@echo "Limpando arquivos compilados..."
ifeq ($(OS),Windows_NT)
	@if exist $(OBJDIR) rmdir /S /Q $(OBJDIR)
	@if exist $(BINDIR) rmdir /S /Q $(BINDIR)
	@if exist *.exe del /Q *.exe
	@if exist game.log del /Q game.log
	@if exist test_log.txt del /Q test_log.txt
	@if exist test_stats.dat del /Q test_stats.dat
else
	@$(RMDIR) $(OBJDIR) $(BINDIR) 2>/dev/null || true
	@$(RM) game.log test_log.txt test_stats.dat 2>/dev/null || true
endif
	@echo "Limpeza concluída!"

# Mostrar informações
info:
	@echo "=== Informações do Projeto ==="
	@echo "Projeto: Pac-Man Terminal"
	@echo "Compilador: $(CC)"
	@echo "Flags: $(CFLAGS)"
	@echo "Sistema: $(if $(OS),$(OS),Unix-like)"
	@echo ""
	@echo "Módulos implementados:"
	@echo "  ✓ Queue (FIFO)"
	@echo "  ✓ Utils (funções auxiliares)"
	@echo "  ✓ Logger (sistema de logging)"
	@echo "  ✓ Stats (estatísticas)"
	@echo "  ✓ Player (lógica do jogador)"
	@echo "  ✓ Maze (renderização de mapas)"
	@echo ""
	@echo "Pendentes:"
	@echo "  ⏳ Ghost (IA dos fantasmas)"
	@echo "  ⏳ Game (loop principal)"
	@echo "  ⏳ Main (integração final)"

# Verificar estilo de código (básico)
check-style:
	@echo "Verificando arquivos de código..."
	@for file in $(SRCDIR)/*.c $(SRCDIR)/*.h; do \
		if [ -f "$$file" ]; then \
			echo "✓ $$file existe"; \
		fi; \
	done

# Instalar dependências (placeholder)
install:
	@echo "Verificando dependências..."
	@echo "GCC: $(shell $(CC) --version 2>/dev/null | head -n1 || echo 'Não encontrado')"
	@echo "Make: $(shell make --version 2>/dev/null | head -n1 || echo 'Não encontrado')"

# Targets que não são arquivos
.PHONY: all dirs test test-basic test-advanced test-complete test-player run-game debug release clean info check-style check install help

# Ajuda - ATUALIZADA
help:
	@echo "=== Makefile do Projeto Pac-Man ==="
	@echo ""
	@echo "Targets disponíveis:"
	@echo "  all           - Compilar tudo (testes + jogo)"
	@echo "  run-game      - Executar o jogo Pac-Man"
	@echo "  test          - Executar todos os testes"
	@echo "  test-player   - Executar teste integrado (player + maze)"
	@echo ""
	@echo "Exemplo de uso:"
	@echo "  make run-game       # Jogar Pac-Man!"
	@echo "  make test-player    # Testar funcionalidade atual"
	@echo "  make clean && make  # Rebuild completo"
