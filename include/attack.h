#ifndef ATTACK_H
#define ATTACK_H

#include <time.h>

// 最大攻击类型数量
#define MAX_ATTACK_TYPES 19
// 最大日志条目数
#define MAX_LOG_ENTRIES 1000

// 攻击类型枚举
typedef enum {
    ARP_REQUEST_FLOOD,        // ARP Request Flood
    ARP_CACHE_POISON,         // ARP Cache Poisoning
    PPPOE_INIT_FLOOD,         // PPPoE Session Initiation Flood
    PPPOE_TERMINATION,        // Blind PPPoE Session Termination
    ICMP_ECHO_FLOOD,          // ICMP Echo Flood
    ICMP_SMURF,               // ICMP Smurf Attack
    ICMP_TCP_RESET,           // ICMP-based TCP Connection Reset
    TCP_SYN_FLOOD,            // TCP SYN Flood
    TCP_LAND,                 // TCP Land Attack
    TCP_BLIND_RESET,          // Blind TCP Connection Reset
    UDP_FLOOD,                // UDP Flood
    DNS_QUERY_FLOOD,          // DNS Query Flood
    DHCP_DISCOVER_FLOOD,      // DHCP Discover Flood
    DHCP_STARVATION,          // DHCP Starvation Attack
    DHCP_RELEASE_FORCING,     // DHCP Release Forcing
    HSRP_HIJACK,              // Cisco HSRP Active Router Hijacking
    PACKET_ADDRESS_CONFIG,    // Packet Address Configuration
    INTELLIGENT_DETECTION,    // Intelligent Address Detection
    SMART_RANDOMIZATION       // Smart Wildcard-based Randomization
} AttackType;

// 攻击日志结构
typedef struct {
    time_t timestamp;         // Timestamp
    AttackType attack_type;   // Attack type
    char target_ip[16];       // Target IP address
    int port;                 // Target port
    int packets_sent;         // Number of packets sent
    int bytes_sent;           // Number of bytes sent
} AttackLog;

// Target structure
typedef struct Target {
    char ip[16];              // IP address
    int port;                 // Port
    AttackType attack_type;   // Attack type
    int thread_count;         // Number of threads
    int running;              // Is running
} Target;

// Attack context structure
typedef struct {
    Target* target;           // Target
    AttackType attack_type;   // Attack type
    void* config;             // Configuration parameters
} AttackContext;

// Attack module structure
typedef struct {
    const char* name;         // Attack name
    void (*handler)(AttackContext* ctx);  // Attack handler function
    void (*setup)(void* config);          // Attack setup function
} AttackModule;

// Function declarations
const char* get_attack_name(AttackType type);
void log_attack(AttackType type, const char* target_ip, int port, int packets_sent, int bytes_sent);
void init_attack_modules(void);
void start_attack(AttackType type, Target* target);

#endif // ATTACK_H