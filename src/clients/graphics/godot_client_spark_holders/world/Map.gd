extends Node3D

var mutex
var timer = 0
var real_delta = 0

var owned_character

var navigations = {}
var multiMeshInstances = {}
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

@onready var n_hud = $"../HUD"
@onready var n_sun = $"../WorldEnvironment/Sun"
@onready var n_blocks = $Blocks
@onready var n_ground = $Blocks/GroundColliders
@onready var n_characters = $Characters
@onready var n_projectiles = $Projectiles
@onready var n_furnitures = $Furnitures

@onready var n_inventory = n_hud.inventory

# Called when the node enters the scene tree for the first time.
func _ready():
	mutex = Mutex.new()
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
	create_blocks()
	
	furnitures_data = Values.link.getFurnitures()
	display_map()
	
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
				if !next_characters_data.has(character_id):
					n_characters.remove_child(characters[character_id])
					characters.erase(character_id)
			var update = false
			if floor(next_characters_data[owned_character]["x"] / Values.chunk_size) != floor(characters_data[owned_character]["x"] / Values.chunk_size) \
				or floor(next_characters_data[owned_character]["y"] / Values.chunk_size) != floor(characters_data[owned_character]["y"] / Values.chunk_size) \
				or floor(next_characters_data[owned_character]["z"] / Values.chunk_size) != floor(characters_data[owned_character]["z"] / Values.chunk_size):
				update = true
			for character_id in next_characters_data:
				if !characters_data.has(character_id):
					add_character(character_id, next_characters_data[character_id])
				var character = characters[character_id]
				var pos = Vector3(next_characters_data[character_id]["y"], next_characters_data[character_id]["z"], next_characters_data[character_id]["x"])
				if character.global_position.distance_to(pos) >= 0.5:
					character.global_position = pos
			characters_data = next_characters_data
			var next_projectiles_data = Values.link.getProjectiles()
			for projectile_id in projectiles_data.keys():
				if !next_projectiles_data.has(projectile_id):
					n_projectiles.remove_child(projectiles[projectile_id])
					projectiles.erase(projectile_id)
			for projectile_id in next_projectiles_data:
				if !projectiles_data.has(projectile_id):
					add_projectile(projectile_id, next_projectiles_data[projectile_id])
			projectiles_data = next_projectiles_data
			Values.next_state_ready = false
			mutex.unlock()
			# costly operations out of mutex
			n_inventory.update_inventory()
			update_furnitures()
			if update:
				create_blocks()
				furnitures_data = Values.link.getFurnitures()
				display_map()
		else:
			mutex.unlock()

func _physics_process(delta):
	real_delta += delta
	timer += delta
	if mutex.try_lock():
		var done = timer >= Values.tickrate / 1000.
		for character_id in characters_data:
			var character = characters[character_id]
			var speed = Vector3(characters_data[character_id]["vy"], characters_data[character_id]["vz"], characters_data[character_id]["vx"])
			character.move(speed, real_delta)
		#for projectile_id in projectiles_data:
		#	var dest = Vector3(projectiles_data[projectile_id]["y"], projectiles[projectile_id].transform.origin.y, projectiles_data[projectile_id]["x"])
		#	var movement = (dest - projectiles[projectile_id].transform.origin).normalized() * 10
		#	projectiles[projectile_id].move_and_collide(Vector3(movement.x, 0, movement.z) * delta)
		#	projectiles[projectile_id].rotation_degrees = Vector3(0, projectiles_data[projectile_id]["orientation_z"], 0)
		#	if projectiles[projectile_id].transform.origin.distance_to(dest) < 0.1:
		#		projectiles[projectile_id].transform.origin = dest
		#	done = done && (projectiles[projectile_id].transform.origin == dest)
		real_delta = 0
		if done:
			timer = 0
		mutex.unlock()
		
func _physics_process_fast(_delta):
	for character_id in characters_data:
		var character = characters[character_id]
		character.global_position = Vector3(characters_data[character_id]["y"], characters_data[character_id]["z"], characters_data[character_id]["x"])

func create_blocks():
	reset_map()
	current_blocks = Values.link.getBlocks(Settings.chunk_height, Settings.chunk_radius)
	for coord in current_blocks:
		add_block(coord)

func add_block(coord: Vector3):
	if current_blocks[coord] == "TXT_MIST" || current_blocks[coord] == "TXT_VOID":
		return
	blocks_count[current_blocks[coord]] = blocks_count[current_blocks[coord]] + 1
	var collider = StaticBody3D.new()
	
	if blocks_data[current_blocks[coord]]["unwalkable"]:
		collider.collision_layer = 0x21
	else:
		collider.collision_layer = 0x11
	collider.collision_mask = 0x0
	collider.set_transform(Transform3D.IDENTITY.translated(Vector3(coord.x + 0.5, coord.y + 0.5, coord.z + 0.5)))
	var shape = CollisionShape3D.new()
	if blocks_data[current_blocks[coord]]["type"] == Values.macros["BLOCK_SOLID"]:
		shape.shape = BoxShape3D.new()
	elif blocks_data[current_blocks[coord]]["type"] == Values.macros["BLOCK_LIQUID"]:
		shape.shape = BoxShape3D.new()
	elif blocks_data[current_blocks[coord]]["type"] == Values.macros["BLOCK_SLOPE"]:
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
	n_ground.add_child(collider)
	colliders[coord] = collider

func reset_map():
	for block_type in multiMeshInstances:
		blocks_count[block_type] = 0
	# clear furnitures
	for coord in furnitures.keys():
		n_furnitures.remove_child(furnitures[coord])
		furnitures.erase(coord)
	for coord in colliders.keys():
		n_ground.remove_child(colliders[coord])
		colliders.erase(coord)

func update_furnitures():
	var updated_furnitures = Values.link.getUpdatedFurnitures()
	if not updated_furnitures.is_empty():
		var new_furnitures_data = Values.link.getFurnitures()
		for coord in updated_furnitures:
			n_furnitures.remove_child(furnitures[coord])
			furnitures.erase(coord)
			n_ground.remove_child(colliders[coord])
			colliders.erase(coord)
			add_furniture(new_furnitures_data[coord], coord)

func display_map():
	var block_current = {}
	for block_type in multiMeshInstances:
		multiMeshInstances[block_type].multimesh.set_instance_count(blocks_count[block_type])
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
		multiMeshInstances[block_type].multimesh.set_instance_transform(block_current[block_type], Transform3D.IDENTITY.translated(coord).rotated_local(Vector3.UP, deg_to_rad(angle_offset + blocks_data[block_type]["orientation_z"])))
		block_current[block_type] = block_current[block_type] + 1
	for coord in furnitures_data:
		add_furniture(furnitures_data[coord], coord)

func initialize_block(block: String):
	if block != "TXT_MIST" && block != "TXT_VOID":
		blocks_data[block] = Values.link.getDataFromBlock(block)
		blocks_count[block] = 0
		materials[block] = load(blocks_data[block]["material"])
		var multimeshinstance = MultiMeshInstance3D.new()
		var multimesh = MultiMesh.new()
		var mesh = BoxMesh.new()
		if blocks_data[block]["type"] == Values.macros["BLOCK_SOLID"]:
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
		if blocks_data[block]["type"] == Values.macros["BLOCK_SOLID"] || blocks_data[block]["type"] == Values.macros["BLOCK_STAIRS"]:
			material.uv1_scale = Vector3(3.0, 2.0, 1.0)
		mesh.surface_set_material(0, material)
		multimesh.set_mesh(mesh)
		multimesh.transform_format = MultiMesh.TRANSFORM_3D
		multimeshinstance.set_multimesh(multimesh)
		multiMeshInstances[block] = multimeshinstance
		n_blocks.add_child(multimeshinstance)
		
		if blocks_data[block]["type"] != Values.macros["BLOCK_LIQUID"] && blocks_data[block]["type"] != Values.macros["BLOCK_GAS"] && material.albedo_texture:
			var img = material.albedo_texture.get_image()
			blocks_img[block] = ImageTexture.create_from_image(img)

func add_furniture(furniture_data: Dictionary, coord: Vector3):
	if !base_furnitures.has(furniture_data["name"]):
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
	furniture.transform.origin = Vector3(coord.x + float(furniture_data["sizeY"]) / 2.0, coord.y + float(furniture_data["sizeZ"]) / 2.0, coord.z + float(furniture_data["sizeX"]) / 2.0)
	furniture.rotation_degrees = Vector3(0, furniture_data["orientation_z"], 0)
	furnitures[coord] = furniture
	var collider = StaticBody3D.new()
	
	if furniture_data["unwalkable"]:
		collider.collision_layer = 0x50
	else:
		collider.collision_layer = 0x40
	var shape = CollisionShape3D.new()
	shape.shape = BoxShape3D.new()
	shape.shape.set_size(Vector3(furniture_data["sizeY"], furniture_data["sizeZ"], furniture_data["sizeX"]))
	collider.transform.origin = furniture.transform.origin
	collider.add_child(shape)
	n_ground.add_child(collider)
	colliders[coord] = collider
	n_furnitures.add_child(furniture)

func add_fire(coord: Vector3, size: float, light_power: int):
	var fire = base_fire.instantiate()
	fire.initialize(coord, size, light_power)
	return fire

func get_color(character_data: Dictionary):
	match(Values.link.getRelation(characters_data[owned_character]["team"], character_data["team"])):
		"SAME": return "0000FF"
		"ALLY": return "00FF00"
		"NEUTRAL": return "FFFF00"
		"ENEMY": return "FF0000"

func add_character(character_id: int, character_data: Dictionary):
	var character = base_character.instantiate()
	character.scale_object_local(Vector3(character_data["sizeY"], character_data["sizeZ"], character_data["sizeX"]))
	character.set_color(get_color(character_data))
	character.transform.origin = Vector3(character_data["y"], character_data["z"] + 0, character_data["x"])
	character.rotation_degrees = Vector3(0, character_data["orientation_z"], character_data["orientation_x"])
	characters[character_id] = character
	character.id = character_id
	character.character = character_data["name"]
	n_characters.add_child(character)
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
