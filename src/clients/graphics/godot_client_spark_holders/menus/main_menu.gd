extends Control

# Called when the node enters the scene tree for the first time.
func _ready():
	$Menu/Singleplayer.grab_focus()

func _on_singleplayer_pressed():
	get_tree().change_scene_to_file("res://menus/singleplayer.tscn")

func _on_multiplayer_pressed():
	get_tree().change_scene_to_file("res://menus/multiplayer.tscn")

func _on_settings_pressed():
	get_tree().change_scene_to_file("res://menus/settings.tscn")

func _on_quit_pressed():
	if Values.server_pid != -1:
		OS.kill(Values.server_pid)
	get_tree().quit()
