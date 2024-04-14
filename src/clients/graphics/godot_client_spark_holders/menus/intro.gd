extends Control

# Called when the node enters the scene tree for the first time.
func _ready():
	load_settings()
	$AnimationPlayer.play("Fade in")
	await get_tree().create_timer(6).timeout
	$AnimationPlayer.play("Fade out")
	await get_tree().create_timer(3).timeout
	get_tree().change_scene_to_file("res://menus/menu.tscn")
	
func load_settings():
	var settings = FileAccess.open("res://data/settings_client.data", FileAccess.READ)
	var content = settings.get_as_text(true)
	var settings_dic = {}
	for setting in content.split("\n"):
		if not setting.is_empty() and setting[0] != "#":
			var key_and_data = setting.split("=")
			settings_dic[key_and_data[0].trim_prefix(" ").trim_suffix(" ")] = key_and_data[1].trim_prefix(" ").trim_suffix(" ")
	set_resolution(settings_dic["RESOLUTION"], settings_dic["WINDOW_TYPE"])
	settings.close()

func set_resolution(resolution_str: String, mode: String):
	var key_and_data = resolution_str.split("x")
	var resolution = Vector2(int(key_and_data[0]), int(key_and_data[1]))
	if mode == "FULLSCREEN":
		get_window().set_size(resolution)
		get_window().set_mode(Window.MODE_FULLSCREEN)
		get_window().set_size(resolution)
	elif mode == "WINDOWED":
		get_window().set_mode(Window.MODE_WINDOWED)
		get_window().set_flag(Window.FLAG_BORDERLESS, false)
		get_window().set_flag(Window.FLAG_RESIZE_DISABLED, true)
		get_window().set_size(resolution)
	elif mode == "WINDOW_BORDERLESS":
		get_window().set_mode(Window.MODE_WINDOWED)
		get_window().set_flag(Window.FLAG_BORDERLESS, true)
		get_window().set_flag(Window.FLAG_RESIZE_DISABLED, true)
		get_window().set_size(resolution)
	Values.CURRENT_RESOLUTION = resolution
