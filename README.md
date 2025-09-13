# DDoS Attack Tool (DDoSer)

This is a network penetration testing framework designed for security research and educational purposes. It implements various types of network attacks to help understand and defend against DDoS threats.

## About Project Structure
```
DDoSer/
├── Makefile
├── README.md
├── ZH.README.md
├── .gitignore
├── LICENSE
├── include/
│   ├── attack.h
│   ├── network.h
│   └── ui.h
└── src/
    ├── attack.c
    ├── main.c
    ├── network.c
    └── ui.c
```

## Features

- Multiple attack types including:
  - ARP-based attacks
  - PPPoE attacks
  - ICMP-based attacks
  - TCP-based attacks
  - UDP-based attacks
  - DNS attacks
  - DHCP attacks
  - Cisco HSRP attacks
  - Intelligent address detection and randomization
- Configurable attack targets and parameters
- Real-time statistics monitoring
- Preset attack configurations
- User-friendly terminal interface with ncurses

## Prerequisites

- GCC compiler
- ncurses library
- cJSON library
- Unix-like environment (Linux/macOS)

## Installation

1. Clone the repository:
   ```
   git clone https://github.com/yourusername/DDoSer.git
   cd DDoSer
   ```

2. Install dependencies:
   ```
   # On Ubuntu/Debian
   sudo apt-get install libncurses5-dev libcjson-dev

   # On Arch Linux
   sudo pacman -S base-devel ncurses cjson

   # On Gentoo Linux
   sudo emerge dev-libs/ncurses dev-libs/cjson

   # On macOS with Homebrew
   brew install ncurses cjson
   ```

3. Compile the project:
   ```
   make
   ```

## Usage

1. Run the tool:
   ```
   sudo ./ddos-tool
   ```

2. Use the following keyboard shortcuts:
   - F1: Add/Edit Target
   - F2: Network Configuration
   - F3: View Statistics
   - F4: Save Configuration
   - F5: Load Presets
   - Q: Quit

## Plan

- [ ] Add more attack types
- [ ] Improve network configuration options
- [ ] Add support for Windows
- [ ] Enhance user interface with ncurses
- [ ] Add more attack parameters

## Disclaimer

This tool is for educational and research purposes only. Unauthorized use of this tool to attack networks is illegal and unethical. The developers are not responsible for any misuse of this software.

## License

This project is licensed under the NC-OSL License - see the [LICENSE](LICENSE) file for details.
