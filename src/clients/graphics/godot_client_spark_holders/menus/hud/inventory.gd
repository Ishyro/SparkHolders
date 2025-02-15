extends Control

@onready var character_slot_mantle = $CharacterInventory/Slots/Mantle
@onready var character_slot_helmet = $CharacterInventory/Slots/Helmet
@onready var character_slot_armor = $CharacterInventory/Slots/Armor
@onready var character_slot_gauntlets = $CharacterInventory/Slots/Gauntlets
@onready var character_slot_boots = $CharacterInventory/Slots/Boots
@onready var character_slot_lantern = $CharacterInventory/Slots/Lantern
@onready var character_slot_amulet = $CharacterInventory/Slots/Amulet
@onready var character_slot_ring_1 = $CharacterInventory/Slots/Ring1
@onready var character_slot_ring_2 = $CharacterInventory/Slots/Ring2
@onready var character_slot_weapon_1 = $CharacterInventory/Slots/Weapon1
@onready var character_slot_weapon_2 = $CharacterInventory/Slots/Weapon2
@onready var character_slot_backup_weapon = $CharacterInventory/Slots/Weapon3
@onready var character_slot_bag = $CharacterInventory/Slots/Bag
@onready var character_slot_base_inventory = $CharacterInventory/Slots/BaseInventory

@onready var character_label = $CharacterInventory/Name

@onready var other_slot_mantle = $OtherInventory/Main/Slots/Mantle
@onready var other_slot_helmet = $OtherInventory/Main/Slots/Helmet
@onready var other_slot_armor = $OtherInventory/Main/Slots/Armor
@onready var other_slot_gauntlets = $OtherInventory/Main/Slots/Gauntlets
@onready var other_slot_boots = $OtherInventory/Main/Slots/Boots
@onready var other_slot_lantern = $OtherInventory/Main/Slots/Lantern
@onready var other_slot_amulet = $OtherInventory/Main/Slots/Amulet
@onready var other_slot_ring_1 = $OtherInventory/Main/Slots/Ring1
@onready var other_slot_ring_2 = $OtherInventory/Main/Slots/Ring2
@onready var other_slot_weapon_1 = $OtherInventory/Main/Slots/Weapon1
@onready var other_slot_weapon_2 = $OtherInventory/Main/Slots/Weapon2
@onready var other_slot_backup_weapon = $OtherInventory/Main/Slots/Weapon3
@onready var other_slot_bag = $OtherInventory/Main/Slots/Bag
@onready var other_slot_base_inventory = $OtherInventory/Main/Slots/BaseInventory
@onready var other_slot_container_content = $OtherInventory/Container/Background/ContainerInventory

@onready var other_container = $OtherInventory/Container
@onready var other_gear = $OtherInventory/Main

@onready var n_items = $Items
@onready var n_containers = $Containers

var slots = []

var base_item = preload("res://models/item.tscn")
var base_container = preload("res://menus/hud/container.tscn")

var character_inventory = {}
var other_inventory = {}

var items = []
var all_items = []
var containers = {}
var selected_item
var dragged_item
var dragged_position

func _ready():
	character_slot_mantle.item_slot = Values.macros["ITEM_SLOT_MANTLE"]
	slots.push_back(character_slot_mantle)
	character_slot_helmet.item_slot = Values.macros["ITEM_SLOT_HELMET"]
	slots.push_back(character_slot_helmet)
	character_slot_armor.item_slot = Values.macros["ITEM_SLOT_ARMOR"]
	slots.push_back(character_slot_armor)
	character_slot_gauntlets.item_slot = Values.macros["ITEM_SLOT_GAUNTLETS"]
	slots.push_back(character_slot_gauntlets)
	character_slot_boots.item_slot = Values.macros["ITEM_SLOT_BOOTS"]
	slots.push_back(character_slot_boots)
	character_slot_lantern.item_slot = Values.macros["ITEM_SLOT_LANTERN"]
	slots.push_back(character_slot_lantern)
	character_slot_amulet.item_slot = Values.macros["ITEM_SLOT_AMULET"]
	slots.push_back(character_slot_amulet)
	character_slot_ring_1.item_slot = Values.macros["ITEM_SLOT_RING_1"]
	slots.push_back(character_slot_ring_1)
	character_slot_ring_2.item_slot = Values.macros["ITEM_SLOT_RING_2"]
	slots.push_back(character_slot_ring_2)
	character_slot_weapon_1.item_slot = Values.macros["ITEM_SLOT_WEAPON_1"]
	slots.push_back(character_slot_weapon_1)
	character_slot_weapon_2.item_slot = Values.macros["ITEM_SLOT_WEAPON_2"]
	slots.push_back(character_slot_weapon_2)
	character_slot_backup_weapon.item_slot = Values.macros["ITEM_SLOT_WEAPON_3"]
	slots.push_back(character_slot_backup_weapon)
	character_slot_bag.item_slot = Values.macros["ITEM_SLOT_BAG"]
	slots.push_back(character_slot_bag)
	character_slot_base_inventory.item_slot = Values.macros["ITEM_SLOT_INSIDE_BASE_INVENTORY"]
	slots.push_back(character_slot_base_inventory)
	
	other_slot_mantle.item_slot = Values.macros["ITEM_SLOT_MANTLE"]
	slots.push_back(other_slot_mantle)
	other_slot_helmet.item_slot = Values.macros["ITEM_SLOT_HELMET"]
	slots.push_back(other_slot_helmet)
	other_slot_armor.item_slot = Values.macros["ITEM_SLOT_ARMOR"]
	slots.push_back(other_slot_armor)
	other_slot_gauntlets.item_slot = Values.macros["ITEM_SLOT_GAUNTLETS"]
	slots.push_back(other_slot_gauntlets)
	other_slot_boots.item_slot = Values.macros["ITEM_SLOT_BOOTS"]
	slots.push_back(other_slot_boots)
	other_slot_lantern.item_slot = Values.macros["ITEM_SLOT_LANTERN"]
	slots.push_back(other_slot_lantern)
	other_slot_amulet.item_slot = Values.macros["ITEM_SLOT_AMULET"]
	slots.push_back(other_slot_amulet)
	other_slot_ring_1.item_slot = Values.macros["ITEM_SLOT_RING_1"]
	slots.push_back(other_slot_ring_1)
	other_slot_ring_2.item_slot = Values.macros["ITEM_SLOT_RING_2"]
	slots.push_back(other_slot_ring_2)
	other_slot_weapon_1.item_slot = Values.macros["ITEM_SLOT_WEAPON_1"]
	slots.push_back(other_slot_weapon_1)
	other_slot_weapon_2.item_slot = Values.macros["ITEM_SLOT_WEAPON_2"]
	slots.push_back(other_slot_weapon_2)
	other_slot_backup_weapon.item_slot = Values.macros["ITEM_SLOT_WEAPON_3"]
	slots.push_back(other_slot_backup_weapon)
	other_slot_bag.item_slot = Values.macros["ITEM_SLOT_BAG"]
	slots.push_back(other_slot_bag)
	other_slot_base_inventory.item_slot = Values.macros["ITEM_SLOT_INSIDE_BASE_INVENTORY"]
	slots.push_back(other_slot_base_inventory)
	other_slot_container_content.item_slot = Values.macros["ITEM_SLOT_INSIDE_BAG"]
	slots.push_back(other_slot_container_content)

func _get_drag_data(at_position):
	for item in all_items:
		if item.get_global_rect().has_point(at_position):
			selected_item = item
			var preview = Control.new()
			var preview_item = base_item.instantiate()
			var preview_color = ColorRect.new()
			preview_color.color = Color(0.188, 0.188, 0.188, 0.251)
			preview_item.size = item.size
			preview_color.size = item.size
			preview_item.texture = item.texture
			preview_item.data = item.data
			preview_item.item_coord = item.item_coord
			preview_item.item_slot = item.item_slot
			preview.add_child(preview_item)
			preview.add_child(preview_color)
			set_drag_preview(preview)
			dragged_item = preview
			return preview_item

func _can_drop_data(at_position, data):
	var current_slot = null
	for slot in slots:
		if slot.get_global_rect().has_point(at_position):
			current_slot = slot
			break
	if current_slot != null:
		dragged_item.global_position = current_slot.global_position + clamp((dragged_item.global_position - current_slot.global_position).snapped(Values.SLOT_SIZE), Vector2.ZERO, current_slot.size - data.size) 
		dragged_position = dragged_item.global_position
		if current_slot.get_global_rect().encloses(data.get_global_rect()):
			# check slot / item compatibility
			var can_drop = (current_slot.item_slot == Values.macros["ITEM_SLOT_MANTLE"] and data.data["subtype"] == Values.macros["ITEM_MANTLE"]) \
				or (current_slot.item_slot == Values.macros["ITEM_SLOT_HELMET"] and data.data["subtype"] == Values.macros["ITEM_HELMET"]) \
				or (current_slot.item_slot == Values.macros["ITEM_SLOT_ARMOR"] and data.data["subtype"] == Values.macros["ITEM_CUIRASS"]) \
				or (current_slot.item_slot == Values.macros["ITEM_SLOT_GAUNTLETS"] and data.data["subtype"] == Values.macros["ITEM_GAUNTLETS"]) \
				or (current_slot.item_slot == Values.macros["ITEM_SLOT_BOOTS"] and data.data["subtype"] == Values.macros["ITEM_BOOTS"]) \
				or (current_slot.item_slot == Values.macros["ITEM_SLOT_LANTERN"] and data.data["subtype"] == Values.macros["ITEM_LANTERN"]) \
				or (current_slot.item_slot == Values.macros["ITEM_SLOT_AMULET"] and data.data["subtype"] == Values.macros["ITEM_AMULET"]) \
				or ((current_slot.item_slot == Values.macros["ITEM_SLOT_RING_1"] or current_slot.item_slot == Values.macros["ITEM_SLOT_RING_2"]) \
					and data.data["subtype"] == Values.macros["ITEM_RING"]) \
				or ((current_slot.item_slot == Values.macros["ITEM_SLOT_WEAPON_1"] or current_slot.item_slot == Values.macros["ITEM_SLOT_WEAPON_2"] or current_slot.item_slot == Values.macros["ITEM_SLOT_WEAPON_3"]) \
					and data.data["type"] == Values.macros["ITEM_WEAPON"]) \
				or (current_slot.item_slot == Values.macros["ITEM_SLOT_BAG"] and data.data["subtype"] == Values.macros["ITEM_BAG"]) \
				or current_slot.item_slot == Values.macros["ITEM_SLOT_INSIDE_BASE_INVENTORY"] \
				or current_slot.item_slot == Values.macros["ITEM_SLOT_INSIDE_BAG"]
			# check size availlable for slots with multiple items
			# swap items
			var item_to_swap = null
			for item in all_items:
				if item != selected_item and item.get_global_rect().intersects(data.get_global_rect()):
					# swap only 2 items at the same time
					if item_to_swap != null:
						return false
					item_to_swap = item
			if item_to_swap != null:
				var new_rect = Rect2(selected_item.get_global_rect().position, item_to_swap.get_global_rect().size)
				for slot in slots:
					if slot.get_global_rect().has_point(selected_item.get_global_rect().position):
						if not slot.get_global_rect().encloses(new_rect):
							return false
						else:
							# check compatibility
							can_drop = can_drop and ((slot.item_slot == Values.macros["ITEM_SLOT_MANTLE"] and item_to_swap.data["subtype"] == Values.macros["ITEM_MANTLE"]) \
							or (slot.item_slot == Values.macros["ITEM_SLOT_HELMET"] and item_to_swap.data["subtype"] == Values.macros["ITEM_HELMET"]) \
							or (slot.item_slot == Values.macros["ITEM_SLOT_ARMOR"] and item_to_swap.data["subtype"] == Values.macros["ITEM_CUIRASS"]) \
							or (slot.item_slot == Values.macros["ITEM_SLOT_GAUNTLETS"] and item_to_swap.data["subtype"] == Values.macros["ITEM_GAUNTLETS"]) \
							or (slot.item_slot == Values.macros["ITEM_SLOT_BOOTS"] and item_to_swap.data["subtype"] == Values.macros["ITEM_BOOTS"]) \
							or (slot.item_slot == Values.macros["ITEM_SLOT_LANTERN"] and item_to_swap.data["subtype"] == Values.macros["ITEM_LANTERN"]) \
							or (slot.item_slot == Values.macros["ITEM_SLOT_AMULET"] and item_to_swap.data["subtype"] == Values.macros["ITEM_AMULET"]) \
							or ((slot.item_slot == Values.macros["ITEM_SLOT_RING_1"] or slot.item_slot == Values.macros["ITEM_SLOT_RING_2"]) \
								and item_to_swap.data["subtype"] == Values.macros["ITEM_RING"]) \
							or ((slot.item_slot == Values.macros["ITEM_SLOT_WEAPON_1"] or slot.item_slot == Values.macros["ITEM_SLOT_WEAPON_2"] or slot.item_slot == Values.macros["ITEM_SLOT_WEAPON_3"]) \
								and item_to_swap.data["type"] == Values.macros["ITEM_WEAPON"]) \
							or (slot.item_slot == Values.macros["ITEM_SLOT_BAG"] and item_to_swap.data["subtype"] == Values.macros["ITEM_BAG"]) \
							or slot.item_slot == Values.macros["ITEM_SLOT_INSIDE_BASE_INVENTORY"] \
							or slot.item_slot == Values.macros["ITEM_SLOT_INSIDE_BAG"])
				for item in all_items:
					if item != selected_item and item != item_to_swap and item.get_global_rect().intersects(new_rect):
						return false
			return can_drop
		else:
			return false 
	else:
		return false

func _drop_data(at_position, data):
	var slot1 = Values.link.getItemSlot(selected_item.data["id"])
	var slot2 = {}
	slot2["id"] = Values.link.getPlayerId()
	for slot in slots:
		if slot.get_global_rect().has_point(at_position):
			var vec2 = ((dragged_position - slot.get_global_rect().position) / Values.SLOT_SIZE).round()
			slot2["x"] = vec2.x
			slot2["y"] = vec2.y
			slot2["slot"] = slot.item_slot
			break
	if selected_item.container != null:
		for item in all_items:
			# swap items
			if item != selected_item and item.get_global_rect().intersects(data.get_global_rect()):
				# first item inside a container
				# put second item inside the container
				if items.count(item) == 1:
					# remove second item from the standard list
					items.erase(item)
					n_items.remove_child(item)
				# second item inside a container
				else:
					# remove second item from the other container
					item.container.remove(item)
				selected_item.container.add(item)
				item.global_position = selected_item.global_position
				slot2 = Values.link.getItemSlot(item.data["id"])
		selected_item.container.remove(selected_item)
		items.append(selected_item)
		n_items.add_child(selected_item)
	else:
		for item in all_items:
			if item != selected_item and item.get_global_rect().intersects(data.get_global_rect()):
				if item.container != null:
					item.container.remove(item)
					items.append(item)
					n_items.add_child(item)
				item.global_position = selected_item.global_position
				break
	for container_item in containers:
		var container = containers[container_item]
		if container.visible and container.get_global_rect().has_point(dragged_position):
			n_items.remove_child(selected_item)
			container.add(selected_item)
	selected_item.global_position = dragged_position
	if slot1["id"] != slot2["id"] or slot1["x"] != slot2["x"] or slot1["y"] != slot2["y"] or slot1["slot"] != slot2["slot"]:
		send_gear_swap_action(Values.macros["ACTION_SWAP_GEAR"], slot1, slot2)

func _drop_data_old(at_position, data):
	var slot1 = Values.link.getItemSlot(selected_item.data["id"])
	var slot2 = {}
	slot2["id"] = Values.link.getPlayerId()
	for slot in slots:
		if slot.get_global_rect().has_point(at_position):
			var vec2 = ((dragged_position - slot.get_global_rect().position) / Values.SLOT_SIZE).round()
			slot2["x"] = vec2.x
			slot2["y"] = vec2.y
			slot2["slot"] = slot.item_slot
			break
	for item in all_items:
		if item != selected_item and item.get_global_rect().intersects(data.get_global_rect()):
			# swap items
			for container_item in containers:
				var container = containers[container_item]
				if container.visible and container.get_global_rect().has_point(selected_item.global_position):
					# first item inside a container
					# put second item inside the container
					if items.count(item) == 1:
						# remove second item from the standard list
						items.erase(item)
						n_items.remove_child(item)
					# second item inside a container
					else:
						# remove second item from the other container
						for container2_item in containers:
							var container2 = containers[container2_item]
							if container2.visible and container != container2 and container2.get_global_rect().has_point(item.global_position):
								container2.remove(item)
								break
					container.add(item)
					break
			item.global_position = selected_item.global_position
			slot2 = Values.link.getItemSlot(item.data["id"])
			break
	for container_item in containers:
		var container = containers[container_item]
		if container.visible and container.get_global_rect().has_point(dragged_position):
			# second item inside a container
			# put first item inside the container
			if items.count(selected_item) == 1:
				# remove first item from the standard list
				items.erase(selected_item)
				n_items.remove_child(selected_item)
			# first item inside a container
			else:
				# remove first item from the other container
				for container2_item in containers:
					var container2 = containers[container2_item]
					if container2.visible and container != container2 and container2.get_global_rect().has_point(selected_item.global_position):
						container2.remove(selected_item)
						break
			container.add(selected_item)
			break
	selected_item.global_position = dragged_position
	if slot1["id"] != slot2["id"] or slot1["x"] != slot2["x"] or slot1["y"] != slot2["y"] or slot1["slot"] != slot2["slot"]:
		send_gear_swap_action(Values.macros["ACTION_SWAP_GEAR"], slot1, slot2)

func send_gear_swap_action(type, slot1, slot2):
	Values.action_mutex.lock()
	Values.action["type"] = type
	Values.action["arg1"] = slot1
	Values.action["arg2"] = slot2
	Values.action["mana_cost"] = 0
	Values.link.send_action(Values.action)
	Values.action_mutex.unlock()

func update_inventory():
	character_inventory = Values.link.getInventoryFromCharacter()
	character_label.text = Values.link.getStatsFromCharacter()["name"]

func open_other_inventory():
	other_inventory = Values.link.getInventoryFromCharacter()

func display_inventory(with_other: bool, with_gear: bool):
	visible = true
	for item in items:
		n_items.remove_child(item)
	items.clear()
	all_items.clear()
	for container in containers:
		n_containers.remove_child(containers[container])
	containers.clear()
	if !character_inventory["mantle"].is_empty():
		var mantle = base_item.instantiate()
		mantle.data = character_inventory["mantle"]
		mantle.item_coord = Vector2.ZERO
		mantle.item_slot = Values.macros["ITEM_SLOT_MANTLE"]
		mantle.size = Vector2(mantle.data["sizeX"], mantle.data["sizeY"]) * Values.SLOT_SIZE
		mantle.texture = load(mantle.data.path)
		items.push_back(mantle)
		n_items.add_child(mantle)
		mantle.global_position = character_slot_mantle.global_position
	if !character_inventory["helmet"].is_empty():
		var helmet = base_item.instantiate()
		helmet.data = character_inventory["helmet"]
		helmet.item_coord = Vector2.ZERO
		helmet.item_slot = Values.macros["ITEM_SLOT_HELMET"]
		helmet.size = Vector2(helmet.data["sizeX"], helmet.data["sizeY"]) * Values.SLOT_SIZE
		helmet.texture = load(helmet.data.path)
		items.push_back(helmet)
		n_items.add_child(helmet)
		helmet.global_position = character_slot_helmet.global_position
	if !character_inventory["armor"].is_empty():
		var armor = base_item.instantiate()
		armor.data = character_inventory["armor"]
		armor.item_coord = Vector2.ZERO
		armor.item_slot = Values.macros["ITEM_SLOT_ARMOR"]
		armor.size = Vector2(armor.data["sizeX"], armor.data["sizeY"]) * Values.SLOT_SIZE
		armor.texture = load(armor.data.path)
		items.push_back(armor)
		n_items.add_child(armor)
		armor.global_position = character_slot_armor.global_position
	if !character_inventory["gauntlets"].is_empty():
		var gauntlets = base_item.instantiate()
		gauntlets.data = character_inventory["gauntlets"]
		gauntlets.item_coord = Vector2.ZERO
		gauntlets.item_slot = Values.macros["ITEM_SLOT_GAUNTLETS"]
		gauntlets.size = Vector2(gauntlets.data["sizeX"], gauntlets.data["sizeY"]) * Values.SLOT_SIZE
		gauntlets.texture = load(gauntlets.data.path)
		items.push_back(gauntlets)
		n_items.add_child(gauntlets)
		gauntlets.global_position = character_slot_gauntlets.global_position
	if !character_inventory["boots"].is_empty():
		var boots = base_item.instantiate()
		boots.data = character_inventory["boots"]
		boots.item_coord = Vector2.ZERO
		boots.item_slot = Values.macros["ITEM_SLOT_BOOTS"]
		boots.size = Vector2(boots.data["sizeX"], boots.data["sizeY"]) * Values.SLOT_SIZE
		boots.texture = load(boots.data.path)
		items.push_back(boots)
		n_items.add_child(boots)
		boots.global_position = character_slot_boots.global_position
	if !character_inventory["lantern"].is_empty():
		var lantern = base_item.instantiate()
		lantern.data = character_inventory["lantern"]
		lantern.item_coord = Vector2.ZERO
		lantern.item_slot = Values.macros["ITEM_SLOT_LANTERN"]
		lantern.size = Vector2(lantern.data["sizeX"], lantern.data["sizeY"]) * Values.SLOT_SIZE
		lantern.texture = load(lantern.data.path)
		items.push_back(lantern)
		n_items.add_child(lantern)
		lantern.global_position = character_slot_lantern.global_position
	if !character_inventory["amulet"].is_empty():
		var amulet = base_item.instantiate()
		amulet.data = character_inventory["amulet"]
		amulet.item_coord = Vector2.ZERO
		amulet.item_slot = Values.macros["ITEM_SLOT_AMULET"]
		amulet.size = Vector2(amulet.data["sizeX"], amulet.data["sizeY"]) * Values.SLOT_SIZE
		amulet.texture = load(amulet.data.path)
		items.push_back(amulet)
		n_items.add_child(amulet)
		amulet.global_position = character_slot_amulet.global_position
	if !character_inventory["ring_1"].is_empty():
		var ring_1 = base_item.instantiate()
		ring_1.data = character_inventory["ring_1"]
		ring_1.item_coord = Vector2.ZERO
		ring_1.item_slot = Values.macros["ITEM_SLOT_RING_1"]
		ring_1.size = Vector2(ring_1.data["sizeX"], ring_1.data["sizeY"]) * Values.SLOT_SIZE
		ring_1.texture = load(ring_1.data.path)
		items.push_back(ring_1)
		n_items.add_child(ring_1)
		ring_1.global_position = character_slot_ring_1.global_position
	if !character_inventory["ring_2"].is_empty():
		var ring_2 = base_item.instantiate()
		ring_2.data = character_inventory["ring_2"]
		ring_2.item_coord = Vector2.ZERO
		ring_2.item_slot = Values.macros["ITEM_SLOT_RING_2"]
		ring_2.size = Vector2(ring_2.data["sizeX"], ring_2.data["sizeY"]) * Values.SLOT_SIZE
		ring_2.texture = load(ring_2.data.path)
		items.push_back(ring_2)
		n_items.add_child(ring_2)
		ring_2.global_position = character_slot_ring_2.global_position
	if !character_inventory["weapon_1"].is_empty():
		var weapon_1 = base_item.instantiate()
		weapon_1.data = character_inventory["weapon_1"]
		weapon_1.item_coord = Vector2.ZERO
		weapon_1.item_slot = Values.macros["ITEM_SLOT_WEAPON_1"]
		weapon_1.size = Vector2(weapon_1.data["sizeX"], weapon_1.data["sizeY"]) * Values.SLOT_SIZE
		weapon_1.texture = load(weapon_1.data.path)
		items.push_back(weapon_1)
		n_items.add_child(weapon_1)
		weapon_1.global_position = character_slot_weapon_1.global_position
	if !character_inventory["weapon_2"].is_empty():
		var weapon_2 = base_item.instantiate()
		weapon_2.data = character_inventory["weapon_2"]
		weapon_2.item_coord = Vector2.ZERO
		weapon_2.item_slot = Values.macros["ITEM_SLOT_WEAPON_2"]
		weapon_2.size = Vector2(weapon_2.data["sizeX"], weapon_2.data["sizeY"]) * Values.SLOT_SIZE
		weapon_2.texture = load(weapon_2.data.path)
		items.push_back(weapon_2)
		n_items.add_child(weapon_2)
		weapon_2.global_position = character_slot_weapon_2.global_position
	if !character_inventory["backup_weapon"].is_empty():
		var backup_weapon = base_item.instantiate()
		backup_weapon.data = character_inventory["backup_weapon"]
		backup_weapon.item_coord = Vector2.ZERO
		backup_weapon.item_slot = Values.macros["ITEM_SLOT_WEAPON_3"]
		backup_weapon.size = Vector2(backup_weapon.data["sizeX"], backup_weapon.data["sizeY"]) * Values.SLOT_SIZE
		backup_weapon.texture = load(backup_weapon.data.path)
		items.push_back(backup_weapon)
		n_items.add_child(backup_weapon)
		backup_weapon.global_position = character_slot_backup_weapon.global_position
	if !character_inventory["bag"].is_empty():
		var bag = base_item.instantiate()
		bag.data = character_inventory["bag"]
		bag.item_coord = Vector2.ZERO
		bag.item_slot = Values.macros["ITEM_SLOT_BAG"]
		bag.size = Vector2(bag.data["sizeX"], bag.data["sizeY"]) * Values.SLOT_SIZE
		bag.texture = load(bag.data.path)
		items.push_back(bag)
		n_items.add_child(bag)
		bag.global_position = character_slot_bag.global_position
		var container = base_container.instantiate()
		n_containers.add_child(container)
		slots.append(container.n_slot)
		container.visible = false
		containers[bag] = container
		all_items.append_array(container.initialize("Bag", bag.data))
	for coord in character_inventory["base_inventory"]:
		var item = base_item.instantiate()
		item.data = character_inventory["base_inventory"][coord]
		item.item_coord = coord
		item.item_slot = Values.macros["ITEM_SLOT_INSIDE_BASE_INVENTORY"]
		item.size = Vector2(item.data["sizeX"], item.data["sizeY"]) * Values.SLOT_SIZE
		item.texture = load(item.data.path)
		items.push_back(item)
		n_items.add_child(item)
		item.global_position = character_slot_base_inventory.global_position + coord * Values.SLOT_SIZE
	if with_other:
		if with_gear:
			if !other_inventory["mantle"].is_empty():
				var mantle = base_item.instantiate()
				mantle.data = other_inventory["mantle"]
				mantle.item_coord = Vector2.ZERO
				mantle.item_slot = Values.macros["ITEM_SLOT_MANTLE"]
				mantle.owned = false
				mantle.size = Vector2(mantle.data["sizeX"], mantle.data["sizeY"]) * Values.SLOT_SIZE
				mantle.texture = load(mantle.data.path)
				items.push_back(mantle)
				n_items.add_child(mantle)
				mantle.global_position = other_slot_mantle.global_position
			if !other_inventory["helmet"].is_empty():
				var helmet = base_item.instantiate()
				helmet.data = other_inventory["helmet"]
				helmet.item_coord = Vector2.ZERO
				helmet.item_slot = Values.macros["ITEM_SLOT_HELMET"]
				helmet.owned = false
				helmet.size = Vector2(helmet.data["sizeX"], helmet.data["sizeY"]) * Values.SLOT_SIZE
				helmet.texture = load(helmet.data.path)
				items.push_back(helmet)
				n_items.add_child(helmet)
				helmet.global_position = other_slot_helmet.global_position
			if !other_inventory["armor"].is_empty():
				var armor = base_item.instantiate()
				armor.data = other_inventory["armor"]
				armor.item_coord = Vector2.ZERO
				armor.item_slot = Values.macros["ITEM_SLOT_ARMOR"]
				armor.owned = false
				armor.size = Vector2(armor.data["sizeX"], armor.data["sizeY"]) * Values.SLOT_SIZE
				armor.texture = load(armor.data.path)
				items.push_back(armor)
				n_items.add_child(armor)
				armor.global_position = other_slot_armor.global_position
			if !other_inventory["gauntlets"].is_empty():
				var gauntlets = base_item.instantiate()
				gauntlets.data = other_inventory["gauntlets"]
				gauntlets.item_coord = Vector2.ZERO
				gauntlets.item_slot = Values.macros["ITEM_SLOT_GAUNTLETS"]
				gauntlets.owned = false
				gauntlets.size = Vector2(gauntlets.data["sizeX"], gauntlets.data["sizeY"]) * Values.SLOT_SIZE
				gauntlets.texture = load(gauntlets.data.path)
				items.push_back(gauntlets)
				n_items.add_child(gauntlets)
				gauntlets.global_position = other_slot_gauntlets.global_position
			if !other_inventory["boots"].is_empty():
				var boots = base_item.instantiate()
				boots.data = other_inventory["boots"]
				boots.item_coord = Vector2.ZERO
				boots.item_slot = Values.macros["ITEM_SLOT_BOOTS"]
				boots.owned = false
				boots.size = Vector2(boots.data["sizeX"], boots.data["sizeY"]) * Values.SLOT_SIZE
				boots.texture = load(boots.data.path)
				items.push_back(boots)
				n_items.add_child(boots)
				boots.global_position = other_slot_boots.global_position
			if !other_inventory["lantern"].is_empty():
				var lantern = base_item.instantiate()
				lantern.data = other_inventory["lantern"]
				lantern.item_coord = Vector2.ZERO
				lantern.item_slot = Values.macros["ITEM_SLOT_LANTERN"]
				lantern.owned = false
				lantern.size = Vector2(lantern.data["sizeX"], lantern.data["sizeY"]) * Values.SLOT_SIZE
				lantern.texture = load(lantern.data.path)
				items.push_back(lantern)
				n_items.add_child(lantern)
				lantern.global_position = other_slot_lantern.global_position
			if !other_inventory["amulet"].is_empty():
				var amulet = base_item.instantiate()
				amulet.data = other_inventory["amulet"]
				amulet.item_coord = Vector2.ZERO
				amulet.item_slot = Values.macros["ITEM_SLOT_AMULET"]
				amulet.owned = false
				amulet.size = Vector2(amulet.data["sizeX"], amulet.data["sizeY"]) * Values.SLOT_SIZE
				amulet.texture = load(amulet.data.path)
				items.push_back(amulet)
				n_items.add_child(amulet)
				amulet.global_position = other_slot_amulet.global_position
			if !other_inventory["ring_1"].is_empty():
				var ring_1 = base_item.instantiate()
				ring_1.data = other_inventory["ring_1"]
				ring_1.item_coord = Vector2.ZERO
				ring_1.item_slot = Values.macros["ITEM_SLOT_RING_1"]
				ring_1.owned = false
				ring_1.size = Vector2(ring_1.data["sizeX"], ring_1.data["sizeY"]) * Values.SLOT_SIZE
				ring_1.texture = load(ring_1.data.path)
				items.push_back(ring_1)
				n_items.add_child(ring_1)
				ring_1.global_position = other_slot_ring_1.global_position
			if !other_inventory["ring_2"].is_empty():
				var ring_2 = base_item.instantiate()
				ring_2.data = other_inventory["ring_2"]
				ring_2.item_coord = Vector2.ZERO
				ring_2.item_slot = Values.macros["ITEM_SLOT_RING_2"]
				ring_2.owned = false
				ring_2.size = Vector2(ring_2.data["sizeX"], ring_2.data["sizeY"]) * Values.SLOT_SIZE
				ring_2.texture = load(ring_2.data.path)
				items.push_back(ring_2)
				n_items.add_child(ring_2)
				ring_2.global_position = other_slot_ring_2.global_position
			if !other_inventory["weapon_1"].is_empty():
				var weapon_1 = base_item.instantiate()
				weapon_1.data = other_inventory["weapon_1"]
				weapon_1.item_coord = Vector2.ZERO
				weapon_1.item_slot = Values.macros["ITEM_SLOT_WEAPON_1"]
				weapon_1.owned = false
				weapon_1.size = Vector2(weapon_1.data["sizeX"], weapon_1.data["sizeY"]) * Values.SLOT_SIZE
				weapon_1.texture = load(weapon_1.data.path)
				items.push_back(weapon_1)
				n_items.add_child(weapon_1)
				weapon_1.global_position = other_slot_weapon_1.global_position
			if !other_inventory["weapon_2"].is_empty():
				var weapon_2 = base_item.instantiate()
				weapon_2.data = other_inventory["weapon_2"]
				weapon_2.item_coord = Vector2.ZERO
				weapon_2.item_slot = Values.macros["ITEM_SLOT_WEAPON_2"]
				weapon_2.owned = false
				weapon_2.size = Vector2(weapon_2.data["sizeX"], weapon_2.data["sizeY"]) * Values.SLOT_SIZE
				weapon_2.texture = load(weapon_2.data.path)
				items.push_back(weapon_2)
				n_items.add_child(weapon_2)
				weapon_2.global_position = other_slot_weapon_2.global_position
			if !other_inventory["backup_weapon"].is_empty():
				var backup_weapon = base_item.instantiate()
				backup_weapon.data = other_inventory["backup_weapon"]
				backup_weapon.item_coord = Vector2.ZERO
				backup_weapon.item_slot = Values.macros["ITEM_SLOT_WEAPON_3"]
				backup_weapon.owned = false
				backup_weapon.size = Vector2(backup_weapon.data["sizeX"], backup_weapon.data["sizeY"]) * Values.SLOT_SIZE
				backup_weapon.texture = load(backup_weapon.data.path)
				items.push_back(backup_weapon)
				n_items.add_child(backup_weapon)
				backup_weapon.global_position = other_slot_backup_weapon.global_position
			if !other_inventory["bag"].is_empty():
				var bag = base_item.instantiate()
				bag.data = other_inventory["bag"]
				bag.item_coord = Vector2.ZERO
				bag.item_slot = Values.macros["ITEM_SLOT_BAG"]
				bag.owned = false
				bag.size = Vector2(bag.data["sizeX"], bag.data["sizeY"]) * Values.SLOT_SIZE
				bag.texture = load(bag.data.path)
				items.push_back(bag)
				n_items.add_child(bag)
				bag.global_position = other_slot_bag.global_position
				other_container.set_custom_minimum_size(Vector2(80, 104) + Vector2(other_inventory["bag"]["contentX"], other_inventory["bag"]["contentY"]) * Values.SLOT_SIZE)
			else:
				other_container.set_custom_minimum_size(Vector2(0, 0))
			for coord in other_inventory["base_inventory"]:
				var item = base_item.instantiate()
				item.data = other_inventory["base_inventory"][coord]
				item.item_coord = coord
				item.item_slot = Values.macros["ITEM_SLOT_INSIDE_BASE_INVENTORY"]
				item.owned = false
				item.size = Vector2(item.data["sizeX"], item.data["sizeY"]) * Values.SLOT_SIZE
				item.texture = load(item.data.path)
				items.push_back(item)
				n_items.add_child(item)
				item.global_position = other_slot_base_inventory.global_position + coord * Values.SLOT_SIZE
			other_gear.visible = true
		for coord in other_inventory["bag_content"]:
			var item = base_item.instantiate()
			item.data = other_inventory["bag_content"][coord]
			item.item_coord = coord
			item.item_slot = Values.macros["ITEM_SLOT_INSIDE_BAG"]
			item.owned = false
			item.size = Vector2(item.data["sizeX"], item.data["sizeY"]) * Values.SLOT_SIZE
			item.texture = load(item.data.path)
			items.push_back(item)
			n_items.add_child(item)
			item.global_position = other_slot_container_content.global_position + coord * Values.SLOT_SIZE
	all_items.append_array(items)

func _on_visibility_changed():
	if visible:
		Values.link.pause()
	else:
		other_gear.visible = false
		Values.link.unpause()

func _unhandled_input(event):
	if event is InputEventMouseButton and event.button_index == MOUSE_BUTTON_RIGHT and event.is_released():
		for container in containers:
			if container.get_global_rect().has_point(event.position):
				containers[container].visible = not containers[container].visible
