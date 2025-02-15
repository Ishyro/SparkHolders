extends TextureRect

var data = {}
var drag = false
var diff

@onready var n_name = $Name
@onready var n_slot = $Background/ContainerInventory

@onready var n_items = $Items

var base_item = preload("res://models/item.tscn")
var items = []

func initialize(title: String, container: Dictionary):
	n_name.text = title
	data = container
	n_slot.item_slot = Values.macros["ITEM_SLOT_INSIDE_BAG"]
	set_custom_minimum_size(Vector2(80, 104) + Vector2(container["contentX"], container["contentY"]) * Values.SLOT_SIZE)
	for coord in container["content"]:
		var item = base_item.instantiate()
		item.data = container["content"][coord]
		item.item_coord = coord
		item.item_slot = Values.macros["ITEM_SLOT_INSIDE_BAG"]
		item.texture = load(item.data.path)
		item.size = Vector2(item.data["sizeX"], item.data["sizeY"]) * Values.SLOT_SIZE
		item.container = self
		items.push_back(item)
		n_items.add_child(item)
		item.global_position = global_position + Vector2(40, 64) + coord * Values.SLOT_SIZE
	return items

func add(item):
	items.append(item)
	item.container = self
	n_items.add_child(item)

func remove(item):
	items.erase(item)
	item.container = null
	n_items.remove_child(item)

func _on_gui_input(event: InputEvent):
	if event is InputEventMouseButton and event.button_index == MOUSE_BUTTON_LEFT and event.is_pressed():
		if not n_slot.get_global_rect().has_point(get_viewport().get_mouse_position()):
			drag = true
			diff = get_viewport().get_mouse_position() - global_position
	if event is InputEventMouseButton and event.button_index == MOUSE_BUTTON_LEFT and event.is_released():
		if not n_slot.get_global_rect().has_point(get_viewport().get_mouse_position()):
			drag = false
	if event is InputEventMouseMotion and drag:
		global_position = get_viewport().get_mouse_position() - diff

func _on_close_pressed() -> void:
	visible = false
