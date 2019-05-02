#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include "machine.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Config {
    public:
        Config() {
            std::cout << "Loading config file..." << std::endl;
            this->load_config();
        }

        std::vector<Machine> get_machines() {
            std::vector<Machine> copy = this->machines;
            return copy;
        }

        std::string get_company() {
            return this->company;
        }

        std::string get_location() {
            return this->location;
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
                mach.detection_locations = points_;
                this->machines.push_back(mach);
            }
        }
};