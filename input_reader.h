#pragma once

#include "transport_catalogue.h"

namespace transport_catalogue {

    namespace query {

        enum QueryType {
            Bus,
            Stop
        };


        struct Command {
            QueryType type;
            std::string stop_name;
            std::string bus_name;

            std::string origin_command;
            std::string desc_command;
            std::vector<std::string> route;
            bool is_circle = 0;
            Coordinates coordinates;

            std::vector<std::string> ParseRoute();
            Coordinates ParseCoordinates();
            std::vector<std::pair<int, std::string>> ParseDistance(std::vector<std::string>&);

            void ParseCommandString(std::string input, TransportCatalogue& tc);
        };


        class InputReader {
        public:
            void ParseInput(TransportCatalogue&);
        private:
            TransportCatalogue tc;
        };
    }
}

