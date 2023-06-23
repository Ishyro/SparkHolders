extends CharacterBody3D

const EYE_HEIGHT_STAND = 1.6
const EYE_HEIGHT_CROUCH = 1.4

const MOVEMENT_SPEED_GROUND = 0.6
const MOVEMENT_SPEED_AIR = 0.11
const MOVEMENT_SPEED_CROUCH_MODIFIER = 0.5
const MOVEMENT_FRICTION_GROUND = 0.9
const MOVEMENT_FRICTION_AIR = 0.98

var _mouse_motion = Vector2()

@onready var head = $Head
@onready var raycast = $Head/RayCast3D
@onready var camera_attributes = $Head/Camera3D.attributes
@onready var crosshair = $"../Crosshair"


func _ready():
	Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)
	var collider = CollisionShape3D.new()
	collider.shape = CapsuleShape3D.new()
	collider.shape.set_height(2)
	collider.shape.set_radius(0.3)
	#collider.transform.origin = Vector3(0, 1, 0)
	add_child(collider)

func _process(_delta):
	# Mouse movement.
	_mouse_motion.y = clamp(_mouse_motion.y, -1560, 1560)
	transform.basis = Basis.from_euler(Vector3(0, _mouse_motion.x * -0.001, 0))
	head.transform.basis = Basis.from_euler(Vector3(_mouse_motion.y * -0.001, 0, 0))


func _physics_process(_delta):
	camera_attributes.dof_blur_far_enabled = true
	camera_attributes.dof_blur_far_distance = 10 * 1.5
	camera_attributes.dof_blur_far_transition = 10 * 0.125
	
	# Keyboard movement.
	var movement_vec2 = Input.get_vector("move_left", "move_right", "move_forward", "move_back")
	var movement = transform.basis * (Vector3(movement_vec2.x, 0, movement_vec2.y))

	movement *= MOVEMENT_SPEED_GROUND

	velocity += Vector3(movement.x, 0, movement.z)
	# Apply horizontal friction.
	velocity.x *= MOVEMENT_FRICTION_GROUND
	velocity.z *= MOVEMENT_FRICTION_GROUND
	move_and_slide()

func _input(event):
	if event is InputEventMouseMotion:
		if Input.get_mouse_mode() == Input.MOUSE_MODE_CAPTURED:
			_mouse_motion += event.relative
