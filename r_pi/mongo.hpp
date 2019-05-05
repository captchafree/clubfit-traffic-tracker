#ifndef MONGO_H
#define MONGO_H

#include "machine.hpp"
#include "curl/curl.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Mongo {
    public:
        static void add_machine_data(const std::vector<Machine> machines) {

            //Create request
            CURL *hnd = curl_easy_init();

            curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
            curl_easy_setopt(hnd, CURLOPT_URL, "67.205.145.153:5000/vision");

            struct curl_slist *headers = NULL;
            headers = curl_slist_append(headers, "cache-control: no-cache");
            headers = curl_slist_append(headers, "Content-Type: application/json");
            curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

            // Redirect response output
            curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, write_data);

            std::string body = generate_body(machines);
            curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, body.c_str());

            // Send request
            CURLcode ret = curl_easy_perform(hnd);
        }

    private:
        static std::string generate_body(const std::vector<Machine> machines) {
            Config config = Config::get_instance();

            if(machines.empty()) {
                json body;
                body["company"] = config.get_company();
                body["location"] = config.get_location();
                body["machine"] = NULL;

                std::time_t current_time = std::time(nullptr);
                body["time"] = current_time;
                return body;
            }

            std::vector<json> payload;
            for(int i = 0; i < machines.size(); i++) {
                json machine;
                machine["company"] = config.get_company();
                machine["location"] = config.get_location();
                machine["machine"] = machines[i].name;

                std::time_t current_time = std::time(nullptr);
                machine["time"] = current_time;

                payload.push_back(machine);
            }

            json data;
            data["data"] = payload;
            std::string result = data.dump();
            return result;
        }

        static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
            return size * nmemb;
        }
};

#endif