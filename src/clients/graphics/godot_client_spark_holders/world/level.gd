extends Node3D

var material = StandardMaterial3D.new()
var maps = {}

# Called when the node enters the scene tree for the first time.
func _ready():
	material.albedo_color = "000000"

func add_map(map_name: String, pos: Vector3, size: Vector3):
	var map = BoxMesh.new()
	map.mesh.set_size(Vector3(size.x / 100, size.y / 10, size.z / 100))
	map.set_surface_override_material(0, material)
	map.transform.origin = Vector3(pos.x - size.x / 50, 0, pos.z - size.z / 50)
	maps[map_name] = map
	add_child(map)
