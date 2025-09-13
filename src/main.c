#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <locale.h>
#include "attack.h"
#include "network.h"
#include "ui.h"

// 全局配置结构
typedef struct {
	char config_file[256];
	int debug_mode;
	char log_file[256];
} Config;

static Config g_config = {0};

// 版本信息
#define VERSION "0.01a"
#define AUTHOR "DDoser"

void show_help(const char *program_name) {
	printf("Usage: %s [OPTIONS]\n", program_name);
	printf("A powerful DDoS attack tool with multiple attack vectors.\n\n");
	printf("Options:\n");
	printf("  -c, --config FILE     Specify configuration file\n");
	printf("  -d, --debug           Enable debug mode\n");
	printf("  -h, --help            Show this help message\n");
	printf("  -l, --log FILE        Specify log file path\n");
	printf("  -V, --version         Show version information\n\n");
	printf("Examples:\n");
	printf("  %s -c config.json     # Load configuration from file\n", program_name);
	printf("  %s -d                 # Run in debug mode\n", program_name);
	printf("  %s -l ddos.log        # Log to specific file\n", program_name);
}

void show_version(void) {
	printf("DDoser v%s\n", VERSION);
	printf("Copyright (C) 2023 %s\n", AUTHOR);
	printf("License: MIT\n\n");
	printf("This is free software; you are free to change and redistribute it.\n");
	printf("There is NO WARRANTY, to the extent permitted by law.\n");
}

int parse_arguments(int argc, char *argv[]) {
	static struct option long_options[] = {
		{"config", required_argument, 0, 'c'},
		{"debug", no_argument, 0, 'd'},
		{"help", no_argument, 0, 'h'},
		{"log", required_argument, 0, 'l'},
		{"version", no_argument, 0, 'V'},
		{0, 0, 0, 0}
	};
	
	int opt;
	int option_index = 0;
	
	while ((opt = getopt_long(argc, argv, "c:dhl:V", long_options, &option_index)) != -1) {
		switch (opt) {
		case 'c':
			strncpy(g_config.config_file, optarg, sizeof(g_config.config_file) - 1);
			g_config.config_file[sizeof(g_config.config_file) - 1] = '\0';
			break;
		case 'd':
			g_config.debug_mode = 1;
			break;
		case 'h':
			show_help(argv[0]);
			return 0;
		case 'l':
			strncpy(g_config.log_file, optarg, sizeof(g_config.log_file) - 1);
			g_config.log_file[sizeof(g_config.log_file) - 1] = '\0';
			break;
		case 'V':
			show_version();
			return 0;
		default:
			fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
			return -1;
		}
	}
	
	return 1;
}

int main(int argc, char *argv[]) {
	// 设置locale以支持UTF-8
	setlocale(LC_ALL, "");
	
	// 解析命令行参数
	int result = parse_arguments(argc, argv);
	if (result <= 0) {
		return result == 0 ? 0 : 1;
	}
	
	// 初始化日志系统
	if (g_config.log_file[0] != '\0') {
		// 这里应该初始化日志系统，但目前我们只是打印信息
		printf("Logging to file: %s\n", g_config.log_file);
		// TODO: 实际的日志系统初始化
	}
	
	// 初始化网络模块
	if (init_network() != 0) {
		fprintf(stderr, "Failed to initialize network module\n");
		return 1;
	}
	
	// 初始化攻击模块
	init_attack_modules();
	
	// 初始化用户界面
	init_ui();
	
	// 主循环
	while (1) {
		update_interface();
		handle_input();
	}
	
	// 清理资源
	end_ui();
	cleanup_network();
	
	return 0;
}