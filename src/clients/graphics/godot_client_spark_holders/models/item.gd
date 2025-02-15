extends TextureRect

@onready var selector = $Selector

var data = {}
var item_coord = Vector2.ZERO
var item_slot = 0
var owned = true
var container = null

func _on_mouse_entered() -> void:
	selector.visible = true

func _on_mouse_exited() -> void:
	selector.visible = false
