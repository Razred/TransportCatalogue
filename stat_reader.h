#pragma once

#include "transport_catalogue.h"

void OutRequest(transport_catalogue::TransportCatalogue &tc);
void RequestBus(transport_catalogue::TransportCatalogue &tc, const std::string&);
void RequestStop(transport_catalogue::TransportCatalogue &tc, const std::string&);

