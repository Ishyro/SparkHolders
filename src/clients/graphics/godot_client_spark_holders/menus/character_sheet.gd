extends Control

@onready var map = $"../../Map"

@onready var concentrationMax = $Mutable/MainStats/Concentration
@onready var soulBurnThreshold = $Mutable/MainStats/SoulBurnThreshold
@onready var strengthening = $Mutable/MainStats/Strengthening
@onready var shieldMax = $Mutable/MainStats/Shield
@onready var meditation = $Mutable/MainStats/Meditation
@onready var manaMax = $Mutable/MainStats/Mana
@onready var flow = $Mutable/MainStats/Flow
@onready var healthMax = $Mutable/MainStats/Health
@onready var rawPower = $Mutable/MainStats/RawPower

@onready var n_mainClassTier = $Mutable/Classes/MainClassTier
@onready var n_subClassTier = $Mutable/Classes/SubClassTier
@onready var n_specClassTier = $Mutable/Classes/SpecClassTier
@onready var n_raceTier = $Mutable/Classes/RaceTier
@onready var n_mainClass = $Mutable/Classes/MainClass
@onready var n_subClass = $Mutable/Classes/SubClass
@onready var n_specClass = $Mutable/Classes/SpecClass
@onready var n_race = $Mutable/Classes/Race

@onready var n_originTier = $Mutable/Ways/OriginTier
@onready var n_cultureTier = $Mutable/Ways/CultureTier
@onready var n_religionTier = $Mutable/Ways/ReligionTier
@onready var n_professionTier = $Mutable/Ways/ProfessionTier
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
@onready var res_satiety = $Mutable/SubStats/Resources/Content/Values/Satiety
@onready var res_stamina = $Mutable/SubStats/Resources/Content/Values/Stamina

@onready var sec_size = $Mutable/SubStats/Secondary/Content/Values1/Size
@onready var sec_cloaking = $Mutable/SubStats/Secondary/Content/Values1/Cloaking
@onready var sec_global_speed = $Mutable/SubStats/Secondary/Content/Values1/GlobalSpeed
@onready var sec_movement_speed = $Mutable/SubStats/Secondary/Content/Values1/MovementSpeed
@onready var sec_hand_action_speed = $Mutable/SubStats/Secondary/Content/Values1/HandActionSpeed

@onready var sec_vision_power = $Mutable/SubStats/Secondary/Content/Values2/VisionPower
@onready var sec_vision_range = $Mutable/SubStats/Secondary/Content/Values2/VisionRange
@onready var sec_detection_range = $Mutable/SubStats/Secondary/Content/Values2/DetectionRange

var numbers = preload("res://hud/police/Numbers.tres")
var small_numbers = preload("res://hud/police/SmallNumbers.tres")
var big_text = preload("res://hud/police/BigText.tres")
var text = preload("res://hud/police/Text.tres")
var small_text = preload("res://hud/police/SmallText.tres")

var big_tiers = []
var small_tiers = []

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
	for i in range(0, 10):
		big_tiers.push_back(load("res://hud/tiers/" + str(i + 1) + "_big.png"))
		small_tiers.push_back(load("res://hud/tiers/" + str(i + 1) + "_small.png"))

func set_value(label, value, str_size, font_small, font_big):
	label.text = value
	if value.length() > str_size:
		label.set_label_settings(font_small)
	else:
		label.set_label_settings(font_big)

func display_stats():
	visible = true
	character_stats = Values.link.getStatsFromCharacter(Values.selected_team.id)
	origin = Values.link.getDataFromWay(character_stats["origin"])
	culture = Values.link.getDataFromWay(character_stats["culture"])
	religion = Values.link.getDataFromWay(character_stats["religion"])
	profession = Values.link.getDataFromWay(character_stats["profession"])

	mainClass = Values.link.getDataFromClass(character_stats["main_class"])
	#subClass = Values.link.getDataFromClass(character_stats["sub_class"])
	#specClass = Values.link.getDataFromClass(character_stats["spec_class"])
	race = Values.link.getDataFromRace(character_stats["race"])

	# Main
	set_value(sheet_name, character_stats["name"], 15, text, small_text)
	set_value(sheet_level, str(character_stats["level"]), 4, small_numbers, numbers)
	#set_value(concentrationMax, str(character_stats["concentration"]), 4, small_numbers, numbers)
	set_value(concentrationMax, str(0), 4, small_numbers, numbers)
	set_value(soulBurnThreshold, str(character_stats["soulBurnThreshold"]), 4, small_numbers, numbers)
	set_value(strengthening, str(character_stats["strengthening"]), 4, small_numbers, numbers)
	set_value(shieldMax, str(character_stats["maxShield"]), 4, small_numbers, numbers)
	#set_value(meditation, str(character_stats["meditation"]), 4, small_numbers, numbers)
	set_value(meditation, str(0), 4, small_numbers, numbers)
	set_value(manaMax, str(character_stats["maxMana"]), 4, small_numbers, numbers)
	set_value(flow, str(character_stats["flow"]), 4, small_numbers, numbers)
	set_value(healthMax, str(character_stats["maxHp"]), 4, small_numbers, numbers)
	set_value(rawPower, str(character_stats["rawPower"]), 4, small_numbers, numbers)
	
	experience_bar.min_value = 1000 * (character_stats["level"] - 1) * (character_stats["level"] - 1)
	experience_bar.max_value = 1000 * (character_stats["level"]) * (character_stats["level"])
	experience_bar.value = character_stats["xp"]
	
	n_mainClassTier.texture = big_tiers[mainClass["tier"] - 1]
	n_mainClass.texture = load("")
	#n_subClassTier.texture = big_tiers[subClass["tier"] - 1]
	n_subClass.texture = load("")
	#n_specClassTier.texture = big_tiers[specClass["tier"] - 1]
	n_specClass.texture = load("")
	n_raceTier.texture = big_tiers[race["tier"] - 1]
	n_race.texture = load("")
	n_originTier.texture = big_tiers[origin["tier"] - 1]
	n_origin.texture = load("")
	n_cultureTier.texture = big_tiers[culture["tier"] - 1]
	n_culture.texture = load("")
	n_religionTier.texture = big_tiers[religion["tier"] - 1]
	n_religion.texture = load("")
	n_professionTier.texture = big_tiers[profession["tier"] - 1]
	n_profession.texture = load("")
	
	# Resources
	res_shield.text = str(character_stats["shield"]) + " / " + str(character_stats["maxShield"])
	res_health.text = str(character_stats["hp"]) + " / " + str(character_stats["maxHp"])
	res_mana.text = str(character_stats["mana"]) + " / " + str(character_stats["maxMana"])
	res_soulburn.text = str(character_stats["currentSoulBurn"]) + " / " + str(character_stats["soulBurnThreshold"])
	res_satiety.text = str(map.round_float(character_stats["satiety"])) + "%"
	res_stamina.text = str(map.round_float(character_stats["stamina"])) + "%"

	# Secondaries
	sec_size.text = str(map.round_float(character_stats["size"]))
	sec_cloaking.text = str(0)
	sec_global_speed.text = str(character_stats["globalSpeed"] * 100) + "%"
	sec_movement_speed.text = str(character_stats["movementSpeed"] * 100) + "%"
	sec_hand_action_speed.text = str(character_stats["handActionSpeed"] * 100) + "%"

	sec_vision_power.text = str(character_stats["visionPower"])
	sec_vision_range.text = str(character_stats["visionRange"])
	sec_detection_range.text = str(character_stats["detectionRange"])
