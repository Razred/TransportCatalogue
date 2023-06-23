#include <sstream>
#include <iostream>
#include <string>
#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"

using namespace std;
using namespace transport_catalogue;
using namespace query;

int main() {
    TransportCatalogue tc;
    InputReader ir;
    ir.ParseInput(tc);
    OutRequest(tc);
    return 0;
}