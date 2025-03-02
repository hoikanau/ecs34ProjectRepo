// #include "CSVBusSystem.h"
// #include "DSVReader.h"
// #include "StringDataSource.h"
// #include <cassert>
// #include <iostream>
// #include <memory>

// int main() {
//     // Sample CSV data for stops.
//     // Format per row: stop_id,node_id
//     std::string stopsCSV = "1,100\n2,200\n";
    
//     // Sample CSV data for routes.
//     // Format per row: route_name,stop_id1,stop_id2,...
//     std::string routesCSV = "RouteA,1,2\n";

//     // Create data sources from the CSV strings.
//     auto stopsSource = std::make_shared<CStringDataSource>(stopsCSV);
//     auto routesSource = std::make_shared<CStringDataSource>(routesCSV);

//     // Create DSV readers for stops and routes (using comma as delimiter).
//     auto stopsReader = std::make_shared<CDSVReader>(stopsSource, ',');
//     auto routesReader = std::make_shared<CDSVReader>(routesSource, ',');

//     // Create the CSV bus system.
//     CCSVBusSystem busSystem(stopsReader, routesReader);

//     // Test the number of stops and routes.
//     assert(busSystem.StopCount() == 2);
//     assert(busSystem.RouteCount() == 1);

//     // Test retrieval of stops by index.
//     auto stop0 = busSystem.StopByIndex(0);
//     auto stop1 = busSystem.StopByIndex(1);
//     assert(stop0 != nullptr);
//     assert(stop1 != nullptr);
//     assert(stop0->ID() == 1);
//     assert(stop0->NodeID() == 100);
//     assert(stop1->ID() == 2);
//     assert(stop1->NodeID() == 200);

//     // Test retrieval of a stop by its ID.
//     auto stopById = busSystem.StopByID(2);
//     assert(stopById != nullptr);
//     assert(stopById->ID() == 2);
//     assert(stopById->NodeID() == 200);

//     // Test retrieval of route by index.
//     auto route0 = busSystem.RouteByIndex(0);
//     assert(route0 != nullptr);
//     assert(route0->Name() == "RouteA");
//     assert(route0->StopCount() == 2);
//     assert(route0->GetStopID(0) == 1);
//     assert(route0->GetStopID(1) == 2);

//     // Test retrieval of route by name.
//     auto routeByName = busSystem.RouteByName("RouteA");
//     assert(routeByName != nullptr);
//     assert(routeByName->Name() == "RouteA");

//     std::cout << "All CSVBusSystem tests passed!" << std::endl;
//     return 0;
// }
