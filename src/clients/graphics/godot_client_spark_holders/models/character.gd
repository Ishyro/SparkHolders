extends CharacterBody3D

var character
var id

# How fast the player moves in meters per second.
var speed = 0 #14
#@export var accel = 40
# The downward acceleration when in the air, in meters per second squared.
#@export var fall_acceleration = 40 #75

var target_velocity = Vector3.ZERO

@onready var nav = $NavigationAgent3D
@onready var range_mesh = $AttackRange
@onready var shape = $Shape

var range_to_clear = 0
var tick_time = 100. / 1000.
var current_time = 0

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
	current_time += delta
	if transform.origin.is_equal_approx(dest) or current_time >= tick_time:
		if not transform.origin.is_equal_approx(dest):
			pass
			#global_transform.origin = dest
		range_to_clear = 0
		current_time = 0
		return true
	else:
		if range_to_clear == 0:
			range_to_clear = global_transform.origin.distance_to(dest)
			speed = range_to_clear / tick_time
		global_transform = global_transform.looking_at(dest, Vector3.UP, true).scaled_local(scale)
		rotation = Vector3(0, rotation.y, 0)
		var direction = (dest - global_transform.origin).normalized()
		velocity = direction * speed * delta
		global_transform.origin = global_transform.origin + velocity
		return false
