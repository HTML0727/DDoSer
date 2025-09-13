# DDoS攻击工具 (DDoSer)

这是一个为安全研究和教育目的设计的网络渗透测试框架。它实现了多种类型的网络攻击，以帮助理解并防御DDoS威胁。

## 功能特性

- 多种攻击类型，包括：
  - 基于ARP的攻击
  - PPPoE攻击
  - 基于ICMP的攻击
  - 基于TCP的攻击
  - 基于UDP的攻击
  - DNS攻击
  - DHCP攻击
  - Cisco HSRP攻击
  - 智能地址检测和随机化
- 可配置的攻击目标和参数
- 实时统计监控
- 预设攻击配置
- 使用ncurses的用户友好终端界面

## 环境要求

- GCC编译器
- ncurses库
- cJSON库
- 类Unix环境（Linux/macOS）

## 安装步骤

1. 克隆仓库：
   ```
   git clone https://github.com/yourusername/DDoSer.git
   cd DDoSer
   ```

2. 安装依赖：
   ```
   # Ubuntu/Debian系统
   sudo apt-get install libncurses5-dev libcjson-dev
   
   # macOS系统（使用Homebrew）
   brew install ncurses cjson
   ```

3. 编译项目：
   ```
   make
   ```

## 使用方法

1. 运行工具：
   ```
   sudo ./ddos-tool
   ```

2. 使用以下键盘快捷键：
   - F1: 添加/编辑目标
   - F2: 网络配置
   - F3: 查看统计信息
   - F4: 保存配置
   - F5: 加载预设
   - Q: 退出

## 免责声明

本工具仅用于教育和研究目的。未经授权使用本工具攻击网络是非法且不道德的。开发者不对本软件的任何滥用行为负责。

## 许可证

本项目采用MIT许可证 - 详见[LICENSE](LICENSE)文件了解详情。