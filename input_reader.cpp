#include "input_reader.h"
#include <unordered_map>
#include <string_view>
#include <algorithm>

using namespace std;

namespace transport_catalogue {
    namespace query {

        vector<string> SplitString(string_view input, char delimiter) {
            vector<string> result;
            size_t start = 0;
            size_t end = 0;
            while (end != string_view::npos) {
                    end = input.find(delimiter, start);
                    result.push_back(string(input.substr(start, end - start)));
                    start = (end == string_view::npos) ? end : end + 1;
            }
            return result;
        }

        void DeleteSpaces(string &str) {
            size_t start = str.find_first_not_of(' ');
            size_t end = str.find_last_not_of(' ');
            str = str.substr(start, end - start + 1);
        }

        vector<string> Command::ParseRoute() {
            vector<string> route;
            if (desc_command.find('>') != string::npos) {
                is_circle = 1;
                route = SplitString(desc_command, '>');
            }
            if (desc_command.find('-') != string::npos) {
                is_circle = 0;
                route = SplitString(desc_command, '-');
            }
            for (auto &val: route) {
                DeleteSpaces(val);
            }
            return route;
        }

        Coordinates Command::ParseCoordinates() {
            Coordinates result;
            auto tmp = SplitString(desc_command, ',');
            for (auto &val : tmp) {
                DeleteSpaces(val);
            }
            result.lat = stod(tmp[0]);
            result.lng = stod(tmp[1]);
            return result;
        }

        vector<pair<int, string>> Command::ParseDistance(std::vector<std::string>& distance_to) {
            vector<pair<int, string>> result;
            pair<int, string> tmp;
            for (auto &input : distance_to) {
                DeleteSpaces(input);
                size_t separatorPos = input.find("to");
                std::string numberStr = input.substr(0, separatorPos);
                numberStr.erase(std::remove_if(numberStr.begin(), numberStr.end(), ::isspace), numberStr.end());
                int number = std::stoi(numberStr);
                std::string name = input.substr(separatorPos + 2); // +2 для пропуска разделителя "to" и пробела
                size_t firstLetterPos = name.find_first_not_of(' ');
                size_t lastLetterPos = name.find_last_not_of(' ');
                name = name.substr(firstLetterPos, lastLetterPos - firstLetterPos + 1);

                result.push_back({number, name});
            }
            return result;
        }

        void Command::ParseCommandString(std::string input, TransportCatalogue &tc) {
            static unordered_map<std::string, QueryType> const table = {
                    {"Stop", QueryType::Stop},
                    {"Bus",  QueryType::Bus}
            };
            origin_command = move(input);
            auto vec_route = SplitString(origin_command, ' ');
            auto start = origin_command.find_first_not_of(' ');
            auto end = origin_command.find(' ');
            string type = {origin_command.begin() + start, origin_command.begin() + end};
            if (table.at(type) == QueryType::Bus) {
                Command::type = QueryType::Bus;
                auto pos = origin_command.find(':');
                if (pos != string::npos) {
                    desc_command = origin_command.substr(origin_command.find_first_not_of(' ', pos + 1));
                }
                bus_name = origin_command.substr(end + 1, pos - end - 1);
                route = Command::ParseRoute();
                tc.addBus(move(bus_name), move(route), is_circle);
            }
            if (table.at(type) == QueryType::Stop) {
                Command::type = QueryType::Stop;
                auto pos = origin_command.find(':');
                if (pos != string::npos) {
                    desc_command = origin_command.substr(origin_command.find_first_not_of(' ', pos + 1));
                }
                stop_name = origin_command.substr(end + 1, pos - end - 1);
                Coordinates coordinates = ParseCoordinates();
                tc.addStop(stop_name, move(coordinates));
                auto distance_to = SplitString(desc_command, ',');
                distance_to = {distance_to.begin() + 2, distance_to.end()};
                auto distance = ParseDistance(distance_to);
                for (auto val: distance) {
                    tc.SetDistance(stop_name, val.second, val.first);
                }
            }
        }

        void InputReader::ParseInput(TransportCatalogue &tc) {
            this->tc = tc;
            int query_count;
            std::cin >> query_count;
            std::cin.ignore();
            string full_command;
            for (int i = 0; i < query_count; i++) {
                getline(cin, full_command);
                Command command;
                command.ParseCommandString(move(full_command), tc);
            }
        }
    }
}