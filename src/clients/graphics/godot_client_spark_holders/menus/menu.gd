extends Control

@onready var settings = $Settings
@onready var adventure_selector = $AdventureSelector

@onready var single_player_new = $Container/SubMenus/SinglePlayer/New
@onready var single_player_load = $Container/SubMenus/SinglePlayer/Load
@onready var multi_player_new = $Container/SubMenus/MultiPlayer/New
@onready var multi_player_load = $Container/SubMenus/MultiPlayer/Load
@onready var multi_player_join = $Container/SubMenus/MultiPlayer/Join

func _ready():
	$Container/MainMenu/Singleplayer.grab_focus.call_deferred()
	
func _on_singleplayer_pressed():
	single_player_new.visible = not single_player_new.visible
	single_player_load.visible = not single_player_load.visible
	multi_player_new.visible = false
	multi_player_load.visible = false 
	multi_player_join.visible = false

func _on_multiplayer_pressed():
	single_player_new.visible = false
	single_player_load.visible = false
	multi_player_new.visible = not multi_player_new.visible
	multi_player_load.visible = not multi_player_load.visible
	multi_player_join.visible = not multi_player_join.visible

func _on_settings_pressed():
	settings.visible = true

func _on_quit_pressed():
	get_tree().quit()

func _on_new_single_pressed():
	Values.multiplayer_mode = false
	adventure_selector.visible = true

func _on_load_single_pressed():
	Values.multiplayer_mode = false
	Values.server_owner = true
	OS.create_process("./SparkHolders.Server.exe", ["data/adventures/test.data", str(Values.multiplayer_mode)])
	get_tree().change_scene_to_file("res://world/world.tscn")

func _on_new_multi_pressed():
	Values.multiplayer_mode = true
	adventure_selector.visible = true

func _on_load_multi_pressed():
	Values.multiplayer_mode = true
	Values.server_owner = true
	OS.create_process("./SparkHolders.Server.exe", ["data/adventures/test.data", str(Values.multiplayer_mode)])
	get_tree().change_scene_to_file("res://world/world.tscn")

func _on_join_pressed():
	Values.multiplayer_mode = true
	get_tree().change_scene_to_file("res://menus/character_creator.tscn")
