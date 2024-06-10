enum bone : int {
	Head = 110,
	Neck = 67,
	Chest = 66,
	Pelvis = 2,
	LShoulder = 9,
	LElbow = 10,
	LHand = 11,
	RShoulder = 38,
	RElbow = 39,
	RHand = 40,
	LHip = 71,
	LKnee = 72,
	LFoot = 73,
	RHip = 78,
	RKnee = 79,
	RFoot = 82,
	Root = 0
};

enum EFortWeaponType : int
{
	Rifle = 0,
	Shotgun = 1,
	Smg = 2,
	Sniper = 3
};

enum offset {
	Uworld = 0x12BA7050,
	PrimaryPickupItemEntry = 0x350,
	levels = 0x178, // https://dumpspace.spuckwaffel.com/Games/?hash=6b77eceb&type=classes&idx=UWorld&member=Levels
	ItemDefinition = 0x18,
	OwningGameInstance = 0x1d8,
	displayname = 0x38,
	tier = 0xEB,
	last_sumbit = 0x2E8,
	last_render = 0x2F0,
	current_wepon = 0xA68,
	weapon_data = 0x500,
	btargetedbuild = 0x1710,
	game_state = 0x160,
	local_player = 0x38,
	player_controller = 0x30,
	acknowledged_pawn = 0x338,
	skeletal_mesh = 0x318,
	player_state = 0x2b0,
	root_component = 0x198,
	velocity = 0x168,
	relative_location = 0x120,
	relative_rotation = 0x138,
	team_index = 0x1211,
	player_array = 0x2A8,
	pawn_private = 0x308,
	component_to_world = 0x1c0,
	bone_array = 0x5B8,
	pname = 0xaf0,
};

