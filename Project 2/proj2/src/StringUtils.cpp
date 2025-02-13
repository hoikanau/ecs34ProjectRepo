#include "StringUtils.h"
#include <iostream>
#include <sstream>
#include <cctype>
#include <cmath>
#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

namespace StringUtils{

std::string Slice(const std::string &str, ssize_t start, ssize_t end) noexcept{
    string inputString = str;
    stringstream resultString;
    // Check if the start is less than 0 meaning the user input a negative index
    if (start < 0){
        // Add the negative index by the size of the given string to get the positive index
        start = start + inputString.size();
    }
    // Check if the end is less than 0 meaning the user input a negative index
    if (end < 0){
        // Same process to get the positive index
        end = end + inputString.size();
    }
    // Loop from start index to the end index
    for (ssize_t i = start; i < end; i++){
        // Input each index from the start to the end index but not including the end
        resultString << inputString.at(i);
    }
    // store the parsed string into finalString then return it
    string finalString = resultString.str();
    return finalString;
}

std::string Capitalize(const std::string &str) noexcept{
    string inputString = str;
    // Turn all the string into lower case
    for (size_t i = 0; i < inputString.length(); i++){
        inputString.at(i) = tolower(inputString.at(i));
    }
    // Turn the first char to upper case
    inputString.at(0) = toupper(inputString.at(0));
    // store the string in finalString
    string finalString = inputString;
    return finalString;
}

std::string Upper(const std::string &str) noexcept{
    string inputString = str;
    // Turn all the char to upper case by looping through the size
    for(size_t i = 0; i < inputString.size(); i++){
        inputString.at(i) = toupper(inputString.at(i));
    }
    string finalString = inputString;
    return finalString;
}

std::string Lower(const std::string &str) noexcept{
    string inputString = str;
    // Loop through the size of the string and turn individual char to lower case
    for(size_t i = 0; i < inputString.size(); i++){
        inputString.at(i) = tolower(inputString.at(i));
    }
    string finalString = inputString;
    return finalString;
}

std::string LStrip(const std::string &str) noexcept{
    string inputString = str;
    int firstChar;
    // Loop throuhg the entire string from left to right
    for (size_t i = 0; i < inputString.size(); i++){
        // Break out of the loop once we detect the first instance where the string is not a white space
        if(inputString.at(i) != ' '){
            firstChar = i;
            break;
        }
    }
    // Parse the string using the slice function i created with start being firstChar to the size of the string
    string finalString = StringUtils::Slice(inputString, firstChar, inputString.size());
    return finalString;
}

std::string RStrip(const std::string &str) noexcept{
    string inputString = str;
    int firstChar;
    // Loop through the entire string from right to left
    for (int i = str.size() - 1; i > -1; i--){
        // break oout of loop once we detect the first instance where the string is not a white space
        if(inputString.at(i) != ' '){
            firstChar = i+1;
            break;
        }
    }
    // Parse the string with firstChar as the end and 0 as the start
    string finalString = StringUtils::Slice(inputString, 0, firstChar);

    // for(size_t i = str.size() - 1; i > -1; i--){
    //     cout << inputString.at(i);
    // }
    // cout << "\n";
    return finalString;
}

std::string Strip(const std::string &str) noexcept{
    string inputString = str;
    int leftFirstChar;
    int rightFirstChar;
    // Find the first instance on the left where the char is not a white space
    for (size_t i = 0; i < inputString.size(); i++){
        if(inputString.at(i) != ' '){
            // cout << "Found the first char that is not a whitespace from the left which is: " << inputString.at(i) << " at index: " << i << "\n";
            leftFirstChar = i;
            break;
        }
    }
    // Find the first instance on the right where the char is not a white space
    for (int i = str.size() - 1; i > -1; i--){
        if(inputString.at(i) != ' '){
            // cout << "Found the first char that is not a whitespace from the right which is: " << inputString.at(i) << " at index: " << i << "\n";
            rightFirstChar = i+1;
            break;
        }
    }
    // Parse the string from leftFirstChar to rightFirstChar
    string finalString = StringUtils::Slice(inputString, leftFirstChar, rightFirstChar);
    return finalString;
}

// Helper function to fill empty spaces
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
    // FIgure out if the given width is odd or even
    if(width % 2 == 0){
        widthEven = true;
    }else{
        widthEven = false;
    }
    // Figure out if the size of the string is odd or even
    if(inputString.size() % 2 == 0){
        stringEven = true;
    }else{
        stringEven = false;
    }
    // 3 cases:
    //    1. Even string even width or odd string and odd width
    //    2. Even string odd width
    //    3. Odd string even width
    if((widthEven && stringEven) || (!widthEven && !stringEven)){
        // calculate how much whitespace to fill the left
        fillLeft = (width/2) - (str.size()/2);
        // Throw fillLeft into helper function
        fillString(runningString, fillLeft, fill);
        // Throw given string into the string stream
        runningString << inputString;
        // the right has equivalent white spaces needed to fill
        fillRight = fillLeft;
        // Throw fillRight into helper function
        fillString(runningString, fillRight, fill);
    }else if(widthEven == false && stringEven){
        // Same process
        fillLeft = (width/2) - (str.size()/2);
        fillLeft++;
        fillString(runningString, fillLeft, fill);
        runningString << inputString;
        // Left heavy so there is more whitespace on the left than right
        fillRight = fillLeft - 1;
        fillString(runningString, fillRight, fill);
    }else if(widthEven && stringEven == false){
        // Same process
        fillLeft = (width/2) - (inputString.size()/2);
        // Right heavy so there is more white space on the right than left
        fillLeft--;
        fillString(runningString, fillLeft, fill);
        runningString << inputString;
        fillRight = fillLeft + 1;
        fillString(runningString, fillRight, fill);
    }
    // Store string to finalString and return
    finalString = runningString.str();
    return finalString;
}

std::string LJust(const std::string &str, int width, char fill) noexcept{
    string inputString = str;
    stringstream runningString;
    // Calculate the amount of whitespaces needed to fill on the right
    int amountToFill = width - inputString.size();
    amountToFill = abs(amountToFill);
    runningString << inputString;
    // Fill using helper function
    fillString(runningString, amountToFill, fill);
    string finalString = runningString.str();
    return finalString;
}

std::string RJust(const std::string &str, int width, char fill) noexcept{
    string inputString = str;
    stringstream runningString;
    // Calculate amount of whitespaces needed to fill on the left
    int amountToFill = width - inputString.size();
    amountToFill = abs(amountToFill);
    // Fill using helper function
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
    // int sizeOfOldString = old.size();
    unordered_map<int,bool> OldStringIndexes;
    // Find the first instance where the string is the same
    int matchingIndex = inputString.find(old);
    // Mark the index in a map to be replaced later
    OldStringIndexes[matchingIndex] = true;
    // loop until there are no more same string in the given string
    while(matchingIndex != -1){
        matchingIndex = inputString.find(old, matchingIndex+1);
        if (matchingIndex != -1){
            OldStringIndexes[matchingIndex] = true;
        }
    }
    // loop for the size of the string
    for(size_t i = 0; i < inputString.size(); i++){
        // if the index has been marked as the string to be replaced
        if(OldStringIndexes[i] == true){
            // input the replace string in place of the old string
            runningString << replaceAs;
            // push the pointer past the old string
            i = i +(old.size()-1);
        }else{
            // If it is not one of the marked indexes just input the char
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
    // for(size_t i = matchingIndex; i < matchingIndex+splt.size(); i++){
    //     SplitCharIndex[i] = true;
    // }

    // while(matchingIndex != -1){
    //     matchingIndex = inputString.find(splt, matchingIndex+1);
    //     if (matchingIndex != -1){
    //         // cout << "Found matching starting at: " << matchingIndex << "\n";
    //         SplitCharIndex[matchingIndex] = true;
    //         // cout << matchingIndex << " " << splt.size() << "\n";
    //         for(size_t i = matchingIndex; i < matchingIndex+splt.size(); i++){
    //             // cout << "Rest of the matching string: " << i << "\n";
    //             SplitCharIndex[i] = true;
    //         }
    //     }
    // }
    // for(size_t i = 0; i < inputString.size(); i++){
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

    // for(size_t i = VectorOfString.size()-1; i > -1; i--){
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
    // Loop for the size of the vector
    for (size_t i = 0; i < vect.size(); i++){
        // Do not input the char used to join the strings in the vector if it is the beginning or end
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

    // for(size_t i = 0; i < inputString.size(); i++){
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
    //             for(size_t i = 0; i < tabsize-1; i++){
    //                 runningString << " ";
    //             }
    //         }
    //         startSplit = endSplit + searchFor.size();
    //     // }
    // }
    // Base case if the tab doesnt exist then just return the string
    if(inputString.find("\t") == -1){
        return inputString;
    }

    // loop for all the characters on the input string
    for(char x : inputString){
        // if the char is a tab then:
        if (x == tab){
            // Calculate the whitespaces left given the current position of the string
            // i.e. how many white spaces left to input
            whiteSpaces = (currPos % tabsize) - tabsize;
            whiteSpaces = abs(whiteSpaces);
            // Input i < whiteSpaces amount of white spaces
            for(size_t i = 0; i < whiteSpaces; i++){
                runningString << ' ';
            }
            // Update the current position pass the white spaces
            currPos += whiteSpaces;
        }else{
            // if it is not a tab just input the char
            runningString << x;
            currPos += 1;
        }
    }
    string finalString = runningString.str();
    
    return finalString;
}

int EditDistance(const std::string &left, const std::string &right, bool ignorecase) noexcept{
    string inputStringOne = left;
    string inputStringTwo = right;
    // Introduce a matrix of size inputStringOne by inputStringTwo while including 2 base cases of empty strings
    // Hence "inputStringOne.size() + 1"
    vector<vector<int>> Matrix2D (inputStringOne.size() + 1, vector<int>(inputStringTwo.size() + 1, 0));
    // Base cases that we can determine the immediate minimum length
    // if(left == right){
    //     return 0;
    // }
    // if(left == " "){
    //     return right.size();
    // }
    // if(right == " "){
    //     return left.size();
    // }
    // Loops for the size of inputStringOne plus the empty string
    for(size_t i = 0; i < inputStringOne.size() + 1; i++){
        // The empty string will always have a minimum of the size of the subtring at the given index
        Matrix2D[i][0] = i;
    }
    // Same thing for the second string
    for(size_t i = 0; i < inputStringTwo.size() + 1; i++){
        Matrix2D[0][i] = i;
    }

    // Nested for loop that loops through all the "squares" in the 2D matrix to calculate all the minimum operations
    // for insertion, deletion, replacement, or if the string is the same
    // Additionally we already computed both column 0 and row 0 because they are empty strings so we can start at 1
    for(size_t i = 1; i < inputStringOne.size()+1; i++){
        for(size_t j = 1; j < inputStringTwo.size()+1; j++){
            // There are 3 paths are can take so we must consider all the operations that can occur and take the minimum
            // Deleting means to remove the character
            int deleting = Matrix2D[i-1][j]+1;
            // Inserting is to place a character in front;
            int inserting = Matrix2D[i][j-1]+1;
            // Replacing is to replace the current char to the new char we want from the second string
            int replacing = Matrix2D[i-1][j-1];
            // This ensures that if the two characters we are replacing are in fact different add 1 to the operation
            if(inputStringOne[i-1] != inputStringTwo[j-1]){
                replacing = replacing + 1;
            }
            // Calculate the minimum operation given the 3 different techniques
            Matrix2D[i][j] = min(min(deleting,inserting),replacing);
        }
    }
    // The method I am computing is from top to bottom so the minimum operations is at the bottom right most box
    return Matrix2D[inputStringOne.size()][inputStringTwo.size()];
}

};