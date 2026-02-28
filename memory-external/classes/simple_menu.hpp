#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <iostream>
#include "render.hpp"
#include "config.hpp"
#include "globals.hpp"

namespace simple_menu {
    inline bool is_open = false;
    inline int selected_tab = 0;
    
    struct Tab {
        std::string name;
        int x, y, width, height;
    };
    
    struct Toggle {
        std::string label;
        bool* value;
        int x, y, width, height;
    };
    
    inline std::vector<Tab> tabs;
    inline std::vector<std::vector<Toggle>> tab_items;
    
    void init() {
        tabs.clear();
        tab_items.clear();
        
        // Tab 1: ESP
        tabs.push_back({"ESP", 0, 0, 0, 0});
        std::vector<Toggle> esp_items;
        esp_items.push_back({"Box ESP", &config::show_box_esp, 0, 0, 0, 0});
        esp_items.push_back({"Skeleton ESP", &config::show_skeleton_esp, 0, 0, 0, 0});
        esp_items.push_back({"Head Tracker", &config::show_head_tracker, 0, 0, 0, 0});
        esp_items.push_back({"Team ESP", &config::team_esp, 0, 0, 0, 0});
        esp_items.push_back({"Extra Flags", &config::show_extra_flags, 0, 0, 0, 0});
        tab_items.push_back(esp_items);
        
        // Tab 2: Settings
        tabs.push_back({"Settings", 0, 0, 0, 0});
        std::vector<Toggle> settings_items;
        settings_items.push_back({"Auto Update", &config::automatic_update, 0, 0, 0, 0});
        tab_items.push_back(settings_items);
    }
    
    void render(HDC hdc) {
        if (!is_open) return;
        
        const int menu_x = 50;
        const int menu_y = 50;
        const int menu_width = 400;
        const int menu_height = 500;
        const int tab_height = 40;
        
        // Background - solid color
        render::DrawFilledBox(hdc, menu_x, menu_y, menu_width, menu_height, RGB(30, 30, 30));
        render::DrawBorderBox(hdc, menu_x, menu_y, menu_width, menu_height, RGB(0, 200, 200));
        
        // Title
        render::RenderText(hdc, menu_x + 10, menu_y + 10, "CS2 ESP Menu", RGB(0, 200, 200), 18);
        
        // Close button
        int close_x = menu_x + menu_width - 30;
        int close_y = menu_y + 5;
        render::DrawFilledBox(hdc, close_x, close_y, 25, 25, RGB(200, 50, 50));
        render::RenderText(hdc, close_x + 7, close_y + 5, "X", RGB(255, 255, 255), 14);
        
        // Tabs
        int tab_y = menu_y + 50;
        int tab_width = menu_width / tabs.size();
        
        for (size_t i = 0; i < tabs.size(); i++) {
            tabs[i].x = menu_x + (i * tab_width);
            tabs[i].y = tab_y;
            tabs[i].width = tab_width;
            tabs[i].height = tab_height;
            
            COLORREF bg_color = (i == selected_tab) ? RGB(0, 150, 150) : RGB(50, 50, 50);
            render::DrawFilledBox(hdc, tabs[i].x, tabs[i].y, tabs[i].width, tabs[i].height, bg_color);
            render::DrawBorderBox(hdc, tabs[i].x, tabs[i].y, tabs[i].width, tabs[i].height, RGB(0, 200, 200));
            
            COLORREF text_color = (i == selected_tab) ? RGB(255, 255, 255) : RGB(150, 150, 150);
            render::RenderText(hdc, tabs[i].x + 10, tabs[i].y + 12, tabs[i].name.c_str(), text_color, 14);
        }
        
        // Items
        if (selected_tab < tab_items.size()) {
            int item_y = tab_y + tab_height + 20;
            
            for (size_t i = 0; i < tab_items[selected_tab].size(); i++) {
                Toggle& item = tab_items[selected_tab][i];
                item.x = menu_x + 20;
                item.y = item_y;
                item.width = menu_width - 40;
                item.height = 35;
                
                // Item background
                render::DrawFilledBox(hdc, item.x, item.y, item.width, item.height, RGB(40, 40, 40));
                
                // Label
                render::RenderText(hdc, item.x + 10, item.y + 10, item.label.c_str(), RGB(200, 200, 200), 13);
                
                // Toggle switch
                int toggle_x = item.x + item.width - 60;
                int toggle_y = item.y + 7;
                int toggle_w = 50;
                int toggle_h = 20;
                
                bool is_on = item.value && *item.value;
                COLORREF toggle_color = is_on ? RGB(0, 200, 0) : RGB(100, 100, 100);
                render::DrawFilledBox(hdc, toggle_x, toggle_y, toggle_w, toggle_h, toggle_color);
                render::DrawBorderBox(hdc, toggle_x, toggle_y, toggle_w, toggle_h, RGB(200, 200, 200));
                
                const char* status = is_on ? "ON" : "OFF";
                render::RenderText(hdc, toggle_x + 10, toggle_y + 3, status, RGB(255, 255, 255), 12);
                
                item_y += item.height + 10;
            }
        }
        
        // Footer
        render::RenderText(hdc, menu_x + 10, menu_y + menu_height - 25, "Click to toggle | X to close", RGB(150, 150, 150), 11);
    }
    
    void handle_click(int x, int y) {
        if (!is_open) return;
        
        const int menu_x = 50;
        const int menu_y = 50;
        const int menu_width = 400;
        
        // Close button
        int close_x = menu_x + menu_width - 30;
        int close_y = menu_y + 5;
        if (x >= close_x && x <= close_x + 25 && y >= close_y && y <= close_y + 25) {
            is_open = false;
            return;
        }
        
        // Tabs
        for (size_t i = 0; i < tabs.size(); i++) {
            if (x >= tabs[i].x && x <= tabs[i].x + tabs[i].width &&
                y >= tabs[i].y && y <= tabs[i].y + tabs[i].height) {
                selected_tab = i;
                return;
            }
        }
        
        // Items
        if (selected_tab < tab_items.size()) {
            for (size_t i = 0; i < tab_items[selected_tab].size(); i++) {
                Toggle& item = tab_items[selected_tab][i];
                if (x >= item.x && x <= item.x + item.width &&
                    y >= item.y && y <= item.y + item.height) {
                    if (item.value) {
                        *item.value = !*item.value;
                        try {
                            config::save();
                        } catch (...) {}
                    }
                    return;
                }
            }
        }
    }
    
    void toggle() {
        is_open = !is_open;
        
        std::cout << "[simple_menu] Toggle called! is_open = " << is_open << std::endl;
        
        // Window style'ı değiştir
        if (g::overlayHwnd) {
            LONG exStyle = GetWindowLong(g::overlayHwnd, GWL_EXSTYLE);
            std::cout << "[simple_menu] Current exStyle: " << exStyle << std::endl;
            
            if (is_open) {
                // Menü açıkken: transparent'ı kaldır, mouse tıklamalarını al
                exStyle &= ~WS_EX_TRANSPARENT;
                std::cout << "[simple_menu] Menu OPENED - removing WS_EX_TRANSPARENT" << std::endl;
            } else {
                // Menü kapalıyken: transparent yap, mouse tıklamalarını geçir
                exStyle |= WS_EX_TRANSPARENT;
                std::cout << "[simple_menu] Menu CLOSED - adding WS_EX_TRANSPARENT" << std::endl;
            }
            SetWindowLong(g::overlayHwnd, GWL_EXSTYLE, exStyle);
            std::cout << "[simple_menu] New exStyle: " << GetWindowLong(g::overlayHwnd, GWL_EXSTYLE) << std::endl;
        } else {
            std::cout << "[simple_menu] ERROR: overlayHwnd is NULL!" << std::endl;
        }
    }
}
