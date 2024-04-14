extends Control

@onready var MainMenu = $MainMenu
@onready var SinglePlayer = $SinglePlayer
@onready var MultiPlayer = $MultiPlayer
@onready var Settings = $Settings

func _ready():
	$MainMenu/Singleplayer.set_text("Singleplayer")
	$MainMenu/Multiplayer.set_text("Multiplayer")
	$MainMenu/Settings.set_text("Settings")
	$MainMenu/Quit.set_text("Quit")
	$SinglePlayer/New.set_text("New Game")
	$SinglePlayer/Load.set_text("Load Game")
	$SinglePlayer/Settings.set_text("Settings")
	$SinglePlayer/Back.set_text("Back")
	$MultiPlayer/New.set_text("New Game")
	$MultiPlayer/Load.set_text("Load Game")
	$MultiPlayer/Join.set_text("Join Game")
	$MultiPlayer/Settings.set_text("Settings")
	$MultiPlayer/Back.set_text("Back")
	$MainMenu/Singleplayer.grab_focus.call_deferred()
	
func _on_singleplayer_pressed():
	MainMenu.visible = false
	SinglePlayer.visible = true
	$SinglePlayer/New.grab_focus.call_deferred()

func _on_multiplayer_pressed():
	MainMenu.visible = false
	MultiPlayer.visible = true
	$MultiPlayer/New.grab_focus.call_deferred()

func _on_settings_pressed():
	Settings.visible = true

func _on_quit_pressed():
	if Values.server_pid != -1:
		OS.kill(Values.server_pid)
	get_tree().quit()

func _on_new_single_pressed():
	Values.server_pid = OS.create_process("./SparkHolders.Server.exe", ["data/adventures/test.data", "1"])
	get_tree().change_scene_to_file("res://world/world.tscn")

func _on_load_single_pressed():
	Values.server_pid = OS.create_process("./SparkHolders.Server.exe", ["data/adventures/test.data", "1"])
	get_tree().change_scene_to_file("res://world/world.tscn")

func _on_back_single_pressed():
	SinglePlayer.visible = false
	MainMenu.visible = true
	$MainMenu/Singleplayer.grab_focus.call_deferred()

func _on_new_multi_pressed():
	Values.server_pid = OS.create_process("./SparkHolders.Server.exe", ["data/adventures/test.data", "1"])
	get_tree().change_scene_to_file("res://world/world.tscn")

func _on_load_multi_pressed():
	Values.server_pid = OS.create_process("./SparkHolders.Server.exe", ["data/adventures/test.data", "1"])
	get_tree().change_scene_to_file("res://world/world.tscn")

func _on_join_pressed():
	get_tree().change_scene_to_file("res://world/world.tscn")

func _on_back_multi_pressed():
	MultiPlayer.visible = false
	MainMenu.visible = true
	$MainMenu/Singleplayer.grab_focus.call_deferred()
