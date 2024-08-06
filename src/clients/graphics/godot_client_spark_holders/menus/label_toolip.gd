extends Label

func _make_custom_tooltip(for_text):
	var tooltip = preload("res://menus/tooltip.tscn").instantiate()
	var length = for_text.length()
	tooltip.get_node("Label").text = for_text
	tooltip.custom_minimum_size = Vector2(1000, 80 + length * 1.75)
	return tooltip
