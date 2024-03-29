extends Control

@onready var continueButton = $Menu/Continue

@onready var map = $"../../Map"
@onready var view = $"../../View"
@onready var hud = $"../../HUD"

func _on_continue_pressed():
	view.pause_state = false
	if !view.perspective3:
		Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)
	visible = false

func _on_save_pressed():
	pass # Replace with function body.

func _on_load_pressed():
	pass # Replace with function body.szqdzq

func _on_menu_pressed():
	Values.link.close()
	map.thread.wait_to_finish()
	if Values.server_pid != -1:
		OS.kill(Values.server_pid)
	get_tree().change_scene_to_file("res://menus/main_menu.tscn")

func _on_quit_pressed():
	Values.link.close()
	map.thread.wait_to_finish()
	if Values.server_pid != -1:
		OS.kill(Values.server_pid)
	get_tree().quit()

func _on_visibility_changed():
	if visible:
		if hud:
			hud.visible = false
		continueButton.grab_focus()
	else:
		if hud:
			hud.visible = true
