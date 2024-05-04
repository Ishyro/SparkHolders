extends Control

@onready var map = $"../Map"

@onready var block = $Block
@onready var block_description = $Block/Description

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

@onready var hour_box = $HourBox
@onready var hour_label = $HourBox/Label

@onready var inventory = $Inventory

@onready var skill_button_1 = $Skills/Skillbar/Tabs/Button1
@onready var skill_button_2 = $Skills/Skillbar/Tabs/Button2
@onready var skill_button_3 = $Skills/Skillbar/Tabs/Button3
@onready var skill_button_4 = $Skills/Skillbar/Tabs/Button4
@onready var skill_button_5 = $Skills/Skillbar/Tabs/Button5
@onready var skill_button_6 = $Skills/Skillbar/Tabs/Button6
@onready var skill_button_7 = $Skills/Skillbar/Tabs/Button7
@onready var skill_button_8 = $Skills/Skillbar/Tabs/Button8
@onready var skill_button_9 = $Skills/Skillbar/Tabs/Button9
@onready var skill_button_10 = $Skills/Skillbar/Tabs/Button10
@onready var skill_button_11 = $Skills/Skillbar/Tabs/Button11
@onready var skill_button_12 = $Skills/Skillbar/Tabs/Button12

var skill_slots = []

var small_numbers = preload("res://menus/hud/police/SmallNumbers.tres")
var numbers = preload("res://menus/hud/police/Numbers.tres")
var numbers_box = preload("res://menus/hud/police/NumbersBox.tres")
var numbers_white = preload("res://menus/hud/police/NumbersWhite.tres")
var small_text = preload("res://menus/hud/police/SmallText.tres")
var text = preload("res://menus/hud/police/Text.tres")
var big_text = preload("res://menus/hud/police/BigText.tres")

var big_tiers = []
var small_tiers = []

var base_skill = preload("res://menus/hud/skill.tscn")

var skills = {}
var current_skills = []
var selected_skill
var current_skill_slot = 0
var selected_skill_slot = 0

func _ready():
	characters_button.set_pressed_no_signal(true)
	projectiles_button.set_pressed_no_signal(true)
	furnitures_button.set_pressed_no_signal(true)
	skill_button_1.set_pressed_no_signal(true)
	for i in range(1, 11):
		big_tiers.push_back(load("res://menus/hud/tiers/" + str(i) + "_big.png"))
		small_tiers.push_back(load("res://menus/hud/tiers/" + str(i) + "_small.png"))
	for i in range(0, 12):
		current_skills.push_back([])
		for j in range(0, 13):
			current_skills[i].push_back(base_skill.instantiate())
	for i in range(1, 14):
		skill_slots.push_back(get_node("Skills/Skillbar/Slots/Skill" + str(i)))
		skill_slots[i - 1].pos = i - 1
	current_skills[0][0].texture = $Skills/Skillbar/Slots/Skill1.texture
	current_skills[0][0].data = $Skills/Skillbar/Slots/Skill1.data
	current_skills[0][1].texture = $Skills/Skillbar/Slots/Skill2.texture
	current_skills[0][1].data = $Skills/Skillbar/Slots/Skill2.data

func change_amount(type: String, current_value: float, max_value: float):
	if type == "SHIELD":
		shield.material.set_shader_parameter("level", current_value / max_value)
		shield_label.text = str(int(floor(current_value)))
	if type == "HEALTH":
		health.material.set_shader_parameter("level", current_value / max_value)
		health_label.text = str(int(floor(current_value)))
	if type == "MANA":
		mana.material.set_shader_parameter("level", current_value / max_value)
		mana_label.text = str(int(floor(current_value)))
	if type == "SOULBURN":
		soulburn.material.set_shader_parameter("level", current_value / max_value)
		soulburn_label.text = str(int(floor(current_value)))

func hide_phantom(character_id: int):
	map.phantoms[Values.selected_team.id].visible = false
	if map.phantom_lines.has(character_id):
		for phantom_line in map.phantom_lines[character_id]:
			phantom_line.visible = false

func display_projectile(_projectile, _projectile_data: Dictionary):
	pass

func display_target(character, _character_data: Dictionary):
	character.select()
	block.visible = false

func display_team(character, character_data: Dictionary):
	character.select()
	change_amount("SHIELD", character_data["shield"], character_data["maxShield"])
	change_amount("HEALTH", character_data["hp"], character_data["maxHp"])
	change_amount("MANA", character_data["mana"], character_data["maxMana"])
	change_amount("SOULBURN", character_data["currentSoulBurn"], character_data["soulBurnThreshold"])

func display_block(_block_object, _block_data: Dictionary):
	#target.visible = false
	block.visible = true
	
func update_mouse_box(mouse_coord: Vector2, ap_cost: String):
	mouse_box._set_position(Vector2(mouse_coord.x - mouse_box.get_size().x / 2.0, mouse_coord.y))
	if Values.selected_team:
		mouse_label.text = "(" + String.num(Values.coord.z, 3) + ", " + String.num(Values.coord.x, 3) + ", " + String.num(Values.coord.y, 3) + ")\n" + ap_cost
	else:
		mouse_label.text = "(" + String.num(Values.coord.z, 3) + ", " + String.num(Values.coord.x, 3) + ", " + String.num(Values.coord.y, 3) + ")"

func update(character, character_data: Dictionary):
	hour_label.text = "" + Values.link.getClock()
	change_amount("SHIELD", character_data["shield"], character_data["maxShield"])
	change_amount("HEALTH", character_data["hp"], character_data["maxHp"])
	change_amount("MANA", character_data["mana"], character_data["maxMana"])
	change_amount("SOULBURN", character_data["currentSoulBurn"], character_data["soulBurnThreshold"])

# Map options
func _on_grid_toggled(_button_pressed):
	map.n_grid.visible = !map.n_grid.visible

func _on_lights_toggled(_button_pressed):
	map.n_lights.visible = !map.n_lights.visible
	map.n_blocks.visible = !map.n_blocks.visible

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

# Skills
func _get_drag_data(at_position):
	for skill in skills:
		if skills[skill].get_global_rect().has_point(at_position):
			var preview = Control.new()
			var preview_skill = base_skill.instantiate()
			preview_skill.texture = skills[skill].texture
			preview_skill.size = skills[skill].size
			preview_skill.data = skills[skill].data
			preview.add_child(preview_skill)
			set_drag_preview(preview)
			return preview_skill
	for skill in skill_slots:
		if skill.get_global_rect().has_point(at_position):
			selected_skill = skill
			selected_skill_slot = current_skill_slot
			var preview = Control.new()
			var preview_skill = base_skill.instantiate()
			preview_skill.texture = skill.texture
			preview_skill.size = skill.size
			preview_skill.data = skill.data
			preview.add_child(preview_skill)
			set_drag_preview(preview)
			return preview_skill

func _can_drop_data(at_position, _data):
	var current_slot = null
	for slot in skill_slots:
		if slot.get_global_rect().has_point(at_position):
			current_slot = slot
			break
	return current_slot != null

func _drop_data(at_position, data):
	for slot in skill_slots:
		if slot.get_global_rect().has_point(at_position):
			if slot.texture != null && selected_skill != null:
				if selected_skill_slot == current_skill_slot:
					selected_skill.texture = slot.texture
					selected_skill.data = slot.data
				current_skills[selected_skill_slot][selected_skill.pos].texture = slot.texture
				current_skills[selected_skill_slot][selected_skill.pos].data = slot.data
			elif selected_skill != null:
				if selected_skill_slot == current_skill_slot:
					selected_skill.texture = null
					selected_skill.data = {}
				current_skills[selected_skill_slot][selected_skill.pos].texture = null
				current_skills[selected_skill_slot][selected_skill.pos].data = {}
				selected_skill = null
			slot.texture = data.texture
			slot.data = data.data
			current_skills[current_skill_slot][slot.pos].texture = data.texture
			current_skills[current_skill_slot][slot.pos].data = data.data
			return

func _on_button_1_toggled(button_pressed):
	if button_pressed:
		skill_button_2.set_pressed_no_signal(false)
		skill_button_3.set_pressed_no_signal(false)
		skill_button_4.set_pressed_no_signal(false)
		skill_button_5.set_pressed_no_signal(false)
		skill_button_6.set_pressed_no_signal(false)
		skill_button_7.set_pressed_no_signal(false)
		skill_button_8.set_pressed_no_signal(false)
		skill_button_9.set_pressed_no_signal(false)
		skill_button_10.set_pressed_no_signal(false)
		skill_button_11.set_pressed_no_signal(false)
		skill_button_12.set_pressed_no_signal(false)
		current_skill_slot = 0
		var i = 0
		for skill in current_skills[current_skill_slot]:
			skill_slots[i].texture = skill.texture
			skill_slots[i].data = skill.data
			i += 1
	else:
		skill_button_1.set_pressed_no_signal(true)

func _on_button_2_toggled(button_pressed):
	if button_pressed:
		skill_button_1.set_pressed_no_signal(false)
		skill_button_3.set_pressed_no_signal(false)
		skill_button_4.set_pressed_no_signal(false)
		skill_button_5.set_pressed_no_signal(false)
		skill_button_6.set_pressed_no_signal(false)
		skill_button_7.set_pressed_no_signal(false)
		skill_button_8.set_pressed_no_signal(false)
		skill_button_9.set_pressed_no_signal(false)
		skill_button_10.set_pressed_no_signal(false)
		skill_button_11.set_pressed_no_signal(false)
		skill_button_12.set_pressed_no_signal(false)
		current_skill_slot = 1
		var i = 0
		for skill in current_skills[current_skill_slot]:
			skill_slots[i].texture = skill.texture
			skill_slots[i].data = skill.data
			i += 1
	else:
		skill_button_2.set_pressed_no_signal(true)
		
func _on_button_3_toggled(button_pressed):
	if button_pressed:
		skill_button_1.set_pressed_no_signal(false)
		skill_button_2.set_pressed_no_signal(false)
		skill_button_4.set_pressed_no_signal(false)
		skill_button_5.set_pressed_no_signal(false)
		skill_button_6.set_pressed_no_signal(false)
		skill_button_7.set_pressed_no_signal(false)
		skill_button_8.set_pressed_no_signal(false)
		skill_button_9.set_pressed_no_signal(false)
		skill_button_10.set_pressed_no_signal(false)
		skill_button_11.set_pressed_no_signal(false)
		skill_button_12.set_pressed_no_signal(false)
		current_skill_slot = 2
		var i = 0
		for skill in current_skills[current_skill_slot]:
			skill_slots[i].texture = skill.texture
			skill_slots[i].data = skill.data
			i += 1
	else:
		skill_button_3.set_pressed_no_signal(true)

func _on_button_4_toggled(button_pressed):
	if button_pressed:
		skill_button_1.set_pressed_no_signal(false)
		skill_button_2.set_pressed_no_signal(false)
		skill_button_3.set_pressed_no_signal(false)
		skill_button_5.set_pressed_no_signal(false)
		skill_button_6.set_pressed_no_signal(false)
		skill_button_7.set_pressed_no_signal(false)
		skill_button_8.set_pressed_no_signal(false)
		skill_button_9.set_pressed_no_signal(false)
		skill_button_10.set_pressed_no_signal(false)
		skill_button_11.set_pressed_no_signal(false)
		skill_button_12.set_pressed_no_signal(false)
		current_skill_slot = 3
		var i = 0
		for skill in current_skills[current_skill_slot]:
			skill_slots[i].texture = skill.texture
			skill_slots[i].data = skill.data
			i += 1
	else:
		skill_button_4.set_pressed_no_signal(true)

func _on_button_5_toggled(button_pressed):
	if button_pressed:
		skill_button_1.set_pressed_no_signal(false)
		skill_button_2.set_pressed_no_signal(false)
		skill_button_3.set_pressed_no_signal(false)
		skill_button_4.set_pressed_no_signal(false)
		skill_button_6.set_pressed_no_signal(false)
		skill_button_7.set_pressed_no_signal(false)
		skill_button_8.set_pressed_no_signal(false)
		skill_button_9.set_pressed_no_signal(false)
		skill_button_10.set_pressed_no_signal(false)
		skill_button_11.set_pressed_no_signal(false)
		skill_button_12.set_pressed_no_signal(false)
		current_skill_slot = 4
		var i = 0
		for skill in current_skills[current_skill_slot]:
			skill_slots[i].texture = skill.texture
			skill_slots[i].data = skill.data
			i += 1
	else:
		skill_button_5.set_pressed_no_signal(true)

func _on_button_6_toggled(button_pressed):
	if button_pressed:
		skill_button_1.set_pressed_no_signal(false)
		skill_button_2.set_pressed_no_signal(false)
		skill_button_3.set_pressed_no_signal(false)
		skill_button_4.set_pressed_no_signal(false)
		skill_button_5.set_pressed_no_signal(false)
		skill_button_7.set_pressed_no_signal(false)
		skill_button_8.set_pressed_no_signal(false)
		skill_button_9.set_pressed_no_signal(false)
		skill_button_10.set_pressed_no_signal(false)
		skill_button_11.set_pressed_no_signal(false)
		skill_button_12.set_pressed_no_signal(false)
		current_skill_slot = 5
		var i = 0
		for skill in current_skills[current_skill_slot]:
			skill_slots[i].texture = skill.texture
			skill_slots[i].data = skill.data
			i += 1
	else:
		skill_button_6.set_pressed_no_signal(true)

func _on_button_7_toggled(button_pressed):
	if button_pressed:
		skill_button_1.set_pressed_no_signal(false)
		skill_button_2.set_pressed_no_signal(false)
		skill_button_3.set_pressed_no_signal(false)
		skill_button_4.set_pressed_no_signal(false)
		skill_button_5.set_pressed_no_signal(false)
		skill_button_6.set_pressed_no_signal(false)
		skill_button_8.set_pressed_no_signal(false)
		skill_button_9.set_pressed_no_signal(false)
		skill_button_10.set_pressed_no_signal(false)
		skill_button_11.set_pressed_no_signal(false)
		skill_button_12.set_pressed_no_signal(false)
		current_skill_slot = 6
		var i = 0
		for skill in current_skills[current_skill_slot]:
			skill_slots[i].texture = skill.texture
			skill_slots[i].data = skill.data
			i += 1
	else:
		skill_button_7.set_pressed_no_signal(true)

func _on_button_8_toggled(button_pressed):
	if button_pressed:
		skill_button_1.set_pressed_no_signal(false)
		skill_button_2.set_pressed_no_signal(false)
		skill_button_3.set_pressed_no_signal(false)
		skill_button_4.set_pressed_no_signal(false)
		skill_button_5.set_pressed_no_signal(false)
		skill_button_6.set_pressed_no_signal(false)
		skill_button_7.set_pressed_no_signal(false)
		skill_button_9.set_pressed_no_signal(false)
		skill_button_10.set_pressed_no_signal(false)
		skill_button_11.set_pressed_no_signal(false)
		skill_button_12.set_pressed_no_signal(false)
		current_skill_slot = 7
		var i = 0
		for skill in current_skills[current_skill_slot]:
			skill_slots[i].texture = skill.texture
			skill_slots[i].data = skill.data
			i += 1
	else:
		skill_button_8.set_pressed_no_signal(true)

func _on_button_9_toggled(button_pressed):
	if button_pressed:
		skill_button_1.set_pressed_no_signal(false)
		skill_button_2.set_pressed_no_signal(false)
		skill_button_3.set_pressed_no_signal(false)
		skill_button_4.set_pressed_no_signal(false)
		skill_button_5.set_pressed_no_signal(false)
		skill_button_6.set_pressed_no_signal(false)
		skill_button_7.set_pressed_no_signal(false)
		skill_button_8.set_pressed_no_signal(false)
		skill_button_10.set_pressed_no_signal(false)
		skill_button_11.set_pressed_no_signal(false)
		skill_button_12.set_pressed_no_signal(false)
		current_skill_slot = 8
		var i = 0
		for skill in current_skills[current_skill_slot]:
			skill_slots[i].texture = skill.texture
			skill_slots[i].data = skill.data
			i += 1
	else:
		skill_button_9.set_pressed_no_signal(true)

func _on_button_10_toggled(button_pressed):
	if button_pressed:
		skill_button_1.set_pressed_no_signal(false)
		skill_button_2.set_pressed_no_signal(false)
		skill_button_3.set_pressed_no_signal(false)
		skill_button_4.set_pressed_no_signal(false)
		skill_button_5.set_pressed_no_signal(false)
		skill_button_6.set_pressed_no_signal(false)
		skill_button_7.set_pressed_no_signal(false)
		skill_button_8.set_pressed_no_signal(false)
		skill_button_9.set_pressed_no_signal(false)
		skill_button_11.set_pressed_no_signal(false)
		skill_button_12.set_pressed_no_signal(false)
		current_skill_slot = 9
		var i = 0
		for skill in current_skills[current_skill_slot]:
			skill_slots[i].texture = skill.texture
			skill_slots[i].data = skill.data
			i += 1
	else:
		skill_button_10.set_pressed_no_signal(true)

func _on_button_11_toggled(button_pressed):
	if button_pressed:
		skill_button_1.set_pressed_no_signal(false)
		skill_button_2.set_pressed_no_signal(false)
		skill_button_3.set_pressed_no_signal(false)
		skill_button_4.set_pressed_no_signal(false)
		skill_button_5.set_pressed_no_signal(false)
		skill_button_6.set_pressed_no_signal(false)
		skill_button_7.set_pressed_no_signal(false)
		skill_button_8.set_pressed_no_signal(false)
		skill_button_9.set_pressed_no_signal(false)
		skill_button_10.set_pressed_no_signal(false)
		skill_button_12.set_pressed_no_signal(false)
		current_skill_slot = 10
		var i = 0
		for skill in current_skills[current_skill_slot]:
			skill_slots[i].texture = skill.texture
			skill_slots[i].data = skill.data
			i += 1
	else:
		skill_button_11.set_pressed_no_signal(true)

func _on_button_12_toggled(button_pressed):
	if button_pressed:
		skill_button_1.set_pressed_no_signal(false)
		skill_button_2.set_pressed_no_signal(false)
		skill_button_3.set_pressed_no_signal(false)
		skill_button_4.set_pressed_no_signal(false)
		skill_button_5.set_pressed_no_signal(false)
		skill_button_6.set_pressed_no_signal(false)
		skill_button_7.set_pressed_no_signal(false)
		skill_button_8.set_pressed_no_signal(false)
		skill_button_9.set_pressed_no_signal(false)
		skill_button_10.set_pressed_no_signal(false)
		skill_button_11.set_pressed_no_signal(false)
		current_skill_slot = 11
		var i = 0
		for skill in current_skills[current_skill_slot]:
			skill_slots[i].texture = skill.texture
			skill_slots[i].data = skill.data
			i += 1
	else:
		skill_button_12.set_pressed_no_signal(true)
