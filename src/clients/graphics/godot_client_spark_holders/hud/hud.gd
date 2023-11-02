extends Control

@onready var map = $"../Map"

@onready var tile = $Tile
@onready var tile_description = $Tile/Description

@onready var grid_button = $MapOptions/Options/Grid
@onready var lights_button = $MapOptions/Options/Lights
@onready var characters_button = $MapOptions/Options/Characters
@onready var projectiles_button = $MapOptions/Options/Projectiles
@onready var furnitures_button = $MapOptions/Options/Furnitures

@onready var move = $ActionButtons/Move
@onready var attack = $ActionButtons/Attack
@onready var interact = $ActionButtons/Interact
@onready var rest = $ActionButtons/Rest

@onready var shield = $Orbs/Shield
@onready var health = $Orbs/Health
@onready var mana = $Orbs/Mana
@onready var soulburn = $Orbs/SoulBurn

@onready var shield_label = $Orbs/Shield/Label
@onready var health_label = $Orbs/Health/Label
@onready var mana_label = $Orbs/Mana/Label
@onready var soulburn_label = $Orbs/SoulBurn/Label

@onready var mouse_box = $MouseBox
@onready var mouse_label = $MouseBox/Label

var numbers = preload("res://hud/police/Numbers.tres")
var small_numbers = preload("res://hud/police/SmallNumbers.tres")
var text = preload("res://hud/police/Text.tres")
var big_text = preload("res://hud/police/BigText.tres")

var big_tiers = []
var small_tiers = []

# Called when the node enters the scene tree for the first time.
func _ready():
	characters_button.set_pressed_no_signal(true)
	projectiles_button.set_pressed_no_signal(true)
	furnitures_button.set_pressed_no_signal(true)
	for i in range(0, 10):
		big_tiers.push_back(load("res://hud/tiers/" + str(i + 1) + "_big.png"))
		small_tiers.push_back(load("res://hud/tiers/" + str(i + 1) + "_small.png"))

func change_amount(type: String, current_value: float, max_value: float):
	if type == "SHIELD":
		shield.material.set_shader_parameter("level", current_value / max_value)
		shield_label.text = str(current_value)
	if type == "HEALTH":
		health.material.set_shader_parameter("level", current_value / max_value)
		health_label.text = str(current_value)
	if type == "MANA":
		mana.material.set_shader_parameter("level", current_value / max_value)
		mana_label.text = str(current_value)
	if type == "SOULBURN":
		soulburn.material.set_shader_parameter("level", current_value / max_value)
		soulburn_label.text = str(current_value)

func hide_phantom(character_id: int):
	map.phantoms[Values.selected_team.id].visible = false
	if map.phantom_lines.has(character_id):
		for phantom_line in map.phantom_lines[character_id]:
			phantom_line.visible = false

func set_skill_tab(n: int):
	$Skills/Tabs.set_current_tab(n)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass

func display_projectile(_projectile, _projectile_data: Dictionary):
	pass

func display_target(character, _character_data: Dictionary):
	character.select()
	tile.visible = false

func display_team(character, character_data: Dictionary):
	character.select()
	change_amount("SHIELD", character_data["shield"], character_data["maxShield"])
	change_amount("HEALTH", character_data["hp"], character_data["maxHp"])
	change_amount("MANA", character_data["mana"], character_data["maxMana"])
	change_amount("SOULBURN", character_data["currentSoulBurn"], character_data["soulBurnThreshold"])

func display_inventory():
	pass

func display_tile(_tile_object, _tile_data: Dictionary):
	#target.visible = false
	tile.visible = true
	
func update_mouse_box(mouse_coord: Vector2, ap_cost: String):
	mouse_box._set_position(Vector2(mouse_coord.x - mouse_box.get_size().x / 2.0, mouse_coord.y))
	if Values.selected_team:
		mouse_label.text = "(" + String.num(Values.coord.z, 3) + " , " + String.num(Values.coord.x, 3) + ")\n" + ap_cost
	else:
		mouse_label.text = "(" + String.num(Values.coord.z, 3) + " , " + String.num(Values.coord.x, 3) + ")"

# Map options
func _on_grid_toggled(_button_pressed):
	map.n_grid.visible = !map.n_grid.visible

func _on_lights_toggled(_button_pressed):
	map.n_lights.visible = !map.n_lights.visible
	map.n_tiles.visible = !map.n_tiles.visible

func _on_characters_toggled(_button_pressed):
	map.n_characters.visible = !map.n_characters.visible

func _on_projectiles_toggled(_button_pressed):
	map.n_projectiles.visible = !map.n_projectiles.visible

func _on_furnitures_toggled(_button_pressed):
	map.n_furnitures.visible = !map.n_furnitures.visible

func _on_move_toggled(button_pressed):
	if button_pressed:
		attack.set_pressed_no_signal(false)
		interact.set_pressed_no_signal(false)
		rest.set_pressed_no_signal(false)
		Values.mode = Values.ACTION_MODE_MOVE
		map.phantoms[Values.selected_team.id].visible = true
		map.characters[Values.selected_team.id].range_mesh.visible = false
		map.set_navigation_mesh(Values.selected_team.id)
	else:
		move.set_pressed_no_signal(true)

func _on_attack_toggled(button_pressed):
	if button_pressed:
		move.set_pressed_no_signal(false)
		interact.set_pressed_no_signal(false)
		rest.set_pressed_no_signal(false)
		Values.mode = Values.ACTION_MODE_ATTACK
		hide_phantom(Values.selected_team.id)
		map.characters[Values.selected_team.id].range_mesh.visible = true
	else:
		map.characters[Values.selected_team.id].range_mesh.visible = false
		move.set_pressed(true)

func _on_interact_toggled(button_pressed):
	if button_pressed:
		move.set_pressed_no_signal(false)
		attack.set_pressed_no_signal(false)
		rest.set_pressed_no_signal(false)
		Values.mode = Values.ACTION_MODE_ACTIVATION
		hide_phantom(Values.selected_team.id)
		map.characters[Values.selected_team.id].range_mesh.visible = false
	else:
		move.set_pressed(true)

func _on_rest_toggled(button_pressed):
	if button_pressed:
		move.set_pressed_no_signal(false)
		attack.set_pressed_no_signal(false)
		interact.set_pressed_no_signal(false)
		Values.mode = Values.ACTION_MODE_WAIT
		hide_phantom(Values.selected_team.id)
		map.characters[Values.selected_team.id].range_mesh.visible = false
	else:
		move.set_pressed(true)
