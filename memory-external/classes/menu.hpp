#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <iostream>
#include "render.hpp"
#include "config.hpp"
#include "globals.hpp"

namespace menu {
    inline bool is_open = false;
    inline int selected_category = 0;
    inline int scroll_offset = 0;
    inline int max_visible_items = 10;
    
    // Menü pozisyonu ve sürükleme
    inline int menu_x = 120;
    inline int menu_y = 110;
    inline bool is_dragging = false;
    inline int drag_offset_x = 0;
    inline int drag_offset_y = 0;
    
    struct MenuItem {
        std::string label;
        bool* toggle_value = nullptr;
        int* int_value = nullptr;
        float* float_value = nullptr;
        int min_value = 0;
        int max_value = 100;
        bool is_slider = false;
        bool is_key_bind = false; // Tuş atama için
        bool is_target_selector = false; // Hedef seçimi için
        int x, y, width, height;
    };

    struct Category {
        std::string name;
        std::string icon;
        std::vector<MenuItem> items;
        int x, y, width, height;
    };

    inline std::vector<Category> categories;

    void init() {
        categories.clear();
        
        // === AIMBOT KATEGORİSİ ===
        Category aimbot_cat;
        aimbot_cat.name = "Aimbot";
        aimbot_cat.icon = "A";
        
        MenuItem aimbot;
        aimbot.label = "Aimbot Active";
        aimbot.toggle_value = &config::aimbot_enabled;
        
        MenuItem aimbot_key;
        aimbot_key.label = "Aimbot Key";
        aimbot_key.int_value = &config::aimbot_key;
        aimbot_key.is_key_bind = true;
        
        MenuItem aimbot_target;
        aimbot_target.label = "Target Point";
        aimbot_target.int_value = &config::aimbot_target;
        aimbot_target.is_target_selector = true;
        
        MenuItem aimbot_fov;
        aimbot_fov.label = "FOV (Area)";
        aimbot_fov.float_value = &config::aimbot_fov;
        aimbot_fov.min_value = 0;
        aimbot_fov.max_value = 250;
        aimbot_fov.is_slider = true;
        
        MenuItem aimbot_smooth;
        aimbot_smooth.label = "Smoothing";
        aimbot_smooth.float_value = &config::aimbot_smooth;
        aimbot_smooth.min_value = 1;
        aimbot_smooth.max_value = 10;
        aimbot_smooth.is_slider = true;
        
        MenuItem aimbot_humanizer;
        aimbot_humanizer.label = "Humanizer";
        aimbot_humanizer.toggle_value = &config::aimbot_humanizer;
        
        MenuItem aimbot_deathmatch;
        aimbot_deathmatch.label = "Deathmatch Mode";
        aimbot_deathmatch.toggle_value = &config::aimbot_deathmatch_mode;
        
        MenuItem aimbot_wall_check;
        aimbot_wall_check.label = "Wall Check";
        aimbot_wall_check.toggle_value = &config::aimbot_wall_check;
        
        aimbot_cat.items.push_back(aimbot);
        aimbot_cat.items.push_back(aimbot_key);
        aimbot_cat.items.push_back(aimbot_target);
        aimbot_cat.items.push_back(aimbot_fov);
        aimbot_cat.items.push_back(aimbot_smooth);
        aimbot_cat.items.push_back(aimbot_humanizer);
        aimbot_cat.items.push_back(aimbot_deathmatch);
        aimbot_cat.items.push_back(aimbot_wall_check);
        
        categories.push_back(aimbot_cat);

        // === TRIGGERBOT KATEGORİSİ ===
        Category trigger_cat;
        trigger_cat.name = "Triggerbot";
        trigger_cat.icon = "T";
        
        MenuItem triggerbot;
        triggerbot.label = "Triggerbot Active";
        triggerbot.toggle_value = &config::triggerbot_enabled;
        
        MenuItem triggerbot_key;
        triggerbot_key.label = "Triggerbot Key";
        triggerbot_key.int_value = &config::triggerbot_key;
        triggerbot_key.is_key_bind = true;
        
        MenuItem triggerbot_delay;
        triggerbot_delay.label = "Delay (ms)";
        triggerbot_delay.int_value = &config::triggerbot_delay;
        triggerbot_delay.min_value = 0;
        triggerbot_delay.max_value = 500;
        triggerbot_delay.is_slider = true;
        
        MenuItem triggerbot_team;
        triggerbot_team.label = "Team Check";
        triggerbot_team.toggle_value = &config::triggerbot_team_check;
        
        trigger_cat.items.push_back(triggerbot);
        trigger_cat.items.push_back(triggerbot_key);
        trigger_cat.items.push_back(triggerbot_delay);
        trigger_cat.items.push_back(triggerbot_team);
        
        categories.push_back(trigger_cat);

        // === ESP KATEGORİSİ ===
        Category esp_cat;
        esp_cat.name = "ESP";
        esp_cat.icon = "E";
        
        MenuItem box_esp;
        box_esp.label = "2D Box";
        box_esp.toggle_value = &config::show_box_esp;
        
        MenuItem box_3d_esp;
        box_3d_esp.label = "3D Box";
        box_3d_esp.toggle_value = &config::show_3d_box_esp;
        
        MenuItem skeleton_esp;
        skeleton_esp.label = "Skeleton";
        skeleton_esp.toggle_value = &config::show_skeleton_esp;
        
        MenuItem head_tracker;
        head_tracker.label = "Head Tracker";
        head_tracker.toggle_value = &config::show_head_tracker;
        
        MenuItem team_esp;
        team_esp.label = "Team ESP (Show Teammates)";
        team_esp.toggle_value = &config::team_esp;
        
        MenuItem extra_flags;
        extra_flags.label = "Extra Info";
        extra_flags.toggle_value = &config::show_extra_flags;
        
        esp_cat.items.push_back(box_esp);
        esp_cat.items.push_back(box_3d_esp);
        esp_cat.items.push_back(skeleton_esp);
        esp_cat.items.push_back(head_tracker);
        esp_cat.items.push_back(team_esp);
        esp_cat.items.push_back(extra_flags);
        
        categories.push_back(esp_cat);

        // === MISC CATEGORY ===
        Category misc;
        misc.name = "Other";
        misc.icon = "M";
        
        MenuItem show_fps;
        show_fps.label = "FPS Counter";
        show_fps.toggle_value = &config::show_fps;
        
        MenuItem show_crosshair;
        show_crosshair.label = "Crosshair";
        show_crosshair.toggle_value = &config::show_crosshair;
        
        MenuItem show_bomb_timer;
        show_bomb_timer.label = "Bomb Timer";
        show_bomb_timer.toggle_value = &config::show_bomb_timer;
        
        MenuItem show_grenade_pred;
        show_grenade_pred.label = "Grenade Prediction";
        show_grenade_pred.toggle_value = &config::show_grenade_prediction;
        
        MenuItem auto_update;
        auto_update.label = "Auto Update";
        auto_update.toggle_value = &config::automatic_update;
        
        misc.items.push_back(show_fps);
        misc.items.push_back(show_crosshair);
        misc.items.push_back(show_bomb_timer);
        misc.items.push_back(show_grenade_pred);
        misc.items.push_back(auto_update);
        
        categories.push_back(misc);
    }

    void toggle() {
        is_open = !is_open;
        
        if (g::overlayHwnd) {
            LONG exStyle = GetWindowLong(g::overlayHwnd, GWL_EXSTYLE);
            
            if (is_open) {
                // Menü açık - transparent'ı kaldır ama NOACTIVATE ekle (focus değişmesin)
                exStyle &= ~WS_EX_TRANSPARENT;
                exStyle |= WS_EX_NOACTIVATE;
                SetWindowLong(g::overlayHwnd, GWL_EXSTYLE, exStyle);
            } else {
                // Menü kapalı - transparent ekle, NOACTIVATE kaldır
                exStyle |= WS_EX_TRANSPARENT;
                exStyle &= ~WS_EX_NOACTIVATE;
                SetWindowLong(g::overlayHwnd, GWL_EXSTYLE, exStyle);
            }
        }
    }

    void render(HDC hdc) {
        if (!is_open) return;

        const int menu_width = 780;
        const int menu_height = 600;
        const int sidebar_width = 180;
        const int header_height = 50;

        // Shadow - tamamen opak, sadece menü boyutunda
        for (int i = 3; i > 0; i--) {
            render::DrawFilledBox(hdc, menu_x + i*2, menu_y + i*2, menu_width, menu_height, RGB(10, 10, 10));
        }

        // Ana arka plan - tamamen opak RGB(30,30,35) - SADECE MENÜ ALANI
        render::DrawFilledBox(hdc, menu_x, menu_y, menu_width, menu_height, RGB(30, 30, 35));
        
        // Dış çerçeve - opak
        render::DrawBorderBox(hdc, menu_x-1, menu_y-1, menu_width+2, menu_height+2, RGB(0, 200, 200));
        render::DrawBorderBox(hdc, menu_x, menu_y, menu_width, menu_height, RGB(0, 150, 150));

        // Sidebar arka planı - tamamen opak RGB(25,25,30)
        render::DrawFilledBox(hdc, menu_x, menu_y, sidebar_width, menu_height, RGB(25, 25, 30));
        
        // Sidebar çerçevesi
        render::DrawLine(hdc, menu_x + sidebar_width, menu_y, menu_x + sidebar_width, menu_y + menu_height, RGB(0, 200, 200));

        // Header bar - opak (sürüklenebilir alan)
        render::DrawFilledBox(hdc, menu_x, menu_y, menu_width, header_height, RGB(25, 25, 30));
        render::DrawLine(hdc, menu_x, menu_y + header_height, menu_x + menu_width, menu_y + header_height, RGB(0, 200, 200));
        
        // Sürükleme göstergesi (header'da 6 nokta - grip dots)
        int dot_start_x = menu_x + menu_width / 2 - 15;
        int dot_y_base = menu_y + header_height / 2 - 3;
        COLORREF dot_color = RGB(100, 100, 100);
        for (int row = 0; row < 2; row++) {
            for (int col = 0; col < 3; col++) {
                int dot_x = dot_start_x + col * 12;
                int dot_y_pos = dot_y_base + row * 6;
                render::DrawFilledBox(hdc, dot_x, dot_y_pos, 3, 3, dot_color);
            }
        }

        // Logo box - opak
        render::DrawFilledBox(hdc, menu_x + 15, menu_y + 10, 40, 30, RGB(0, 180, 180));
        render::DrawBorderBox(hdc, menu_x + 15, menu_y + 10, 40, 30, RGB(0, 220, 220));
        render::RenderText(hdc, menu_x + 20, menu_y + 15, "CS2", RGB(255, 255, 255), 16);

        // Title - RAVEN MOD
        render::RenderText(hdc, menu_x + 65, menu_y + 18, "RAVEN MOD", RGB(0, 220, 220), 18);
        
        // Version badge - opak
        render::DrawFilledBox(hdc, menu_x + menu_width - 70, menu_y + 15, 50, 20, RGB(0, 180, 180));
        render::RenderText(hdc, menu_x + menu_width - 62, menu_y + 19, "v1.0", RGB(255, 255, 255), 11);

        // Close button - opak
        int close_x = menu_x + menu_width - 35;
        int close_y = menu_y + 10;
        render::DrawFilledBox(hdc, close_x, close_y, 25, 25, RGB(200, 50, 50));
        render::DrawBorderBox(hdc, close_x, close_y, 25, 25, RGB(255, 100, 100));
        render::RenderText(hdc, close_x + 7, close_y + 5, "X", RGB(255, 255, 255), 14);

        // Categories
        int cat_y = menu_y + 70;
        
        for (size_t i = 0; i < categories.size(); i++) {
            categories[i].x = menu_x + 10;
            categories[i].y = cat_y;
            categories[i].width = sidebar_width - 20;
            categories[i].height = 45;

            bool is_selected = (i == selected_category);
            
            if (is_selected) {
                // Seçili kategori - opak
                render::DrawFilledBox(hdc, categories[i].x, categories[i].y, categories[i].width, categories[i].height, RGB(0, 180, 180));
                render::DrawBorderBox(hdc, categories[i].x, categories[i].y, categories[i].width, categories[i].height, RGB(0, 220, 220));
            } else {
                // Normal kategori - opak
                render::DrawFilledBox(hdc, categories[i].x, categories[i].y, categories[i].width, categories[i].height, RGB(35, 35, 40));
            }

            // Icon
            render::RenderText(hdc, categories[i].x + 12, categories[i].y + 12, categories[i].icon.c_str(), 
                is_selected ? RGB(255, 255, 255) : RGB(0, 180, 180), 18);

            // Name
            render::RenderText(hdc, categories[i].x + 45, categories[i].y + 15, categories[i].name.c_str(), 
                is_selected ? RGB(255, 255, 255) : RGB(180, 180, 180), 13);

            cat_y += 55;
        }

        // Content area background - tamamen opak RGB(35,35,40) - SADECE MENÜ İÇİNDE
        int content_x = menu_x + sidebar_width;
        int content_y = menu_y + 50;
        int content_width = menu_width - sidebar_width;
        int content_height = menu_height - 50;
        
        // Content area'yı doldur - SADECE MENÜ SINIRLARINDA
        render::DrawFilledBox(hdc, content_x, content_y, content_width, content_height, RGB(35, 35, 40));

        // Content area
        if (selected_category < categories.size()) {
            int item_content_x = content_x + 25;
            int item_content_y = content_y + 15;
            
            // Category title
            render::RenderText(hdc, item_content_x, item_content_y, categories[selected_category].name.c_str(), RGB(0, 220, 220), 18);
            render::DrawFilledBox(hdc, item_content_x, item_content_y + 25, 60, 2, RGB(0, 220, 220));

            // Items with scroll
            int item_y = item_content_y + 40;
            int visible_start = scroll_offset;
            int visible_end = scroll_offset + max_visible_items;
            
            // Scroll limiti ayarla
            int total_items = categories[selected_category].items.size();
            int max_scroll = total_items > max_visible_items ? total_items - max_visible_items : 0;
            if (scroll_offset > max_scroll) scroll_offset = max_scroll;
            if (scroll_offset < 0) scroll_offset = 0;
            
            for (size_t i = visible_start; i < categories[selected_category].items.size() && i < visible_end; i++) {
                MenuItem& item = categories[selected_category].items[i];
                
                item.x = item_content_x;
                item.y = item_y;
                item.width = content_width - 50;
                item.height = item.is_slider ? 55 : 38;

                // Item background - opak
                render::DrawFilledBox(hdc, item.x - 5, item.y - 2, item.width + 10, item.height, RGB(30, 30, 35));

                if (item.is_slider) {
                    render::RenderText(hdc, item.x, item_y + 2, item.label.c_str(), RGB(220, 220, 220), 12);

                    int slider_x = item.x;
                    int slider_y = item_y + 22;
                    int slider_width = item.width - 70;
                    int slider_height = 8;

                    // Slider background - opak
                    render::DrawFilledBox(hdc, slider_x, slider_y, slider_width, slider_height, RGB(40, 40, 45));
                    render::DrawBorderBox(hdc, slider_x, slider_y, slider_width, slider_height, RGB(60, 60, 65));

                    float value_percent = 0;
                    float display_value = 0;

                    if (item.float_value) {
                        value_percent = (*item.float_value - item.min_value) / (float)(item.max_value - item.min_value);
                        display_value = *item.float_value;
                    } else if (item.int_value) {
                        value_percent = (*item.int_value - item.min_value) / (float)(item.max_value - item.min_value);
                        display_value = (float)*item.int_value;
                    }

                    int filled_width = (int)(slider_width * value_percent);
                    if (filled_width > 0) {
                        // Slider fill - opak
                        render::DrawFilledBox(hdc, slider_x, slider_y, filled_width, slider_height, RGB(0, 200, 200));
                    }

                    // Slider handle - opak
                    int handle_x = slider_x + filled_width - 4;
                    render::DrawFilledBox(hdc, handle_x, slider_y - 2, 8, slider_height + 4, RGB(0, 220, 220));

                    // Değeri göster - float için ondalık, int için tam sayı
                    char value_text[32];
                    if (item.float_value) {
                        sprintf_s(value_text, "%.1f", display_value);
                    } else {
                        sprintf_s(value_text, "%d", (int)display_value);
                    }
                    render::RenderText(hdc, slider_x + slider_width + 15, item_y + 2, value_text, RGB(0, 220, 220), 13);

                } else if (item.is_target_selector) {
                    // Target selector button - hedef seçimi
                    int button_x = item.x + item.width - 120;
                    int button_y = item_y + 5;
                    int button_width = 110;
                    int button_height = 28;

                    // Button background - opak
                    render::DrawFilledBox(hdc, button_x, button_y, button_width, button_height, RGB(0, 180, 180));
                    render::DrawBorderBox(hdc, button_x, button_y, button_width, button_height, RGB(0, 220, 220));

                    // Hedef adını göster
                    const char* target_name = config::get_target_name(item.int_value ? *item.int_value : 1);
                    render::RenderText(hdc, button_x + 10, button_y + 8, target_name, RGB(255, 255, 255), 12);

                    // Label
                    render::RenderText(hdc, item.x, item_y + 10, item.label.c_str(), RGB(220, 220, 220), 12);
                    
                } else if (item.is_key_bind) {
                    // Key bind button - tuş atama
                    int button_x = item.x + item.width - 120;
                    int button_y = item_y + 5;
                    int button_width = 110;
                    int button_height = 28;

                    // Button background - opak
                    render::DrawFilledBox(hdc, button_x, button_y, button_width, button_height, RGB(0, 180, 180));
                    render::DrawBorderBox(hdc, button_x, button_y, button_width, button_height, RGB(0, 220, 220));

                    // Tuş adını göster
                    const char* key_name = config::get_key_name(item.int_value ? *item.int_value : VK_XBUTTON2);
                    render::RenderText(hdc, button_x + 10, button_y + 8, key_name, RGB(255, 255, 255), 12);

                    // Label
                    render::RenderText(hdc, item.x, item_y + 10, item.label.c_str(), RGB(220, 220, 220), 12);
                    
                } else {
                    // Toggle switch - opak
                    int toggle_x = item.x + item.width - 65;
                    int toggle_y = item_y + 5;
                    int toggle_width = 54;
                    int toggle_height = 28;

                    bool is_on = item.toggle_value && *item.toggle_value;
                    COLORREF toggle_bg = is_on ? RGB(0, 200, 200) : RGB(50, 50, 55);

                    // Toggle background - opak
                    render::DrawFilledBox(hdc, toggle_x, toggle_y, toggle_width, toggle_height, toggle_bg);
                    render::DrawBorderBox(hdc, toggle_x, toggle_y, toggle_width, toggle_height, RGB(0, 220, 220));

                    // Toggle circle - opak beyaz
                    int circle_x = is_on ? toggle_x + toggle_width - 26 : toggle_x + 2;
                    int circle_y = toggle_y + 2;
                    render::DrawFilledBox(hdc, circle_x, circle_y, 24, 24, RGB(255, 255, 255));

                    // Label
                    render::RenderText(hdc, item.x, item_y + 10, item.label.c_str(), RGB(220, 220, 220), 12);
                    
                    // Status badge - opak
                    const char* status = is_on ? "ON" : "OFF";
                    COLORREF status_color = is_on ? RGB(0, 255, 120) : RGB(120, 120, 120);
                    int badge_x = toggle_x - 70;
                    render::DrawFilledBox(hdc, badge_x, item_y + 8, 55, 18, is_on ? RGB(0, 80, 60) : RGB(45, 45, 50));
                    render::RenderText(hdc, badge_x + 8, item_y + 10, status, status_color, 10);
                }

                item_y += item.height + 8;
            }
            
            // Scroll indicator (sağ tarafta)
            if (total_items > max_visible_items) {
                int scroll_bar_x = content_x + content_width - 15;
                int scroll_bar_y = content_y + 60;
                int scroll_bar_height = content_height - 100;
                
                // Scroll bar background
                render::DrawFilledBox(hdc, scroll_bar_x, scroll_bar_y, 8, scroll_bar_height, RGB(40, 40, 45));
                
                // Scroll thumb
                float scroll_percent = (float)scroll_offset / max_scroll;
                int thumb_height = (scroll_bar_height * max_visible_items) / total_items;
                if (thumb_height < 20) thumb_height = 20;
                int thumb_y = scroll_bar_y + (int)((scroll_bar_height - thumb_height) * scroll_percent);
                
                render::DrawFilledBox(hdc, scroll_bar_x, thumb_y, 8, thumb_height, RGB(0, 200, 200));
            }
        }

        // Footer - opak - SADECE MENÜ GENİŞLİĞİNDE
        render::DrawFilledBox(hdc, menu_x, menu_y + menu_height - 25, menu_width, 25, RGB(25, 25, 30));
        render::DrawLine(hdc, menu_x, menu_y + menu_height - 25, menu_x + menu_width, menu_y + menu_height - 25, RGB(0, 200, 200));
        render::RenderText(hdc, menu_x + 15, menu_y + menu_height - 18, "DELETE: Close | Drag: Move | END: Exit", RGB(150, 150, 150), 10);
        
        // Status indicator - opak
        render::DrawFilledBox(hdc, menu_x + menu_width - 150, menu_y + menu_height - 18, 10, 10, RGB(0, 255, 0));
        render::RenderText(hdc, menu_x + menu_width - 135, menu_y + menu_height - 18, "Auto Update", RGB(150, 150, 150), 10);
    }
    
    void handle_mouse_down(int x, int y) {
        if (!is_open) return;
        
        const int menu_width = 780;
        const int header_height = 50;
        
        // Header'a tıklandı mı? (sürükleme başlat)
        if (x >= menu_x && x <= menu_x + menu_width &&
            y >= menu_y && y <= menu_y + header_height) {
            
            // Close button kontrolü
            int close_x = menu_x + menu_width - 35;
            int close_y = menu_y + 10;
            if (x >= close_x && x <= close_x + 25 && y >= close_y && y <= close_y + 25) {
                toggle();
                return;
            }
            
            // Header'ın geri kalanına tıklandı - sürükleme başlat
            is_dragging = true;
            drag_offset_x = x - menu_x;
            drag_offset_y = y - menu_y;
            return;
        }
    }
    
    void handle_mouse_up(int x, int y) {
        if (is_dragging) {
            is_dragging = false;
        }
    }
    
    void handle_mouse_move(int x, int y) {
        if (is_dragging) {
            // Menüyü sürükle
            menu_x = x - drag_offset_x;
            menu_y = y - drag_offset_y;
            
            // Ekran sınırları içinde tut
            const int menu_width = 780;
            const int menu_height = 600;
            
            if (menu_x < 0) menu_x = 0;
            if (menu_y < 0) menu_y = 0;
            if (menu_x + menu_width > g::gameBounds.right) menu_x = g::gameBounds.right - menu_width;
            if (menu_y + menu_height > g::gameBounds.bottom) menu_y = g::gameBounds.bottom - menu_height;
        }
    }
    
    void handle_mouse_click(int x, int y) {
        if (!is_open) return;
        
        const int menu_width = 780;
        
        // Close button
        int close_x = menu_x + menu_width - 35;
        int close_y = menu_y + 10;
        if (x >= close_x && x <= close_x + 25 && y >= close_y && y <= close_y + 25) {
            toggle();
            return;
        }
        
        // Categories
        for (size_t i = 0; i < categories.size(); i++) {
            if (x >= categories[i].x && x <= categories[i].x + categories[i].width &&
                y >= categories[i].y && y <= categories[i].y + categories[i].height) {
                selected_category = i;
                scroll_offset = 0; // Kategori değişince scroll'u sıfırla
                return;
            }
        }
        
        // Items
        if (selected_category < categories.size()) {
            for (size_t i = 0; i < categories[selected_category].items.size(); i++) {
                MenuItem& item = categories[selected_category].items[i];
                if (x >= item.x && x <= item.x + item.width &&
                    y >= item.y && y <= item.y + item.height) {
                    
                    if (item.is_slider) {
                        // Slider click - değeri ayarla
                        int slider_x = item.x;
                        int slider_width = item.width - 70;
                        int click_pos = x - slider_x;
                        
                        if (click_pos >= 0 && click_pos <= slider_width) {
                            float percent = (float)click_pos / (float)slider_width;
                            float new_value = item.min_value + (percent * (item.max_value - item.min_value));
                            
                            if (item.float_value) {
                                *item.float_value = new_value;
                            } else if (item.int_value) {
                                *item.int_value = (int)new_value;
                            }
                            
                            try {
                                config::save();
                            } catch (...) {}
                        }
                    } else if (item.is_key_bind && item.int_value) {
                        // Key bind click - tuş değiştir (cycle through keys)
                        int current_key = *item.int_value;
                        
                        // Tuş listesi: Mouse 1-5, Shift, Ctrl, Alt, Caps
                        static const int keys[] = {
                            VK_LBUTTON, VK_RBUTTON, VK_MBUTTON, 
                            VK_XBUTTON1, VK_XBUTTON2,
                            VK_SHIFT, VK_CONTROL, VK_MENU, VK_CAPITAL
                        };
                        
                        // Mevcut tuşun indexini bul
                        int current_index = 4; // Default: Mouse 5
                        for (int j = 0; j < 9; j++) {
                            if (keys[j] == current_key) {
                                current_index = j;
                                break;
                            }
                        }
                        
                        // Sonraki tuşa geç
                        current_index = (current_index + 1) % 9;
                        *item.int_value = keys[current_index];
                        
                        try {
                            config::save();
                        } catch (...) {}
                    } else if (item.is_target_selector && item.int_value) {
                        // Target selector click - hedef değiştir (cycle: Kafa -> Göğüs -> Ayak)
                        int current_target = *item.int_value;
                        current_target = (current_target + 1) % 3; // 0, 1, 2 arası döngü
                        *item.int_value = current_target;
                        
                        try {
                            config::save();
                        } catch (...) {}
                        
                    } else if (item.toggle_value) {
                        // Toggle click
                        *item.toggle_value = !*item.toggle_value;
                        try {
                            config::save();
                        } catch (...) {}
                    }
                    return;
                }
            }
        }
    }
    
    void handle_mouse_wheel(int delta) {
        if (!is_open) return;
        
        // Mouse wheel ile scroll
        // delta > 0 = yukarı scroll (öğeleri aşağı kaydır)
        // delta < 0 = aşağı scroll (öğeleri yukarı kaydır)
        
        if (delta > 0) {
            scroll_offset--;
        } else if (delta < 0) {
            scroll_offset++;
        }
        
        // Scroll limitlerini kontrol et
        if (selected_category < categories.size()) {
            int total_items = categories[selected_category].items.size();
            int max_scroll = total_items > max_visible_items ? total_items - max_visible_items : 0;
            
            if (scroll_offset < 0) scroll_offset = 0;
            if (scroll_offset > max_scroll) scroll_offset = max_scroll;
        }
    }
}
