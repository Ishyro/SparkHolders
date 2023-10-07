extends StaticBody3D

var base_mist = preload("res://models/mist.tscn")
var tile
var map

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass

func create(coord: Vector3, tile_name: String, map_id: int, solid: bool, unwalkable, mist: bool, material: Material):
	tile = tile_name
	map = map_id
	$Mesh.mesh = BoxMesh.new()
	$Shape.shape = BoxShape3D.new()
	if solid:
		$Mesh.mesh.set_size(Vector3(1, 3, 1))
		$Shape.shape.set_size(Vector3(1, 3, 1))
		transform.origin = Vector3(coord.x + 0.5, coord.y + 1.5, coord.z + 0.5)
	elif unwalkable:
		$Mesh.mesh = BoxMesh.new()
		$Mesh.mesh.set_size(Vector3(1, 0.8, 1))
		$Shape.shape.set_size(Vector3.ONE)
		transform.origin = Vector3(coord.x + 0.5, coord.y + 0.4, coord.z + 0.5)
		collision_layer = 0x0021
	else:
		$Mesh.mesh.set_size(Vector3.ONE)
		$Shape.shape.set_size(Vector3.ONE)
		transform.origin = Vector3(coord.x + 0.5, coord.y + 0.5, coord.z + 0.5)
	if(mist):
		add_child(base_mist.instantiate())
		$Shape.shape.set_size(Vector3(1, 3, 1))
		collision_layer = 0x0031
	$Mesh.set_surface_override_material(0, material)
