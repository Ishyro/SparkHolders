extends Node3D

const MOVEMENT_SPEED = 0.6
const MOVEMENT_FRICTION = 0.5

var _mouse_motion = Vector2.ZERO
var velocity = Vector3.ZERO
var pause_state = false

@onready var camera = $Camera3D
@onready var camera_attributes = camera.attributes
@onready var pause = $Pause
@onready var grid = $"../Map/Grid"


func _ready():
	#Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)
	pass

func _process(_delta):
	if(not pause_state):
		# Mouse movement.
		_mouse_motion.x = clamp(_mouse_motion.x, -1560, 1560)
		camera.rotation_degrees = Vector3(min(-30, max(-90, camera.rotation_degrees.x + _mouse_motion.y * -0.001)), camera.rotation_degrees.y, camera.rotation_degrees.z)

func _physics_process(_delta):
	if(not pause_state):
		camera_attributes.dof_blur_far_enabled = true
		camera_attributes.dof_blur_far_distance = 100 * 1.5
		camera_attributes.dof_blur_far_transition = 100 * 0.125
		
		# Keyboard movement.
		var movement_vec2 = Input.get_vector("move_left", "move_right", "move_forward", "move_back")
		movement_vec2 = movement_vec2.rotated(deg_to_rad(-camera.rotation_degrees.y))
		var movement = Vector3(movement_vec2.x, 0, movement_vec2.y)
		
		movement *= MOVEMENT_SPEED * ( 3 + transform.origin.y) / 10

		velocity += Vector3(movement.x, 0, movement.z)
		# Apply horizontal friction.
		velocity.x *= MOVEMENT_FRICTION
		velocity.z *= MOVEMENT_FRICTION
		transform.origin += velocity

func _input(event):
	if event.is_action_pressed("pause"):
		pause_state = not pause_state
		pause.visible = pause_state
		if pause_state:
			$Pause/Menu/Continue.grab_focus()
	if not pause_state:
		if event.is_action_pressed("rotate_up_down") and event.double_click:
				camera.rotation_degrees = Vector3(-60, camera.rotation_degrees.y, 0)
		if Input.is_action_pressed("rotate_up_down"):
			if event is InputEventMouseMotion:
				_mouse_motion += event.relative
		else:
			_mouse_motion = Vector2.ZERO
		if event.is_action_pressed("zoom_in"):
			transform.origin = Vector3(transform.origin.x, max(transform.origin.y - 1, 2), transform.origin.z)
		if event.is_action_pressed("zoom_out"):
			transform.origin = Vector3(transform.origin.x, min(transform.origin.y + 1, 20), transform.origin.z)
		if event.is_action_pressed("rotate_left"):
			camera.rotation_degrees += Vector3(0, 45, 0)
		if event.is_action_pressed("rotate_right"):
			camera.rotation_degrees += Vector3(0, -45, 0)
		if event.is_action_pressed("display_grid"):
			grid.visible = true
		if event.is_action_released("display_grid"):
			grid.visible = false
