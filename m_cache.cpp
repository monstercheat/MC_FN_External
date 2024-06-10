#include "m_helper.hpp"

void DrawThreeDBox(const fvector& base, const fvector& head, float width, float height, float tick, ImU32 color) {
	// Calculate the corner points
	fvector halfWidthOffset = { width / 2, 0, 0 };
	fvector halfHeightOffset = { 0, height / 2, 0 };

	fvector bottom1 = { base.x - halfWidthOffset.x, base.y - halfHeightOffset.y, base.z };
	fvector bottom2 = { base.x + halfWidthOffset.x, base.y - halfHeightOffset.y, base.z };
	fvector bottom3 = { base.x + halfWidthOffset.x, base.y + halfHeightOffset.y, base.z };
	fvector bottom4 = { base.x - halfWidthOffset.x, base.y + halfHeightOffset.y, base.z };

	fvector top1 = { head.x - halfWidthOffset.x, head.y - halfHeightOffset.y, head.z + 20 };
	fvector top2 = { head.x + halfWidthOffset.x, head.y - halfHeightOffset.y, head.z + 20 };
	fvector top3 = { head.x + halfWidthOffset.x, head.y + halfHeightOffset.y, head.z + 20 };
	fvector top4 = { head.x - halfWidthOffset.x, head.y + halfHeightOffset.y, head.z + 20 };

	// Project the 3D world coordinates to 2D screen coordinates
	fvector2d screenBottom1 = ue5->w2s(bottom1);
	fvector2d screenBottom2 = ue5->w2s(bottom2);
	fvector2d screenBottom3 = ue5->w2s(bottom3);
	fvector2d screenBottom4 = ue5->w2s(bottom4);

	fvector2d screenTop1 = ue5->w2s(top1);
	fvector2d screenTop2 = ue5->w2s(top2);
	fvector2d screenTop3 = ue5->w2s(top3);
	fvector2d screenTop4 = ue5->w2s(top4);

	// Draw the lines
	auto draw_list = ImGui::GetForegroundDrawList();
	ImVec2 points[] = { ImVec2(screenBottom1.x, screenBottom1.y), ImVec2(screenBottom2.x, screenBottom2.y), ImVec2(screenBottom3.x, screenBottom3.y), ImVec2(screenBottom4.x, screenBottom4.y),
						ImVec2(screenTop1.x, screenTop1.y), ImVec2(screenTop2.x, screenTop2.y), ImVec2(screenTop3.x, screenTop3.y), ImVec2(screenTop4.x, screenTop4.y) };

	for (int i = 0; i < 4; i++) {
		draw_list->AddLine(points[i], points[(i + 1) % 4], color, tick);
		draw_list->AddLine(points[i + 4], points[(i + 1) % 4 + 4], color, tick);
		draw_list->AddLine(points[i], points[i + 4], color, tick);
	}
}

class g_fn	 {
public:
;
	auto actor_loop() -> void {

		if (g_font)
		{
			switch (g_font_type)
			{
			case 0:
				ImGui::PushFont(proggy);
				break;
			case 1:
				ImGui::PushFont(FortniteFont);
				break;
			case 2:
				ImGui::PushFont(Verdana);
				break;
			case 3:
				ImGui::PushFont(Arial);
				break;
			}
		}

		ue5->get_camera();

		ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
		float target_dist = FLT_MAX;
		uintptr_t target_entity = 0;

		if (g_watermark) {
			g_draw->draw_string(17, 5, 0, ImColor(255, 255, 255, 255), false, true, std::string("MC FN Private").c_str());

		}

		

		if (g_framerate) {
			//virginia += 20;
			char fppss[256];
			sprintf_s(fppss, std::string("Cheat FPS %.3f ms/frame (%.1f FPS)").c_str(), 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			g_draw->draw_string(15, 5, 36, ImColor(255, 255, 255, 255), false, true, fppss);
		}

		const float centerWidth = screen_width / 2;
		const float centerHeight = screen_height / 2;

		for (const auto& cached : entity_list) {
			auto root_bone = ue5->Bone(cached.skeletal_mesh, bone::Root);
			root = ue5->w2s(root_bone);
			root_box = ue5->w2s(fvector(root_bone.x, root_bone.y, root_bone.z + 30));
			root_box1 = ue5->w2s(fvector(root_bone.x, root_bone.y, root_bone.z - 15));
			auto head_bone = ue5->Bone(cached.skeletal_mesh, bone::Head);
			head = ue5->w2s(head_bone);
			head_box = ue5->w2s(fvector(head_bone.x, head_bone.y, head_bone.z + 15));

			float box_height = abs(head.y - root_box1.y);
			float box_width = box_height * g_width;
			float distance = camera_postion.location.distance(root_bone) * 0.01;




			if (distance > g_render_distance && arrays->acknowledged_pawn) continue;

			if (g_aimbot) {

				auto dx = head.x - (screen_width / 2);
				auto dy = head.y - (screen_height / 2);
				auto dist = sqrtf(dx * dx + dy * dy);

				if (g_visible_check) {
					if (ue5->is_visible(cached.skeletal_mesh)) {
						if (dist < g_aimfov && dist < target_dist) {
							target_dist = dist;
							target_entity = cached.entity;
						}
					}
				}
				else {
					if (dist < g_aimfov && dist < target_dist) {
						target_dist = dist;
						target_entity = cached.entity;
					}
				}
			}

			if (g_bIsStaggered)
			{
				bool bIsStaggered = Read<BYTE>(cached.entity + 0x759); //https://dumpspace.spuckwaffel.com/Games/?hash=6b77eceb&type=classes&idx=AFortPawn&member=bIsStaggered : 1
				if (!bIsStaggered)
					continue;
			}

			if (g_bIsInvulnerable)
			{
				bool bIsInvulnerable = Read<BYTE>(cached.entity + 0x75a); //https://dumpspace.spuckwaffel.com/Games/?hash=6b77eceb&type=classes&idx=AFortPawn&member=bIsInvulnerable : 1
				if (!bIsInvulnerable)
					continue;
			}

			if (g_bIgnoreDead) {
				bool bIsDying = (Read<BYTE>(cached.entity + 0x758) & 0x10); // https://dumpspace.spuckwaffel.com/Games/?hash=6b77eceb&type=classes&idx=AFortPawn&member=bIsDying
				bool bisDBNO = (Read<BYTE>(cached.entity + 0x982) & 0x10); // https://dumpspace.spuckwaffel.com/Games/?hash=6b77eceb&type=classes&idx=AFortPawn&member=bIsDBNO 
				if (bisDBNO || bIsDying)
					continue;
			}

			ImColor vis;
			ImColor invis;
			ImColor normal;

			vis = ImVec4(g_color_Visible[0], g_color_Visible[1], g_color_Visible[2], 1.0f);
			invis = ImVec4(g_color_invisible[0], g_color_invisible[1], g_color_invisible[2], 1.0f);
			normal = ImVec4(g_color[0], g_color[1], g_color[2], 1.0f);

			ImColor visibleColor;

			if (g_visible_check && g_enable_esp)
			{
				visibleColor = ue5->is_visible(cached.skeletal_mesh) ? ImColor(vis) : ImColor(invis);
			}
			else
			{
				visibleColor = ImColor(normal);
			}

			if (g_line && g_enable_esp)
			{
				switch (g_line_type)
				{
				case 0: //top
					draw_list->AddLine(ImVec2(screen_width / 2, 0), ImVec2(head_box.x, head_box.y), visibleColor, g_line_thick);
					break;
				case 1:	//middle	
					draw_list->AddLine(ImVec2(screen_width / 2, screen_height / 2), ImVec2(head_box.x, head_box.y), visibleColor, g_line_thick);
					break;
				case 2: //bottom
					draw_list->AddLine(ImVec2(screen_width / 2, screen_height), ImVec2(root_box.x, root_box.y), visibleColor, g_line_thick);
					break;
				}
			}

			if (g_box_esp && g_enable_esp) {
				switch (g_box_type)
				{
				case 0:  //2d
					if (g_outline)
					{
						draw_list->AddLine(ImVec2(head_box.x - (box_width / 2), head_box.y), ImVec2(root.x + (box_width / 2), head_box.y), ImColor(0, 0, 0), g_box_thick + 2);
						draw_list->AddLine(ImVec2(head_box.x - (box_width / 2), head_box.y), ImVec2(head_box.x - (box_width / 2), root.y), ImColor(0, 0, 0), g_box_thick + 2);
						draw_list->AddLine(ImVec2(head_box.x - (box_width / 2), root.y), ImVec2(root.x + (box_width / 2), root.y), ImColor(0, 0, 0), g_box_thick + 2);
						draw_list->AddLine(ImVec2(root.x + (box_width / 2), head_box.y), ImVec2(root.x + (box_width / 2), root.y), ImColor(0, 0, 0), g_box_thick + 2);
					}

					draw_list->AddLine(ImVec2(head_box.x - (box_width / 2), head_box.y), ImVec2(root.x + (box_width / 2), head_box.y), visibleColor, g_box_thick);
					draw_list->AddLine(ImVec2(head_box.x - (box_width / 2), head_box.y), ImVec2(head_box.x - (box_width / 2), root.y), visibleColor, g_box_thick);
					draw_list->AddLine(ImVec2(head_box.x - (box_width / 2), root.y), ImVec2(root.x + (box_width / 2), root.y), visibleColor, g_box_thick);
					draw_list->AddLine(ImVec2(root.x + (box_width / 2), head_box.y), ImVec2(root.x + (box_width / 2), root.y), visibleColor, g_box_thick);
					break;
				case 1: /*3d*/
					if (g_outline)
					{
						DrawThreeDBox(root_bone, head_bone, 218.4375 * g_width, 100, g_box_thick + 2.0, ImColor(0, 0, 0));
					}

					DrawThreeDBox(root_bone, head_bone, 218.4375 * g_width, 100, g_box_thick, visibleColor);

					break;
				case 2: /*rounded*/
					if (g_outline)
					{
						draw_list->AddRect(ImVec2(head_box.x - box_width / 2, head_box.y), ImVec2((head_box.x - box_width / 2) + box_width, head_box.y + box_height), ImColor(0, 0, 0), 10, ImDrawCornerFlags_All, g_box_thick + 2.0);
					}

					draw_list->AddRect(ImVec2(head_box.x - box_width / 2, head_box.y), ImVec2((head_box.x - box_width / 2) + box_width, head_box.y + box_height), visibleColor, 10, ImDrawCornerFlags_All, g_box_thick);

					break;
				case 3: /*cornered*/
					if (g_outline)
					{
						g_draw->draw_cornered_box(root.x - (box_width / 2), head_box.y, box_width, box_height, ImColor(0, 0, 0), g_box_thick + 2.0, 0, 0);
					}
					//DrawThreeDBox(root_bone, head_bone, 100, 100, 1.995f, visibleColor);

					g_draw->draw_cornered_box(root.x - (box_width / 2), head_box.y, box_width, box_height, visibleColor, g_box_thick, 0, 0);
					break;
				case 4: /*bounding*/
					if (g_outline)
					{
						g_draw->bounding(head_box.x - (box_width / 2), head_box.y, box_width, box_height, ImColor(0, 0, 0), g_box_thick + 2.5);
					}

					g_draw->bounding(head_box.x - (box_width / 2), head_box.y, box_width, box_height, visibleColor, g_box_thick);
					break;
				}
			}
		
			if (g_username && g_enable_esp) {

				if (arrays->player_array_size > 1)
				{
					if (g_teamid)
					{
						std::string username_str = "[ID:" + std::to_string(cached.team_index) + " | " + ue5->get_player_name(cached.player_state) + " (" + std::to_string(cached.kills) + ")" "]";
						ImVec2 text_size = ImGui::CalcTextSize(username_str.c_str());
						g_draw->draw_string(g_font_size, head_box.x - (text_size.x / 2), head_box.y - 20, ImColor(g_usernamecolor[0], g_usernamecolor[1], g_usernamecolor[2]), false, true, username_str.c_str());
					}
					else
					{
						std::string username_str = ue5->get_player_name(cached.player_state) + " (" + std::to_string(cached.kills) + ")";
						ImVec2 text_size = ImGui::CalcTextSize(username_str.c_str());
						g_draw->draw_string(g_font_size, head_box.x - (text_size.x / 2), head_box.y - 20, ImColor(g_usernamecolor[0], g_usernamecolor[1], g_usernamecolor[2]), false, true, username_str.c_str());
					}
				}	
			}

			if (g_weapon_esp && g_enable_esp)
			{
				if (arrays->acknowledged_pawn)
				{
					WeaponInformation held_weapon{ };

					uint64_t player_weapon = Read<uint64_t>(cached.entity + offset::current_wepon);

					if (mem::is_valid(player_weapon)) {
						held_weapon.ammo_count = Read <int32_t>(player_weapon + 0xe54);

						uint64_t weapon_data = Read <uint64_t>(player_weapon + offset::weapon_data);

						if (mem::is_valid(weapon_data)) {
							held_weapon.tier = Read <BYTE>(weapon_data + offset::tier);

							uint64_t ftext_ptr = Read <uint64_t>(weapon_data + offset::displayname);

							if (mem::is_valid(ftext_ptr)) {
								uint64_t ftext_data = Read <uint64_t>(ftext_ptr + 0x28);
								int ftext_length = Read <int>(ftext_ptr + 0x30);
								if (ftext_length > 0 && ftext_length < 50) {
									//wchar_t* ftext_buf = new wchar_t[ftext_length];
									//
									//Read_physical(ftext_data, ftext_buf, ftext_length * sizeof(wchar_t));
									//std::wstring wstr_buf(ftext_buf);
									//held_weapon.weapon_name = std::string(wstr_buf.begin(), wstr_buf.end());
									//delete[] ftext_buf;


									// Assuming ftext_length is the length of the text, not including a null terminator
									wchar_t* ftext_buf = new wchar_t[ftext_length + 1]; // Allocate one extra wchar_t for null-termination

									// Read the data into the buffer, leaving space for the null terminator
									read_array(ftext_data, ftext_buf, ftext_length * sizeof(wchar_t));

									// Manually null-terminate the buffer
									//ftext_buf[ftext_length] = L'\0';

									// Validate that the buffer contains valid data before using it
									// This could mean checking against specific criteria relevant to your application
									// For simplicity, we'll just ensure it's not an empty string
									if (ftext_buf[0] != L'\0') {
										std::wstring wstr_buf(ftext_buf);
										// Convert std::wstring to std::string
										// Note: This conversion is simplistic and assumes ASCII content in the wstring.
										// For more complex scenarios, consider using a proper conversion library
										held_weapon.weapon_name = std::string(wstr_buf.begin(), wstr_buf.end());
									}
									else {
										// Handle the case where the buffer doesn't contain valid data
										// This could mean setting a default value or handling an error
										held_weapon.weapon_name = "Unkown"; // Example default value
									}

									delete[] ftext_buf; // Clean up the allocated buffer
								}
							}
						}

						WeaponInfo = held_weapon;

						std::string final = ("[") + WeaponInfo.weapon_name + " [" + std::to_string(WeaponInfo.ammo_count) + "]";

						ImVec2 TextSize = ImGui::CalcTextSize(final.c_str());

						ImVec2 text_size = ImGui::CalcTextSize(final.c_str());
						int add;
						if (g_distance)
						{
							add = 25;
						}
						else
						{
							add = 5;
						}

						if (g_weapon_rarity)
						{
							if (WeaponInfo.tier == 0)
							{ //true
								if (strstr(WeaponInfo.weapon_name.c_str(), ("[Pickaxe]")) != nullptr)
								{
									g_draw->draw_string(g_font_size, (root.x) - (TextSize.x / 2), (root.y + add), ImColor(255, 255, 255), false, true, final.c_str());
								}
								else
								{
									std::string fina1l = ("[Building Plan]");
									ImVec2 TextSiz1e = ImGui::CalcTextSize(fina1l.c_str());
									g_draw->draw_string(g_font_size, (root.x) - (TextSiz1e.x / 2), (root.y + add), ImColor(255, 255, 255), false, true, fina1l.c_str());
								}
							}
							if (WeaponInfo.tier == 1)
							{
								g_draw->draw_string(g_font_size, (root.x) - (TextSize.x / 2), (root.y + add), ImColor(170, 165, 169), false, true, final.c_str());
							}
							if (WeaponInfo.tier == 2)
							{
								g_draw->draw_string(g_font_size, (root.x) - (TextSize.x / 2), (root.y + add), ImColor(30, 255, 0), false, true, final.c_str());
							}
							if (WeaponInfo.tier == 3)
							{
								g_draw->draw_string(g_font_size, (root.x) - (TextSize.x / 2), (root.y + add), ImColor(0, 112, 221), false, true, final.c_str());
							}
							if (WeaponInfo.tier == 4)
							{
								g_draw->draw_string(g_font_size, (root.x) - (TextSize.x / 2), (root.y + add), ImColor(163, 53, 238), false, true, final.c_str());
							}
							if (WeaponInfo.tier == 5)
							{
								g_draw->draw_string(g_font_size, (root.x) - (TextSize.x / 2), (root.y + add), ImColor(255, 128, 0), false, true, final.c_str());
							}
							if (WeaponInfo.tier == 6)
							{
								g_draw->draw_string(g_font_size, (root.x) - (TextSize.x / 2), (root.y + add), ImColor(255, 255, 0), false, true, final.c_str());
							}
						}
						else
						{
							if (WeaponInfo.tier == 0)
							{
								if (strstr(WeaponInfo.weapon_name.c_str(), ("[Pickaxe]")) != nullptr)
								{
									g_draw->draw_string(g_font_size, (root.x) - (TextSize.x / 2), (root.y + add), visibleColor, false, true, final.c_str());
								}
								else
								{
									std::string fina1l = ("[Building Plan]");
									ImVec2 TextSiz1e = ImGui::CalcTextSize(fina1l.c_str());
									g_draw->draw_string(g_font_size, (root.x) - (TextSiz1e.x / 2), (root.y + add), visibleColor, false, true, fina1l.c_str());
								}
							}
							else
							{
								g_draw->draw_string(g_font_size, root.x - (text_size.x / 2), root.y + 20, visibleColor, false, true, final.c_str());
							}
						}
					}
				}						
			}	

			if (g_platform) {
				
				if (arrays->acknowledged_pawn)
				{
					DWORD_PTR test_platform = Read<DWORD_PTR>((u)cached.player_state + 0x438);
					char platform[64]; // Use char array for reading narrow strings
					ReadString64(test_platform, platform, sizeof(platform));
					std::string platformStr(platform); // Convert to std::string if needed
					std::string str_platform(platformStr.begin(), platformStr.end());
					ImVec2 DistanceTextSize2 = ImGui::CalcTextSize(str_platform.c_str());

					if (str_platform.find(("XBL")) != std::string::npos || str_platform.find(("XSX")) != std::string::npos) {
						char XboxText[] = "XBOX";
						ImVec2 DistanceTextSize2 = ImGui::CalcTextSize(XboxText);
						g_draw->draw_string(g_font_size, head_box.x - 3, head_box.y - 31, ImColor(g_platform_color[0], g_platform_color[1], g_platform_color[2]), true, true, XboxText);

					}
					else if (str_platform.find(("PSN")) != std::string::npos || str_platform.find(("PS5")) != std::string::npos) {
						char PSNText[] = "PSN";
						ImVec2 DistanceTextSize2 = ImGui::CalcTextSize(PSNText);
						g_draw->draw_string(g_font_size, head_box.x - 3, head_box.y - 31, ImColor(g_platform_color[0], g_platform_color[1], g_platform_color[2]), true, true, PSNText);
					}
					else if (str_platform.find(("WIN")) != std::string::npos) {
						char WINText[] = "WIN";
						ImVec2 DistanceTextSize2 = ImGui::CalcTextSize(WINText);
						g_draw->draw_string(g_font_size, head_box.x - 3, head_box.y - 31, ImColor(g_platform_color[0], g_platform_color[1], g_platform_color[2]), true, true, WINText);
					}
					else if (str_platform.find(("SWH")) != std::string::npos || str_platform.find(("SWT")) != std::string::npos) {
						char SWHText[] = "SWITCH";
						ImVec2 DistanceTextSize2 = ImGui::CalcTextSize(SWHText);
						g_draw->draw_string(g_font_size, head_box.x - 3, head_box.y - 31, ImColor(g_platform_color[0], g_platform_color[1], g_platform_color[2]), true, true, SWHText);
					}
					else if (str_platform.find(("MAC")) != std::string::npos) {
						char MACText[] = "MAC";
						ImVec2 DistanceTextSize2 = ImGui::CalcTextSize(MACText);
						g_draw->draw_string(g_font_size, head_box.x - 3, head_box.y - 31, ImColor(g_platform_color[0], g_platform_color[1], g_platform_color[2]), true, true, MACText);
					}
					else if (str_platform.find(("LNX")) != std::string::npos) {
						char LINUXText[] = "LINUX";
						ImVec2 DistanceTextSize2 = ImGui::CalcTextSize(LINUXText);
						g_draw->draw_string(g_font_size, head_box.x - 3, head_box.y - 31, ImColor(g_platform_color[0], g_platform_color[1], g_platform_color[2]), true, true, LINUXText);
					}
					else if (str_platform.find(("IOS")) != std::string::npos) {
						char IOSText[] = "IOS";
						ImVec2 DistanceTextSize2 = ImGui::CalcTextSize(IOSText);
						g_draw->draw_string(g_font_size, head_box.x - 3, head_box.y - 31, ImColor(g_platform_color[0], g_platform_color[1], g_platform_color[2]), true, true, IOSText);
					}
					else if (str_platform.find(("AND")) != std::string::npos) {
						char ANDText[] = "ANDROID";
						ImVec2 DistanceTextSize2 = ImGui::CalcTextSize(ANDText);
						g_draw->draw_string(20, head_box.x + 5, head_box.y, ImColor(g_platform_color[0], g_platform_color[1], g_platform_color[2]), true, true, ANDText);
					}
					else {
						g_draw->draw_string(g_font_size, head_box.x - 3, head_box.y - 31, ImColor(g_platform_color[0], g_platform_color[1], g_platform_color[2]), true, true, str_platform.c_str());
					}
				}		
			}

			if (arrays->acknowledged_pawn && g_weapon || g_prediction) {

				uint64_t player_weapon = Read<uint64_t>(arrays->acknowledged_pawn + offset::current_wepon); // 	struct AFortWeapon* CurrentWeapon;

				if (mem::is_valid(player_weapon)) {

					uint64_t weapon_data = Read<uint64_t>(player_weapon + offset::weapon_data);	//struct UFortWeaponItemDefinition* WeaponData;

					if (mem::is_valid(weapon_data)) {

						uint64_t ftext_ptr = Read<uint64_t>(weapon_data + offset::displayname);

						if (mem::is_valid(ftext_ptr)) {
							uint64_t ftext_data = Read<uint64_t>(ftext_ptr + 0x28);
							int ftext_length = Read<int>(ftext_ptr + 0x30);
							if (ftext_length > 0 && ftext_length < 50) {
								wchar_t* ftext_buf = new wchar_t[ftext_length];

								read_array(ftext_data, ftext_buf, ftext_length * sizeof(wchar_t));
								std::wstring wstr_buf(ftext_buf);

								LocalPlayerWeapon = std::string(wstr_buf.begin(), wstr_buf.end());

								wchar_t* WeaponName = ftext_buf;

								delete[] ftext_buf;

								if (wcsstr(WeaponName, (L"Assault Rifle")) || wcsstr(WeaponName, (L"Havoc Suppressed Assault Rifle")) || wcsstr(WeaponName, (L"Red-Eye Assault Rifle"))
									|| wcsstr(WeaponName, (L"Suppressed Assault Rifle")) || wcsstr(WeaponName, (L"Striker Burst Rifle")) || wcsstr(WeaponName, (L"Burst Assault Rifle"))
									|| wcsstr(WeaponName, (L"Ranger Assault Rifle")) || wcsstr(WeaponName, (L"Flapjack Rifle")) || wcsstr(WeaponName, (L"Heavy Assault Rifle"))
									|| wcsstr(WeaponName, (L"MK-Seven Assault Rifle")) || wcsstr(WeaponName, (L"MK-Alpha Assault Rifle")) || wcsstr(WeaponName, (L"Combat Assault Rifle"))
									|| wcsstr(WeaponName, (L"Tactical Assault Rifle")) || wcsstr(WeaponName, (L"Warforged Assault Rifle")) || wcsstr(WeaponName, (L"Striker AR")) || wcsstr(WeaponName, (L"Sideways Rifle")) || wcsstr(WeaponName, (L"AR")) || wcsstr(WeaponName, (L"Nemesis AR")) || wcsstr(WeaponName, (L"Makeshift Rifle"))) {
									HeldWeaponType = EFortWeaponType::Rifle;
		
									if (g_weapon)
									{
										g_aimbot = rifle::aimbot;
										g_render_fov = rifle::render_fov;
										g_aimfov = rifle::aim_fov;
										g_target_line = rifle::target_line;
										g_smooth = rifle::smooth;
									}
								}
								if (wcsstr(WeaponName, (L"Shotgun"))) {

									HeldWeaponType = EFortWeaponType::Shotgun;
									if (g_weapon)
									{
										g_aimbot = shotgun::aimbot;
										g_render_fov = shotgun::render_fov;
										g_target_line = shotgun::target_line;
										g_aimfov = shotgun::aim_fov;
										g_smooth = shotgun::smooth;
									}
								}
								if (wcsstr(WeaponName, (L"Smg")) || wcsstr(WeaponName, (L"Submachine Gun")) || wcsstr(WeaponName, (L"Hyper SMG")) || wcsstr(WeaponName, (L"Thunder Burst SMG")) || wcsstr(WeaponName, (L"SMG")) || wcsstr(WeaponName, (L"Harbinger  Smg")) || wcsstr(WeaponName, (L"Ranger Pistol")) || wcsstr(WeaponName, (L"Pistol"))) {
									HeldWeaponType = EFortWeaponType::Smg;
									if (g_weapon)
									{
										g_aimbot = smg::aimbot;
										g_render_fov = smg::render_fov;
										g_target_line = smg::target_line;
										g_aimfov = smg::aim_fov;
										g_smooth = smg::smooth;
									}
								}
								if (wcsstr(WeaponName, (L"Hunting Rifle")) || wcsstr(WeaponName, (L"Heavy Sniper Rifle")) || wcsstr(WeaponName, (L"Suppressed Sniper Rifle"))
									|| wcsstr(WeaponName, (L"Storm Scout")) || wcsstr(WeaponName, (L"Bolt-Action Sniper Rifle")) || wcsstr(WeaponName, (L"Automatic Sniper Rifle"))
									|| wcsstr(WeaponName, (L"DMR")) || wcsstr(WeaponName, (L"Huntress DMR")) || wcsstr(WeaponName, (L"Hunter Bolt-Action Sniper"))
									|| wcsstr(WeaponName, (L"Crossbow")) || wcsstr(WeaponName, (L"Reaper Sniper Rifle")) || wcsstr(WeaponName, (L"Mechanical Bow"))) {
									HeldWeaponType = EFortWeaponType::Sniper;
									if (g_weapon)
									{
										g_aimbot = sniper::aimbot;
										g_render_fov = sniper::render_fov;
										g_aimfov = sniper::aim_fov;
										g_target_line = sniper::target_line;
										g_smooth = sniper::smooth;
									}
								}
							}
						}
					}
				}
			}
		
			if (g_distance && g_enable_esp)
			{
				std::string Distance = "[" + std::to_string((int)distance) + "M]";
				ImVec2 text_size = ImGui::CalcTextSize(Distance.c_str());
				g_draw->draw_string(g_font_size, root.x - (text_size.x / 2), root.y + 5, ImColor(g_distance_color[0], g_distance_color[1], g_distance_color[2]), false, true, Distance.c_str());

			}

			
			if (g_rendered && g_enable_esp)
			{
				char players_found[256];
				sprintf_s(players_found, (("Players Rendered: %d")), entity_list.size());
				g_draw->draw_string(15, 5, 18, ImColor(255, 255, 255), false, true, players_found);
			}
				
			if (g_skeleton && g_enable_esp) {
				
				fvector2d ChestAtLeft = ue5->w2s(ue5->Bone(cached.skeletal_mesh, 37));
				fvector2d ChestAtRight = ue5->w2s(ue5->Bone(cached.skeletal_mesh, 8));
				fvector2d Chest = { ChestAtLeft.x + (ChestAtRight.x - ChestAtLeft.x) / 2, ChestAtLeft.y };
				fvector2d Neck = ue5->w2s(ue5->Bone(cached.skeletal_mesh, 66));
				fvector2d Pelvis = ue5->w2s(ue5->Bone(cached.skeletal_mesh, 2));
				fvector2d LeftThigh = ue5->w2s(ue5->Bone(cached.skeletal_mesh, 79));
				fvector2d RightThigh = ue5->w2s(ue5->Bone(cached.skeletal_mesh, 72));
				fvector2d LeftLeg = ue5->w2s(ue5->Bone(cached.skeletal_mesh, 78));
				fvector2d RightLeg = ue5->w2s(ue5->Bone(cached.skeletal_mesh, 71));
				fvector2d LeftFoot = ue5->w2s(ue5->Bone(cached.skeletal_mesh, 80));
				fvector2d RightFoot = ue5->w2s(ue5->Bone(cached.skeletal_mesh, 73));
				fvector2d LeftFeet = ue5->w2s(ue5->Bone(cached.skeletal_mesh, 82));
				fvector2d RightFeet = ue5->w2s(ue5->Bone(cached.skeletal_mesh, 75));
				fvector2d LeftShoulder = ue5->w2s(ue5->Bone(cached.skeletal_mesh, 38));
				fvector2d RightShoulder = ue5->w2s(ue5->Bone(cached.skeletal_mesh, 9));
				fvector2d LeftElbow = ue5->w2s(ue5->Bone(cached.skeletal_mesh, 39));
				fvector2d RightElbow = ue5->w2s(ue5->Bone(cached.skeletal_mesh, 10));
				fvector2d LeftHand = ue5->w2s(ue5->Bone(cached.skeletal_mesh, 91));
				fvector2d RightHand = ue5->w2s(ue5->Bone(cached.skeletal_mesh, 33));

				g_draw->line(Neck, Chest, g_skeleton_thickness, visibleColor, g_skel_outline);
				g_draw->line(Chest, RightShoulder, g_skeleton_thickness, visibleColor, g_skel_outline);
				g_draw->line(Chest, LeftShoulder, g_skeleton_thickness, visibleColor, g_skel_outline);
				g_draw->line(LeftShoulder, LeftElbow, g_skeleton_thickness, visibleColor, g_skel_outline);
				g_draw->line(RightShoulder, RightElbow, g_skeleton_thickness, visibleColor, g_skel_outline);
				g_draw->line(RightElbow, RightHand, g_skeleton_thickness, visibleColor, g_skel_outline);
				g_draw->line(LeftElbow, LeftHand, g_skeleton_thickness, visibleColor, g_skel_outline);
				g_draw->line(Chest, Pelvis, g_skeleton_thickness, visibleColor, g_skel_outline);
				g_draw->line(Pelvis, LeftLeg, g_skeleton_thickness, visibleColor, g_skel_outline);
				g_draw->line(Pelvis, RightLeg, g_skeleton_thickness, visibleColor, g_skel_outline);
				g_draw->line(RightLeg, RightThigh, g_skeleton_thickness, visibleColor, g_skel_outline);
				g_draw->line(LeftLeg, LeftThigh, g_skeleton_thickness, visibleColor, g_skel_outline);
				g_draw->line(LeftThigh, LeftFoot, g_skeleton_thickness, visibleColor, g_skel_outline);
				g_draw->line(RightThigh, RightFoot, g_skeleton_thickness, visibleColor, g_skel_outline);
				g_draw->line(RightFoot, RightFeet, g_skeleton_thickness, visibleColor, g_skel_outline);
				g_draw->line(LeftFoot, LeftFeet, g_skeleton_thickness, visibleColor, g_skel_outline);
			}
		}

		

		if (target_entity && g_aimbot) {

			auto closest_mesh = Read <std::uint64_t>(target_entity + offset::skeletal_mesh);

			fvector hitbox;
			fvector2d hitbox_screen;

			switch (g_hitbox) {
			case 0:
				hitbox_screen = ue5->w2s(ue5->Bone(closest_mesh, bone::Head));
				hitbox = ue5->Bone(closest_mesh, bone::Head);;
				break;
			case 1:
				hitbox_screen = ue5->w2s(ue5->Bone(closest_mesh, bone::Neck));
				hitbox = ue5->Bone(closest_mesh, bone::Neck);;
				break;
			case 2:
				hitbox_screen = ue5->w2s(ue5->Bone(closest_mesh, bone::Chest));
				hitbox = ue5->Bone(closest_mesh, bone::Chest);;
				break;
			case 3:

				std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));

				std::uniform_int_distribution<int> distribution(0, 3);

				int randomHitbox = distribution(rng);

				switch (randomHitbox) 
				{

				case 0:

					hitbox_screen = ue5->w2s(ue5->Bone(closest_mesh, bone::Neck));
					hitbox = ue5->Bone(closest_mesh, bone::Neck);

					break;
				case 1:

					hitbox_screen = ue5->w2s(ue5->Bone(closest_mesh, bone::Chest));

					hitbox = ue5->Bone(closest_mesh, bone::Chest);

					break;
				case 2:

					hitbox_screen = ue5->w2s(ue5->Bone(closest_mesh, bone::RShoulder));

					hitbox = ue5->Bone(closest_mesh, bone::RShoulder);

					break;
				}
			}

			float projectileSpeed = 0;
			float projectileGravityScale = 0;

			if (g_prediction) {

				if (strstr(LocalPlayerWeapon.c_str(), (("Dragon's Breath Sniper"))) || strstr(LocalPlayerWeapon.c_str(), (("Storm Scout"))) ||
					strstr(LocalPlayerWeapon.c_str(), (("Storm Scout Sniper Rifle"))) || strstr(LocalPlayerWeapon.c_str(), (("Hunting Rifle"))) ||
					strstr(LocalPlayerWeapon.c_str(), (("Explosive Repeater Rifle"))) || strstr(LocalPlayerWeapon.c_str(), (("Bolt-Action Sniper Rifle"))) ||
					strstr(LocalPlayerWeapon.c_str(), (("Suppressed Sniper Rifle"))) || strstr(LocalPlayerWeapon.c_str(), (("Lever Action Rifle"))),
					strstr(LocalPlayerWeapon.c_str(), (("Boom Sniper Rifle")))) {

					projectileSpeed = 30000.f;
					projectileGravityScale = 0.12f;
				}
				else if (strstr(LocalPlayerWeapon.c_str(), (("Heavy Sniper Rifle"))) || strstr(LocalPlayerWeapon.c_str(), (("Hunter Bolt-Action Sniper")))) {
					projectileSpeed = 45000.f;
					projectileGravityScale = 0.12f;
				}
				else if (strstr(LocalPlayerWeapon.c_str(), (("Huntress DMR")))) {
					projectileSpeed = 88000.f;
					projectileGravityScale = 2.5f;
				}
				else if (strstr(LocalPlayerWeapon.c_str(), (("Cobra DMR"))) || strstr(LocalPlayerWeapon.c_str(), (("DMR"))) || strstr(LocalPlayerWeapon.c_str(), (("Thermal DMR")))) {
					projectileSpeed = 53000.f;
					projectileGravityScale = 0.15f;
				}
				else if (strstr(LocalPlayerWeapon.c_str(), (("Automatic Sniper Rifle")))) {
					projectileSpeed = 50000.f;
					projectileGravityScale = 0.12f;
				}
				else if (strstr(LocalPlayerWeapon.c_str(), (("Reaper Sniper Rifle"))))
				{
					projectileSpeed = 60000;
					projectileGravityScale = 3.5;
				}
				else if (strstr(LocalPlayerWeapon.c_str(), (("Nemesis AR"))))
				{
					projectileSpeed = 80000;
					projectileGravityScale = 3.5;
				}
				else if (strstr(LocalPlayerWeapon.c_str(), (("Warforged Assault Rifle AR"))))
				{
					projectileSpeed = 80000;
					projectileGravityScale = 3.5;
				}
				else if (strstr(LocalPlayerWeapon.c_str(), (("Striker AR"))))
				{
					projectileSpeed = 80000;
					projectileGravityScale = 3.5;
				}
				else if (strstr(LocalPlayerWeapon.c_str(), (("Harbinger SMG"))) || strstr(LocalPlayerWeapon.c_str(), (("Thunder Bust SMG"))))
				{
					projectileSpeed = 80000;
					projectileGravityScale = 3.5;
				}
				else if (strstr(LocalPlayerWeapon.c_str(), (("SMG"))))
				{
					projectileSpeed = 70000;
					projectileGravityScale = 3;
				}
				else if (strstr(LocalPlayerWeapon.c_str(), (("Huntress DMR"))))
				{
					projectileSpeed = 88000;
					projectileGravityScale = 3;
				}
				else if (strstr(LocalPlayerWeapon.c_str(), (("Pistol"))))
				{
					projectileSpeed = 60000;
					projectileGravityScale = 2.0;
				}
				else {
					projectileSpeed = 0;
					projectileGravityScale = 0;
				}
			}
			int bone;
			if (projectileSpeed) {
				bone = bone::Head;
			}

			auto root = Read<uintptr_t>(target_entity + offset::root_component);
			auto Distance = camera_postion.location.distance(hitbox);
			fvector Velocity = Read<fvector>(root + offset::velocity);

			if (projectileSpeed) {
				hitbox = PredictLocation(hitbox, Velocity, projectileSpeed, projectileGravityScale, Distance); //dmr
			}

			 hitbox_screen = ue5->w2s(hitbox);

			if (hitbox.x != 0 || hitbox.y != 0 && (get_cross_distance(hitbox.x, hitbox.y, screen_width / 2, screen_height / 2) <= g_aimfov))
			{
				if (ue5->is_visible(closest_mesh))
				{
					if (g_target_line)
						draw_list->AddLine(ImVec2(centerWidth, centerHeight), ImVec2(hitbox_screen.x, hitbox_screen.y), ImColor(0, 0, 0), 1.5);

					if (GetAsyncKeyState(aimkey))
						input->move(hitbox_screen);
				}
			}
		}
		else {
			target_dist = FLT_MAX;
			target_entity = NULL;
		}

		ImGui::PopFont();
	}
};
 static g_fn* m_main = new g_fn();