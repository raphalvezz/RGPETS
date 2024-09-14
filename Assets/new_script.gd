extends Node2D

# Sinal para indicar quando um movimento é completado
signal completed

const NUM_HOLES = 6  # Número de buracos por jogador, sem contar os armazéns

# Inicialização das sementes em cada buraco
var board = [4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 0]  # 6 buracos + 1 armazém para cada jogador

var player_turn = true  # Controla a vez de cada jogador

func _ready():
	# Conecta o sinal completed deste node a uma função chamada _on_Completed
	call_deferred("connect_holes")

	# Configurações iniciais do jogo
	reset_board()
	update_board_display()

# Reseta o tabuleiro para o estado inicial
func reset_board():
	board = [4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 0]

# Atualiza a exibição do tabuleiro com base no estado atual do 'board'
func update_board_display():
	for i in range(1, NUM_HOLES * 2 + 1):  # Ajuste similar ao connect_holes()
		var hole_path = "Board/Hole" + str(i)
		for j in range(1, 5):  # Acessar as 4 sprites dentro de cada Hole
			var sprite_path = hole_path + "/Sprite2D" + str(j)
			print("Procurando node:", sprite_path)  # Debug
			if has_node(sprite_path):
				var sprite = get_node(sprite_path)
				sprite.texture = preload("res://bolinha.png")
				if j <= board[i - 1]:  # Mostrar apenas o número correto de bolinhas
					sprite.visible = true
				else:
					sprite.visible = false
			else:
				print("Caminho do node não encontrado: ", sprite_path)
	
	# Atualiza os armazéns
	for j in range(2):
		var storage_path = "Board/Hole-maior" + (str(j + 1) if j == 1 else "")
		print("Procurando armazém:", storage_path)  # Debug
		if has_node(storage_path):
			for k in range(1, 5):  # Acessar as 4 sprites dentro do armazém
				var sprite_path = storage_path + "/Sprite2D" + str(k)
				if has_node(sprite_path):
					var sprite = get_node(sprite_path)
					sprite.texture = preload("res://bolinha.png")
					if k <= board[NUM_HOLES + j * (NUM_HOLES + 1)]:  # Ajusta para os armazéns
						sprite.visible = true
					else:
						sprite.visible = false
				else:
					print("Sprite2D não encontrado em: ", sprite_path)
		else:
			print("Caminho do node não encontrado: ", storage_path)

# Conecta eventos de entrada para todos os buracos
func connect_holes():
	for i in range(1, NUM_HOLES * 2 + 1):
		var hole_path = "Board/Hole" + str(i)
		print("Conectando evento para:", hole_path)  # Debug
		if has_node(hole_path):
			var hole = get_node(hole_path)
			hole.connect("input_event", Callable(self, "_on_Hole_input_event").bind(i))
		else:
			print("Caminho do node não encontrado: ", hole_path)

	# Conecta os armazéns
	for j in range(2):
		var storage_path = "Board/Hole-maior" + (str(j + 1) if j == 1 else "")
		print("Conectando evento para armazém:", storage_path)  # Debug
		if has_node(storage_path):
			var storage = get_node(storage_path)
			storage.connect("input_event", Callable(self, "_on_Hole_input_event").bind(NUM_HOLES + j * (NUM_HOLES + 1)))
		else:
			print("Caminho do node não encontrado: ", storage_path)

# Manipula o clique no buraco
func _on_Hole_input_event(_viewport, event, _shape_idx, index):
	if event is InputEventMouseButton and event.pressed:
		process_hole_click(index)

# Processa o clique no buraco
func process_hole_click(index):
	if (player_turn and index <= NUM_HOLES) or (!player_turn and index > NUM_HOLES and index < 2 * NUM_HOLES + 2):
		var seeds = board[index - 1]  # Ajusta o índice para o array
		board[index - 1] = 0  # Remove as sementes do buraco clicado
		var current_index = index

		while seeds > 0:
			current_index = (current_index + 1) % (NUM_HOLES * 2 + 2)
			if current_index != index:  # Evita mover a semente de volta para o buraco original
				board[current_index - 1] += 1  # Adiciona uma semente ao próximo buraco
				move_bolinha(index, current_index)
				seeds -= 1
				await get_tree().create_timer(0.5).timeout  # Adiciona um pequeno atraso entre movimentos

		update_board_display()
		player_turn = !player_turn  # Alterna turnos
		emit_signal("completed")  # Emite o sinal ao completar um movimento

# Função chamada quando o sinal completed é emitido
func _on_Completed():
	print("Movimento completo!")

# Função para visualizar a movimentação de uma bolinha de um buraco para outro
func move_bolinha(from_index, to_index):
	var from_hole_path = "Board/Hole" + str(from_index) + "/Sprite2D"
	var to_hole_path = "Board/Hole" + str(to_index) + "/Sprite2D"
	print("Movendo bolinha de:", from_hole_path, " para:", to_hole_path)  # Debug
	if has_node(from_hole_path) and has_node(to_hole_path):
		var from_hole = get_node(from_hole_path)
		var to_hole = get_node(to_hole_path)
		var bolinha = from_hole.duplicate()
		add_child(bolinha)
		bolinha.global_position = from_hole.global_position

		var tween = Tween.new()
		add_child(tween)
		tween.interpolate_property(bolinha, "global_position", bolinha.global_position, to_hole.global_position, 0.5, Tween.TRANS_LINEAR, Tween.EASE_IN_OUT)
		tween.start()
		await tween.tween_completed
		bolinha.queue_free()
	else:
		print("Erro: Node de origem ou destino não encontrado")
