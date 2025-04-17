extends Control

@onready var world = $"../../../World"

@onready var strength = $Mutable/MainStats/Strength
@onready var threshold = $Mutable/MainStats/Threshold
@onready var transcendence = $Mutable/MainStats/Transcendence
@onready var shieldMax = $Mutable/MainStats/Shield
@onready var attunement = $Mutable/MainStats/Attunement
@onready var manaMax = $Mutable/MainStats/Mana
@onready var flow = $Mutable/MainStats/Flow
@onready var healthMax = $Mutable/MainStats/Health
@onready var rawPower = $Mutable/MainStats/RawPower

@onready var n_mainClass = $Mutable/Classes/MainClass
@onready var n_subClass = $Mutable/Classes/SubClass
@onready var n_specClass = $Mutable/Classes/SpecClass
@onready var n_race = $Mutable/Classes/Race

@onready var n_origin = $Mutable/Ways/Origin
@onready var n_culture = $Mutable/Ways/Culture
@onready var n_religion = $Mutable/Ways/Religion
@onready var n_profession = $Mutable/Ways/Profession

@onready var sheet_name = $Mutable/MainNode/Name
@onready var sheet_level = $Mutable/MainNode/Level

@onready var experience_bar = $Mutable/Experience

@onready var res_shield = $Mutable/SubStats/Resources/Content/Values/Shield
@onready var res_health = $Mutable/SubStats/Resources/Content/Values/Health
@onready var res_mana = $Mutable/SubStats/Resources/Content/Values/Mana
@onready var res_soulburn = $Mutable/SubStats/Resources/Content/Values/Soulburn
@onready var res_hunger = $Mutable/SubStats/Resources/Content/Values/Hunger
@onready var res_thirst = $Mutable/SubStats/Resources/Content/Values/Thirst
@onready var res_stamina = $Mutable/SubStats/Resources/Content/Values/Stamina
@onready var res_sanity = $Mutable/SubStats/Resources/Content/Values/Sanity

@onready var sec_size = $Mutable/SubStats/Secondary/Content/Values1/Size
@onready var sec_cloaking = $Mutable/SubStats/Secondary/Content/Values1/Cloaking
@onready var sec_global_speed = $Mutable/SubStats/Secondary/Content/Values1/GlobalSpeed
@onready var sec_movement_speed = $Mutable/SubStats/Secondary/Content/Values1/MovementSpeed
@onready var sec_hand_action_speed = $Mutable/SubStats/Secondary/Content/Values1/HandActionSpeed

@onready var sec_vision_power = $Mutable/SubStats/Secondary/Content/Values2/VisionPower
@onready var sec_vision_range = $Mutable/SubStats/Secondary/Content/Values2/VisionRange
@onready var sec_detection_range = $Mutable/SubStats/Secondary/Content/Values2/DetectionRange

var numbers = preload("res://menus/hud/police/Numbers.tres")
var small_numbers = preload("res://menus/hud/police/SmallNumbers.tres")
var big_text = preload("res://menus/hud/police/BigText.tres")
var text = preload("res://menus/hud/police/Text.tres")
var small_text = preload("res://menus/hud/police/SmallText.tres")

var character_stats

var origin
var culture
var religion
var profession

var mainClass
var subClass
var specClass
var race

func _ready():
	pass

func set_value(label, value, str_size, font_small, font_big):
	label.text = value
	if value.length() > str_size:
		label.set_label_settings(font_small)
	else:
		label.set_label_settings(font_big)

func display_stats():
	visible = true
	character_stats = Values.link.getStatsFromCharacter()
	origin = Values.link.getDataFromWay(character_stats["origin"])
	culture = Values.link.getDataFromWay(character_stats["culture"])
	religion = Values.link.getDataFromWay(character_stats["religion"])
	profession = Values.link.getDataFromWay(character_stats["profession"])

	mainClass = Values.link.getDataFromClass(character_stats["main_class"])
	subClass = Values.link.getDataFromClass(character_stats["sub_class"])
	specClass = Values.link.getDataFromClass(character_stats["spec_class"])
	race = Values.link.getDataFromRace(character_stats["race"])

	# Main
	set_value(sheet_name, character_stats["name"], 15, text, small_text)
	set_value(sheet_level, str(character_stats["level"]), 4, small_numbers, numbers)
	set_value(strength, str(character_stats["strength"]), 4, small_numbers, numbers)
	set_value(threshold, str(character_stats["threshold"]), 4, small_numbers, numbers)
	set_value(transcendence, str(character_stats["transcendence"]), 4, small_numbers, numbers)
	set_value(shieldMax, str(character_stats["maxShield"]), 4, small_numbers, numbers)
	set_value(attunement, str(character_stats["attunement"]), 4, small_numbers, numbers)
	set_value(manaMax, str(character_stats["maxMana"]), 4, small_numbers, numbers)
	set_value(flow, str(character_stats["flow"]), 4, small_numbers, numbers)
	set_value(healthMax, str(character_stats["maxHp"]), 4, small_numbers, numbers)
	set_value(rawPower, str(character_stats["rawPower"]), 4, small_numbers, numbers)
	
	experience_bar.min_value = 1000 * (character_stats["level"] - 1) * (character_stats["level"] - 1)
	experience_bar.max_value = 1000 * (character_stats["level"]) * (character_stats["level"])
	experience_bar.value = character_stats["xp"]
	
	n_mainClass.texture = load(mainClass["path"])
	if !subClass.is_empty():
		n_subClass.texture = load(subClass["path"])
	if !specClass.is_empty():
		n_specClass.texture = load(specClass["path"])
	n_race.texture = load(race["path"])
	n_origin.texture = load(origin["path"])
	n_culture.texture = load(culture["path"])
	n_religion.texture = load(religion["path"])
	n_profession.texture = load(profession["path"])
	
	# Resources
	res_shield.text = str(character_stats["shield"]) + " / " + str(character_stats["maxShield"])
	res_health.text = str(character_stats["hp"]) + " / " + str(character_stats["maxHp"])
	res_mana.text = str(character_stats["mana"]) + " / " + str(character_stats["maxMana"])
	res_soulburn.text = str(character_stats["currentSoulBurn"]) + " / " + str(character_stats["threshold"])
	res_hunger.text = str(world.round_float(character_stats["hunger"])) + "%"
	res_thirst.text = str(world.round_float(character_stats["thirst"])) + "%"
	res_stamina.text = str(world.round_float(character_stats["stamina"])) + "%"
	res_sanity.text = str(world.round_float(character_stats["sanity"])) + "%"

	# Secondaries
	sec_size.text = str(world.round_float(character_stats["sizeX"]))
	sec_cloaking.text = str(0)
	sec_global_speed.text = str(character_stats["globalSpeed"] * 100) + "%"
	sec_movement_speed.text = str(character_stats["movementSpeed"] * 100) + "%"
	sec_hand_action_speed.text = str(character_stats["handActionSpeed"] * 100) + "%"

	sec_vision_power.text = str(character_stats["visionPower"])
	sec_vision_range.text = str(character_stats["visionRange"])
	sec_detection_range.text = str(character_stats["detectionRange"])

func _on_visibility_changed():
	if visible:
		Values.link.pause()
	else:
		Values.link.unpause()
