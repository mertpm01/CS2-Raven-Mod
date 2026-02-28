#include "config.hpp"

namespace config {
	bool read() {
		if (!updater::file_good(file_path)) {
			save();
			return false;
		}

		std::ifstream f(file_path);

		json data;
		try {
			data = json::parse(f);
		}
		catch (const std::exception& e) {
			save();
		}

		if (data.empty())
			return false;

		if (data["show_box_esp"].is_boolean())
			show_box_esp = data["show_box_esp"];
		if (data["show_3d_box_esp"].is_boolean())
			show_3d_box_esp = data["show_3d_box_esp"];
		if (data["show_skeleton_esp"].is_boolean())
			show_skeleton_esp = data["show_skeleton_esp"];
		if (data["show_head_tracker"].is_boolean())
			show_head_tracker = data["show_head_tracker"];
		if (data["team_esp"].is_boolean())
			team_esp = data["team_esp"];
		if (data["automatic_update"].is_boolean())
			automatic_update = data["automatic_update"];
		if (data["render_distance"].is_number())
			render_distance = data["render_distance"];
		if (data["flag_render_distance"].is_number())
			flag_render_distance = data["flag_render_distance"];
		if (data["show_extra_flags"].is_boolean())
			show_extra_flags = data["show_extra_flags"];

		// Misc settings
		if (data["show_fps"].is_boolean())
			show_fps = data["show_fps"];
		if (data["show_crosshair"].is_boolean())
			show_crosshair = data["show_crosshair"];
		if (data["show_bomb_timer"].is_boolean())
			show_bomb_timer = data["show_bomb_timer"];
		if (data["show_grenade_prediction"].is_boolean())
			show_grenade_prediction = data["show_grenade_prediction"];

		// Aimbot settings
		if (data["aimbot_enabled"].is_boolean())
			aimbot_enabled = data["aimbot_enabled"];
		if (data["aimbot_team_check"].is_boolean())
			aimbot_team_check = data["aimbot_team_check"];
		if (data["aimbot_deathmatch_mode"].is_boolean())
			aimbot_deathmatch_mode = data["aimbot_deathmatch_mode"];
		if (data["aimbot_humanizer"].is_boolean())
			aimbot_humanizer = data["aimbot_humanizer"];
		if (data["aimbot_fov"].is_number())
			aimbot_fov = data["aimbot_fov"];
		if (data["aimbot_smooth"].is_number())
			aimbot_smooth = data["aimbot_smooth"];
		if (data["aimbot_key"].is_number())
			aimbot_key = data["aimbot_key"];
		if (data["aimbot_visible_check"].is_boolean())
			aimbot_visible_check = data["aimbot_visible_check"];
		if (data["aimbot_predict_movement"].is_boolean())
			aimbot_predict_movement = data["aimbot_predict_movement"];
		if (data["aimbot_target"].is_number())
			aimbot_target = data["aimbot_target"];
		if (data["aimbot_wall_check"].is_boolean())
			aimbot_wall_check = data["aimbot_wall_check"];
		if (data["rcs_enabled"].is_boolean())
			rcs_enabled = data["rcs_enabled"];
		
		// Triggerbot settings
		if (data["triggerbot_enabled"].is_boolean())
			triggerbot_enabled = data["triggerbot_enabled"];
		if (data["triggerbot_team_check"].is_boolean())
			triggerbot_team_check = data["triggerbot_team_check"];
		if (data["triggerbot_delay"].is_number())
			triggerbot_delay = data["triggerbot_delay"];
		if (data["triggerbot_key"].is_number())
			triggerbot_key = data["triggerbot_key"];

		if (data.find("esp_box_color_team") != data.end()) {
			esp_box_color_team = {
				data["esp_box_color_team"][0].get<int>(),
				data["esp_box_color_team"][1].get<int>(),
				data["esp_box_color_team"][2].get<int>()
			};
		}

		if (data.find("esp_box_color_enemy") != data.end()) {
			esp_box_color_enemy = {
				data["esp_box_color_enemy"][0].get<int>(),
				data["esp_box_color_enemy"][1].get<int>(),
				data["esp_box_color_enemy"][2].get<int>()
			};
		}

		if (data.find("esp_skeleton_color_team") != data.end()) {
			esp_box_color_team = {
				data["esp_skeleton_color_team"][0].get<int>(),
				data["esp_skeleton_color_team"][1].get<int>(),
				data["esp_skeleton_color_team"][2].get<int>()
			};
		}

		if (data.find("esp_skeleton_color_enemy") != data.end()) {
			esp_box_color_enemy = {
				data["esp_skeleton_color_enemy"][0].get<int>(),
				data["esp_skeleton_color_enemy"][1].get<int>(),
				data["esp_skeleton_color_enemy"][2].get<int>()
			};
		}

		if (data.find("esp_name_color") != data.end()) {
			esp_name_color = {
				data["esp_name_color"][0].get<int>(),
				data["esp_name_color"][1].get<int>(),
				data["esp_name_color"][2].get<int>()
			};
		}

		if (data.find("esp_distance_color") != data.end()) {
			esp_distance_color = {
				data["esp_distance_color"][0].get<int>(),
				data["esp_distance_color"][1].get<int>(),
				data["esp_distance_color"][2].get<int>()
			};
		}

		//if (data["rainbow"].is_boolean())
		//	rainbow = data["rainbow"];
		//if (data["rainbow_speed"].is_number())
		//	rainbow_speed = data["rainbow_speed"];

		return true;
	}

	void save() {
		json data;
		
		data["show_box_esp"] = show_box_esp;
		data["show_3d_box_esp"] = show_3d_box_esp;
		data["show_skeleton_esp"] = show_skeleton_esp;
		data["show_head_tracker"] = show_head_tracker;
		data["team_esp"] = team_esp;
		data["automatic_update"] = automatic_update;
		data["render_distance"] = render_distance;
		data["flag_render_distance"] = flag_render_distance;
		data["show_extra_flags"] = show_extra_flags;

		// Misc settings
		data["show_fps"] = show_fps;
		data["show_crosshair"] = show_crosshair;
		data["show_bomb_timer"] = show_bomb_timer;
		data["show_grenade_prediction"] = show_grenade_prediction;

		// Aimbot settings
		data["aimbot_enabled"] = aimbot_enabled;
		data["aimbot_team_check"] = aimbot_team_check;
		data["aimbot_deathmatch_mode"] = aimbot_deathmatch_mode;
		data["aimbot_humanizer"] = aimbot_humanizer;
		data["aimbot_fov"] = aimbot_fov;
		data["aimbot_smooth"] = aimbot_smooth;
		data["aimbot_key"] = aimbot_key;
		data["aimbot_visible_check"] = aimbot_visible_check;
		data["aimbot_predict_movement"] = aimbot_predict_movement;
		data["aimbot_target"] = aimbot_target;
		data["aimbot_wall_check"] = aimbot_wall_check;
		data["rcs_enabled"] = rcs_enabled;
		
		// Triggerbot settings
		data["triggerbot_enabled"] = triggerbot_enabled;
		data["triggerbot_team_check"] = triggerbot_team_check;
		data["triggerbot_delay"] = triggerbot_delay;
		data["triggerbot_key"] = triggerbot_key;

		data["esp_box_color_team"] = { esp_box_color_team.r, esp_box_color_team.g, esp_box_color_team.b };
		data["esp_box_color_enemy"] = { esp_box_color_enemy.r, esp_box_color_enemy.g, esp_box_color_enemy.b };
		data["esp_skeleton_color_team"] = { esp_box_color_team.r, esp_box_color_team.g, esp_box_color_team.b };
		data["esp_skeleton_color_enemy"] = { esp_box_color_enemy.r, esp_box_color_enemy.g, esp_box_color_enemy.b };
		data["esp_name_color"] = { esp_name_color.r, esp_name_color.g, esp_name_color.b };
		data["esp_distance_color"] = { esp_distance_color.r, esp_distance_color.g, esp_distance_color.b };

		//data["rainbow"] = rainbow;
		//data["rainbow_speed"] = rainbow_speed;

		std::ofstream output(file_path);
		output << std::setw(4) << data << std::endl;
		output.close();

		utils.update_console_title();
	}
}
