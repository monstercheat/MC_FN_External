#include <mutex>
#include <memory>
inline bool g_aimbot = 0;



inline bool g_performance = 0;

inline int mcnugget_distance = 50;

inline bool g_player_debug = 0;

inline bool chicken_draw = 0;

inline bool supplydrop = 0;

inline int supplydrop_distance = 50;

inline bool g_teamid = 0;

inline bool g_developer = 0;

inline bool g_target_line = 0;

inline bool draw_chests = 0;

inline bool lootesp = 0;

inline bool g_font = 1;

inline int g_font_type = 1;


inline bool g_loot_snapline = 0;

inline bool draw_cooler = 0;

inline bool g_prediction = 0;

inline int cooler_render = 50;

inline bool draw_ammo = 0;

inline bool g_weapon_rarity = 0;

inline int ammo_render = 50;

inline bool g_platform = 0;

inline float developer_distance = 250;

inline int lammarender = 50;

inline int vehiclesrender = 50;

inline bool lamma = 0;

inline bool pickups = 0;

inline bool draw_vehicles = 0;

inline bool g_weapon_esp = 0;

inline bool g_weapon = 0;

inline bool g_bTargetedBuild = 0;

inline int chestrender = 50;

inline int pickupsrender = 50;

inline float g_width = 0.37;

inline bool g_bIgnoreHidden = 0;

inline bool g_legit = 0;

inline bool g_bIsStaggered = 0;

inline bool g_bIsInvulnerable = 0;

inline bool Common = 0;

inline bool rare = 0;

inline bool purple = 0;

inline bool gold = 0;

inline bool mythic = 0;

inline bool UnCommon = 0;

inline bool g_bIgnoreDead = 0;

inline float g_skeleton_thickness = 2;

inline bool g_framerate = 1;

inline bool g_skel_outline = 0;

inline bool g_rendered = 1;

inline float g_details_font_isze = 12;

inline bool g_dll = 0;

inline bool g_kernel = 0;

inline float g_font_size = 15.5;

inline bool g_visible_check = 1;

inline bool g_watermark = 1;

inline bool g_line = 0;

inline int g_line_type = 0;

inline float g_aimfov = 300;

inline int g_hitbox = 0;

inline bool g_skeleton = 0;

inline bool g_box_esp = 1;

inline bool g_enable_esp = 1;

inline bool g_distance = 1;

inline bool g_username = 0;

inline bool g_snapline = 0;

static float g_color_Visible[3] = { 255 / 255.0f, 0 / 255.0f, 255 / 255.0f };
static float g_color_invisible[3] = { 173 / 255.0f, 255 / 255.0f, 17 / 255.0f };
static float g_color[3] = { 1.00f, 1.00f, 1.00f };
static float g_fov_color[3] = { 1.00f, 1.00f, 1.00f };
static float g_usernamecolor[3] = { 1.00f, 1.00f, 1.00f };
static float g_distance_color[3] = { 1.00f, 1.00f, 1.00f };
static float g_platform_color[3] = { 1.00f, 1.00f, 1.00f };

float g_render_distance = 275;

inline int g_box_type = 3;

inline bool g_outline = 1;

inline float g_box_thick = 1.955f;

inline int g_line_thick = 2;

inline float g_aim_max_dist = 275;

inline float g_smooth = 4.2f;

inline bool g_render_fov = 1;

inline bool g_render_menu = 1;

inline bool in_lobby;

inline float m_width = 0;
inline float m_height = 0;


namespace rifle
{
	inline bool aimbot = 1;
	inline bool visible_check = 1;
	inline bool render_fov = 0;
	inline float aim_fov = 15;
	inline bool target_line = 0;
	inline float smooth = 3;
	inline int hitbox = 0;
	inline int aimkey;
}

namespace shotgun
{
	inline bool aimbot = 1;
	inline bool visible_check = 1;
	inline float aim_fov = 15;
	inline bool target_line = 0;
	inline bool render_fov = 0;
	inline float smooth = 3;
	inline int hitbox = 0;
	inline int aimkey;
}

int HeldWeaponType;
__int64 TargetedBuild;
bool bTargetedBuild;
bool developer = 0;
//int virginia = 0;

namespace smg
{
	inline bool aimbot = 1;
	inline bool visible_check = 1;
	inline bool render_fov = 0;
	inline 	float aim_fov = 15;
	inline bool target_line = 0;
	inline 	float smooth = 3;
	inline int hitbox = 0;
	inline int aimkey;
}

namespace sniper
{
	inline bool aimbot = 1;
	inline bool visible_check = 1;
	inline	float aim_fov = 15;
	inline bool render_fov = 0;
	inline bool target_line = 0;
	inline float smooth = 3;
	inline int hitbox = 0;
	inline int aimkey;
}

namespace Preview
{
	namespace box3d {
		float topleftfront_x = -72.f;
		float topleftfront_y = 82.f;

		float topleftback_x = -54.f;
		float topleftback_y = 61.f;

		float toprightfront_x = 61.f;
		float toprightfront_y = 82.f;

		float toprightback_x = 74.f;
		float toprightback_y = 61.f;

		float bottomleftfront_x = -85.f;
		float bottomleftfront_y = 335.f;

		float bottomleftback_x = -67.f;
		float bottomleftback_y = 305.f;

		float bottomrightfront_x = 49.f;
		float bottomrightfront_y = 342.f;

		float bottomrightback_x = 64.f;
		float bottomrightback_y = 312.f;
	}

	float head_bone_x = 0;
	float head_bone_y = 94;

	float neck_bone_x = 4;
	float neck_bone_y = 114;

	float chest_bone_x = 2;
	float chest_bone_y = 122;

	float left_shoulder_bone_x = -24;
	float left_shoulder_bone_y = 124;

	float left_elbow_bone_x = -37;
	float left_elbow_bone_y = 153;

	float left_hand_bone_x = -51;
	float left_hand_bone_y = 196;

	float right_hand_bone_x = 47;
	float right_hand_bone_y = 190;

	float right_elbow_bone_x = 43;
	float right_elbow_bone_y = 167;

	float right_shoulder_bone_x = 20;
	float right_shoulder_bone_y = 124;

	float pelvis_bone_x = -6;
	float pelvis_bone_y = 184;

	float left_thigh_bone_x = -22;
	float left_thigh_bone_y = 186;

	float right_thigh_bone_x = 12;
	float right_thigh_bone_y = 186;

	float right_knee_bone_x = 25;
	float right_knee_bone_y = 253;

	float left_knee_bone_x = -37;
	float left_knee_bone_y = 245;

	float right_foot_bone_x = 31;
	float right_foot_bone_y = 310;

	float left_foot_bone_x = -47;
	float left_foot_bone_y = 308;

	float right_footfinger_bone_x = 37;
	float right_footfinger_bone_y = 324;

	float left_footfinger_bone_x = -57;
	float left_footfinger_bone_y = 322;

}