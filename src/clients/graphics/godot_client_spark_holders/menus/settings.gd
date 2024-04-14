extends Control

var small_numbers = preload("res://menus/hud/police/SmallNumbers.tres")
var numbers = preload("res://menus/hud/police/Numbers.tres")
var numbers_box = preload("res://menus/hud/police/NumbersBox.tres")
var numbers_white = preload("res://menus/hud/police/NumbersWhite.tres")
var small_text = preload("res://menus/hud/police/SmallText.tres")
var text = preload("res://menus/hud/police/Text.tres")
var big_text = preload("res://menus/hud/police/BigText.tres")

var client_settings_dic = {}
var server_settings_dic = {}

# Called when the node enters the scene tree for the first time.
func _ready():
	$Menu/Close.grab_focus()
	load_client_settings()
	load_server_settings()

func load_client_settings():
	var settings = FileAccess.open("res://data/settings_client.data", FileAccess.READ)
	var content = settings.get_as_text(true)
	for setting in content.split("\n"):
		if not setting.is_empty() and setting[0] != "#":
			var key_and_data = setting.split("=")
			client_settings_dic[key_and_data[0].trim_prefix(" ").trim_suffix(" ")] = key_and_data[1].trim_prefix(" ").trim_suffix(" ")
	settings.close()

func save_client_settings():
	var settings = FileAccess.open("res://data/settings_client.data", FileAccess.READ)
	var content = settings.get_as_text(true)
	settings.close()
	settings = FileAccess.open("res://data/settings_client.data", FileAccess.WRITE)
	for setting in content.split("\n"):
		if not setting.is_empty() and setting[0] != "#":
			var key_and_data = setting.split("=")
			var setting_key = key_and_data[0].trim_prefix(" ").trim_suffix(" ")
			settings.store_line(setting_key + " = " + client_settings_dic[setting_key])
		else:
			settings.store_line(setting)
	settings.close()

func load_server_settings():
	var settings = FileAccess.open("res://data/settings_server.data", FileAccess.READ)
	var content = settings.get_as_text(true)
	for setting in content.split("\n"):
		if not setting.is_empty() and setting[0] != "#":
			var key_and_data = setting.split("=")
			server_settings_dic[key_and_data[0].trim_prefix(" ").trim_suffix(" ")] = key_and_data[1].trim_prefix(" ").trim_suffix(" ")
	settings.close()

func save_server_settings():
	var settings = FileAccess.open("res://data/settings_server.data", FileAccess.READ)
	var content = settings.get_as_text(true)
	settings.close()
	settings = FileAccess.open("res://data/settings_server.data", FileAccess.WRITE)
	for setting in content.split("\n"):
		if not setting.is_empty() and setting[0] != "#":
			var key_and_data = setting.split("=")
			var setting_key = key_and_data[0].trim_prefix(" ").trim_suffix(" ")
			settings.store_line(setting_key + " = " + server_settings_dic[setting_key])
		else:
			settings.store_line(setting)
	settings.close()

func set_resolution():
	var resolution_str = client_settings_dic["RESOLUTION"]
	var mode = client_settings_dic["WINDOW_TYPE"]
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
	elif mode == "BORDERLESS":
		get_window().set_mode(Window.MODE_WINDOWED)
		get_window().set_flag(Window.FLAG_BORDERLESS, true)
		get_window().set_flag(Window.FLAG_RESIZE_DISABLED, true)
		get_window().set_size(resolution)
	#numbers_box.font_size = 18 * resolution.y / Values.BASE_RESOLUTION.y
	#numbers_white.font_size = 36 * resolution.y / Values.BASE_RESOLUTION.y
	#small_numbers.font_size = 32 * resolution.y / Values.BASE_RESOLUTION.y
	#numbers.font_size = 48 * resolution.y / Values.BASE_RESOLUTION.y
	#small_text.font_size = 16 * resolution.y / Values.BASE_RESOLUTION.y
	#text.font_size = 28 * resolution.y / Values.BASE_RESOLUTION.y
	#big_text.font_size = 36 * resolution.y / Values.BASE_RESOLUTION.y
	Values.CURRENT_RESOLUTION = resolution

func _on_fullscreen_pressed():
	client_settings_dic["WINDOW_TYPE"] = "FULLSCREEN"

func _on_windowed_pressed():
	client_settings_dic["WINDOW_TYPE"] = "WINDOWED"

func _on_borderless_window_pressed():
	client_settings_dic["WINDOW_TYPE"] = "WINDOW_BORDERLESS"

func _on_close_pressed():
	visible = false

func _on_save_pressed():
	save_client_settings()
	save_server_settings()
