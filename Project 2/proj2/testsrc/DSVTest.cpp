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
    auto sink = make_shared<CStringDataSink>();
    CDSVWriter writer(sink, ',', false);
    vector<std::string> header = {"Name", "Age", "Comment"};
    writer.WriteRow(header);
    vector<std::string> row1 = {"Bob", "12", "Brain damaged"};
    writer.WriteRow(row1);
    vector<std::string> row2 = {"Tester", "0", "This is a \"test\" com,ment"};
    writer.WriteRow(row2);
    string expected_output = 
        "Name,Age,Comment\n"
        "Bob,12,Brain damaged\n"
        "Tester,0,\"This is a \"\"test\"\" com,ment\"\n";
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