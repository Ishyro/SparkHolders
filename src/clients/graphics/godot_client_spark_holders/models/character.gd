extends CharacterBody3D

var character
var id

@onready var nav = $NavigationAgent3D
@onready var range_mesh = $AttackRange

func set_color(color: String):
	var material = StandardMaterial3D.new()
	material.albedo_color = color
	$Color.set_surface_override_material(0, material)
	$Selection.set_surface_override_material(0, material)

func set_attack_range(weapon_range: float):
	$AttackRange.scale_object_local(Vector3(weapon_range, weapon_range, weapon_range))

func select():
	$Selection.visible = true
	
func unselect():
	$Selection.visible = false
