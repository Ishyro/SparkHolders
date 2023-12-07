extends Node3D

const MOVEMENT_SPEED = 0.6
const MOVEMENT_FRICTION = 0.5

var _mouse_motion = Vector2.ZERO
var velocity = Vector3.ZERO
var pause_state = false

@onready var camera = $Camera3D
@onready var camera_attributes = camera.attributes
@onready var pause = $"../Menus/Pause"
@onready var character_sheet = $"../Menus/CharacterSheet"
@onready var hud = $"../HUD"
@onready var map = $"../Map"

func _ready():
	Input.set_mouse_mode(Input.MOUSE_MODE_CONFINED)

func update_mouse_coordinates():
	var mouse_coords = camera.get_viewport().get_mouse_position()
	var from = camera.project_ray_origin(mouse_coords)
	var to = from + camera.project_ray_normal(mouse_coords) * 1000.0
	var space = get_world_3d().direct_space_state
	var query = PhysicsRayQueryParameters3D.create(from, to, 0x000f)
	query.collide_with_areas = true
	var result = space.intersect_ray(query)
	if not result.is_empty():
		Values.coord = result["position"]
		if Input.is_action_pressed("select"):
			Values.selection_changed = true
			var selection = result["collider"]
			if Values.selected_target:
				Values.selected_target.unselect()
			if Values.selected_team:
				Values.selected_team.unselect()
			if "character" in selection:
				var owned = false
				for character_id in map.owned_characters:
					if character_id == selection.id:
						Values.selected_team = selection
						owned = true
						map.select_character(Values.selected_team.id)
						break
				if !owned:
					Values.selected_target = selection
				Values.selected_projectile = null
				Values.selected_block = null
			if "phantom" in selection:
				Values.selected_team = map.characters[selection.id]
				map.clear_actions(Values.selected_team.id)
				Values.mode = Values.ACTION_MODE_MOVE
				map.phantoms[Values.selected_team.id].collision_layer = 0x0010
				map.set_navigation_mesh(Values.selected_team.id)
			if "projectile" in selection:
				Values.selected_projectile = selection
				Values.selected_target = null
				Values.selected_block = null
			if "block" in selection:
				Values.selected_projectile = null
				Values.selected_target = null
				Values.selected_block = selection
	if not pause_state:
		var ap_cost = ""
		if(Values.selected_team and Values.mode == Values.ACTION_MOVE and not Values.updating_state):
			ap_cost = map.update_phantom(Values.selected_team.id)
		hud.update_mouse_box(mouse_coords, ap_cost)

func _process(_delta):
	if(not pause_state):
		# Mouse movement.
		_mouse_motion.x = clamp(_mouse_motion.x, -1560, 1560)
		camera.rotation_degrees = Vector3(clamp(camera.rotation_degrees.x + _mouse_motion.y * -0.001, -90, 0), camera.rotation_degrees.y, camera.rotation_degrees.z)
		if Values.selection_changed:
			Values.selection_changed = false
			if Values.selected_projectile:
				hud.display_projectile(Values.selected_projectile, map.projectiles_data[Values.selected_projectile.id])
			if Values.selected_target:
				if map.characters_data.has(Values.selected_target.id):
					hud.display_target(Values.selected_target, map.characters_data[Values.selected_target.id])
				else:
					Values.selected_target = null
			if Values.selected_team:
				hud.display_team(Values.selected_team, map.characters_data[Values.selected_team.id])
			if Values.selected_block:
				if map.tiles_img.has(Values.selected_block.block):
					$"../HUD/Block/Picture".texture = map.tiles_img[Values.selected_block.block]
					hud.display_block(Values.selected_block, map.tiles_data[Values.selected_block.block])

func _physics_process(_delta):
	if(not pause_state):
		update_mouse_coordinates()
		camera_attributes.dof_blur_far_enabled = true
		camera_attributes.dof_blur_far_distance = 100 * 1.5
		camera_attributes.dof_blur_far_transition = 100 * 0.125
		
		# Keyboard movement.
		var movement_vec2 = Input.get_vector("move_left", "move_right", "move_forward", "move_back")
		movement_vec2 = movement_vec2.rotated(deg_to_rad(-camera.rotation_degrees.y))
		var movement = Vector3(movement_vec2.x, 0, movement_vec2.y)
		
		movement *= MOVEMENT_SPEED * ( 3 + transform.origin.y) / 20

		velocity += Vector3(movement.x, 0, movement.z)
		# Apply horizontal friction.
		velocity.x *= MOVEMENT_FRICTION
		velocity.z *= MOVEMENT_FRICTION
		transform.origin += velocity

func _unhandled_input(event):
	if event.is_action_pressed("pause"):
		if character_sheet.visible:
			character_sheet.visible = false
		else:
			pause_state = not pause_state
			pause.visible = pause_state
	if not pause_state:
		if event.is_action_pressed("rotate_up_down") and event.double_click:
				camera.rotation_degrees = Vector3(-60, camera.rotation_degrees.y, 0)
		if Input.is_action_pressed("rotate_up_down"):
			if event is InputEventMouseMotion:
				_mouse_motion += event.relative
		else:
			_mouse_motion = Vector2.ZERO
		if event.is_action_pressed("zoom_in"):
			transform.origin = Vector3(transform.origin.x, max(transform.origin.y - 1, 2), transform.origin.z)
		if event.is_action_pressed("zoom_out"):
			transform.origin = Vector3(transform.origin.x, min(transform.origin.y + 1, 20), transform.origin.z)
		if event.is_action_pressed("rotate_left"):
			camera.rotation_degrees += Vector3(0, 45, 0)
		if event.is_action_pressed("rotate_right"):
			camera.rotation_degrees += Vector3(0, -45, 0)
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
			if !character_sheet.visible:
				character_sheet.display_stats()
			else:
				character_sheet.visible = false
		if event.is_action_pressed("inventory"):
			if !hud.inventory.visible:
				hud.inventory.display_inventory()
			else:
				hud.inventory.visible = false
		if event.is_action_pressed("action") && !Values.updating_state && !Values.link.hasState():
			Values.action_muxtex.lock()
			if Values.mode == Values.ACTION_MODE_MOVE && map.baking_done:
				Values.mode = Values.ACTION_MODE_NONE
				map.phantoms[Values.selected_team.id].collision_layer = 0x0008
				var is_first = true
				map.clear_actions(Values.selected_team.id)
				for vec in map.characters[Values.selected_team.id].nav.get_current_navigation_path():
					if is_first:
						is_first = false
					else:
						vec = map.round_vec(vec)
						map.add_targeted_action(Values.selected_team.id, Values.ACTION_MOVE, Values.TARGET_COORDINATES, 0, Vector3(vec.z, vec.x, map.characters_data[Values.selected_team.id]["z"]))
			if Values.mode == Values.ACTION_MODE_ACTIVATION:
				map.add_targeted_action(Values.selected_team.id, Values.ACTION_ACTIVATION, Values.TARGET_TILE, 0, Vector3(floor(Values.coord.z), floor(Values.coord.x), map.characters_data[Values.selected_team.id]["z"]))
			if Values.mode == Values.ACTION_MODE_ATTACK:
				map.add_targeted_action(Values.selected_team.id, Values.ACTION_STRIKE, Values.TARGET_CHARACTER, Values.selected_target.id, Vector3.ZERO)
			Values.action_muxtex.unlock()
		if event.is_action_pressed("send_actions"):
			map.send_actions()
