extends CharacterBody3D

var character
var id

@onready var nav = $NavigationAgent3D

func set_color(color: String):
	var material = StandardMaterial3D.new()
	material.albedo_color = color
	$Color.set_surface_override_material(0, material)
	$Selection.set_surface_override_material(0, material)
	
func select():
	$Selection.visible = true
	
func unselect():
	$Selection.visible = false
