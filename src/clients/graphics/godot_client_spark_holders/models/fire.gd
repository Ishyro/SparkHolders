extends Node3D

func initialize(coords: Vector3, size: float, light_power: int):
	var flames = $Flames
	var smoke = $Smoke
	var light = $Light
	transform.origin = coords
	flames.lifetime *= size
	flames.draw_pass_1 = QuadMesh.new()
	flames.draw_pass_1.size = Vector2(size, size)
	flames.process_material.emission_sphere_radius *= size 
	smoke.lifetime *= size
	smoke.draw_pass_1 = QuadMesh.new()
	smoke.draw_pass_1.size = Vector2(size, size)
	smoke.process_material.emission_sphere_radius *= size
	light.omni_range = light_power
	light.light_energy *= light_power
	if Settings.white_light:
		light.light_color = Color.WHITE
	if not Settings.shadows:
		light.set_shadow(false)
