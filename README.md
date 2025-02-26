

# DFA Minimization Algorithm Implementation🚀

> **Book of reference 🧾:**  
> Kozen, D. C. (1997). *Automata and Computability.*  
>  
> In this project, we implement the DFA minimization algorithm as described by Kozen (1997). Our objective is to identify equivalent states in a DFA and collapse them, resulting in a minimized automaton that accepts the same language as the original in a c++ program.
> 
---
## Student Information 👨‍🎓

- **Full Names:**  
  - Matias Martinez Moreno
  - Samuel Orozco Valencia

- **Class Number:**  
  Classroom Wednesdays SI2002-2 (7309)

---
## Environment and Tools 💻
- **Operating System:**  
  - Windows 11
- **Programming Language:**  
  - C++ (compiled using g++)
- **Tools Used:**  
  - Visual Studio Code (Editor)  
  - MSYS2 / MinGW 
  - Command Prompt / Terminal (for compilation and execution)
---
## How to run the Implementation❓

1. **Compilation:**
   - Open your terminal or command prompt.
   - Navigate to the directory containing the source code file (`main.cpp`).
   - Compile the code using the following command:
     ```sh
     g++ -o dfa_minimization main.cpp
     ```
2. **Execution:**
   - Ensure the input file `Test.txt` is in the same directory as the compiled executable.
   - Run the executable with:
     ```sh
     ./dfa_minimization
     ```
   - The program will read the DFA from `Test.txt` and print out the pairs of equivalent states in lexicographical order.
---
# Input and output ✌️
**Input Format:**

1. A line with the number of cases `c > 0`.
2. For each case:
   - A line with a number `n > 0` (number of states).
   - A line with the alphabet (symbols separated by spaces).
   - A line with the final states (space-separated).
   - `n` lines representing the transition table (each line: state index followed by transitions).

**Output Format:**
- For each case, a single line is printed containing the equivalent state pairs in lexicographical order, with the pairs separated by spaces.
---
**Example:**
*Input:*
```
1
6
a
1 4
0 1
1 2
2 3
3 4
4 5
5 0
```
*Output:*
`(0,3) (1,4) (2,5)`

---
## 🟢 Explanation of the Algorithm 

The algorithm is designed to minimize a Deterministic Finite Automaton (DFA) by identifying and merging equivalent states. 

### **Algorithm Overview**

1. **Initialization:**
   - **Objective:** Mark pairs of states that are immediately distinguishable, that is, one state is final and the other is not.
   - **Code Fragment:**
     ```cpp
     // Mark pairs where one state is final and the other is not
     for (int i = 0; i < machine.totalStates; i++) {
         for (int j = i + 1; j < machine.totalStates; j++) {
             bool isAcceptingState_i = machine.acceptingStates.count(i);
             bool isAcceptingState_j = machine.acceptingStates.count(j);
             if (isAcceptingState_i!= isAcceptingState_j) {
                 diffMatrix[i][j] = true;
             }
         }
     }
     ```

2. **Iterative Refinement:**
   - **Objective:** Iteratively check pairs of states. If, for any symbol, the transitions from the two states lead to a pair that is already marked as distinguishable, mark the current pair as distinguishable.
- **Code Fragment:**
  ```cpp
  bool changeOccurred;
  do {
      changeOccurred = false;  // Reset the change indicator for each iteration
      for (int i = 0; i < machine.totalStates; i++) {
          for (int j = i + 1; j < machine.totalStates; j++) {
              // Process the pair only if it has not been marked as distinguishable yet
              if (!diffMatrix[i][j]) {
                  // Iterate over each symbol of the alphabet to check transitions
                  for (size_t k = 0; k < machine.charSet.size(); k++) {
                      // Get the state transitioned to from i and j with the k-th symbol
                      int nextState1 = machine.transTable[i][k];
                      int nextState2 = machine.transTable[j][k];
                      // If the transitions lead to different states
                      if (nextState1 != nextState2) {
                          // Ensure the pair is compared in order (smaller, larger)
                          int minState = min(nextState1, nextState2);
                          int maxState = max(nextState1, nextState2);
                          // If the state pair from the transition is already marked as distinguishable,
                          // mark the pair (i,j) as distinguishable.
                          if (diffMatrix[minState][maxState]) {
                              diffMatrix[i][j] = true;
                              changeOccurred = true;  // Indicate that a change occurred in this iteration
                              break;         // Exit the symbol loop as the pair is now distinguishable
                          }
                      }
                  }
              }
          }
      }
  } while (changeOccurred); // Repeat until no new changes are detected
  


3. **Collection of Equivalent States:**
   - **Objective:** After the iterative marking process, pairs of states that remain unmarked are considered equivalent. These pairs can then be merged to form the minimized DFA.
   - **Code Fragment:**
```cpp
for (int i = 0; i < machine.totalStates; i++) {
    for (int j = i + 1; j < machine.totalStates; j++) {
        if (!diffMatrix[i][j]) {  // If the pair (i,j) is not distinguishable
            equivSet.insert({min(i, j), max(i, j)});
        }
    }
}
   ```

### **Output Generation**

- The minimized DFA's equivalent state pairs are printed on a single line for each case.
- **Output Code Snippet:**
  ```cpp
  if (!equivSet.empty()) {
      bool firstPair = true;
      // Iterate through each pair in the set
      for (const auto &p : equivSet) {
          if (!firstPair) cout << " ";
          cout << "(" << statePair.first << "," << statePair.second << ")";
          firstPair = false;
      }
      cout << endl;
  } else {
      cout << endl; // Print an empty line if there are no equivalent pairs
  }



