#ifndef MENU_FUNCTIONS_HPP
#define MENU_FUNCTIONS_HPP

#include <ncurses.h>
#include <menu.h>
#include <form.h>
#include <cstdlib> 
#include <cstring>
#include <cstdlib> 
#include <string>


#include "helpers.hpp"
#include "rosnetconfig.hpp"


void view_rosconfig(WINDOW *main_win, WINDOW *info_win) {
    std::vector<RosNetConfig> configs = get_rosconfig(info_win);
    if (configs.empty()) {
        view_message_at_info(info_win, "No configurations found", 0, 0);
        return;
    }

    view_message_at_info(info_win, "Found configurations:", 0, 0);

    // View configurations in main_win
    int y = 1;

    for (const auto& config : configs) {
        mvwprintw(main_win, y, 1, "%d) Name: %s ; Server IP: %s ; Client IP: %s", y,config.name.c_str(),config.server_ip.c_str(),config.client_ip.c_str());
        y++;
    }

    wrefresh(main_win);
}

void delete_from_rosconfig(WINDOW *main_win, WINDOW *info_win) {
    std::vector<RosNetConfig> configs = get_rosconfig(info_win);

    if (configs.empty()) {
        view_message_at_info(info_win, "No configurations found", 0, 0);
        return;
    }

    view_message_at_info(info_win, "Found configurations to delete:", 0, 0);

    // Display configurations in main_win
    int y = 1;

    for (size_t i = 0; i < configs.size(); ++i) {
        mvwprintw(main_win, y, 1, "%d) Name: %s ; Server IP: %s ; Client IP: %s", i + 1,
                  configs[i].name.c_str(), configs[i].server_ip.c_str(), configs[i].client_ip.c_str());
        y++;
    }

    mvwprintw(main_win, y + 2, 1, "Enter the number of the configuration you want to delete (or press 'q' to cancel): ");
    wrefresh(main_win);

    while (true) {
        char input[10] = {0};
        int pos = 0;
        int ch;

        while ((ch = getch()) != '\n') {
            if (isdigit(ch)) {
                input[pos++] = ch;
                mvwprintw(main_win, y + 2, 1, "Enter the number of the configuration you want to delete (or press 'q' to cancel): %s", input);
                wrefresh(main_win);
            } else if (ch == 'q' || ch == 'Q') {
                view_message_at_info(info_win, "Deletion canceled", 0, 0);
                return;
            } else if (ch == KEY_DC) {
                view_message_at_info(info_win, "Delete key pressed. Please enter a valid number", 0, 0);
                return;
            }
        }

        int choice = atoi(input);

        if (choice >= 1 && static_cast<size_t>(choice) <= configs.size()) {
            configs.erase(configs.begin() + choice - 1);
            update_rosconfig_file(configs);
            view_message_at_info(info_win, "Configuration deleted successfully", 0, 0);
            break;
        } else {
            view_message_at_info(info_win, "Invalid choice. Please enter a valid number", 0, 0);
        }
    }
}


void add_to_rosconfig(WINDOW *main_win, WINDOW *info_win) {
    std::vector<RosNetConfig> configs = get_rosconfig(info_win);

    if (configs.size() >= 5) {
        view_message_at_info(info_win, "Cannot have more than 5 configurations", 0, 0);
        return;
    }

    FIELD *fields[7];
    fields[0] = new_field(1, 10, 0, 0, 0, 0); // Label for name
    fields[1] = new_field(1, 40, 0, 15, 0, 0); // Input for name
    fields[2] = new_field(1, 10, 2, 0, 0, 0); // Label for server IP
    fields[3] = new_field(1, 40, 2, 15, 0, 0); // Input for server IP
    fields[4] = new_field(1, 10, 4, 0, 0, 0); // Label for client IP
    fields[5] = new_field(1, 40, 4, 15, 0, 0); // Input for client IP
    fields[6] = NULL; // End of fields

    set_field_buffer(fields[0], 0, "Name");
    set_field_buffer(fields[1], 0, "");
    set_field_buffer(fields[2], 0, "Server IP");
    set_field_buffer(fields[3], 0, "");
    set_field_buffer(fields[4], 0, "Client IP");
    set_field_buffer(fields[5], 0, "");

    set_field_opts(fields[0], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(fields[1], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
    set_field_opts(fields[2], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(fields[3], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
    set_field_opts(fields[4], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(fields[5], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);

    set_field_back(fields[1], A_UNDERLINE);
    set_field_back(fields[3], A_UNDERLINE);
    set_field_back(fields[5], A_UNDERLINE);

    FORM *form = new_form(fields);
    set_form_win(form, main_win);
    set_form_sub(form, derwin(main_win, 10, 60, 1, 1));
    post_form(form);

    refresh();
    wrefresh(main_win);

    view_message_at_info(info_win, "Use arrow up and down to move", 0, 0);
    int ch;
    curs_set(1);


    while ((ch = getch()) != '\n') {
        driver(ch, form, fields, main_win);
    }
    curs_set(0);

    form_driver(form, REQ_NEXT_FIELD);
    form_driver(form, REQ_PREV_FIELD);

    std::string name = trim_whitespaces(field_buffer(fields[1], 0));
    std::string server_ip = trim_whitespaces(field_buffer(fields[3], 0));
    std::string client_ip = trim_whitespaces(field_buffer(fields[5], 0));

    if (!name.empty() && !server_ip.empty() && !client_ip.empty()) {
        if(!check_ip_validity(server_ip.c_str()) || !check_ip_validity(client_ip.c_str())) {
            view_message_at_info(info_win, "Invalid IP address", 0, 0);
            return;
        }
        RosNetConfig new_config{name, server_ip, client_ip};
        configs.push_back(new_config);
        update_rosconfig_file(configs);
        char message[256];
        snprintf(message, sizeof(message), "Configuration added successfully: Name: %s, Server IP: %s, Client IP: %s", name.c_str(), server_ip.c_str(), client_ip.c_str());
        view_message_at_info(info_win, message, 0, 0);    } else {
        view_message_at_info(info_win, "All fields are required", 0, 0);
    }

    unpost_form(form);
    free_form(form);
    for (int i = 0; i < 6; i++) {
        free_field(fields[i]);
    }
}

void view_system_env(WINDOW *main_win, WINDOW *info_win) {

    std::string rcFile = std::string(getenv("HOME")) + "/.bashrc";

    std::ifstream zshFile(std::string(getenv("HOME")) + "/.zshrc");
    if (zshFile.good()) {
        rcFile = std::string(getenv("HOME")) + "/.zshrc";
        zshFile.close();
    }

    std::ifstream file(rcFile);
    std::string line;
    bool foundMarker = false;
    bool foundROSConfig = false;
    std::string rosMasterURI;
    std::string rosIP;

    std::string markerLine = "# The following lines are automatically generated by ros-config-cli, do not modify under any circumstances.";

    if (file.is_open()) {
        while (std::getline(file, line)) {
            if (!foundMarker) {
                if (line.find(markerLine) != std::string::npos) {
                    foundMarker = true;
                }
            } else {
                if (line.find("export ROS_MASTER_URI=") != std::string::npos) {
                    rosMasterURI = line.substr(line.find("=") + 1);  
                    foundROSConfig = true;
                } else if (line.find("export ROS_IP") != std::string::npos) {
                    rosIP = line.substr(line.find("=") + 1);  
                    foundROSConfig = true;
                }
            }

            if (foundROSConfig && !rosMasterURI.empty() && !rosIP.empty()) {
                break;
            }
        }
        file.close();

        werase(info_win); 

        if (foundROSConfig) {
            view_message_at_info(info_win, "ROS settings found:", 0, 0);
            mvwprintw(main_win, 2, 1, "ROS_MASTER_URI: %s", rosMasterURI.c_str());
            mvwprintw(main_win, 3, 1, "ROS_IP: %s", rosIP.c_str());

        } else {
            view_message_at_info(info_win, "No ROS settings found", 0, 0);
        }
    } else {
        view_message_at_info(info_win, "Error reading file", 0, 0);
    }
}

void update_system_env(WINDOW *main_win, WINDOW *info_win){
    
    std::vector<RosNetConfig> configs = get_rosconfig(info_win);
    if (configs.empty()) {
        view_message_at_info(info_win, "No configurations found", 0, 0);
        return;
    }

    view_message_at_info(info_win, "Found configurations:", 0, 0);
    int y = 1;

    for (const auto& config : configs) {
        mvwprintw(main_win, y, 1, "%d) Name: %s ; Server IP: %s ; Client IP: %s", y,config.name.c_str(),config.server_ip.c_str(),config.client_ip.c_str());
        y++;
    }

    wrefresh(main_win);


    mvwprintw(main_win, y + 2, 1, "Enter the number of the config you want to select as ROS_MASTER_URI and ROS_IP: ");
    wrefresh(main_win);

    int selectedConfig = 0;
    std::string userInput;


    while (true) {
        mvwprintw(main_win, y + 3, 1, userInput.c_str()); 
        wrefresh(main_win);

        int ch = wgetch(main_win);

        if (ch == '\n') { 
            break;
        } else if (ch == KEY_BACKSPACE || ch == 127) { 
            if (!userInput.empty()) {
                userInput.pop_back();
            }
        } else if (ch >= '0' && ch <= '9') { 
            userInput += ch;
        }
    }

    selectedConfig = atoi(userInput.c_str());

    if (selectedConfig < 1 || selectedConfig > static_cast<int>(configs.size())) {
        view_message_at_info(info_win, "Invalid selection", 0, 0);
        return;
    }

    const RosNetConfig& selectedConfigData = configs[selectedConfig - 1]; 


    std::string rcFileBash = std::string(getenv("HOME")) + "/.bashrc";
    std::string rcFileZsh = std::string(getenv("HOME")) + "/.zshrc";
    std::string rcFile;

    // Check which file exists
    if (access(rcFileBash.c_str(), F_OK) != -1) {
        rcFile = rcFileBash;
    } else if (access(rcFileZsh.c_str(), F_OK) != -1) {
        rcFile = rcFileZsh;
    } else {
        view_message_at_info(info_win, "Error: No shell configuration file found", 0, 0);
        return;
    }

    std::string markerLine = "# The following lines are automatically generated by ros-config-cli, do not modify under any circumstances.";

    std::ifstream inFile(rcFile);
    std::ofstream outFile(rcFile + ".tmp"); 

    if (!inFile.is_open() || !outFile.is_open()) {
        view_message_at_info(info_win, "Error: Unable to open file for reading or writing", 0, 0);
        return;
    }

    bool foundMarker = false;
    std::string line;

    
    while (std::getline(inFile, line)) {
        if (!foundMarker && line.find(markerLine) != std::string::npos) {
            foundMarker = true;
            outFile << line << std::endl;
            
            std::getline(inFile, line); 
            std::getline(inFile, line); 
            outFile << "export ROS_MASTER_URI=http://" << selectedConfigData.server_ip << ":11311" << std::endl;
            outFile << "export ROS_IP=" << selectedConfigData.client_ip << std::endl;
        } else {
            outFile << line << std::endl;
        }
    }

    if (!foundMarker) {
        outFile << std::endl << markerLine << std::endl;
        outFile << "export ROS_MASTER_URI=" << selectedConfigData.server_ip << std::endl;
        outFile << "export ROS_IP=" << selectedConfigData.client_ip << std::endl;
    }

    inFile.close();
    outFile.close();

    if (std::rename((rcFile + ".tmp").c_str(), rcFile.c_str()) != 0) {
        view_message_at_info(info_win, "Error: Failed to rename temporary file", 0, 0);
    } else {
        view_message_at_info(info_win, "ROS configuration updated successfully", 0, 0);
    }

}

void handle_menu_selection(const char *choice, WINDOW *main_win, WINDOW *info_win){
    if (strcmp(choice, "View configs") == 0) 
    {
        view_rosconfig(main_win, info_win);
    }
    else if (strcmp(choice, "Add config") == 0) 
    {
        add_to_rosconfig(main_win, info_win);
    }
    else if (strcmp(choice, "Delete config") == 0) 
    {
        delete_from_rosconfig(main_win, info_win);
    }
    else if(strcmp(choice, "View System Env") == 0){
        view_system_env(main_win, info_win);
    } 
    else if (strcmp(choice, "Update System Env") == 0)
    {
        update_system_env(main_win, info_win);
    }
}

#endif