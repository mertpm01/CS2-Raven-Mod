#include <thread>
#include <cmath>
#include <chrono>
#include <map>
#include <random>
#include <Windows.h>
#include "reader.hpp"
#include "../classes/render.hpp"
#include "../classes/config.hpp"
#include "../classes/globals.hpp"

namespace hack {
	// Aimbot için önceki pozisyonları sakla
	std::map<uintptr_t, Vector3> previous_positions;
	std::map<uintptr_t, std::chrono::steady_clock::time_point> last_update_time;
	
	// İnsanlaştırma için
	std::chrono::steady_clock::time_point last_aim_time;
	std::mt19937 rng(std::random_device{}());
	
	// Rastgele sayı üret (insanlaştırıcı için)
	float random_float(float min, float max) {
		std::uniform_real_distribution<float> dist(min, max);
		return dist(rng);
	}

	// 3D mesafe hesapla
	float calculate_3d_distance(const Vector3& from, const Vector3& to) {
		float dx = to.x - from.x;
		float dy = to.y - from.y;
		float dz = to.z - from.z;
		return sqrtf(dx * dx + dy * dy + dz * dz);
	}
	
	// Hız tahmini - hareket eden hedeflere önden atış
	Vector3 predict_position(CPlayer* player, float bullet_speed = 3000.0f) {
		Vector3 predicted = player->origin;
		
		// Önceki pozisyon var mı?
		auto it = previous_positions.find((uintptr_t)player);
		if (it != previous_positions.end()) {
			auto time_it = last_update_time.find((uintptr_t)player);
			if (time_it != last_update_time.end()) {
				// Zaman farkını hesapla
				auto now = std::chrono::steady_clock::now();
				float dt = std::chrono::duration<float>(now - time_it->second).count();
				
				if (dt > 0.001f && dt < 1.0f) { // Geçerli zaman aralığı
					// Hızı hesapla
					Vector3 velocity;
					velocity.x = (player->origin.x - it->second.x) / dt;
					velocity.y = (player->origin.y - it->second.y) / dt;
					velocity.z = (player->origin.z - it->second.z) / dt;
					
					// Mermi uçuş süresini hesapla
					float distance = calculate_3d_distance(g_game.localOrigin, player->origin);
					float time_to_hit = distance / bullet_speed;
					
					// Gelecekteki pozisyonu tahmin et
					predicted.x += velocity.x * time_to_hit;
					predicted.y += velocity.y * time_to_hit;
					predicted.z += velocity.z * time_to_hit;
				}
			}
		}
		
		// Pozisyonu güncelle
		previous_positions[(uintptr_t)player] = player->origin;
		last_update_time[(uintptr_t)player] = std::chrono::steady_clock::now();
		
		return predicted;
	}
	
	// Hedef önceliği hesapla (düşük = daha iyi)
	float calculate_target_priority(CPlayer* player, float screen_dist, float world_dist) {
		float priority = screen_dist; // Temel: ekran mesafesi
		
		// Düşük can = daha yüksek öncelik
		priority *= (player->health / 100.0f);
		
		// Yakın mesafe = daha yüksek öncelik
		priority *= (world_dist / 5000.0f);
		
		// Flashlanmış düşman = daha yüksek öncelik
		if (player->flashAlpha > 100) {
			priority *= 0.5f;
		}
		
		return priority;
	}
	
	// Otomatik atış - KALDIRILDI (triggerbot'a taşındı)
	
	// TRIGGERBOT - nişangahın üzerinde düşman varsa otomatik ateş
	void triggerbot() {
		// Triggerbot aktif mi?
		if (!config::triggerbot_enabled)
			return;
		
		// Triggerbot tuşu basılı mı?
		if (!(GetAsyncKeyState(config::triggerbot_key) & 0x8000))
			return;
		
		// Oyun verisi hazır mı?
		if (g_game.players.empty())
			return;
		
		// Ekran merkezi
		float screen_center_x = g::gameBounds.right / 2.0f;
		float screen_center_y = g::gameBounds.bottom / 2.0f;
		
		// Nişangahın üzerinde düşman var mı kontrol et
		for (auto& player : g_game.players) {
			// Sağlık kontrolü
			if (player.health <= 0)
				continue;
			
			// Takım kontrolü
			if (config::triggerbot_team_check) {
				if (player.team == g_game.localTeam)
					continue;
			}
			
			// Oyuncunun göğüs pozisyonu
			Vector3 chest_pos;
			chest_pos.x = player.origin.x;
			chest_pos.y = player.origin.y;
			chest_pos.z = player.origin.z + 50.0f;
			
			// Ekrana çevir
			Vector3 screen_pos = g_game.world_to_screen(&chest_pos);
			
			// Ekran dışında mı?
			if (screen_pos.z < 0.01f)
				continue;
			
			// Nişangaha olan mesafe
			float dx = screen_pos.x - screen_center_x;
			float dy = screen_pos.y - screen_center_y;
			float distance = sqrtf(dx * dx + dy * dy);
			
			// Nişangahın üzerinde mi? (30 pixel içinde)
			if (distance < 30.0f) {
				// Gecikme uygula
				static auto last_trigger_time = std::chrono::steady_clock::now();
				auto now = std::chrono::steady_clock::now();
				auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_trigger_time).count();
				
				if (elapsed >= config::triggerbot_delay) {
					// Ateş et
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					Sleep(10);
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					last_trigger_time = now;
					return; // Bir düşmana ateş ettikten sonra çık
				}
			}
		}
	}

	// İYİLEŞTİRİLMİŞ AIMBOT - Daha hassas, akıllı ve pürüzsüz
	void aimbot() {
		if (!config::aimbot_enabled) return;
		if (!(GetAsyncKeyState(config::aimbot_key) & 0x8000)) return;
		if (g_game.players.empty()) return;

		auto current_time = std::chrono::steady_clock::now();
		auto time_since_last = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_aim_time).count();
		if (time_since_last < 1) return; // 1ms - çok hızlı

		float screen_center_x = g::gameBounds.right / 2.0f;
		float screen_center_y = g::gameBounds.bottom / 2.0f;

		float best_distance = 999999.0f;
		CPlayer* best_target = nullptr;
		Vector3 best_screen_pos;
		float best_world_dist = 0;

		// En yakın hedefi bul (basit ve etkili)
		for (auto& player : g_game.players) {
			if (player.health <= 0) continue;
			
			if (!config::aimbot_deathmatch_mode && player.team == g_game.localTeam) continue;

			float world_distance = calculate_3d_distance(g_game.localOrigin, player.origin);
			if (world_distance > 15000.0f) continue; // 150m max

			// Hız tahmini
			Vector3 predicted = predict_position(&player);
			
			// Hedef pozisyon
			Vector3 target_pos = predicted;
			if (config::aimbot_target == 0) target_pos.z += 65.0f; // Kafa
			else if (config::aimbot_target == 1) target_pos.z += 50.0f; // Göğüs
			else target_pos.z += 10.0f; // Ayak
			
			Vector3 screen_pos = g_game.world_to_screen(&target_pos);
			if (screen_pos.z < 0.01f) continue;

			float dx = screen_pos.x - screen_center_x;
			float dy = screen_pos.y - screen_center_y;
			float screen_dist = sqrtf(dx * dx + dy * dy);
			
			if (config::aimbot_fov > 0 && screen_dist > config::aimbot_fov) continue;
			if (config::aimbot_wall_check && !player.is_spotted) continue;

			// En yakın hedefi seç (ekran mesafesine göre)
			if (screen_dist < best_distance) {
				best_distance = screen_dist;
				best_target = &player;
				best_screen_pos = screen_pos;
				best_world_dist = world_distance;
			}
		}

		if (best_target != nullptr) {
			float dx = best_screen_pos.x - screen_center_x;
			float dy = best_screen_pos.y - screen_center_y;

			// İnsanlaştırıcı
			if (config::aimbot_humanizer) {
				dx += random_float(-0.5f, 0.5f);
				dy += random_float(-0.5f, 0.5f);
			}

			// Akıllı yumuşatma - mesafeye göre otomatik ayarlama
			float smooth = config::aimbot_smooth;
			if (best_world_dist < 3000.0f) smooth *= 1.8f; // Çok yakın
			else if (best_world_dist < 6000.0f) smooth *= 1.3f; // Yakın
			
			// Hedef hızına göre yumuşatma azalt (hızlı hedefler için)
			float target_speed = sqrtf(
				(best_target->origin.x - previous_positions[(uintptr_t)best_target].x) *
				(best_target->origin.x - previous_positions[(uintptr_t)best_target].x) +
				(best_target->origin.y - previous_positions[(uintptr_t)best_target].y) *
				(best_target->origin.y - previous_positions[(uintptr_t)best_target].y)
			);
			if (target_speed > 200.0f) smooth *= 0.8f; // Hızlı hareket eden hedef
			
			dx /= smooth;
			dy /= smooth;

			// Hassas hareket - küçük hareketlerde bile tepki ver
			if (fabs(dx) > 0.1f || fabs(dy) > 0.1f) {
				mouse_event(MOUSEEVENTF_MOVE, (LONG)dx, (LONG)dy, 0, 0);
				last_aim_time = current_time;
			}
		}
	}
	std::vector<std::pair<std::string, std::string>> boneConnections = {
						{"neck_0", "spine_1"},
						{"spine_1", "spine_2"},
						{"spine_2", "pelvis"},
						{"spine_1", "arm_upper_L"},
						{"arm_upper_L", "arm_lower_L"},
						{"arm_lower_L", "hand_L"},
						{"spine_1", "arm_upper_R"},
						{"arm_upper_R", "arm_lower_R"},
						{"arm_lower_R", "hand_R"},
						{"pelvis", "leg_upper_L"},
						{"leg_upper_L", "leg_lower_L"},
						{"leg_lower_L", "ankle_L"},
						{"pelvis", "leg_upper_R"},
						{"leg_upper_R", "leg_lower_R"},
						{"leg_lower_R", "ankle_R"}
	};

	void loop() {

		std::lock_guard<std::mutex> lock(reader_mutex);

		if (g_game.isC4Planted)
		{
			Vector3 c4Origin = g_game.c4.get_origin();
			const Vector3 c4ScreenPos = g_game.world_to_screen(&c4Origin);

			if (c4ScreenPos.z >= 0.01f) {
				float c4Distance = g_game.localOrigin.calculate_distance(c4Origin);
				float c4RoundedDistance = std::round(c4Distance / 500.f);

				float height = 10 - c4RoundedDistance;
				float width = height * 1.4f;

				render::DrawFilledBox(
					g::hdcBuffer,
					c4ScreenPos.x - (width / 2),
					c4ScreenPos.y - (height / 2),
					width,
					height,
					config::esp_box_color_enemy
				);

				render::RenderText(
					g::hdcBuffer,
					c4ScreenPos.x + (width / 2 + 5),
					c4ScreenPos.y,
					"C4",
					config::esp_name_color,
					10
				);
			}
		}

		int playerIndex = 0;
		uintptr_t list_entry;

		/**
		* Loop through all the players in the entity list
		**/
		for (auto player = g_game.players.begin(); player < g_game.players.end(); player++) {
			// Sağlık kontrolü - ölü oyuncuları atla (performans)
			if (player->health <= 0)
				continue;
			
			// Temel pozisyon hesaplamaları
			const Vector3 screenPos = g_game.world_to_screen(&player->origin);
			
			// Ekran dışı kontrolü - erken çık
			if (screenPos.z < 0.01f || !utils.is_in_bounds(screenPos, g_game.game_bounds.right, g_game.game_bounds.bottom))
				continue;
			
			// Head pozisyonu sadece gerekirse hesapla
			const Vector3 screenHead = g_game.world_to_screen(&player->head);

			// 2D box boyutları (iyileştirilmiş)
			const float height = screenPos.y - screenHead.y;
			const float width = height / 2.2f; // Daha iyi oran

			// Mesafe hesaplama - cache'le
			float distance = g_game.localOrigin.calculate_distance(player->origin);
			int roundedDistance = std::round(distance / 10.f);

			// Renk seçimi - tek seferlik
			bool isTeammate = (g_game.localTeam == player->team);
			COLORREF boxColor = isTeammate ? config::esp_box_color_team : config::esp_box_color_enemy;
			COLORREF skeletonColor = isTeammate ? config::esp_skeleton_color_team : config::esp_skeleton_color_enemy;

			// 3D BOX ESP - optimize edilmiş
			if (config::show_3d_box_esp) {
				// Sabit boyutlar
				static const float box_width = 32.0f;
				static const float box_depth = 32.0f;
				static const float box_height = 72.0f;
				
				// Köşe hesaplamaları - inline
				float hw = box_width * 0.5f;
				float hd = box_depth * 0.5f;
				float px = player->origin.x;
				float py = player->origin.y;
				float pz = player->origin.z;
				
				// 8 köşe - stack allocation
				Vector3 corners[8] = {
					{px - hw, py - hd, pz},           // 0: Sol ön alt
					{px + hw, py - hd, pz},           // 1: Sağ ön alt
					{px + hw, py + hd, pz},           // 2: Sağ arka alt
					{px - hw, py + hd, pz},           // 3: Sol arka alt
					{px - hw, py - hd, pz + box_height}, // 4: Sol ön üst
					{px + hw, py - hd, pz + box_height}, // 5: Sağ ön üst
					{px + hw, py + hd, pz + box_height}, // 6: Sağ arka üst
					{px - hw, py + hd, pz + box_height}  // 7: Sol arka üst
				};
				
				// Ekrana çevir - tek loop
				Vector3 sc[8];
				bool allVisible = true;
				for (int i = 0; i < 8; i++) {
					sc[i] = g_game.world_to_screen(&corners[i]);
					if (sc[i].z < 0.01f) {
						allVisible = false;
						break; // Erken çık
					}
				}
				
				// Tüm köşeler görünüyorsa çiz
				if (allVisible) {
					// Alt yüz (4 çizgi)
					render::DrawLine(g::hdcBuffer, sc[0].x, sc[0].y, sc[1].x, sc[1].y, boxColor);
					render::DrawLine(g::hdcBuffer, sc[1].x, sc[1].y, sc[2].x, sc[2].y, boxColor);
					render::DrawLine(g::hdcBuffer, sc[2].x, sc[2].y, sc[3].x, sc[3].y, boxColor);
					render::DrawLine(g::hdcBuffer, sc[3].x, sc[3].y, sc[0].x, sc[0].y, boxColor);
					
					// Üst yüz (4 çizgi)
					render::DrawLine(g::hdcBuffer, sc[4].x, sc[4].y, sc[5].x, sc[5].y, boxColor);
					render::DrawLine(g::hdcBuffer, sc[5].x, sc[5].y, sc[6].x, sc[6].y, boxColor);
					render::DrawLine(g::hdcBuffer, sc[6].x, sc[6].y, sc[7].x, sc[7].y, boxColor);
					render::DrawLine(g::hdcBuffer, sc[7].x, sc[7].y, sc[4].x, sc[4].y, boxColor);
					
					// Dikey kenarlar (4 çizgi)
					render::DrawLine(g::hdcBuffer, sc[0].x, sc[0].y, sc[4].x, sc[4].y, boxColor);
					render::DrawLine(g::hdcBuffer, sc[1].x, sc[1].y, sc[5].x, sc[5].y, boxColor);
					render::DrawLine(g::hdcBuffer, sc[2].x, sc[2].y, sc[6].x, sc[6].y, boxColor);
					render::DrawLine(g::hdcBuffer, sc[3].x, sc[3].y, sc[7].x, sc[7].y, boxColor);
				}
			}

			// HEAD TRACKER
			if (config::show_head_tracker) {
				render::DrawCircle(
					g::hdcBuffer,
					player->bones.bonePositions["head"].x,
					player->bones.bonePositions["head"].y - width / 12,
					width / 5,
					skeletonColor
				);
			}

			// SKELETON ESP
			if (config::show_skeleton_esp) {
				for (const auto& connection : boneConnections) {
					const std::string& boneFrom = connection.first;
					const std::string& boneTo = connection.second;

					render::DrawLine(
						g::hdcBuffer,
						player->bones.bonePositions[boneFrom].x, player->bones.bonePositions[boneFrom].y,
						player->bones.bonePositions[boneTo].x, player->bones.bonePositions[boneTo].y,
						skeletonColor
					);
				}
			}

			// 2D BOX ESP (iyileştirilmiş - daha stabil)
			if (config::show_box_esp) {
				float boxLeft = screenHead.x - width / 2;
				float boxTop = screenHead.y;
				
				// Ana kutu
				render::DrawBorderBox(
					g::hdcBuffer,
					boxLeft,
					boxTop,
					width,
					height,
					boxColor
				);
				
				// Köşe işaretleri (daha profesyonel görünüm)
				int cornerSize = width / 4;
				float boxRight = screenHead.x + width / 2;
				float boxBottom = screenPos.y;
				
				// Sol üst
				render::DrawLine(g::hdcBuffer, boxLeft, boxTop, boxLeft + cornerSize, boxTop, boxColor);
				render::DrawLine(g::hdcBuffer, boxLeft, boxTop, boxLeft, boxTop + cornerSize, boxColor);
				// Sağ üst
				render::DrawLine(g::hdcBuffer, boxRight, boxTop, boxRight - cornerSize, boxTop, boxColor);
				render::DrawLine(g::hdcBuffer, boxRight, boxTop, boxRight, boxTop + cornerSize, boxColor);
				// Sol alt
				render::DrawLine(g::hdcBuffer, boxLeft, boxBottom, boxLeft + cornerSize, boxBottom, boxColor);
				render::DrawLine(g::hdcBuffer, boxLeft, boxBottom, boxLeft, boxBottom - cornerSize, boxColor);
				// Sağ alt
				render::DrawLine(g::hdcBuffer, boxRight, boxBottom, boxRight - cornerSize, boxBottom, boxColor);
				render::DrawLine(g::hdcBuffer, boxRight, boxBottom, boxRight, boxBottom - cornerSize, boxColor);
			}

			// HEALTH BAR (sol taraf - optimize) - Sadece 2D veya 3D box aktifse göster
			if (config::show_box_esp || config::show_3d_box_esp) {
				float healthPercent = player->health * 0.01f; // /100.0f yerine
				int healthBarHeight = height * healthPercent;
				int healthBarY = screenHead.y + (height - healthBarHeight);
				float barLeft = screenHead.x - (width / 2 + 6);
				
				// Arka plan (siyah)
				render::DrawFilledBox(
					g::hdcBuffer,
					barLeft,
					screenHead.y,
					3,
					height,
					RGB(0, 0, 0)
				);
				
				// Health bar (gradient: yeşil->sarı->kırmızı)
				int healthRed = 255 - player->health * 2.55f;
				int healthGreen = player->health * 2.55f;
				render::DrawFilledBox(
					g::hdcBuffer,
					barLeft,
					healthBarY,
					3,
					healthBarHeight,
					RGB(healthRed, healthGreen, 0)
				);

				// ARMOR BAR (sol taraf - health'in yanında)
				if (player->armor > 0) {
					float armorPercent = player->armor * 0.01f;
					int armorBarHeight = height * armorPercent;
					int armorBarY = screenHead.y + (height - armorBarHeight);
					float armorLeft = screenHead.x - (width / 2 + 11);
					
					// Arka plan
					render::DrawFilledBox(
						g::hdcBuffer,
						armorLeft,
						screenHead.y,
						3,
						height,
						RGB(0, 0, 0)
					);
					
					// Armor bar (mavi)
					render::DrawFilledBox(
						g::hdcBuffer,
						armorLeft,
						armorBarY,
						3,
						armorBarHeight,
						RGB(0, 150, 255)
					);
				}

				// NAME (üstte) - strlen yerine cache
				int nameLen = player->name.length();
				render::RenderText(
					g::hdcBuffer,
					screenHead.x - (nameLen * 3),
					screenHead.y - 15,
					player->name.c_str(),
					config::esp_name_color,
					11
				);
			}

			// Mesafe kontrolü (fazla bilgi gösterme)
			if (roundedDistance > config::flag_render_distance)
				continue;

			// HEALTH TEXT ve ARMOR TEXT (sağ taraf) - Sadece ekstra bilgi aktifse göster
			if (config::show_extra_flags) {
				int textY = screenHead.y;
				char healthText[16];
				sprintf_s(healthText, "%dHP", player->health);
				render::RenderText(
					g::hdcBuffer,
					screenHead.x + (width / 2 + 5),
					textY,
					healthText,
					RGB(255 - player->health * 2.55f, player->health * 2.55f, 0),
					10
				);
				textY += 12;

				// ARMOR TEXT
				if (player->armor > 0) {
					char armorText[16];
					sprintf_s(armorText, "%dAR", player->armor);
					render::RenderText(
						g::hdcBuffer,
						screenHead.x + (width / 2 + 5),
						textY,
						armorText,
						RGB(0, 150, 255),
						10
					);
					textY += 12;
				}

				// Weapon
				render::RenderText(
					g::hdcBuffer,
					screenHead.x + (width / 2 + 5),
					textY,
					player->weapon.c_str(),
					config::esp_distance_color,
					10
				);
				textY += 12;

				// Distance
				char distText[32];
				sprintf_s(distText, "%dm", roundedDistance);
				render::RenderText(
					g::hdcBuffer,
					screenHead.x + (width / 2 + 5),
					textY,
					distText,
					config::esp_distance_color,
					10
				);
				textY += 12;

				// Money
				char moneyText[16];
				sprintf_s(moneyText, "$%d", player->money);
				render::RenderText(
					g::hdcBuffer,
					screenHead.x + (width / 2 + 5),
					textY,
					moneyText,
					RGB(0, 200, 0),
					10
				);
				textY += 12;

				// Flash status
				if (player->flashAlpha > 100) {
					render::RenderText(
						g::hdcBuffer,
						screenHead.x + (width / 2 + 5),
						textY,
						"FLASHED",
						RGB(255, 255, 0),
						10
					);
					textY += 12;
				}

				// Defusing status
				if (player->is_defusing) {
					render::RenderText(
						g::hdcBuffer,
						screenHead.x + (width / 2 + 5),
						textY,
						"DEFUSING",
						RGB(255, 0, 0),
						10
					);
				}
			}
		}
		
		// Granat/Molotof Tahmini - optimize edilmiş
		if (config::show_grenade_prediction) {
			// Frame throttling - her 2 frame'de bir tara (performans)
			static int grenade_frame_counter = 0;
			grenade_frame_counter++;
			
			if (grenade_frame_counter % 2 == 0) {
				// Entity list'ten granatları tara
				for (int i = 0; i < 128; i++) { // 256'dan 128'e düşürdük
					uintptr_t list_entry = g_game.process->read<uintptr_t>(g_game.process->read<uintptr_t>(g_game.base_client.base + updater::offsets::dwEntityList) + (8 * (i & 0x7FFF) >> 9) + 16);
					if (!list_entry) continue;
					
					uintptr_t entity = g_game.process->read<uintptr_t>(list_entry + 120 * (i & 0x1FF));
					if (!entity) continue;
					
					// Entity designName oku
					uintptr_t entity_identity = g_game.process->read<uintptr_t>(entity + 0x10);
					if (!entity_identity) continue;
					
					uintptr_t design_name_ptr = g_game.process->read<uintptr_t>(entity_identity + 0x20);
					if (!design_name_ptr) continue;
					
					// Design name'i oku (32 byte yeterli)
					char name_buffer[32] = {0};
					g_game.process->read_raw(design_name_ptr, name_buffer, 31);
					
					// Hızlı string kontrolü - strstr kullan
					bool is_smoke = strstr(name_buffer, "smoke") != nullptr;
					bool is_molotov = strstr(name_buffer, "molotov") != nullptr || strstr(name_buffer, "incgren") != nullptr;
					bool is_he = strstr(name_buffer, "hegren") != nullptr;
					bool is_flash = strstr(name_buffer, "flash") != nullptr;
					
					if (!is_smoke && !is_molotov && !is_he && !is_flash)
						continue;
					
					// Tip ve renk belirle
					const char* projectile_type;
					COLORREF color;
					
					if (is_smoke) {
						projectile_type = "SMOKE";
						color = RGB(200, 200, 200);
					} else if (is_molotov) {
						projectile_type = "MOLOTOV";
						color = RGB(255, 100, 0);
					} else if (is_he) {
						projectile_type = "HE";
						color = RGB(255, 0, 0);
					} else {
						projectile_type = "FLASH";
						color = RGB(255, 255, 255);
					}
					
					// Pozisyonu oku
					uintptr_t game_scene_node = g_game.process->read<uintptr_t>(entity + updater::offsets::m_pGameSceneNode);
					if (!game_scene_node) continue;
					
					Vector3 origin = g_game.process->read<Vector3>(game_scene_node + updater::offsets::m_vecAbsOrigin);
					
					// Sıfır pozisyon kontrolü
					if (origin.x == 0 && origin.y == 0 && origin.z == 0) continue;
					
					// Hızı oku
					Vector3 velocity = g_game.process->read<Vector3>(entity + updater::offsets::m_vecVelocity);
					
					Vector3 screen_pos = g_game.world_to_screen(&origin);
					
					if (screen_pos.z >= 0.01f) {
						// Mevcut pozisyon - daire çiz
						render::DrawCircle(g::hdcBuffer, (int)screen_pos.x, (int)screen_pos.y, 10, color);
						render::DrawFilledBox(g::hdcBuffer, screen_pos.x - 4, screen_pos.y - 4, 8, 8, color);
						
						// Tip yazısı
						render::RenderText(
							g::hdcBuffer,
							screen_pos.x + 15,
							screen_pos.y - 5,
							projectile_type,
							color,
							11
						);
						
						// Yörünge tahmini - sadece hızı varsa
						float vel_magnitude = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
						
						if (vel_magnitude > 10.0f) {
							static const float gravity = 800.0f;
							static const float time_step = 0.1f;
							static const int max_steps = 40; // 50'den 40'a düşürdük
							static const float air_resistance = 0.98f;
							
							Vector3 sim_pos = origin;
							Vector3 sim_vel = velocity;
							Vector3 last_screen_pos = screen_pos;
							Vector3 landing_pos = origin;
							bool found_landing = false;
							
							// Yörüngeyi simüle et
							for (int step = 0; step < max_steps; step++) {
								// Fizik hesaplamaları
								sim_vel.z -= gravity * time_step;
								sim_vel.x *= air_resistance;
								sim_vel.y *= air_resistance;
								sim_vel.z *= air_resistance;
								
								sim_pos.x += sim_vel.x * time_step;
								sim_pos.y += sim_vel.y * time_step;
								sim_pos.z += sim_vel.z * time_step;
								
								// Yere çarptı mı?
								if (sim_pos.z < origin.z - 150.0f) {
									landing_pos = sim_pos;
									found_landing = true;
									break;
								}
								
								// Her 3 adımda bir nokta çiz
								if (step % 3 == 0) {
									Vector3 sim_screen = g_game.world_to_screen(&sim_pos);
									
									if (sim_screen.z >= 0.01f) {
										render::DrawLine(
											g::hdcBuffer,
											(int)last_screen_pos.x,
											(int)last_screen_pos.y,
											(int)sim_screen.x,
											(int)sim_screen.y,
											color
										);
										
										last_screen_pos = sim_screen;
									}
								}
							}
							
							// İniş noktasını göster
							if (found_landing) {
								Vector3 landing_screen = g_game.world_to_screen(&landing_pos);
								
								if (landing_screen.z >= 0.01f) {
									// İniş noktası - X işareti
									static const int x_size = 12;
									int lx = (int)landing_screen.x;
									int ly = (int)landing_screen.y;
									
									render::DrawLine(g::hdcBuffer, lx - x_size, ly - x_size, lx + x_size, ly + x_size, color);
									render::DrawLine(g::hdcBuffer, lx + x_size, ly - x_size, lx - x_size, ly + x_size, color);
									render::DrawCircle(g::hdcBuffer, lx, ly, 15, color);
								}
							}
						}
					}
				}
			}
		}
		
		// Aimbot'u çalıştır
		aimbot();
		
		// Triggerbot'u çalıştır (ayrı özellik)
		triggerbot();
		
		// FOV çemberi çiz (aimbot aktifse) - ekran merkezinde
		if (config::aimbot_enabled && config::aimbot_fov > 0) {
			float screen_center_x = g::gameBounds.right / 2.0f;
			float screen_center_y = g::gameBounds.bottom / 2.0f;
			
			// FOV değerini pixel olarak çiz (view angle sistemi kullanıyor ama görsel için pixel)
			render::DrawCircle(
				g::hdcBuffer,
				(int)screen_center_x,
				(int)screen_center_y,
				(int)config::aimbot_fov,
				RGB(0, 255, 0) // Yeşil çember
			);
		}
		
		// FPS Göstergesi - Düzeltilmiş
		if (config::show_fps) {
			static auto last_fps_time = std::chrono::steady_clock::now();
			static int frame_count = 0;
			static int current_fps = 0;
			
			frame_count++;
			auto now = std::chrono::steady_clock::now();
			auto elapsed = std::chrono::duration<float>(now - last_fps_time).count();
			
			if (elapsed >= 1.0f) {
				current_fps = (int)(frame_count / elapsed);
				frame_count = 0;
				last_fps_time = now;
			}
			
			// FPS kutusu
			char fps_text[32];
			sprintf_s(fps_text, "FPS: %d", current_fps);
			
			int fps_width = 80;
			int fps_height = 25;
			
			render::DrawFilledBox(g::hdcBuffer, 10, 10, fps_width, fps_height, RGB(20, 20, 25));
			render::DrawBorderBox(g::hdcBuffer, 10, 10, fps_width, fps_height, RGB(0, 200, 200));
			render::RenderText(g::hdcBuffer, 18, 15, fps_text, RGB(0, 220, 220), 13);
		}
		
		// Crosshair
		if (config::show_crosshair) {
			int center_x = g::gameBounds.right / 2;
			int center_y = g::gameBounds.bottom / 2;
			int size = 10;
			int gap = 3;
			int thickness = 2;
			
			// Yatay çizgi - sol
			render::DrawFilledBox(g::hdcBuffer, center_x - size - gap, center_y - thickness/2, size, thickness, RGB(0, 255, 0));
			// Yatay çizgi - sağ
			render::DrawFilledBox(g::hdcBuffer, center_x + gap, center_y - thickness/2, size, thickness, RGB(0, 255, 0));
			// Dikey çizgi - üst
			render::DrawFilledBox(g::hdcBuffer, center_x - thickness/2, center_y - size - gap, thickness, size, RGB(0, 255, 0));
			// Dikey çizgi - alt
			render::DrawFilledBox(g::hdcBuffer, center_x - thickness/2, center_y + gap, thickness, size, RGB(0, 255, 0));
			
			// Merkez nokta
			render::DrawFilledBox(g::hdcBuffer, center_x - 1, center_y - 1, 2, 2, RGB(255, 0, 0));
		}
		
		// Bomba Timer - Basit ve Çalışan Versiyon
		if (config::show_bomb_timer && g_game.isC4Planted) {
			static auto bomb_plant_time = std::chrono::steady_clock::now();
			static bool was_planted = false;
			
			// Bomba yeni dikildi mi?
			if (!was_planted) {
				bomb_plant_time = std::chrono::steady_clock::now();
				was_planted = true;
			}
			
			// Geçen süreyi hesapla
			auto now = std::chrono::steady_clock::now();
			float elapsed = std::chrono::duration<float>(now - bomb_plant_time).count();
			float remaining = 40.0f - elapsed; // C4 40 saniyede patlar
			
			if (remaining < 0) remaining = 0;
			
			int center_x = g::gameBounds.right / 2;
			int timer_y = 80;
			
			// Arka plan kutusu
			render::DrawFilledBox(g::hdcBuffer, center_x - 120, timer_y - 10, 240, 100, RGB(20, 20, 25));
			render::DrawBorderBox(g::hdcBuffer, center_x - 120, timer_y - 10, 240, 100, RGB(255, 50, 50));
			
			// Başlık
			render::RenderText(g::hdcBuffer, center_x - 70, timer_y, "BOMBA DIKILDI", RGB(255, 50, 50), 16);
			
			// Geri sayım - büyük ve net
			char time_text[32];
			int seconds = (int)remaining;
			int milliseconds = (int)((remaining - seconds) * 10);
			sprintf_s(time_text, "%d.%d", seconds, milliseconds);
			
			// Renk - yeşilden kırmızıya
			int red = 255;
			int green = (int)(remaining / 40.0f * 255.0f);
			if (green > 255) green = 255;
			
			render::RenderText(g::hdcBuffer, center_x - 35, timer_y + 25, time_text, RGB(red, green, 0), 32);
			
			// Progress bar
			int bar_width = 200;
			int bar_x = center_x - 100;
			int bar_y = timer_y + 65;
			float progress = remaining / 40.0f;
			
			render::DrawFilledBox(g::hdcBuffer, bar_x, bar_y, bar_width, 8, RGB(40, 40, 45));
			render::DrawFilledBox(g::hdcBuffer, bar_x, bar_y, (int)(bar_width * progress), 8, RGB(red, green, 0));
			render::DrawBorderBox(g::hdcBuffer, bar_x, bar_y, bar_width, 8, RGB(255, 50, 50));
			
			// Uyarı
			if (remaining < 10.0f) {
				render::RenderText(g::hdcBuffer, center_x - 60, timer_y + 78, "ACIL DEFUSE!", RGB(255, 0, 0), 12);
			}
		} else {
			// Bomba defuse edildi veya patladı - reset
			static bool was_planted = false;
			was_planted = false;
		}
		
		// std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

