extends Node3D

var levels = {}
var base_level = preload("res://world/level.tscn")

func add_map(map_name: String, pos: Vector3, size: Vector3):
	if levels.has(pos.y):
		levels[pos.y].add_map(map_name, pos, size)
	else:
		var level = base_level.instantiate()
		level.set_name("level" + str(pos.y))
		level.add_map(map_name, pos, size)
		levels[pos.y] = level
