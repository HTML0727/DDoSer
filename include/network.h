#ifndef NETWORK_H
#define NETWORK_H

#include <pthread.h>
#include "attack.h"

#define MAX_TARGETS 100

// Function declarations
int init_network(void);
void cleanup_network(void);
int add_target(const char* ip, int port, AttackType type, int threads);
int remove_target(int index);
void start_attack_on_target(int index);
void stop_attack_on_target(int index);
void* attack_worker(void* arg);
int validate_ip(const char* ip);
int validate_port(int port);

#endif // NETWORK_H