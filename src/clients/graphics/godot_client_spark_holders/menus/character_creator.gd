extends Control

var character_name = ""

var classes = []
var races = []
var origins = []
var cultures = []
var religions = []
var professions = []

var n_ways = []
var n_ways_up = []
var n_ways_down = []

@onready var start_button = $Divider/Character/Buttons/Start
@onready var way_selector = $WaySelector

@onready var n_class = $Divider/Ways/Class
@onready var n_race = $Divider/Ways2/Race
@onready var n_origin = $Divider/Ways/Origin
@onready var n_culture = $Divider/Ways2/Culture
@onready var n_religion = $Divider/Ways/Religion
@onready var n_profession = $Divider/Ways2/Profession

@onready var n_up = $WaySelector/Background/ScrollContainer/VBoxContainer/HBoxContainer
@onready var n_down = $WaySelector/Background/ScrollContainer/VBoxContainer/HBoxContainer2

@onready var baseHp = $Divider/Character/Stats/Base/Hp
@onready var baseMana = $Divider/Character/Stats/Base/Mana
@onready var baseShield = $Divider/Character/Stats/Base/Shield
@onready var baseSoulBurn = $Divider/Character/Stats/Base/SoulBurn
@onready var baseDamage = $Divider/Character/Stats/Base/Damage
@onready var baseFlow = $Divider/Character/Stats/Base/Flow

@onready var incrHp = $Divider/Character/Stats/Increments/Hp
@onready var incrMana = $Divider/Character/Stats/Increments/Mana
@onready var incrShield = $Divider/Character/Stats/Increments/Shield
@onready var incrSoulBurn = $Divider/Character/Stats/Increments/SoulBurn
@onready var incrDamage = $Divider/Character/Stats/Increments/Damage
@onready var incrFlow = $Divider/Character/Stats/Increments/Flow

var base_way = preload("res://menus/way.tscn")

# Called when the node enters the scene tree for the first time.
func _ready():
	Values.link.initialize(Values.ip, Settings.Port, Settings.Lang)
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
			Values.WAY_PROFESSION : professions.append(way)
	n_class.initialize(classes[0])
	n_race.initialize(races[0])
	n_origin.initialize(origins[0])
	n_culture.initialize(cultures[0])
	n_religion.initialize(religions[0])
	n_profession.initialize(professions[0])
	update_stats()
	$Divider/Character/Name.grab_focus.call_deferred()
	
func update_stats():
	var data = {}
	for stat in ["baseHp", "baseMana", "baseShield", "baseDamageMult", "baseSoulBurn", "baseFlow", "hpIncr", "manaIncr", "shieldIncr", "damageMultIncr", "soulBurnIncr", "flowIncr"]:
		data[stat] = n_class.data[stat] + n_race.data[stat] + n_origin.data[stat] + n_culture.data[stat] + n_religion.data[stat] + n_profession.data[stat] 
	baseHp.text = str(data["baseHp"])
	baseMana.text = str(data["baseMana"])
	baseShield.text = str(data["baseShield"])
	if data["baseDamageMult"] >= 0:
		baseDamage.text = "+" + str(data["baseDamageMult"]) + "%"
	else:
		baseDamage.text = str(data["baseDamageMult"]) + "%"
	baseSoulBurn.text = str(data["baseSoulBurn"])
	baseFlow.text = str(data["baseFlow"])
	if data["hpIncr"] >= 0:
		incrHp.text = "+" + str(data["hpIncr"])
	else:
		incrHp.text = str(data["hpIncr"])
	if data["manaIncr"] >= 0:
		incrMana.text = "+" + str(data["manaIncr"])
	else:
		incrMana.text = str(data["manaIncr"])
	if data["shieldIncr"] >= 0:
		incrShield.text = "+" + str(data["shieldIncr"])
	else:
		incrShield.text = str(data["shieldIncr"])
	if data["damageMultIncr"] >= 0:
		incrDamage.text = "+" + str(data["damageMultIncr"]) + "%"
	else:
		incrDamage.text = str(data["damageMultIncr"]) + "%"
	if data["soulBurnIncr"] >= 0:
		incrSoulBurn.text = "+" + str(data["soulBurnIncr"])
	else:
		incrSoulBurn.text = str(data["soulBurnIncr"])
	if data["flowIncr"] >= 0:
		incrFlow.text = "+" + str(data["flowIncr"])
	else:
		incrFlow.text = str(data["flowIncr"])

func add_ways(datas: Array):
	for n_way in n_ways_up:
			n_up.remove_child(n_way)
	for n_way in n_ways_down:
			n_down.remove_child(n_way)
	n_ways.clear()
	n_ways_up.clear()
	n_ways_down.clear()
	var up = true
	for data in datas:
		var way = base_way.instantiate()
		n_ways.append(way)
		if up :
			n_up.add_child(way)
			n_ways_up.append(way)
			up = false
		else:
			n_down.add_child(way)
			n_ways_down.append(way)
			up = true
		way.initialize(data)
		if not Values.link.isCompatible(data["name"], n_class.data["name"], n_race.data["name"], n_origin.data["name"], n_culture.data["name"], n_religion.data["name"], n_profession.data["name"]):
			way.unvalid.visible = true

func _on_start_pressed():
	Values.link.sendChoices(character_name, n_class.data["name"], n_race.data["name"], n_origin.data["name"], n_culture.data["name"], n_religion.data["name"], n_profession.data["name"])
	get_tree().change_scene_to_file("res://world/world.tscn")

func _on_cancel_pressed():
	Values.link.close()
	if Values.server_pid != -1:
		OS.kill(Values.server_pid)
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
						Values.WAY_PROFESSION : n_profession.initialize(n_way.data)
					way_selector.visible = false
					update_stats()
					start_button.disabled = character_name == "" or n_class.data.is_empty() or n_race.data.is_empty() or n_origin.data.is_empty() or n_culture.data.is_empty() or n_religion.data.is_empty() or n_profession.data.is_empty()
					break
	else:
		if event is InputEventMouseButton and event.button_index == MouseButton.MOUSE_BUTTON_LEFT and not event.is_pressed():
			if n_class.get_global_rect().has_point(event.global_position):
				add_ways(classes)
				way_selector.visible = true
			elif n_race.get_global_rect().has_point(event.global_position):
				add_ways(races)
				way_selector.visible = true
			elif n_origin.get_global_rect().has_point(event.global_position):
				add_ways(origins)
				way_selector.visible = true
			elif n_culture.get_global_rect().has_point(event.global_position):
				add_ways(cultures)
				way_selector.visible = true
			elif n_religion.get_global_rect().has_point(event.global_position):
				add_ways(religions)
				way_selector.visible = true
			elif n_profession.get_global_rect().has_point(event.global_position):
				add_ways(professions)
				way_selector.visible = true

func _on_name_text_submitted(new_text):
	character_name = new_text
	start_button.disabled = character_name == "" or n_class.data.is_empty() or n_race.data.is_empty() or n_origin.data.is_empty() or n_culture.data.is_empty() or n_religion.data.is_empty() or n_profession.data.is_empty()
