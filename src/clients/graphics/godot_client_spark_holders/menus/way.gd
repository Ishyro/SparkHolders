extends Control

var data = {}

@onready var title = $Background/Content/Name
@onready var picture = $Background/Content/Divider/Picture
@onready var unvalid = $Unvalid

@onready var baseHp = $Background/Content/Divider/Stats/Base/Hp
@onready var baseMana = $Background/Content/Divider/Stats/Base/Mana
@onready var baseShield = $Background/Content/Divider/Stats/Base/Shield
@onready var baseSoulBurn = $Background/Content/Divider/Stats/Base/SoulBurn
@onready var baseDamage = $Background/Content/Divider/Stats/Base/Damage
@onready var baseFlow = $Background/Content/Divider/Stats/Base/Flow

@onready var incrHp = $Background/Content/Divider/Stats/Increments/Hp
@onready var incrMana = $Background/Content/Divider/Stats/Increments/Mana
@onready var incrShield = $Background/Content/Divider/Stats/Increments/Shield
@onready var incrSoulBurn = $Background/Content/Divider/Stats/Increments/SoulBurn
@onready var incrDamage = $Background/Content/Divider/Stats/Increments/Damage
@onready var incrFlow = $Background/Content/Divider/Stats/Increments/Flow

func initialize(way: Dictionary):
	data = way
	title.text = data["name"]
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
	picture.texture = load(data["path"])
	tooltip_text = data["name"] + "_DESCRIPTION"
