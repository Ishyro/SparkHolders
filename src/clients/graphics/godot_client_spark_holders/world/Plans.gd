extends Node3D

var plans = {}
var base_plan = preload("res://world/plan.tscn")

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass

func add_map(plan_name: String, map_name: String, pos: Vector3, size: Vector3):
	if plans.has(plan_name):
		plans[plan_name].add_map(map_name, pos, size)
	else:
		var plan = base_plan.instantiate()
		plan.set_name(plan_name)
		plan.add_map(map_name, pos, size)
		plans[plan_name] = plan
		add_child(plan)
