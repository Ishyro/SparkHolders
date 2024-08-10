extends Control

var character_name = ""

var classes = []
var races = []
var origins = []
var cultures = []
var religions = []
var educations = []

var n_ways = []
var n_dummies = []
var choices = []
var indice = 0

@onready var start_button = $Divider/Character/Buttons/Start
@onready var way_selector = $WaySelector
@onready var name_border = $Divider/Character/Name/Border
@onready var name_edit = $Divider/Character/Name/LineEdit

@onready var n_race = $Divider/Ways1/RaceContainer/Background/Divider/Race
@onready var n_origin = $Divider/Ways1/OriginContainer/Background/Divider/Origin
@onready var n_religion = $Divider/Ways1/ReligionContainer/Background/Divider/Religion
@onready var n_class = $Divider/Ways2/ClassContainer/Background/Divider/Class
@onready var n_culture = $Divider/Ways2/CultureContainer/Background/Divider/Culture
@onready var n_education = $Divider/Ways2/EducationContainer/Background/Divider/Education

@onready var n_left = $WaySelector/Background/Divider/Left
@onready var n_choices = $WaySelector/Background/Divider/Container
@onready var n_right = $WaySelector/Background/Divider/Right

@onready var baseHealth = $Divider/Character/Stats/Base/Health
@onready var baseMana = $Divider/Character/Stats/Base/Mana
@onready var baseShield = $Divider/Character/Stats/Base/Shield
@onready var baseThreshold = $Divider/Character/Stats/Base/Threshold
@onready var baseStrength = $Divider/Character/Stats/Base/Strength
@onready var baseFlow = $Divider/Character/Stats/Base/Flow
@onready var baseAttunement = $Divider/Character/Stats/Base/Attunement
@onready var baseTranscendence = $Divider/Character/Stats/Base/Transcendence

@onready var incrHealth = $Divider/Character/Stats/Increments/Health
@onready var incrMana = $Divider/Character/Stats/Increments/Mana
@onready var incrShield = $Divider/Character/Stats/Increments/Shield
@onready var incrThreshold = $Divider/Character/Stats/Increments/Threshold
@onready var incrStrength = $Divider/Character/Stats/Increments/Strength
@onready var incrFlow = $Divider/Character/Stats/Increments/Flow
@onready var incrAttunement = $Divider/Character/Stats/Increments/Attunement
@onready var incrTranscendence = $Divider/Character/Stats/Increments/Transcendence

var base_way = preload("res://menus/way.tscn")

# Called when the node enters the scene tree for the first time.
func _ready():
	Values.link.initialize(Values.ip, Settings.Port, Settings.password)
	var attributess = Values.link.getStartingAttributes()
	for way in attributess:
		classes.append(way)
	var ways = Values.link.getStartingWays()
	for way in ways:
		match(way["type"]):
			Values.WAY_RACE : races.append(way)
			Values.WAY_ORIGIN : origins.append(way)
			Values.WAY_CULTURE : cultures.append(way)
			Values.WAY_RELIGION : religions.append(way)
			Values.WAY_PROFESSION : educations.append(way)
	n_class.initialize(classes[0])
	n_race.initialize(races[0])
	n_origin.initialize(origins[0])
	n_culture.initialize(cultures[0])
	n_religion.initialize(religions[0])
	n_education.initialize(educations[0])
	update_stats()

func update_stats():
	var data = {}
	for stat in ["baseHp", "baseMana", "baseShield", "baseStrength", "baseThreshold", "baseFlow", "baseAttunement", "baseTranscendence", "hpIncr", "manaIncr", "shieldIncr", "strengthIncr", "thresholdIncr", "flowIncr", "attunementIncr", "transcendenceIncr"]:
		data[stat] = n_class.data[stat] + n_race.data[stat] + n_origin.data[stat] + n_culture.data[stat] + n_religion.data[stat] + n_education.data[stat] 
	baseHealth.text = str(data["baseHp"])
	baseMana.text = str(data["baseMana"])
	baseShield.text = str(data["baseShield"])
	if data["baseStrength"] >= 0:
		baseStrength.text = "+" + str(data["baseStrength"]) + "%"
	else:
		baseStrength.text = str(data["baseStrength"]) + "%"
	baseThreshold.text = str(data["baseThreshold"])
	baseFlow.text = str(data["baseFlow"])
	baseAttunement.text = str(data["baseAttunement"])
	baseTranscendence.text = str(data["baseTranscendence"])
	if data["hpIncr"] >= 0:
		incrHealth.text = "+" + str(data["hpIncr"])
	else:
		incrHealth.text = str(data["hpIncr"])
	if data["manaIncr"] >= 0:
		incrMana.text = "+" + str(data["manaIncr"])
	else:
		incrMana.text = str(data["manaIncr"])
	if data["shieldIncr"] >= 0:
		incrShield.text = "+" + str(data["shieldIncr"])
	else:
		incrShield.text = str(data["shieldIncr"])
	if data["strengthIncr"] >= 0:
		incrStrength.text = "+" + str(data["strengthIncr"]) + "%"
	else:
		incrStrength.text = str(data["strengthIncr"]) + "%"
	if data["thresholdIncr"] >= 0:
		incrThreshold.text = "+" + str(data["thresholdIncr"])
	else:
		incrThreshold.text = str(data["thresholdIncr"])
	if data["flowIncr"] >= 0:
		incrFlow.text = "+" + str(data["flowIncr"])
	else:
		incrFlow.text = str(data["flowIncr"])
	if data["attunementIncr"] >= 0:
		incrAttunement.text = "+" + str(data["attunementIncr"])
	else:
		incrAttunement.text = str(data["attunementIncr"])
	if data["transcendenceIncr"] >= 0:
		incrTranscendence.text = "+" + str(data["transcendenceIncr"])
	else:
		incrTranscendence.text = str(data["transcendenceIncr"])

func display_ways():
	for way in n_ways:
		n_choices.remove_child(way)
	for dummy in n_dummies:
		n_choices.remove_child(dummy)
	n_ways.clear()
	n_dummies.clear()
	var end = min(indice + 4, choices.size())
	n_left.disabled = indice == 0
	n_right.disabled = end >= choices.size()
	for i in range(indice, end):
		var way = base_way.instantiate()
		n_ways.append(way)
		n_choices.add_child(way)
		way.size_flags_vertical = SIZE_EXPAND_FILL
		way.size_flags_horizontal = SIZE_EXPAND_FILL
		way.initialize(choices[i])
		var incompatible = Values.link.getIncompatible(choices[i]["name"], n_class.data["name"], n_race.data["name"], n_origin.data["name"], n_culture.data["name"], n_religion.data["name"], n_education.data["name"])
		if incompatible != "":
			way.unvalid.visible = true
			way.tooltip_text = Values.link.getEnglishFromKey(choices[i]["name"]) + " and " + Values.link.getEnglishFromKey(incompatible) + " are incompatible."
	for i in range(end, indice + 4):
		var dummy = Control.new()
		n_dummies.append(dummy)
		n_choices.add_child(dummy)
		dummy.size_flags_vertical = SIZE_EXPAND_FILL
		dummy.size_flags_horizontal = SIZE_EXPAND_FILL
	way_selector.visible = true

func _on_start_pressed():
	Values.link.sendChoices(character_name, n_class.data["name"], n_race.data["name"], n_origin.data["name"], n_culture.data["name"], n_religion.data["name"], n_education.data["name"])
	get_tree().change_scene_to_file("res://world/world.tscn")

func _on_cancel_pressed():
	Values.link.close(Values.server_owner)
	get_tree().change_scene_to_file("res://menus/menu.tscn")

func _input(event):
	if way_selector.visible:
		if event.is_action_pressed("pause"):
			way_selector.visible = false
		if event is InputEventMouseButton and event.button_index == MouseButton.MOUSE_BUTTON_LEFT and not event.is_pressed():
			for n_way in n_ways:
				if not n_way.unvalid.visible and n_way.get_global_rect().has_point(event.global_position):
					match n_way.data["type"]:
						Values.WAY_CLASS : n_class.initialize(n_way.data)
						Values.WAY_RACE : n_race.initialize(n_way.data)
						Values.WAY_ORIGIN : n_origin.initialize(n_way.data)
						Values.WAY_CULTURE : n_culture.initialize(n_way.data)
						Values.WAY_RELIGION : n_religion.initialize(n_way.data)
						Values.WAY_PROFESSION : n_education.initialize(n_way.data)
					way_selector.visible = false
					update_stats()
					start_button.disabled = character_name == "" or n_class.data.is_empty() or n_race.data.is_empty() or n_origin.data.is_empty() or n_culture.data.is_empty() or n_religion.data.is_empty() or n_education.data.is_empty()
					break
	else:
		if event is InputEventMouseButton and event.button_index == MouseButton.MOUSE_BUTTON_LEFT and not event.is_pressed():
			if n_class.get_global_rect().has_point(event.global_position):
				indice = 0
				choices = classes
				display_ways()
			elif n_race.get_global_rect().has_point(event.global_position):
				indice = 0
				choices = races
				display_ways()
			elif n_origin.get_global_rect().has_point(event.global_position):
				indice = 0
				choices = origins
				display_ways()
			elif n_culture.get_global_rect().has_point(event.global_position):
				indice = 0
				choices = cultures
				display_ways()
			elif n_religion.get_global_rect().has_point(event.global_position):
				indice = 0
				choices = religions
				display_ways()
			elif n_education.get_global_rect().has_point(event.global_position):
				indice = 0
				choices = educations
				display_ways()

func _on_line_edit_text_changed(new_text):
	character_name = new_text
	start_button.disabled = character_name == "" or n_class.data.is_empty() or n_race.data.is_empty() or n_origin.data.is_empty() or n_culture.data.is_empty() or n_religion.data.is_empty() or n_education.data.is_empty()

func _on_name_mouse_entered():
	name_border.color = Values.gold

func _on_name_mouse_exited():
	if not name_edit.has_focus():
		name_border.color = Values.ink

func _on_line_edit_focus_entered():
	name_border.visible = true

func _on_line_edit_focus_exited():
	name_border.visible = false

func _on_left_pressed():
	indice = max(0, indice - 4)
	display_ways()

func _on_right_pressed():
	indice += 4
	display_ways()
