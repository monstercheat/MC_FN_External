#include "m_cfg.hpp"

class c_render {
public:
    auto imgui() -> bool {

        DXGI_SWAP_CHAIN_DESC swap_chain_description;
        ZeroMemory(&swap_chain_description, sizeof(swap_chain_description));
        swap_chain_description.BufferCount = 2;
        swap_chain_description.BufferDesc.Width = 0;
        swap_chain_description.BufferDesc.Height = 0;
        swap_chain_description.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swap_chain_description.BufferDesc.RefreshRate.Numerator = 60;
        swap_chain_description.BufferDesc.RefreshRate.Denominator = 1;
        swap_chain_description.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        swap_chain_description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swap_chain_description.OutputWindow = window_handle;
        swap_chain_description.SampleDesc.Count = 1;
        swap_chain_description.SampleDesc.Quality = 0;
        swap_chain_description.Windowed = 1;
        swap_chain_description.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        D3D_FEATURE_LEVEL d3d_feature_lvl;

        const D3D_FEATURE_LEVEL d3d_feature_array[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

        D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, d3d_feature_array, 2, D3D11_SDK_VERSION, &swap_chain_description, &d3d_swap_chain, &d3d_device, &d3d_feature_lvl, &d3d_device_ctx);

        ID3D11Texture2D* pBackBuffer;
      

        d3d_swap_chain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

        d3d_device->CreateRenderTargetView(pBackBuffer, NULL, &d3d_render_target);

        pBackBuffer->Release();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

        static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
        ImFontConfig icons_config;

        ImFontConfig CustomFont;
        CustomFont.FontDataOwnedByAtlas = false;

        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        icons_config.OversampleH = 2.5;
        icons_config.OversampleV = 2.5;

        ImGui_ImplWin32_Init(window_handle);
        ImGui_ImplDX11_Init(d3d_device, d3d_device_ctx);

        ImFontConfig font_config;
        font_config.OversampleH = 1;
        font_config.OversampleV = 1;
        font_config.PixelSnapH = 1;

        static const ImWchar ranges[] =
        {
            0x0020, 0x00FF,
            0x0400, 0x044F,
            0,
        };

        d3d_swap_chain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

        d3d_device->CreateRenderTargetView(pBackBuffer, NULL, &d3d_render_target);


        pBackBuffer->Release();

        IMGUI_CHECKVERSION();

        ImGui::CreateContext();


        io.IniFilename = NULL;

        (void)io;

        MenuFont = io.Fonts->AddFontFromFileTTF(("C:\\Windows\\Fonts\\Arial.ttf"), 12.f);
        Arial = io.Fonts->AddFontFromFileTTF(("C:\\Windows\\Fonts\\Arial.ttf"), 15.f);
        Verdana = io.Fonts->AddFontFromFileTTF(("C:\\Windows\\Fonts\\Verdana.ttf"), 15.f); //cour.ttf
        proggy = io.Fonts->AddFontFromMemoryTTF(proggyclean, sizeof(proggyclean), 15.f);
        FortniteFont = io.Fonts->AddFontFromMemoryTTF(fortnite, sizeof(fortnite), 15.f);

        ImGui_ImplWin32_Init(window_handle);

        ImGui_ImplDX11_Init(d3d_device, d3d_device_ctx);

        d3d_device->Release();

        return true;
    }

    auto hijack() -> bool {
        window_handle = FindWindowA(("MedalOverlayClass"), ("MedalOverlay"));
        if (!window_handle) return false;

        DwmExtendFrameIntoClientArea(window_handle, &margin);
        SetWindowPos(window_handle, 0, 0, 0, screen_width, screen_height, 0);
        return true;
    }

    auto menu() -> void {

        if (g_render_fov) {
            ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(screen_width / 2, screen_height / 2), g_aimfov, ImColor(g_fov_color[0], g_fov_color[1], g_fov_color[2]), 64, 1);
        }

        struct tab
        {
            const char* tab_name;
            int tab_id;
        };

        std::vector<tab> tabs{ {"aimbot", 0}, {"weapons", 1}, {"visuals", 2}, {"world", 3}, {"misc", 4} , {"adjustment", 5} , {"configs", 6} , {"colors", 7} , {"info", 8} };
        static int current_tab = 0;

        if (GetAsyncKeyState(VK_INSERT) & 1)
            g_render_menu = !g_render_menu;

        if (g_render_menu) {


            ImGui::SetNextWindowPos(ImVec2(window_pos.x, window_pos.y), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(window_size.x, window_size.y), ImGuiCond_Once);
            ImGui::SetNextWindowBgAlpha(1.0f);

            //ImGui::PushStyleColor(ImGuiCol_CheckMark, AccentColor.x());
            ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(AccentColor.r, AccentColor.g, AccentColor.b, AccentColor.a));

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, AccentColor.x());

            ImGui::PushStyleColor(ImGuiCol_SliderGrab, AccentColor.x());
            ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, AccentColor.x());
            ImGui::PushStyleColor(ImGuiCol_Header, AccentColor.x());
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, AccentColor.x());
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, AccentColor.x());

            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
            
            ImGui::Begin("MC FN", NULL, window_flags);
            {
                ImGui::PushFont(FortniteFont);
                auto size = ImGui::GetWindowSize();

                ImGui::BeginChild("tabs", ImVec2(85, size.y - 15), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

                ImGui::PushFontShadow(IM_COL32(0, 0, 0, 255));

                ImGui::PushStyleColor(ImGuiCol_Text, AccentColor.x());

                //AlignForWidth(ImGui::CalcTextSize("// UCHIHA").x);
                //ImGui::Text("// UCHIHA");    
                
                //AlignForWidth(ImGui::CalcTextSize("// Crimson").x);
                //ImGui::Text("// Crimson");  
                
                //AlignForWidth(ImGui::CalcTextSize("// Thunder").x);
                //ImGui::Text("// Thunder"); 

                AlignForWidth(ImGui::CalcTextSize("MC FN").x);
                ImGui::Text("MC FN");

                ImGui::PopStyleColor();

                ImGui::Spacing();

                for (const auto& tab : tabs)
                {
                    bool tmp = current_tab == tab.tab_id;
                    if (ImGui::Selectable(tab.tab_name, &tmp)) current_tab = tab.tab_id;
                }

                ImGui::SetCursorPos(ImVec2(5, ImGui::GetWindowSize().y - 16));

                ImGui::Text(Config::Username.c_str());

                ImGui::PopFontShadow();

                ImGui::EndChild();

                ImGui::SameLine();

                ImGui::BeginChild("content", ImVec2((size.x - 85) - 26, size.y - 15), true);
                switch (current_tab)
                {
                case 0: // aim
                {
                    ImGui::Checkbox("enable aimbot", &g_aimbot);
                    ImGui::SameLine();
                    HotkeyButton(aimkey, ChangeKey, keystatus);
                    ImGui::Checkbox("enable prediction", &g_prediction);            
                   

                    ImGui::Combo(("aimbot hitbox"), &g_hitbox, (" head\0\ neck\0\ chest\0\ random"));
                    ImGui::SliderFloat("aimbot fov", &g_aimfov, 10.f, 500.f);
                    ImGui::SliderFloat("smoothing", &g_smooth, 1.f, 20.f);
                    ImGui::Checkbox("draw fov", &g_render_fov);
                    ImGui::Checkbox("visible check", &g_visible_check);
                    ImGui::Checkbox("target line", &g_target_line);
                    ImGui::ColorEdit3("fov color",g_fov_color);
                    

                    break;
                }
                case 1: // weapon cfg
                {
                    ImGui::Checkbox("enable weapon cfg", &g_weapon);
                    if (g_weapon)
                    {
                        if (ImGui::Button("rifle", ImVec2(52, 25)))
                            weaponconfigsubs = 0;
                        ImGui::SameLine();
                        if (ImGui::Button("pump", ImVec2(52, 25)))
                            weaponconfigsubs = 1;
                        ImGui::SameLine();
                        if (ImGui::Button("smg", ImVec2(52, 25)))
                            weaponconfigsubs = 2;
                        ImGui::SameLine();
                        if (ImGui::Button("sniper", ImVec2(52, 25)))
                            weaponconfigsubs = 3;

                        if (weaponconfigsubs == 0)
                        {
                            ImGui::Combo(("rifle bone"), &rifle::hitbox, (" head\0\ neck\0\ chest\0\ random"));
                            ImGui::Checkbox("fov", &rifle::render_fov);
                            ImGui::SliderFloat("rifle fov", &rifle::aim_fov, 0.f, 300.f);
                            ImGui::SliderFloat("rifle smooth", &rifle::smooth, 0.f, 15.f);
                            ImGui::Checkbox("target line", &rifle::target_line);
                            HotkeyButton(rifle::aimkey, ChangeKey1, keystatus1);
                        }

                        if (weaponconfigsubs == 1)
                        {
                            ImGui::Combo(("shotgun bone"), &shotgun::hitbox, (" head\0\ neck\0\ chest\0\ random"));
                            ImGui::Checkbox("fov", &shotgun::render_fov);
                            ImGui::SliderFloat("shotgun fov", &shotgun::aim_fov, 0.f, 300.f);
                            ImGui::SliderFloat("shotgun smooth", &shotgun::smooth, 0.f, 15.f);
                            ImGui::Checkbox("target line", &shotgun::target_line);
                            HotkeyButton(shotgun::aimkey, ChangeKey2, keystatus2);
                        }

                        if (weaponconfigsubs == 2)
                        {
                            ImGui::Combo(("smg bone"), &smg::hitbox, (" head\0\ neck\0\ chest\0\ random"));
                            ImGui::Checkbox("fov", &smg::render_fov);
                            ImGui::SliderFloat("smg fov", &smg::aim_fov, 0.f, 300.f);
                            ImGui::SliderFloat("smg smooth", &smg::smooth, 0.f, 15.f);
                            ImGui::Checkbox("target line", &smg::target_line);
                            HotkeyButton(smg::aimkey, ChangeKey3, keystatus3);
                        }

                        if (weaponconfigsubs == 3)
                        {
                            ImGui::Combo(("sniper bone"), &sniper::hitbox, (" head\0\ neck\0\ chest\0\ random"));
                            ImGui::Checkbox("fov", &sniper::render_fov);
                            ImGui::SliderFloat("sniper fov", &sniper::aim_fov, 0.f, 300.f);
                            ImGui::SliderFloat("sniper smooth", &sniper::smooth, 0.f, 15.f);
                            ImGui::Checkbox("target line", &sniper::target_line);
                            HotkeyButton(sniper::aimkey, ChangeKey8, keystatus10);
                        }
                    }
                    break;
                }
                case 2: // loot esp
                {
                    ImGui::Checkbox("enable", &g_enable_esp);
                    ImGui::Checkbox("box", &g_box_esp);
                    if (g_box_esp)
                    {
                        ImGui::SameLine();  ImGui::Combo(("box"), &g_box_type, (" 2d\0\ 3d\0\ rounded\0\ cornered\0\ bounding\0"));
                    }
                    else
                    {
                    }
                    ImGui::Checkbox("line", &g_line);
                    if (g_line)
                    {
                        ImGui::SameLine();  ImGui::Combo(("line"), &g_line_type, (" top\0\ middle\0\ bottom\0"));
                    }
                    else
                    {
                    }
                    ImGui::Checkbox("username + kills", &g_username);
                    ImGui::Checkbox("distance", &g_distance);
                    ImGui::Checkbox("skeleton", &g_skeleton);
                    ImGui::Checkbox("platform", &g_platform);
                    ImGui::Checkbox("weapon + ammo", &g_weapon_esp);
                    ImGui::Checkbox("teamid", &g_teamid);
                    ImGui::SameLine();
                    ImGui::TextDisabled("?");

                    if (ImGui::IsItemHovered()) {
                        ImGui::SetTooltip("works only when username enabled");
                    }
                    ImGui::SliderFloat("render distance", &g_render_distance, 0, 275, "%.1f");
                    break;
                }
                case 3: // loot
                {
                    ImGui::Checkbox("enable", &lootesp);
                    ImGui::Checkbox("pickups", &pickups);
                    ImGui::SameLine();
                    ImGui::TextDisabled("?");

                    if (ImGui::IsItemHovered()) {
                        ImGui::SetTooltip("crashes cheat sometimes");
                    }

                    ImGui::Checkbox("chests", &draw_chests);
                    ImGui::Checkbox("ammobox", &draw_ammo);
                    ImGui::Checkbox("cooler container", &draw_cooler);
                    ImGui::Checkbox("vehicles", &draw_vehicles);
                    ImGui::Checkbox("llama", &lamma);
                    ImGui::Checkbox("supply drop", &supplydrop);
                    ImGui::Checkbox("chicken", &chicken_draw);
                    ImGui::Checkbox("developer", &g_developer);
                    ImGui::SliderInt("loot distance", &pickupsrender, 1, 180);
                    ImGui::SliderInt("supplydrop distance", &supplydrop_distance, 1, 180);
                    ImGui::SliderInt("chests distance", &chestrender, 1, 180);
                    ImGui::SliderInt("ammobox distance", &ammo_render, 1, 180);
                    ImGui::SliderInt("vehicles distance", &vehiclesrender, 1, 180);
                    ImGui::SliderInt("cooler container distance", &cooler_render, 1, 180);
                    ImGui::SliderInt("llama distance", &lammarender, 1, 180);
                    ImGui::SliderInt("chicken distance", &mcnugget_distance, 1, 180);
                    break;
                }
                case 4: // adjustment 
                {
                    ImGui::Checkbox("font", &g_font);
                    if (g_font)
                    {
                        ImGui::SameLine();  ImGui::Combo(("type"), &g_font_type, (" default\0\ fortnite \0\ verdana \0\ arial \0"));
                    }
                    ImGui::Checkbox("outline", &g_outline);
                    ImGui::Checkbox("loot snapline", &g_loot_snapline);
                    ImGui::Checkbox("weapon rarity", &g_weapon_rarity);
                    ImGui::Checkbox("skeleton outline", &g_skel_outline);
                    ImGui::Checkbox("ignore dbno", &g_bIgnoreDead);
                    ImGui::Checkbox("ignore staggered", &g_bIsStaggered);
                    ImGui::Checkbox("ignore invulnerable", &g_bIsInvulnerable);
                    ImGui::Checkbox("framerate", &g_framerate);
                    ImGui::Checkbox("performance mode", &g_performance);
                    ImGui::SameLine();
                    ImGui::TextDisabled("?");

                    if (ImGui::IsItemHovered()) {
                        ImGui::SetTooltip("renders people only in ur fov");
                    }
                    ImGui::PopFlagged();
                    break;
                }
                case 5: //adjustment
                {
                    ImGui::SliderFloat("box thick", &g_box_thick, 1, 5);
                    ImGui::SliderInt("line thick", &g_line_thick, 1, 5);
                    ImGui::SliderFloat("box width", &g_width, 0, 1);
                    ImGui::SliderFloat("skeleton thick", &g_skeleton_thickness, 1, 5);
              
                    break;
                }
                case 6: //configs

                    if (ImGui::Button("Save Config", ImVec2(100, 25)))
                    {
                        SaveConfig();
                    }
                    if (ImGui::Button("Load Config", ImVec2(100, 25)))
                    {
                        LoadConfig();
                    }                 
                    break;
                case 7: //colors
                {
                    ImGui::ColorEdit3("vis color", g_color_Visible);
                    ImGui::ColorEdit3("inv color", g_color_invisible);
                    ImGui::ColorEdit3("distance color", g_distance_color);
                    ImGui::ColorEdit3("username color", g_usernamecolor);
                    ImGui::ColorEdit3("platform color", g_platform_color);
                    break;
                }
                case 8: //info
                {
                    ImGui::TextColored(ImColor(254, 236, 0), "battleye ->");
                    ImGui::SameLine();
                    ImGui::TextColored(ImColor(211, 211, 211), "undetected");
                    ImGui::TextColored(ImColor(43, 184, 230), "easyanticheat ->");
                    ImGui::SameLine();
                    //ImGui::TextColored(ImColor(255, 165, 0), "under development");
                    ImGui::TextColored(ImColor(211, 211, 211), "undetected");
                    ImGui::TextColored(ImColor(211, 211, 211), "build date -> 10:40PM 3/07/2024");
                    break;
                }
                    
                }
                ImGui::EndChild();
            }
            ImGui::End();

            ImGui::PopStyleColor(3);
        }
    }

    auto draw() -> void {

        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 0.f);

        ImGui::StyleColorsDark();

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();


        m_main->actor_loop();

        floot->LevelDrawing();
           
        menu();

        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        d3d_device_ctx->OMSetRenderTargets(1, &d3d_render_target, nullptr);
        d3d_device_ctx->ClearRenderTargetView(d3d_render_target, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        d3d_swap_chain->Present(1, 0);
    }

    auto render() -> bool {

        static RECT rect_og;
        MSG msg = { NULL };
        ZeroMemory(&msg, sizeof(MSG));

        while (msg.message != WM_QUIT)
        {
            UpdateWindow(window_handle);
            ShowWindow(window_handle, SW_SHOW);

            if (PeekMessageA(&msg, window_handle, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

            ImGuiIO& io = ImGui::GetIO();
            io.ImeWindowHandle = window_handle;
            io.DeltaTime = 1.0f / 60.0f;


            POINT p_cursor;
            GetCursorPos(&p_cursor);
            io.MousePos.x = p_cursor.x;
            io.MousePos.y = p_cursor.y;

            if (GetAsyncKeyState(VK_LBUTTON)) {
                io.MouseDown[0] = true;
                io.MouseClicked[0] = true;
                io.MouseClickedPos[0].x = io.MousePos.x;
                io.MouseClickedPos[0].x = io.MousePos.y;
            }
            else
                io.MouseDown[0] = false;
            draw();
        }
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        DestroyWindow(window_handle);

        return true;
    }



     auto render_call() -> void {

        hijack();
        Beep(500, 500);
        // MessageBoxA(NULL, "Successfull!", "SMV", MB_OK);
        imgui();
        render();
     }
};
static c_render* m_render = new c_render();