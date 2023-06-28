extends Node3D

var grid = []
var board = []
var tiles = []
var lights = []
var characters = {}
var projectiles = {}
var loots = {}
var materials = {}

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

var height
var width
var level
var plan
var mid_height
var mid_width
var ori = Vector3(50, 0, 60)#Vector3.ZERO
var link = GodotLink.new()

@onready var n_view = $"../View"
@onready var n_grid = $Grid
@onready var n_tiles = $Tiles
@onready var n_lights = $Lights
@onready var n_characters = $Characters
@onready var n_projectiles = $Projectiles
@onready var n_loots = $Loots

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
	var ip = "84.97.162.152";
	# var ip = "192.168.168.164";
	# var ip = "192.168.1.83";
	# var ip = "127.0.0.1";
	link.initialize(ip)
	#link.receiveState()
	grid_material.albedo_color = "000000"
	height = 21
	width = 21
	level = 0
	plan = "Aytlanta"
	mid_height = height / 2.0
	mid_width = width / 2.0
	n_view.transform.origin = Vector3(ori.x + mid_height - 5, n_view.transform.origin.y, ori.z + mid_width)
	for i in height:
		tiles.append([])
		lights.append([])
		for j in width:
			tiles[i].append([])
			lights[i].append([])
	board.append([])
	for tile in link.getAvaillableTiles():
		materials[tile] = load(link.getPathFromTile(tile))
	create_grid()
	create_board()
	create_tiles()
	add_character(Vector3(ori.x + mid_height, 1, ori.z + mid_width), "0000FF", 0.3, 90., 1)
	add_character(Vector3(ori.x + mid_height + 2, 1, ori.z + mid_width + 2), "FF0000", 0.5, 180., 2)
	add_projectile(Vector3(ori.x + mid_height + 7, 1, ori.z + mid_width + 2), 0.1, 270., 1)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass
	
func create_grid():
	var pos = Vector3(ori.x + mid_height, ori.y, ori.z + mid_width)
	for x in range(0, height + 1):
		var line = MeshInstance3D.new()
		line.mesh = BoxMesh.new()
		line.mesh.set_size(Vector3(0.05, 0.001, width))
		line.set_surface_override_material(0, grid_material)
		line.transform.origin = pos + Vector3(x - mid_height, 1.001, 0)
		grid.append(line)
		n_grid.add_child(line)
	for z in range(0, width + 1):
		var line = MeshInstance3D.new()
		line.mesh = BoxMesh.new()
		line.mesh.set_size(Vector3(height, 0.001, 0.05))
		line.set_surface_override_material(0, grid_material)
		line.transform.origin = pos + Vector3(0, 1.001, z - mid_width)
		grid.append(line)
		n_grid.add_child(line)

func create_board():
	var pos = Vector3(ori.x + mid_height, ori.y, ori.z + mid_width)
	# 1
	var rod = MeshInstance3D.new()
	rod.mesh = BoxMesh.new()
	rod.mesh.set_size(Vector3(0.1, 1, width + 0.1))
	rod.set_surface_override_material(0, board_material)
	rod.transform.origin = pos + Vector3(-0.05 - mid_height, 0.5, -0.05)
	board.append(rod)
	add_child(rod)
	# 2
	rod = MeshInstance3D.new()
	rod.mesh = BoxMesh.new()
	rod.mesh.set_size(Vector3(height + 0.1, 1, 0.1))
	rod.set_surface_override_material(0, board_material)
	rod.transform.origin = pos + Vector3(0.05, 0.5, -0.05 - mid_width)
	board.append(rod)
	add_child(rod)
	# 3
	rod = MeshInstance3D.new()
	rod.mesh = BoxMesh.new()
	rod.mesh.set_size(Vector3(0.1, 1, width + 0.1))
	rod.set_surface_override_material(0, board_material)
	rod.transform.origin = pos + Vector3(0.05 + mid_height, 0.5, 0.05)
	board.append(rod)
	add_child(rod)
	# 4
	rod = MeshInstance3D.new()
	rod.mesh = BoxMesh.new()
	rod.mesh.set_size(Vector3(height + 0.1, 1, 0.1))
	rod.set_surface_override_material(0, board_material)
	rod.transform.origin = pos + Vector3(-0.05, 0.5, 0.05 + mid_width)
	board.append(rod)
	add_child(rod)

func create_tiles():
	for x in range(0, height):
		for z in range(0, width):
			var light = MeshInstance3D.new()
			light.mesh = PlaneMesh.new()
			light.mesh.set_size(Vector2.ONE)
			light.set_surface_override_material(0, light_c)
			light.transform.origin = Vector3(ori.x + x + 0.5, 1.001, ori.z + z + 0.5)
			lights[x][z] = light
			n_lights.add_child(light)
			var tile = MeshInstance3D.new()
			tile.mesh = BoxMesh.new()
			tile.mesh.set_size(Vector3.ONE)
			tile.set_surface_override_material(0, materials["TXT_GRASS"])
			tile.transform.origin = Vector3(ori.x + x + 0.5, 0.5, ori.z + z + 0.5)
			tiles[x][z] = tile
			n_tiles.add_child(tile)
	
func add_character(pos: Vector3, color: String, size: float, orientation: float, id: int):
	var character = base_character.instantiate()
	character.scale_object_local(Vector3(size, size, size))
	character.set_color(color)
	character.transform.origin = pos
	character.rotation_degrees += Vector3(0, orientation, 0)
	characters[id] = character
	n_characters.add_child(character)
	
func add_projectile(pos: Vector3, size: float, orientation: float, id: int):
	var projectile = base_projectile.instantiate()
	projectile.scale_object_local(Vector3(size, size, size))
	projectile.transform.origin = pos
	projectile.rotation_degrees += Vector3(0, orientation, 0)
	projectiles[id] = projectile
	n_projectiles.add_child(projectile)
