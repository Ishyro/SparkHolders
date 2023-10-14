shader_type sky;

uniform vec3 skyColor : source_color;
uniform vec3 horizonColor : source_color;

uniform sampler2D cloud_texture;
uniform sampler2D cloud_texture2;
uniform vec2 cloud_direction = vec2(0.002, 0.0);
uniform vec2 cloud_direction2 = vec2(0.0, 0.001);
uniform vec2 cloud_scale;
uniform float cloud_distance;

void sky() {
	vec2 sky_uv = EYEDIR.xz / EYEDIR.y;
	float col = clamp(abs(EYEDIR.y) / 0.05, 0.0, 1.0);
	vec3 finalColor = mix(horizonColor, skyColor, col);
	vec3 clouds = texture(cloud_texture, sky_uv * cloud_scale + TIME * cloud_direction).rgb - texture(cloud_texture2, sky_uv * cloud_scale + TIME * cloud_direction2).rgb;
	if(EYEDIR.y <= 0.) {
		COLOR = finalColor * LIGHT0_ENERGY;
	}
	else {
		COLOR = mix(finalColor * LIGHT0_ENERGY, clouds, 0.1);
	}
}
