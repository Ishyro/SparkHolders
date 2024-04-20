extends Button

func _on_mouse_entered():
	grab_focus.call_deferred()
