// OpenStreetMapTest.cpp
#include "gtest/gtest.h"
#include "OpenStreetMap.h"
#include "XMLReader.h"
#include "StringDataSource.h" // Assumed to provide a data source from a std::string

TEST(OpenStreetMapTest, BasicParsing)
{
    // Create a sample osm file with raw string syntax
    std::string sampleOSMXML = R"(
<?xml version='1.0' encoding='UTF-8'?> 	  			 	 
<osm version="0.6" generator="osmconvert 0.8.5">
    <node id="95709160" lat="38.5588083" lon="-121.6699366"/>
    <node id="95709161" lat="38.5585993" lon="-121.6709351"/>
    	<tag k="highway" v="motorway_junction"/>
    </node>
    <way id="10745214">
        <nd ref="95709160"/>
        <nd ref="95709161"/>
        <tag k="highway" v="residential"/>
        <tag k="lanes" v="2"/>
        <tag k="name" v="Gregory Place"/>
        <tag k="surface" v="asphalt"/>
    </way>
</osm>
    )";

    // Create a StringDataSource with the OSM XML.
    auto src = std::make_shared<StringDataSource>(sampleOSMXML);
    auto XMLReader = std::make_shared<CXMLReader>(src);

    // Create the OpenStreetMap object.
    COpenStreetMap DavisOSM(XMLReader);

    // Verify the counts.
    EXPECT_EQ(DavisOSM.NodeCount(), 2);
    EXPECT_EQ(DavisOSM.WayCount(), 1);

    // Verify the first node's properties.
    auto node = DavisOSM.NodeByIndex(0);
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->ID(), 1u); // using unsigned literal for TNodeID
    auto loc = node->Location();
    EXPECT_DOUBLE_EQ(loc.first, 51.0);
    EXPECT_DOUBLE_EQ(loc.second, 0.1);
    EXPECT_EQ(node->AttributeCount(), 1u);
    EXPECT_EQ(node->GetAttribute("amenity"), "cafe");

    // Verify the way's properties.
    auto way = DavisOSM.WayByIndex(0);
    ASSERT_NE(way, nullptr);
    EXPECT_EQ(way->ID(), 10u);
    EXPECT_EQ(way->NodeCount(), 2u);
    EXPECT_EQ(way->GetNodeID(0), 1u);
    EXPECT_EQ(way->GetNodeID(1), 2u);
    EXPECT_EQ(way->AttributeCount(), 1u);
    EXPECT_EQ(way->GetAttribute("highway"), "residential");
}
