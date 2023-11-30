extends Control

# Called when the node enters the scene tree for the first time.
func _ready():
	$Menu/New.grab_focus()

func _on_new_pressed():
	Values.server_pid = OS.create_process("./SparkHolders.Server.exe", ["data/adventures/test.data", "1"])
	get_tree().change_scene_to_file("res://world/world.tscn")

func _on_load_pressed():
	Values.server_pid = OS.create_process("./SparkHolders.Server.exe", ["data/adventures/test.data", "1"])
	get_tree().change_scene_to_file("res://world/world.tscn")

func _on_join_pressed():
	get_tree().change_scene_to_file("res://world/world.tscn")

func _on_settings_pressed():
	get_tree().change_scene_to_file("res://menus/settings.tscn")

func _on_back_pressed():
	get_tree().change_scene_to_file("res://menus/main_menu.tscn")
