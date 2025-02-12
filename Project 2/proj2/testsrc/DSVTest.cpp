#include <gtest/gtest.h>
#include <memory>
#include <string>
#include "StringDataSink.h"
#include "StringDataSource.h"
#include "DataSource.h"
#include "DataSink.h"
#include "DSVWriter.h"
#include "DSVWriter.cpp"
#include "DSVReader.h"
#include "DSVReader.cpp"
using namespace std;

TEST(CDSVWriterTest, TestForWriter1) {
    // Create a string sink
    auto sink = make_shared<CStringDataSink>();

    // Instantiate the DSV writer (CSV-style) with ',' as the delimiter
    // and quoteall = false (only quote when necessary)
    CDSVWriter writer(sink, ',', false);

    // Write a header row
    vector<std::string> header = {"Name", "Age", "Comment"};
    writer.WriteRow(header);

    // Write a simple row with no quotes required
    vector<std::string> row1 = {"Alice", "30", "No issues"};
    writer.WriteRow(row1);

    // Write a row that contains quotes and commas
    //  - The field "He said \"Hello,\" then left" should be quoted and internal quotes doubled
    vector<std::string> row2 = {"Bob", "40", "He said \"Hello,\" then left"};
    writer.WriteRow(row2);

    // Build the expected CSV output as a single string (including newlines)
    // Note that the quotes around "He said \"Hello,\" then left" will become "He said ""Hello,"" then left"
    string expected_output = 
        "Name,Age,Comment\n"
        "Alice,30,No issues\n"
        "Bob,40,\"He said \"\"Hello,\"\" then left\"\n";

    // Check if the sink's string matches the expected CSV output
    EXPECT_EQ(sink->String(), expected_output);
}


TEST(CDSVReaderTest, TestForReader1){
    vector<string> row;
    string testCSV = 
        "This,Is,A,Test\n"
        "This,Is,The,Fourth\n"
        "Testing,\"Tes,ting\",Testing,testing\n";
    auto testSource = make_shared<CStringDataSource>(testCSV);
    CDSVReader myReader(testSource, ',');
    ASSERT_TRUE(myReader.ReadRow(row));
    ASSERT_EQ(row.size(), 4);
    EXPECT_EQ(row[0],"This");
    EXPECT_EQ(row[1],"Is");
    ASSERT_TRUE(myReader.ReadRow(row));
    EXPECT_EQ(row[2], "The");
    ASSERT_TRUE(myReader.ReadRow(row));
    EXPECT_EQ(row[1], "Tes,ting");
    ASSERT_FALSE(myReader.ReadRow(row));
}