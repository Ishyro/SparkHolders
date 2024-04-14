extends TextureButton

@onready var label = $Label

var small_text = preload("res://menus/hud/police/SmallText.tres")
var small_text_white = preload("res://menus/hud/police/SmallTextWhite.tres")

func set_text(text: String):
	label.set_text(text) 

func _on_pressed():
	label.set_label_settings(small_text_white)

func _on_mouse_entered():
	grab_focus.call_deferred()

func _on_focus_entered():
	label.set_label_settings(small_text_white)

func _on_focus_exited():
	label.set_label_settings(small_text)
