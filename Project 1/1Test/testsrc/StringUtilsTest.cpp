#include <gtest/gtest.h>
#include "../include/StringUtils.h"

#include <vector>
#include <iostream>
using namespace std;

TEST(StringUtilsTest, SliceTest){
    string myString = "Hello world!";
    string slicedString = StringUtils::Slice(myString, 2, 7);
    EXPECT_EQ(slicedString, "llo w");
    slicedString = StringUtils::Slice(myString,0,0);
}

TEST(StringUtilsTest, Capitalize){
    int totalStrings = 5;
    string stringOne = "test";
    string stringTwo = "Abc dEF";
    string stringThree = "aiSONf sdflwl dn";
    string stringFour = "hello WORLD!";
    string stringFive = "witChcrAft";
    string capString = StringUtils::Capitalize(stringOne);
    EXPECT_EQ(capString, "Test");
    capString = StringUtils::Capitalize(stringTwo);
    EXPECT_EQ(capString, "Abc def");
    capString = StringUtils::Capitalize(stringThree);
    EXPECT_EQ(capString, "Aisonf sdflwl dn");
    capString = StringUtils::Capitalize(stringFour);
    EXPECT_EQ(capString, "Hello world!");
    capString = StringUtils::Capitalize(stringFive);
    EXPECT_EQ(capString, "Witchcraft");
    
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
