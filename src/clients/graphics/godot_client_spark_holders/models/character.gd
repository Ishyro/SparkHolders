extends CharacterBody3D

var character
var id

# How fast the player moves in meters per second.
@export var speed = 8 #14
@export var accel = 40
# The downward acceleration when in the air, in meters per second squared.
@export var fall_acceleration = 40 #75

var target_velocity = Vector3.ZERO

@onready var nav = $NavigationAgent3D
@onready var range_mesh = $AttackRange
@onready var shape = $Shape

var checkpoint = null

func set_color(color: String):
	var material = StandardMaterial3D.new()
	material.albedo_color = color
	$Color.set_surface_override_material(0, material)
	$Selection.set_surface_override_material(0, material)

func set_attack_range(weapon_range: float):
	$AttackRange.scale_object_local(Vector3(weapon_range, weapon_range, weapon_range))

func select():
	$Selection.visible = true

func unselect():
	$Selection.visible = false

func move_towards(dest: Vector3, delta: float):
	nav.target_position = dest
	#if not checkpoint or nav.is_navigation_finished():
	checkpoint = nav.get_next_path_position()
	var space = get_world_3d().direct_space_state
	var query = PhysicsRayQueryParameters3D.create(checkpoint, checkpoint + Vector3(0, -10, 0), 0x1)
	var result = space.intersect_ray(query)
	if not result.is_empty():
		checkpoint = result["position"]
	if not transform.origin.is_equal_approx(checkpoint):
		global_transform = global_transform.looking_at(checkpoint, Vector3.UP, true).scaled_local(scale)
		rotation = Vector3(0, rotation.y, 0)
	var direction = (checkpoint - global_transform.origin).normalized()
	velocity = velocity.lerp(direction * speed, accel * delta)
	move_and_slide()
