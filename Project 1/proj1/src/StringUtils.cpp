#include "StringUtils.h"
#include <iostream>
#include <sstream>
#include <cctype>
#include <cmath>
#include <unordered_map>
#include <string>

using namespace std;

namespace StringUtils{

std::string Slice(const std::string &str, ssize_t start, ssize_t end) noexcept{
    string inputString = str;
    stringstream resultString;
    if (start < 0){
        start = start + str.size();
    }
    if (end < 0){
        end = end + str.size();
    }
    for (int i = start; i < end; i++){
        resultString << inputString.at(i);
    }
    string finalString = resultString.str();
    return finalString;
}

std::string Capitalize(const std::string &str) noexcept{
    string inputString = str;
    for (int i = 0; i < inputString.length(); i++){
        inputString.at(i) = tolower(inputString.at(i));
    }
    inputString.at(0) = toupper(inputString.at(0));
    string finalString = inputString;
    return finalString;
}

std::string Upper(const std::string &str) noexcept{
    string inputString = str;
    for(int i = 0; i < inputString.size(); i++){
        inputString.at(i) = toupper(inputString.at(i));
    }
    string finalString = inputString;
    return finalString;
}

std::string Lower(const std::string &str) noexcept{
    string inputString = str;
    for(int i = 0; i < inputString.size(); i++){
        inputString.at(i) = tolower(inputString.at(i));
    }
    string finalString = inputString;
    return finalString;
}

std::string LStrip(const std::string &str) noexcept{
    string inputString = str;
    int firstChar;
    for (int i = 0; i < inputString.size(); i++){
        if(inputString.at(i) != ' '){
            firstChar = i;
            break;
        }
    }

    string finalString = StringUtils::Slice(inputString, firstChar, inputString.size());
    return finalString;
}

std::string RStrip(const std::string &str) noexcept{
    string inputString = str;
    int firstChar;
    for (int i = str.size() - 1; i > -1; i--){
        if(inputString.at(i) != ' '){
            firstChar = i+1;
            break;
        }
    }
    string finalString = StringUtils::Slice(inputString, 0, firstChar);

    // for(int i = str.size() - 1; i > -1; i--){
    //     cout << inputString.at(i);
    // }
    // cout << "\n";
    return finalString;
}

std::string Strip(const std::string &str) noexcept{
    string inputString = str;
    int leftFirstChar;
    int rightFirstChar;
    for (int i = 0; i < inputString.size(); i++){
        if(inputString.at(i) != ' '){
            // cout << "Found the first char that is not a whitespace from the left which is: " << inputString.at(i) << " at index: " << i << "\n";
            leftFirstChar = i;
            break;
        }
    }
    for (int i = str.size() - 1; i > -1; i--){
        if(inputString.at(i) != ' '){
            // cout << "Found the first char that is not a whitespace from the right which is: " << inputString.at(i) << " at index: " << i << "\n";
            rightFirstChar = i+1;
            break;
        }
    }
    string finalString = StringUtils::Slice(inputString, leftFirstChar, rightFirstChar);
    return finalString;
}

void fillString(stringstream& myString, int amount, char fill){
    for (int i = 0; i < amount; i++){
        myString << fill;
    }
}

std::string Center(const std::string &str, int width, char fill) noexcept{
    string inputString = str;
    bool widthEven = false;
    bool stringEven = false;
    int fillLeft;
    int fillRight;
    stringstream runningString;
    string finalString;
    if(width % 2 == 0){
        widthEven = true;
    }else{
        widthEven = false;
    }

    if(inputString.size() % 2 == 0){
        stringEven = true;
    }else{
        stringEven = false;
    }

    if((widthEven && stringEven) || (!widthEven && !stringEven)){
        fillLeft = (width/2) - (str.size()/2);
        fillString(runningString, fillLeft, fill);
        runningString << inputString;
        fillRight = fillLeft;
        fillString(runningString, fillRight, fill);
    }else if(widthEven == false && stringEven){
        fillLeft = (width/2) - (str.size()/2);
        fillLeft++;
        fillString(runningString, fillLeft, fill);
        runningString << inputString;
        fillRight = fillLeft - 1;
        fillString(runningString, fillRight, fill);
    }else if(widthEven && stringEven == false){
        fillLeft = (width/2) - (inputString.size()/2);
        fillLeft--;
        fillString(runningString, fillLeft, fill);
        runningString << inputString;
        fillRight = fillLeft + 1;
        fillString(runningString, fillRight, fill);
    }
    finalString = runningString.str();
    return finalString;
}

std::string LJust(const std::string &str, int width, char fill) noexcept{
    string inputString = str;
    stringstream runningString;
    int amountToFill = width - inputString.size();
    amountToFill = abs(amountToFill);
    runningString << inputString;
    fillString(runningString, amountToFill, fill);
    string finalString = runningString.str();
    return finalString;
}

std::string RJust(const std::string &str, int width, char fill) noexcept{
    string inputString = str;
    stringstream runningString;
    int amountToFill = width - inputString.size();
    amountToFill = abs(amountToFill);
    fillString(runningString, amountToFill, fill);
    runningString << inputString;
    string finalString = runningString.str();
    return finalString;
}

std::string Replace(const std::string &str, const std::string &old, const std::string &rep) noexcept{
    stringstream runningString;
    string stringToReplace = old;
    string replaceAs = rep;
    string inputString = str;
    int sizeOfOldString = old.size();
    unordered_map<int,bool> OldStringIndexes;
    // string testOld = "Bye";
    // string testString = "ByeHelloBye ByBye";
    int matchingIndex = inputString.find(old);
    // cout << "This: " << matchingIndex << "\n";
    OldStringIndexes[matchingIndex] = true;
    while(matchingIndex != -1){
        matchingIndex = inputString.find(old, matchingIndex+1);
        if (matchingIndex != -1){
            OldStringIndexes[matchingIndex] = true;
        }
    }
    
    for(int i = 0; i < inputString.size(); i++){
        if(OldStringIndexes[i] == true){
            runningString << replaceAs;
            i = i +(old.size()-1);
        }else{
            runningString << inputString.at(i);
        }
    }
    string finalString = runningString.str();
    return finalString;
}

std::vector< std::string > Split(const std::string &str, const std::string &splt) noexcept{
    vector<string> FinalVector;
    string inputString = str;
    int startSplit = 0;
    int endSplit = 0;
    // while loop that checks if endSplit is equal to npos meaning that there are no more string that is equal to the given splt
    while(endSplit != string::npos){
        endSplit = inputString.find(splt, startSplit);
        // Parse the string so we only include the part that is separated by the split indicator
        // Then push the string onto the vector
        FinalVector.push_back(inputString.substr(startSplit, endSplit-startSplit));
        startSplit = endSplit + splt.size();
    }
     

    
    // string inputString = str;
    // unordered_map<int,bool> SplitCharIndex;
    // unordered_map<int,string> StringPerIndex;
    // vector<string> FinalVector;
    // vector<string> VectorOfString;
    // int stringOrder = 0;

    // // Base case / First instance
    // int matchingIndex = inputString.find(splt);
    // SplitCharIndex[matchingIndex] = true;
    // for(int i = matchingIndex; i < matchingIndex+splt.size(); i++){
    //     SplitCharIndex[i] = true;
    // }

    // while(matchingIndex != -1){
    //     matchingIndex = inputString.find(splt, matchingIndex+1);
    //     if (matchingIndex != -1){
    //         // cout << "Found matching starting at: " << matchingIndex << "\n";
    //         SplitCharIndex[matchingIndex] = true;
    //         // cout << matchingIndex << " " << splt.size() << "\n";
    //         for(int i = matchingIndex; i < matchingIndex+splt.size(); i++){
    //             // cout << "Rest of the matching string: " << i << "\n";
    //             SplitCharIndex[i] = true;
    //         }
    //     }
    // }
    // for(int i = 0; i < inputString.size(); i++){
    //     if(SplitCharIndex[i] == true){
    //         stringOrder++;
    //     }else{
    //         StringPerIndex[stringOrder].push_back(inputString.at(i));
    //     }
    // }

    // auto i = StringPerIndex.begin();
    // while(i != StringPerIndex.end()){
    //     VectorOfString.push_back(i->second);
    //     i++;
    // }

    // for(int i = VectorOfString.size()-1; i > -1; i--){
    //     FinalVector.push_back(VectorOfString.at(i));
    // }

    // cout <<"\n";
    // for(auto i : FinalVector){
    //     cout << i << "\n";
    // }
    // cout << "\n";

    return FinalVector;
}


std::string Join(const std::string &str, const std::vector< std::string > &vect) noexcept{
    stringstream runningString;
    string charToJoin = str;
    for(int i = 0; i < vect.size(); i++){
        if (i == vect.size()-1){
            runningString << vect.at(i);
        }else{
            runningString << vect.at(i) << charToJoin;
        }
    }
    string finalString = runningString.str();
    return finalString;
}

std::string ExpandTabs(const std::string &str, int tabsize) noexcept{
    string inputString = str;
    stringstream runningString;
    int currPos = 0;
    int whiteSpaces = 0;
    char tab = '\t';
    // unordered_map<int,bool> TabLocations;
    // int matchingIndex = inputString.find("\t");
    // TabLocations[matchingIndex] = true;
    // TabLocations[matchingIndex+1] = true;
    // while(matchingIndex != -1){
    //     matchingIndex = inputString.find("\t", matchingIndex+1);
    //     if (matchingIndex != -1){
    //         TabLocations[matchingIndex] = true;
    //         TabLocations[matchingIndex+1] = true;
    //     }
    // }

    // for(int i = 0; i < inputString.size(); i++){
    //     if(TabLocations[i] == true){
    //         if()
    //         for(int j = 0; j < tabsize; j++){
    //             runningString << " ";
    //         }
    //     }else{
    //         runningString << inputString.at(i);
    //     }
    // }
    // for(const auto& [key, value] : TabLocations){
    //     cout << key << " ";
    // // }
    // cout << "\n";

    // int startSplit = 0;
    // int endSplit = 0;
    // string searchFor = "\t";
    // // while loop that checks if endSplit is equal to npos meaning tabs in the string
    // while(endSplit != string::npos){
    //     endSplit = inputString.find("\t", startSplit);
    //     // Parse the string so we only include the part that is separated by a tab
    //     // Then push the string onto the stringstream
    //     // if(endSplit != string::npos){
    //         runningString << inputString.substr(startSplit, endSplit-startSplit);
    //         if(endSplit != string::npos){
    //             for(int i = 0; i < tabsize-1; i++){
    //                 runningString << " ";
    //             }
    //         }
    //         startSplit = endSplit + searchFor.size();
    //     // }
    // }
    if(inputString.find("\t") == -1){
        return inputString;
    }

    for(char x : inputString){
        if (x == tab){
            whiteSpaces = (currPos % tabsize) - tabsize;
            whiteSpaces = abs(whiteSpaces);
            for(int i = 0; i < whiteSpaces; i++){
                runningString << ' ';
            }
            currPos += whiteSpaces;
        }else{
            runningString << x;
            currPos += 1;
        }
    }
    string finalString = runningString.str();
    
    return finalString;
}

int EditDistance(const std::string &left, const std::string &right, bool ignorecase) noexcept{
    // Replace code here
    return 0;
}

};