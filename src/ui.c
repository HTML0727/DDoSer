#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cJSON.h>
#include <dirent.h>
#include "ui.h"
#include "network.h"
#include "attack.h"

// Global variables for UI
static NetworkInterface net_interface = {
    .interface = "eth0",
    .mac_address = "00:00:00:00:00:00",
    .gateway = "192.168.1.1",
    .netmask = "255.255.255.0",
    .log_path = "logs/ddos.log"
};

static TargetEntry targets[MAX_TARGETS];
static int target_count = 0;
static SystemStats stats = {0, 0, 0.0, 0.0};
// static int current_panel = 0; // 0: main, 1: config, 2: network, 3: stats 
static int selected_target = 0;

// Color pairs
enum {
    COLOR_DEFAULT = 1,
    COLOR_HEADER,
    COLOR_TARGET_RUNNING,
    COLOR_TARGET_STOPPED,
    COLOR_PANEL_BORDER,
    COLOR_BUTTON,
    COLOR_INPUT,
    COLOR_ERROR,
    COLOR_SUCCESS
};

void init_ui(void) {
    // Set locale for UTF-8 support
    setlocale(LC_ALL, "");
    
    // Create directories if they don't exist
    mkdir("config", 0755);
    mkdir("logs", 0755);
    mkdir("presets", 0755);
    
    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    mousemask(ALL_MOUSE_EVENTS, NULL);
    
    // Check if terminal supports colors
    if (has_colors()) {
        start_color();
        
        // Initialize color pairs
        init_pair(COLOR_DEFAULT, COLOR_WHITE, COLOR_BLACK);
        init_pair(COLOR_HEADER, COLOR_BLACK, COLOR_CYAN);
        init_pair(COLOR_TARGET_RUNNING, COLOR_GREEN, COLOR_BLACK);
        init_pair(COLOR_TARGET_STOPPED, COLOR_RED, COLOR_BLACK);
        init_pair(COLOR_PANEL_BORDER, COLOR_BLUE, COLOR_BLACK);
        init_pair(COLOR_BUTTON, COLOR_BLACK, COLOR_WHITE);
        init_pair(COLOR_INPUT, COLOR_YELLOW, COLOR_BLACK);
        init_pair(COLOR_ERROR, COLOR_RED, COLOR_BLACK);
        init_pair(COLOR_SUCCESS, COLOR_GREEN, COLOR_BLACK);
    }
    
    // Load config if exists
    // load_config();
    
    // Load presets
    load_presets();
}

const char* get_attack_type_str(AttackType type) {
    switch (type) {
        case ARP_REQUEST_FLOOD: return "ARP Request Flood";
        case ARP_CACHE_POISON: return "ARP Cache Poisoning";
        case PPPOE_INIT_FLOOD: return "PPPoE Init Flood";
        case PPPOE_TERMINATION: return "PPPoE Termination";
        case ICMP_ECHO_FLOOD: return "ICMP Echo Flood";
        case ICMP_SMURF: return "ICMP Smurf";
        case ICMP_TCP_RESET: return "ICMP TCP Reset";
        case TCP_SYN_FLOOD: return "TCP SYN Flood";
        case TCP_LAND: return "TCP Land";
        case TCP_BLIND_RESET: return "TCP Blind Reset";
        case UDP_FLOOD: return "UDP Flood";
        case DNS_QUERY_FLOOD: return "DNS Query Flood";
        case DHCP_DISCOVER_FLOOD: return "DHCP Discover Flood";
        case DHCP_STARVATION: return "DHCP Starvation";
        case DHCP_RELEASE_FORCING: return "DHCP Release Forcing";
        case HSRP_HIJACK: return "HSRP Hijack";
        case PACKET_ADDRESS_CONFIG: return "Packet Address Config";
        case INTELLIGENT_DETECTION: return "Intelligent Detection";
        case SMART_RANDOMIZATION: return "Smart Randomization";
        default: return "Unknown";
    }
}

void handle_input(void) {
    int ch = getch();
    
    switch (ch) {
        case KEY_F(1):
            show_config_dialog(selected_target);
            break;
        case KEY_F(2):
            show_network_config_dialog();
            break;
        case KEY_F(3):
            show_stats_chart();
            break;
        case KEY_F(4):
            save_config();
            break;
        case KEY_F(5):
            load_presets();
            break;
        case KEY_UP:
            if (selected_target > 0) selected_target--;
            break;
        case KEY_DOWN:
            if (selected_target < target_count - 1) selected_target++;
            break;
        case 'a':
        case 'A':
            show_config_dialog(-1); // Add new target
            break;
        case 'd':
        case 'D':
            if (target_count > 0) {
                // remove_target(selected_target);
                if (selected_target >= target_count) selected_target = target_count - 1;
            }
            break;
        case 's':
        case 'S':
            if (target_count > 0) {
                // start_attack_on_target(selected_target);
            }
            break;
        case 'x':
        case 'X':
            if (target_count > 0) {
                // stop_attack_on_target(selected_target);
            }
            break;
        case 'q':
        case 'Q':
            end_ui();
            exit(0);
        default:
            // Handle mouse events
            if (ch == KEY_MOUSE) {
                MEVENT event;
                if (getmouse(&event) == OK) {
                    // Handle mouse click
                }
            }
            break;
    }
}

void show_network_config_dialog(void) {
    // Create a new window for the dialog
    int height = 15, width = 60;
    int starty = (LINES - height) / 2;
    int startx = (COLS - width) / 2;
    
    WINDOW* dialog = newwin(height, width, starty, startx);
    box(dialog, 0, 0);
    
    // Dialog title
    mvwprintw(dialog, 1, 2, "Network Configuration");
    
    // Input fields
    mvwprintw(dialog, 3, 2, "Interface: ");
    mvwprintw(dialog, 4, 2, "MAC Address: ");
    mvwprintw(dialog, 5, 2, "Gateway: ");
    mvwprintw(dialog, 6, 2, "Netmask: ");
    mvwprintw(dialog, 7, 2, "Log Path: ");
    
    // Current values
    mvwprintw(dialog, 3, 15, "%s", net_interface.interface);
    mvwprintw(dialog, 4, 15, "%s", net_interface.mac_address);
    mvwprintw(dialog, 5, 15, "%s", net_interface.gateway);
    mvwprintw(dialog, 6, 15, "%s", net_interface.netmask);
    mvwprintw(dialog, 7, 15, "%s", net_interface.log_path);
    
    // Instructions
    mvwprintw(dialog, height - 3, 2, "Press any key to close...");
    
    // Refresh and wait for input
    wrefresh(dialog);
    getch();
    delwin(dialog);
}

void update_interface(void) {
    // Clear screen
    clear();
    
    // Draw header
    attron(COLOR_PAIR(COLOR_HEADER) | A_BOLD);
    mvprintw(0, 0, "DDoS Attack Tool v1.0 - Network Penetration Testing Framework");
    attroff(COLOR_PAIR(COLOR_HEADER) | A_BOLD);
    
    // Draw border
    attron(COLOR_PAIR(COLOR_PANEL_BORDER));
    box(stdscr, 0, 0);
    attroff(COLOR_PAIR(COLOR_PANEL_BORDER));
    
    // Draw network interface info panel
    mvprintw(2, 2, "Interface: %s", net_interface.interface);
    mvprintw(2, 30, "MAC: %s", net_interface.mac_address);
    mvprintw(3, 2, "Gateway: %s", net_interface.gateway);
    mvprintw(3, 30, "Netmask: %s", net_interface.netmask);
    
    // Draw system stats panel
    mvprintw(5, 2, "Packets Sent: %d", stats.packets_sent);
    mvprintw(5, 30, "Bytes Sent: %d", stats.bytes_sent);
    mvprintw(6, 2, "CPU Usage: %.2f%%", stats.cpu_usage);
    mvprintw(6, 30, "Memory Usage: %.2f%%", stats.memory_usage);
    
    // Draw target list header
    mvprintw(8, 2, "Targets:");
    mvprintw(9, 2, "IP Address       Port  Attack Type           Threads  Status");
    
    // Draw targets
    for (int i = 0; i < target_count && i < LINES - 12; i++) {
        TargetEntry* t = &targets[i];
        int color = t->running ? COLOR_TARGET_RUNNING : COLOR_TARGET_STOPPED;
        
        attron(COLOR_PAIR(color));
        mvprintw(10 + i, 2, "%-15s  %-4d  %-20s  %-7d  %s",
                t->ip, t->port, get_attack_type_str(t->attack_type),
                t->threads, t->running ? "Running" : "Stopped");
        attroff(COLOR_PAIR(color));
        
        // Highlight selected target
        if (i == selected_target) {
            mvprintw(10 + i, 1, ">");
        }
    }
    
    // Draw footer with instructions
    mvprintw(LINES - 2, 2, "F1:Add/Edit Target  F2:Network Config  F3:Stats  F4:Save  F5:Load Presets  Q:Quit");
    
    // Refresh screen
    refresh();
}

void save_config(void) {
    cJSON* root = cJSON_CreateObject();
    cJSON* targets_array = cJSON_CreateArray();
    
    // Add targets to JSON array
    for (int i = 0; i < target_count; i++) {
        cJSON* target = cJSON_CreateObject();
        cJSON_AddStringToObject(target, "ip", targets[i].ip);
        cJSON_AddNumberToObject(target, "port", targets[i].port);
        cJSON_AddNumberToObject(target, "attack_type", targets[i].attack_type);
        cJSON_AddNumberToObject(target, "threads", targets[i].threads);
        cJSON_AddItemToArray(targets_array, target);
    }
    
    cJSON_AddItemToObject(root, "targets", targets_array);
    
    // Write to file
    char* json_string = cJSON_Print(root);
    FILE* file = fopen("config/default.json", "w");
    if (file) {
        fprintf(file, "%s", json_string);
        fclose(file);
    }
    
    // 修改前: cJSON_free(json_string);
    free(json_string);  // 修改为使用标准的 free 函数
    cJSON_Delete(root);
    
    // Show success message
    mvprintw(LINES - 1, 2, "Configuration saved successfully!");
    refresh();
    sleep(1);
}

void end_ui(void) {
    save_config();
    endwin();
}

void show_config_dialog(int index) {
    // Create a new window for the dialog
    int height = 15, width = 60;
    int starty = (LINES - height) / 2;
    int startx = (COLS - width) / 2;
    
    WINDOW* dialog = newwin(height, width, starty, startx);
    box(dialog, 0, 0);
    
    // Dialog title
    mvwprintw(dialog, 1, 2, index >= 0 ? "Edit Target" : "Add New Target");
    
    // Input fields
    mvwprintw(dialog, 3, 2, "IP Address: ");
    mvwprintw(dialog, 4, 2, "Port: ");
    mvwprintw(dialog, 5, 2, "Attack Type: ");
    mvwprintw(dialog, 6, 2, "Threads: ");
    
    // Current values for editing
    char ip[16] = "";
    char port_str[8] = "";
    char threads_str[8] = "";
    int attack_type = 0;
    
    if (index >= 0 && index < target_count) {
        strcpy(ip, targets[index].ip);
        sprintf(port_str, "%d", targets[index].port);
        attack_type = targets[index].attack_type;
        sprintf(threads_str, "%d", targets[index].threads);
    }
    
    mvwprintw(dialog, 3, 15, "%s", ip);
    mvwprintw(dialog, 4, 15, "%s", port_str);
    mvwprintw(dialog, 5, 15, "%s", get_attack_type_str(attack_type));
    mvwprintw(dialog, 6, 15, "%s", threads_str);
    
    // Instructions
    mvwprintw(dialog, height - 3, 2, "Press any key to close...");
    
    // Refresh and wait for input
    wrefresh(dialog);
    getch();
    delwin(dialog);
}

void show_stats_chart(void) {
    // Create a new window for the chart
    int height = 20, width = 60;
    int starty = (LINES - height) / 2;
    int startx = (COLS - width) / 2;
    
    WINDOW* chart = newwin(height, width, starty, startx);
    box(chart, 0, 0);
    
    // Chart title
    mvwprintw(chart, 1, 2, "Attack Statistics Chart");
    
    // Draw simple bar chart (simulated data)
    mvwprintw(chart, 3, 2, "Packets Sent:");
    for (int i = 0; i < 20; i++) {
        mvwprintw(chart, 4, 2 + i, "*");
    }
    
    mvwprintw(chart, 6, 2, "Bytes Sent:");
    for (int i = 0; i < 15; i++) {
        mvwprintw(chart, 7, 2 + i, "*");
    }
    
    // Instructions
    mvwprintw(chart, height - 3, 2, "Press any key to close...");
    
    // Refresh and wait for input
    wrefresh(chart);
    getch();
    delwin(chart);
}

void load_presets(void) {
    DIR* dir = opendir("presets");
    if (!dir) return;
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        // Check if file has .jsonc extension
        if (strstr(entry->d_name, ".jsonc") != NULL) {
            // Load and parse preset file
            char filepath[260];
            snprintf(filepath, sizeof(filepath), "presets/%s", entry->d_name);
            // parse_preset_file(filepath);
        }
    }
    
    closedir(dir);
}