extends Control

var client_settings_dic = {}
var server_settings_dic = {}
var client_settings_changed = {}
var server_settings_changed = {}

# General
var languages = { "English": 0, "Français": 1 }
var fonts = { "Emperialisme": 0, "Endor": 1, "Endor Alt": 2, "New Rocker": 3, "Rooters": 4 }
var fonts_paths = { 
	"Emperialisme": "res://menus/hud/police/emperialisme/Emperialisme Regular.otf",
	"Endor": "res://menus/hud/police/endor/ENDOR___.ttf",
	"Endor Alt": "res://menus/hud/police/endor/ENDORALT.ttf",
	"New Rocker": "res://menus/hud/police/new-rocker/NewRocker-Regular.ttf",
	"Rooters": "res://menus/hud/police/rooters/Rooters.otf"
}
# Video
var window_types = { "Fullscreen": 0, "Windowed": 1, "Borderless Window": 2 }
var resolutions = { "1280x720": 0, "1280x1080": 1, "1440x1080": 2, "1920x1080": 3, "2880x1920": 4, "3840x2160": 5, "7680x4320": 6 }
var antialiasings = { "Off": 0, "x2": 1, "x4": 2, "x8": 3 }

# General
@onready var n_language = $Divider/MarginContainer/ClientServer/Client/General/LanguageValue
@onready var n_font = $Divider/MarginContainer/ClientServer/Client/General/FontValue
@onready var n_port = $Divider/MarginContainer/ClientServer/Client/General/PortValue
@onready var n_terranday = $Divider/MarginContainer/ClientServer/Client/General/TerranDayValue
# Video
@onready var n_resolution = $Divider/MarginContainer/ClientServer/Client/Video/ResolutionValue
@onready var n_window_type = $Divider/MarginContainer/ClientServer/Client/Video/WindowTypeValue
@onready var n_antialiasing = $Divider/MarginContainer/ClientServer/Client/Video/AntiAliasingValue
@onready var n_vsync = $Divider/MarginContainer/ClientServer/Client/Video/VSyncValue
@onready var n_shadows = $Divider/MarginContainer/ClientServer/Client/Video/ShadowsValue
@onready var n_whitelight = $Divider/MarginContainer/ClientServer/Client/Video/WhileLightValue

# Server
@onready var n_tickduration = $Divider/MarginContainer/ClientServer/Server/Gameplay/TickDurationValue
@onready var n_buyingpricemodifier = $Divider/MarginContainer/ClientServer/Server/Gameplay/BuyingPriceModifierValue

@onready var n_serverport = $Divider/MarginContainer/ClientServer/Server/General/PortValue
@onready var n_seed = $Divider/MarginContainer/ClientServer/Server/General/SeedValue
@onready var n_pasword = $Divider/MarginContainer/ClientServer/Server/General/PasswordValue

func _ready():
	# General
	for language in languages:
		n_language.add_item(language)
	for font in fonts:
		n_font.add_item(font)
	# Video
	for resolution in resolutions:
		n_resolution.add_item(resolution)
	for window_type in window_types:
		n_window_type.add_item(window_type)
	for antialiasing in antialiasings:
		n_antialiasing.add_item(antialiasing)
	
	$Divider/Buttons/Save.grab_focus.call_deferred()
	load_client_settings()
	load_server_settings()
	apply_client_settings(true)
	apply_server_settings()

func load_client_settings():
	var settings = FileAccess.open("res://data/settings_client.data", FileAccess.READ)
	var content = settings.get_as_text(true).strip_edges(false)
	for setting in content.split("\n"):
		if not setting.is_empty() and setting[0] != "#":
			var key_and_data = setting.split("=")
			client_settings_dic[key_and_data[0].strip_edges()] = key_and_data[1].strip_edges()
			client_settings_changed[key_and_data[0].strip_edges()] = false
	settings.close()

func save_client_settings():
	var settings = FileAccess.open("res://data/settings_client.data", FileAccess.READ)
	var content = settings.get_as_text(true).strip_edges(false)
	settings.close()
	settings = FileAccess.open("res://data/settings_client.data", FileAccess.WRITE)
	for setting in content.split("\n"):
		if not setting.is_empty() and setting[0] != "#":
			var key_and_data = setting.split("=")
			var setting_key = key_and_data[0].strip_edges()
			settings.store_line(setting_key + " = " + client_settings_dic[setting_key])
		else:
			settings.store_line(setting)
	settings.close()

func load_server_settings():
	var settings = FileAccess.open("res://data/settings_server.data", FileAccess.READ)
	var content = settings.get_as_text(true).strip_edges(false)
	for setting in content.split("\n"):
		if not setting.is_empty() and setting[0] != "#":
			var key_and_data = setting.split("=")
			server_settings_dic[key_and_data[0].strip_edges()] = key_and_data[1].strip_edges()
			server_settings_changed[key_and_data[0].strip_edges()] = false
	settings.close()

func save_server_settings():
	var settings = FileAccess.open("res://data/settings_server.data", FileAccess.READ)
	var content = settings.get_as_text(true).strip_edges(false)
	settings.close()
	settings = FileAccess.open("res://data/settings_server.data", FileAccess.WRITE)
	for setting in content.split("\n"):
		if not setting.is_empty() and setting[0] != "#":
			var key_and_data = setting.split("=")
			var setting_key = key_and_data[0].strip_edges()
			settings.store_line(setting_key + " = " + server_settings_dic[setting_key])
		else:
			settings.store_line(setting)
	settings.close()
	
func apply_client_settings(forced_update):
	var need_to_save = false
	var keys = [ "FONT", "ANTI_ALIASING" ]
	for key in keys:
		if client_settings_changed[key]:
			need_to_save = true
			break
	# General
	if client_settings_changed["LANG"] or forced_update:
		n_language.select(languages[client_settings_dic["LANG"]])
		Settings.Lang = client_settings_dic["LANG"]
		client_settings_changed["LANG"] = false
	if client_settings_changed["FONT"] or forced_update:
		n_font.select(fonts[client_settings_dic["FONT"]])
		ProjectSettings.set_setting("gui/theme/custom_font", fonts_paths[client_settings_dic["FONT"]])
		client_settings_changed["FONT"] = false
	if client_settings_changed["PORT"] or forced_update:
		n_port.text = client_settings_dic["PORT"]
		Settings.Port = int(client_settings_dic["PORT"])
		client_settings_changed["PORT"] = false
	if client_settings_changed["TERRAN_DAY"] or forced_update:
		if client_settings_dic["TERRAN_DAY"] == "On":
			n_terranday.set_pressed(true)
			Settings.TerranDay = true
		else:
			n_terranday.set_pressed(false)
			Settings.TerranDay = false
		client_settings_changed["EDGE_PANNING"] = false
	# Video
	if client_settings_changed["RESOLUTION"] or client_settings_changed["WINDOW_TYPE"] or forced_update:
		set_resolution()
		n_resolution.select(resolutions[client_settings_dic["RESOLUTION"]])
		n_window_type.select(window_types[client_settings_dic["WINDOW_TYPE"]])
		client_settings_changed["RESOLUTION"] = false
		client_settings_changed["WINDOW_TYPE"] = false
	# no forced update for ProjectSettings since they only apply on restart
	n_antialiasing.select(antialiasings[client_settings_dic["ANTI_ALIASING"]])
	if client_settings_changed["ANTI_ALIASING"]:
		match client_settings_dic["ANTI_ALIASING"]:
			"Off" : ProjectSettings.set_setting("rendering/anti_aliasing/quality/msaa_3d", 0)
			"x2" : ProjectSettings.set_setting("rendering/anti_aliasing/quality/msaa_3d", 1)
			"x4" : ProjectSettings.set_setting("rendering/anti_aliasing/quality/msaa_3d", 2)
			"x8" : ProjectSettings.set_setting("rendering/anti_aliasing/quality/msaa_3d", 3)
		client_settings_changed["ANTI_ALIASING"] = false
	if client_settings_changed["VSYNC"] or forced_update:
		if client_settings_dic["VSYNC"] == "On":
			n_vsync.set_pressed(true)
			DisplayServer.window_set_vsync_mode(DisplayServer.VSYNC_ENABLED)
		else:
			n_vsync.set_pressed(false)
			DisplayServer.window_set_vsync_mode(DisplayServer.VSYNC_DISABLED)
		client_settings_changed["VSYNC"] = false
	if client_settings_changed["SHADOWS"] or forced_update:
		if client_settings_dic["SHADOWS"] == "On":
			n_shadows.set_pressed(true)
			Settings.shadows = true
		else:
			n_shadows.set_pressed(false)
			Settings.shadows = false
		client_settings_changed["SHADOWS"] = false
	if client_settings_changed["WHITE_LIGHTS"] or forced_update:
		if client_settings_dic["WHITE_LIGHTS"] == "On":
			n_whitelight.set_pressed(true)
			Settings.white_light = true
		else:
			n_whitelight.set_pressed(false)
			Settings.white_light = false
		client_settings_changed["WHITE_LIGHTS"] = false
	if need_to_save:
		ProjectSettings.save()


func apply_server_settings():
	# Gameplay
	n_tickduration.text = server_settings_dic["TICK_DURATION"]
	n_buyingpricemodifier.text = server_settings_dic["BUYING_PRICE_MODIFIER"]
	# General
	n_serverport.text = server_settings_dic["PORT"]
	n_seed.text = server_settings_dic["SEED"]
	n_pasword.text = server_settings_dic["MASTER_PASSWORD"]
	Settings.password = server_settings_dic["MASTER_PASSWORD"]

func set_resolution():
	var resolution_str = client_settings_dic["RESOLUTION"]
	var mode = client_settings_dic["WINDOW_TYPE"]
	var key_and_data = resolution_str.split("x")
	var resolution = Vector2(int(key_and_data[0]), int(key_and_data[1]))
	if mode == "Fullscreen":
		get_window().set_mode(Window.MODE_FULLSCREEN)
	elif mode == "Windowed":
		get_window().set_mode(Window.MODE_WINDOWED)
		get_window().set_flag(Window.FLAG_BORDERLESS, false)
		get_window().set_flag(Window.FLAG_RESIZE_DISABLED, true)
	elif mode == "Borderless Window":
		get_window().set_mode(Window.MODE_WINDOWED)
		get_window().set_flag(Window.FLAG_BORDERLESS, true)
		get_window().set_flag(Window.FLAG_RESIZE_DISABLED, true)
	get_window().set_size(resolution)
	Settings.Resolution = resolution

func _on_save_pressed():
	save_client_settings()
	save_server_settings()
	apply_client_settings(false)

func _on_save_and_close_pressed():
	save_client_settings()
	save_server_settings()
	apply_client_settings(false)
	visible = false

func _on_close_pressed():
	visible = false

func _on_reset_pressed():
	pass # Replace with function body.

# General
func _on_language_value_item_selected(index):
	client_settings_dic["LANG"] = n_language.get_item_text(index)
	client_settings_changed["LANG"] = true

func _on_font_value_item_selected(index):
	client_settings_dic["FONT"] = n_font.get_item_text(index)
	client_settings_changed["FONT"] = true

func _on_port_value_text_submitted(new_text):
	if new_text.is_valid_int():
		client_settings_dic["PORT"] = new_text
	else:
		n_port.text = client_settings_dic["PORT"]
	client_settings_changed["PORT"] = true

func _on_terran_day_value_toggled(button_pressed):
	if button_pressed:
		n_terranday.set_text("On")
		client_settings_dic["TERRAN_DAY"] = "On"
	else:
		n_terranday.set_text("Off")
		client_settings_dic["TERRAN_DAY"] = "Off"
	client_settings_changed["TERRAN_DAY"] = true

# Video
func _on_resolution_value_item_selected(index):
	client_settings_dic["RESOLUTION"] = n_resolution.get_item_text(index)
	client_settings_changed["RESOLUTION"] = true
	
func _on_window_type_value_item_selected(index):
	client_settings_dic["WINDOW_TYPE"] = n_window_type.get_item_text(index)
	client_settings_changed["WINDOW_TYPE"] = true

func _on_anti_aliasing_value_item_selected(index):
	client_settings_dic["ANTI_ALIASING"] = n_antialiasing.get_item_text(index)
	client_settings_changed["ANTI_ALIASING"] = true

func _on_v_sync_value_toggled(button_pressed):
	if button_pressed:
		n_vsync.set_text("On")
		client_settings_dic["V_SYNC"] = "On"
	else:
		n_vsync.set_text("Off")
		client_settings_dic["V_SYNC"] = "Off"
	client_settings_changed["V_SYNC"] = true

func _on_shadows_value_toggled(button_pressed):
	if button_pressed:
		n_shadows.set_text("On")
		client_settings_dic["SHADOWS"] = "On"
	else:
		n_shadows.set_text("Off")
		client_settings_dic["SHADOWS"] = "Off"
	client_settings_changed["SHADOWS"] = true

func _on_while_light_value_toggled(button_pressed):
	if button_pressed:
		n_whitelight.set_text("On")
		client_settings_dic["WHITE_LIGHTS"] = "On"
	else:
		n_whitelight.set_text("Off")
		client_settings_dic["WHITE_LIGHTS"] = "Off"
	client_settings_changed["WHITE_LIGHTS"] = true

func _on_tick_duration_value_text_submitted(new_text):
	if new_text.is_valid_float():
		server_settings_dic["TICK_DURATION"] = new_text
	else:
		n_serverport.text = server_settings_dic["TICK_DURATION"]

func _on_buying_price_modifier_value_text_submitted(new_text):
	if new_text.is_valid_float():
		server_settings_dic["BUYING_PRICE_MODIFIER"] = new_text
	else:
		n_serverport.text = server_settings_dic["BUYING_PRICE_MODIFIER"]

func _on_serverport_value_text_submitted(new_text):
	if new_text.is_valid_int():
		server_settings_dic["PORT"] = new_text
	else:
		n_serverport.text = server_settings_dic["PORT"]

func _on_seed_value_text_submitted(new_text):
	if new_text.is_valid_int() or new_text == "rand":
		server_settings_dic["SEED"] = new_text
	else:
		n_seed.text = server_settings_dic["SEED"]

func _on_password_value_text_submitted(new_text):
	server_settings_dic["MASTER_PASSWORD"] = new_text


