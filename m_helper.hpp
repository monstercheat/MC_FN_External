#include "m_world.cpp"
extern uint64_t GameBase;
class loop {
public:

	static auto actors_call( ) -> void{

		std::thread([&]() { for (;; ) { actors(); } }).detach();
	}

	static auto actors() -> void {

		temporary_entity_list.clear();
		std::lock_guard<std::mutex> lock(fuck);

		uintptr_t uworld = Read <uintptr_t>(GameBase + offset::Uworld);
		arrays->uworld = uworld;
		if (!arrays->uworld == false) {
			std::cout << "uworld is null" << std::endl;
			//Sleep(1000);
			//return; // Exit or handle the error appropriately
		}

		uintptr_t game_instance = Read <uintptr_t>(uworld + offset::OwningGameInstance); //https://dumpspace.spuckwaffel.com/Games/?hash=6b77eceb&type=classes&idx=UWorld&member=OwningGameInstance
		arrays->game_instance = game_instance;
		if (!arrays->game_instance) {
			//std::cout << "game_instance is null" << std::endl;
			//Sleep(1000);

			//return; // Exit or handle the error appropriately
		}

		uintptr_t local_player = Read <uintptr_t>(Read <uintptr_t>(game_instance + offset::local_player));
		arrays->local_player = local_player;
		if (!arrays->local_player) {
			//std::cout << "local_player is null" << std::endl;
			//Sleep(1000);
			//return; // Exit or handle the error appropriately
		}

		uintptr_t player_controller = Read <uintptr_t>(local_player + offset::player_controller);
		arrays->player_controller = player_controller;
		if (!arrays->player_controller) {
			//std::cout << "player_controller is null" << std::endl;
			//Sleep(1000);

			return; // Exit or handle the error appropriately
		}

		uintptr_t acknowledged_pawn = Read <uintptr_t>(player_controller + offset::acknowledged_pawn); //https://dumpspace.spuckwaffel.com/Games/?hash=6b77eceb&type=classes&idx=APlayerController&member=AcknowledgedPawn
		arrays->acknowledged_pawn = acknowledged_pawn;
		if (!arrays->acknowledged_pawn) {
			//std::cout << "acknowledged_pawn_MAINNNN is null" << std::endl;
			//Sleep(1000);

			//return; // Exit or handle the error appropriately
		}

		uintptr_t skeletal_mesh = Read <uintptr_t>(acknowledged_pawn + offset::skeletal_mesh); //https://dumpspace.spuckwaffel.com/Games/?hash=6b77eceb&type=classes&idx=ACharacter&member=Mesh
		arrays->skeletal_mesh = skeletal_mesh;
		if (!arrays->skeletal_mesh) {
			std::cout << "skeletal_mesh is null" << std::endl;
			//Sleep(1000);

			//return; // Exit or handle the error appropriately
		}

		uintptr_t player_state = Read <uintptr_t>(acknowledged_pawn + offset::player_state); //https://dumpspace.spuckwaffel.com/Games/?hash=6b77eceb&type=classes&idx=APawn&member=PlayerState
		arrays->player_state = player_state;
		if (!arrays->player_state) {
			//std::cout << "player_state is null" << std::endl;
			//Sleep(1000);
			Sleep(500);

			//return; // Exit or handle the error appropriately
		}

		uintptr_t root_component = Read <uintptr_t>(acknowledged_pawn + offset::root_component);  //https://dumpspace.spuckwaffel.com/Games/?hash=6b77eceb&type=classes&idx=AActor&member=RootComponent
		arrays->root_component = root_component;
		if (!arrays->root_component) {
			//std::cout << "root_component is null" << std::endl;
			Sleep(500);

			//return; // Exit or handle the error appropriately
		}

		fvector relative_location = Read <fvector>(root_component + offset::relative_location); //https://dumpspace.spuckwaffel.com/Games/?hash=6b77eceb&type=classes&idx=USceneComponent&member=RelativeLocation
		arrays->relative_location = relative_location;
		if (!arrays->relative_location.x == 0 && arrays->relative_location.y == 0) {
			//std::cout << "relative_location is null" << std::endl;
			Sleep(500);

			//return; // Exit or handle the error appropriately
		}
		int team_index = Read <int>(player_state + offset::team_index); //https://dumpspace.spuckwaffel.com/Games/?hash=6b77eceb&type=classes&idx=AFortPlayerStateAthena&member=TeamIndex
		arrays->team_index = team_index;
		if (!arrays->team_index) {
			//std::cout << "team_index is null" << std::endl;
			Sleep(500);

			//return; // Exit or handle the error appropriately
		}

		uintptr_t game_state = Read <uintptr_t>(uworld + offset::game_state); //https://dumpspace.spuckwaffel.com/Games/?hash=6b77eceb&type=classes&idx=UWorld&member=GameState
		arrays->game_state = game_state;
		if (!arrays->game_state) {
			//std::cout << "game_state is null" << std::endl;
			Sleep(500);

			//return; // Exit or handle the error appropriately
		}

		uintptr_t player_array = Read <uintptr_t>(game_state + offset::player_array); //https://dumpspace.spuckwaffel.com/Games/?hash=6b77eceb&type=classes&idx=AGameStateBase&member=PlayerArray
		arrays->player_array = player_array;
		if (!arrays->player_array) {
		//std::cout << "player_array is null" << std::endl;
		Sleep(500);

			//return; // Exit or handle the error appropriately
		}

		int player_array_size = Read <int>(game_state + (offset::player_array + sizeof(uintptr_t)));
		arrays->player_array_size = player_array_size;
		//std::cout << "total players: " << player_array_size << std::endl;

		if (developer)
		{
			std::cout << uworld << std::endl;
			std::cout << game_instance << std::endl;
		}

		if (g_weapon)
		{
			if (arrays->acknowledged_pawn)
			{
				uint64_t player_weapon = Read <uint64_t>(arrays->acknowledged_pawn + offset::current_wepon); // 	struct AFortWeapon* CurrentWeapon;

				if (mem::is_valid(player_weapon)) {

					uint64_t weapon_data = Read <uint64_t>(player_weapon + offset::weapon_data);	//struct UFortWeaponItemDefinition* WeaponData;

					if (mem::is_valid(weapon_data)) {

						uint64_t ftext_ptr = Read <uint64_t>(weapon_data + offset::displayname);

						if (mem::is_valid(ftext_ptr)) {
							uint64_t ftext_data = Read <uint64_t>(ftext_ptr + 0x28);
							int ftext_length = Read <int>(ftext_ptr + 0x30);
							if (ftext_length > 0 && ftext_length < 50) {
								wchar_t* ftext_buf = new wchar_t[ftext_length];
								read_array(ftext_data, ftext_buf, ftext_length * sizeof(wchar_t));

								wchar_t* WeaponName = ftext_buf;

								delete[] ftext_buf;

								if (wcsstr(WeaponName, (L"Assault Rifle")) || wcsstr(WeaponName, (L"AR")) || wcsstr(WeaponName, (L"Ambush Striker AR")) || wcsstr(WeaponName, (L"Striker AR")) || wcsstr(WeaponName, (L"Drum Gun"))) {
									HeldWeaponType = EFortWeaponType::Rifle;
									g_aimbot = rifle::aimbot;
									g_visible_check = rifle::visible_check;
									g_aimfov = rifle::aim_fov;
									g_smooth = rifle::smooth;
									g_hitbox = rifle::hitbox;
									g_target_line = rifle::target_line;
									aimkey = rifle::aimkey;

									if (debug_weapons)
									{
										std::cout << ("RIFLE HELD") << std::endl;
									}
								}

								if (wcsstr(WeaponName, (L"Shotgun")) || wcsstr(WeaponName, (L"Pump Shotgun")) || wcsstr(WeaponName, (L"Pump"))) {
									HeldWeaponType = EFortWeaponType::Shotgun;
									g_aimbot = shotgun::aimbot;
									g_visible_check = shotgun::visible_check;
									g_aimfov = shotgun::aim_fov;
									g_smooth = shotgun::smooth;
									g_hitbox = shotgun::hitbox;
									g_target_line = shotgun::target_line;
									aimkey = shotgun::aimkey;
									if (debug_weapons)
									{
										std::cout << ("SHOTGUN HELD") << std::endl;
									}
								}

								if (wcsstr(WeaponName, (L"Smg")) || wcsstr(WeaponName, (L"Submachine Gun")) || wcsstr(WeaponName, (L"Combat Smg")) || wcsstr(WeaponName, (L"Pistol")) || wcsstr(WeaponName, (L"Machine Smg"))
									|| wcsstr(WeaponName, (L"Burst SMG")) || wcsstr(WeaponName, (L"Hyper SMG"))) {
									HeldWeaponType = EFortWeaponType::Smg;
									g_aimbot = smg::aimbot;
									g_visible_check = smg::visible_check;
									g_render_fov = smg::render_fov;
									g_aimfov = smg::aim_fov;
									g_smooth = smg::smooth;
									g_hitbox = smg::hitbox;
									g_target_line = smg::target_line;
									aimkey = smg::aimkey;
									if (debug_weapons)
									{
										std::cout << ("SMG HELD") << std::endl;
									}
								}

								if (wcsstr(WeaponName, (L"Sniper Rifle")) || wcsstr(WeaponName, (L"Storm Scout")) || wcsstr(WeaponName, (L"Reaper Sniper Rifle")) || wcsstr(WeaponName, (L"Hunter Bolt-Action Sniper")) || wcsstr(WeaponName, (L"Suppressed Sniper Rifle"))
									|| wcsstr(WeaponName, (L"DMR")) || wcsstr(WeaponName, (L"Crossbow"))) {
									HeldWeaponType = EFortWeaponType::Smg;
									g_aimbot = sniper::aimbot;
									g_visible_check = sniper::visible_check;
									g_aimfov = sniper::aim_fov;
									g_smooth = sniper::smooth;
									g_target_line = sniper::target_line;
									g_hitbox = sniper::hitbox;
									aimkey = sniper::aimkey;
									if (debug_weapons)
									{
										std::cout << ("sniper HELD") << std::endl;
									}
								}
							}
						}
					}
				}
			}
		}

		for (int i = 0; i < arrays->player_array_size; ++i) {
			auto player_state = Read<uintptr_t>(arrays->player_array + (i * sizeof(uintptr_t))); auto current_actor = Read<uintptr_t>(player_state + offset::pawn_private);

			if (current_actor == arrays->acknowledged_pawn) continue;
			auto skeletalmesh = Read<uintptr_t>(current_actor + offset::skeletal_mesh);
			if (!skeletalmesh) continue;

			auto base_bone = ue5->Bone(skeletalmesh, bone::Root);
			if (base_bone.x == 0 && base_bone.y == 0 && base_bone.z == 0) continue;

			if (g_performance && !ue5->in_screen(ue5->w2s(ue5->Bone(skeletalmesh, bone::Pelvis)))) continue;	
			 
			auto root_component = Read <uintptr_t>(current_actor + offset::root_component);
			auto relative_location1 = Read <fvector>(root_component + offset::relative_location);
			auto team_index = Read<int>(player_state + offset::team_index);
			auto kills1 = Read<int>(player_state + 0x1264); //IntProperty Class FortniteGame.FortPlayerStateAthena.KillScore

			if (arrays->acknowledged_pawn && Read<int>(player_state + offset::team_index) == arrays->team_index) {
				continue;
			}

			entity cached_actors{ };
			cached_actors.entity = current_actor;
			cached_actors.skeletal_mesh = skeletalmesh;
			cached_actors.root_component = root_component;
			cached_actors.relative_location = relative_location1;
			cached_actors.player_state = player_state;
			cached_actors.team_index = team_index;
			cached_actors.kills = kills1;

			temporary_entity_list.push_back(cached_actors);
		}

		entity_list.clear();
		entity_list = temporary_entity_list;
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	}	
}; static loop* m_cache = new loop();



