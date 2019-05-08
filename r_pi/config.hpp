#ifndef CONFIG_H
#define CONFIG_H

#include "machine.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

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
        std::vector<Machine> get_machines() {
            std::vector<Machine> copy = this->machines;
            return copy;
        }

        /*
        Returns the company specified in the config file
        */
        std::string get_company() {
            return this->company;
        }

        /*
        Returns the location specified in the config file
        */
        std::string get_location() {
            return this->location;
        }

    protected:
        Config() {
            std::cout << "Loading config file..." << std::endl;
            this->load_config();
        }

    private:
        std::string company;
        std::string location;

        std::vector<Machine> machines;

        /*
        Loads the config file.
        */
        void load_config() {
            
            std::ifstream config("config/config.json", std::ifstream::binary);
            auto json = json::parse(config);

            this->company = json["company"];
            this->location = json["location"];

            auto machines = json["machines"];

            for(int i = 0; i < machines.size(); i++) {
                Machine mach;
                auto mach_data = machines[i];

                mach.name = mach_data["name"];
                auto points = mach_data["points"];

                std::vector<cv::Point> points_;
                for(int j = 0; j < points.size(); j++) {
                    auto point = points[j];
                    int x = point["x"];
                    int y = point["y"];
                    points_.push_back(cv::Point(x, y));
                }
                mach.contour_points = points_;
                this->machines.push_back(mach);
            }
        }
};

#endif