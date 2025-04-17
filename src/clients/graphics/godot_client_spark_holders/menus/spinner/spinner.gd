extends AspectRatioContainer

@onready var outer = $Outer

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	outer.pivot_offset = outer.size / 2.
	var tween = get_tree().create_tween().bind_node(outer).set_trans(Tween.TRANS_LINEAR).set_loops()
	tween.tween_property(outer, "rotation_degrees", 360.0, 4).from(0)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
