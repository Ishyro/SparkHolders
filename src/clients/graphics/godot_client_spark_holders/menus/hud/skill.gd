extends TextureRect

@onready var selector = $Selector
var data = {}
var pos

func initialize(skill):
	data = skill
	tooltip_text = data["name"]
	texture = load(data["path"])

func clean():
	data = {}
	tooltip_text = ""
	texture = null

func _make_custom_tooltip(for_text):
	var tooltip = preload("res://menus/titled_tooltip.tscn").instantiate()
	if for_text != "":
		var title_text = Values.link.getEnglishFromKey(for_text)
		var label_text = Values.link.getEnglishFromKey(for_text + "_DESC")
		var length = label_text.length()
		tooltip.get_node("Background/Container/Title").text = title_text
		tooltip.get_node("Background/Container/Label").text = label_text
		tooltip.custom_minimum_size = Vector2(1000, 160 + length * 1.75)
	return tooltip

func _on_mouse_entered() -> void:
	selector.visible = true

func _on_mouse_exited() -> void:
	selector.visible = false
