extends Node3D

const MOVEMENT_SPEED = 0.6
const MOVEMENT_FRICTION = 0.5

var _mouse_motion = Vector2.ZERO
var velocity = Vector3.ZERO
var pause_state = false

@onready var camera = $Camera
@onready var camera_attributes = camera.attributes
@onready var pause = $"../Menus/Pause"
@onready var character_sheet = $"../Menus/CharacterSheet"
@onready var hud = $"../HUD"
@onready var map = $"../Map"

func _ready():
	Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)
	camera_attributes.dof_blur_far_enabled = true
	camera_attributes.dof_blur_far_distance = 100 * 1.5
	camera_attributes.dof_blur_far_transition = 100 * 0.125

func update_mouse_coordinates():
	var mouse_coords
	var from
	var to
	mouse_coords = camera.get_viewport().get_mouse_position()
	from = camera.project_ray_origin(mouse_coords)
	to = from + camera.project_ray_normal(mouse_coords) * 1000.0
	var space = get_world_3d().direct_space_state
	var query = PhysicsRayQueryParameters3D.create(from, to, 0x40)
	var result = space.intersect_ray(query)
	Values.selection_mutex.lock()
	if not result.is_empty():
		Values.coord = map.colliders.find_key(result["collider"])
		if not Values.coord:
			Values.coord = result["position"]
		else:
			if map.furnitures.has(Values.coord) \
				and (map.characters[map.owned_character].global_position).distance_to(map.furnitures[Values.coord].global_position) \
				< 0.5 + (map.characters_data[map.owned_character]["sizeX"] + map.characters_data[map.owned_character]["sizeY"]) * 0.25 \
				+ (map.furnitures_data[Values.coord]["sizeX"] + map.characters_data[map.owned_character]["sizeY"]) * 0.25:
				Values.selected = map.furnitures[Values.coord]
				Values.selected_data = map.furnitures_data[Values.coord]
	else:
		Values.selected = null
		Values.selected_data = {}
	hud.update_selection()
	Values.selection_mutex.unlock()

func _process(_delta):
	if not pause_state:
		# Mouse movement.
		if not Values.free_mouse_state:
			_mouse_motion.y = clamp(_mouse_motion.y, -1560, 1560)
			camera.transform.basis = Basis.from_euler(Vector3(_mouse_motion.y * -0.001, _mouse_motion.x * -0.001, 0))

func _physics_process(_delta):
	if not pause_state and not Values.free_mouse_state:
		update_mouse_coordinates()
		camera.transform.origin = map.characters[map.owned_character].transform.origin + Vector3(0, 1.6, 0)
		var movement_vec2 = Input.get_vector("move_left", "move_right", "move_forward", "move_back")
		if movement_vec2 != Vector2.ZERO and not Values.move_set:
			# invert x axis
			movement_vec2.x = -movement_vec2.x
			movement_vec2 = movement_vec2.rotated(camera.rotation.y - PI / 2.)
			var angle = rad_to_deg(movement_vec2.angle())
			if angle < 0: 
				angle = angle + 360
			send_oriented_action(Values.macros["ACTION_MOVE"], angle)

func _unhandled_input(event):
	if event.is_action_released("pause"):
		if character_sheet.visible:
			character_sheet.visible = false
		elif hud.inventory.visible:
			hud.inventory.visible = false
			Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)
		elif hud.skillbook.visible:
			hud.skillbook.visible = false
			Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)
		else:
			pause_state = not pause_state
			if pause_state:
				Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE)
			else:
				Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)
			pause.visible = pause_state
	if not pause_state:
		if event.is_action_pressed("free_mouse"):
			Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE)
			Values.free_mouse_state = true
		if event.is_action_released("free_mouse"):
			Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)
			Values.free_mouse_state = false
		elif event is InputEventMouseMotion:
			_mouse_motion += event.relative
		if event.is_action_released("skill_tab_1"):
			hud.skill_button_1.set_pressed(true)
		if event.is_action_released("skill_tab_2"):
			hud.skill_button_2.set_pressed(true)
		if event.is_action_released("skill_tab_3"):
			hud.skill_button_3.set_pressed(true)
		if event.is_action_released("skill_tab_4"):
			hud.skill_button_4.set_pressed(true)
		if event.is_action_released("skill_tab_5"):
			hud.skill_button_5.set_pressed(true)
		if event.is_action_released("skill_tab_6"):
			hud.skill_button_6.set_pressed(true)
		if event.is_action_released("skill_tab_7"):
			hud.skill_button_7.set_pressed(true)
		if event.is_action_released("skill_tab_8"):
			hud.skill_button_8.set_pressed(true)
		if event.is_action_released("skill_tab_9"):
			hud.skill_button_9.set_pressed(true)
		if event.is_action_released("skill_tab_10"):
			hud.skill_button_10.set_pressed(true)
		if event.is_action_released("skill_tab_11"):
			hud.skill_button_11.set_pressed(true)
		if event.is_action_released("skill_tab_12"):
			hud.skill_button_12.set_pressed(true)
		if event.is_action_released("display_stats"):
			if not character_sheet.visible:
				character_sheet.display_stats()
			else:
				character_sheet.visible = false
		if event.is_action_released("inventory"):
			if not hud.inventory.visible:
				Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE)
				Values.free_mouse_state = true
				hud.inventory.display_inventory(false, false)
			else:
				Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)
				Values.free_mouse_state = false
				hud.inventory.visible = false
		if event.is_action_released("skill_book"):
			if not hud.skillbook.visible:
				Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE)
				Values.free_mouse_state = true
				hud.display_skillbook()
			else:
				Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)
				Values.free_mouse_state = false
				hud.skillbook.visible = false
		# Actions
		if not Values.free_mouse_state: #and not Values.updating_state and not Values.link.hasState():
			Values.selection_mutex.lock()
			if event.is_action_released("interact") and Values.selected != null:
				send_targeted_action(Values.macros["ACTION_ACTIVATION"], Values.macros["TARGET_FURNITURE"], Values.selected_data["id"], Vector3.ZERO)
			if event.is_action_released("attack"):
				send_targeted_action(Values.macros["ACTION_STRIKE"], Values.macros["TARGET_CHARACTER"], Values.selected_data["id"], Vector3.ZERO)
			if event.is_action_released("skill"):
				send_targeted_action(Values.macros["ACTION_STRIKE"], Values.macros["TARGET_CHARACTER"], Values.selected_data["id"], Vector3.ZERO)
			if event.is_action_released("jump"):
				send_base_action(Values.macros["ACTION_JUMP"])
			if event.is_action_pressed("run") or event.is_action_released("run"):
				send_base_action(Values.macros["ACTION_RUN"])
			Values.selection_mutex.unlock()

func send_base_action(type):
	if type != Values.macros["ACTION_JUMP"] and type != Values.macros["ACTION_RUN"]:
		Values.action_mutex.lock()
	Values.action["type"] = type
	Values.action["arg1"] = 0
	Values.action["arg2"] = 0
	Values.action["mana_cost"] = 0
	Values.link.send_action(Values.action)
	if type != Values.macros["ACTION_JUMP"] and type != Values.macros["ACTION_RUN"]:
		Values.action_mutex.unlock()
	
func send_oriented_action(type, orientation_z):
	if type == Values.macros["ACTION_MOVE"]:
		Values.action_move_mutex.lock()
	else:
		Values.action_mutex.lock()
	Values.action["type"] = type
	Values.action["arg1"] = orientation_z
	Values.action["arg2"] = 0
	Values.action["mana_cost"] = 0
	if type == Values.macros["ACTION_MOVE"]:
		Values.move_set = true
	Values.link.send_action(Values.action)
	if type == Values.macros["ACTION_MOVE"]:
		Values.action_move_mutex.unlock()
	else:
		Values.action_mutex.unlock()
	
func send_targeted_action(type, target_type, target_id, pos):
	Values.action_mutex.lock()
	Values.action["type"] = type
	var target = {}
	target["type"] = target_type
	target["id"] = target_id
	target["pos"] = pos
	Values.action["arg1"] = target
	Values.action["arg2"] = 0
	Values.action["mana_cost"] = 0
	Values.link.send_action(Values.action)
	Values.action_mutex.unlock()

func send_skill_action(type, target_type, target_id, pos, skill, mana_cost):
	Values.action_mutex.lock()
	Values.action["type"] = type
	var target = {}
	target["type"] = target_type
	target["id"] = target_id
	target["pos"] = pos
	Values.action["arg1"] = target
	Values.action["arg2"] = skill
	Values.action["mana_cost"] = mana_cost
	Values.link.send_action(Values.action)
	Values.action_mutex.unlock()
