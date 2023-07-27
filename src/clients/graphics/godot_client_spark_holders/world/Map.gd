extends Node3D

var owned_characters = []
var phantoms = {}
var phantom_lines = {}

var grid = []
var board = []
var tiles = []
var lights = []
var characters = {}
var projectiles = {}
var furnitures = {}
var tiles_data = {}
var characters_data = {}
var projectiles_data = {}
var materials = {}

var tiles_img = {}

var board_material = preload("res://world/board_material.tres")
var grid_material = StandardMaterial3D.new()
var light_0 = StandardMaterial3D.new()
var light_1 = StandardMaterial3D.new()
var light_2 = StandardMaterial3D.new()
var light_3 = StandardMaterial3D.new()
var light_4 = StandardMaterial3D.new()
var light_5 = StandardMaterial3D.new()
var light_6 = StandardMaterial3D.new()
var light_7 = StandardMaterial3D.new()
var light_8 = StandardMaterial3D.new()
var light_9 = StandardMaterial3D.new()
var light_a = StandardMaterial3D.new()
var light_b = StandardMaterial3D.new()
var light_c = StandardMaterial3D.new()
var light_d = StandardMaterial3D.new()
var light_e = StandardMaterial3D.new()
var light_f = StandardMaterial3D.new()

var base_character = preload("res://models/character.tscn")
var base_phantom = preload("res://models/phantom.tscn")
var base_projectile = preload("res://models/projectile.tscn")
var base_tile = preload("res://world/tile.tscn")

var plan
var offset = Vector3.ZERO
var size = Vector3.ZERO
var mid_size = Vector3.ZERO

class Pathfinding:
	extends AStar3D
	
	var size
	var edge = sqrt(2)
	
	func _init(arg):
		size = arg

	func _compute_cost(u, v):
		var diff = abs(u - v)
		if diff == 1 || diff == size.x:
			return 1
		else:
			return edge

	func _estimate_cost(u, v):
		var diff = abs(u - v)
		if diff == 1 || diff == size.x:
			return 1
		else:
			return edge

var pathfind

@onready var n_view = $"../View"
@onready var n_grid = $Grid
@onready var n_tiles = $Tiles
@onready var n_lights = $Lights
@onready var n_characters = $Characters
@onready var n_projectiles = $Projectiles
@onready var n_furnitures = $Furnitures

# Called when the node enters the scene tree for the first time.
func _ready():
	light_0.albedo_color = "000000"
	light_1.albedo_color = "111111"
	light_2.albedo_color = "222222"
	light_3.albedo_color = "333333"
	light_4.albedo_color = "444444"
	light_5.albedo_color = "555555"
	light_5.albedo_color = "666666"
	light_7.albedo_color = "777777"
	light_8.albedo_color = "888888"
	light_9.albedo_color = "999999"
	light_a.albedo_color = "aaaaaa"
	light_b.albedo_color = "bbbbbb"
	light_c.albedo_color = "cccccc"
	light_d.albedo_color = "dddddd"
	light_e.albedo_color = "eeeeee"
	light_f.albedo_color = "ffffff"
	# var ip = "84.97.162.152";
	# var ip = "192.168.168.164";
	# var ip = "192.168.1.83";
	var ip = "127.0.0.1";
	Values.link.initialize(ip)
	Values.link.receiveState()
	grid_material.albedo_color = "000000"
	offset = Values.link.getOffsets()
	size = Values.link.getSizes()
	pathfind = Pathfinding.new(size)
	mid_size = Vector3(size.x / 2.0, size.y / 2.0, size.z / 2.0)
	plan = "Aytlanta"
	n_view.transform.origin = Vector3(offset.x + mid_size.x - 5, offset.y + n_view.transform.origin.y, offset.z + mid_size.z)
	for i in size.x:
		tiles.append([])
		lights.append([])
		for j in size.z:
			tiles[i].append([])
			lights[i].append([])
	board.append([])
	for tile in Values.link.getAvaillableTiles():
		tiles_data[tile] = Values.link.getDataFromTile(tile)
		if tile != "TXT_MIST":
			materials[tile] = load(tiles_data[tile]["path"])
		else:
			materials[tile] = board_material
		if tile == "TXT_GRASS" || tile == "TXT_FLOOR_STONE" || tile == "TXT_WALL_STONE" || tile == "TXT_MIST":
			var img = materials[tile].albedo_texture.get_image()
			tiles_img[tile] = ImageTexture.create_from_image(img)
	owned_characters = Values.link.getControlledParty()
	characters_data = Values.link.getCharacters()
	projectiles_data = Values.link.getProjectiles()
	create_grid()
	create_board()
	create_tiles()
	create_paths()
	for character_id in characters_data:
		add_character(character_id, characters_data[character_id])
	for projectile_id in projectiles_data:
		add_projectile(projectile_id, projectiles_data[projectile_id])
	for character_id in owned_characters:
		phantoms[character_id] = base_phantom.instantiate()
		phantoms[character_id].scale_object_local(Vector3(characters_data[character_id]["size"], characters_data[character_id]["size"], characters_data[character_id]["size"]))
		phantoms[character_id].id = character_id
		phantoms[character_id].transform.origin = characters[str(character_id)].transform.origin
		n_characters.add_child(phantoms[character_id])

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass
	
func create_grid():
	var pos = Vector3(offset.x + mid_size.x, offset.y, offset.z + mid_size.z)
	for x in range(0, size.x + 1):
		var line = MeshInstance3D.new()
		line.mesh = BoxMesh.new()
		line.mesh.set_size(Vector3(0.05, 0.001, size.z))
		line.set_surface_override_material(0, grid_material)
		line.transform.origin = pos + Vector3(x - mid_size.x, offset.y + 1.001, 0)
		grid.append(line)
		n_grid.add_child(line)
	for z in range(0, size.z + 1):
		var line = MeshInstance3D.new()
		line.mesh = BoxMesh.new()
		line.mesh.set_size(Vector3(size.x, 0.001, 0.05))
		line.set_surface_override_material(0, grid_material)
		line.transform.origin = pos + Vector3(0, offset.y + 1.001, z - mid_size.z)
		grid.append(line)
		n_grid.add_child(line)

func create_board():
	var pos = Vector3(offset.x + mid_size.x, offset.y, offset.z + mid_size.z)
	# 1
	var rod = StaticBody3D.new()
	rod.collision_layer = 0x0001
	rod.collision_mask = 0x000f
	var rod_mesh = MeshInstance3D.new()
	var rod_shape = CollisionShape3D.new()
	rod_mesh.mesh = BoxMesh.new()
	rod_shape.shape = BoxShape3D.new()
	rod_mesh.mesh.set_size(Vector3(0.1, 1, size.z + 0.1))
	rod_shape.shape.set_size(Vector3(0.1, 2, size.z + 0.1))
	rod_mesh.set_surface_override_material(0, board_material)
	rod.add_child(rod_mesh)
	rod.add_child(rod_shape)
	rod.transform.origin = pos + Vector3(-0.05 - mid_size.x, offset.y + 0.5, -0.05)
	board.append(rod)
	add_child(rod)
	# 2
	rod = StaticBody3D.new()
	rod.collision_layer = 0x0001
	rod.collision_mask = 0x000f
	rod_mesh = MeshInstance3D.new()
	rod_shape = CollisionShape3D.new()
	rod_mesh.mesh = BoxMesh.new()
	rod_shape.shape = BoxShape3D.new()
	rod_mesh.mesh.set_size(Vector3(size.x + 0.1, 1, 0.1))
	rod_shape.shape.set_size(Vector3(size.x + 0.1, 2, 0.1))
	rod_mesh.set_surface_override_material(0, board_material)
	rod.add_child(rod_mesh)
	rod.add_child(rod_shape)
	rod.transform.origin = pos + Vector3(0.05, offset.y + 0.5, -0.05 - mid_size.z)
	board.append(rod)
	add_child(rod)
	# 3
	rod = StaticBody3D.new()
	rod.collision_layer = 0x0001
	rod.collision_mask = 0x000f
	rod_mesh = MeshInstance3D.new()
	rod_shape = CollisionShape3D.new()
	rod_mesh.mesh = BoxMesh.new()
	rod_shape.shape = BoxShape3D.new()
	rod_mesh.mesh.set_size(Vector3(0.1, 1, size.z + 0.1))
	rod_shape.shape.set_size(Vector3(0.1, 2, size.z + 0.1))
	rod_mesh.set_surface_override_material(0, board_material)
	rod.add_child(rod_mesh)
	rod.add_child(rod_shape)
	rod.transform.origin = pos + Vector3(0.05 + mid_size.x, offset.y + 0.5, 0.05)
	board.append(rod)
	add_child(rod)
	# 4
	rod = StaticBody3D.new()
	rod.collision_layer = 0x0001
	rod.collision_mask = 0x000f
	rod_mesh = MeshInstance3D.new()
	rod_shape = CollisionShape3D.new()
	rod_mesh.mesh = BoxMesh.new()
	rod_shape.shape = BoxShape3D.new()
	rod_mesh.mesh.set_size(Vector3(size.x + 0.1, 1, 0.1))
	rod_shape.shape.set_size(Vector3(size.x + 0.1, 2, 0.1))
	rod_mesh.set_surface_override_material(0, board_material)
	rod.add_child(rod_mesh)
	rod.add_child(rod_shape)
	rod.transform.origin = pos + Vector3(-0.05, offset.y + 0.5, 0.05 + mid_size.z)
	board.append(rod)
	add_child(rod)
	
func get_light(light: int):
	match light:
		0: return light_0
		1: return light_1
		2: return light_2
		3: return light_3
		4: return light_4
		5: return light_5
		6: return light_6
		7: return light_7
		8: return light_8
		9: return light_9
		10: return light_a
		11: return light_b
		12: return light_c
		13: return light_d
		14: return light_e
		15: return light_f

func create_tiles():
	var current_tiles = Values.link.getTiles()
	var current_lights = Values.link.getLights()
	for x in range(0, size.x):
		for z in range(0, size.z):
			var light = MeshInstance3D.new()
			light.mesh = BoxMesh.new()
			light.set_surface_override_material(0, get_light(current_lights[x][z]))
			if tiles_data[current_tiles[x][z]]["solid"]:
				light.mesh.set_size(Vector3(1, 3, 1))
				light.transform.origin = Vector3(offset.x + x + 0.5, offset.y + 1.5, offset.z + z + 0.5)
			else:
				light.mesh.set_size(Vector3.ONE)
				light.transform.origin = Vector3(offset.x + x + 0.5, offset.y + 0.5, offset.z + z + 0.5)
			lights[x][z] = light
			n_lights.add_child(light)
			var tile = base_tile.instantiate()
			tile.create(Vector3(offset.x + x, offset.y, offset.z +z), current_tiles[x][z], tiles_data[current_tiles[x][z]]["solid"], current_tiles[x][z] == "TXT_MIST", materials[current_tiles[x][z]])
			tiles[x][z] = tile
			n_tiles.add_child(tile)
			
func create_paths():
	var indice = 0
	for x in range(0, size.x):
		for z in range(0, size.z):
			pathfind.add_point(indice, Vector3(x + 0.5, offset.y + 1, z + 0.5), tiles_data[tiles[x][z].tile]["ap_cost"])
			indice = indice + 1
	for i in range(0, indice, 2):
		var x = floor(i / size.z)
		var z = max(0, i - x * size.z)
		if !tiles_data[tiles[x][z].tile]["solid"]:
			if z > 0 && tiles_data[tiles[x][z - 1].tile]["solid"]:
				pathfind.connect_points(i - 1, i, true)
				if x > 0 && tiles_data[tiles[x - 1][z - 1].tile]["solid"]:
					pathfind.connect_points(i - size.z - 1, i, true)
				if x < size.x - 1 && tiles_data[tiles[x + 1][z - 1].tile]["solid"]:
					pathfind.connect_points(i, i + size.z - 1, true)
			if x > 0 && tiles_data[tiles[x - 1][z].tile]["solid"]:
				pathfind.connect_points(i - size.z, i, true)
			if z < size.z - 1 && tiles_data[tiles[x][z + 1].tile]["solid"]:
				pathfind.connect_points(i, i + 1, true)
				if x > 0 && tiles_data[tiles[x - 1][z + 1].tile]["solid"]:
					pathfind.connect_points(i - size.z + 1, i, true)
				if x < size.z - 1 && tiles_data[tiles[x + 1][z + 1].tile]["solid"]:
					pathfind.connect_points(i, i + size.z + 1, true)
			if x < size.x - 1 && tiles_data[tiles[x + 1][z].tile]["solid"]:
				pathfind.connect_points(i, i + size.z, true)

func get_color(character_data: Dictionary):
	match(Values.link.getRelation(characters_data[owned_characters[0]]["team"], character_data["team"])):
		"SAME": return "0000FF"
		"ALLY": return "00FF00"
		"NEUTRAL": return "FFFF00"
		"ENEMY": return "FF0000"

func add_character(character_id: int, character_data: Dictionary):
	var character = base_character.instantiate()
	character.scale_object_local(Vector3(character_data["size"], character_data["size"], character_data["size"]))
	character.set_color(get_color(character_data))
	character.transform.origin = Vector3(character_data["y"], character_data["z"] + 1, character_data["x"])
	character.rotation_degrees = Vector3(0, character_data["orientation"], 0)
	characters[str(character_id)] = character
	character.id = character_id
	character.character = character_data["name"]
	n_characters.add_child(character)

func add_projectile(projectile_id: int, projectile_data: Dictionary):
	var projectile = base_projectile.instantiate()
	projectile.scale_object_local(Vector3(projectile_data["size"], projectile_data["size"], projectile_data["size"]))
	projectile.transform.origin = Vector3(projectile_data["y"], projectile_data["z"] + 1, projectile_data["x"])
	projectile.rotation_degrees = Vector3(0, projectile_data["orientation"], 0)
	projectile[str(projectile_id)] = projectile
	projectile.id = projectile_id
	projectile.projectile = projectile_data["name"]
	n_projectiles.add_child(projectile)
	
func update_phantom(character_id: int, delta):
	phantoms[character_id].visible = true
	var distance = characters[str(character_id)].transform.origin.distance_to(phantoms[character_id].transform.origin) / characters_data[character_id]["size"]
	var movement = (Values.coord - phantoms[character_id].transform.origin) * 100
	phantoms[character_id].move_and_collide(Vector3(movement.x, 0, movement.z) * delta)
	phantoms[character_id].rotation_degrees = Vector3(0, Values.link.getOrientationToTarget(Vector2(characters_data[character_id]["x"], characters_data[character_id]["y"]), Vector2(Values.coord.z, Values.coord.x)), 0)
