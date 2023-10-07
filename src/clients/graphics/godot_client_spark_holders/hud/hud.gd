extends Control

@onready var map = $"../Map"

@onready var tile = $Tile
@onready var tile_description = $Tile/Description

@onready var owned_shield_bar = $Self/Margin/Content/Bars/ShieldBar/BarContainer/Bar
@onready var owned_life_bar = $Self/Margin/Content/Bars/HealthBar/BarContainer/Bar
@onready var owned_mana_bar = $Self/Margin/Content/Bars/ManaBar/BarContainer/Bar
@onready var owned_soulburn_bar = $Self/Margin/Content/Bars/SoulBurnBar/BarContainer/Bar
@onready var owned_shield = $Self/Margin/Content/Bars/ShieldBar/Current/Label
@onready var owned_life = $Self/Margin/Content/Bars/HealthBar/Current/Label
@onready var owned_mana = $Self/Margin/Content/Bars/ManaBar/Current/Label
@onready var owned_soulburn = $Self/Margin/Content/Bars/SoulBurnBar/Current/Label

@onready var owned_armor = $Self/Margin/Content/MainStats/Armor/Label
@onready var owned_damage = $Self/Margin/Content/MainStats/Damage/Label
@onready var owned_flow = $Self/Margin/Content/MainStats/Flow/Label

@onready var owned_slash_reduction = $Self/Margin/Content/Resistances/Raw1/SlashReduction/Label
@onready var owned_blunt_reduction = $Self/Margin/Content/Resistances/Raw1/BluntReduction/Label
@onready var owned_puncture_reduction = $Self/Margin/Content/Resistances/Raw1/PunctureReduction/Label
@onready var owned_mind_reduction = $Self/Margin/Content/Resistances/Raw1/MindReduction/Label

@onready var owned_fire_reduction = $Self/Margin/Content/Resistances/Raw2/FireReduction/Label
@onready var owned_lightning_reduction = $Self/Margin/Content/Resistances/Raw2/LightningReduction/Label
@onready var owned_frost_reduction = $Self/Margin/Content/Resistances/Raw2/FrostReduction/Label
@onready var owned_poison_reduction = $Self/Margin/Content/Resistances/Raw2/PoisonReduction/Label

@onready var target = $Target
@onready var target_shield_bar = $Target/Margin/Content/Bars/ShieldBar/BarContainer/Bar
@onready var target_life_bar = $Target/Margin/Content/Bars/HealthBar/BarContainer/Bar
@onready var target_mana_bar = $Target/Margin/Content/Bars/ManaBar/BarContainer/Bar
@onready var target_soulburn_bar = $Target/Margin/Content/Bars/SoulBurnBar/BarContainer/Bar
@onready var target_shield = $Target/Margin/Content/Bars/ShieldBar/Current/Label
@onready var target_life = $Target/Margin/Content/Bars/HealthBar/Current/Label
@onready var target_mana = $Target/Margin/Content/Bars/ManaBar/Current/Label
@onready var target_soulburn = $Target/Margin/Content/Bars/SoulBurnBar/Current/Label

@onready var target_armor = $Target/Margin/Content/MainStats/Armor/Label
@onready var target_damage = $Target/Margin/Content/MainStats/Damage/Label
@onready var target_flow = $Target/Margin/Content/MainStats/Flow/Label

@onready var target_slash_reduction = $Target/Margin/Content/Resistances/Raw1/SlashReduction/Label
@onready var target_blunt_reduction = $Target/Margin/Content/Resistances/Raw1/BluntReduction/Label
@onready var target_puncture_reduction = $Target/Margin/Content/Resistances/Raw1/PunctureReduction/Label
@onready var target_mind_reduction = $Target/Margin/Content/Resistances/Raw1/MindReduction/Label

@onready var target_fire_reduction = $Target/Margin/Content/Resistances/Raw2/FireReduction/Label
@onready var target_lightning_reduction = $Target/Margin/Content/Resistances/Raw2/LightningReduction/Label
@onready var target_frost_reduction = $Target/Margin/Content/Resistances/Raw2/FrostReduction/Label
@onready var target_poison_reduction = $Target/Margin/Content/Resistances/Raw2/PoisonReduction/Label

@onready var grid_button = $MapOptions/Options/Grid
@onready var lights_button = $MapOptions/Options/Lights
@onready var characters_button = $MapOptions/Options/Characters
@onready var projectiles_button = $MapOptions/Options/Projectiles
@onready var furnitures_button = $MapOptions/Options/Furnitures

@onready var mouse_box = $MouseBox
@onready var mouse_label = $MouseBox/Label

var shield_fluid = preload("res://models/bars/shield_fluid.tres")
var health_fluid = preload("res://models/bars/health_fluid.tres")
var mana_fluid = preload("res://models/bars/mana_fluid.tres")
var soulburn_fluid = preload("res://models/bars/soulburn_fluid.tres")

# Called when the node enters the scene tree for the first time.
func _ready():
	characters_button.set_pressed_no_signal(true)
	projectiles_button.set_pressed_no_signal(true)
	furnitures_button.set_pressed_no_signal(true)
	owned_shield_bar.change_shader(shield_fluid)
	owned_life_bar.change_shader(health_fluid)
	owned_mana_bar.change_shader(mana_fluid)
	owned_soulburn_bar.change_shader(soulburn_fluid)
	target_shield_bar.change_shader(shield_fluid)
	target_life_bar.change_shader(health_fluid)
	target_mana_bar.change_shader(mana_fluid)
	target_soulburn_bar.change_shader(soulburn_fluid)
	
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
	target_life.text = str(character_data["hp"])
	target_life_bar.change_amount(character_data["hp"] / max(1, character_data["maxHp"]))
	target_mana.text = str(character_data["mana"])
	target_mana_bar.change_amount(character_data["mana"] / max(1, character_data["maxMana"]))
	target_soulburn.text = str(character_data["soulBurn"])
	target_soulburn_bar.change_amount(character_data["soulBurn"] / max(1, character_data["soulBurnTreshold"]))
	target_damage.text = str(character_data["damages"][Values.DAMAGE_SLASH])
	target_flow.text = str(character_data["flow"])
	target_armor.text = str(character_data["armor"])
	target_slash_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_SLASH] * 100.0)) + "%"
	target_blunt_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_BLUNT] * 100.0)) + "%"
	target_puncture_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_PUNCTURE] * 100.0)) + "%"
	target_mind_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_MIND] * 100.0)) + "%"
	target_fire_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_FIRE] * 100.0)) + "%"
	target_lightning_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_LIGHTNING] * 100.0)) + "%"
	target_frost_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_FROST] * 100.0)) + "%"
	target_poison_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_POISON] * 100.0)) + "%"
	target.visible = true

func display_team(character, character_data: Dictionary):
	character.select()
	owned_life.text = str(character_data["hp"])
	owned_life_bar.change_amount(character_data["hp"] / max(1, character_data["maxHp"]))
	owned_mana.text = str(character_data["mana"])
	owned_mana_bar.change_amount(character_data["mana"] / max(1, character_data["maxMana"]))
	owned_soulburn.text = str(character_data["soulBurn"])
	owned_soulburn_bar.change_amount(character_data["soulBurn"] / max(1, character_data["soulBurnTreshold"]))
	owned_damage.text = str(character_data["damages"][Values.DAMAGE_SLASH])
	owned_flow.text = str(character_data["flow"])
	owned_armor.text = str(character_data["armor"])
	owned_slash_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_SLASH] * 100.0)) + "%"
	owned_blunt_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_BLUNT] * 100.0)) + "%"
	owned_puncture_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_PUNCTURE] * 100.0)) + "%"
	owned_mind_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_MIND] * 100.0)) + "%"
	owned_fire_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_FIRE] * 100.0)) + "%"
	owned_lightning_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_LIGHTNING] * 100.0)) + "%"
	owned_frost_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_FROST] * 100.0)) + "%"
	owned_poison_reduction.text = str(int(character_data["damage_reductions"][Values.DAMAGE_POISON] * 100.0)) + "%"

func display_tile(_tile_object, _tile_data: Dictionary):
	target.visible = false
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

# Actions
func _on_move_pressed():
	Values.mode = Values.ACTION_MODE_MOVE
	map.phantoms[Values.selected_team.id].visible = true
	map.characters[Values.selected_team.id].range_mesh.visible = false
	map.bake_navigation_mesh(Values.selected_team.id)

func _on_attack_pressed():
	Values.mode = Values.ACTION_MODE_ATTACK
	map.characters[Values.selected_team.id].range_mesh.visible = true

func _on_loot_pressed():
	Values.mode = Values.ACTION_MODE_LOOT
	map.characters[Values.selected_team.id].range_mesh.visible = false

func _on_wait_pressed():
	Values.mode = Values.ACTION_MODE_WAIT
	map.characters[Values.selected_team.id].range_mesh.visible = false

func _on_speak_pressed():
	map.characters[Values.selected_team.id].range_mesh.visible = false
	pass

func _on_inventory_pressed():
	pass # Replace with function body.

func _on_sleep_pressed():
	map.characters[Values.selected_team.id].range_mesh.visible = false
	pass # Replace with function body.

func _on_breakpoint_pressed():
	pass # Replace with function body.
