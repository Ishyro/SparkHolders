extends Control

var selected_adventure = "test"

# General
var tutorials = { "test": 0, "nightmare": 1 }
var first_era = {}
var second_era = {}
var third_era = {}
var mods = {}

var adventures_paths = { 
	"test": "data/adventures/test.data",
	"nightmare": "data/adventures/nightmare.data"
}

# Official
@onready var n_tutorials_adventure_value = $Divider/MarginContainer/TabContainer/Base/Tutorials/HBoxContainer/VBoxContainer/AdventureValue
@onready var n_tutorials_adventure_miniature = $Divider/MarginContainer/TabContainer/Base/Tutorials/HBoxContainer/VBoxContainer/AdventureMiniature
@onready var n_tutorials_description = $Divider/MarginContainer/TabContainer/Base/Tutorials/HBoxContainer/Description

@onready var n_first_era_adventure_value = $"Divider/MarginContainer/TabContainer/Base/First Era"/HBoxContainer/VBoxContainer/AdventureValue
@onready var n_first_era_adventure_miniature = $"Divider/MarginContainer/TabContainer/Base/First Era"/HBoxContainer/VBoxContainer/AdventureMiniature
@onready var n_first_era_description = $"Divider/MarginContainer/TabContainer/Base/First Era"/HBoxContainer/Description

@onready var n_second_era_adventure_value = $"Divider/MarginContainer/TabContainer/Base/Second Era"/HBoxContainer/VBoxContainer/AdventureValue
@onready var n_second_era_adventure_miniature = $"Divider/MarginContainer/TabContainer/Base/Second Era"/HBoxContainer/VBoxContainer/AdventureMiniature
@onready var n_second_era_description = $"Divider/MarginContainer/TabContainer/Base/Second Era"/HBoxContainer/Description

@onready var n_third_era_adventure_value = $"Divider/MarginContainer/TabContainer/Base/Third Era"/HBoxContainer/VBoxContainer/AdventureValue
@onready var n_third_era_adventure_miniature = $"Divider/MarginContainer/TabContainer/Base/Third Era"/HBoxContainer/VBoxContainer/AdventureMiniature
@onready var n_third_era_description = $"Divider/MarginContainer/TabContainer/Base/Third Era"/HBoxContainer/Description

# Mods
@onready var n_mods_adventure_value = $Divider/MarginContainer/TabContainer/Mods/HBoxContainer/VBoxContainer/AdventureValue
@onready var n_mods_adventure_miniature = $Divider/MarginContainer/TabContainer/Mods/HBoxContainer/VBoxContainer/AdventureMiniature
@onready var n_mods_description = $Divider/MarginContainer/TabContainer/Mods/HBoxContainer/Description

func _ready():
	# General
	for adventure in tutorials:
		n_tutorials_adventure_value.add_item(adventure)
	for adventure in first_era:
		n_tutorials_adventure_value.add_item(adventure)
	for adventure in second_era:
		n_tutorials_adventure_value.add_item(adventure)
	for adventure in third_era:
		n_tutorials_adventure_value.add_item(adventure)
	for adventure in mods:
		n_tutorials_adventure_value.add_item(adventure)
	
	selected_adventure = tutorials.keys()[0]
	$Divider/Buttons/Start.grab_focus.call_deferred()

func _on_start_pressed():
	Values.server_owner = true
	match OS.get_name():
		"Windows":
			OS.create_process("./SparkHolders.Server.exe", ["data/adventures/test.data", str(Values.multiplayer_mode)])
		"Linux", "FreeBSD", "NetBSD", "OpenBSD", "BSD":
			OS.create_process("./SparkHolders.Server.x86_64", ["data/adventures/test.data", str(Values.multiplayer_mode)])
	get_tree().change_scene_to_file("res://menus/character_creator.tscn")

func _on_close_pressed():
	print("close")
	visible = false

func _on_tutorial_value_item_selected(index):
	selected_adventure = n_tutorials_adventure_value.get_item_text(index)

func _on_first_era_value_item_selected(index):
	selected_adventure = n_first_era_adventure_value.get_item_text(index)

func _on_second_era_value_item_selected(index):
	selected_adventure = n_second_era_adventure_value.get_item_text(index)

func _on_third_era_value_item_selected(index):
	selected_adventure = n_third_era_adventure_value.get_item_text(index)

func _on_mod_value_item_selected(index):
	selected_adventure = n_mods_adventure_value.get_item_text(index)
