extends Node3D

var mutex
var thread: Thread
var map_to_update = false
var real_delta = 0

var owned_character

var current_blocks = {}
var blocks_count = {}

var colliders = {}
var characters = {}
var projectiles = {}
var furnitures = {}
var blocks_data = {}
var characters_data = {}
var projectiles_data = {}
var furnitures_data = {}
var base_furnitures = {}
var base_furnitures_off = {}
var materials = {}

var blocks_img = {}

var board_material = preload("res://resources/materials/board_material.tres")
var base_fire = preload("res://models/fire.tscn")

var base_character = preload("res://models/character.tscn")
var base_projectile = preload("res://models/projectile.tscn")

# We can't add or remove child directly on the Tree
# So we will constently juggle between 2 Map Nodes
# Only one will be on the Root at a time
var next_n_map : Node3D = null
var next_n_colliders : Node3D = null
var next_n_furnitures : Node3D = null
var next_colliders = {}
var next_furnitures = {}
var next_multi_mesh_instances = {}

var next_map_1 : bool
var n_map_1 : Node3D = Node3D.new()
var n_blocks_1 : Node3D = Node3D.new()
var n_colliders_1 : Node3D = Node3D.new()
var n_furnitures_1 : Node3D = Node3D.new()
var colliders_1 = {}
var furnitures_1 = {}
var multi_mesh_instances_1 = {}

var n_map_2 : Node3D = Node3D.new()
var n_blocks_2 : Node3D = Node3D.new()
var n_colliders_2 : Node3D = Node3D.new()
var n_furnitures_2 : Node3D = Node3D.new()
var colliders_2 = {}
var furnitures_2 = {}
var multi_mesh_instances_2 = {}
#

@onready var n_hud = $HUD
@onready var n_inventory = n_hud.inventory
@onready var n_sun = $WorldEnvironment/Sun
@onready var n_characters = $Mobs/Characters
@onready var n_projectiles = $Mobs/Projectiles

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
	for block in Values.link.getAvaillableBlocks():
		initialize_block(block)
	
	# initialize map juggling
	n_map_1.set_name("Map1")
	n_blocks_1.set_name("Blocks")
	n_colliders_1.set_name("Colliders")
	n_furnitures_1.set_name("Furnitures")
	n_map_1.add_child(n_blocks_1)
	n_map_1.add_child(n_colliders_1)
	n_map_1.add_child(n_furnitures_1)
	
	n_map_2.set_name("Map2")
	n_blocks_2.set_name("Blocks")
	n_colliders_2.set_name("Colliders")
	n_furnitures_2.set_name("Furnitures")
	n_map_2.add_child(n_blocks_2)
	n_map_2.add_child(n_colliders_2)
	n_map_2.add_child(n_furnitures_2)
	
	add_child(n_map_1)
	next_map_1 = false
	next_n_map = n_map_2
	next_n_colliders = n_colliders_2
	next_n_furnitures = n_furnitures_2
	next_furnitures = furnitures_2
	next_colliders = colliders_2
	next_multi_mesh_instances = multi_mesh_instances_2
	
	# no need to launch a thread for the initialization
	process_new_map()
	update_map()
	n_hud.update(characters_data[owned_character])
	n_inventory.update_inventory()
	n_sun.set_param(Light3D.PARAM_ENERGY, 2.5 * float(Values.link.getBaseLight()) / float(Values.link.getMaxLight()))

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	if mutex.try_lock():
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
	# outside of try_lock
	if map_to_update:
		update_map()

func process_new_map():
	create_blocks()
	furnitures_data = Values.link.getFurnitures()
	display_map()
	mutex.lock()
	map_to_update = true
	mutex.unlock()

func update_map():
	mutex.lock()
	add_child(next_n_map)
	if next_map_1:
		remove_child(get_node("Map2"))
	else:
		remove_child(get_node("Map1"))
	furnitures = next_furnitures
	colliders = next_colliders
	next_map_1 = not next_map_1
	if next_map_1:
		next_n_map = n_map_1
		next_n_colliders = n_colliders_1
		next_n_furnitures = n_furnitures_1
		next_furnitures = furnitures_1
		next_colliders = colliders_1
		next_multi_mesh_instances = multi_mesh_instances_1
	else:
		next_n_map = n_map_2
		next_n_colliders = n_colliders_2
		next_n_furnitures = n_furnitures_2
		next_furnitures = furnitures_2
		next_colliders = colliders_2
		next_multi_mesh_instances = multi_mesh_instances_2
	map_to_update = false
	reset_furnitures_and_colliders()
	mutex.unlock()

func _physics_process(delta):
	real_delta += delta
	#if mutex.try_lock():
	mutex.lock()
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

func create_blocks():
	# reset blocks
	for block_type in next_multi_mesh_instances:
		blocks_count[block_type] = 0
	current_blocks = Values.link.getBlocks(Settings.chunk_height, Settings.chunk_radius)
	for coord in current_blocks:
		add_block(coord)

func add_block(coord: Vector3):
	if current_blocks[coord] == "TXT_MIST" or current_blocks[coord] == "TXT_VOID":
		return
	blocks_count[current_blocks[coord]] = blocks_count[current_blocks[coord]] + 1
	var collider = StaticBody3D.new()
	collider.collision_mask = 0x0
	collider.set_transform(Transform3D.IDENTITY.translated(Vector3(coord.x + 0.5, coord.y + 0.5, coord.z + 0.5)))
	var shape = CollisionShape3D.new()
	shape.name = "Shape"
	if blocks_data[current_blocks[coord]]["type"] == Values.macros["BLOCK_SOLID"]:
		collider.collision_layer = 0x11
		shape.shape = BoxShape3D.new()
	elif blocks_data[current_blocks[coord]]["type"] == Values.macros["BLOCK_LIQUID"]:
		collider.collision_layer = 0x1
		shape.shape = BoxShape3D.new()
	elif blocks_data[current_blocks[coord]]["type"] == Values.macros["BLOCK_SLOPE"]:
		collider.collision_layer = 0x21
		shape.shape = ConvexPolygonShape3D.new()
		shape.shape.set_points(
			PackedVector3Array(
				[
					Vector3(-0.5, 0.5, 0.5), Vector3(0.5, -0.5, 0.5), Vector3(-0.5, 0.5, -0.5),
					Vector3(-0.5, -0.5, 0.5), Vector3(0.5, -0.5, -0.5), Vector3(-0.5, -0.5, -0.5)
				]
			)
		)
	elif blocks_data[current_blocks[coord]]["type"] == Values.macros["BLOCK_STAIRS"]:
		collider.collision_layer = 0x21
		shape.shape = ConvexPolygonShape3D.new()
		shape.shape.set_points(
			PackedVector3Array(
				[
					Vector3(-0.5, 0.5, 0.5), Vector3(0.5, -0.5, 0.5), Vector3(-0.5, 0.5, -0.5),
					Vector3(-0.5, -0.5, 0.5), Vector3(0.5, -0.5, -0.5), Vector3(-0.5, -0.5, -0.5)
				]
			)
		)
	shape.rotation_degrees = Vector3(0, 90 + blocks_data[current_blocks[coord]]["orientation_z"], 0)
	collider.add_child(shape)
	next_n_colliders.add_child(collider)
	next_colliders[coord] = collider

func reset_furnitures_and_colliders():
	for coord in next_furnitures.keys():
		next_n_furnitures.remove_child(next_furnitures[coord])
		next_furnitures.erase(coord)
	for coord in next_colliders.keys():
		next_n_colliders.remove_child(next_colliders[coord])
		next_colliders.erase(coord)

func update_furnitures():
	var n_furnitures : Node3D
	var n_colliders : Node3D
	if next_map_1:
		n_furnitures = get_node("Map2/Furnitures")
		n_colliders = get_node("Map2/Colliders")
	else:
		n_furnitures = get_node("Map1/Furnitures")
		n_colliders = get_node("Map1/Colliders")
	var updated_furnitures = Values.link.getUpdatedFurnitures()
	if not updated_furnitures.is_empty():
		var new_furnitures_data = Values.link.getFurnitures()
		for coord in updated_furnitures:
			n_furnitures.remove_child(furnitures[coord])
			furnitures.erase(coord)
			if next_furnitures.has(coord):
				next_n_furnitures.remove_child(next_furnitures[coord])
				next_furnitures.erase(coord)
			n_colliders.remove_child(colliders[coord])
			colliders.erase(coord)
			if next_colliders.has(coord):
				next_n_colliders.remove_child(next_colliders[coord])
				next_colliders.erase(coord)
			add_furniture(new_furnitures_data[coord], coord, false)

func display_map():
	var block_current = {}
	for block_type in next_multi_mesh_instances:
		next_multi_mesh_instances[block_type].multimesh.set_instance_count(blocks_count[block_type])
		blocks_count[block_type] = 0
		block_current[block_type] = 0
	for block in current_blocks:
		var block_type = current_blocks[block]
		var coord = Vector3.ZERO
		var angle_offset
		if blocks_data[block_type]["type"] == Values.macros["BLOCK_SOLID"]:
			coord = Vector3(block.x + 0.5, block.y + 0.5, block.z + 0.5)
			angle_offset = 90
		elif blocks_data[block_type]["type"] == Values.macros["BLOCK_LIQUID"]:
			coord = Vector3(block.x + 0.5, block.y + 0.8, block.z + 0.5)
			angle_offset = 90
		elif blocks_data[block_type]["type"] == Values.macros["BLOCK_SLOPE"]:
			coord = Vector3(block.x + 0.5, block.y + 0.5, block.z + 0.5)
			angle_offset = 90
		elif blocks_data[block_type]["type"] == Values.macros["BLOCK_STAIRS"]:
			coord = Vector3(block.x + 0.5, block.y + 0.5, block.z + 0.5)
			angle_offset = 180
		next_multi_mesh_instances[block_type].multimesh.set_instance_transform(block_current[block_type], Transform3D.IDENTITY.translated(coord).rotated_local(Vector3.UP, deg_to_rad(angle_offset + blocks_data[block_type]["orientation_z"])))
		block_current[block_type] = block_current[block_type] + 1
	for coord in furnitures_data:
		add_furniture(furnitures_data[coord], coord, true)

func initialize_block(block: String):
	if block != "TXT_MIST" and block != "TXT_VOID":
		blocks_data[block] = Values.link.getDataFromBlock(block)
		blocks_count[block] = 0
		materials[block] = load(blocks_data[block]["material"])
		var multimeshinstance = MultiMeshInstance3D.new()
		var multimesh = MultiMesh.new()
		var mesh
		if blocks_data[block]["type"] == Values.macros["BLOCK_SOLID"]:
			mesh = BoxMesh.new()
			mesh.set_size(Vector3.ONE)
		elif blocks_data[block]["type"] == Values.macros["BLOCK_LIQUID"]:
			mesh = PlaneMesh.new()
			mesh.orientation = PlaneMesh.FACE_Y
			mesh.set_size(Vector2(1, 1))
		elif blocks_data[block]["type"] == Values.macros["BLOCK_SLOPE"]:
			mesh = PrismMesh.new()
			mesh.left_to_right = 0
		elif blocks_data[block]["type"] == Values.macros["BLOCK_STAIRS"]:
			mesh = ArrayMesh.new()
			var vertices = PackedVector3Array(
					[
						# front bottom
						Vector3(-0.5, 0, 0), Vector3(0.5, 0, 0),#0-1
						Vector3(-0.5, 0, 0.25), Vector3(0.5, 0, 0.25),#2-3
						Vector3(-0.5, -0.25, 0.25), Vector3(0.5, -0.25, 0.25),#4-5
						Vector3(-0.5, -0.25, 0.5), Vector3(0.5, -0.25, 0.5),#6-7
						
						# x3 for UV
						Vector3(-0.5, -0.5, 0.5), Vector3(0.5, -0.5, 0.5),#8-9
						Vector3(-0.5, -0.5, 0.5), Vector3(0.5, -0.5, 0.5),#10-11
						Vector3(-0.5, -0.5, 0.5), Vector3(0.5, -0.5, 0.5),#12-13
						
						# back
						# x3 for UV
						Vector3(-0.5, -0.5, -0.5), Vector3(0.5, -0.5, -0.5),#14-15
						Vector3(-0.5, -0.5, -0.5), Vector3(0.5, -0.5, -0.5),#16-17
						Vector3(-0.5, -0.5, -0.5), Vector3(0.5, -0.5, -0.5),#18-19
						
						# x2 for UV
						Vector3(-0.5, 0.5, -0.5), Vector3(0.5, 0.5, -0.5),#20-21
						Vector3(-0.5, 0.5, -0.5), Vector3(0.5, 0.5, -0.5),#22-23
						
						#front up
						Vector3(-0.5, 0.5, -0.25), Vector3(0.5, 0.5, -0.25),#24-25
						Vector3(-0.5, 0.25, -0.25), Vector3(0.5, 0.25, -0.25),#26-27
						Vector3(-0.5, 0.25, 0), Vector3(0.5, 0.25, 0),#28-29
						Vector3(-0.5, 0, 0), Vector3(0.5, 0, 0),#30-31
						
						#side right
						Vector3(-0.5, -0.25, -0.5), Vector3(-0.5, -0.25, 0.25), Vector3(-0.5, -0.25, 0.5), #32-33-34
						Vector3(-0.5, 0, -0.5), Vector3(-0.5, 0, 0), Vector3(-0.5, 0, 0.25), #35-36-37
						Vector3(-0.5, 0.25, -0.5), Vector3(-0.5, 0.25, -0.25), Vector3(-0.5, 0.25, 0), #38-39-40
						Vector3(-0.5, 0.5, -0.5), Vector3(-0.5, 0.5, -0.25), #41-42
						
						#side left
						Vector3(0.5, -0.25, -0.5), Vector3(0.5, -0.25, 0.25), Vector3(0.5, -0.25, 0.5), #43-44-45
						Vector3(0.5, 0, -0.5), Vector3(0.5, 0, 0), Vector3(0.5, 0, 0.25), #46-47-48
						Vector3(0.5, 0.25, -0.5), Vector3(0.5, 0.25, -0.25), Vector3(0.5, 0.25, 0), #49-50-51
						Vector3(0.5, 0.5, -0.5), Vector3(0.5, 0.5, -0.25), #52-53
					]
			)
			var indexes = PackedInt32Array(
					[
						# front bottom
						0,1,2, 1,3,2,
						2,3,4, 3,5,4,
						4,5,6, 5,7,6,
						6,7,8, 7,9,8,
						
						# bottom
						10,11,14, 11,15,14,
						
						# back
						16,17,20, 17,21,20,
						
						# front top
						22,23,24, 23,25,24,
						24,25,26, 25,27,26,
						26,27,28, 27,29,28,
						28,29,30, 29,31,30,
						
						# side right
						18,32,12, 12,32,34,
						32,35,33, 33,35,37,
						35,38,36, 36,38,40,
						38,41,39, 39,41,42,
						
						# side left
						19,13,43, 13,45,43,
						43,44,46, 44,48,46,
						46,47,49, 47,51,49,
						49,50,52, 50,53,52,
					]
			)
			# our FRONT is x axis instead of z, so FRONT is LEFT
			var normals = PackedVector3Array(
					[
						# front bottom
						(Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(), (Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(),#0-1
						(Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(), (Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(),#2-3
						(Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(), (Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(),#4-5
						(Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(), (Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(),#6-7
						
						# x3 for UV
						Vector3.MODEL_FRONT, Vector3.MODEL_FRONT,#8-9
						Vector3.MODEL_BOTTOM, Vector3.MODEL_BOTTOM,#10-11
						Vector3.MODEL_RIGHT, Vector3.MODEL_LEFT,#12-13
						
						# back
						# x3 for UV
						Vector3.MODEL_BOTTOM, Vector3.MODEL_BOTTOM,#14-15
						Vector3.MODEL_REAR, Vector3.MODEL_REAR,#16-17
						Vector3.MODEL_RIGHT, Vector3.MODEL_LEFT,#18-19
						
						# x2 for UV
						Vector3.MODEL_REAR, Vector3.MODEL_REAR,#20-21
						(Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(), (Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(),#22-23
						
						#front up
						(Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(), (Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(),#24-25
						(Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(), (Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(),#26-27
						(Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(), (Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(),#28-29
						(Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(), (Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(),#30-31
						
						#side right
						Vector3.MODEL_RIGHT, Vector3.MODEL_RIGHT, (Vector3.MODEL_RIGHT + Vector3.MODEL_TOP).normalized(), #32-33-34
						Vector3.MODEL_RIGHT, Vector3.MODEL_RIGHT, (Vector3.MODEL_RIGHT + Vector3.MODEL_TOP).normalized(), #35-36-37
						Vector3.MODEL_RIGHT, Vector3.MODEL_RIGHT, (Vector3.MODEL_RIGHT + Vector3.MODEL_TOP).normalized(), #38-39-40
						Vector3.MODEL_RIGHT, (Vector3.MODEL_RIGHT + Vector3.MODEL_TOP).normalized(), #41-42
						
						#side left
						Vector3.MODEL_LEFT, Vector3.MODEL_LEFT, (Vector3.MODEL_LEFT + Vector3.MODEL_TOP).normalized(), #43-44-45
						Vector3.MODEL_LEFT, Vector3.MODEL_LEFT, (Vector3.MODEL_LEFT + Vector3.MODEL_TOP).normalized(), #46-47-48
						Vector3.MODEL_LEFT, Vector3.MODEL_LEFT, (Vector3.MODEL_LEFT + Vector3.MODEL_TOP).normalized(), #49-50-51
						Vector3.MODEL_LEFT, (Vector3.MODEL_LEFT + Vector3.MODEL_TOP).normalized(), #52-53
					]
			)
			var uvs = PackedVector2Array(
					[
						# front bottom
						Vector2(0, 0), Vector2(1.0 / 3.0, 0),#0-1
						Vector2(0, 0.125), Vector2(1.0 / 3.0, 0.125),#2-3
						Vector2(0, 0.25), Vector2(1.0 / 3.0, 0.25),#4-5
						Vector2(0, 0.375), Vector2(1.0 / 3.0, 0.375),#6-7
						
						# x3 for UV
						Vector2(0, 0.5), Vector2(1.0 / 3.0, 0.5),#8-9
						Vector2(0, 0), Vector2(1.0 / 3.0, 0),#10-11
						Vector2(1.0 / 3.0, 0.5), Vector2(1.0 / 3.0, 0.5),#12-13
						
						# back
						# x3 for UV
						Vector2(0, 0.5), Vector2(1.0 / 3.0, 0.5),#14-15
						Vector2(0, 0.5), Vector2(1.0 / 3.0, 0.5),#16-17
						Vector2(0, 0.5), Vector2(0, 0.5),#18-19
						
						# x2 for UV
						Vector2(0, 0), Vector2(1.0 / 3.0, 0),#20-21
						Vector2(0, 0), Vector2(1.0 / 3.0, 0),#22-23
						
						#front up
						Vector2(0, 0.125), Vector2(1.0 / 3.0, 0.125),#24-25
						Vector2(0, 0.25), Vector2(1.0 / 3.0, 0.25),#26-27
						Vector2(0, 0.375), Vector2(1.0 / 3.0, 0.375),#28-29
						Vector2(0, 0.5), Vector2(1.0 / 3.0, 0.5),#30-31
						
						#side left
						Vector2(0, 0.375), Vector2(0.75 / 3.0, 0.375), Vector2(1.0 / 3.0, 0.375), #32-33-34
						Vector2(0, 0.25), Vector2(0.5 / 3.0, 0.25), Vector2(0.75 / 3.0, 0.25), #35-36-37
						Vector2(0, 0.125), Vector2(0.25 / 3.0, 0.125), Vector2(0.5 / 3.0, 0.125), #38-39-40
						Vector2(0, 0), Vector2(0.25 / 3.0, 0), #41-42
						
						#side right
						Vector2(0, 0.375), Vector2(0.75 / 3.0, 0.375), Vector2(1.0 / 3.0, 0.375), #43-44-45
						Vector2(0, 0.25), Vector2(0.5 / 3.0, 0.25), Vector2(0.75 / 3.0, 0.25), #46-47-48
						Vector2(0, 0.125), Vector2(0.25 / 3.0, 0.125), Vector2(0.5 / 3.0, 0.125), #49-50-51
						Vector2(0, 0), Vector2(0.25 / 3.0, 0), #52-53
					]
			)
			var tool = SurfaceTool.new()
			tool.begin(Mesh.PRIMITIVE_TRIANGLES)
			for i in range(vertices.size()):
				tool.set_uv(uvs[i])
				tool.set_normal(normals[i])
				tool.add_vertex(vertices[i])
			for index in indexes:
				tool.add_index(index)
			#tool.generate_normals()
			tool.generate_tangents()
			mesh = tool.commit()
		var material = materials[block]
		if blocks_data[block]["type"] == Values.macros["BLOCK_SOLID"] or blocks_data[block]["type"] == Values.macros["BLOCK_STAIRS"]:
			material.uv1_scale = Vector3(3.0, 2.0, 1.0)
		mesh.surface_set_material(0, material)
		multimesh.set_mesh(mesh)
		multimesh.transform_format = MultiMesh.TRANSFORM_3D
		multimeshinstance.set_multimesh(multimesh)
		multi_mesh_instances_1[block] = multimeshinstance
		n_blocks_1.add_child(multimeshinstance)
		var multimeshinstance_2 = multimeshinstance.duplicate()
		multi_mesh_instances_2[block] = multimeshinstance_2
		n_blocks_2.add_child(multimeshinstance_2)
		
		if blocks_data[block]["type"] != Values.macros["BLOCK_LIQUID"] and blocks_data[block]["type"] != Values.macros["BLOCK_GAS"] and material.albedo_texture:
			var img = material.albedo_texture.get_image()
			blocks_img[block] = ImageTexture.create_from_image(img)

func add_furniture(furniture_data: Dictionary, coord: Vector3, new: bool):
	if not base_furnitures.has(furniture_data["name"]):
		var model = load(furniture_data["path"])
		base_furnitures[furniture_data["name"]] = model
		if furniture_data.has("path_off"):
			if FileAccess.file_exists(furniture_data["path_off"]):
				base_furnitures_off[furniture_data["name"]] = load(furniture_data["path_off"])
			else:
				base_furnitures_off[furniture_data["name"]] = model
	var furniture
	if furniture_data["isOn"]:
		furniture = base_furnitures[furniture_data["name"]].instantiate()
	else:
		furniture = base_furnitures_off[furniture_data["name"]].instantiate()
	
	if furniture_data["light"] > 0:
		furniture.add_child(add_fire(furniture_data["fire_pos"], furniture_data["fire_size"], furniture_data["light"]))
	furniture.transform.origin = Vector3(coord.x + float(furniture_data["size"].y) / 2.0, coord.y + float(furniture_data["size"].z) / 2.0, coord.z + float(furniture_data["size"].x) / 2.0)
	furniture.rotation_degrees = Vector3(0, furniture_data["orientation_z"], 0)
	var collider = StaticBody3D.new()
	
	if furniture_data["solid"]:
		collider.collision_layer = 0x19
	else:
		collider.collision_layer = 0x9
	collider.collision_mask = 0x0
	var shape = CollisionShape3D.new()
	shape.name = "Shape"
	shape.shape = BoxShape3D.new()
	shape.shape.set_size(Vector3(furniture_data["size"].y, furniture_data["size"].z, furniture_data["size"].x))
	collider.transform.origin = furniture.transform.origin
	collider.add_child(shape)
	if new:
		next_furnitures[coord] = furniture
		next_colliders[coord] = collider
		next_n_colliders.add_child(collider)
		next_n_furnitures.add_child(furniture)
	else:
		furnitures[coord] = furniture
		colliders[coord] = collider
		if next_map_1:
			get_node("Map2/Colliders").add_child(collider)
			get_node("Map2/Furnitures").add_child(furniture)
		else:
			get_node("Map1/Colliders").add_child(collider)
			get_node("Map1/Furnitures").add_child(furniture)

func add_fire(coord: Vector3, size: float, light_power: int):
	var fire = base_fire.instantiate()
	fire.initialize(coord, size, light_power)
	return fire

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
