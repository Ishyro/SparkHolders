extends Control

var data = {}

@onready var title = $Background/Content/Name
@onready var picture = $Background/Content/Divider/Picture
@onready var unvalid = $Unvalid
@onready var border = $Border

@onready var baseHealth = $Background/Content/Divider/Stats/Base/Health
@onready var baseMana = $Background/Content/Divider/Stats/Base/Mana
@onready var baseShield = $Background/Content/Divider/Stats/Base/Shield
@onready var baseThreshold = $Background/Content/Divider/Stats/Base/Threshold
@onready var baseStrength = $Background/Content/Divider/Stats/Base/Strength
@onready var baseFlow = $Background/Content/Divider/Stats/Base/Flow

@onready var incrHealth = $Background/Content/Divider/Stats/Increments/Health
@onready var incrMana = $Background/Content/Divider/Stats/Increments/Mana
@onready var incrShield = $Background/Content/Divider/Stats/Increments/Shield
@onready var incrThreshold = $Background/Content/Divider/Stats/Increments/Threshold
@onready var incrStrength = $Background/Content/Divider/Stats/Increments/Strength
@onready var incrFlow = $Background/Content/Divider/Stats/Increments/Flow

func initialize(way: Dictionary):
	data = way
	title.text = Values.link.getEnglishFromKey(data["name"])
	baseHealth.text = str(data["baseHp"])
	baseMana.text = str(data["baseMana"])
	baseShield.text = str(data["baseShield"])
	if data["baseDamageMult"] >= 0:
		baseStrength.text = "+" + str(data["baseDamageMult"]) + "%"
	else:
		baseStrength.text = str(data["baseDamageMult"]) + "%"
	baseThreshold.text = str(data["baseSoulBurn"])
	baseFlow.text = str(data["baseFlow"])
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
	if data["damageMultIncr"] >= 0:
		incrStrength.text = "+" + str(data["damageMultIncr"]) + "%"
	else:
		incrStrength.text = str(data["damageMultIncr"]) + "%"
	if data["soulBurnIncr"] >= 0:
		incrThreshold.text = "+" + str(data["soulBurnIncr"])
	else:
		incrThreshold.text = str(data["soulBurnIncr"])
	if data["flowIncr"] >= 0:
		incrFlow.text = "+" + str(data["flowIncr"])
	else:
		incrFlow.text = str(data["flowIncr"])
	picture.texture = load(data["path"])
	tooltip_text = Values.link.getEnglishFromKey(data["name"] + "_DESC")

func _on_mouse_entered():
	border.color = Values.gold

func _on_mouse_exited():
	border.color = Values.ink

func _make_custom_tooltip(for_text):
	var tooltip = preload("res://menus/tooltip.tscn").instantiate()
	var length = for_text.length()
	tooltip.get_node("Label").text = for_text
	tooltip.custom_minimum_size = Vector2(1000, 80 + length * 1.75)
	return tooltip
