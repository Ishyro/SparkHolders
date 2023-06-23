extends Node

var materials = {}

# Called when the node enters the scene tree for the first time.
func _ready():
	var link = GodotLink.new()
	# var ip = "84.97.162.152";
	# var ip = "192.168.168.164";
	# var ip = "192.168.1.83";
	var ip = "127.0.0.1";
	link.initialize(ip)
	for tile in link.getAvaillableTiles():
		#materials[tile] = load("res://data/tiles/" + material + ".tres")
		if tile == "TXT_GRASS" || tile == "TXT_FLOOR_STONE" :
			materials[tile] = load(link.getPathFromTile(tile))
	for x in range(0, 10):
		for z in range(0, 10):
			_create_tile_mesh(Vector3(x, 0, z), "TXT_GRASS")
			_create_tile_mesh(Vector3(x - 10, 0, z - 10), "TXT_FLOOR_STONE")

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass

func _create_tile_mesh(pos: Vector3, tile: String):
	var mi = MeshInstance3D.new()
	mi.mesh = BoxMesh.new()
	mi.mesh.set_size(Vector3.ONE)
	mi.set_surface_override_material(0, materials[tile])
	mi.transform.origin = Vector3(pos)
	add_child(mi)
