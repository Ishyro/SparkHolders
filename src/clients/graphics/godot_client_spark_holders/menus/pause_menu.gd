extends Control

# Called when the node enters the scene tree for the first time.
func _ready():
	$Menu/Continue.grab_focus()

func _on_continue_pressed():
	$"../../View".pause_state = false
	visible = false

func _on_save_pressed():
	pass # Replace with function body.

func _on_load_pressed():
	pass # Replace with function body.szqdzq

func _on_menu_pressed():
	OS.kill(Values.server_pid)
	get_tree().change_scene_to_file("res://menus/main_menu.tscn")

func _on_quit_pressed():
	OS.kill(Values.server_pid)
	get_tree().quit()
