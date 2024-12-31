extends Control

@onready var map = $"../Map"

@onready var shield = $Orbs/Divider/Body/Shield/Shader
@onready var health = $Orbs/Divider/Body/Health/Shader
@onready var mana = $Orbs/Divider/Soul/Mana/Shader
@onready var soulburn = $Orbs/Divider/Soul/SoulBurn/Shader

@onready var shield_label = $Orbs/Divider/Body/Shield/Label
@onready var health_label = $Orbs/Divider/Body/Health/Label
@onready var mana_label = $Orbs/Divider/Soul/Mana/Label
@onready var soulburn_label = $Orbs/Divider/Soul/SoulBurn/Label

@onready var hour_box = $HourBox
@onready var hour_label = $HourBox/Label

@onready var inventory = $Inventory
@onready var skillbook = $Skills/SkillBook

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

var skills = []

var fire_count = 0
var air_count = 0
var water_count = 0
var earth_count = 0

var current_skills = []
var selected_skill
var current_skill_slot = 0
var selected_skill_slot = 0

func _ready():
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
	for i in range(1, 128):
		skills.push_back(get_node("Skills/SkillBook/MarginContainer/Tabs/Fire/Skill" + str(i)))
	for i in range(1, 128):
		skills.push_back(get_node("Skills/SkillBook/MarginContainer/Tabs/Air/Skill" + str(i)))
	for i in range(1, 128):
		skills.push_back(get_node("Skills/SkillBook/MarginContainer/Tabs/Water/Skill" + str(i)))
	for i in range(1, 128):
		skills.push_back(get_node("Skills/SkillBook/MarginContainer/Tabs/Earth/Skill" + str(i)))

func change_amount(type: String, current_value: float, max_value: float):
	if type == "SHIELD":
		shield.material.set_shader_parameter("level", current_value / max_value)
		shield_label.text = str(int(ceil(current_value)))
	if type == "HEALTH":
		health.material.set_shader_parameter("level", current_value / max_value)
		health_label.text = str(int(ceil(current_value)))
	if type == "MANA":
		mana.material.set_shader_parameter("level", current_value / max_value)
		mana_label.text = str(int(ceil(current_value)))
	if type == "SOULBURN":
		soulburn.material.set_shader_parameter("level", current_value / max_value)
		soulburn_label.text = str(int(ceil(current_value)))

#func update_mouse_box(mouse_coord: Vector2):
	#mouse_box._set_position(Vector2(mouse_coord.x - mouse_box.get_size().x / 2.0, mouse_coord.y))
	#mouse_label.text = "(" + String.num(Values.coord.z, 3) + ", " + String.num(Values.coord.x, 3) + ", " + String.num(Values.coord.y, 3) + ")"

func update(character_data: Dictionary):
	hour_label.text = "" + Values.link.getClock(Settings.TerranDay)
	change_amount("SHIELD", character_data["shield"], character_data["maxShield"])
	change_amount("HEALTH", character_data["hp"], character_data["maxHp"])
	change_amount("MANA", character_data["mana"], character_data["maxMana"])
	change_amount("SOULBURN", character_data["currentSoulBurn"], character_data["soulBurnThreshold"])

# Skills
func display_skillbook():
	for i in range(1, fire_count):
		get_node("Skills/SkillBook/MarginContainer/Tabs/Fire/Skill" + str(i)).clean()
	for i in range(1, air_count):
		get_node("Skills/SkillBook/MarginContainer/Tabs/Air/Skill" + str(i)).clean()
	for i in range(1, water_count):
		get_node("Skills/SkillBook/MarginContainer/Tabs/Water/Skill" + str(i)).clean()
	for i in range(1, earth_count):
		get_node("Skills/SkillBook/MarginContainer/Tabs/Earth/Skill" + str(i)).clean()
	fire_count = 0
	air_count = 0
	water_count = 0
	earth_count = 0
	for skill in Values.link.getSkillsFromCharacter():
		if skill["school"] == Values.macros["SKILL_FIRE"]:
			fire_count+=1
			get_node("Skills/SkillBook/MarginContainer/Tabs/Fire/Skill" + str(fire_count)).initialize(skill)
	skillbook.visible = true

func _on_skillbook_close_pressed():
	Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)
	Values.free_mouse_state = false
	skillbook.visible = false

func get_skill(number: int):
	return current_skills[current_skill_slot][number].data

func _get_drag_data(at_position):
	for skill in skills:
		if skill.get_global_rect().has_point(at_position):
			var preview = Control.new()
			var preview_skill = base_skill.instantiate()
			preview_skill.texture = skill.texture
			preview_skill.size = skill.size
			preview_skill.data = skill.data
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
	return current_slot != null or skillbook.get_global_rect().has_point(at_position)

func _drop_data(at_position, data):
	if selected_skill != null:
		current_skills[selected_skill_slot][selected_skill.pos].clean()
		selected_skill.clean()
	for slot in skill_slots:
		if slot.get_global_rect().has_point(at_position):
			if selected_skill != null:
				if slot.texture != null:
					if selected_skill_slot == current_skill_slot:
						selected_skill.initialize(slot.data)
					current_skills[selected_skill_slot][selected_skill.pos].initialize(slot.data)
				selected_skill = null
			slot.initialize(data.data)
			current_skills[current_skill_slot][slot.pos].initialize(data.data)
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
