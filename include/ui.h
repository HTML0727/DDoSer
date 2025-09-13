#ifndef UI_H
#define UI_H

#include <ncurses.h>
#include "attack.h"

#define MAX_TARGETS 100

// UI component structures
typedef struct {
    char interface[32];
    char mac_address[18];
    char gateway[16];
    char netmask[16];
    char log_path[256];
} NetworkInterface;

typedef struct {
    char ip[16];
    int port;
    AttackType attack_type;
    int threads;
    int running;
} TargetEntry;

typedef struct {
    int packets_sent;
    int bytes_sent;
    double cpu_usage;
    double memory_usage;
} SystemStats;

// Function declarations
void init_ui(void);
void handle_input(void);
void update_interface(void);
void end_ui(void);
void show_config_dialog(int index);
void show_network_config_dialog(void);
void show_stats_chart(void);
void load_presets(void);
void save_config(void);
const char* get_attack_type_str(AttackType type);

#endif // UI_H