#include "gtest/gtest.h"
#include "OpenStreetMap.h"
#include "StringDataSource.h"
#include "StringDataSource.h"
#include "StringDataSink.h"
#include "XMLReader.h"
#include "XMLWriter.h"
#include "XMLEntity.h"



TEST(OpenStreetMapTest, TestingSampleOSMFileFromDavis)
{
    // Test OSM file
    std::string testOSM = R"(<?xml version='1.0' encoding='UTF-8'?> 	  			 	 
<osm version="0.6" generator="osmconvert 0.8.5">
    <node id="95709160" lat="38.5588083" lon="-121.6699366"/>
    <node id="95709161" lat="38.5585993" lon="-121.6709351">
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
    auto src = std::make_shared<CStringDataSource>(testOSM);
    auto xmlReader = std::make_shared<CXMLReader>(src);
    // Create the OpenStreetMap object.
    COpenStreetMap DavisOSM(xmlReader);

    // Checking the count of both total nodes and total ways
    // The sample only have 2 nodes and 1 way
    EXPECT_EQ(DavisOSM.NodeCount(), 2);
    EXPECT_EQ(DavisOSM.WayCount(), 1);

    // node will be referencing this first <node id="95709160" lat="38.5588083" lon="-121.6699366"/>
    auto node = DavisOSM.NodeByIndex(0);
    // Check if node is indeed referencing something (we don't know what it is yet it is just not a nullptr)
    ASSERT_NE(node, nullptr);
    // Check if it is not equal to -1 (just my way to determine whether the node has a ID or not)
    EXPECT_NE(node->ID(), -1); 
    // Check if the node matches the ID it is expected to have
    EXPECT_EQ(node->ID(), 95709160);
    // Check if the first value in the pair is equal to the expected latitude value
    EXPECT_DOUBLE_EQ(node->Location().first, 38.5588083);
    // Check if the first value in the pair is equal to the expected longitutde value
    EXPECT_DOUBLE_EQ(node->Location().second, -121.6699366);
    // This node does not have any attributes show it should be 0
    EXPECT_EQ(node->AttributeCount(), 0);
    // Run GetAttribute to ensure that there is nothing if it returns empty string ""
    EXPECT_EQ(node->GetAttribute("test"), "");

    // Now let's see if the next node matches the sample osm file: <node id="95709161" lat="38.5585993" lon="-121.6709351">
    node = DavisOSM.NodeByIndex(1);
    ASSERT_NE(node, nullptr);
    EXPECT_NE(node->ID(), -1);
    EXPECT_EQ(node->ID(), 95709161);
    EXPECT_DOUBLE_EQ(node->Location().first, 38.5585993);
    EXPECT_DOUBLE_EQ(node->Location().second, -121.6709351);
    // This node contains only one attribute
    EXPECT_EQ(node->AttributeCount(), 1);
    EXPECT_EQ(node->GetAttribute("highway"), "motorway_junction");


    // Now let's test the way contents
    // Very similar to how we would check the nodes
    auto way = DavisOSM.WayByIndex(0);
    ASSERT_NE(way, nullptr);
    EXPECT_EQ(way->ID(), 10745214);
    EXPECT_EQ(way->NodeCount(), 2);
    EXPECT_EQ(way->GetNodeID(0), 95709160);
    EXPECT_EQ(way->GetNodeID(1), 95709161);
    EXPECT_EQ(way->AttributeCount(), 4);
    EXPECT_EQ(way->GetAttribute("highway"), "residential");
    EXPECT_EQ(way->GetAttribute("lanes"), "2");
    EXPECT_EQ(way->GetAttribute("name"), "Gregory Place");
    EXPECT_EQ(way->GetAttribute("surface"), "asphalt");
}
