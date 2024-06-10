#include "imgui/imgui.h"
#include <dxgi.h>
#include <string>
#include "m_cache.cpp"
#include "m_xor.h"
#include <D3D11.h>
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_internal.h"
#include "m_font.hpp"
#include "m_fortnite.hpp"#include <vector>
#include <sstream>
#include <string>


HWND window_handle;
HWND windows = NULL;
ID3D11Device* d3d_device;
ID3D11DeviceContext* d3d_device_ctx;
IDXGISwapChain* d3d_swap_chain;
ID3D11RenderTargetView* d3d_render_target;
D3DPRESENT_PARAMETERS d3d_present_params;
static IDXGISwapChain* pSwapChain = NULL;
DWORD window_flags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize;


MARGINS margin{ -1 };

void AlignForWidth(float width, float alignment = 0.5f)
{
    ImGuiStyle& style = ImGui::GetStyle();
    float avail = ImGui::GetContentRegionAvail().x;
    float off = (avail - width) * alignment;
    if (off > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
}

std::string ReadStringFromIni_Float(std::string file, std::string app, std::string key, int float_code)
{
    char buf[100];
    GetPrivateProfileStringA(app.c_str(), key.c_str(), _xor_("NULL").c_str(), buf, 100, file.c_str());

    if (float_code == 0)
    {
        system(_xor_("cls").c_str());
        std::cout << _xor_(" [").c_str() << _xor_("ERR - 14").c_str() << _xor_("] - Contact Support.").c_str();
        Sleep(5000);
        exit(0);
    }
    else if (float_code == 1) {

    }
    else if (float_code == 2) {

    }
    else if (float_code == 3) {

    }
    else if (float_code == 4) {
        g_aimfov = std::stof(buf);
    }
    else if (float_code == 5) {
        g_smooth = std::stof(buf);
    }
    else if (float_code == 6) {
        aimkey = std::stof(buf);
    }
    else if (float_code == 7) {
        triggerkey = std::stof(buf);
    }
    else if (float_code == 8) {
        rifle::aim_fov = std::stof(buf);
    }
    else if (float_code == 9) {
        rifle::smooth = std::stof(buf);
    }
    else if (float_code == 10) {
        shotgun::aim_fov = std::stof(buf);
    }
    else if (float_code == 11) {
        shotgun::smooth = std::stof(buf);
    }
    else if (float_code == 12) {
        smg::aim_fov = std::stof(buf);
    }
    else if (float_code == 13) {
        smg::smooth = std::stof(buf);
    }
    else if (float_code == 14) {
        sniper::aim_fov = std::stof(buf);
    }
    else if (float_code == 15) {
        sniper::smooth = std::stof(buf);
    }
    else if (float_code == 16) {
        g_render_distance = std::stof(buf);
    }
    return (std::string)buf;
}

void WriteStringToIni(std::string string, std::string file, std::string app, std::string key)
{
    WritePrivateProfileStringA(app.c_str(), key.c_str(), string.c_str(), file.c_str());
}
std::string ReadStringFromIni(std::string file, std::string app, std::string key)
{
    char buf[100];
    GetPrivateProfileStringA(app.c_str(), key.c_str(), _xor_("NULL").c_str(), buf, 100, file.c_str());
    return (std::string)buf;
}

std::string FloatArrayToString(const float* array, int size) {
    std::ostringstream oss;
    for (int i = 0; i < size; ++i) {
        oss << std::to_string(array[i]);
        if (i < size - 1) oss << ",";
    }
    return oss.str();
}


void SaveConfig()
{
    if (g_aimbot)
        WriteStringToIni(_xor_("1").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("aimbot").c_str());
    else
        WriteStringToIni(_xor_("0").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("aimbot").c_str());
    if (g_prediction)
        WriteStringToIni(_xor_("1").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("prediction").c_str());
    else
        WriteStringToIni(_xor_("0").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("prediction").c_str());
    if (g_dll)
        WriteStringToIni(_xor_("1").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("dllmethod").c_str());
    else
        WriteStringToIni(_xor_("0").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("dllmethod").c_str());
    if (g_kernel)
        WriteStringToIni(_xor_("1").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("kernelmethod").c_str());
    else
        WriteStringToIni(_xor_("0").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("kernelmethod").c_str());
    if (g_render_fov)
        WriteStringToIni(_xor_("1").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("drawfov").c_str());
    else
        WriteStringToIni(_xor_("0").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("drawfov").c_str());
    if (g_bTargetedBuild)
        WriteStringToIni(_xor_("1").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("triggerbot").c_str());
    else
        WriteStringToIni(_xor_("0").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("triggerbot").c_str());
    if (g_distance)
        WriteStringToIni(_xor_("1").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("visuals").c_str(), _xor_("distance").c_str());
    else
        WriteStringToIni(_xor_("0").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("visuals").c_str(), _xor_("distance").c_str());
    if (g_skeleton)
        WriteStringToIni(_xor_("1").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("visuals").c_str(), _xor_("skeleton").c_str());
    else
        WriteStringToIni(_xor_("0").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("visuals").c_str(), _xor_("skeleton").c_str());
    if (g_visible_check)
        WriteStringToIni(_xor_("1").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("visuals").c_str(), _xor_("Vischeck").c_str());
    else
        WriteStringToIni(_xor_("0").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("visuals").c_str(), _xor_("Vischeck").c_str());
    if (g_username)
        WriteStringToIni(_xor_("1").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("visuals").c_str(), _xor_("name").c_str());
    else
        WriteStringToIni(_xor_("0").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("visuals").c_str(), _xor_("name").c_str());
    if (g_weapon_esp)
        WriteStringToIni(_xor_("1").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("visuals").c_str(), _xor_("weapon_esp").c_str());
    else
        WriteStringToIni(_xor_("0").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("visuals").c_str(), _xor_("weapon_esp").c_str());
    if (g_weapon)
        WriteStringToIni(_xor_("1").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("weapon_cfgs").c_str());
    else
        WriteStringToIni(_xor_("0").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("weapon_cfgs").c_str());
    if (g_render_fov)
        WriteStringToIni(_xor_("1").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("misc").c_str(), _xor_("circle_fov").c_str());
    else
        WriteStringToIni(_xor_("0").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("misc").c_str(), _xor_("circle_fov").c_str());

    if (rifle::render_fov)
        WriteStringToIni(_xor_("1").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("rifle").c_str(), _xor_("rifle_show_fov").c_str());
    else
        WriteStringToIni(_xor_("0").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("rifle").c_str(), _xor_("rifle_show_fov").c_str());
    if (shotgun::render_fov)
        WriteStringToIni(_xor_("1").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("shotgun").c_str(), _xor_("shotgun_show_fov").c_str());
    else
        WriteStringToIni(_xor_("0").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("shotgun").c_str(), _xor_("shotgun_show_fov").c_str());
    if (smg::render_fov)
        WriteStringToIni(_xor_("1").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("smg").c_str(), _xor_("smg_show_fov").c_str());
    else
        WriteStringToIni(_xor_("0").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("smg").c_str(), _xor_("smg_show_fov").c_str());
    if (sniper::render_fov)
        WriteStringToIni(_xor_("1").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("sniper").c_str(), _xor_("sinper_show_fov").c_str());
    else
        WriteStringToIni(_xor_("0").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("sniper").c_str(), _xor_("sinper_show_fov").c_str());
    if (pickups)
        WriteStringToIni(_xor_("1").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("world").c_str(), _xor_("pickups").c_str());
    else
        WriteStringToIni(_xor_("0").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("world").c_str(), _xor_("pickups").c_str());
    if (draw_chests)
        WriteStringToIni(_xor_("1").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("world").c_str(), _xor_("chests").c_str());
    else
        WriteStringToIni(_xor_("0").c_str(), _xor_("C:\\smv.config").c_str(), _xor_("world").c_str(), _xor_("chests").c_str());

    WriteStringToIni(std::to_string(g_aimfov), _xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("aimfov").c_str());
    WriteStringToIni(std::to_string(g_prediction), _xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("prediction").c_str());
    WriteStringToIni(std::to_string(g_dll), _xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("dllmethod").c_str());
    WriteStringToIni(std::to_string(g_kernel), _xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("kernelmethod").c_str());
    WriteStringToIni(std::to_string(g_smooth), _xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("smooth").c_str());
    WriteStringToIni(std::to_string(aimkey), _xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("aim_key").c_str());
    WriteStringToIni(std::to_string(triggerkey), _xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("trigger_key").c_str());
    WriteStringToIni(std::to_string(rifle::aim_fov), _xor_("C:\\smv.config").c_str(), _xor_("rifle").c_str(), _xor_("rifle_fov").c_str());
    WriteStringToIni(std::to_string(rifle::smooth), _xor_("C:\\smv.config").c_str(), _xor_("rifle").c_str(), _xor_("rifle_smooth").c_str());
    WriteStringToIni(std::to_string(shotgun::aim_fov), _xor_("C:\\smv.config").c_str(), _xor_("shogtun").c_str(), _xor_("shogtun_fov").c_str());
    WriteStringToIni(std::to_string(shotgun::smooth), _xor_("C:\\smv.config").c_str(), _xor_("shogtun").c_str(), _xor_("shogtun_smooth").c_str());
    WriteStringToIni(std::to_string(smg::aim_fov), _xor_("C:\\smv.config").c_str(), _xor_("smg").c_str(), _xor_("smg_fov").c_str());
    WriteStringToIni(std::to_string(smg::smooth), _xor_("C:\\smv.config").c_str(), _xor_("smg").c_str(), _xor_("smg_smooth").c_str());
    WriteStringToIni(std::to_string(sniper::render_fov), _xor_("C:\\smv.config").c_str(), _xor_("sniper").c_str(), _xor_("sinper_show_fov").c_str());
    WriteStringToIni(std::to_string(sniper::aim_fov), _xor_("C:\\smv.config").c_str(), _xor_("sniper").c_str(), _xor_("sniper_fov").c_str());
    WriteStringToIni(std::to_string(sniper::smooth), _xor_("C:\\smv.config").c_str(), _xor_("sniper").c_str(), _xor_("sniper_smooth").c_str());
    WriteStringToIni(std::to_string(pickupsrender), _xor_("C:\\smv.config").c_str(), _xor_("world").c_str(), _xor_("pickup_dist").c_str());
    WriteStringToIni(std::to_string(chestrender), _xor_("C:\\smv.config").c_str(), _xor_("world").c_str(), _xor_("chest_dist").c_str());
    WriteStringToIni(FloatArrayToString(g_color_Visible, 4), _xor_("C:\\smv.config").c_str(), _xor_("colors").c_str(), _xor_("BoxVisible").c_str());
    WriteStringToIni(FloatArrayToString(g_color_invisible, 4), _xor_("C:\\smv.config").c_str(), _xor_("colors").c_str(), _xor_("BoxNotVisible").c_str());

}

std::vector<float> StringToFloatArray(const std::string& str) {
    std::vector<float> result;
    std::istringstream iss(str);
    std::string token;

    while (std::getline(iss, token, ',')) {
        result.push_back(std::stof(token));
    }

    return result;
}

void ReadColorsFromIni(std::string file, std::string app, std::string key, float* colorArray, int colorArraySize) {
    char buf[100];
    GetPrivateProfileStringA(app.c_str(), key.c_str(), _xor_("NULL").c_str(), buf, 100, file.c_str());

    if (strcmp(buf, _xor_("NULL").c_str()) != 0) {
        std::vector<float> colors = StringToFloatArray(buf);
        if (colors.size() == colorArraySize) {
            // Apply the loaded colors to the colorArray
            for (int i = 0; i < colorArraySize; ++i) {
                colorArray[i] = colors[i];
            }
        }
        else {
            // Handle error: Incorrect number of color components in the loaded string
            system(_xor_("cls").c_str());
            std::cout << _xor_(" [").c_str() << _xor_("ERR - 15").c_str() << _xor_("] - Incorrect number of color components.").c_str();
            Sleep(5000);
            exit(0);
        }
    }
}

void LoadConfig()
{
    if (ReadStringFromIni(_xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("aimbot").c_str()) == _xor_("1").c_str())
        g_aimbot = true;
    else
        g_aimbot = false;

    if (ReadStringFromIni(_xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("prediction").c_str()) == _xor_("1").c_str())
        g_prediction = true;
    else
        g_prediction = false;

    if (ReadStringFromIni(_xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("inputmethod").c_str()) == _xor_("1").c_str())
        g_dll = true;
    else
        g_dll = false;

    if (ReadStringFromIni(_xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("kernelmethod").c_str()) == _xor_("1").c_str())
        g_kernel = true;
    else
        g_kernel = false;

    if (ReadStringFromIni(_xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("drawfov").c_str()) == _xor_("1").c_str())
        g_render_fov = true;
    else
        g_render_fov = false;

    if (ReadStringFromIni(_xor_("C:\\smv.config").c_str(), _xor_("sniper").c_str(), _xor_("sinper_show_fov").c_str()) == _xor_("1").c_str())
        sniper::render_fov = true;
    else
        sniper::render_fov = false;


    if (ReadStringFromIni(_xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("triggerbot").c_str()) == _xor_("1").c_str())
        g_bTargetedBuild = true;
    else
        g_bTargetedBuild = false;

    if (ReadStringFromIni(_xor_("C:\\smv.config").c_str(), _xor_("visuals").c_str(), _xor_("distance").c_str()) == _xor_("1").c_str())
        g_distance = true;
    else
        g_distance = false;

    if (ReadStringFromIni(_xor_("C:\\smv.config").c_str(), _xor_("visuals").c_str(), _xor_("skeleton").c_str()) == _xor_("1").c_str())
        g_skeleton = true;
    else
        g_skeleton = false;

    if (ReadStringFromIni(_xor_("C:\\smv.config").c_str(), _xor_("visuals").c_str(), _xor_("Vischeck").c_str()) == _xor_("1").c_str())
        g_visible_check = true;
    else
        g_visible_check = false;

    if (ReadStringFromIni(_xor_("C:\\smv.config").c_str(), _xor_("visuals").c_str(), _xor_("name").c_str()) == _xor_("1").c_str())
        g_username = true;
    else
        g_username = false;

    if (ReadStringFromIni(_xor_("C:\\smv.config").c_str(), _xor_("visuals").c_str(), _xor_("weapon_esp").c_str()) == _xor_("1").c_str())
        g_weapon_esp = true;
    else
        g_weapon_esp = false;

    if (ReadStringFromIni(_xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("weapon_cfgs").c_str()) == _xor_("1").c_str())
        g_weapon = true;
    else
        g_weapon = false;

    if (ReadStringFromIni(_xor_("C:\\smv.config").c_str(), _xor_("misc").c_str(), _xor_("circle_fov").c_str()) == _xor_("1").c_str())
        g_render_fov = true;
    else
        g_render_fov = false;

    if (ReadStringFromIni(_xor_("C:\\smv.config").c_str(), _xor_("rifle").c_str(), _xor_("rifle_show_fov").c_str()) == _xor_("1").c_str())
        rifle::render_fov = true;
    else
        rifle::render_fov = false;
    if (ReadStringFromIni(_xor_("C:\\smv.config").c_str(), _xor_("shotgun").c_str(), _xor_("shotgun_show_fov").c_str()) == _xor_("1").c_str())
        shotgun::render_fov = true;
    else
        shotgun::render_fov = false;
    if (ReadStringFromIni(_xor_("C:\\smv.config").c_str(), _xor_("smg").c_str(), _xor_("smg_show_fov").c_str()) == _xor_("1").c_str())
        smg::render_fov = true;
    else
        smg::render_fov = false;
    if (ReadStringFromIni(_xor_("C:\\smv.config").c_str(), _xor_("sniper").c_str(), _xor_("sniper_show_fov").c_str()) == _xor_("1").c_str())
        sniper::render_fov = true;
    else
        sniper::render_fov = false;
    if (ReadStringFromIni(_xor_("C:\\smv.config").c_str(), _xor_("world").c_str(), _xor_("pickups").c_str()) == _xor_("1").c_str())
        pickups = true;
    else
        pickups = false;

    if (ReadStringFromIni(_xor_("C:\\smv.config").c_str(), _xor_("world").c_str(), _xor_("chests").c_str()) == _xor_("1").c_str())
        draw_chests = true;
    else
        draw_chests = false;

    ReadStringFromIni_Float(_xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("aimfov").c_str(), 4);
    g_aimfov;
    ReadStringFromIni_Float(_xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("prediction").c_str(), 4);
    g_prediction;
    ReadStringFromIni_Float(_xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("dllmethod").c_str(), 4);
    g_dll;
    ReadStringFromIni_Float(_xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("kernelmethod").c_str(), 4);
    g_kernel;
    ReadStringFromIni_Float(_xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("smooth").c_str(), 5);
    g_smooth;
    ReadStringFromIni_Float(_xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("aim_key").c_str(), 8);
    aimkey;
    ReadStringFromIni_Float(_xor_("C:\\smv.config").c_str(), _xor_("aimbot").c_str(), _xor_("trigger_key").c_str(), 9);
    triggerkey;
    ReadStringFromIni_Float(_xor_("C:\\smv.config").c_str(), _xor_("rifle").c_str(), _xor_("rifle_fov").c_str(), 10);
    rifle::aim_fov;
    ReadStringFromIni_Float(_xor_("C:\\smv.config").c_str(), _xor_("rifle").c_str(), _xor_("rifle_smooth").c_str(), 11);
    rifle::smooth;
    ReadStringFromIni_Float(_xor_("C:\\smv.config").c_str(), _xor_("shogtun").c_str(), _xor_("shogtun_fov").c_str(), 12);
    shotgun::aim_fov;
    ReadStringFromIni_Float(_xor_("C:\\smv.config").c_str(), _xor_("shogtun").c_str(), _xor_("shogtun_smooth").c_str(), 13);
    shotgun::smooth;
    ReadStringFromIni_Float(_xor_("C:\\smv.config").c_str(), _xor_("smg").c_str(), _xor_("smg_fov").c_str(), 14);
    smg::aim_fov;
    ReadStringFromIni_Float(_xor_("C:\\smv.config").c_str(), _xor_("smg").c_str(), _xor_("smg_smooth").c_str(), 15);
    smg::smooth;
    ReadStringFromIni_Float(_xor_("C:\\smv.config").c_str(), _xor_("sniper").c_str(), _xor_("sniper_fov").c_str(), 16);
    sniper::aim_fov;
    ReadStringFromIni_Float(_xor_("C:\\smv.config").c_str(), _xor_("sniper").c_str(), _xor_("sniper_smooth").c_str(), 17);
    sniper::smooth;
    ReadStringFromIni_Float(_xor_("C:\\smv.config").c_str(), _xor_("world").c_str(), _xor_("pickup_dist").c_str(), 18);
    pickupsrender;
    ReadStringFromIni_Float(_xor_("C:\\smv.config").c_str(), _xor_("world").c_str(), _xor_("chest_dist").c_str(), 21);
    chestrender;
    ReadColorsFromIni(_xor_("C:\\smv.config").c_str(), _xor_("colors").c_str(), _xor_("BoxVisible").c_str(), g_color_Visible, 4);
    ReadColorsFromIni(_xor_("C:\\smv.config").c_str(), _xor_("colors").c_str(), _xor_("BoxNotVisible").c_str(), g_color_invisible, 4);

}

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

namespace Globals
{
	inline bool showMenu = false;
	inline Present oPresent;
	inline ImGuiStyle* style;
	inline std::string window = "main";
}

namespace Config
{
	inline std::string Username;
	inline bool checkbox_State = false;
	inline int slider_Int = 2;
	inline float slider_Float = 1.5f;
	inline int slider_TripleInt[3] = { 1, 2, 3 };
	inline std::string dropdown_Selection = "0";
	inline std::string text_Input;
}

namespace ImGui
{
    inline bool custom_UseFontShadow;
    inline unsigned int custom_FontShadowColor;
    inline bool FlaggedItem = false;

    inline static void PushFontShadow(unsigned int col)
    {
        custom_UseFontShadow = true;
        custom_FontShadowColor = col;
    }

    inline static void PopFontShadow(void)
    {
        custom_UseFontShadow = false;
    }

    inline static void PushFlagged()
    {
        FlaggedItem = true;
    }

    inline static void PopFlagged()
    {
        FlaggedItem = false;
    }

};

struct Color
{
    float r, g, b, a;
    Color()
    {
        r = 1.f, g = 1.f, b = 1.f, a = 1.f;
    }
    Color(float _r, float _g, float _b, float _a)
    {
        this->r = _r;
        this->g = _g;
        this->b = _b;
        this->a = _a;
    }
    ImVec4 x()
    {
        return ImVec4(r, g, b, a);
    }
    ImU32 c()
    {
        return IM_COL32(r * 255.f, g * 255.f, b * 255.f, a * 255.f);
    }
    ImU32 c(int m)
    {
        return IM_COL32((r * 255.f) - m, (g * 255.f) - m, (b * 255.f) - m, (a * 255.f) - m);
    }
};
static int weaponconfigsubs = 0;
ImVec2 window_pos{ 0, 0 };
ImVec2 window_size{ 600, 330 }; //255, 0, 127
Color AccentColor_CRIMSON = Color{ 203 / 255.f, 114 / 255.f, 255 / 255.f, 1.f };
Color AccentColor = Color{ 255 / 255.f, 0 / 255.f, 127 / 255.f, 1.f };
// Converting hexadecimal color #a01514 to a Color object with normalized RGBA values
//Color AccentColor_UCHIHA = Color{ 0xA0 / 255.f, 0x15 / 255.f, 0x14 / 255.f, 1.0f };

//Color SecondAccentColor = Color{ 255 / 255.f, 0 / 255.f, 127 / 255.f, 1.f };
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 0.f);
