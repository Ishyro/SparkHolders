extends Node

# Constants
const BLOCK_SOLID = 1
const BLOCK_LIQUID = 2
const BLOCK_GAS = 3
const BLOCK_STAIRS = 4
const BLOCK_SLOPE = 5

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
const DAMAGE_SOLAR = 9
const DAMAGE_AETHER = 10

const ACTION_MODE_NONE = 0
const ACTION_MODE_MOVE = 1
const ACTION_MODE_ATTACK = 2
const ACTION_MODE_ACTIVATION = 3
const ACTION_MODE_WAIT = 4
const ACTION_MODE_SPEAK = 5
const ACTION_MODE_SLEEP = 6
const ACTION_MODE_SKILL = 7

const ACTION_IDLE = 0
const ACTION_MOVE = 1
const ACTION_STRIKE = 2
const ACTION_HEAVY_STRIKE = 3
const ACTION_ACTIVATION = 4
const ACTION_RESPITE = 5
const ACTION_SHOOT = 6
const ACTION_RELOAD = 7
const ACTION_GRAB = 8
const ACTION_SWAP_GEAR = 9
const ACTION_USE_ITEM = 10
const ACTION_USE_SKILL = 11
const ACTION_REST = 12
const ACTION_TALKING = 13
const ACTION_ECONOMICS = 14
const ACTION_BREAKPOINT = 15

const TARGET_NONE = 0
const TARGET_SELF = 1
const TARGET_CHARACTER = 2
const TARGET_COORDINATES = 3
const TARGET_BLOCK = 4
const TARGET_PERMISSIVE = 5
const TARGET_MULTIPLE = 6

# Variables
var server_pid = -1
var link = GodotLink.new()
var updating_state = false
var next_state_ready = false
var mode = ACTION_MODE_NONE

var coord = Vector3.ZERO
var selection_changed = false
var selected_team = null
var selected_target = null
var selected_projectile = null
var selected_block = null

var actions = {}
var action_muxtex = Mutex.new()
