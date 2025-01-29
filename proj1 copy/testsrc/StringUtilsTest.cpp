#include <gtest/gtest.h>
#include "../include/StringUtils.h"

#include <iostream>
using namespace std;

TEST(StringUtilsTest, SliceTest){
    EXPECT_EQ(1,1);
}

TEST(StringUtilsTest, Capitalize){
    string myString = "test";
    string capString = StringUtils::Capitalize(myString);
    EXPECT_EQ(capString, "TEST");
}

TEST(StringUtilsTest, Upper){
    
}

TEST(StringUtilsTest, Lower){
    
}

TEST(StringUtilsTest, LStrip){
    
}

TEST(StringUtilsTest, RStrip){
    
}

TEST(StringUtilsTest, Strip){
    
}

TEST(StringUtilsTest, Center){
    
}

TEST(StringUtilsTest, LJust){
    
}

TEST(StringUtilsTest, RJust){
    
}

TEST(StringUtilsTest, Replace){
    
}

TEST(StringUtilsTest, Split){
    
}

TEST(StringUtilsTest, Join){
    
}

TEST(StringUtilsTest, ExpandTabs){
    
}

TEST(StringUtilsTest, EditDistance){
    
}
