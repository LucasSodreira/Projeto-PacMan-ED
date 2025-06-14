# Makefile para o Projeto Pac-Man
# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
SRCDIR = src
TESTDIR = test
OBJDIR = obj
BINDIR = bin

# Detectar sistema operacional
ifeq ($(OS),Windows_NT)
    EXE_EXT = .exe
    RM = del /Q
    MKDIR = mkdir
else
    EXE_EXT = 
    RM = rm -f
    MKDIR = mkdir -p
endif

# Arquivos fonte
SOURCES = $(SRCDIR)/queue.c $(SRCDIR)/utils.c $(SRCDIR)/stats.c $(SRCDIR)/logger.c \
          $(SRCDIR)/game.c $(SRCDIR)/ghost.c $(SRCDIR)/player.c $(SRCDIR)/maze.c
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Executáveis de teste
TEST_BASIC = $(BINDIR)/test_structs$(EXE_EXT)
TEST_ADVANCED = $(BINDIR)/test_advanced$(EXE_EXT)
TEST_COMPLETE = $(BINDIR)/test_complete$(EXE_EXT)
TEST_GAME = $(BINDIR)/test_game_integration$(EXE_EXT)

# Alvo padrão
all: dirs $(TEST_BASIC) $(TEST_ADVANCED) $(TEST_COMPLETE) $(TEST_GAME)

# Criar diretórios necessários
dirs:
	$(MKDIR) $(OBJDIR) $(BINDIR)

# Compilar objetos
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Teste básico das estruturas
$(TEST_BASIC): $(OBJECTS) $(TESTDIR)/test_structs.c
	$(CC) $(CFLAGS) $(OBJECTS) $(TESTDIR)/test_structs.c -o $@

# Teste avançado das estruturas
$(TEST_ADVANCED): $(OBJECTS) $(TESTDIR)/test_advanced.c
	$(CC) $(CFLAGS) $(OBJECTS) $(TESTDIR)/test_advanced.c -o $@

# Teste completo do sistema
$(TEST_COMPLETE): $(OBJECTS) $(TESTDIR)/test_complete.c
	$(CC) $(CFLAGS) $(OBJECTS) $(TESTDIR)/test_complete.c -o $@

# Teste de integração do jogo
$(TEST_GAME): $(OBJECTS) $(TESTDIR)/test_game_integration.c
	$(CC) $(CFLAGS) $(OBJECTS) $(TESTDIR)/test_game_integration.c -o $@

# Executar testes
test: all
	$(TEST_BASIC)
	$(TEST_ADVANCED)
	$(TEST_COMPLETE)
	$(TEST_GAME)

# Teste de integração específico
test-game: $(TEST_GAME)
	$(TEST_GAME)

# Compilação com debug
debug: CFLAGS += -DDEBUG -O0
debug: all

# Compilação otimizada
release: CFLAGS += -O2 -DNDEBUG
release: all

# Limpar arquivos compilados
clean:
ifeq ($(OS),Windows_NT)
	if exist $(OBJDIR) rmdir /S /Q $(OBJDIR)
	if exist $(BINDIR) rmdir /S /Q $(BINDIR)
else
	$(RM) -r $(OBJDIR) $(BINDIR)
endif

# Mostrar informações
info:
	@echo "Projeto: Pac-Man Terminal"
	@echo "Compilador: $(CC)"
	@echo "Flags: $(CFLAGS)"
	@echo "Arquivos fonte: $(SOURCES)"
	@echo "Sistema: $(OS)"

# Verificar estilo de código (se disponível)
check-style:
	@echo "Verificando estilo de código..."
	@for file in $(SOURCES) $(SRCDIR)/*.h; do \
		echo "Verificando $$file..."; \
	done

# Targets que não são arquivos
.PHONY: all dirs test test-basic test-advanced test-complete debug release clean info check-style

# Ajuda
help:
	@echo "Targets disponíveis:"
	@echo "  all          - Compilar todos os testes"
	@echo "  test         - Executar todos os testes"
	@echo "  test-basic   - Executar apenas teste básico"
	@echo "  test-advanced- Executar apenas teste avançado"
	@echo "  test-complete- Executar teste completo do sistema"
	@echo "  debug        - Compilar com símbolos de debug"
	@echo "  release      - Compilar otimizado"
	@echo "  clean        - Limpar arquivos compilados"
	@echo "  info         - Mostrar informações do projeto"
	@echo "  help         - Mostrar esta ajuda"
