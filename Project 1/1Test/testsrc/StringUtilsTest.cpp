#include <gtest/gtest.h>
#include "../include/StringUtils.h"

#include <vector>
#include <iostream>
using namespace std;

TEST(StringUtilsTest, SliceTest){
    string myString = "Hello World!";
    string slicedString = StringUtils::Slice(myString, 2, 5);
    // EXPECT_EQ(slicedString, "llo");
    string negSliceString = StringUtils::Slice(myString, -10, 5);
    EXPECT_EQ(slicedString,negSliceString);

    string stringTestOne = StringUtils::Slice("ABCDEFGHIJKLMNO", 4, 12);
    string stringTestTwo = StringUtils::Slice("ABCDEFGHIJKLMNO", -11, -3);
    EXPECT_EQ(stringTestOne, stringTestTwo);
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
    string myString = "helLo";
    string result = StringUtils::Upper(myString);
    EXPECT_EQ(result, "HELLO");
}

TEST(StringUtilsTest, Lower){
    string myString = "THERE Is No STrinG";
    string result = StringUtils::Lower(myString);
    EXPECT_EQ(result, "there is no string");
}

TEST(StringUtilsTest, LStrip){
    string myString = "         Hello     ";
    string result = StringUtils::LStrip(myString);
    EXPECT_EQ(result, "Hello     ");
}

TEST(StringUtilsTest, RStrip){
    string myString = "     Hello            ";
    string result = StringUtils::RStrip(myString);
    EXPECT_EQ(result, "     Hello");
    EXPECT_EQ(StringUtils::RStrip("     He l l o w     orl    d            "), "     He l l o w     orl    d");
}

TEST(StringUtilsTest, Strip){
    string myString = " asdofinsadoifnios  s d  s sdjk fsnd ks nksj nsk nsk ssjkdnfknsdkjfnksjdnf ns sjsn ksn    ";
    string result = StringUtils::Strip(myString);
    EXPECT_EQ(result, "asdofinsadoifnios  s d  s sdjk fsnd ks nksj nsk nsk ssjkdnfknsdkjfnksjdnf ns sjsn ksn");
}

TEST(StringUtilsTest, Center){
    string myString = "banana";
    string result = StringUtils::Center(myString, 20, 'O');
    EXPECT_EQ(result, "OOOOOOObananaOOOOOOO");
    result = StringUtils::Center(myString, 19, 'O');
    EXPECT_EQ(result, "OOOOOOObananaOOOOOO");
    result = StringUtils::Center("Bug", 10, 'O');
    EXPECT_EQ(result, "OOOBugOOOO");
    result = StringUtils::Center("Bug", 9, 'O');
    EXPECT_EQ(result, "OOOBugOOO");
}

TEST(StringUtilsTest, LJust){
    string myString = " Mem mem mi mi   ";
    string result = StringUtils::LJust(myString, 20, 'O');
    EXPECT_EQ(result, " Mem mem mi mi   OOO");
}

TEST(StringUtilsTest, RJust){
    string myString = " Mem mem mi mi   ";
    string result = StringUtils::RJust(myString, 40, 'O');
    EXPECT_EQ(result, "OOOOOOOOOOOOOOOOOOOOOOO Mem mem mi mi   ");
    
}

TEST(StringUtilsTest, Replace){
    string myString = "ByeHelloBye ByBye";
    string result = StringUtils::Replace(myString,"Bye","Hello");
    EXPECT_EQ(result, "HelloHelloHello ByHello");

    myString = "here there wheretherehereherthe whe hehe when        hetherere";
    result = StringUtils::Replace(myString, "re", "a");

    EXPECT_EQ(result, "hea thea wheatheaheaherthe whe hehe when        hetheaa");

}

TEST(StringUtilsTest, Split){
    string myString = "there are no donuts but there are bagels";
    vector<string> expected = {"there", "are", "no", "donuts", "but", "there", "are", "bagels"};
    vector<string> result = StringUtils::Split(myString," ");
    // StringUtils::Split("laaPahlaaaslldlaadsfnwlaaskdflaa30234", "la");
    EXPECT_EQ(result, expected);
    // string toughTest = "A\ntougher\ntest\nto\npass!";
    // result = StringUtils::Split(toughTest, "A");
    // EXPECT_EQ(result, expected);
}

TEST(StringUtilsTest, Join){
    vector<string> myVector = {"there", "are", "no", "bagels"};
    string result = StringUtils::Join("/", myVector);
    EXPECT_EQ(result, "there/are/no/bagels");
    
}

TEST(StringUtilsTest, ExpandTabs){
    string myString = "H\te\t\t\t\tl\tl\to";
    // StringUtils::ExpandTabs("H\tesdaslkfsdf\tl\tl\to");
    string result = StringUtils::ExpandTabs(myString);

    EXPECT_EQ(result,"H   e               l   l   o");
    string myStringTwo = "H\tell\to\tAnd\tByeBy\te";
    string resultTwo = StringUtils::ExpandTabs(myStringTwo);
    EXPECT_EQ(resultTwo, "H   ell o   And ByeBy   e");
}

TEST(StringUtilsTest, EditDistance){
    string myString = "Horse";
    string target = "ros";
    int result = StringUtils::EditDistance(myString,target);
    EXPECT_EQ(result, 3);
}
