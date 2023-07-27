extends SubViewport

func change_shader(shader):
	$Tube/Content.set_surface_override_material(0, shader)
	
func change_amount(value: float):
	$Tube/Content.set_instance_shader_parameter("visibilityYLimit", value)
