extends Node3D

var tiles = {}
var characters = {}
var projectiles = {}
var loots = {}
var materials = {}
var board_material = preload("res://world/board_material.tres")
var grid_material = StandardMaterial3D.new()

var base_character = preload("res://characters/character.tscn")

var height
var width
var mid_height
var mid_width
var ori = Vector3.ZERO
var link = GodotLink.new()

@onready var n_view = $"../View"
@onready var n_grid = $Grid
@onready var n_tiles = $Tiles
@onready var n_characters = $Characters
@onready var n_projectiles = $Projectiles
@onready var n_loots = $Loots

# Called when the node enters the scene tree for the first time.
func _ready():
	# var ip = "84.97.162.152";
	# var ip = "192.168.168.164";
	# var ip = "192.168.1.83";
	var ip = "127.0.0.1";
	link.initialize(ip)
	grid_material.albedo_color = "000000"
	height = 21
	width = 21
	mid_height = height / 2.0
	mid_width = width / 2.0
	n_view.transform.origin = Vector3(mid_height - 5, n_view.transform.origin.y, mid_width)
	for tile in link.getAvaillableTiles():
		materials[tile] = load(link.getPathFromTile(tile))
	for x in range(0, height):
		for z in range(0, width):
			_create_tile_mesh(Vector3(x, 0, z), "TXT_GRASS")
	_create_grid()
	_create_board()
	add_character(Vector3(mid_height, 0.5, mid_width), "0000FF", 0.3)
	add_character(Vector3(mid_height + 2, 0.5, mid_width + 2), "FF0000", 0.6)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass
	
func _create_grid():
	var pos = Vector3(ori.x + mid_height, ori.y, ori.z + mid_width)
	for x in range(0, height + 1):
		var line = MeshInstance3D.new()
		line.mesh = BoxMesh.new()
		line.mesh.set_size(Vector3(0.05, 0.001, width))
		line.set_surface_override_material(0, grid_material)
		line.transform.origin = pos + Vector3(x - mid_height, 0.5, 0)
		n_grid.add_child(line)
	for z in range(0, width + 1):
		var line = MeshInstance3D.new()
		line.mesh = BoxMesh.new()
		line.mesh.set_size(Vector3(height, 0.001, 0.05))
		line.set_surface_override_material(0, grid_material)
		line.transform.origin = pos + Vector3(0, 0.5, z - mid_width)
		n_grid.add_child(line)

func _create_board():
	var pos = Vector3(ori.x + mid_height, ori.y, ori.z + mid_width)
	# 1
	var rod = MeshInstance3D.new()
	rod.mesh = BoxMesh.new()
	rod.mesh.set_size(Vector3(0.1, 1, width + 0.1))
	rod.set_surface_override_material(0, board_material)
	rod.transform.origin = pos + Vector3(-0.05 - mid_height, 0, -0.05)
	add_child(rod)
	# 2
	rod = MeshInstance3D.new()
	rod.mesh = BoxMesh.new()
	rod.mesh.set_size(Vector3(height + 0.1, 1, 0.1))
	rod.set_surface_override_material(0, board_material)
	rod.transform.origin = pos + Vector3(0.05, 0, -0.05 - mid_width)
	add_child(rod)
	# 3
	rod = MeshInstance3D.new()
	rod.mesh = BoxMesh.new()
	rod.mesh.set_size(Vector3(0.1, 1, width + 0.1))
	rod.set_surface_override_material(0, board_material)
	rod.transform.origin = pos + Vector3(0.05 + mid_height, 0, 0.05)
	add_child(rod)
	# 4
	rod = MeshInstance3D.new()
	rod.mesh = BoxMesh.new()
	rod.mesh.set_size(Vector3(height + 0.1, 1, 0.1))
	rod.set_surface_override_material(0, board_material)
	rod.transform.origin = pos + Vector3(-0.05, 0, 0.05 + mid_width)
	add_child(rod)

func _create_tile_mesh(pos: Vector3, tile: String):
	var mi = MeshInstance3D.new()
	mi.mesh = BoxMesh.new()
	mi.mesh.set_size(Vector3.ONE)
	mi.set_surface_override_material(0, materials[tile])
	mi.transform.origin = Vector3(pos.x + 0.5, pos.y, pos.z + 0.5)
	n_tiles.add_child(mi)
	
func add_character(pos: Vector3, color: String, size: float):
	var character = base_character.instantiate()
	character.scale_object_local(Vector3(size, size, size))
	character.set_color(color)
	character.transform.origin = pos
	n_characters.add_child(character)
