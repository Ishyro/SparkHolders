extends StaticBody3D

var base_mist = preload("res://models/mist.tscn")
var tile

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass

func create(coord: Vector3, tile_name: String, solid: bool, mist: bool, material: StandardMaterial3D):
	tile = tile_name
	$Mesh.mesh = BoxMesh.new()
	$Shape.shape = BoxShape3D.new()
	if solid:
		$Mesh.mesh.set_size(Vector3(1, 3, 1))
		$Shape.shape.set_size(Vector3(1, 3, 1))
		transform.origin = Vector3(coord.x + 0.5, coord.y + 1.5, coord.z + 0.5)
	else:
		$Mesh.mesh.set_size(Vector3.ONE)
		$Shape.shape.set_size(Vector3.ONE)
		transform.origin = Vector3(coord.x + 0.5, coord.y + 0.5, coord.z + 0.5)
	if(mist):
		add_child(base_mist.instantiate())
	$Mesh.set_surface_override_material(0, material)
