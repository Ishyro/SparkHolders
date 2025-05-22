extends Node3D

var base_fire = preload("res://models/fire.tscn")

var blocks_data = {}
var blocks_count = {}
var current_blocks = {}
var materials = {}
var furnitures = {}
# key collider, value Vector3
var colliders = {}

var multimeshes = {}

func display_chunk(furnitures_data: Dictionary):
	var block_current = {}
	for block_type in blocks_count:
		multimeshes[block_type].multimesh.set_instance_count(blocks_count[block_type])
		block_current[block_type] = 0
	for block in current_blocks:
		var block_type = current_blocks[block]
		var coord = Vector3.ZERO
		var angle_offset
		if blocks_data[block_type]["type"] == Values.macros["BLOCK_SOLID"]:
			coord = Vector3(block.x + 0.5, block.y + 0.5, block.z + 0.5)
			angle_offset = 90
		elif blocks_data[block_type]["type"] == Values.macros["BLOCK_LIQUID"]:
			coord = Vector3(block.x + 0.5, block.y + 0.8, block.z + 0.5)
			angle_offset = 90
		elif blocks_data[block_type]["type"] == Values.macros["BLOCK_SLOPE"]:
			coord = Vector3(block.x + 0.5, block.y + 0.5, block.z + 0.5)
			angle_offset = 90
		elif blocks_data[block_type]["type"] == Values.macros["BLOCK_STAIRS"]:
			coord = Vector3(block.x + 0.5, block.y + 0.5, block.z + 0.5)
			angle_offset = 180
		multimeshes[block_type].multimesh.set_instance_transform(block_current[block_type], Transform3D.IDENTITY.translated(coord).rotated_local(Vector3.UP, deg_to_rad(angle_offset + blocks_data[block_type]["orientation_z"])))
		block_current[block_type] = block_current[block_type] + 1
	for coord in furnitures_data:
		add_furniture(furnitures_data[coord], coord)

func add_block(coord: Vector3, chunk_current_blocks: Dictionary):
	current_blocks = chunk_current_blocks
	if not blocks_count.has(current_blocks[coord]):
		initialize_block(current_blocks[coord])
	blocks_count[current_blocks[coord]] = blocks_count[current_blocks[coord]] + 1

func initialize_block(block: String):
	if block != "TXT_MIST" and block != "TXT_VOID":
		blocks_data[block] = Values.link.getDataFromBlock(block)
		blocks_count[block] = 0
		materials[block] = load(blocks_data[block]["material"])
		var multimeshinstance = MultiMeshInstance3D.new()
		var multimesh = MultiMesh.new()
		var mesh
		if blocks_data[block]["type"] == Values.macros["BLOCK_SOLID"]:
			mesh = BoxMesh.new()
			mesh.set_size(Vector3.ONE)
		elif blocks_data[block]["type"] == Values.macros["BLOCK_LIQUID"]:
			mesh = PlaneMesh.new()
			mesh.orientation = PlaneMesh.FACE_Y
			mesh.set_size(Vector2(1, 1))
		elif blocks_data[block]["type"] == Values.macros["BLOCK_SLOPE"]:	
			mesh = PrismMesh.new()
			mesh.left_to_right = 0
		elif blocks_data[block]["type"] == Values.macros["BLOCK_STAIRS"]:
			mesh = ArrayMesh.new()
			var vertices = PackedVector3Array(
					[
						# front bottom
						Vector3(-0.5, 0, 0), Vector3(0.5, 0, 0),#0-1
						Vector3(-0.5, 0, 0.25), Vector3(0.5, 0, 0.25),#2-3
						Vector3(-0.5, -0.25, 0.25), Vector3(0.5, -0.25, 0.25),#4-5
						Vector3(-0.5, -0.25, 0.5), Vector3(0.5, -0.25, 0.5),#6-7
						
						# x3 for UV
						Vector3(-0.5, -0.5, 0.5), Vector3(0.5, -0.5, 0.5),#8-9
						Vector3(-0.5, -0.5, 0.5), Vector3(0.5, -0.5, 0.5),#10-11
						Vector3(-0.5, -0.5, 0.5), Vector3(0.5, -0.5, 0.5),#12-13
						
						# back
						# x3 for UV
						Vector3(-0.5, -0.5, -0.5), Vector3(0.5, -0.5, -0.5),#14-15
						Vector3(-0.5, -0.5, -0.5), Vector3(0.5, -0.5, -0.5),#16-17
						Vector3(-0.5, -0.5, -0.5), Vector3(0.5, -0.5, -0.5),#18-19
						
						# x2 for UV
						Vector3(-0.5, 0.5, -0.5), Vector3(0.5, 0.5, -0.5),#20-21
						Vector3(-0.5, 0.5, -0.5), Vector3(0.5, 0.5, -0.5),#22-23
						
						#front up
						Vector3(-0.5, 0.5, -0.25), Vector3(0.5, 0.5, -0.25),#24-25
						Vector3(-0.5, 0.25, -0.25), Vector3(0.5, 0.25, -0.25),#26-27
						Vector3(-0.5, 0.25, 0), Vector3(0.5, 0.25, 0),#28-29
						Vector3(-0.5, 0, 0), Vector3(0.5, 0, 0),#30-31
						
						#side right
						Vector3(-0.5, -0.25, -0.5), Vector3(-0.5, -0.25, 0.25), Vector3(-0.5, -0.25, 0.5), #32-33-34
						Vector3(-0.5, 0, -0.5), Vector3(-0.5, 0, 0), Vector3(-0.5, 0, 0.25), #35-36-37
						Vector3(-0.5, 0.25, -0.5), Vector3(-0.5, 0.25, -0.25), Vector3(-0.5, 0.25, 0), #38-39-40
						Vector3(-0.5, 0.5, -0.5), Vector3(-0.5, 0.5, -0.25), #41-42
						
						#side left
						Vector3(0.5, -0.25, -0.5), Vector3(0.5, -0.25, 0.25), Vector3(0.5, -0.25, 0.5), #43-44-45
						Vector3(0.5, 0, -0.5), Vector3(0.5, 0, 0), Vector3(0.5, 0, 0.25), #46-47-48
						Vector3(0.5, 0.25, -0.5), Vector3(0.5, 0.25, -0.25), Vector3(0.5, 0.25, 0), #49-50-51
						Vector3(0.5, 0.5, -0.5), Vector3(0.5, 0.5, -0.25), #52-53
					]
			)
			var indexes = PackedInt32Array(
					[
						# front bottom
						0,1,2, 1,3,2,
						2,3,4, 3,5,4,
						4,5,6, 5,7,6,
						6,7,8, 7,9,8,
						
						# bottom
						10,11,14, 11,15,14,
						
						# back
						16,17,20, 17,21,20,
						
						# front top
						22,23,24, 23,25,24,
						24,25,26, 25,27,26,
						26,27,28, 27,29,28,
						28,29,30, 29,31,30,
						
						# side right
						18,32,12, 12,32,34,
						32,35,33, 33,35,37,
						35,38,36, 36,38,40,
						38,41,39, 39,41,42,
						
						# side left
						19,13,43, 13,45,43,
						43,44,46, 44,48,46,
						46,47,49, 47,51,49,
						49,50,52, 50,53,52,
					]
			)
			# our FRONT is x axis instead of z, so FRONT is LEFT
			var normals = PackedVector3Array(
					[
						# front bottom
						(Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(), (Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(),#0-1
						(Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(), (Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(),#2-3
						(Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(), (Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(),#4-5
						(Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(), (Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(),#6-7
						
						# x3 for UV
						Vector3.MODEL_FRONT, Vector3.MODEL_FRONT,#8-9
						Vector3.MODEL_BOTTOM, Vector3.MODEL_BOTTOM,#10-11
						Vector3.MODEL_RIGHT, Vector3.MODEL_LEFT,#12-13
						
						# back
						# x3 for UV
						Vector3.MODEL_BOTTOM, Vector3.MODEL_BOTTOM,#14-15
						Vector3.MODEL_REAR, Vector3.MODEL_REAR,#16-17
						Vector3.MODEL_RIGHT, Vector3.MODEL_LEFT,#18-19
						
						# x2 for UV
						Vector3.MODEL_REAR, Vector3.MODEL_REAR,#20-21
						(Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(), (Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(),#22-23
						
						#front up
						(Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(), (Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(),#24-25
						(Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(), (Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(),#26-27
						(Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(), (Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(),#28-29
						(Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(), (Vector3.MODEL_FRONT + Vector3.MODEL_TOP).normalized(),#30-31
						
						#side right
						Vector3.MODEL_RIGHT, Vector3.MODEL_RIGHT, (Vector3.MODEL_RIGHT + Vector3.MODEL_TOP).normalized(), #32-33-34
						Vector3.MODEL_RIGHT, Vector3.MODEL_RIGHT, (Vector3.MODEL_RIGHT + Vector3.MODEL_TOP).normalized(), #35-36-37
						Vector3.MODEL_RIGHT, Vector3.MODEL_RIGHT, (Vector3.MODEL_RIGHT + Vector3.MODEL_TOP).normalized(), #38-39-40
						Vector3.MODEL_RIGHT, (Vector3.MODEL_RIGHT + Vector3.MODEL_TOP).normalized(), #41-42
						
						#side left
						Vector3.MODEL_LEFT, Vector3.MODEL_LEFT, (Vector3.MODEL_LEFT + Vector3.MODEL_TOP).normalized(), #43-44-45
						Vector3.MODEL_LEFT, Vector3.MODEL_LEFT, (Vector3.MODEL_LEFT + Vector3.MODEL_TOP).normalized(), #46-47-48
						Vector3.MODEL_LEFT, Vector3.MODEL_LEFT, (Vector3.MODEL_LEFT + Vector3.MODEL_TOP).normalized(), #49-50-51
						Vector3.MODEL_LEFT, (Vector3.MODEL_LEFT + Vector3.MODEL_TOP).normalized(), #52-53
					]
			)
			var uvs = PackedVector2Array(
					[
						# front bottom
						Vector2(0, 0), Vector2(1.0 / 3.0, 0),#0-1
						Vector2(0, 0.125), Vector2(1.0 / 3.0, 0.125),#2-3
						Vector2(0, 0.25), Vector2(1.0 / 3.0, 0.25),#4-5
						Vector2(0, 0.375), Vector2(1.0 / 3.0, 0.375),#6-7
						
						# x3 for UV
						Vector2(0, 0.5), Vector2(1.0 / 3.0, 0.5),#8-9
						Vector2(0, 0), Vector2(1.0 / 3.0, 0),#10-11
						Vector2(1.0 / 3.0, 0.5), Vector2(1.0 / 3.0, 0.5),#12-13
						
						# back
						# x3 for UV
						Vector2(0, 0.5), Vector2(1.0 / 3.0, 0.5),#14-15
						Vector2(0, 0.5), Vector2(1.0 / 3.0, 0.5),#16-17
						Vector2(0, 0.5), Vector2(0, 0.5),#18-19
						
						# x2 for UV
						Vector2(0, 0), Vector2(1.0 / 3.0, 0),#20-21
						Vector2(0, 0), Vector2(1.0 / 3.0, 0),#22-23
						
						#front up
						Vector2(0, 0.125), Vector2(1.0 / 3.0, 0.125),#24-25
						Vector2(0, 0.25), Vector2(1.0 / 3.0, 0.25),#26-27
						Vector2(0, 0.375), Vector2(1.0 / 3.0, 0.375),#28-29
						Vector2(0, 0.5), Vector2(1.0 / 3.0, 0.5),#30-31
						
						#side left
						Vector2(0, 0.375), Vector2(0.75 / 3.0, 0.375), Vector2(1.0 / 3.0, 0.375), #32-33-34
						Vector2(0, 0.25), Vector2(0.5 / 3.0, 0.25), Vector2(0.75 / 3.0, 0.25), #35-36-37
						Vector2(0, 0.125), Vector2(0.25 / 3.0, 0.125), Vector2(0.5 / 3.0, 0.125), #38-39-40
						Vector2(0, 0), Vector2(0.25 / 3.0, 0), #41-42
						
						#side right
						Vector2(0, 0.375), Vector2(0.75 / 3.0, 0.375), Vector2(1.0 / 3.0, 0.375), #43-44-45
						Vector2(0, 0.25), Vector2(0.5 / 3.0, 0.25), Vector2(0.75 / 3.0, 0.25), #46-47-48
						Vector2(0, 0.125), Vector2(0.25 / 3.0, 0.125), Vector2(0.5 / 3.0, 0.125), #49-50-51
						Vector2(0, 0), Vector2(0.25 / 3.0, 0), #52-53
					]
			)
			var tool = SurfaceTool.new()
			tool.begin(Mesh.PRIMITIVE_TRIANGLES)
			for i in range(vertices.size()):
				tool.set_uv(uvs[i])
				tool.set_normal(normals[i])
				tool.add_vertex(vertices[i])
			for index in indexes:
				tool.add_index(index)
			#tool.generate_normals()
			tool.generate_tangents()
			mesh = tool.commit()
		var material = materials[block]
		if blocks_data[block]["type"] == Values.macros["BLOCK_SOLID"] or blocks_data[block]["type"] == Values.macros["BLOCK_STAIRS"]:
			material.uv1_scale = Vector3(3.0, 2.0, 1.0)
		mesh.surface_set_material(0, material)
		multimesh.set_mesh(mesh)
		multimesh.transform_format = MultiMesh.TRANSFORM_3D
		multimeshinstance.set_multimesh(multimesh)
		multimeshinstance.name = block
		multimeshes[block] = multimeshinstance
		$Blocks.add_child(multimeshinstance)

func add_furniture(furniture_data: Dictionary, coord: Vector3):
	if not Values.base_furnitures_models.has(furniture_data["name"]):
		var model = load(furniture_data["path"])
		Values.base_furnitures_models[furniture_data["name"]] = model
		if furniture_data.has("path_off"):
			if FileAccess.file_exists(furniture_data["path_off"]):
				Values.base_furnitures_models_off[furniture_data["name"]] = load(furniture_data["path_off"])
			else:
				Values.base_furnitures_models_off[furniture_data["name"]] = model
	var furniture
	if furniture_data["isOn"]:
		furniture = Values.base_furnitures_models[furniture_data["name"]].instantiate()
	else:
		furniture = Values.base_furnitures_models_off[furniture_data["name"]].instantiate()
	
	if furniture_data["light"] > 0:
		furniture.add_child(add_fire(furniture_data["fire_pos"], furniture_data["fire_size"], furniture_data["light"]))
	furniture.transform.origin = Vector3(coord.x + float(furniture_data["size"].y) / 2.0, coord.y + float(furniture_data["size"].z) / 2.0, coord.z + float(furniture_data["size"].x) / 2.0)
	furniture.rotation_degrees = Vector3(0, furniture_data["orientation_z"], 0)
	var collider = StaticBody3D.new()
	
	if furniture_data["solid"]:
		collider.collision_layer = 0x19
	else:
		collider.collision_layer = 0x9
	collider.collision_mask = 0x0
	var shape = CollisionShape3D.new()
	shape.name = "Shape"
	shape.shape = BoxShape3D.new()
	shape.shape.set_size(Vector3(furniture_data["size"].y, furniture_data["size"].z, furniture_data["size"].x))
	collider.transform.origin = furniture.transform.origin
	collider.add_child(shape)
	if furnitures.has(coord): # this is an update, remove previous furniture
		$Furnitures.remove_child(furnitures[coord])
		$Colliders.remove_child(colliders[coord])
	furnitures[coord] = furniture
	colliders[coord] = collider
	collider.set_meta("coord", coord)
	collider.set_meta("type", "FURNITURE")
	collider.set_meta("data", furniture_data)
	$Colliders.add_child(collider)
	$Furnitures.add_child(furniture)

func add_fire(coord: Vector3, size: float, light_power: int):
	var fire = base_fire.instantiate()
	fire.initialize(coord, size, light_power)
	return fire
