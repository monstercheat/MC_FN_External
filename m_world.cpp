#include "m_world.hpp"
std::mutex worlddly_mtx;

extern uint64_t GameBase;

class World
{
public:
	static const size_t BUFF_SIZE = 3024;

	static std::string GetNameFromIndex_OLD(int key) {
		uint32_t ChunkOffset = static_cast<uint32_t>(key >> 16);
		uint16_t NameOffset = static_cast<uint16_t>(key);
		uint64_t NamePoolChunk = Read<uint64_t>(GameBase + FNAMEPOOL_OFFSET + (8 * ChunkOffset) + 16) + (2 * NameOffset);
		if (!mem::is_valid(NamePoolChunk))return std::string("");

		uint16_t nameEntry = Read<uint16_t>(NamePoolChunk);
		if (!mem::is_valid(NamePoolChunk))return std::string("");


		int nameLength = nameEntry >> 6;
		char buff[BUFF_SIZE] = {};

		if (nameLength < 0 || nameLength >= BUFF_SIZE) {
			// Name length is invalid, return an empty string to prevent buffer overflow
			return std::string("");
		}

		// Adjust the read size to ensure it does not exceed the buffer limits
		size_t safeReadSize = min(static_cast<size_t>(2 * nameLength), BUFF_SIZE - 1);

		int v14 = nameLength; // r15d
		int v15; // r8d
		int v16; // ecx
		char* v17 = buff; // rdx
		int v18; // eax
		unsigned int v19; // ecx

		v15 = 0;
		v16 = 28;
		if (v14)
		{
			ReadString64((NamePoolChunk + 2), buff, 2 * nameLength);
			do
			{
				v18 = v15++;
				v19 = (v18 | 0xB000) + v16;
				v18 = v19 ^ ~*v17;
				LOBYTE(v18);
				v16 = v19 >> 2;
				*v17++ = v18;
			} while (v15 < v14);
		}
		buff[nameLength] = '\0'; // Ensure null-termination
		return std::string(buff);
	}

	static std::string GetNameFromIndex(int key) {
		try {
			uint32_t ChunkOffset = static_cast<uint32_t>(key >> 16);
			uint16_t NameOffset = static_cast<uint16_t>(key & 0xFFFF); // Ensure correct mask for lower 16 bits
			uint64_t NamePoolChunkBase = GameBase + FNAMEPOOL_OFFSET;
			uint64_t NamePoolChunk = Read<uint64_t>(NamePoolChunkBase + (8 * ChunkOffset) + 16) + (2 * NameOffset);

			if (!mem::is_valid(NamePoolChunk)) {
				return "nan";
			}

			uint16_t nameEntry = Read<uint16_t>(NamePoolChunk);
			if (!mem::is_valid(NamePoolChunk)) {
				return "nan";
			}

			int nameLength = nameEntry >> 6;
			if (nameLength <= 0 || nameLength >= BUFF_SIZE) {
				return "nan";
			}

			char buff[BUFF_SIZE] = {};
			ReadString64((NamePoolChunk + 2), (buff), static_cast<size_t>(2 * nameLength));

			// Original character processing logic, safely encapsulated
			int v15 = 0; // Current character index
			int v16 = 28; // Initial seed value for the transformation algorithm
			for (; v15 < nameLength; ++v15) {
				unsigned int v19 = (v15 | 0xB000) + v16;
				buff[v15] = (v19 ^ ~buff[v15]) & 0xFF; // Ensure char is masked correctly
				v16 = v19 >> 2;
			}

			buff[nameLength] = '\0'; // Ensure null-termination
			return std::string(buff);
		}
		catch (const std::exception& e) {
			// Handle standard exceptions
			return "nan";
		}
		catch (...) {
			// Handle non-standard exceptions
			return "nan";
		}
	}
	// Function to get a string name from FName using the index or another index if the length condition is met
    std::string fname(int key)
	{
		uint32_t ChunkOffset = (uint32_t)((int)(key) >> 16);
		uint16_t NameOffset = (uint16_t)key;

		uint64_t NamePoolChunk = Read<uint64_t>(GameBase + FNAMEPOOL_OFFSET + (8 * ChunkOffset) + 16) + (unsigned int)(2 * NameOffset);

		// If the length condition is met, get another index and use it to get the name
		if (Read<uint16_t>(NamePoolChunk) < 64)
		{
			auto a1 = Read<DWORD>(NamePoolChunk + 2);
			return GetNameFromIndex(a1);
		}
		else
		{
			return GetNameFromIndex(key);
		}
	}
}; static World* world = new World();
// Function to get a string name from a given index in the FName pool
///s//td::mutex lootMutex;

class loott
{
public:


	static auto levels_call() -> void {
		std::thread([&]() {
			for (;;)
			{
				try
				{
					CacheLevels();
				}
				catch (...) {
					// Ignore any exceptions and continue execution
				}
			}
			}).detach();
	}

	static auto CacheLevels() -> void {
		constexpr size_t max_items = 800; // Maximum number of items to limit vector growth and prevent potential buffer overflow.
		constexpr size_t max_items_count = 8000; // Maximum number of items to limit vector growth and prevent potential buffer overflow.

		for (;;) {

			if (!arrays->acknowledged_pawn) {
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
				std::cout << "invalid u acknowledged_pawn!" << std::endl;

				continue;
			}

			if (!(draw_chests || pickups || draw_ammo)) {
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
				std::cout << "invalid u world!" << std::endl;

				continue;
			}
			

			std::vector<item> mrxd;
			uintptr_t ItemLevels = Read<uintptr_t>(arrays->uworld + offset::levels);
			if (!mem::is_valid(ItemLevels)) {
				std::cout << "invalid u ItemLevels!" << std::endl;

				continue; // Skip iteration if ItemLevels is invalid
			}
			std::cout << "got ItemLevels!" << std::endl;

			DWORD itemSize = Read<DWORD>(arrays->uworld + (offset::levels + sizeof(PVOID)));
			std::cout << "item size: " << itemSize << std::endl;
			if (itemSize == NULL) {
				continue; // Skip to next item if ItemLevel is invalid
			}
			itemSize = min(itemSize, max_items); // Limit the number of items processed
	

			for (DWORD i = 0; i < itemSize; ++i) {

				if (!arrays->acknowledged_pawn) {
					std::this_thread::sleep_for(std::chrono::milliseconds(200));
					continue;
				}
				//if (!arrays->acknowledged_pawn)goto startfa;

				uintptr_t ItemLevel = Read<uintptr_t>(ItemLevels + (i * sizeof(uintptr_t)));
				if (!mem::is_valid(ItemLevel)) {
					continue; // Skip to next item if ItemLevel is invalid
				}

				DWORD itemCount = Read<DWORD>(ItemLevel + (0xA0 + sizeof(PVOID)));
				std::cout << "item count: " << itemCount << std::endl;


				if (itemCount == NULL) {
					continue; // Skip to next item if ItemLevel is invalid
				}

				itemCount = min(itemCount, max_items_count); // Limit the number of items processed

				uintptr_t ItemsPawns = Read<uintptr_t>(ItemLevel + 0xA0);
				if (!mem::is_valid(ItemsPawns)) {
					continue; // Skip if ItemsPawns is invalid
				}

				for (DWORD j = 0; j < itemCount; ++j) {
					uintptr_t CurrentItemPawn = Read<uintptr_t>(ItemsPawns + (j * sizeof(uintptr_t)));
					if (!mem::is_valid(CurrentItemPawn)) {
						continue; // Skip if CurrentItemPawn is invalid
					}

					fvector ItemPosition = GetLocation(CurrentItemPawn);
					float ItemDist = camera_postion.location.distance(ItemPosition) / 100.f;

					int ItemIndex = Read<int>(CurrentItemPawn + 0x18);
					auto CurrentItemPawnName = world->fname(ItemIndex);
					bool bIsPickup = pickups || draw_ammo || draw_chests;

					if (bIsPickup) {
						item newItem;
						newItem.Actor = CurrentItemPawn;
						newItem.Name = CurrentItemPawnName;
						newItem.isPickup = bIsPickup;
						newItem.distance = ItemDist;

						// Efficiently insert in a sorted position by distance
						auto it = std::lower_bound(mrxd.begin(), mrxd.end(), newItem, [](const item& a, const item& b) {
							return a.distance < b.distance;
							});
						mrxd.insert(it, newItem);

						// Keep the vector size limited to max_items
						if (mrxd.size() > max_items) {
							mrxd.resize(max_items); // Efficiently trim to max_items
						}
					}
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
			
			std::lock_guard<std::mutex> lock(worlddly_mtx);
			item_pawns.clear();
			item_pawns = std::move(mrxd);
		}
	}

	void LevelDrawing() {
		std::vector<item> levelListCopy;
		// Copy the list of cached items
		{
			std::lock_guard<std::mutex> lock(worlddly_mtx);
			levelListCopy = item_pawns;
		}


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

		// Iterate over each item in the copied list and draw relevant information on the screen
		for (const auto& entity : levelListCopy)
		{
			//if (!arrays->acknowledged_pawn)break;

				if (arrays->acknowledged_pawn)
				{
					// Check various conditions and draw information based on item type and distance
					if (draw_chests && strstr(entity.Name.c_str(), _xor_("Tiered_Chest").c_str()) || strstr(entity.Name.c_str(), _xor_("AlwaysSpawn_NormalChest").c_str()) || strstr(entity.Name.c_str(), _xor_("AlwaysSpawn_RareChest").c_str())) {
						if (entity.distance <= chestrender)
						{
							fvector Location = GetLocation(entity.Actor);
							fvector2d Screen = ue5->w2s(Location);
							if (ue5->in_screen(Screen))
							{
								auto is_despawning = (Read<char>(entity.Actor + 0xd92) >> 1);
								if (is_despawning) continue;

								std::string wtf2 = ("Chest [") + std::to_string((int)entity.distance) + ("m]");
								ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());

								g_draw->draw_string(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(245, 245, 0), true, true, wtf2.c_str());

								if (g_loot_snapline)
								{
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(screen_width / 2, screen_height / 2), ImVec2(Screen.x, Screen.y), ImColor(255, 255, 255), g_line_thick);
								}
							}
						}
					}

					if (draw_ammo && strstr(entity.Name.c_str(), _xor_("Tiered_Ammo").c_str())) {
						if (entity.distance <= ammo_render)
						{
							auto is_despawning = (Read<char>(entity.Actor + 0xd92) >> 1);
							if (is_despawning) continue;
							fvector Location = GetLocation(entity.Actor);
							fvector2d Screen = ue5->w2s(Location);

							if (ue5->in_screen(Screen))
							{
								std::string wtf2 = ("Ammobox [") + std::to_string((int)entity.distance) + ("m]");
								ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
								g_draw->draw_string(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(192, 192, 192), true, true, wtf2.c_str());

								if (g_loot_snapline)
								{
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(screen_width / 2, screen_height / 2), ImVec2(Screen.x, Screen.y), ImColor(255, 255, 255), g_line_thick);
								}
							}
						}
					}

					if (g_developer)
					{
						if (entity.distance <= developer_distance)
						{
							fvector Location = GetLocation(entity.Actor);
							fvector2d Screen = ue5->w2s(Location);

							if (ue5->in_screen(Screen))
							{
								std::string wtf2 = entity.Name.c_str();
								ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
								g_draw->draw_string(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(192, 192, 192), true, true, wtf2.c_str());

								if (g_loot_snapline)
								{
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(screen_width / 2, screen_height / 2), ImVec2(Screen.x, Screen.y), ImColor(255, 255, 255), g_line_thick);
								}

							}
						}
					}

					if (draw_vehicles && strstr(entity.Name.c_str(), _xor_("Valet_SportsCar_Vehicle_C").c_str())) {
						if (entity.distance <= vehiclesrender)
						{
							fvector Location = GetLocation(entity.Actor);
							fvector2d Screen = ue5->w2s(Location);

							if (ue5->in_screen(Screen))
							{
								std::string wtf2 = ("Sports Car [") + std::to_string((int)entity.distance) + ("m]");
								ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
								g_draw->draw_string(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(192, 192, 192), true, true, wtf2.c_str());

								if (g_loot_snapline)
								{
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(screen_width / 2, screen_height / 2), ImVec2(Screen.x, Screen.y), ImColor(255, 255, 255), g_line_thick);
								}
							}
						}
					}

					if (draw_vehicles && strstr(entity.Name.c_str(), _xor_("Vehicle_Hoverboard_C").c_str())) {
						if (entity.distance <= vehiclesrender)
						{
							fvector Location = GetLocation(entity.Actor);
							fvector2d Screen = ue5->w2s(Location);

							if (ue5->in_screen(Screen))
							{
								std::string wtf2 = ("Hoverboard [") + std::to_string((int)entity.distance) + ("m]");
								ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
								g_draw->draw_string(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(192, 192, 192), true, true, wtf2.c_str());

								if (g_loot_snapline)
								{
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(screen_width / 2, screen_height / 2), ImVec2(Screen.x, Screen.y), ImColor(255, 255, 255), g_line_thick);
								}
							}
						}
					}

					if (chicken_draw && strstr(entity.Name.c_str(), _xor_("NPC_Pawn_Irwin_Prey_Nug_C").c_str())) {
						if (entity.distance <= mcnugget_distance)
						{
							fvector Location = GetLocation(entity.Actor);
							fvector2d Screen = ue5->w2s(Location);

							if (ue5->in_screen(Screen))
							{
								std::string wtf2 = ("mcnugget [") + std::to_string((int)entity.distance) + ("m]");
								ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
								g_draw->draw_string(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(192, 192, 192), true, true, wtf2.c_str());

								if (g_loot_snapline)
								{
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(screen_width / 2, screen_height / 2), ImVec2(Screen.x, Screen.y), ImColor(255, 255, 0), g_line_thick);
								}
							}
						}
					}

					if (supplydrop && strstr(entity.Name.c_str(), _xor_("B_LinedNotebookSuppyDrop_Ocean_C").c_str()) || strstr(entity.Name.c_str(), _xor_("B_LinedNotebookSuppyDrop_Lake_C").c_str())) {
						if (entity.distance <= supplydrop_distance)
						{
							fvector Location = GetLocation(entity.Actor);
							fvector2d Screen = ue5->w2s(Location);

							if (ue5->in_screen(Screen))
							{
								std::string wtf2 = ("Supply Drop [") + std::to_string((int)entity.distance) + ("m]");
								ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
								g_draw->draw_string(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(192, 192, 192), true, true, wtf2.c_str());

								if (g_loot_snapline)
								{
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(screen_width / 2, screen_height / 2), ImVec2(Screen.x, Screen.y), ImColor(0, 255, 0), g_line_thick);
								}
							}
						}
					}

					if (draw_vehicles && strstr(entity.Name.c_str(), _xor_("Motorcycle_DirtBike_Vehicle_C").c_str())) {
						if (entity.distance <= vehiclesrender)
						{
							fvector Location = GetLocation(entity.Actor);
							fvector2d Screen = ue5->w2s(Location);

							if (ue5->in_screen(Screen))
							{
								std::string wtf2 = ("Motorcycle [") + std::to_string((int)entity.distance) + ("m]");
								ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
								g_draw->draw_string(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(192, 192, 192), true, true, wtf2.c_str());

								if (g_loot_snapline)
								{
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(screen_width / 2, screen_height / 2), ImVec2(Screen.x, Screen.y), ImColor(255, 255, 255), g_line_thick);
								}
							}
						}
					}

					if (draw_cooler && strstr(entity.Name.c_str(), _xor_("Cooler_Container_C").c_str()))
					{
						if (entity.distance <= cooler_render)
						{
							fvector Location = GetLocation(entity.Actor);
							fvector2d Screen = ue5->w2s(Location);

							if (ue5->in_screen(Screen))
							{
								auto is_despawning = (Read<char>(entity.Actor + 0xd92) >> 1);
								if (is_despawning) continue;
								std::string wtf2 = ("Cooler Container [") + std::to_string((int)entity.distance) + ("m]");
								ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
								g_draw->draw_string(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(186, 242, 239), true, true, wtf2.c_str());

								if (g_loot_snapline)
								{
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(screen_width / 2, screen_height / 2), ImVec2(Screen.x, Screen.y), ImColor(255, 255, 255), g_line_thick);
								}
							}
						}
					}

					if (draw_vehicles && strstr(entity.Name.c_str(), _xor_("Valet_BasicSUV_Vehicle_C").c_str()))
					{
						if (entity.distance <= vehiclesrender)
						{
							fvector Location = GetLocation(entity.Actor);
							fvector2d Screen = ue5->w2s(Location);
							if (ue5->in_screen(Screen))
							{
								std::string wtf2 = ("SUV [") + std::to_string((int)entity.distance) + ("m]");
								ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
								g_draw->draw_string(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 255, 255), true, true, wtf2.c_str());

								if (g_loot_snapline)
								{
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(screen_width / 2, screen_height / 2), ImVec2(Screen.x, Screen.y), ImColor(255, 255, 255), g_line_thick);
								}
							}
						}
					}

					if (lamma && strstr(entity.Name.c_str(), _xor_("AthenaSupplyDrop_Llama_C").c_str()))
					{
						if (entity.distance <= lammarender)
						{
							fvector Location = GetLocation(entity.Actor);
							fvector2d Screen = ue5->w2s(Location);
							if (ue5->in_screen(Screen))
							{
								std::string wtf2 = ("Lamma [") + std::to_string((int)entity.distance) + ("m]");
								ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
								g_draw->draw_string(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 0, 255), true, true, wtf2.c_str());

								if (g_loot_snapline)
								{
									ImGui::GetBackgroundDrawList()->AddLine(ImVec2(screen_width / 2, screen_height / 2), ImVec2(Screen.x, Screen.y), ImColor(255, 255, 255), g_line_thick);
								}
							}
						}
					}

					if (pickups && strstr(entity.Name.c_str(), _xor_("FortPickupAthena").c_str()) || strstr(entity.Name.c_str(), _xor_("Fort_Pickup_Creative_C").c_str()))
					{
						if (entity.distance <= pickupsrender)
						{
							auto definition = Read <uint64_t>(entity.Actor + offset::PrimaryPickupItemEntry + offset::ItemDefinition); // AFortPickup::PrimaryPickupItemEntry : FFortItemEntry::ItemDefinition -> 0x18
							if (mem::is_valid(definition))
							{
								fvector Location = GetLocation(entity.Actor);
								fvector2d Screen = ue5->w2s(Location);
								if (ue5->in_screen(Screen))
								{
									BYTE tier = Read <BYTE>(definition + offset::tier); //UFortItemDefinition::Tier
									ImColor Color, RGBAColor;
									fvector Location = GetLocation(entity.Actor);
									fvector2d ChestPosition = ue5->w2s(Location);
									uint64_t ftext_ptr = Read <uint64_t>(definition + offset::displayname);

									if (mem::is_valid(ftext_ptr))
									{

										uint64_t ftext_data = Read <uint64_t>(ftext_ptr + 0x28);

										if (!mem::is_valid(ftext_data)) continue;

										int ftext_length = Read <int>(ftext_ptr + 0x30);
										if (ftext_length > 0 && ftext_length < 50)
										{
											wchar_t* ftext_buf = new wchar_t[ftext_length];
											if (!read_array(ftext_data, ftext_buf, ftext_length * sizeof(wchar_t))) {};
											wchar_t* WeaponName = ftext_buf;
											delete[] ftext_buf;

											std::string Text = wchar_to_char(WeaponName);
											std::string wtf2 = Text + " [" + std::to_string((int)entity.distance) + ("m]");
											if (tier == 2 && (Common))
											{
												Color = ImColor(0, 255, 0);
											}
											else if ((tier == 3) && (rare))
											{
												Color = ImColor(0, 150, 255);
											}
											else if ((tier == 4) && (purple))
											{
												Color = ImColor(255, 0, 255);
											}
											else if ((tier == 5) && (gold))
											{
												Color = ImColor(255, 255, 0);
											}
											else if ((tier == 6) && (mythic))
											{
												Color = ImColor(245, 245, 0);
											}

											else if ((tier == 0) || (tier == 1) && (UnCommon))
											{
												Color = ImColor(255, 255, 255);
											}

											ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
											g_draw->draw_string(13, ChestPosition.x - (text_size.x / 2), ChestPosition.y - 18, Color, true, true, wtf2.c_str());

											if (g_loot_snapline)
											{
												ImGui::GetBackgroundDrawList()->AddLine(ImVec2(screen_width / 2, screen_height / 2), ImVec2(ChestPosition.x, ChestPosition.y), ImColor(255, 255, 255), g_line_thick);
											}
										}
									}
								}
							}
						}
					}
				}
								
		}
	}
}; static loott* floot = new loott;