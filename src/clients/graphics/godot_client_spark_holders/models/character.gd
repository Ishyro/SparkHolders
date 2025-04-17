extends CharacterBody3D

var character
var id

@onready var shape = $Shape

func move(speed: Vector3, delta: float):
	velocity = speed * delta
	#global_transform.origin = global_transform.origin + velocity
	var previous = global_transform.origin
	# TODO
	# crash when physics are on another thread
	var collision = move_and_collide(velocity)
	if collision:
		global_transform.origin = previous
