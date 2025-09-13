#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "attack.h"
#include "network.h"

// Attack modules array
AttackModule attacks[MAX_ATTACK_TYPES];
int attack_count = 0;

// Attack log array
AttackLog logs[MAX_LOG_ENTRIES];
int log_count = 0;
pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

// Attack name mapping table
static const char* attack_names[] = {
    [ARP_REQUEST_FLOOD] = "ARP Request Flood",
    [ARP_CACHE_POISON] = "ARP Cache Poisoning",
    [PPPOE_INIT_FLOOD] = "PPPoE Session Initiation Flood",
    [PPPOE_TERMINATION] = "Blind PPPoE Session Termination",
    [ICMP_ECHO_FLOOD] = "ICMP Echo Flood",
    [ICMP_SMURF] = "ICMP Smurf Attack",
    [ICMP_TCP_RESET] = "ICMP-based TCP Connection Reset",
    [TCP_SYN_FLOOD] = "TCP SYN Flood",
    [TCP_LAND] = "TCP Land Attack",
    [TCP_BLIND_RESET] = "Blind TCP Connection Reset",
    [UDP_FLOOD] = "UDP Flood",
    [DNS_QUERY_FLOOD] = "DNS Query Flood",
    [DHCP_DISCOVER_FLOOD] = "DHCP Discover Flood",
    [DHCP_STARVATION] = "DHCP Starvation Attack",
    [DHCP_RELEASE_FORCING] = "DHCP Release Forcing",
    [HSRP_HIJACK] = "Cisco HSRP Active Router Hijacking",
    [PACKET_ADDRESS_CONFIG] = "Packet Address Configuration",
    [INTELLIGENT_DETECTION] = "Intelligent Address Detection",
    [SMART_RANDOMIZATION] = "Smart Wildcard-based Randomization"
};

const char* get_attack_name(AttackType type) {
    if (type >= 0 && type < sizeof(attack_names)/sizeof(attack_names[0])) {
        return attack_names[type];
    }
    return "Unknown Attack";
}

void log_attack(AttackType type, const char* target_ip, int port, int packets_sent, int bytes_sent) {
    pthread_mutex_lock(&log_mutex);
    
    if (log_count < MAX_LOG_ENTRIES) {
        logs[log_count].timestamp = time(NULL);
        logs[log_count].attack_type = type;
        strncpy(logs[log_count].target_ip, target_ip, sizeof(logs[log_count].target_ip) - 1);
        logs[log_count].target_ip[sizeof(logs[log_count].target_ip) - 1] = '\0';
        logs[log_count].port = port;
        logs[log_count].packets_sent = packets_sent;
        logs[log_count].bytes_sent = bytes_sent;
        log_count++;
    }
    
    pthread_mutex_unlock(&log_mutex);
    
    // Print log to console
    time_t now = time(NULL);
    char* time_str = ctime(&now);
    time_str[strlen(time_str) - 1] = '\0'; // Remove newline
    
    printf("[%s] %s attack on %s:%d - Packets: %d, Bytes: %d\n", 
           time_str, get_attack_name(type), target_ip, port, packets_sent, bytes_sent);
}

// Attack handler function implementations
void arp_request_flood_handler(AttackContext* ctx) {
    printf("Executing ARP Request Flood on %s\n", ctx->target->ip);
    log_attack(ARP_REQUEST_FLOOD, ctx->target->ip, ctx->target->port, 1000, 60000);
}

void arp_cache_poison_handler(AttackContext* ctx) {
    printf("Executing ARP Cache Poisoning on %s\n", ctx->target->ip);
    log_attack(ARP_CACHE_POISON, ctx->target->ip, ctx->target->port, 500, 30000);
}

void pppoe_init_flood_handler(AttackContext* ctx) {
    printf("Executing PPPoE Session Initiation Flood on %s\n", ctx->target->ip);
    log_attack(PPPOE_INIT_FLOOD, ctx->target->ip, ctx->target->port, 800, 48000);
}

void pppoe_termination_handler(AttackContext* ctx) {
    printf("Executing Blind PPPoE Session Termination on %s\n", ctx->target->ip);
    log_attack(PPPOE_TERMINATION, ctx->target->ip, ctx->target->port, 300, 18000);
}

void icmp_echo_flood_handler(AttackContext* ctx) {
    printf("Executing ICMP Echo Flood on %s\n", ctx->target->ip);
    log_attack(ICMP_ECHO_FLOOD, ctx->target->ip, ctx->target->port, 1200, 72000);
}

void icmp_smurf_handler(AttackContext* ctx) {
    printf("Executing ICMP Smurf Attack on %s\n", ctx->target->ip);
    log_attack(ICMP_SMURF, ctx->target->ip, ctx->target->port, 1500, 90000);
}

void icmp_tcp_reset_handler(AttackContext* ctx) {
    printf("Executing ICMP-based TCP Connection Reset on %s\n", ctx->target->ip);
    log_attack(ICMP_TCP_RESET, ctx->target->ip, ctx->target->port, 400, 24000);
}

void tcp_syn_flood_handler(AttackContext* ctx) {
    printf("Executing TCP SYN Flood on %s\n", ctx->target->ip);
    log_attack(TCP_SYN_FLOOD, ctx->target->ip, ctx->target->port, 2000, 120000);
}

void tcp_land_handler(AttackContext* ctx) {
    printf("Executing TCP Land Attack on %s\n", ctx->target->ip);
    log_attack(TCP_LAND, ctx->target->ip, ctx->target->port, 250, 15000);
}

void tcp_blind_reset_handler(AttackContext* ctx) {
    printf("Executing Blind TCP Connection Reset on %s\n", ctx->target->ip);
    log_attack(TCP_BLIND_RESET, ctx->target->ip, ctx->target->port, 600, 36000);
}

void udp_flood_handler(AttackContext* ctx) {
    printf("Executing UDP Flood on %s\n", ctx->target->ip);
    log_attack(UDP_FLOOD, ctx->target->ip, ctx->target->port, 1800, 108000);
}

void dns_query_flood_handler(AttackContext* ctx) {
    printf("Executing DNS Query Flood on %s\n", ctx->target->ip);
    log_attack(DNS_QUERY_FLOOD, ctx->target->ip, ctx->target->port, 900, 54000);
}

void dhcp_discover_flood_handler(AttackContext* ctx) {
    printf("Executing DHCP Discover Flood on %s\n", ctx->target->ip);
    log_attack(DHCP_DISCOVER_FLOOD, ctx->target->ip, ctx->target->port, 400, 24000);
}

void dhcp_starvation_handler(AttackContext* ctx) {
    printf("Executing DHCP Starvation Attack on %s\n", ctx->target->ip);
    log_attack(DHCP_STARVATION, ctx->target->ip, ctx->target->port, 700, 42000);
}

void dhcp_release_forcing_handler(AttackContext* ctx) {
    printf("Executing DHCP Release Forcing on %s\n", ctx->target->ip);
    log_attack(DHCP_RELEASE_FORCING, ctx->target->ip, ctx->target->port, 350, 21000);
}

void hsrp_hijack_handler(AttackContext* ctx) {
    printf("Executing Cisco HSRP Active Router Hijacking on %s\n", ctx->target->ip);
    log_attack(HSRP_HIJACK, ctx->target->ip, ctx->target->port, 300, 18000);
}

void packet_address_config_handler(AttackContext* ctx) {
    printf("Executing Packet Address Configuration on %s\n", ctx->target->ip);
    log_attack(PACKET_ADDRESS_CONFIG, ctx->target->ip, ctx->target->port, 500, 30000);
}

void intelligent_detection_handler(AttackContext* ctx) {
    printf("Executing Intelligent Address Detection on %s\n", ctx->target->ip);
    log_attack(INTELLIGENT_DETECTION, ctx->target->ip, ctx->target->port, 600, 36000);
}

void smart_randomization_handler(AttackContext* ctx) {
    printf("Executing Smart Wildcard-based Randomization on %s\n", ctx->target->ip);
    log_attack(SMART_RANDOMIZATION, ctx->target->ip, ctx->target->port, 800, 48000);
}

void init_attack_modules(void) {
    // Initialize attack modules
    attacks[ARP_REQUEST_FLOOD] = (AttackModule){
        .name = "ARP Request Flood",
        .handler = arp_request_flood_handler,
        .setup = NULL
    };
    
    attacks[ARP_CACHE_POISON] = (AttackModule){
        .name = "ARP Cache Poisoning",
        .handler = arp_cache_poison_handler,
        .setup = NULL
    };
    
    attacks[PPPOE_INIT_FLOOD] = (AttackModule){
        .name = "PPPoE Session Initiation Flood",
        .handler = pppoe_init_flood_handler,
        .setup = NULL
    };
    
    attacks[PPPOE_TERMINATION] = (AttackModule){
        .name = "Blind PPPoE Session Termination",
        .handler = pppoe_termination_handler,
        .setup = NULL
    };
    
    attacks[ICMP_ECHO_FLOOD] = (AttackModule){
        .name = "ICMP Echo Flood",
        .handler = icmp_echo_flood_handler,
        .setup = NULL
    };
    
    attacks[ICMP_SMURF] = (AttackModule){
        .name = "ICMP Smurf Attack",
        .handler = icmp_smurf_handler,
        .setup = NULL
    };
    
    attacks[ICMP_TCP_RESET] = (AttackModule){
        .name = "ICMP-based TCP Connection Reset",
        .handler = icmp_tcp_reset_handler,
        .setup = NULL
    };
    
    attacks[TCP_SYN_FLOOD] = (AttackModule){
        .name = "TCP SYN Flood",
        .handler = tcp_syn_flood_handler,
        .setup = NULL
    };
    
    attacks[TCP_LAND] = (AttackModule){
        .name = "TCP Land Attack",
        .handler = tcp_land_handler,
        .setup = NULL
    };
    
    attacks[TCP_BLIND_RESET] = (AttackModule){
        .name = "Blind TCP Connection Reset",
        .handler = tcp_blind_reset_handler,
        .setup = NULL
    };
    
    attacks[UDP_FLOOD] = (AttackModule){
        .name = "UDP Flood",
        .handler = udp_flood_handler,
        .setup = NULL
    };
    
    attacks[DNS_QUERY_FLOOD] = (AttackModule){
        .name = "DNS Query Flood",
        .handler = dns_query_flood_handler,
        .setup = NULL
    };
    
    attacks[DHCP_DISCOVER_FLOOD] = (AttackModule){
        .name = "DHCP Discover Flood",
        .handler = dhcp_discover_flood_handler,
        .setup = NULL
    };
    
    attacks[DHCP_STARVATION] = (AttackModule){
        .name = "DHCP Starvation Attack",
        .handler = dhcp_starvation_handler,
        .setup = NULL
    };
    
    attacks[DHCP_RELEASE_FORCING] = (AttackModule){
        .name = "DHCP Release Forcing",
        .handler = dhcp_release_forcing_handler,
        .setup = NULL
    };
    
    attacks[HSRP_HIJACK] = (AttackModule){
        .name = "Cisco HSRP Active Router Hijacking",
        .handler = hsrp_hijack_handler,
        .setup = NULL
    };
    
    attacks[PACKET_ADDRESS_CONFIG] = (AttackModule){
        .name = "Packet Address Configuration",
        .handler = packet_address_config_handler,
        .setup = NULL
    };
    
    attacks[INTELLIGENT_DETECTION] = (AttackModule){
        .name = "Intelligent Address Detection",
        .handler = intelligent_detection_handler,
        .setup = NULL
    };
    
    attacks[SMART_RANDOMIZATION] = (AttackModule){
        .name = "Smart Wildcard-based Randomization",
        .handler = smart_randomization_handler,
        .setup = NULL
    };
    
    attack_count = SMART_RANDOMIZATION + 1;
}

void start_attack(AttackType type, Target* target) {
    if (type >= 0 && type < MAX_ATTACK_TYPES && attacks[type].handler) {
        AttackContext ctx = {
            .target = target,
            .attack_type = type
        };
        attacks[type].handler(&ctx);
    }
}