extends Node3D

var mutex
var thread: Thread
var map_to_update = false
var real_delta = 0

var owned_character

var current_chunks = {}
var next_chunks = {}
var next_chunk_ids = []

var characters = {}
var projectiles = {}
var furnitures = {}
var characters_data = {}
var projectiles_data = {}
var materials = {}

var blocks_img = {}

var board_material = preload("res://resources/materials/board_material.tres")

var base_character = preload("res://models/character.tscn")
var base_projectile = preload("res://models/projectile.tscn")
var base_chunk = preload("res://world/chunk.tscn")

@onready var n_hud = $HUD
@onready var n_inventory = n_hud.inventory
@onready var n_sun = $WorldEnvironment/Sun
@onready var n_characters = $Mobs/Characters
@onready var n_projectiles = $Mobs/Projectiles
@onready var n_chunks = $Chunks

# Called when the node enters the scene tree for the first time.
func _ready():
	mutex = Mutex.new()
	thread = Thread.new()
	Values.chunk_size = Values.macros["CHUNK_SIZE"]
	Values.link.getState()
	owned_character = Values.link.getPlayerId()
	characters_data = Values.link.getCharacters()
	projectiles_data = Values.link.getProjectiles()
	for character_id in characters_data:
		add_character(character_id, characters_data[character_id])
	for projectile_id in projectiles_data:
		add_projectile(projectile_id, projectiles_data[projectile_id])
	
	# no need to launch a thread for the initialization
	process_new_map()
	update_map()

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	if mutex.try_lock():
		if map_to_update:
			update_map()
		Values.action_mutex.lock()
		while Values.link.hasState():
			Values.link.getState()
			Values.next_state_ready = true
			Values.move_set = false
		Values.action_mutex.unlock()
		if Values.next_state_ready:
			# set sky light
			n_sun.set_param(Light3D.PARAM_ENERGY, 2.5 * float(Values.link.getBaseLight()) / float(Values.link.getMaxLight()))
			var next_characters_data = Values.link.getCharacters()
			n_hud.update(next_characters_data[owned_character])
			for character_id in characters_data.keys():
				if not next_characters_data.has(character_id):
					n_characters.remove_child(characters[character_id])
					characters.erase(character_id)
			var update = false
			if floor(next_characters_data[owned_character]["coord"] / Values.chunk_size) != floor(characters_data[owned_character]["coord"] / Values.chunk_size):
				update = true
			for character_id in next_characters_data:
				if not characters_data.has(character_id):
					add_character(character_id, next_characters_data[character_id])
				var character = characters[character_id]
				var pos = Vector3(next_characters_data[character_id]["coord"].y, next_characters_data[character_id]["coord"].z + next_characters_data[character_id]["size"].z / 2., next_characters_data[character_id]["coord"].x)
				if character.global_position.distance_to(pos) >= 0.5:
					character.global_position = pos
			characters_data = next_characters_data
			var next_projectiles_data = Values.link.getProjectiles()
			for projectile_id in projectiles_data.keys():
				if not next_projectiles_data.has(projectile_id):
					n_projectiles.remove_child(projectiles[projectile_id])
					projectiles.erase(projectile_id)
			for projectile_id in next_projectiles_data:
				if not projectiles_data.has(projectile_id):
					add_projectile(projectile_id, next_projectiles_data[projectile_id])
			projectiles_data = next_projectiles_data
			Values.next_state_ready = false
			n_inventory.update_inventory()
			update_furnitures()
			mutex.unlock()
			# costly operations out of mutex in another thread
			if update:
				if thread.is_started():
					thread.wait_to_finish()
					if map_to_update:
						update_map()
				thread.start(process_new_map)
		else:
			mutex.unlock()

func process_new_map():
	create_blocks()
	mutex.lock()
	map_to_update = true
	mutex.unlock()

func update_map():
	display_map()
	next_chunks = {}
	map_to_update = false

func _physics_process(delta):
	real_delta += delta
	if mutex.try_lock():
		for character_id in characters_data:
			var character = characters[character_id]
			var speed = Vector3(characters_data[character_id]["speed"].y, characters_data[character_id]["speed"].z, characters_data[character_id]["speed"].x)
			character.move(speed, real_delta)
		#for projectile_id in projectiles_data:
		#	var dest = Vector3(projectiles_data[projectile_id]["y"], projectiles[projectile_id].transform.origin.y, projectiles_data[projectile_id]["x"])
		#	var movement = (dest - projectiles[projectile_id].transform.origin).normalized() * 10
		#	projectiles[projectile_id].move_and_collide(Vector3(movement.x, 0, movement.z) * delta)
		#	projectiles[projectile_id].rotation_degrees = Vector3(0, projectiles_data[projectile_id]["orientation_z"], 0)
		#	if projectiles[projectile_id].transform.origin.distance_to(dest) < 0.1:
		#		projectiles[projectile_id].transform.origin = dest
		#	done = done and (projectiles[projectile_id].transform.origin == dest)
		real_delta = 0
	mutex.unlock()

func create_chunk(coord: Vector3):
	var chunk = base_chunk.instantiate()
	chunk.name = "Chunk_" + String.num(coord.x) + "_" + String.num(coord.y) + "_" + String.num(coord.z)
	var current_blocks = Values.link.getBlocks(coord) 
	for block_coord in current_blocks:
		chunk.add_block(block_coord, current_blocks)
	chunk.display_chunk(Values.link.getFurnitures(coord))
	next_chunks[coord] = chunk

func create_blocks():
	next_chunk_ids = Values.link.getChunkIds(Settings.chunk_height, Settings.chunk_radius)
	for chunk_coord in next_chunk_ids:
		if not current_chunks.has(chunk_coord):
			create_chunk(chunk_coord)

func update_furnitures():
	var updated_furnitures = Values.link.getUpdatedFurnitures()
	if not updated_furnitures.is_empty():
		for coord in updated_furnitures:
			current_chunks[coord - coord.posmod(Values.chunk_size)].add_furniture(updated_furnitures[coord], coord)

func display_map():
	for chunk_id in current_chunks.keys():
		if not next_chunk_ids.has(chunk_id):
			n_chunks.remove_child(current_chunks[chunk_id])
			current_chunks.erase(chunk_id)
	for chunk_id in next_chunk_ids:
		if not current_chunks.has(chunk_id):
			n_chunks.add_child(next_chunks[chunk_id])
			current_chunks[chunk_id] = next_chunks[chunk_id]
	next_chunks.clear()

func add_character(character_id: int, character_data: Dictionary):
	var character = base_character.instantiate()
	character.transform.origin = Vector3(character_data["coord"].x, character_data["coord"].z + character_data["size"].z / 2., character_data["coord"].x)
	character.rotation_degrees = Vector3(0, character_data["orientation"].z, character_data["orientation"].x - 90.)
	characters[character_id] = character
	character.id = character_id
	character.character = character_data["name"]
	n_characters.add_child(character)
	#character.scale_object_local(Vector3(character_data["size"].y, character_data["size"].z, character_data["size"].x))
	character.shape.shape.set_size(Vector3(character_data["size"].y, character_data["size"].z, character_data["size"].x))
	character.mesh_debug.mesh.set_size(Vector3(character_data["size"].y, character_data["size"].z, character_data["size"].x))
	if owned_character == character_id:
		character.visible = false

func add_projectile(projectile_id: int, projectile_data: Dictionary):
	var projectile = base_projectile.instantiate()
	projectile.scale_object_local(Vector3(projectile_data["sizeY"], projectile_data["sizeZ"], projectile_data["sizeX"]))
	projectile.transform.origin = Vector3(projectile_data["y"], projectile_data["z"] + 1, projectile_data["x"])
	projectile.rotation_degrees = Vector3(0, projectile_data["orientation_z"], projectile_data["orientation_x"])
	projectiles[projectile_id] = projectile
	projectile.id = projectile_id
	projectile.projectile = projectile_data["name"]
	n_projectiles.add_child(projectile)

func round_float(number: float):
	var value = int(number * 1000. + 0.5)
	return float(value) / 1000.

func round_vec(vec: Vector3):
	return Vector3(round_float(vec.x), round_float(vec.y), round_float(vec.z))
