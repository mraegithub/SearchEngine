// Created by Matthew Guevarra (UIN: 662725501)
// Project 2, search.h CS251
//This assignment creates a functional search engine that can index search terms and retrieves results

/** Creative Component: Made a 'stop list' that can filter out any words from the index that the user specifies.
 * This not only acts as a filter for 'stop words' but for any word the user decides to use.
 * This component is activate by calling the function 'buildIndex' overloaded with a 1 as an extra parameter. This function
 * can be called in main. Using this function will look like 'buildIndex(filename, index, 1)'
 **/
#include <iostream>
#include "search.h"
using namespace std;
int main() {
   
    string filename = "stackoverflow.txt";
    
    
    // Use this function to call and test the functions you write inside of
    // search.h.  If you do not call the functions here, nothing will run.
    //searchEngine("cplusplus.txt");
    
    searchEngine(filename);
    




    
    
    return 0;
    
}

