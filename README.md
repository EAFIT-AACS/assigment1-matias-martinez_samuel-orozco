
# DFA Minimization Algorithm Implementation🚀

> **Book of reference 🧾:**  
> Kozen, D. C. (1997). *Automata and Computability.*  
>  
> In this project, we implement the DFA minimization algorithm as described by Kozen (1997). Our objective is to identify equivalent states in a DFA and collapse them, resulting in a minimized automaton that accepts the same language as the original in a c++ program.
> 
---

## Student Information 👨‍🎓

- **Full Names:**  
  Matias Martinez Moreno
  Samuel Orozco Valencia

- **Class Number:**  
  Classroom Wednesdays SI2002-2 (7309)

---

## Environment and Tools 💻

- **Operating System:**  
  - Windows 11
- **Programming Language:**  
  C++ (compiled using g++)

- **Tools Used:**  
  - Visual Studio Code (Editor)  
  - MSYS2 / MinGW (for compiling C++ code on Windows)  
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

## 🟢 Explanation of the Algorithm 

The algorithm is designed to minimize a Deterministic Finite Automaton (DFA) by identifying and merging equivalent states. 

### **Algorithm Overview**

1. **Initialization:**
   - **Objective:** Mark pairs of states that are immediately distinguishable, that is, one state is final and the other is not.
   - **Code Fragment:**
     ```cpp
     // Mark pairs where one state is final and the other is not
     for (int i = 0; i < dfa.numStates; i++) {
         for (int j = i + 1; j < dfa.numStates; j++) {
             bool iIsFinal = dfa.finalStates.count(i);
             bool jIsFinal = dfa.finalStates.count(j);
             if (iIsFinal != jIsFinal) {
                 distinguishable[i][j] = true;
             }
         }
     }
     ```

2. **Iterative Refinement:**
   - **Objective:** Iteratively check pairs of states. If, for any symbol, the transitions from the two states lead to a pair that is already marked as distinguishable, mark the current pair as distinguishable.
   - **Code Fragment:**
     ```cpp
     bool changed;
     do {
         changed = false;
         for (int i = 0; i < dfa.numStates; i++) {
             for (int j = i + 1; j < dfa.numStates; j++) {
                 if (!distinguishable[i][j]) {
                     for (size_t k = 0; k < dfa.alphabet.size(); k++) {
                         int nextI = dfa.transitionTable[i][k];
                         int nextJ = dfa.transitionTable[j][k];
                         if (nextI != nextJ) {
                             int minState = min(nextI, nextJ);
                             int maxState = max(nextI, nextJ);
                             if (distinguishable[minState][maxState]) {
                                 distinguishable[i][j] = true;
                                 changed = true;
                                 break;
                             }
                         }
                     }
                 }
             }
         }
     } while (changed);
     ```

3. **Collection of Equivalent States:**
   - **Objective:** After the iterative marking process, pairs of states that remain unmarked are considered equivalent. These pairs can then be merged to form the minimized DFA.
   - **Code Fragment:**
     ```cpp
     for (int i = 0; i < dfa.numStates; i++) {
         for (int j = i + 1; j < dfa.numStates; j++) {
             if (!distinguishable[i][j]) {
                 equivalentStates.insert({min(i, j), max(i, j)});
             }
         }
     }
     ```

### **Output Generation**

- The minimized DFA's equivalent state pairs are printed on a single line for each case.
- **Output Code Snippet:**
  ```cpp
  if (!equivalentStates.empty()) {
      bool first = true;
      for (const auto &p : equivalentStates) {
          if (!first) cout << " ";
          cout << "(" << p.first << ", " << p.second << ")";
          first = false;
      }
      cout << endl;
  } else {
      cout << endl;
  }

