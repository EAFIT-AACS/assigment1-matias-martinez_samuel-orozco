//MATIAS MARTINEZ
//SAMUEL OROZCO
#include <vector>    
#include <iostream>   
#include <set>        // Allows using the set data structure (ordered collection of elements)
#include <map>        // Allows using the map data structure (key-value association)
#include <sstream>    // Allows manipulation of strings
#include <limits>
#include <fstream>    //Allows file handling (ifstream for reading the text file)
#include <algorithm>
using namespace std;  

struct DFA {
    int totalStates;                    // Total number of states in the automaton
    vector<char> charSet;               // Set of symbols that compose the DFA alphabet
    set<int> acceptingStates;           // Set of final accepting states of the DFA
    vector<vector<int>> transTable;     // Transition table
                                        // Each row corresponds to a state, and each column to a symbol of the alphabet.
                                        // The value at [i][j] represents the state transitioned to from state i with the corresponding symbol.
};

DFA readDFAFromFile(ifstream &file);
/*
 * The algorithm consists of:
 * 1:Initially marking pairs of states where one is final and the other is not.
 * 2:Iterating over the transition table and marking as distinguishable those pairs whose
 *    transitions lead to states that have already been marked.
 * 3:Collecting the pairs of states that have not been marked (are equivalent).
*/
set<pair<int, int>> minimizeDFA(DFA &machine);

int main() {
    // Open the input file named "Test.txt" 
    ifstream file("Test.txt");

    // Verify that the file was opened correctly
    if (!file) {
        cout << "Error while opening the file, make sure it has the correct extension and that the file is in the same folder as the Main.cpp"<< endl;
        return 1; 
    }

    int testCases; // Variable that will store the number of cases (DFAs) to be read
    file >> testCases; // Read the number of cases from the file
    file.ignore();     

    //Process each case  iteratively
    while (testCases--) {
        // Read the DFA from the file
        DFA machine = readDFAFromFile(file);
        
        // Minimize the DFA and obtain the set of equivalent state pairs
        set<pair<int, int>> equivSet = minimizeDFA(machine);

        // Print the equivalent pairs in a single line, separated by spaces
        if (!equivSet.empty()) {
            bool firstPair = true; //Flag to control printing of the separator
            // Iterate through each pair in the set
            for (const auto &statePair : equivSet) {
                if (!firstPair) 
                    cout << " ";  // Print a space between pairs, except before the first one
                cout << "(" << statePair.first << "," << statePair.second << ")";  
                firstPair = false;  // Update the flag after the first element
            }
            cout << endl; 
        } else {
            cout << endl; //empty line if there are no equivalent pairs
        }
    }

    file.close(); //Close the input file
    return 0;  //terminate the program 
}

DFA readDFAFromFile(ifstream &file) {
    DFA machine;         
    string inputLine;    

    getline(file, inputLine);         // Read the line that contains the number of states
    machine.totalStates = stoi(inputLine);    // Convert the string to an integer and store it in totalStates

    getline(file, inputLine);         // Read the line that contains the alphabet (symbols separated by spaces)
    istringstream charStream(inputLine);  // Create a stream from the line to process each symbol
    char currentChar;                 // Variable to store each symbol
    while (charStream >> currentChar) {  // Extract each symbol from the stream
        machine.charSet.push_back(currentChar); // Add the symbol to the DFA's alphabet vector
    }

    getline(file, inputLine);         // Read the line that contains the final states (separated by spaces)
    istringstream acceptStream(inputLine);  // Create a stream from the line
    int accState;              // Variable to store each final state
    while (acceptStream >> accState) { // Extract each final state
        machine.acceptingStates.insert(accState); // Insert the state into the set of final states
    }

    // Resize the transition table to have as many rows as states,
    // and each row to have as many columns as symbols in the alphabet.
    machine.transTable.resize(machine.totalStates, vector<int>(machine.charSet.size()));

    // Read each of the transitions for each state
    for (int i = 0; i < machine.totalStates; i++) {
        getline(file, inputLine);    // Read the line corresponding to the transitions of state i
        istringstream transStream(inputLine);  // Create a stream to process the line
        int dummyState;
        transStream >> dummyState;     
        // Read the transitions for each symbol of the alphabet
        for (int j = 0; j < machine.charSet.size(); j++) {
            transStream >> machine.transTable[i][j]; //Store the corresponding transition
        }
    }
    return machine; // return the DFA constructed with the file information
}

set<pair<int, int>> minimizeDFA(DFA &machine) {
    // Create a matrix to mark pairs of distinguishable states.
    vector<vector<bool>> diffMatrix(machine.totalStates, vector<bool>(machine.totalStates, false));

    // Set to store pairs of equivalent states (not distinguishable)
    set<pair<int, int>>equivSet;

    //Mark the pairs of states that are immediately distinguishable
    for (int i = 0; i < machine.totalStates; i++) {
        for (int j = i + 1; j < machine.totalStates; j++) {
            bool isAcceptingState_i = machine.acceptingStates.count(i);  // Check if state i is final
            bool isAcceptingState_j = machine.acceptingStates.count(j);  // Check if state j is final
            if (isAcceptingState_i != isAcceptingState_j) { // If one is final and the other is not, mark as distinguishable
                diffMatrix[i][j] = true;
            }
        }
    }

    // Apply the table marking algorithm to detect other pairs of distinguishable states.
    // A do-while loop is used to repeat the process until no changes occur.
    bool changeOccurred;
    do {
        changeOccurred = false; 
        for (int i = 0; i < machine.totalStates; i++) {
            for (int j = i + 1; j < machine.totalStates; j++) {
                // Process the pair only if it has not been marked as distinguishable yet
                if (!diffMatrix[i][j]) {
                    // Iterate over each symbol of the alphabet to check transitions
                    for (size_t k = 0; k < machine.charSet.size(); k++) {
                        int nextState1 = machine.transTable[i][k];
                        int nextState2 = machine.transTable[j][k];
                        // If the transitions lead to different states
                        if (nextState1 != nextState2) {
                            int minState = min(nextState1, nextState2);
                            int maxState = max(nextState1, nextState2);
                            // If the state pair from the transition is already marked as distinguishable,
                            // we mark the pair (i,j)as distinguishable.
                            if (diffMatrix[minState][maxState]) {
                                diffMatrix[i][j] = true;
                                changeOccurred = true;  // Indicate that a change occurred in this iteration
                                break; // Exit the symbol loop as the pair is now distinguishable
                            }
                        }
                    }
                }
            }
        }
    } while (changeOccurred);//Repeat until no new changes are detected

    // Collect the pairs of states that have not been marked as distinguishable
    // These pairs are equivalent and can be collapsed in the DFA minimization.
    for (int i = 0; i < machine.totalStates; i++) {
        for (int j = i + 1; j < machine.totalStates; j++) {
            if (!diffMatrix[i][j]) {  // If the pair (i,j) is not distinguishable
                equivSet.insert({min(i, j), max(i, j)});
            }
        }
    }
    return equivSet;  // Return the set of the equivalent state pairs
} 
