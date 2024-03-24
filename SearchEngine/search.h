// Created by Matthew Guevarra (UIN: 662725501)
// Project 2, search.h CS251
//This assignment creates a functional search engine that can index search terms and retrieves results

/** Creative Component: Made a 'stop list' that can filter out any words from the index that the user specifies.
 * This not only acts as a filter for 'stop words' but for any word the user decides to use.
 * This component is activate by calling the function 'buildIndex' overloaded with a 1 as an extra parameter. This function
 * can be called in main. Using this function will look like 'buildIndex(filename, index, 1)'
 **/

#pragma once

#include <iostream>
#include <set>
#include <map>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;


// Removes punctuation characters from beginning and end of string.
// This is done by removing the punctuation at the front, then flipping the string
// and removing at the front again. The string is then flipped to its original position.
string cleanToken(string s) {
    /**
    bool validToken = false;
    for (int i = 0; i < s.size(); i++) {
        if (isalpha(s.at(i))) {
            validToken = true;
            break;
        }
    }
    if (validToken == true) {
        string firstString;
        string secondString;
        int firstChar = 0;
        for (int i = 0; i < s.size(); i++) {
            if (!ispunct(s.at(i))) {
                firstString = s.substr(i, s.size());
                break;
            }
        }
        int n = firstString.size();
        for (int i = 0; i < n / 2; i++) {
            swap(firstString[i], firstString[n - i - 1]);
        }
        for (int i = 0; i < n; i++) {
            if (!ispunct(firstString.at(i))) {
                secondString = firstString.substr(i, n);
                break;
           }
        } 
        int j = secondString.size();
        for (int i = 0; i < j / 2; i++) {
            swap(secondString[i], secondString[j - i - 1]);
        }
        cout << secondString << endl;
        return secondString;
    } else if (validToken == false)
        return "";
        **/
        string cleanToken = "";
       for (char a: s) {
        if (isalpha(a)|| a == '+') {
            cleanToken = cleanToken + a;
            
        }
       }
       cout << cleanToken << endl;
       
       return cleanToken;

}

//Retrieves words and converts them to lowercase. These words are then passed into a set
// to be used as keys for the map.
set<string> gatherTokens(string text) {
    set<string> tokens;
    
    stringstream lines(text);

    string singleToken;

    while (lines >> singleToken) {
        for (int i = 0; i < singleToken.size(); i++) {
            cout << singleToken << endl;
            singleToken[i] = tolower(singleToken[i]);
        }
        string passedToken = cleanToken(singleToken);
        if (passedToken != "") {
            tokens.insert(passedToken);
        }
    }
    return tokens;  
}

//Helper function for buildIndex. Passes token into map key and assigns key with a URL value;
void buildHelper(string linkURL, string tokenLine, map<string, set<string>> &index) {
   set<string> keys = gatherTokens(tokenLine);
    for (string passToken : keys) {
        index[passToken].insert(linkURL);
    }

}

//Function reads file from input. This file is read and is used to create an index map that will store key words and URLS 
//as key/value pair.
int buildIndex(string filename, map<string, set<string>>& index) {
    ifstream inFS;
    inFS.open(filename);
    if (!inFS.is_open()) {
		cout << "Error: unable to open '" << filename << "'" << endl;
    return 0;
	}
    string linkURL;
    string tokenLine;
    int rowNumber = 0;
    getline(inFS, linkURL);
    while (!inFS.eof() && linkURL != "") {
        getline(inFS, tokenLine);
        buildHelper(linkURL, tokenLine, index);
        getline(inFS, linkURL);
        rowNumber++;
    }
    
    return rowNumber;
    
}

//Helper function that finds set difference, intersection, and union between two sets taken as input
set<string> evaluateMatches(set<string> &queryListOne, set<string> &queryListTwo, char relation) {
    set<string> results;
    if (relation == '-') {
        set_difference(begin(queryListOne), end(queryListOne), begin(queryListTwo), end(queryListTwo), inserter(results, begin(results)));
    } else if (relation == '+') {
        set_intersection(begin(queryListOne), end(queryListOne), begin(queryListTwo), end(queryListTwo), inserter(results, begin(results)));
    } else {
        set_union(begin(queryListOne), end(queryListOne), begin(queryListTwo), end(queryListTwo), inserter(results, begin(results)));
    }
    return results;
}

//Initial query that finds relation of first two URL sets and stores them into a single result set. This result set is then
//Compared with subsequent sets after.
void firstQuery(set <string> &results, vector<string> queryList, map<string, set<string>>& index) {
    set<string> set1 = index[cleanToken(queryList[0])];
    set<string> set2 = index[cleanToken(queryList[1])];
        if (queryList[1].at(0) == '-') {
        set_difference(begin(set1), end(set1), begin(set2), end(set2), inserter(results, begin(results)));
    } else if (queryList[1].at(0) == '+') {
        set_intersection(begin(set1), end(set1), begin(set2), end(set2), inserter(results, begin(results)));
    } else {
        set_union(begin(set1), end(set1), begin(set2), end(set2), inserter(results, begin(results)));
    }
    queryList.erase(queryList.begin());
    queryList.erase(queryList.begin());
}


// Query function that finds URLs based off of search word input. Stores search terms into a vector
// and performs set operations until vector of search terms is empty.
set<string> findQueryMatches(map<string, set<string>>& index, string sentence) {
    for (int i = 0; i < sentence.size(); i++) {
        sentence[i] = tolower(sentence[i]);
    }
    stringstream input(sentence);
    string currentQuery;
    vector<string> queryList;
    set <string> results;
    char relation;
    while (input >> currentQuery) { 
        queryList.push_back(currentQuery);
    }
    if (queryList.size() == 1) {
        results = index[queryList.at(0)];
        return results;
    } 
    if (queryList.size() > 1) {
        firstQuery(results, queryList, index);
    }
    while (queryList.size() > 0) {
        set<string> firstQuery = index[cleanToken(queryList[0])];
        if (queryList[0].at(0) == '-') {
        results = evaluateMatches(results, firstQuery, '-');
        } else if (queryList[0].at(0) == '+') {
        results = evaluateMatches(results, firstQuery, '+');
        } else {
        results = evaluateMatches(results, firstQuery, 'n');
        }
        queryList.erase(queryList.begin());
    }
    return results;  
}

//Creative component. Allows user input of stop words that will be filtered from the index. Is called when
// 'buildIndex' with overloaded 1 is called.
set<string> stopList() {
    cout << "Please enter any words you would like to filter out. Press enter to quit." << endl;
    string filteredWord;
    getline(cin, filteredWord);
    set<string> stopList;
    while (filteredWord != "") {
        stopList.insert(filteredWord);
        getline(cin, filteredWord);
    }
    return stopList;
}

//Helper for creative component. Will prevent words filtered out by 'stopList' from going into map index.
void buildHelperWithStop(string linkURL, string tokenLine, map<string, set<string>> &index, set<string> stopList) {
   set<string> keys = gatherTokens(tokenLine);
    for (string passToken : keys) {
        if (stopList.count(passToken) != 1) {
            index[passToken].insert(linkURL);
        }
    }

}

//Creative function. Calling this version of buildIndex will allow user to uset stopList function. This function will allow
//user to filter out words when creating the index.
int buildIndex(string filename, map<string, set<string>>& index, int i) {
    set<string> newStopList;
    if(i == 1) {
        newStopList = stopList();
    }
    ifstream inFS;
    inFS.open(filename);
    if (!inFS.is_open()) {
		cout << "Error: unable to open '" << filename << "'" << endl;
    return 0;
	}
    string linkURL;
    string tokenLine;
    int rowNumber = 0;
    getline(inFS, linkURL);
    while (!inFS.eof() && linkURL != "") {
        getline(inFS, tokenLine);
        buildHelperWithStop(linkURL, tokenLine, index, newStopList);
        getline(inFS, linkURL);
        rowNumber++;
    }
    
    return rowNumber;
    
}



//Search engine function. Creates index and counts how many URLs are indexed and how many unique terms are created.
//Allows user to continually input key words which are then searched in the index. User can exit out of this function by 
//Hitting the enter key.
void searchEngine(string filename) {
    map<string, set<string>> index;
    int matchNumber = buildIndex(filename, index);

    cout <<"Stand by while building index..." << endl;
    cout << "Indexed " << matchNumber << " pages containing " << index.size() <<" unique terms" << endl;
    cout << endl;
    cout <<"Enter query sentence (press enter to quit): ";
    string input;
    getline(cin, input);
    while (input !="") {
        set<string> engineResults = findQueryMatches(index, input);
        cout << "Found: " << engineResults.size() << " matching pages " << endl;
        for (auto x : engineResults) {
            cout << x << endl;
        }
        cout << endl;
        cout <<"Enter query sentence (press enter to quit): ";
        getline(cin, input);
    }
    cout << "Thank you for searching!" << endl;

    
}


