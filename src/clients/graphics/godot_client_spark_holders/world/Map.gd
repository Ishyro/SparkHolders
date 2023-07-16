extends Node3D

var owned_characters = []

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
var base_projectile = preload("res://models/projectile.tscn")
var base_tile = preload("res://world/tile.tscn")

var plan
var offset = Vector3.ZERO
var size = Vector3.ZERO
var mid_size = Vector3.ZERO

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
	for character_id in characters_data:
		add_character(character_id, characters_data[character_id])
	for projectile_id in projectiles_data:
		add_projectile(projectile_id, projectiles_data[projectile_id])

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
	var rod = MeshInstance3D.new()
	rod.mesh = BoxMesh.new()
	rod.mesh.set_size(Vector3(0.1, 1, size.z + 0.1))
	rod.set_surface_override_material(0, board_material)
	rod.transform.origin = pos + Vector3(-0.05 - mid_size.x, offset.y + 0.5, -0.05)
	board.append(rod)
	add_child(rod)
	# 2
	rod = MeshInstance3D.new()
	rod.mesh = BoxMesh.new()
	rod.mesh.set_size(Vector3(size.x + 0.1, 1, 0.1))
	rod.set_surface_override_material(0, board_material)
	rod.transform.origin = pos + Vector3(0.05, offset.y + 0.5, -0.05 - mid_size.z)
	board.append(rod)
	add_child(rod)
	# 3
	rod = MeshInstance3D.new()
	rod.mesh = BoxMesh.new()
	rod.mesh.set_size(Vector3(0.1, 1, size.z + 0.1))
	rod.set_surface_override_material(0, board_material)
	rod.transform.origin = pos + Vector3(0.05 + mid_size.x, offset.y + 0.5, 0.05)
	board.append(rod)
	add_child(rod)
	# 4
	rod = MeshInstance3D.new()
	rod.mesh = BoxMesh.new()
	rod.mesh.set_size(Vector3(size.x + 0.1, 1, 0.1))
	rod.set_surface_override_material(0, board_material)
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
	character.rotation_degrees += Vector3(0, character_data["orientation"], 0)
	characters[str(character_id)] = character
	character.id = character_id
	character.character = character_data["name"]
	n_characters.add_child(character)

func add_projectile(projectile_id: int, projectile_data: Dictionary):
	var projectile = base_projectile.instantiate()
	projectile.scale_object_local(Vector3(projectile_data["size"], projectile_data["size"], projectile_data["size"]))
	projectile.transform.origin = Vector3(projectile_data["y"], projectile_data["z"] + 1, projectile_data["x"])
	projectile.rotation_degrees += Vector3(0, projectile_data["orientation"], 0)
	projectile[str(projectile_id)] = projectile
	projectile.id = projectile_id
	projectile.projectile = projectile_data["name"]
	n_projectiles.add_child(projectile)

