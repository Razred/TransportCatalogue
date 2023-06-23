#include "stat_reader.h"

void OutRequest(transport_catalogue::TransportCatalogue &tc) {
    int query_count;
    std::cin >> query_count;
    std::cin.ignore();
    std::string full_request;
    for (int i = 0; i < query_count; i++) {
        getline(std::cin, full_request);
        std::string type_request = full_request.substr(0, full_request.find(' '));
        std::string arg_request = move(full_request.substr(type_request.size() + 1));
        if (type_request == "Bus") {
            RequestBus(tc, arg_request);
        }
        if (type_request == "Stop") {
            RequestStop(tc, arg_request);
        }
    }
}

void RequestBus(transport_catalogue::TransportCatalogue &tc, const std::string &arg_request) {
    auto businfo = tc.GetBusInfo(arg_request);
    if (businfo.stops_number > 0) {
        std::cout <<"Bus " << arg_request << ": " << businfo.stops_number << " stops on route, " <<
                  businfo.unique_stops_number << " unique stops, " << businfo.distance << " route length, " << businfo.curvature << " curvature" << std::endl;
    }
    else {
        std::cout << "Bus " << arg_request << ": not found" << std::endl;
    }
}

void RequestStop(transport_catalogue::TransportCatalogue &tc, const std::string &arg_request) {
    auto tmp = tc.SearchStop(arg_request);
    if (tmp == nullptr) {
        std::cout << "Stop " << arg_request << ": not found" << std::endl;
        return;
    }
    auto buses = tc.GetStopInfo(tmp);
    if (buses.size() == 0) {
        std::cout << "Stop " << arg_request << ": no buses" << std::endl;
        return;
    }
    std::cout << "Stop " << arg_request << ": buses";
    for (auto bus : buses) {
        std::cout << " " << bus->name;
    }
    std::cout << std::endl;
}