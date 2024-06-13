#ifndef ROSNETCONFIG_HPP
#define ROSNETCONFIG_HPP

#include <string>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <fstream>
#include <iomanip>
#include <nlohmann/json.hpp>


#include "helpers.hpp"

struct RosNetConfig {
    std::string name;
    std::string server_ip;
    std::string client_ip;
};

bool check_rosconfig_exists() {
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    std::string rosconfig_path = std::string(homedir) + "/.rosconfig";

    return (access(rosconfig_path.c_str(), F_OK) != -1);
}

bool create_rosconfig_file() {
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    std::string rosconfig_path = std::string(homedir) + "/.rosconfig";

    std::ofstream rosconfig_file(rosconfig_path);
    if (!rosconfig_file) {
        return false;
    }

    rosconfig_file << "[]" << std::endl;
    return true;
}

bool add_to_rosconfig(const RosNetConfig& config) {

    nlohmann::json json_config;
    std::string home_path = getenv("HOME");
    std::ifstream file(home_path + "/.rosconfig");
    
    if (file.is_open()) 
    {
        file >> json_config;
        file.close();
    }

    nlohmann::json new_entry = {
        {"name", config.name},
        {"server_ip", config.server_ip},
        {"client_ip", config.client_ip}
    };

    json_config.push_back(new_entry);
    std::ofstream outfile(home_path + "/.rosconfig");

    if (outfile.is_open()) 
    {
        outfile << std::setw(4) << json_config << std::endl;
        outfile.close();
        return true;
    } 
    else 
    {
        return false;
    }
}

std::vector<RosNetConfig> get_rosconfig(WINDOW *win) {
    const char* home_path = getenv("HOME");
    if (!home_path) {
        view_message_at_info(win, "Error: HOME environment variable not set.", 0, 0);
        return std::vector<RosNetConfig>();
    }

    std::string rosconfig_path = std::string(home_path) + "/.rosconfig";

    std::ifstream file(rosconfig_path);
    if (!file.is_open()) {
        // Error opening file
        view_message_at_info(win, "Error: .rosconfig file not found.", 0, 0);
        return std::vector<RosNetConfig>();
    }

    nlohmann::json json_data;
    file >> json_data;

    file.close();

    if (!json_data.is_array()) {
        view_message_at_info(win, "Error: .rosconfig file is not an array.", 0, 0);
        return std::vector<RosNetConfig>();
    }

    std::vector<RosNetConfig> rosconfig_elements;

    for (const auto& element : json_data) {
        RosNetConfig config;
        config.name = element["name"];
        config.server_ip = element["server_ip"];
        config.client_ip = element["client_ip"];
        rosconfig_elements.push_back(config);
    }

    return rosconfig_elements;
}

void update_rosconfig_file(const std::vector<RosNetConfig>& configs) {
    const char* home_path = getenv("HOME");
    if (!home_path) {
        return;
    }
    std::string rosconfig_path = std::string(home_path) + "/.rosconfig";

    std::ofstream file(rosconfig_path);
    if (!file.is_open()) {
        return;
    }

    nlohmann::json json_data;
    for (const auto& config : configs) {
        json_data.push_back({
            {"name", config.name},
            {"server_ip", config.server_ip},
            {"client_ip", config.client_ip}
        });
    }
    file << std::setw(4) << json_data << std::endl;

    file.close();
}

#endif