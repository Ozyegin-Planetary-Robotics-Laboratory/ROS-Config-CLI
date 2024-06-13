# ROS Config CLI

ROS Config CLI is a command-line interface (CLI) tool designed to simplify the management and configuration of settings in the Robot Operating System (ROS) environment.

## Features

- **View**: Display existing URI configurations.
- **Add**: Add new URI configurations.
- **Delete**: Remove URI configurations.
- **View System Env**: View and manage ROS-related environment variables.
- **Update System Env**: Set ROS_MASTER_URI and ROS_IP based on selected configurations.

## Usage

### Installation

Clone this repository to your local machine:

```bash
git clone https://github.com/Ozyegin-Planetary-Robotics-Laboratory/ROS-Config-CLI.git
```

Navigate to the cloned directory and build the executable using the provided Makefile:

```bash
cd ROS-Config-CLI
make
```

After building, navigate to the bin directory and execute the main program:
```bash
cd bin
./main
```


## Dependencies

- **ncurses**: A library for creating text-based user interfaces in the terminal.
- **form**: A component of ncurses that allows you to create and handle forms, which are used to collect user input.
- **menu**: Another component of ncurses that enables you to create menus for user interaction.

Make sure these dependencies are installed on your system before attempting to build and run ROS Config CLI.
