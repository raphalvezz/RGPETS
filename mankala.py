import pygame
import sys
import os

# Inicializar Pygame
pygame.init()

# Configurações da tela para tela cheia
SCREEN = pygame.display.set_mode((0, 0), pygame.FULLSCREEN)
WIDTH, HEIGHT = SCREEN.get_size()
pygame.display.set_caption("Mancala")

# Cores
LIGHT_BLUE = (80, 255, 180)
RED = (255, 0, 0)
BOTTOM = (55, 65, 65, 230)
BLUE = (23, 35, 51)  
WHITE = (255, 255, 255)

# Caminho para as imagens
ASSETS_PATH = os.path.join(os.path.dirname(__file__), 'assets')

# Carregar imagens
bolinha_img = pygame.image.load(os.path.join(ASSETS_PATH, "bolinha.png"))
bolinha_img = pygame.transform.scale(bolinha_img, (int(HEIGHT / 25), int(HEIGHT / 25)))  # Ajustar o tamanho da bolinha
fundo_img = pygame.image.load(os.path.join(ASSETS_PATH, "FUNDO.jpg"))
fundo_img = pygame.transform.scale(fundo_img, (WIDTH, HEIGHT))  # Ajustar o tamanho do fundo

# Tamanho e posições ajustados para tela cheia
NUM_HOLES = 6  # Definindo a variável antes de usá-la
HOLE_RADIUS = int(HEIGHT / 15)
MARGIN = int(WIDTH / 15)
HOLE_SPACING = int(WIDTH / (NUM_HOLES + 2))

# Fonte para o texto
font = pygame.font.SysFont('Arial', int(HEIGHT / 16))
small_font = pygame.font.SysFont('Arial', int(HEIGHT / 24))
tiny_font = pygame.font.SysFont('Arial', int(HEIGHT / 32))

# Tabuleiro inicial
board = [4] * NUM_HOLES + [0] + [4] * NUM_HOLES + [0]
player_turn = 0  # 0 para jogador 1, 1 para jogador 2
message = ""
small_message = ""
capture_mode = False  # Modo de captura
capture_self_mode = False  # Modo de captura ao acertar a própria casa

def draw_board(capture_index=None):
    SCREEN.blit(fundo_img, (0, 0))

    # Desenhar fundo retangular arredondado com transparência
    s = pygame.Surface((WIDTH - 2 * MARGIN + 40, HEIGHT - 2 * MARGIN + 40), pygame.SRCALPHA)
    s.fill(BOTTOM)
    pygame.draw.rect(s, BOTTOM, (0, 0, WIDTH - 2 * MARGIN + 40, HEIGHT - 2 * MARGIN + 40), border_radius=20)
    SCREEN.blit(s, (MARGIN - 20, MARGIN - 20))
        
    # Desenhar os buracos do jogador 1
    for i in range(NUM_HOLES):
        x = MARGIN + (i + 1) * HOLE_SPACING  
        y = HEIGHT - MARGIN - HOLE_RADIUS - 10  
        pygame.draw.circle(SCREEN, BLUE, (x, y), HOLE_RADIUS)
        pygame.draw.circle(SCREEN, BOTTOM, (x, y), HOLE_RADIUS, 2)
        draw_bolinhas(x, y, board[i])
        text_color = LIGHT_BLUE if player_turn == 0 else WHITE
        text = small_font.render(str(board[i]), True, text_color)
        SCREEN.blit(text, (x - text.get_width() // 2, y + HOLE_RADIUS - 7))  # Ajuste final para afastar um pouquinho o texto do buraco para o Player 1

    # Desenhar os buracos do jogador 2
    for i in range(NUM_HOLES):
        x = MARGIN + (NUM_HOLES - i) * HOLE_SPACING  
        y = MARGIN + HOLE_RADIUS + 10  
        pygame.draw.circle(SCREEN, BLUE, (x, y), HOLE_RADIUS)
        pygame.draw.circle(SCREEN, BOTTOM, (x, y), HOLE_RADIUS, 2)
        draw_bolinhas(x, y, board[i + NUM_HOLES + 1])
        text_color = RED if player_turn == 1 else WHITE
        text = small_font.render(str(board[i + NUM_HOLES + 1]), True, text_color)
        SCREEN.blit(text, (x - text.get_width() // 2, y - HOLE_RADIUS - 25))  # Elevar a contagem de bolinhas para o jogador 2

    # Desenhar os armazéns
    # Armazém do jogador 1
    x = WIDTH - MARGIN - HOLE_RADIUS - 20
    y = HEIGHT // 2
    pygame.draw.ellipse(SCREEN, BLUE, (x - HOLE_RADIUS, y - HOLE_RADIUS * 3, HOLE_RADIUS * 2, HOLE_RADIUS * 6))
    pygame.draw.ellipse(SCREEN, BOTTOM, (x - HOLE_RADIUS, y - HOLE_RADIUS * 3, HOLE_RADIUS * 2, HOLE_RADIUS * 6), 2)
    draw_bolinhas(x, y, board[NUM_HOLES], is_store=True)
    text_color_1 = LIGHT_BLUE if player_turn == 0 else WHITE
    text = small_font.render(str(board[NUM_HOLES]), True, text_color_1)
    SCREEN.blit(text, (x - text.get_width() // 2, y + HOLE_RADIUS * 3 + 5))

    # Armazém do jogador 2
    x = MARGIN + HOLE_RADIUS + 20
    y = HEIGHT // 2
    pygame.draw.ellipse(SCREEN, BLUE, (x - HOLE_RADIUS, y - HOLE_RADIUS * 3, HOLE_RADIUS * 2, HOLE_RADIUS * 6))
    pygame.draw.ellipse(SCREEN, BOTTOM, (x - HOLE_RADIUS, y - HOLE_RADIUS * 3, HOLE_RADIUS * 2, HOLE_RADIUS * 6), 2)
    draw_bolinhas(x, y, board[-1], is_store=True)
    text_color_2 = RED if player_turn == 1 else WHITE
    text = small_font.render(str(board[-1]), True, text_color_2)
    SCREEN.blit(text, (x - text.get_width() // 2, y - HOLE_RADIUS * 3 - 15))

    # Desenhar a indicação de vez do jogador
    if capture_self_mode:
        # Inverter a indicação do jogador para CAPTURA CONTRA
        current_player = "Player 2" if player_turn == 0 else "Player 1"
        text_color = RED if player_turn == 0 else LIGHT_BLUE
    else:
        current_player = "Player 1" if player_turn == 0 else "Player 2"
        text_color = LIGHT_BLUE if player_turn == 0 else RED

    # Criar o texto
    text = font.render(current_player, True, text_color)
    text_rect = text.get_rect(center=(WIDTH // 2, 30))

    # Criar uma superfície para o fundo com borda arredondada
    background_surf = pygame.Surface((text_rect.width + 20, text_rect.height + 10), pygame.SRCALPHA)
    pygame.draw.rect(background_surf, (50, 60, 60, 230), background_surf.get_rect(), border_radius=15)

    # Desenhar o fundo e o texto na tela
    SCREEN.blit(background_surf, (text_rect.x - 10, text_rect.y - 5))
    SCREEN.blit(text, text_rect)

    # Desenhar mensagem
    if message:
        msg_text = font.render(message, True, text_color)
        SCREEN.blit(msg_text, (WIDTH // 2 - msg_text.get_width() // 2, HEIGHT // 2 - 100))

    if small_message:
        small_msg_text = tiny_font.render(small_message, True, WHITE)
        SCREEN.blit(small_msg_text, (WIDTH // 2 - small_msg_text.get_width() // 2, HEIGHT // 2 - 50))

    # Desenhar texto de captura
    if capture_index is not None:
        if capture_index < NUM_HOLES:
            x = MARGIN + (capture_index + 1) * HOLE_SPACING
            y = HEIGHT - MARGIN - HOLE_RADIUS - 10
        else:
            x = MARGIN + (2 * NUM_HOLES - capture_index + 1) * HOLE_SPACING
            y = MARGIN + HOLE_RADIUS + 10
        capture_text = font.render("CAPTURE!", True, RED)
        SCREEN.blit(capture_text, (x - capture_text.get_width() // 2, y - HOLE_RADIUS - 45))

def draw_bolinhas(x, y, count, is_store=False):
    bolinha_radius = bolinha_img.get_width() // 2
    distance = bolinha_radius * 1.2
    max_per_row = 4

    if is_store:
        start_x = x - (max_per_row // 2) * distance - distance // 2
        start_y = y - (distance * (count // max_per_row)) / 2 - distance // 2
    else:
        start_x = x - (max_per_row // 2) * distance - distance // 2
        start_y = y - bolinha_radius - distance

    for row in range(count // max_per_row + 1):
        for col in range(min(max_per_row, count - row * max_per_row)):
            offset_x = col * distance
            offset_y = row * distance
            SCREEN.blit(bolinha_img, (start_x + offset_x, start_y + offset_y))

def get_hole_index(pos):
    x, y = pos
    if y > HEIGHT // 2:
        for i in range(NUM_HOLES):
            if abs(x - (MARGIN + (i + 1) * HOLE_SPACING)) < HOLE_RADIUS:
                return i
    else:
        for i in range(NUM_HOLES):
            if abs(x - (MARGIN + (NUM_HOLES - i) * HOLE_SPACING)) < HOLE_RADIUS:
                return i + NUM_HOLES + 1
    return None

def move_seeds(index):
    global board, message, small_message, capture_mode, capture_self_mode
    seeds = board[index]
    board[index] = 0
    last_index = index
    while seeds > 0:
        index = (index + 1) % len(board)
        if player_turn == 0:
            if index == len(board) - 1:
                board[index] += 1
                seeds -= 1
                continue
        else:
            if index == NUM_HOLES:
                board[index] += 1
                seeds -= 1
                continue

        if (player_turn == 0 and index != len(board) - 1) or (player_turn == 1 and index != NUM_HOLES):
            move_seed_animation(index)
            board[index] += 1
            seeds -= 1
            last_index = index

    if board[last_index] == 1:
        opposite_index = len(board) - 2 - last_index
        if (player_turn == 0 and NUM_HOLES + 1 <= last_index < 2 * NUM_HOLES + 1 and board[opposite_index] > 0):
            capture_mode = True
            message = "CAPTURA COMUM!"
            small_message = "Você acertou uma casa vazia do adversário, capturare suas as bolinhas."
        elif (player_turn == 1 and 0 <= last_index < NUM_HOLES and board[opposite_index] > 0):
            capture_mode = True
            message = "CAPTURA COMUM!"
            small_message = "Você acertou uma casa vazia do adversário, capturare suas as bolinhas."
        elif (player_turn == 0 and 0 <= last_index < NUM_HOLES and board[opposite_index] > 0):
            capture_self_mode = True
            message = "CAPTURA CONTRA!"
            small_message = "Você acertou sua própria casa vazia, o adversário irá capturar suas bolinhas."
        elif (player_turn == 1 and NUM_HOLES + 1 <= last_index < 2 * NUM_HOLES + 1 and board[opposite_index] > 0):
            capture_self_mode = True
            message = "CAPTURA CONTRA!"
            small_message = "Você acertou sua própria casa vazia, o adversário irá capturar suas bolinhas."
        else:
            capture_mode = False
            capture_self_mode = False

    return last_index

def move_seed_animation(to_index):
    global board
    from_pos = get_hole_position(to_index - 1)
    to_pos = get_hole_position(to_index)
    steps = 20
    for step in range(steps):
        t = step / steps
        x = from_pos[0] * (1 - t) + to_pos[0] * t
        y = from_pos[1] * (1 - t) + to_pos[1] * t
        draw_board()
        SCREEN.blit(bolinha_img, (x - bolinha_img.get_width() // 2, y - bolinha_img.get_height() // 2))
        pygame.display.flip()
        pygame.time.wait(20)

def get_hole_position(index):
    if index < NUM_HOLES:
        x = MARGIN + (index + 1) * HOLE_SPACING
        y = HEIGHT - MARGIN - HOLE_RADIUS - 10
    elif index == NUM_HOLES:
        x = WIDTH - MARGIN - HOLE_RADIUS - 20
        y = HEIGHT // 2
    elif index == len(board) - 1:
        x = MARGIN + HOLE_RADIUS + 20
        y = HEIGHT // 2
    else:
        x = MARGIN + (2 * NUM_HOLES - index + 1) * HOLE_SPACING
        y = MARGIN + HOLE_RADIUS + 10
    return (x, y)

def capture_seeds(index):
    global board, player_turn, message, small_message
    if player_turn == 0:
        opposite_index = len(board) - 2 - index
        if board[index] == 1 and board[opposite_index] > 0:
            board[NUM_HOLES] += board[index] + board[opposite_index]
            board[index] = 0
            board[opposite_index] = 0
            return True
    else:
        opposite_index = len(board) - 2 - index
        if board[index] == 1 and board[opposite_index] > 0:
            board[-1] += board[index] + board[opposite_index]
            board[index] = 0
            board[opposite_index] = 0
            return True
    return False

def valid_move(index):
    global player_turn
    if player_turn == 0 and 0 <= index < NUM_HOLES:
        return True
    elif player_turn == 1 and NUM_HOLES + 1 <= index < 2 * NUM_HOLES + 1:
        return True
    return False

def check_game_end():
    global board, message, small_message
    if sum(board[:NUM_HOLES]) == 0 or sum(board[NUM_HOLES+1:-1]) == 0:
        board[NUM_HOLES] += sum(board[:NUM_HOLES])
        board[-1] += sum(board[NUM_HOLES+1:-1])
        for i in range(NUM_HOLES):
            board[i] = 0
            board[NUM_HOLES + 1 + i] = 0
        if board[NUM_HOLES] > board[-1]:
            message = "PLAYER 1 VENCEU!"
        elif board[NUM_HOLES] < board[-1]:
            message = "PLAYER 2 VENCEU!"
        else:
            message = "EMPATE!"
        small_message = ""
        return True
    return False

# Loop principal do jogo
running = True
capture_mode = False
capture_self_mode = False
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                running = False
        elif event.type == pygame.MOUSEBUTTONDOWN:
            pos = pygame.mouse.get_pos()
            if capture_mode or capture_self_mode:
                index = get_hole_index(pos)
                if index is not None:
                    if capture_mode and (player_turn == 0 and NUM_HOLES + 1 <= index < 2 * NUM_HOLES + 1 or player_turn == 1 and 0 <= index < NUM_HOLES):
                        if player_turn == 0:
                            board[NUM_HOLES] += board[index]
                        else:
                            board[-1] += board[index]
                        board[index] = 0
                    elif capture_self_mode and ((player_turn == 0 and 0 <= index < NUM_HOLES) or (player_turn == 1 and NUM_HOLES + 1 <= index < 2 * NUM_HOLES + 1)):
                        if player_turn == 0:
                            board[-1] += board[index]
                        else:
                            board[NUM_HOLES] += board[index]
                        board[index] = 0
                    capture_mode = False
                    capture_self_mode = False
                    player_turn = (player_turn + 1) % 2
                    message = ""
                    small_message = ""
            else:
                index = get_hole_index(pos)
                if index is not None and valid_move(index):
                    final_index = move_seeds(index)
                    if final_index == (NUM_HOLES if player_turn == 0 else len(board) - 1):
                        message = "DUPLA JOGADA!"
                        small_message = "Duplas jogadas ocorrem quando a última semente cai no seu armazém."
                    else:
                        if board[final_index] == 1 and ((player_turn == 0 and NUM_HOLES <= final_index < 2 * NUM_HOLES + 1) or (player_turn == 1 and 0 <= final_index < NUM_HOLES)):
                            capture_mode = True
                            message = "CAPTURA COMUM!"
                            small_message = "Você pode capturar as bolinhas do adversário quando acertar uma casa vazia dele."
                        elif board[final_index] == 1 and ((player_turn == 0 and 0 <= final_index < NUM_HOLES) or (player_turn == 1 and NUM_HOLES + 1 <= final_index < 2 * NUM_HOLES + 1)):
                            capture_self_mode = True
                            message = "CAPTURA CONTRA!"
                            small_message = "Você acertou sua própria casa, o adversário pode capturar suas bolinhas."
                        else:
                            message = ""
                            small_message = ""
                            player_turn = (player_turn + 1) % 2
                            capture_mode = False
                            capture_self_mode = False
                    if check_game_end():
                        capture_mode = False
                        capture_self_mode = False

    draw_board()
    pygame.display.flip()

pygame.quit()
sys.exit()