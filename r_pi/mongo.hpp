#ifndef MONGO_H
#define MONGO_H

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "machine.hpp"
#include "curl/curl.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Mongo {
    public:
        static void add_machine_data(const Machine machine) {
            Config config;

            //Create request
            CURL *hnd = curl_easy_init();

            curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
            curl_easy_setopt(hnd, CURLOPT_URL, "67.205.145.153:5000/vision");

            struct curl_slist *headers = NULL;
            headers = curl_slist_append(headers, "cache-control: no-cache");
            headers = curl_slist_append(headers, "Content-Type: application/json");
            curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

            curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, write_data);

            // Generate payload
            json payload;
            payload["company"] = config.get_company();
            payload["location"] = config.get_location();
            payload["machine"] = machine.name;

            std::time_t current_time = std::time(nullptr);
            payload["time"] = current_time;

            std::string result = payload.dump();
            curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, result.c_str());

            //Send request
            CURLcode ret = curl_easy_perform(hnd);
        }

    private:
        static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
            return size * nmemb;
        }
};

#endif