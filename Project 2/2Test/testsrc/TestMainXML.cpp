#include <gtest/gtest.h>
#include "../include/main.h"
#include <fstream>
using namespace std;
// Helper function to create a test XML file
void createTestXMLFile(const std::string &filename, const std::string &content) {
    ofstream file(filename);
    file << content;
    file.close();
}

TEST(DummyTest, AlwaysPasses) {
    cout << "Google Test is running!" << endl;
    ASSERT_TRUE(true);
}

// Test case: Ensure XML parsing works correctly
TEST(XMLParserTest, BasicParsing) {
    string testFile = "test.xml";
    string testXML = "<root><item name='Test' value='123'/></root>";

    createTestXMLFile(testFile, testXML);

    ASSERT_TRUE(parseXML(testFile)); // Expect successful parsing

    // Clean up test file
    remove(testFile.c_str());
}

TEST(SampleXMLTest, SampleParsing){
    string sampleFile = "sample.xml";
    
}