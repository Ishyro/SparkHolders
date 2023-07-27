extends Node


# Constants
const DAMAGE_TYPE_NUMBER = 11
const DAMAGE_SLASH = 0
const DAMAGE_PUNCTURE = 1
const DAMAGE_BLUNT = 2
const DAMAGE_FIRE = 3
const DAMAGE_LIGHTNING = 4
const DAMAGE_FROST = 5
const DAMAGE_POISON = 6
const DAMAGE_ACID = 7
const DAMAGE_MIND = 8
const DAMAGE_TRUE = 9
const DAMAGE_SOUL = 10

const ACTION_NONE = 0
const ACTION_MOVE = 1
const ACTION_SKILL = 2

# Variables
var server_pid = -1
var link = GodotLink.new()
var mode = ACTION_MOVE

var coord = Vector3.ZERO
var selection_changed = false
var selected_team = null
var selected_target = null
var selected_projectile = null
var selected_tile = null
