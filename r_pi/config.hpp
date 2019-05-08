#ifndef CONFIG_H
#define CONFIG_H

#include "machine.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

struct PointOutOfBoundsException : public std::exception {
	const char * what () const throw () {
    	return "Point out of bounds. Value is constrained to that of the camera's resolution.";
    }
};

/*
Handles loading/saving to the config file
*/
class Config {
     public:
        static Config& get_instance() {
            // Since it's a static variable, if the class has already been created,
            // it won't be created again.
            // And it **is** thread-safe in C++11.
            static Config myInstance;

            // Return a reference to our instance.
            return myInstance;
        }

        /*
        Returns the machines specified in the config file
        */
        std::vector<Machine> get_machines() const {
            return this->machines;
        }

        /*
        Returns the company specified in the config file
        */
        std::string get_company() const {
            return data["company"];
        }

        /*
        Returns the location specified in the config file
        */
        std::string get_location() const {
            return data["location"];
        }

        /*
        Returns the location specified in the config file
        */
        int get_camera_resolution_width() const {
            return data["camera_settings"]["resolution"]["width"];
        }

        /*
        Returns the location specified in the config file
        */
        int get_camera_resolution_height() const {
            return data["camera_settings"]["resolution"]["height"];
        }

    protected:
        Config() {
            std::cout << "Loading config file..." << std::endl;
            this->load_config();
        }

    private:
        // The config file represented as a json object
        json data;

        // A list of the machines to be monitored
        std::vector<Machine> machines;

        /*
        Loads the config file.
        */
        void load_config() {
            
            std::ifstream config_file("config/config.json", std::ifstream::binary);
            json data = json::parse(config_file);
            this->data = data;

            json machines = data["machines"];

            for(json machine : machines) {
                Machine current_machine;

                current_machine.name = machine["name"];
                json machine_points = machine["points"];

                std::vector<cv::Point> points_;
                for(auto point : machine_points) {
                    int x = point["x"];
                    int y = point["y"];
                    current_machine.contour_points.push_back(cv::Point(x, y));
                }
                this->machines.push_back(current_machine);
            }

            this->verify_integrity();
        }

        /*
        Verifies that the values in the config file are valid
        */
        void verify_integrity() {
            int width = this->get_camera_resolution_width();
            int height = this->get_camera_resolution_height();

            for (Machine mach : this->machines) {
                for (auto point : mach.contour_points) {
                    if (point.x >= width || point.x < 0 || point.y >= height || point.y < 0) {
                        throw PointOutOfBoundsException();
                    }
                }
            }
        }
};

#endif