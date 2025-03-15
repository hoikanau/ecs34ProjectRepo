CDSVReader utilizes the following files:
    include/DSVReader.h    include/DataSource.h
    src/DSVReader.cpp

This class reads individual rows of a file.

bool End()
   This function of the CDSVReader checks if the file is at the end.
   If the file is at the end we want to stop reading and end the function by returning true otherwise return false.

bool ReadRow(std::vector<std::string> &row)
   This function of the CDSVReader reads the rows of the DSV file and returns true or false if it was successfully read

   For example, if your DSV file is something like this:
        Name,Age,Comment
        Bob,12,Four foot six
        Emily,20,In College

    If you call ReadRow on the DSV file, it will take the first row and input it into the vector/array &row
    Then it reads the individual characters of the row,
       In this case: row = Name,Age,Comment
       it will read The chars one by one until it hits a delimiter which would be the comma


CDSVWriter utilizes the following files:
    include/DSVWriter.h    include/DataSink.h
    src/DSVWriter.cpp

bool WriteRow(const std::vector<std::string> &row)
   DSVWriter only has one public function
   It writes the elements of row into a data sink which is essentially a output file and the function will return true or false if it was successfully written
   
   Before writing the file one must create a new CDSVWriter object by calling CDSVWriter writer(sink, delimiter, quoteall);
    sink refers to the output file
    delmiter refers to the what char that separates the values
    quoteall refers to a boolean if the user wants to quote all the values in the row when writing
    
    For example, if you already setup some CDSVWriter object called writer // CDSVWriter writer(sink, ',', false)
        You will write to a output file determined by sink with commas as spacers/delimiters and you will not quote all the values
    
    Then calling the following lines:
        writer.WriteRow({"Name", "Age", "Comment"});
        writer.WriteRow({"Bob", "21", "Attending ECS34"})
        writer.WriteRow({"Henry", "12", "Child Prodigy"})

    You will get the following output in the sink:
        Name,Age,Comment
        Bob,21,Attending ECS34
        Henry,12,Child Prodigy


CXMLReader:
    - include/XMLReader.h
    - include/XMLEntity.h
    - include/DataSource.h
    - src/XMLReader.cpp

CXMLReader reads XML data from a data source and converts it into XML entities represented by the SXMLEntity. Each entity corresponds to a start and end elements, character, or complete element in the XML document

1. bool End() const
-> Checks whether all XML entities have been read from input

-> Returns TRUE when the entire XML input has been processed, indicating that no more entities available 
-> otherwise, returns FALSE.

2. bool ReadEntity(SXMLEntity &entity, bool skipcdata = false)
-> Reads the next XML entity from the input
-> Returns True if an XML entity is able to read, or FLASE if there are no more entities or if an error occurs during parsing

CXMLWriter: 
    - include/XMLwriter.h
    - include/XMLEntity.h
    - include/DataSource.h
    - src/XMLwriter.cpp

1. bool WriteEntity(const SXMLEntity &entity)
-> Writes a single XML entity to the output
-> StartElement, the function writes an opening tag and pushes the element name onto an internal stack.
-> EndElement, the function writes the corresponding closing tag and pops the element from stack
-> CompleteElement, the function writes a self closing tag
-> CharData, the function writes the text contents after escaped special characters such as &, <, >, ", and '.

2. bool Flush():
-> Ensures that any open XML elements (from previously written start tags) are properly closed
-> Iterates the internal stack of opened elements and writes the end tags
-> Returns true if all open tags are correctly closed


CSVBusSystem:
    - CSVBusSystem.h
    - BusSystem.h
    - DSVReader.h

    * Helper Functions
1. RemoveQuotes Function:
- Purpose: Removes the first and last characters if they are double quotes.
2. Trim Function:
- Purpose: Trims leading and trailing whitespace from a string.


    * SImplementation Structure
This private structure within CCSVBusSystem holds two vectors:

- stops: A vector of shared pointers to objects that implement the SStop interface.
- routes: A vector of shared pointers to objects that implement the SRoute interface.

    * Two inner classes:
- StopImpl:
Implements the SStop interface. It stores a stop ID and a node ID.

- RouteImpl:
Implements the SRoute interface. It stores the route name and a vector of stop IDs that define the route.

* Constructor: CSV Parsing
- Parsing the Stops CSV
* Reading Rows:
- The constructor uses a CDSVReader to read rows from the stops CSV.

* Header Handling:
- The first row is checked; if its first field (after removing quotes and trimming) equals "stop_id", it is skipped.

* Field Conversion:
- Each row is expected to have at least two fields. The first field is converted to a stop ID and the second to a node ID using std::stoull.

* Deduplication:
- An unordered_map is used to ensure that if multiple rows with the same stop ID are encountered, only the first occurrence is stored.

* Storing Data:
- Valid stops are then pushed into the internal vector.

* Parsing the Routes CSV
- Reading Rows: Similarly, a second CDSVReader is used to process the routes CSV.

* Header Handling:
- The first row is skipped if its first field (after cleaning) equals "route_name".

* Route Construction:
- The first field of each row represents the route name.
- The remaining fields are treated as stop IDs, which are cleaned, converted, and then verified against the stops already stored.

* Deduplication Within Route:
- An unordered_set ensures that each stop ID is only included once per route.

* Storing Data:
- Only routes with at least one valid stop are added to the internal routes vector.



* StopCount():
- Returns the number of stops stored.

* RouteCount():
Returns the number of routes stored.

* StopByIndex(index):
- Returns a pointer to the stop at the specified index (or nullptr if out-of-bounds).

* StopByID(id):
- Returns a pointer to the stop with the specified stop ID (or nullptr if not found).

* RouteByIndex(index):
- Returns a pointer to the route at the specified index (or nullptr if out-of-bounds).

* RouteByName(name):
- Returns a pointer to the route with the specified name (or nullptr if not found).


CStreetMap (located in StreetMap.h) has a couple of functions that are implemented to function in conjunction with COpenStreetMap

Struct SNode contains the following public variables: (These variabes will be declared to a)
    TNodeID -> Referenced in COpenStreetMap as nodeID that holds the ID of a node as an unsigned integer 64 bit
    TWayID -> Reference in COpenStreetMap as wayID that holds the ID of a way as an unsigned integer 64 bit
    TLocation -> Reference in COpenStreetMap as attributes that stores a key value pair of the node or way in a pair data structure
    InvalidNodeID -> A variable to call if the user access an invalid node ID
    InvalidWayID -> A variable to call if the user access an invalid way ID

COpenStreetMap is implemented in: COpenStreetMap.cpp
    Which utilizes the following header files: 
        StreetMap.h    OpenStreetMap.h

Before the explanation of the different functions, let's create an object that can access these functions.
Suppose you have some OSM file like the following:
    
<?xml version='1.0' encoding='UTF-8'?> 	  			 	 
<osm version="0.6" generator="osmconvert 0.8.5">
    <node id="151" lat="12" long="33">
    <node id="1234" lat="90" lon="45">
        <tag k="highway" v="motorway_junction"/>
    </node>
    <way id="1344">
        <nd ref="151"/>
        <nd ref="1234">
        <tag k="highway" v="residential"/>
        <tag k="lanes" v="2"/>
    </way>
</osm>

Let's call this file unreadTownOSM.
1. Let's first declare unreadTownOSM as a CStringDataSource object by the following:
    auto dataSource = std::make_shared<CStringDataSource>(unreadTownOSM);

2. Then we parse the unreadTownOSM through out XMLReader by the following:
    auto xmlReader = std::make_shared<CXMLReader>(dataSource);

3. Finally, let's create a COpenStreetMap object with our parsed unreadTownOSM data file. Let's call it ReadTownOSM
    COpenStreetMap TownOSM(xmlReader);

--------------------------------------------------------------
Here I are the list of functions that can be access by TownOSM

COpenStreetMap contains the following 
    size_t NodeCount()
    size_t WayCount()
    shared_ptr<CStreetMap::SNode> NodeByIndex(size_t index)
    shared_ptr<CStreetMap::SNode> NodeByID(TNodeID id)
    shared_ptr<CStreetMap::SWay> WayByIndex(size_t index)
    shared_ptr<CStreetMap::SWay> WayByID(TWayID id)

COpenStreetMap also has the following functions to access attributes of each nodes and ways
    struct SNode contains the following functions:
        TNodeID ID()
        TLocation Location()
        size_t AttributeCount()
        string GetAttributeKey(size_t index)
        bool HasAttribute(const string &key)
        string GetAttribute(const string &key)
    
    struct SWay contains the following functions:
        TWayID ID()
        size_t NodeCount()
        TNodeID GetNodeID(size_t index)
        size_t AttributeCount()
        string GetAttributeKey(size_t index)
        bool HasAttribute(const string &key)
        string GetAttribute(const string &key)
--------------------------------------------------------------

Going back top ReadTownOSM.

1. We can check how many nodes ReadTownOSM contains by calling NodeCount() like the following:
    ReadTownOSM->NodeCount()  -> This will return 2

2. Similarly we can check how many ways are in ReadTownOSM by calling WayCount():
    ReadTownOSM->WayCount()   -> This will return 1

3. Next we can access the individual nodes via the indexes because under the hood it is still in a vector data structure by utilizing NodeByIndex() OR NodeByID() and this should be utilized in conjunction with a SNode function

    For example if you want to get the ID of the first element you do:
        ReadTownOSM->NodeByIndex(0).ID()   -> This will return 151

Ideally after you get the ID of a node, you can use NodeByINdex() or NodeByID() interchangeably but instead of passing an index, you pass the ID of the node respectively

    If you want to get the latitude of the first node you do:
        ReadTownOSM->NodeByIndex(0).Location().first   -> This will return 12

    If you want to get the longitude of the second node you do:
        ReadTownOSM->NodeByIndex(1).Location().second   -> This will return 45

    If you want to get the attribute of the second node at a specific index (if the node contains multiple attributes) you do:
        ReadTownOSM->NodeByIndex(1).GetAttributeKey("0")   -> This will return "highway"

    If you want to see if the node has a specific attribute:
        ReadTownOSM->NodeByIndex(0).HasAttribute("highway")   -> this will return false because node 1 does not have any attributes

    If you want to get the attribute of the node using a specific key do:
        ReadTownOSM->NodeByIndex(1).GetAttribute("highway")   -> This will return "motor_junction"

4. Moving on to accessing the ways of the OSM file. If you want to access a specific way with an index you use WayByIndex otherwise if you want to access a specific way with its ID you use WayByID
    
    To get the ID of a way do:
        ReadTownOSM->WayByIndex(0).ID()   -> This will return 1344

    To get the total amount of nodes that is connect by a way you do:
        ReadTownOSM->WayByID(1344).NodeCount()   -> This will return 2

    To get the total amount of attributes attached to the way do:
        ReadTownOSM->WayByID(1344).AttributeCount()   -> This will return 2

    To get the attribute key at a specific index you do:
        ReadTownOSM->WayByID(1344).GetAttributeKey(1)   -> This will return "lanes" 
    
    To check if the way has a certain attribute you do:
        ReadTownOSM->WayByID(1344).HasAttribute("highway")    -> This will return true because it does have this attribute key
    
    To get the attribute of a certain way using it's key do:
        ReadTownOSM->WayByID(1344).GetAttribute("lanes")   -> This will return "2"

CBusSystemIndexer:
    - BusSystemIndexer.h
    - BusSystem.h
    - BusSystemIndexer.cpp

* SImplementation Structure
    * Members:
        Stops: A std::vector holding shared pointers to bus stops (CBusSystem::SStop).
        Routes: A std::vector holding shared pointers to bus routes (CBusSystem::SRoute).
        Constructor:Retrieves all stops from the bus system using StopCount() and StopByIndex().
                    Sorts the stops by their unique ID.
                    Retrieves all routes from the bus system using RouteCount() and RouteByIndex().
                    Sorts the routes alphabetically by name.

* StopCount():
- Returns the number of stops in the bus system.

* RouteCount():
- Returns the number of routes in the bus system.

* SortedStopByIndex(index):
- Returns a pointer to a stop (from the sorted vector) given its index. Returns nullptr if the index is out of range.

* SortedRouteByIndex(index):
- Returns a pointer to a route (from the sorted vector) given its index. Returns nullptr if the index is out of range.

* StopByNodeID(id):
- Searches the stored stops for one that matches the given node ID. Returns a pointer to the stop if found; otherwise, returns nullptr.

* RoutesByNodeIDs(src, dest, routes):
- Iterates through each route, checking if it contains a stop with node ID equal to src followed later by a stop with node ID equal to dest. If such a route exists, it is inserted into the provided routes unordered set. Returns true if at least one valid route is found.

* RouteBetweenNodeIDs(src, dest):
- A helper function that returns true if there is at least one route connecting the source and destination node IDs.

CTransportationPlannerCommandLine:
    -TransportationPlannerCommandLine.h
    - TransportationPlanner.h
    - StringUtils.h
    - GeographicUtils.h
    - TransportationPlannerCommandLine.cpp

* CmdSource: 
- A shared pointer to a data source (input stream) for reading user commands.

* OSink: 
- A shared pointer to a data sink for normal output.

* ErrSink: 
- A shared pointer to a data sink for error messages.

* ResultsFacto: 
- A shared pointer to a factory for creating result objects.

* Planner: 
- A shared pointer to the transportation planner that handles the map and routing functionality.

* ReadLine():
- Reads a full line of text from the CmdSource character by character until a newline (\n) is encountered.

* WriteOutput(str):
- Writes the provided string to the output sink (OSink) by converting the string into a vector of characters.

* WriteError(str):
- Writes the provided error message string to the error sink (ErrSink) using the same conversion method.

* exit:
- Exits the command processing loop.

* help:
- Displays a help menu listing all available commands and their descriptions.

* count:
- Retrieves and displays the number of nodes in the transportation map from the planner.

* node:
- Expects a node index as a parameter. Displays details of the specified node (its ID and geographic location formatted as degrees, minutes, seconds).

* shortest:
- Expects two parameters (source and destination node IDs). Calculates the shortest path (distance) between these nodes. Outputs the distance in miles or an error if no valid path exists.

* fastest:
- Expects two parameters (source and destination node IDs). Calculates the fastest path (travel time) between these nodes. Outputs the travel time in minutes or an error if no valid path exists.

* print/save:
- These commands currently serve as placeholders. They return an error message indicating that there is no valid path available to print or save.

* Unknown Commands:
- Any unrecognized command results in an error message instructing the user to type "help" for guidance.

DijkstraPathRouter.cpp
    - DijkstraPathRouter.h

CDijkstraPathRouter
    - struct Edge
        CPathRouter::TVertexID destination : Determine the destination of the edge
        double weight : How "far" is the destination
    - struct Vertex
        any tag : Stores the tag of a vertex
        vector<Edge> edges : And store the edges that the vertex extends to / Where can we go from this vertex

* VertexCount()
- Returns the number of vertices in the path router

* TVertexID AddVertex(std::any tag)
- Adds a vertex with the tag provided. The tag can be of any type.

* std::any GetVertexTag(TVertexID id)
- Gets the tag of the vertex specified by id if id is in the path router.
- A std::any() is returned if id is not a valid vertex ID.

* bool AddEdge(TVertexID src, TVertexID dest, double weight, bool bidir = false)
- Adds an edge between src and dest vertices with a weight of weight. If bidir is set to true an additional edge between dest and src is added. If src or dest nodes do not exist, or if the weight is 
- negative the AddEdge will return false, otherwise it returns true.

* bool Precompute(std::chrono::steady_clock::time_point deadline)
- Allows the path router to do any desired precomputation up to the deadline

* double FindShortestPath(TVertexID src, TVertexID dest, std::vector<TVertexID>&path)
- Returns the path distance of the path from src to dest, and fills out path with vertices. If no path exists NoPathExists is returned.

DijkstraTransportationPlanner.cpp
    - TransportationPlanner.h
    - StreetMap.h
    - BusSystem.h
    - PathRouter.h

* std::size_t NodeCount()
- Returns the number of nodes in the street map

* std::shared_ptr<CStreetMap::SNode> SortedNodeByIndex(std::size_t index)
- Returns the street map node specified by index if index is less than the NodeCount(). nullptr is returned if index is greater than or equal to NodeCount(). The nodes are sorted by Node ID.

* double FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID >&path)
- Returns the distance in miles between the src and dest nodes of the shortest path if one exists. NoPathExists is returned if no path exists. The nodes of the shortest path are filled in the path 
- parameter.

* double FindFastestPath(TNodeID src, TNodeID dest, std::vector< TTripStep >&path)
- Returns the time in hours for the fastest path between the src and dest nodes of the if one exists. NoPathExists is returned if no path exists. The transportation mode and nodes of the fastest 
- path are filled in the path parameter.

* bool GetPathDescription(const std::vector< TTripStep > &path, std::vector<std::string > &desc)
- Returns true if the path description is created. Takes the trip steps path and converts it into a human readable set of steps.
