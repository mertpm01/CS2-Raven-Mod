#pragma once
#include <fstream>
#include "utils.h"
#include "json.hpp"
#include "auto_updater.hpp"

using json = nlohmann::json;

struct RGB {
	int r;
	int g;
	int b;

	// Conversion function from RGB to COLORREF
	operator COLORREF() const {
		return RGB(r, g, b);
	}
};

namespace config {
	const std::string file_path = "config.json";

	extern bool read();
	extern void save();

	inline bool automatic_update = false;
	inline bool team_esp = false;
	inline float render_distance = -1.f;
	inline int flag_render_distance = 200;
	inline bool show_box_esp = true;
	inline bool show_3d_box_esp = true; // 3D box ESP - AÇIK
	inline bool show_skeleton_esp = true; // AÇIK
	inline bool show_head_tracker = true; // AÇIK
	inline bool show_extra_flags = true; // AÇIK
	
	// Misc settings
	inline bool show_fps = true;
	inline bool show_crosshair = true; // ON
	inline bool show_bomb_timer = true;
	inline bool show_grenade_prediction = true; // Grenade/molotov trajectory

	// Aimbot settings
	inline bool aimbot_enabled = true; // AÇIK - Otomatik aktif
	inline bool aimbot_team_check = true;
	inline bool aimbot_deathmatch_mode = false; // Ölüm maçı modu - takım kontrolü yok
	inline bool aimbot_humanizer = true; // İnsanlaştırıcı - rastgele gecikme ve sapma
	inline float aimbot_fov = 50.0f; // 0-250 arası
	inline float aimbot_smooth = 3.0f; // 1-5 arası
	inline int aimbot_key = VK_XBUTTON2; // Mouse 5 button (default)
	inline bool aimbot_visible_check = true;
	inline bool aimbot_predict_movement = true; // Hareket tahmini
	inline int aimbot_target = 0; // 0=Kafa (HEADSHOT), 1=Göğüs, 2=Ayak
	inline bool aimbot_wall_check = true; // Duvar kontrolü - açıkken duvar arkasına kilitlenmez
	inline bool rcs_enabled = true; // Recoil Control System - sprey kontrolü
	
	// Triggerbot settings (ayrı özellik)
	inline bool triggerbot_enabled = true; // AÇIK - Otomatik aktif
	inline bool triggerbot_team_check = true;
	inline int triggerbot_delay = 50; // ms - ateş gecikmesi (0-500)
	inline int triggerbot_key = VK_XBUTTON1; // Mouse 4 button (default)
	
	// Aimbot target names for display
	inline const char* get_target_name(int target) {
		switch(target) {
			case 0: return "Kafa";
			case 1: return "Gogus";
			case 2: return "Ayak";
			default: return "Gogus";
		}
	}
	
	// Aimbot key names for display
	inline const char* get_key_name(int vk_code) {
		switch(vk_code) {
			case VK_LBUTTON: return "Mouse 1";
			case VK_RBUTTON: return "Mouse 2";
			case VK_MBUTTON: return "Mouse 3";
			case VK_XBUTTON1: return "Mouse 4";
			case VK_XBUTTON2: return "Mouse 5";
			case VK_SHIFT: return "Shift";
			case VK_CONTROL: return "Ctrl";
			case VK_MENU: return "Alt";
			case VK_CAPITAL: return "Caps";
			default: return "Mouse 5";
		}
	}

	inline RGB esp_box_color_team = { 75, 175, 75 };
	inline RGB esp_box_color_enemy = { 225, 75, 75 };
	inline RGB esp_skeleton_color_team = { 75, 175, 75 };
	inline RGB esp_skeleton_color_enemy = { 225, 75, 75 };
	inline RGB esp_name_color = { 250, 250, 250 };
	inline RGB esp_distance_color = { 75, 75, 175 };
}
