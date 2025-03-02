#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <vector>
#include "StringDataSource.h"
#include "StringDataSink.h"
#include "XMLReader.h"
#include "XMLWriter.h"
#include "XMLEntity.h"

// Test for XMLreader using a simple XML
TEST(XMLReaderTest, BasicParsing) {
    // A simple XML document without extra whitespace
    std::string xml = "<root><child attr=\"value\">Text</child><empty/></root>";
    auto source = std::make_shared<CStringDataSource>(xml);
    CXMLReader reader(source);

    SXMLEntity entity;
    std::vector<SXMLEntity> entities;
    // Read all entities from XML input
    while (reader.ReadEntity(entity)) {
        entities.push_back(entity);
    }

    // Expected:
    // 1. StartElement "root"
    // 2. StartElement "child" with attribut attr="value"
    // 3. CharData "Text"
    // 4. EndElement "child"
    // 5. StartElement "empty"  (for a self-closing element, Expat issues a start and then an end)
    // 6. EndElement "empty"
    // 7. EndElement "root"
    ASSERT_EQ(entities.size(), 7u);

    // Check the "root" element
    EXPECT_EQ(entities[0].DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entities[0].DNameData, "root");

    // Check the "child" start element
    EXPECT_EQ(entities[1].DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entities[1].DNameData, "child");
    ASSERT_EQ(entities[1].DAttributes.size(), 1u);
    EXPECT_EQ(entities[1].DAttributes[0].first, "attr");
    EXPECT_EQ(entities[1].DAttributes[0].second, "value");

    // Check the character data
    EXPECT_EQ(entities[2].DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(entities[2].DNameData, "Text");

    // Check the "child" end element
    EXPECT_EQ(entities[3].DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entities[3].DNameData, "child");

    // Check the "empty" element
    EXPECT_EQ(entities[4].DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entities[4].DNameData, "empty");
    EXPECT_EQ(entities[5].DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entities[5].DNameData, "empty");

    // Check the closing "root" element.
    EXPECT_EQ(entities[6].DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entities[6].DNameData, "root");
}

// Test for XMLReader when skipping character data (CDATA)
TEST(XMLReaderTest, SkipCData) {
    std::string xml = "<root><child attr=\"value\">Text</child><empty/></root>";
    auto source = std::make_shared<CStringDataSource>(xml);
    CXMLReader reader(source);

    SXMLEntity entity;
    std::vector<SXMLEntity> entities;
    // Use the skipcdata flag to avoid returning CharData entities
    while (reader.ReadEntity(entity, true)) {
        entities.push_back(entity);
    }

    // Expected sequence when skipping character data:
    // 1. StartElement "root"
    // 2. StartElement "child"
    // 3. EndElement "child"
    // 4. StartElement "empty"
    // 5. EndElement "empty"
    // 6. EndElement "root"
    ASSERT_EQ(entities.size(), 6u);
    EXPECT_EQ(entities[0].DNameData, "root");
    EXPECT_EQ(entities[1].DNameData, "child");
    EXPECT_EQ(entities[2].DNameData, "child");
    EXPECT_EQ(entities[3].DNameData, "empty");
    EXPECT_EQ(entities[4].DNameData, "empty");
    EXPECT_EQ(entities[5].DNameData, "root");
}

// Test for XMLWriter: write a basic XML document using start/end elements.
TEST(XMLWriterTest, WriteBasicXML) {
    // We will write: <root><child attr="value">Text</child><empty></empty></root>
    auto sink = std::make_shared<CStringDataSink>();
    CXMLWriter writer(sink);

    SXMLEntity rootStart;
    rootStart.DType = SXMLEntity::EType::StartElement;
    rootStart.DNameData = "root";

    SXMLEntity childStart;
    childStart.DType = SXMLEntity::EType::StartElement;
    childStart.DNameData = "child";
    childStart.DAttributes.push_back({"attr", "value"});

    SXMLEntity text;
    text.DType = SXMLEntity::EType::CharData;
    text.DNameData = "Text";

    SXMLEntity childEnd;
    childEnd.DType = SXMLEntity::EType::EndElement;
    childEnd.DNameData = "child";

    SXMLEntity emptyStart;
    emptyStart.DType = SXMLEntity::EType::StartElement;
    emptyStart.DNameData = "empty";

    SXMLEntity emptyEnd;
    emptyEnd.DType = SXMLEntity::EType::EndElement;
    emptyEnd.DNameData = "empty";
\

    SXMLEntity rootEnd;
    rootEnd.DType = SXMLEntity::EType::EndElement;
    rootEnd.DNameData = "root";

    EXPECT_TRUE(writer.WriteEntity(rootStart));
    EXPECT_TRUE(writer.WriteEntity(childStart));
    EXPECT_TRUE(writer.WriteEntity(text));
    EXPECT_TRUE(writer.WriteEntity(childEnd));
    EXPECT_TRUE(writer.WriteEntity(emptyStart));
    EXPECT_TRUE(writer.WriteEntity(emptyEnd));
    EXPECT_TRUE(writer.WriteEntity(rootEnd));

    // Flush to ensure all open elements are closed
    EXPECT_TRUE(writer.Flush());

    std::string output = sink->String();
    // The expected otput (no extra whitespace/newlines) is:
    // "<root><child attr=\"value\">Text</child><empty></empty></root>"
    std::string expected = "<root><child attr=\"value\">Text</child><empty></empty></root>";
    EXPECT_EQ(output, expected);
}

// Test for XMLWrter: writing a self closing (complete) element.
TEST(XMLWriterTest, WriteCompleteElement) {
    auto sink = std::make_shared<CStringDataSink>();
    CXMLWriter writer(sink);

    SXMLEntity complete;
    complete.DType = SXMLEntity::EType::CompleteElement;
    complete.DNameData = "line";
    complete.DAttributes.push_back({"num", "1"});


    EXPECT_TRUE(writer.WriteEntity(complete));

    std::string output = sink->String();
    std::string expected = "<line num=\"1\"/>";
    EXPECT_EQ(output, expected);
}