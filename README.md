# ROS Config CLI

ROS Config CLI is a command-line interface (CLI) tool designed to simplify the management and configuration of settings in the Robot Operating System (ROS) environment.

## Features

- **File Reading**: Read and parse configuration files relevant to ROS.
- **View**: View existing URI configurations.
- **Add**: Add new URI configurations.
- **Delete**: Delete URI configurations.

## Usage

1. **Installation**: Clone this repository to your local machine.
```bash
git clone https://github.com/Ozyegin-Planetary-Robotics-Laboratory/ROS-Config-CLI.git
```

2. **Build**: Run the provided Makefile to build the executable:
```bash
   make
```
3. **Run**: Executr the main program:
```bash
    cd bin
   ./main
```

## Dependencies

- **ncurses**: The NCurses library for creating text-based user interfaces.
- **menu**: The menu library, a part of the NCurses package, for creating interactive menus.


