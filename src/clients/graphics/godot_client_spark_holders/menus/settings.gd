extends Control

var small_numbers = preload("res://menus/hud/police/SmallNumbers.tres")
var numbers = preload("res://menus/hud/police/Numbers.tres")
var numbers_box = preload("res://menus/hud/police/NumbersBox.tres")
var numbers_white = preload("res://menus/hud/police/NumbersWhite.tres")
var small_text = preload("res://menus/hud/police/SmallText.tres")
var text = preload("res://menus/hud/police/Text.tres")
var big_text = preload("res://menus/hud/police/BigText.tres")

# Called when the node enters the scene tree for the first time.
func _ready():
	$Menu/Back.grab_focus()

func _on_back_pressed():
	get_tree().change_scene_to_file("res://menus/main_menu.tscn")
	
func set_resolution(resolution: Vector2, mode: String):
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

func _on_resolution_pressed():
	set_resolution(Vector2(1280, 720), "WINDOWED")
