extends Node3D

const MOVEMENT_SPEED = 0.6
const MOVEMENT_FRICTION = 0.5

var _mouse_motion = Vector2.ZERO
var velocity = Vector3.ZERO
var pause_state = false
var free_mouse_state = false

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
	var mask
	if Values.mode == Values.ACTION_MODE_ACTIVATION:
		mask = 0x40
	else:
		mask = 0x3
	var query = PhysicsRayQueryParameters3D.create(from, to, mask)
	var result = space.intersect_ray(query)
	if not result.is_empty():
		if Values.mode == Values.ACTION_MODE_ACTIVATION:
			Values.coord = map.colliders.find_key(result["collider"])
			if not Values.coord:
				Values.coord = result["position"]
		else:
			Values.coord = result["position"]
		if Input.is_action_pressed("select"):
			Values.selection_changed = true
			var selection = result["collider"]
			if Values.selected_target:
				Values.selected_target.unselect()
			if "character" in selection:
				var owned = false
				for character_id in map.owned_characters:
					if character_id == selection.id:
						owned = true
						break
				if not owned:
					Values.selected_target = selection
				Values.selected_projectile = null
				Values.selected_block = null
			if "projectile" in selection:
				Values.selected_projectile = selection
				Values.selected_target = null
				Values.selected_block = null
			if "block" in selection:
				Values.selected_projectile = null
				Values.selected_target = null
				Values.selected_block = selection
	if not pause_state:
		hud.update_mouse_box(mouse_coords)

func _process(_delta):
	if not pause_state:
		# Mouse movement.
		if not free_mouse_state:
			_mouse_motion.y = clamp(_mouse_motion.y, -1560, 1560)
			camera.transform.basis = Basis.from_euler(Vector3(_mouse_motion.y * -0.001, _mouse_motion.x * -0.001, 0))
		if Values.selection_changed:
			Values.selection_changed = false
			if Values.selected_projectile:
				hud.display_projectile(Values.selected_projectile, map.projectiles_data[Values.selected_projectile.id])
			if Values.selected_target:
				if map.characters_data.has(Values.selected_target.id):
					hud.display_target(Values.selected_target, map.characters_data[Values.selected_target.id])
				else:
					Values.selected_target = null
			if Values.selected_block:
				if map.tiles_img.has(Values.selected_block.block):
					$"../HUD/Block/Picture".texture = map.tiles_img[Values.selected_block.block]
					hud.display_block(Values.selected_block, map.tiles_data[Values.selected_block.block])

func _physics_process(_delta):
	if not pause_state and not free_mouse_state:
		update_mouse_coordinates()
		camera.transform.origin = map.characters[map.owned_character].transform.origin + Vector3(0, 1.6, 0)
		var movement_vec2 = Input.get_vector("move_left", "move_right", "move_forward", "move_back")
		if movement_vec2 != Vector2.ZERO and not Values.move_set:
			# invert x axis
			movement_vec2.x = -movement_vec2.x
			movement_vec2 = movement_vec2.rotated(camera.rotation.y - PI / 2.)
			map.send_oriented_action(Values.ACTION_MOVE, rad_to_deg(movement_vec2.angle()))

func _unhandled_input(event):
	if event.is_action_pressed("pause"):
		if character_sheet.visible:
			character_sheet.visible = false
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
			free_mouse_state = true
		if event.is_action_released("free_mouse"):
			Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)
			free_mouse_state = false
		elif event is InputEventMouseMotion:
			_mouse_motion += event.relative
		if event.is_action_pressed("skill_tab_1"):
			hud.skill_button_1.set_pressed(true)
		if event.is_action_pressed("skill_tab_2"):
			hud.skill_button_2.set_pressed(true)
		if event.is_action_pressed("skill_tab_3"):
			hud.skill_button_3.set_pressed(true)
		if event.is_action_pressed("skill_tab_4"):
			hud.skill_button_4.set_pressed(true)
		if event.is_action_pressed("skill_tab_5"):
			hud.skill_button_5.set_pressed(true)
		if event.is_action_pressed("skill_tab_6"):
			hud.skill_button_6.set_pressed(true)
		if event.is_action_pressed("skill_tab_7"):
			hud.skill_button_7.set_pressed(true)
		if event.is_action_pressed("skill_tab_8"):
			hud.skill_button_8.set_pressed(true)
		if event.is_action_pressed("skill_tab_9"):
			hud.skill_button_9.set_pressed(true)
		if event.is_action_pressed("skill_tab_10"):
			hud.skill_button_10.set_pressed(true)
		if event.is_action_pressed("skill_tab_11"):
			hud.skill_button_11.set_pressed(true)
		if event.is_action_pressed("skill_tab_12"):
			hud.skill_button_12.set_pressed(true)
		if event.is_action_pressed("action_move"):
			hud.move.set_pressed(true)
		if event.is_action_pressed("action_attack"):
			hud.attack.set_pressed(true)
		if event.is_action_pressed("action_interact"):
			hud.interact.set_pressed(true)
		if event.is_action_pressed("action_rest"):
			hud.rest.set_pressed(true)
		if event.is_action_pressed("display_stats"):
			if not character_sheet.visible:
				character_sheet.display_stats()
			else:
				character_sheet.visible = false
		if event.is_action_pressed("inventory"):
			if not hud.inventory.visible:
				hud.inventory.display_inventory()
			else:
				hud.inventory.visible = false
		if event.is_action_pressed("action") and not Values.updating_state and not Values.link.hasState():
			if Values.mode == Values.ACTION_MODE_ACTIVATION:
				map.send_targeted_action(Values.ACTION_ACTIVATION, Values.TARGET_BLOCK, 0, Vector3(floor(Values.coord.z), floor(Values.coord.x), floor(Values.coord.y)))
			if Values.mode == Values.ACTION_MODE_ATTACK:
				map.send_targeted_action(Values.ACTION_STRIKE, Values.TARGET_CHARACTER, Values.selected_target.id, Vector3.ZERO)

func fix_vec(vec: Vector3):
	var space = get_world_3d().direct_space_state
	var query = PhysicsRayQueryParameters3D.create(vec, vec + Vector3(0, -10, 0), 0x1)
	var result = space.intersect_ray(query)
	if not result.is_empty():
		vec = result["position"]
	return map.round_vec(vec)
