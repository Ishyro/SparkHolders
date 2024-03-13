shader_type spatial;

uniform vec3 albedo : source_color;
uniform vec3 albedo2 : source_color;
uniform float metallic : hint_range(0.0, 1.0) = 0;
uniform float roughness : hint_range(0.0, 1.0) = 0.2;
uniform sampler2D wave;
uniform sampler2D texture_normal;
uniform sampler2D texture_normal2;
uniform vec2 wave_direction = vec2(2.0, 0.0);
uniform vec2 wave_direction2 = vec2(0.0, 1.0);
uniform float time_scale : hint_range(0.0, 10.0, 0.25) = 2.0;
uniform float uv_scale : hint_range(0.0, 10.0, 0.25) = 1.0;
uniform float noise_scale = 10.0;
uniform float height_scale = 0.15;
uniform sampler2D screen_texture : hint_screen_texture, repeat_disable;

varying float height;
varying vec3 world_pos;

float fresnel(float amount, vec3 normal, vec3 view) {
	return pow((1.0 - clamp(dot(normalize(normal), normalize(view)), 0.0, 1.0)), amount);
}

vec3 fresnel_glow(float amount, float intensity, vec3 color, vec3 normal, vec3 view) {
	return pow((1.0 - dot(normalize(normal), normalize(view))), amount) * color * intensity;
}
/*
void vertex() {
	world_pos = (MODEL_MATRIX * vec4(VERTEX, 1.0)).xyz;
	height = texture(wave, world_pos.xz / noise_scale + TIME * time_scale).r;
	//height = sin(TIME * time_scale * 25.0 + world_pos.x) - sin(TIME * time_scale * 25.0 + world_pos.z);;
	VERTEX.y += height * height_scale;
}
*/

void fragment() {
	vec2 _uv = uv_scale * UV;
	vec2 _suv = UV;
	
	_uv.x += sin(TIME * time_scale + (_uv.x + _uv.y) * 25.0) * 0.01;
	_uv.y += cos(TIME * time_scale + (_uv.x - _uv.y) * 25.0) * 0.01;
	
	_suv.x += sin(TIME * time_scale + (_suv.x + _suv.y) * 25.0) * 0.01;
	_suv.y += cos(TIME * time_scale + (_suv.x - _suv.y) * 25.0) * 0.01;
	
	float fresnel = fresnel(5.0, NORMAL, VIEW);
	vec3 surface_color = mix(albedo, albedo2, fresnel);

	ALBEDO = surface_color; //mix(texture(screen_texture, _suv).rgb, surface_color, 0.1);
	NORMAL_MAP = mix(texture(texture_normal, _uv).rgb, texture(texture_normal2, _uv).rgb, 0.5);
	NORMAL *= 0.5;
	METALLIC = metallic;
	ROUGHNESS = roughness;
	/*
	vec2 time = (TIME * wave_direction) * time_scale;
	vec2 time2 = (TIME * wave_direction2) * time_scale;
	
	vec3 normal_blend = mix(texture(texture_normal, world_pos.xz + time).rgb, texture(texture_normal2, world_pos.xz + time2).rgb, 0.5);
	
	float fresnel = fresnel(5.0, NORMAL, VIEW);
	vec3 fresnel_color = fresnel_glow(5.0, 5.0, albedo2, NORMAL, VIEW);
	vec3 surface_color = mix(albedo, albedo2, fresnel);
	//surface_color = albedo + fresnel_color;
	
	ALBEDO = surface_color;
	METALLIC = metallic;
	ROUGHNESS = roughness;
	NORMAL_MAP = normal_blend;
	*/
}
