#include "m_vectors.hpp"
#include "ImGui/imgui.h"
#include <d3d9.h>
#include "ImGui/imgui_impl_win32.h"
#include "Driver.h"
#include "m_globals.hpp"
#include "m_definition.hpp"
ImFont* MenuFont;
ImFont* GameFont;
ImFont* IconsFont;
ImFont* FortniteFont;
ImFont* MinecraftFont;
ImFont* Arial;
ImFont* Verdana;
ImFont* proggy;
ImFont* icons;
ImFont* IconFont;
ImFont* IconFontLogs;


class entity {
public:
	uintptr_t
		entity,
		skeletal_mesh,
		root_component,
		player_state;
		
	int
		team_index,
		kills;
	char
		team_number;
	fvector
		relative_location;
	float
		lastrendertime;
	bool
		is_visible;
};
std::vector<entity> entity_list;
std::vector<entity> temporary_entity_list;

struct WeaponInformation
{
	int32_t ammo_count;
	int32_t max_ammo;

	BYTE tier;
	std::string weapon_name;
	std::string buildplan;

};
std::string LocalPlayerWeapon;


WeaponInformation WeaponInfo;

class item {
public:
	uintptr_t
		Actor;

	std::string
		Name,
		isVehicle,
		isChest,
		isLamma,
		isPickup,
		isAmmoBox;
	float
		distance;
};
std::vector<item> item_pawns;

typedef struct _LootEntity {
	std::string GNames;
	uintptr_t ACurrentItem;
}LootEntity;

static std::vector<LootEntity> LootentityList;

namespace o {
class p
{
	public:
		uintptr_t
			uworld,
			game_instance,
			game_state,
			local_player,
			player_controller,
			acknowledged_pawn,
			skeletal_mesh,
			player_state,
			root_component,
			player_array;
		int
			team_index,
			player_array_size;
		fvector
			relative_location,
			velocity;
	};
} static o::p* arrays = new o::p();






