extends VBoxContainer


# Called when the node enters the scene tree for the first time.
func _ready():
	$Offline.grab_focus()

func _on_offline_pressed():
	get_tree().change_scene_to_file("res://menus/offline.tscn")


func _on_multiplayer_pressed():
	get_tree().change_scene_to_file("res://menus/multiplayer.tscn")


func _on_settings_pressed():
	get_tree().change_scene_to_file("res://menus/settings.tscn")


func _on_quit_pressed():
	get_tree().quit()
