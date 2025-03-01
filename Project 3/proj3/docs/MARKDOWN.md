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
