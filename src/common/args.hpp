#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>


namespace Utils::ARGS {

    struct Option {
        std::string key;
        std::string type;
    };


    class CommandLineArgs {
        private:
            std::vector<Option> options;
            int argc;
            char** argv;

        public:
            CommandLineArgs(int argc, char** argv) : argc(argc), argv(argv) {}

            void add_option(const std::string& key, const std::string& type) {
                this->options.push_back({key, type});
            }

            std::string get_string(const std::string& key, const std::string& default_val = "") const {
                std::string val = find_value(key);
                return val.empty() ? default_val : val;
            }

            size_t get_size_t(const std::string& key, size_t default_val = 0) const {
                std::string val = find_value(key);
                
                if (val.empty()) {
                    return default_val;
                }
                
                try {
                    return std::stoul(val);
                } catch (...) {
                    return default_val;
                }
            }

            int get_int(const std::string& key, int default_val = 0) const {
                std::string val = find_value(key);
                
                if (val.empty()) {
                    return default_val;
                }
                
                try {
                    return std::stoi(val);
                } catch (...) {
                    return default_val;
                }
            }

        private:
            std::string find_value(const std::string& key) const {
                for (int i = 1; i < this->argc - 1; ++i) {
                    if (this->argv[i] == key) {
                        return this->argv[i + 1];
                    }
                }
                return "";
            }
    };

}