extends Node3D

func initialize(coords: Vector3, size: float, light_power: int):
	transform.origin = coords
	$Flames.lifetime *= size
	$Flames.draw_pass_1.size = Vector2(size, size)
	$Flames.process_material.emission_sphere_radius *= size 
	$Smoke.lifetime *= size
	$Smoke.draw_pass_1.size = Vector2(size, size)
	$Smoke.process_material.emission_sphere_radius *= size
	$ParticlesFloating.amount *= size
	$ParticlesFloating.lifetime *= size
	$ParticlesFloating.draw_pass_1.size = Vector2(size, size)
	$ParticlesFloating.process_material.emission_sphere_radius *= size
	$Light.omni_range = light_power
	$Light.light_energy *= light_power
	#$Light.light_color = Color.WHITE
