extends Control

@onready var slot_mantle = $Slots/Mantle
@onready var slot_helmet = $Slots/Helmet
@onready var slot_armor = $Slots/Armor
@onready var slot_gauntlets = $Slots/Gauntlets
@onready var slot_boots = $Slots/Boots
@onready var slot_lantern = $Slots/Lantern
@onready var slot_amulet = $Slots/Amulet
@onready var slot_ring_1 = $Slots/Ring1
@onready var slot_ring_2 = $Slots/Ring2
@onready var slot_weapon_1 = $Slots/Weapon1
@onready var slot_weapon_2 = $Slots/Weapon2
@onready var slot_backup_weapon_1 = $Slots/Weapon3
@onready var slot_backup_weapon_2 = $Slots/Weapon4
@onready var slot_bag = $Slots/Bag
@onready var slot_belt = $Slots/Belt
@onready var slot_bag_content = $Slots/BagContent
@onready var slot_belt_content = $Slots/BeltContent
@onready var n_items = $Items

var SLOT_SIZE

var slots = []

var closed_slot = preload("res://menus/hud/closed_slot.png")
var open_slot = preload("res://menus/hud/open_slot.png")
var base_item = preload("res://models/item.tscn")

var inventories = {}

var items = []
var selected_item
var dragged_item
var dragged_position

var bag_content = {}
var belt_content = {}

func _ready():
	SLOT_SIZE = Vector2(33, 33) #* Values.CURRENT_RESOLUTION / Values.BASE_RESOLUTION
	print(SLOT_SIZE)
	print(Values.CURRENT_RESOLUTION)
	print(Values.BASE_RESOLUTION)
	slots.push_back(slot_mantle)
	slots.push_back(slot_helmet)
	slots.push_back(slot_armor)
	slots.push_back(slot_gauntlets)
	slots.push_back(slot_boots)
	slots.push_back(slot_lantern)
	slots.push_back(slot_amulet)
	slots.push_back(slot_ring_1)
	slots.push_back(slot_ring_2)
	slots.push_back(slot_weapon_1)
	slots.push_back(slot_weapon_2)
	slots.push_back(slot_backup_weapon_1)
	slots.push_back(slot_backup_weapon_2)
	slots.push_back(slot_bag)
	slots.push_back(slot_belt)
	slots.push_back(slot_bag_content)
	slots.push_back(slot_belt_content)

func _get_drag_data(at_position):
	for item in items:
		if item.get_rect().has_point(at_position):
			selected_item = item
			var preview = Control.new()
			var preview_item = base_item.instantiate()
			var preview_color = ColorRect.new()
			preview_color.color = Color(0.188, 0.188, 0.188, 0.251)
			preview_item.size = item.size
			preview_color.size = item.size
			preview_item.texture = item.texture
			preview_item.data = item.data
			preview.add_child(preview_item)
			preview.add_child(preview_color)
			set_drag_preview(preview)
			dragged_item = preview
			return preview_item

func _can_drop_data(at_position, data):
	var current_slot = null
	for slot in slots:
		if slot.get_rect().has_point(at_position):
			current_slot = slot
			break
	if current_slot != null:
		dragged_item.global_position = current_slot.global_position + clamp((dragged_item.global_position - current_slot.global_position).snapped(SLOT_SIZE), Vector2.ZERO, current_slot.size - data.size) 
		dragged_position = dragged_item.global_position
		return current_slot.get_global_rect().encloses(data.get_global_rect())
	else:
		return false

func _drop_data(at_position, data):
	for item in items:
		if item.get_rect().has_point(at_position):
			item.global_position = selected_item.global_position
			break
	var item = base_item.instantiate()
	item.data = data.data
	item.size = Vector2(item.data["sizeX"], item.data["sizeY"]) * SLOT_SIZE
	item.texture = data.texture
	item.position = dragged_position - position
	items.erase(selected_item)
	n_items.remove_child(selected_item)
	items.push_back(item)
	n_items.add_child(item)

func update_inventories(ids):
	for id in ids:
		var inventory = Values.link.getInventoryFromCharacter(id)
		if !inventories.has(id):
			inventories[id] = {}
		inventories[id]["mantle"] = inventory["mantle"]
		inventories[id]["helmet"] = inventory["helmet"]
		inventories[id]["armor"] = inventory["armor"]
		inventories[id]["gauntlets"] = inventory["gauntlets"]
		inventories[id]["boots"] = inventory["boots"]
		inventories[id]["lantern"] = inventory["lantern"]
		inventories[id]["amulet"] = inventory["amulet"]
		inventories[id]["ring_1"] = inventory["ring_1"]
		inventories[id]["ring_2"] = inventory["ring_2"]
		inventories[id]["weapon_1"] = inventory["weapon_1"]
		inventories[id]["weapon_2"] = inventory["weapon_2"]
		inventories[id]["backup_weapon_1"] = inventory["backup_weapon_1"]
		inventories[id]["backup_weapon_2"] = inventory["backup_weapon_2"]
		inventories[id]["bag"] = inventory["bag"]
		inventories[id]["belt"] = inventory["belt"]
		inventories[id]["bag_content"] = inventory["bag_content"]
		inventories[id]["belt_content"] = inventory["belt_content"]

func display_inventory():
	SLOT_SIZE = Vector2(33, 33) #*  Values.CURRENT_RESOLUTION / Values.BASE_RESOLUTION
	var id = Values.selected_team.id
	for item in items:
		n_items.remove_child(item)
	items.clear()
	if !inventories[id]["mantle"].is_empty():
		var mantle = base_item.instantiate()
		mantle.data = inventories[id]["mantle"]
		mantle.size = Vector2(mantle.data["sizeX"], mantle.data["sizeY"]) * SLOT_SIZE
		mantle.texture = load(mantle.data.path)
		items.push_back(mantle)
		n_items.add_child(mantle)
		mantle.global_position = slot_mantle.global_position
	if !inventories[id]["helmet"].is_empty():
		var helmet = base_item.instantiate()
		helmet.data = inventories[id]["helmet"]
		helmet.size = Vector2(helmet.data["sizeX"], helmet.data["sizeY"]) * SLOT_SIZE
		helmet.texture = load(helmet.data.path)
		items.push_back(helmet)
		n_items.add_child(helmet)
		helmet.global_position = slot_helmet.global_position
	if !inventories[id]["armor"].is_empty():
		var armor = base_item.instantiate()
		armor.data = inventories[id]["armor"]
		armor.size = Vector2(armor.data["sizeX"], armor.data["sizeY"]) * SLOT_SIZE
		armor.texture = load(armor.data.path)
		items.push_back(armor)
		n_items.add_child(armor)
		armor.global_position = slot_armor.global_position
	if !inventories[id]["gauntlets"].is_empty():
		var gauntlets = base_item.instantiate()
		gauntlets.data = inventories[id]["gauntlets"]
		gauntlets.size = Vector2(gauntlets.data["sizeX"], gauntlets.data["sizeY"]) * SLOT_SIZE
		gauntlets.texture = load(gauntlets.data.path)
		items.push_back(gauntlets)
		n_items.add_child(gauntlets)
		gauntlets.global_position = slot_gauntlets.global_position
	if !inventories[id]["boots"].is_empty():
		var boots = base_item.instantiate()
		boots.data = inventories[id]["boots"]
		boots.size = Vector2(boots.data["sizeX"], boots.data["sizeY"]) * SLOT_SIZE
		boots.texture = load(boots.data.path)
		items.push_back(boots)
		n_items.add_child(boots)
		boots.global_position = slot_boots.global_position
	if !inventories[id]["lantern"].is_empty():
		var lantern = base_item.instantiate()
		lantern.data = inventories[id]["lantern"]
		lantern.size = Vector2(lantern.data["sizeX"], lantern.data["sizeY"]) * SLOT_SIZE
		lantern.texture = load(lantern.data.path)
		items.push_back(lantern)
		n_items.add_child(lantern)
		lantern.global_position = slot_lantern.global_position
	if !inventories[id]["amulet"].is_empty():
		var amulet = base_item.instantiate()
		amulet.data = inventories[id]["amulet"]
		amulet.size = Vector2(amulet.data["sizeX"], amulet.data["sizeY"]) * SLOT_SIZE
		amulet.texture = load(amulet.data.path)
		items.push_back(amulet)
		n_items.add_child(amulet)
		amulet.global_position = slot_amulet.global_position
	if !inventories[id]["ring_1"].is_empty():
		var ring_1 = base_item.instantiate()
		ring_1.data = inventories[id]["ring_1"]
		ring_1.size = Vector2(ring_1.data["sizeX"], ring_1.data["sizeY"]) * SLOT_SIZE
		ring_1.texture = load(ring_1.data.path)
		items.push_back(ring_1)
		n_items.add_child(ring_1)
		ring_1.global_position = slot_ring_1.global_position
	if !inventories[id]["ring_2"].is_empty():
		var ring_2 = base_item.instantiate()
		ring_2.data = inventories[id]["ring_2"]
		ring_2.size = Vector2(ring_2.data["sizeX"], ring_2.data["sizeY"]) * SLOT_SIZE
		ring_2.texture = load(ring_2.data.path)
		items.push_back(ring_2)
		n_items.add_child(ring_2)
		ring_2.global_position = slot_ring_2.global_position
	if !inventories[id]["weapon_1"].is_empty():
		var weapon_1 = base_item.instantiate()
		weapon_1.data = inventories[id]["weapon_1"]
		weapon_1.size = Vector2(weapon_1.data["sizeX"], weapon_1.data["sizeY"]) * SLOT_SIZE
		weapon_1.texture = load(weapon_1.data.path)
		items.push_back(weapon_1)
		n_items.add_child(weapon_1)
		weapon_1.global_position = slot_weapon_1.global_position
	if !inventories[id]["weapon_2"].is_empty():
		var weapon_2 = base_item.instantiate()
		weapon_2.data = inventories[id]["weapon_2"]
		weapon_2.size = Vector2(weapon_2.data["sizeX"], weapon_2.data["sizeY"]) * SLOT_SIZE
		weapon_2.texture = load(weapon_2.data.path)
		items.push_back(weapon_2)
		n_items.add_child(weapon_2)
		weapon_2.global_position = slot_weapon_2.global_position
	if !inventories[id]["backup_weapon_1"].is_empty():
		var backup_weapon_1 = base_item.instantiate()
		backup_weapon_1.data = inventories[id]["backup_weapon_1"]
		backup_weapon_1.size = Vector2(backup_weapon_1.data["sizeX"], backup_weapon_1.data["sizeY"]) * SLOT_SIZE
		backup_weapon_1.texture = load(backup_weapon_1.data.path)
		items.push_back(backup_weapon_1)
		n_items.add_child(backup_weapon_1)
		backup_weapon_1.global_position = slot_backup_weapon_1.global_position
	if !inventories[id]["backup_weapon_2"].is_empty():
		var backup_weapon_2 = base_item.instantiate()
		backup_weapon_2.data = inventories[id]["backup_weapon_2"]
		backup_weapon_2.size = Vector2(backup_weapon_2.data["sizeX"], backup_weapon_2.data["sizeY"]) * SLOT_SIZE
		backup_weapon_2.texture = load(backup_weapon_2.data.path)
		items.push_back(backup_weapon_2)
		n_items.add_child(backup_weapon_2)
		backup_weapon_2.global_position = slot_backup_weapon_2.global_position
	if !inventories[id]["bag"].is_empty():
		var bag = base_item.instantiate()
		bag.data = inventories[id]["bag"]
		bag.size = Vector2(bag.data["sizeX"], bag.data["sizeY"]) * SLOT_SIZE
		bag.texture = load(bag.data.path)
		items.push_back(bag)
		n_items.add_child(bag)
		bag.global_position = slot_bag.global_position
		slot_bag_content.size = Vector2(inventories[id]["bag"]["contentX"], inventories[id]["bag"]["contentY"]) * SLOT_SIZE
	else:
		slot_bag_content.size = Vector2.ZERO
	if !inventories[id]["belt"].is_empty():
		var belt = base_item.instantiate()
		belt.data = inventories[id]["belt"]
		belt.size = Vector2(belt.data["sizeX"], belt.data["sizeY"]) * SLOT_SIZE
		belt.texture = load(belt.data.path)
		items.push_back(belt)
		n_items.add_child(belt)
		belt.global_position = slot_belt.global_position
		slot_belt_content.size = Vector2(inventories[id]["belt"]["contentX"], inventories[id]["belt"]["contentY"]) * SLOT_SIZE
	else:
		slot_belt_content.size = Vector2.ZERO
	for y in range(1, 9):
		for x in range(1, 17):
			if !inventories[id]["bag"].is_empty() && x <= inventories[id]["bag"]["contentX"] && y <= inventories[id]["bag"]["contentY"]:
				get_node("BagContent/Box" + str(x + 16 * (y - 1))).texture = open_slot
			else:
				get_node("BagContent/Box" + str(x + 16 * (y - 1))).texture = closed_slot
	for y in range(1, 2):
		for x in range(1, 17):
			if !inventories[id]["belt"].is_empty() && x <= inventories[id]["belt"]["contentX"] && y <= inventories[id]["belt"]["contentY"]:
				get_node("BeltContent/Box" + str(x + 16 * (y - 1))).texture = open_slot
			else:
				get_node("BeltContent/Box" + str(x + 16 * (y - 1))).texture = closed_slot
	for coord in inventories[id]["bag_content"]:
		var item = base_item.instantiate()
		item.data = inventories[id]["bag_content"][coord]
		item.size = Vector2(item.data["sizeX"], item.data["sizeY"]) * SLOT_SIZE
		item.texture = load(item.data.path)
		items.push_back(item)
		n_items.add_child(item)
		item.global_position = slot_bag_content.global_position + coord * SLOT_SIZE
	for coord in inventories[id]["belt_content"]:
		var item = base_item.instantiate()
		item.data = inventories[id]["belt_content"][coord]
		item.size = Vector2(item.data["sizeX"], item.data["sizeY"]) * SLOT_SIZE
		item.texture = load(item.data.path)
		items.push_back(item)
		n_items.add_child(item)
		item.global_position = slot_belt_content.global_position + coord * SLOT_SIZE
	visible = true
