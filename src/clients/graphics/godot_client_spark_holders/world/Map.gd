extends Node3D

var thread
var mutex
var baking_done = false

var owned_characters = []
var phantoms = {}
var phantom_lines = {}

var navigations = {}
var multiMeshInstances = {}
var maps = {}
var tiles_count = {}

var grid = []
var regions = {}
var lights = {}
var characters = {}
var projectiles = {}
var furnitures = {}
var tiles_data = {}
var characters_data = {}
var projectiles_data = {}
var furnitures_data = {}
var base_furnitures = {}
var base_furnitures_off = {}
var materials = {}

var tiles_img = {}

var board_material = preload("res://data/materials/board_material.tres")
var phantom_material = preload("res://data/materials/phantom.tres")
var base_fire = preload("res://models/fire.tscn")
var grid_material = StandardMaterial3D.new()
var light_0 = StandardMaterial3D.new()
var light_1 = StandardMaterial3D.new()
var light_2 = StandardMaterial3D.new()
var light_3 = StandardMaterial3D.new()
var light_4 = StandardMaterial3D.new()
var light_5 = StandardMaterial3D.new()
var light_6 = StandardMaterial3D.new()
var light_7 = StandardMaterial3D.new()
var light_8 = StandardMaterial3D.new()
var light_9 = StandardMaterial3D.new()
var light_a = StandardMaterial3D.new()
var light_b = StandardMaterial3D.new()
var light_c = StandardMaterial3D.new()
var light_d = StandardMaterial3D.new()
var light_e = StandardMaterial3D.new()
var light_f = StandardMaterial3D.new()

var base_character = preload("res://models/character.tscn")
var base_phantom = preload("res://models/phantom.tscn")
var base_projectile = preload("res://models/projectile.tscn")

@onready var n_view = $"../View"
@onready var n_hud = $"../HUD"
@onready var n_tiles = $Tiles
@onready var n_floor = $Tiles/Floor
@onready var n_grid = $Grid
@onready var n_lights = $Lights
@onready var n_characters = $Characters
@onready var n_projectiles = $Projectiles
@onready var n_furnitures = $Furnitures

# Called when the node enters the scene tree for the first time.
func _ready():
	light_0.albedo_color = "000000"
	light_1.albedo_color = "111111"
	light_2.albedo_color = "222222"
	light_3.albedo_color = "333333"
	light_4.albedo_color = "444444"
	light_5.albedo_color = "555555"
	light_5.albedo_color = "666666"
	light_7.albedo_color = "777777"
	light_8.albedo_color = "888888"
	light_9.albedo_color = "999999"
	light_a.albedo_color = "aaaaaa"
	light_b.albedo_color = "bbbbbb"
	light_c.albedo_color = "cccccc"
	light_d.albedo_color = "dddddd"
	light_e.albedo_color = "eeeeee"
	light_f.albedo_color = "ffffff"
	# var ip = "84.97.162.152"
	# var ip = "192.168.168.164"
	# var ip = "192.168.1.83"
	var ip = "127.0.0.1"
	thread = Thread.new()
	mutex = Mutex.new()
	Values.link.initialize(ip)
	Values.link.getState()
	grid_material.albedo_color = "000000"
	owned_characters = Values.link.getControlledParty()
	init_actions()
	characters_data = Values.link.getCharacters()
	projectiles_data = Values.link.getProjectiles()
	for character_id in characters_data:
		add_character(character_id, characters_data[character_id])
	for projectile_id in projectiles_data:
		add_projectile(projectile_id, projectiles_data[projectile_id])
	var offset = Values.link.getOffsets(owned_characters[0])
	var size = Values.link.getSizes(owned_characters[0])
	var mid_size = Vector3(size.x / 2.0, size.y / 2.0, size.z / 2.0)
	n_view.transform.origin = Vector3(offset.x + mid_size.x - 5, offset.y + n_view.transform.origin.y, offset.z + mid_size.z)
	for tile in Values.link.getAvaillableTiles():
		initialize_tile(tile)
	create_grid()
	for character_id in owned_characters:
		init_tiles(character_id)
	regions = Values.link.getCurrentRegions()
	furnitures_data = Values.link.getFurnitures()
	display_map()
	var map_rid = get_world_3d().get_navigation_map()
	NavigationServer3D.map_set_cell_size(map_rid, 0.05)
	NavigationServer3D.map_set_cell_height(map_rid, 0.05)
	for character_id in owned_characters:
		# Phantom
		phantoms[character_id] = base_phantom.instantiate()
		phantoms[character_id].scale_object_local(Vector3(characters_data[character_id]["size"], characters_data[character_id]["size"], characters_data[character_id]["size"]))
		phantoms[character_id].id = character_id
		phantoms[character_id].transform.origin = characters[character_id].transform.origin
		n_characters.add_child(phantoms[character_id])
		# Navigation3D
		var character_size = round_float(characters_data[character_id]["size"])
		if !navigations.has(character_size):
			navigations[character_size] = NavigationMesh.new()
			navigations[character_size].sample_partition_type = NavigationMesh.SAMPLE_PARTITION_LAYERS
			navigations[character_size].geometry_parsed_geometry_type = NavigationMesh.PARSED_GEOMETRY_MESH_INSTANCES
			navigations[character_size].cell_size = 0.05
			navigations[character_size].cell_height = 0.05
			navigations[character_size].agent_height = 0.0
			navigations[character_size].agent_max_climb = 0.0
			navigations[character_size].agent_max_slope = 0.02
			navigations[character_size].agent_radius = character_size
	
	Values.selected_team = characters[owned_characters[0]]
	select_character(owned_characters[0])
	bake_navigation_meshes()
	n_hud.move.set_pressed(true)
	thread.start(state_updater, Thread.PRIORITY_LOW)
	
func bake_navigation_meshes():
	if !baking_done:
		for size in navigations:
			var data = NavigationMeshSourceGeometryData3D.new()
			NavigationMeshGenerator.parse_source_geometry_data(navigations[size], data, n_tiles)
			NavigationMeshGenerator.bake_from_source_geometry_data(navigations[size], data)
		n_tiles.set_navigation_mesh(null)
		baking_done = true

func set_navigation_mesh(character_id: int):
	n_tiles.set_navigation_mesh(navigations[round_float(characters_data[character_id]["size"])])

func select_character(character_id: int):
	var offset = Values.link.getOffsets(character_id)
	if n_floor.transform.origin.y + 0.5 != round_float(characters_data[character_id]["z"]):
		var offsetX = 5 * sin(deg_to_rad($"../View/Camera3D".rotation_degrees.y))
		var offsetZ = 5 * cos(deg_to_rad($"../View/Camera3D".rotation_degrees.y))
		n_view.transform.origin = Vector3(characters_data[character_id]["y"] + offsetX, offset.y + n_view.transform.origin.y, characters_data[character_id]["x"] + offsetZ)
	n_floor.transform.origin = Vector3(characters_data[character_id]["y"], characters_data[character_id]["z"] + 0.5, characters_data[character_id]["x"])
	bake_navigation_meshes()
	set_navigation_mesh(character_id)
	n_hud.display_team(characters[character_id], characters_data[character_id])

func state_updater():
	var running = true
	while running:
		while (Values.updating_state || !Values.link.hasState()):
			if !Values.updating_state && !Values.link.hasState():
				bake_navigation_meshes()
				set_navigation_mesh(Values.selected_team.id)
			await get_tree().create_timer(0.001).timeout
		mutex.lock()
		running = Values.link.getState()
		if running:
			var next_characters_data = Values.link.getCharacters()
			for character_id in characters_data.keys():
				if !next_characters_data.has(character_id):
					n_characters.remove_child(characters[character_id])
					characters.erase(character_id)
			var new_regions = Values.link.getCurrentRegions()
			for map_id in new_regions:
				if !regions.has(map_id):
					baking_done = false
			for map_id in regions:
				if !new_regions.has(map_id):
					baking_done = false
			regions = new_regions
			for character_id in next_characters_data:
				if !characters_data.has(character_id):
					add_character(character_id, next_characters_data[character_id])
			var update = false
			for character_id in owned_characters:
				if Values.link.needTilesUpdate(character_id):
					init_tiles(character_id)
				else:
					create_tiles(character_id, floor(next_characters_data[character_id]["y"]) - floor(characters_data[character_id]["y"]), floor(next_characters_data[character_id]["x"]) - floor(characters_data[character_id]["x"]))
				if floor(next_characters_data[character_id]["y"]) != floor(characters_data[character_id]["y"]) || floor(next_characters_data[character_id]["x"]) != floor(characters_data[character_id]["x"]):
					update = true
			if update:
				furnitures_data = Values.link.getFurnitures()
				display_map()
			characters_data = next_characters_data
			var next_projectiles_data = Values.link.getProjectiles()
			for projectile_id in projectiles_data.keys():
				if !next_projectiles_data.has(projectile_id):
					n_projectiles.remove_child(projectiles[projectile_id])
					projectiles.erase(projectile_id)
			for projectile_id in next_projectiles_data:
				if !projectiles_data.has(projectile_id):
					add_projectile(projectile_id, next_projectiles_data[projectile_id])
			projectiles_data = next_projectiles_data
		Values.updating_state = true
		mutex.unlock()

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass
	
func _physics_process(delta):
	if mutex.try_lock():
		if Values.updating_state:
			var done = true
			for character_id in characters_data:
				var dest = Vector3(characters_data[character_id]["y"], characters[character_id].transform.origin.y, characters_data[character_id]["x"])
				var movement = (dest - characters[character_id].transform.origin).normalized() * 10
				characters[character_id].collision_layer = 0x0000
				characters[character_id].collision_mask = 0x0000
				characters[character_id].move_and_collide(Vector3(movement.x, 0, movement.z) * delta)
				characters[character_id].collision_layer = 0x0002
				characters[character_id].collision_mask = 0x0013
				characters[character_id].rotation_degrees = Vector3(0, characters_data[character_id]["orientation"], 0)	
				if characters[character_id].transform.origin.distance_to(dest) < 1:#0.05:
					characters[character_id].transform.origin = dest
					if phantoms.has(character_id):
						if phantom_lines.has(character_id):
							for phantom_line in phantom_lines[character_id]:
								n_characters.remove_child(phantom_line)
						phantom_lines[character_id] = []
						phantoms[character_id].transform.origin = dest
						phantoms[character_id].visible = false
				done = done && (characters[character_id].transform.origin == dest)
			for projectile_id in projectiles_data:
				var dest = Vector3(projectiles_data[projectile_id]["y"], projectiles[projectile_id].transform.origin.y, projectiles_data[projectile_id]["x"])
				var movement = (dest - projectiles[projectile_id].transform.origin).normalized() * 10
				projectiles[projectile_id].move_and_collide(Vector3(movement.x, 0, movement.z) * delta)
				projectiles[projectile_id].rotation_degrees = Vector3(0, projectiles_data[projectile_id]["orientation"], 0)
				if projectiles[projectile_id].transform.origin.distance_to(dest) < 0.1:
					projectiles[projectile_id].transform.origin = dest
				done = done && (projectiles[projectile_id].transform.origin == dest)
			Values.updating_state = !done
		mutex.unlock()
	
func create_grid():
	var offset = Values.link.getOffsets(owned_characters[0])
	var size = Values.link.getSizes(owned_characters[0])
	var mid_size = Vector3(size.x / 2.0, size.y / 2.0, size.z / 2.0)
	var pos = Vector3(offset.x + mid_size.x, offset.y, offset.z + mid_size.z)
	for x in range(0, size.x + 1):
		var line = MeshInstance3D.new()
		line.mesh = BoxMesh.new()
		line.mesh.set_size(Vector3(0.05, 0.001, size.z))
		line.set_surface_override_material(0, grid_material)
		line.transform.origin = pos + Vector3(x - mid_size.x, offset.y + 1.001, 0)
		grid.append(line)
		n_grid.add_child(line)
	for z in range(0, size.z + 1):
		var line = MeshInstance3D.new()
		line.mesh = BoxMesh.new()
		line.mesh.set_size(Vector3(size.x, 0.001, 0.05))
		line.set_surface_override_material(0, grid_material)
		line.transform.origin = pos + Vector3(0, offset.y + 1.001, z - mid_size.z)
		grid.append(line)
		n_grid.add_child(line)
	
func get_light(light: int):
	match light:
		0: return light_0
		1: return light_1
		2: return light_2
		3: return light_3
		4: return light_4
		5: return light_5
		6: return light_6
		7: return light_7
		8: return light_8
		9: return light_9
		10: return light_a
		11: return light_b
		12: return light_c
		13: return light_d
		14: return light_e
		15: return light_f

func init_tiles(character_id: int):
	var current_tiles = Values.link.getTiles(character_id)
	var current_lights = Values.link.getLights(character_id)
	var offset = Values.link.getOffsets(character_id)
	var size = Values.link.getSizes(character_id)
	for x in range(0, size.x):
		for z in range(0, size.z):
			baking_done = add_tile(offset, x, z, current_tiles, current_lights) && baking_done

func create_tiles(character_id: int, x_direction: int, z_direction: int):
	if x_direction == 0 && z_direction == 0:
		return
	var current_tiles = Values.link.getTiles(character_id)
	var current_lights = Values.link.getLights(character_id)
	var offset = Values.link.getOffsets(character_id)
	var size = Values.link.getSizes(character_id)
	if x_direction > 0:
		for z in range(0, size.z):
			baking_done = add_tile(offset, size.x - 1, z, current_tiles, current_lights) && baking_done
	if x_direction < 0:
		for z in range(0, size.z):
			baking_done = add_tile(offset, 0, z, current_tiles, current_lights) && baking_done
	if z_direction > 0:
		for x in range(0, size.x):
			baking_done = add_tile(offset, x, size.z - 1, current_tiles, current_lights) && baking_done
	if z_direction < 0:
		for x in range(0, size.x):
			baking_done = add_tile(offset, x, 0, current_tiles, current_lights) && baking_done

func add_tile(offset: Vector3, x: int, z: int, current_tiles: Array, current_lights: Array):
	if current_tiles[x][z] == "TXT_MIST" || current_tiles[x][z] == "TXT_VOID":
		return true
	var global_x = x + offset.x
	var global_z = z + offset.z
	var id = Vector3(global_x, offset.y, global_z)
	var map_id = Values.link.getMapFromCoords(id)
	if lights.has(id):
		n_lights.remove_child(lights[id])
	var light = MeshInstance3D.new()
	light.mesh = BoxMesh.new()
	light.set_surface_override_material(0, get_light(current_lights[x][z]))
	if tiles_data[current_tiles[x][z]]["solid"]:
		light.mesh.set_size(Vector3(1, 3, 1))
		light.transform.origin = Vector3(global_x + 0.5, offset.y + 1.5, global_z + 0.5)
	else:
		light.mesh.set_size(Vector3.ONE)
		light.transform.origin = Vector3(global_x + 0.5, offset.y + 0.5, global_z + 0.5)
	lights[id] = light
	n_lights.add_child(light)
	if !maps.has(map_id):
		maps[map_id] = {}
	if !maps[map_id].has(id):
		maps[map_id][id] = current_tiles[x][z]
		if !tiles_count[current_tiles[x][z]].has(map_id):
			tiles_count[current_tiles[x][z]][map_id] = 1
		else:
			tiles_count[current_tiles[x][z]][map_id] = tiles_count[current_tiles[x][z]][map_id] + 1
		return false
	return true

func display_map():
	var tile_current = {}
	for tile_type in multiMeshInstances:
		var total = 0
		tile_current[tile_type] = 0
		for map_id in regions:
			if tiles_count[tile_type].has(map_id):
				total = total + tiles_count[tile_type][map_id]
		multiMeshInstances[tile_type].multimesh.set_instance_count(total)
	for map_id in regions:
		if maps.has(map_id):
			var map = maps[map_id]
			for tile in map:
				var tile_type = map[tile]
				var coord = Vector3.ZERO
				if tiles_data[tile_type]["solid"]:
					coord = Vector3(tile.x + 0.5, tile.y + 1.5, tile.z + 0.5)
				elif tiles_data[tile_type]["unwalkable"]:
					coord = Vector3(tile.x + 0.5, tile.y + 0.4, tile.z + 0.5)
				else:
					coord = Vector3(tile.x + 0.5, tile.y + 0.5, tile.z + 0.5)
				multiMeshInstances[tile_type].multimesh.set_instance_transform(tile_current[tile_type], Transform3D.IDENTITY.translated(coord))
				tile_current[tile_type] = tile_current[tile_type] + 1
	# clear furnitures
	for coord in furnitures.keys():
		var furniture = furnitures[coord]
		n_furnitures.remove_child(furniture)
		furnitures.erase(furniture)
	for coord in furnitures_data:
		add_furniture(furnitures_data[coord], coord)

func initialize_tile(tile: String):
	if tile != "TXT_MIST" && tile != "TXT_VOID":
		tiles_data[tile] = Values.link.getDataFromTile(tile)
		tiles_count[tile] = {}
		materials[tile] = load(tiles_data[tile]["path"])
		var multimeshinstance = MultiMeshInstance3D.new()
		var multimesh = MultiMesh.new()
		var mesh = BoxMesh.new()
		if tiles_data[tile]["solid"]:
			mesh.set_size(Vector3(1, 3, 1))
		elif tiles_data[tile]["unwalkable"]:
			mesh = BoxMesh.new()
			mesh.set_size(Vector3(1, 0.8, 1))
		else:
			mesh.set_size(Vector3.ONE)
		mesh.surface_set_material(0, materials[tile])
		multimesh.set_mesh(mesh)
		multimesh.transform_format = MultiMesh.TRANSFORM_3D
		multimeshinstance.set_multimesh(multimesh)
		multiMeshInstances[tile] = multimeshinstance
		n_tiles.add_child(multimeshinstance)
		
		if tile == "TXT_GRASS" || tile == "TXT_FLOOR_STONE" || tile == "TXT_WALL_STONE":
			var img = materials[tile].albedo_texture.get_image()
			tiles_img[tile] = ImageTexture.create_from_image(img)

func add_furniture(furniture_data: Dictionary, coords: Vector3):
	if !base_furnitures.has(furniture_data["name"]):
		var model = load(furniture_data["path"])
		base_furnitures[furniture_data["name"]] = model
		if furniture_data.has("path_off"):
			var model_off = load(furniture_data["path_off"])
			if model_off != null:
				base_furnitures_off[furniture_data["name"]] = model_off
			else:
				base_furnitures_off[furniture_data["name"]] = model
	var furniture
	if  furniture_data["isOn"]:
		furniture = base_furnitures[furniture_data["name"]].instantiate()
	else:
		furniture = base_furnitures_off[furniture_data["name"]].instantiate()
	
	if furniture_data["light"] > 0:
		furniture.add_child(add_fire(furniture_data["fire_pos"], furniture_data["fire_size"], furniture_data["light"]))
	furniture.transform.origin = Vector3(coords.x + 0.5, coords.y + 1, coords.z + 0.5)
	furniture.rotation_degrees = Vector3(0, furniture_data["orientation"], 0)
	furnitures[coords] = furniture
	n_furnitures.add_child(furniture)
		
func add_fire(coords: Vector3, size: float, light_power: int):
	var fire = base_fire.instantiate()
	fire.initialize(coords, size, light_power)
	return fire

func get_color(character_data: Dictionary):
	match(Values.link.getRelation(characters_data[owned_characters[0]]["team"], character_data["team"])):
		"SAME": return "0000FF"
		"ALLY": return "00FF00"
		"NEUTRAL": return "FFFF00"
		"ENEMY": return "FF0000"

func add_character(character_id: int, character_data: Dictionary):
	var character = base_character.instantiate()
	character.scale_object_local(Vector3(character_data["size"], character_data["size"], character_data["size"]))
	character.set_color(get_color(character_data))
	character.transform.origin = Vector3(character_data["y"], character_data["z"] + 1, character_data["x"])
	character.rotation_degrees = Vector3(0, character_data["orientation"], 0)
	characters[character_id] = character
	character.id = character_id
	character.character = character_data["name"]
	character.set_attack_range(1 / character_data["size"])
	n_characters.add_child(character)

func add_projectile(projectile_id: int, projectile_data: Dictionary):
	var projectile = base_projectile.instantiate()
	projectile.scale_object_local(Vector3(projectile_data["size"], projectile_data["size"], projectile_data["size"]))
	projectile.transform.origin = Vector3(projectile_data["y"], projectile_data["z"] + 1, projectile_data["x"])
	projectile.rotation_degrees = Vector3(0, projectile_data["orientation"], 0)
	projectiles[projectile_id] = projectile
	projectile.id = projectile_id
	projectile.projectile = projectile_data["name"]
	n_projectiles.add_child(projectile)

func update_phantom(character_id: int):
	var offset = Values.link.getOffsets(character_id)
	phantoms[character_id].visible = true
	phantoms[character_id].transform.origin = Vector3(Values.coord.x, offset.y + 1, Values.coord.z) 
	characters[character_id].nav.target_position = phantoms[character_id].transform.origin
	characters[character_id].nav.get_next_path_position()
	if phantom_lines.has(character_id):
		for phantom_line in phantom_lines[character_id]:
			n_characters.remove_child(phantom_line)
	phantom_lines[character_id] = []
	var previous_vec = characters[character_id].transform.origin + Vector3(0, 0.1, 0)
	var last_angle = Vector3.ZERO
	var ap_cost = 0.
	for vec in characters[character_id].nav.get_current_navigation_path():
		vec = round_vec(vec)
		var distance = previous_vec.distance_to(vec)
		if distance != 0.:
			ap_cost = ap_cost + Values.link.getMoveCost(Values.selected_team.id, previous_vec.z, previous_vec.x, vec.z, vec.x)
			var phantom_line = MeshInstance3D.new()
			phantom_line.mesh = BoxMesh.new()
			phantom_line.mesh.set_size(Vector3(0.1, 0.01, distance))
			last_angle = Vector3(0, Values.link.getOrientationToTarget(Vector2(previous_vec.z, previous_vec.x), Vector2(vec.z, vec.x)), 0)
			phantom_line.rotation_degrees = last_angle
			phantom_line.transform.origin = Vector3((vec.x + previous_vec.x) / 2, offset.y + 1.001, (vec.z + previous_vec.z) / 2)
			phantom_line.set_surface_override_material(0, phantom_material)
			phantom_lines[character_id].push_back(phantom_line)
			n_characters.add_child(phantom_line)
			previous_vec = vec
	phantoms[character_id].rotation_degrees = last_angle
	return String.num(ap_cost, 3)

func round_float(number: float):
	var value = int(number * 1000. + 0.5)
	return float(value) / 1000.

func round_vec(vec: Vector3):
	return Vector3(round_float(vec.x), round_float(vec.y), round_float(vec.z))

func init_actions():
	Values.actions = {}
	Values.actions["ids"] = owned_characters
	Values.actions["types"] = {}
	Values.actions["arg1"] = {}
	Values.actions["arg2"] = {}
	Values.actions["overcharge_power"] = {}
	Values.actions["overcharge_duration"] = {}
	Values.actions["overcharge_range"] = {}
	for id in owned_characters:
		Values.actions["types"][id] = []
		Values.actions["arg1"][id] = []
		Values.actions["arg2"][id] = []
		Values.actions["overcharge_power"][id] = []
		Values.actions["overcharge_duration"][id] = []
		Values.actions["overcharge_range"][id] = []

func clear_actions(id):
	Values.actions["types"][id] = []
	Values.actions["arg1"][id] = []
	Values.actions["arg2"][id] = []
	Values.actions["overcharge_power"][id] = []
	Values.actions["overcharge_duration"][id] = []
	Values.actions["overcharge_range"][id] = []

func add_base_action(id, type):
	Values.actions["types"][id].push_back(type)
	Values.actions["arg1"][id].push_back(0)
	Values.actions["arg2"][id].push_back(0)
	Values.actions["overcharge_power"][id].push_back(0)
	Values.actions["overcharge_duration"][id].push_back(0)
	Values.actions["overcharge_range"][id].push_back(0)
	
func add_targeted_action(id, type, target_type, target_id, pos):
	Values.actions["types"][id].push_back(type)
	var target = {}
	target["type"] = target_type
	target["id"] = target_id
	target["pos"] = pos
	Values.actions["arg1"][id].push_back(target)
	Values.actions["arg2"][id].push_back(0)
	Values.actions["overcharge_power"][id].push_back(0)
	Values.actions["overcharge_duration"][id].push_back(0)
	Values.actions["overcharge_range"][id].push_back(0)
	
func add_gear_action(id, type, item_id):
	Values.actions["types"][id].push_back(type)
	Values.actions["arg1"][id].push_back(item_id)
	Values.actions["arg2"][id].push_back(0)
	Values.actions["overcharge_power"][id].push_back(0)
	Values.actions["overcharge_duration"][id].push_back(0)
	Values.actions["overcharge_range"][id].push_back(0)
	
func add_skill_action(id, type, target_type, target_id, pos, skill, overcharge_power, overcharge_duration, overcharge_range):
	Values.actions["types"][id].push_back(type)
	var target = {}
	target["type"] = target_type
	target["id"] = target_id
	target["pos"] = pos
	Values.actions["arg1"][id].push_back(target)
	Values.actions["arg2"][id].push_back(skill)
	Values.actions["overcharge_power"][id].push_back(overcharge_power)
	Values.actions["overcharge_duration"][id].push_back(overcharge_duration)
	Values.actions["overcharge_range"][id].push_back(overcharge_range)
