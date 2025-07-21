# 📖 MANUAL DO USUÁRIO - PAC-MAN TERMINAL

<div align="center">

**🎮 Guia Completo para Dominar o Pac-Man Terminal 🎮**

*Versão 2.0 - Manual Técnico e de Gameplay*

</div>

---

## 🎯 **INTRODUÇÃO**

Bem-vindo ao **Pac-Man Terminal**, uma implementação moderna e completa do clássico jogo arcade, desenvolvida especificamente para execução em terminal/CMD. Este manual fornece todas as informações necessárias para instalação, configuração, gameplay e solução de problemas.

### **Sobre o Jogo**
- **Plataforma**: Terminal/CMD (Windows, Linux, macOS)
- **Linguagem**: C (C99 padrão)
- **Gênero**: Arcade/Ação
- **Modo**: Single Player
- **Requisitos**: Terminal com suporte a cores ANSI (recomendado)

---

## 🚀 **INSTALAÇÃO E CONFIGURAÇÃO**

### **Pré-requisitos**

#### **Windows**
```cmd
# Verifique se tem um compilador C instalado
gcc --version

# Se não tiver, instale MinGW ou use Visual Studio
# Download: https://www.mingw-w64.org/
```

#### **Linux (Ubuntu/Debian)**
```bash
# Instale as ferramentas de desenvolvimento
sudo apt update
sudo apt install build-essential gcc make

# Verifique a instalação
gcc --version
make --version
```

#### **macOS**
```bash
# Instale Xcode Command Line Tools
xcode-select --install

# Ou use Homebrew
brew install gcc make
```

### **Compilação e Execução**

#### **Método 1: Execução Rápida (Recomendado)**
```bash
# Navegue até o diretório do projeto
cd Projeto-PacMan-ED

# Compile e execute automaticamente
make run-game
```

#### **Método 2: Compilação Manual**
```bash
# Limpe compilações anteriores
make clean

# Compile o projeto
make

# Execute o jogo
# Windows:
bin\pacman.exe

# Linux/macOS:
./bin/pacman
```

#### **Método 3: Compilação com Debug**
```bash
# Para desenvolvedores - compile com informações de debug
make debug

# Execute com informações detalhadas
./bin/pacman
```

### **Configurações Iniciais**

#### **Arquivo config.h - Personalização**
```c
// Velocidade do jogo (em milissegundos)
#define GAME_SPEED_MS 150        // Padrão: 150ms
                                 // Menor = mais rápido
                                 // Maior = mais lento

// Vidas iniciais do jogador
#define DEFAULT_LIVES 3          // Recomendado: 3-5

// Configurações visuais
#define USE_COLORS 1             // 1 = cores habilitadas
                                 // 0 = apenas ASCII
#define CMD_COMPATIBLE 1         // 1 = compatível com CMD

// Pontuação
#define POINTS_PER_DOT 10        // Pontos por ponto coletado
#define POINTS_PER_POWER_PELLET 50  // Pontos por power pellet
#define POINTS_PER_GHOST_EATEN 200  // Pontos por fantasma comido
```

---

## 🎮 **COMO JOGAR**

### **Objetivo Principal**
Controle o Pac-Man (🟡) através do labirinto, coletando todos os pontos (⚪) e power pellets (🔵) enquanto evita os fantasmas (🔴🟢🔵🟣). Complete todos os níveis para vencer o jogo!

### **Controles Básicos**

| Tecla | Ação | Descrição Detalhada |
|-------|------|---------------------|
| **W** | ⬆️ Mover Cima | Move o Pac-Man uma posição para cima |
| **S** | ⬇️ Mover Baixo | Move o Pac-Man uma posição para baixo |
| **A** | ⬅️ Mover Esquerda | Move o Pac-Man uma posição para esquerda |
| **D** | ➡️ Mover Direita | Move o Pac-Man uma posição para direita |
| **P** | ⏸️ Pausar | Pausa/retoma o jogo atual |
| **Q** | 🚪 Sair | Encerra o jogo imediatamente |

**💡 Dicas de Controle:**
- O movimento é instantâneo - não é necessário segurar a tecla
- O Pac-Man não se move continuamente, cada tecla = um movimento
- Você não pode se mover através de paredes (🔵)
- Use a pausa (P) para planejar sua estratégia

### **Elementos do Jogo**

#### **Elementos Coletáveis**
| Símbolo | Nome | Função | Pontos | Estratégia |
|---------|------|--------|---------|------------|
| ⚪ **`.`** | Ponto | Coletável básico | 10 pts | Colete todos para vencer o nível |
| 🔵 **`O`** | Power Pellet | Deixa fantasmas vulneráveis | 50 pts | Use estrategicamente contra fantasmas |

#### **Personagens**
| Símbolo | Nome | Comportamento | Como Lidar |
|---------|------|---------------|------------|
| 🟡 **`P`** | Pac-Man | Controlado pelo jogador | Você! |
| 🔴 **`F`** | Fantasma Vermelho | Perseguição agressiva | Evite ou use power pellet |
| 🟢 **`G`** | Fantasma Verde | Emboscada inteligente | Cuidado com armadilhas |
| 🔵 **`B`** | Fantasma Azul | Movimento errático | Imprevisível - mantenha distância |
| 🟣 **`R`** | Fantasma Rosa | Bloqueio de rotas | Procure rotas alternativas |

#### **Ambiente**
| Símbolo | Nome | Função |
|---------|------|--------|
| 🔵 **`#`** | Parede | Obstáculo intransponível |
| ⬛ **` `** | Espaço Livre | Área navegável |

### **Estados dos Fantasmas**

#### **1. Estado Normal (Perseguição)**
- **Aparência**: Cor padrão do fantasma
- **Comportamento**: Persegue ativamente o Pac-Man
- **Perigo**: ⚠️ ALTO - Causa morte imediata ao toque
- **Estratégia**: Evite a todo custo ou use power pellet

#### **2. Estado Frightened (Assustado)**
- **Aparência**: Cor azul clara/ciano
- **Comportamento**: Foge do Pac-Man, movimento errático
- **Perigo**: ✅ NENHUM - Pode ser comido para pontos
- **Duração**: Limitada (configurável)
- **Estratégia**: Persiga para ganhar pontos extras!

#### **3. Estado Eaten (Comido)**
- **Aparência**: Apenas os "olhos" (cor branca)
- **Comportamento**: Retorna rapidamente à base
- **Perigo**: ✅ NENHUM - Atravessa paredes
- **Estratégia**: Ignore, ele voltará ao normal na base

#### **4. Estado Scatter (Dispersão)**
- **Aparência**: Cor normal
- **Comportamento**: Vai para cantos específicos do mapa
- **Perigo**: ⚠️ MÉDIO - Ainda pode matar, mas menos agressivo
- **Estratégia**: Aproveite para coletar pontos em áreas "abandonadas"

---

## 📊 **SISTEMA DE PONTUAÇÃO**

### **Pontuação Básica**
- **Ponto (.)**: 10 pontos
- **Power Pellet (O)**: 50 pontos
- **Fantasma Comido**: 200 pontos (apenas quando assustado)

### **Bonificações**
- **Completar Nível**: 100 pontos + bonus de velocidade
- **Vida Extra**: A cada 1000 pontos (configurável)

### **Ranking de Dificuldade**
| Score | Classificação | Comentário |
|-------|---------------|------------|
| 0-500 | Iniciante | Aprendendo os controles |
| 500-1500 | Casual | Entendendo a mecânica |
| 1500-3000 | Experiente | Dominando estratégias |
| 3000-5000 | Veterano | Jogador habilidoso |
| 5000+ | Mestre | Elite do Pac-Man! |

---

## 🧠 **ESTRATÉGIAS E DICAS AVANÇADAS**

### **Estratégias de Sobrevivência**

#### **1. Gerenciamento de Power Pellets**
- **Não use imediatamente**: Guarde para momentos críticos
- **Timing é crucial**: Use quando cercado por múltiplos fantasmas
- **Maximize pontos**: Tente comer o máximo de fantasmas possível durante o efeito

#### **2. Padrões de Movimento dos Fantasmas**
- **Estude os comportamentos**: Cada fantasma tem personalidade única
- **Use as paredes**: Fantasmas não atravessam paredes (exceto quando comidos)
- **Preveja movimentos**: Antecipe onde os fantasmas estarão

#### **3. Controle de Território**
- **Limpe áreas seguras primeiro**: Remova pontos de áreas com poucos fantasmas
- **Mantenha rotas de escape**: Sempre tenha um caminho alternativo
- **Use cantos estrategicamente**: Para forçar fantasmas a mudarem direção

### **Estratégias de Pontuação**

#### **Pontuação Máxima por Nível**
```
Pontos Totais = (Pontos × 10) + (Power Pellets × 50) + (Fantasmas Comidos × 200) + Bonus de Nível
```

#### **Combo de Fantasmas**
- Coma múltiplos fantasmas durante um único power pellet
- Ordem ideal: Vermelho → Verde → Azul → Rosa (do mais próximo ao mais distante)

### **Técnicas Avançadas**

#### **1. Corner Cutting**
- Use a geometria do labirinto para cortar caminho
- Aproveite corredores estreitos para escapar

#### **2. Bait and Switch**
- Atraia fantasmas para uma área e escape por outro caminho
- Use power pellets como "isca"

#### **3. Pattern Recognition**
- Memorize padrões de movimento dos fantasmas
- Identifique ciclos de comportamento

---

## 🛠️ **CONFIGURAÇÃO AVANÇADA**

### **Personalização de Mapas**

#### **Formato de Arquivo de Mapa**
Os mapas são arquivos de texto (.txt) na pasta `maps/`:

```
########################
#..........##..........#
#.##.#####.##.#####.##.#
#O##.......##.......##O#
#.##.##.########.##.##.#
#....##....##....##....#
#.######.##..##.######.#
#......#.##FF##.#......#
#.####.#.######.#.####.#
#....#.#........#.#....#
#.##.#.####..####.#.##.#
#..#............#..#...#
##.#.##.##..##.##.#.##.#
#....##.##P.##.##....##
########################
```

#### **Legenda de Símbolos para Criação de Mapas**
| Símbolo | Significado | Obrigatório |
|---------|-------------|-------------|
| `#` | Parede | Sim - defina as bordas |
| `.` | Ponto coletável | Sim - pelo menos 1 |
| `O` | Power pellet | Opcional |
| `P` | Posição inicial do jogador | Sim - apenas 1 |
| `F` | Posição inicial de fantasma | Opcional |
| ` ` (espaço) | Área livre | Sim |

#### **Regras para Mapas Válidos**
1. **Bordas obrigatórias**: Todo mapa deve ter paredes nas bordas
2. **Jogador único**: Apenas uma posição `P` é permitida
3. **Pontos necessários**: Pelo menos um ponto `.` deve existir
4. **Dimensões**: Máximo 40x20 caracteres
5. **Conectividade**: Todas as áreas navegáveis devem ser conectadas

### **Criando Seu Próprio Mapa**

#### **Passo 1: Planejamento**
```
1. Desenhe no papel primeiro
2. Defina áreas abertas e corredores
3. Posicione strategicamente power pellets
4. Determine posições iniciais
```

#### **Passo 2: Implementação**
```
1. Crie um arquivo novo: maps/level3.txt
2. Use um editor de texto simples
3. Siga as regras de símbolos
4. Teste no jogo
```

#### **Passo 3: Balanceamento**
- **Dificuldade progressiva**: Mapas posteriores devem ser mais difíceis
- **Densidade de pontos**: Equilibre áreas vazias e cheias
- **Posicionamento de fantasmas**: Não coloque todos no mesmo lugar

### **Configurações de Performance**

#### **Para Hardware Mais Lento**
```c
// Em config.h
#define GAME_SPEED_MS 200        // Aumente para diminuir velocidade
#define USE_COLORS 0             // Desabilite cores para melhor performance
```

#### **Para Hardware Mais Rápido**
```c
// Em config.h
#define GAME_SPEED_MS 100        // Diminua para maior velocidade
#define ENHANCED_AI 1            // Habilite IA mais complexa (se disponível)
```

---

## 🚀 **NOVIDADES E MELHORIAS RECENTES**

- **IA dos Fantasmas Aprimorada:** Cada fantasma agora possui um comportamento único, tornando o jogo mais desafiador e fiel ao clássico:
  - Blinky (vermelho): persegue diretamente o Pac-Man.
  - Pinky (rosa): tenta emboscar à frente do Pac-Man.
  - Inky (azul): usa a posição do Blinky e do Pac-Man para calcular seu alvo.
  - Clyde (verde): alterna entre perseguir e fugir dependendo da distância.
- **HUD Aprimorado:** O topo da tela mostra o nível atual, score, vidas, estado do jogo e a direção do Pac-Man, tudo com destaque em cores.
- **Robustez:** O jogo agora trata erros de arquivos, mapas inválidos e problemas de memória de forma amigável, exibindo mensagens claras ao usuário e registrando detalhes no log. Todos os recursos são liberados corretamente ao final do jogo.
- **Código Limpo:** O código-fonte foi revisado, removendo funções não utilizadas, duplicidades e mantendo apenas o essencial para facilitar manutenção e expansão.

---

## 🔧 **TROUBLESHOOTING**

### **Problemas de Compilação**

#### **Erro: "gcc command not found"**
```bash
# Windows: Instale MinGW
# Baixe de: https://www.mingw-w64.org/

# Linux: Instale build tools
sudo apt install build-essential

# macOS: Instale Xcode tools
xcode-select --install
```

#### **Erro: "make command not found"**
```bash
# Windows: Instale através do MinGW ou use
gcc -o bin/pacman src/*.c

# Linux/macOS: Instale make
sudo apt install make  # Linux
brew install make      # macOS
```

#### **Erro: "Permission denied"**
```bash
# Linux/macOS: Dê permissão de execução
chmod +x bin/pacman

# Windows: Execute como administrador se necessário
```

### **Problemas de Execução**

#### **Cores não aparecem no terminal**
```bash
# Verifique se seu terminal suporta cores ANSI
echo -e "\033[31mTeste de cor\033[0m"

# Se não funcionar, desabilite cores em config.h
#define USE_COLORS 0
```

#### **Jogo muito rápido/lento**
```c
// Ajuste em config.h
#define GAME_SPEED_MS 150  // Valor padrão
// Aumente para mais lento (200, 250, 300...)
// Diminua para mais rápido (100, 75, 50...)
```

#### **Controles não funcionam**
- **Verifique**: Se está usando WASD (não as setas)
- **Terminal**: Certifique-se de que o terminal está em foco
- **Caps Lock**: Desabilite Caps Lock se estiver ativo

#### **Fantasmas não se movem**
```bash
# Verifique o arquivo de log
cat game.log | grep "fantasma"

# Possíveis causas:
# 1. Mapa sem posições 'F' para fantasmas
# 2. Erro na inicialização dos fantasmas
# 3. Problema na estrutura de dados da fila
```

#### **Jogo trava ou congela**
```bash
# Termine o processo
Ctrl + C

# Verifique logs para erros
tail -20 game.log

# Recompile completamente
make clean
make
```

### **Problemas de Display**

#### **Caracteres estranhos no terminal**
- **Encoding**: Verifique se o terminal usa UTF-8
- **Fonte**: Use uma fonte monospaced (Courier, Consolas, etc.)
- **Terminal**: Use um terminal moderno (Windows Terminal, iTerm2, etc.)

#### **Layout quebrado**
- **Redimensione**: O terminal para pelo menos 80x25 caracteres
- **Zoom**: Diminua o zoom se necessário
- **Fonte**: Use tamanho de fonte adequado

---

## 📊 **LOGS E DEBUGGING**

### **Sistema de Logs**

O jogo gera automaticamente um arquivo `game.log` com informações detalhadas:

#### **Tipos de Log**
```
[DEBUG] - Informações técnicas detalhadas
[INFO]  - Eventos importantes do jogo
[WARN]  - Situações potencialmente problemáticas
[ERROR] - Erros que podem afetar o funcionamento
```

#### **Como Interpretar Logs**
```bash
# Ver últimas entradas
tail -20 game.log

# Procurar por erros
grep "ERROR" game.log

# Acompanhar em tempo real
tail -f game.log
```

### **Informações Úteis nos Logs**
- Inicialização de componentes
- Movimentos do jogador e fantasmas
- Colisões detectadas
- Mudanças de estado dos fantasmas
- Carregamento de mapas
- Pontuação e eventos de jogo

---

## 🎯 **CONQUISTAS E DESAFIOS**

### **Desafios de Gameplay**

#### **Desafios para Iniciantes**
- 🏆 **Primeiro Passo**: Complete o primeiro nível
- 🏆 **Colecionador**: Colete 100 pontos em um jogo
- 🏆 **Sobrevivente**: Termine um nível sem morrer

#### **Desafios Intermediários**
- 🏆 **Caçador de Fantasmas**: Coma 5 fantasmas em um jogo
- 🏆 **Estrategista**: Complete um nível usando apenas 1 power pellet
- 🏆 **Speedrunner**: Complete um nível em menos de 2 minutos

#### **Desafios Avançados**
- 🏆 **Perfeccionista**: Complete um nível com pontuação máxima
- 🏆 **Sem Piedade**: Coma todos os fantasmas em todos os power pellets
- 🏆 **Mestre**: Complete todos os níveis disponíveis

### **Recordes Pessoais**
Mantenha um registro dos seus melhores resultados:
- **Maior pontuação em um jogo**
- **Nível mais alto alcançado**
- **Maior número de fantasmas comidos**
- **Menor tempo para completar um nível**

---

## 📚 **REFERÊNCIA RÁPIDA**

### **Comandos de Compilação**
```bash
make clean          # Limpa arquivos compilados
make                 # Compila o projeto
make run-game        # Compila e executa
make debug           # Compila com debug
make test            # Executa testes (se disponível)
```

### **Configurações Importantes**
```c
// config.h - Principais constantes
GAME_SPEED_MS       // Velocidade do jogo
DEFAULT_LIVES       // Vidas iniciais
USE_COLORS         // Habilita/desabilita cores
MAX_LEVELS         // Número máximo de níveis
```

### **Estrutura de Arquivos**
```
src/               # Código fonte
maps/              # Mapas dos níveis
bin/               # Executáveis
obj/               # Arquivos objeto
docs/              # Documentação
test/              # Testes
game.log           # Log de execução
Makefile           # Script de compilação
```

### **Atalhos de Teclado**
```
W, A, S, D         # Movimento
P                  # Pausar/Retomar
Q                  # Sair
```

---

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
---

<div align="center">

**🎮 Divirta-se jogando PAC-MAN TERMINAL! 🎮**

*"A nostalgia encontra a tecnologia moderna"*

**Versão 2.0 - Manual Completo**

</div>
