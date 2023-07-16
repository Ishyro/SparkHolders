extends Control

@onready var map = $"../Map"

@onready var tile = $Tile
@onready var tile_description = $Tile/Description

@onready var owned_life = $Self/Bars/Life/Margin/Label
@onready var owned_mana = $Self/Bars/Mana/Margin/Label
@onready var owned_soulburn = $Self/Bars/Soulburn/Margin/Label

@onready var owned_damage = $Self/OffensiveStats/Damage/Label
@onready var owned_flow = $Self/OffensiveStats/Flow/Label

@onready var owned_armor = $Self/DefensiveStats/Armor/Label
@onready var owned_slash_reduction = $Self/DefensiveStats/SlashReduction/Label
@onready var owned_blunt_reduction = $Self/DefensiveStats/BluntReduction/Label
@onready var owned_puncture_reduction = $Self/DefensiveStats/PunctureReduction/Label
@onready var owned_fire_reduction = $Self/DefensiveStats/FireReduction/Label
@onready var owned_lightning_reduction = $Self/DefensiveStats/LightningReduction/Label
@onready var owned_frost_reduction = $Self/DefensiveStats/FrostReduction/Label
@onready var owned_poison_reduction = $Self/DefensiveStats/PoisonReduction/Label
@onready var owned_mind_reduction = $Self/DefensiveStats/MindReduction/Label

@onready var target = $Target
@onready var target_life = $Target/Bars/Life/Margin/Label
@onready var target_mana = $Target/Bars/Mana/Margin/Label
@onready var target_soulburn = $Target/Bars/Soulburn/Margin/Label

@onready var target_damage = $Target/OffensiveStats/Damage/Label
@onready var target_flow = $Target/OffensiveStats/Flow/Label

@onready var target_armor = $Target/DefensiveStats/Armor/Label
@onready var target_slash_reduction = $Target/DefensiveStats/SlashReduction/Label
@onready var target_blunt_reduction = $Target/DefensiveStats/BluntReduction/Label
@onready var target_puncture_reduction = $Target/DefensiveStats/PunctureReduction/Label
@onready var target_fire_reduction = $Target/DefensiveStats/FireReduction/Label
@onready var target_lightning_reduction = $Target/DefensiveStats/LightningReduction/Label
@onready var target_frost_reduction = $Target/DefensiveStats/FrostReduction/Label
@onready var target_poison_reduction = $Target/DefensiveStats/PoisonReduction/Label
@onready var target_mind_reduction = $Target/DefensiveStats/MindReduction/Label

@onready var grid_button = $MapOptions/Options/Grid
@onready var lights_button = $MapOptions/Options/Lights
@onready var characters_button = $MapOptions/Options/Characters
@onready var projectiles_button = $MapOptions/Options/Projectiles
@onready var furnitures_button = $MapOptions/Options/Furnitures

# Called when the node enters the scene tree for the first time.
func _ready():
	characters_button.set_pressed_no_signal(true)
	projectiles_button.set_pressed_no_signal(true)
	furnitures_button.set_pressed_no_signal(true)
	
func set_skill_tab(n: int):
	$Skills/Tabs.set_current_tab(n)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass

func display_projectile(_projectile, _projectile_data: Dictionary):
	pass

func display_target(character, character_data: Dictionary):
	character.select()
	tile.visible = false
	target_life.text = str(character_data["hp"]) + " / " + str(character_data["maxHp"])
	target_mana.text = str(character_data["mana"]) + " / " + str(character_data["maxMana"])
	target_soulburn.text = str(character_data["soulBurn"]) + " / " + str(character_data["soulBurnTreshold"])
	target_damage.text = str(character_data["damages"][Values.DAMAGE_SLASH])
	target_flow.text = str(character_data["flow"])
	target_armor.text = str(character_data["armor"])
	target_slash_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_SLASH] * 100.0)) + "%"
	target_blunt_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_BLUNT] * 100.0)) + "%"
	target_puncture_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_PUNCTURE] * 100.0)) + "%"
	target_fire_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_FIRE] * 100.0)) + "%"
	target_lightning_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_LIGHTNING] * 100.0)) + "%"
	target_frost_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_FROST] * 100.0)) + "%"
	target_poison_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_POISON] * 100.0)) + "%"
	target_mind_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_MIND] * 100.0)) + "%"
	target.visible = true

func display_team(character, character_data: Dictionary):
	character.select()
	owned_life.text = str(character_data["hp"]) + " / " + str(character_data["maxHp"])
	owned_mana.text = str(character_data["mana"]) + " / " + str(character_data["maxMana"])
	owned_soulburn.text = str(character_data["soulBurn"]) + " / " + str(character_data["soulBurnTreshold"])
	owned_damage.text = str(character_data["damages"][Values.DAMAGE_SLASH])
	owned_flow.text = str(character_data["flow"])
	owned_armor.text = str(character_data["armor"])
	owned_slash_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_SLASH] * 100.0)) + "%"
	owned_blunt_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_BLUNT] * 100.0)) + "%"
	owned_puncture_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_PUNCTURE] * 100.0)) + "%"
	owned_fire_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_FIRE] * 100.0)) + "%"
	owned_lightning_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_LIGHTNING] * 100.0)) + "%"
	owned_frost_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_FROST] * 100.0)) + "%"
	owned_poison_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_POISON] * 100.0)) + "%"
	owned_mind_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_MIND] * 100.0)) + "%"

func display_tile(_tile_object, _tile_data: Dictionary):
	target.visible = false
	tile.visible = true

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
