/**
 * Unsecure computation of Edit Distance and x?
 * */

#include <iostream>

using namespace std;

#if 0
unsigned edit_distance(string &s1, string &s2) {
  // ed[i] measn to convert s1 to the first ith element of s2
  unsigned ed[6] = { 0 };
  unsigned prevED = 0;  // It's really hard to name this var properly.

  // Initialization: how many edits are needed to convert the first ith
  // letter(s) of the second string to be the same as the first string
  // (the whole string).
  for (unsigned i = 1; i <= 5; i++) ed[i] = i;
  
  for (unsigned i = 1; i <= 5; i++) {
    prevED = ed[0];
    ed[0] = i;
    for (unsigned j = 1; j <= 5; j++) {
      unsigned lastRound = ed[j];
      if (s2[j - 1] == s1[i - 1]) {
        ed[j] = prevED;
      } else {
        ed[j] = min(prevED,     // replace the ith char of s1 to match s2's
                    min(ed[j],       // delete the ith char of s1
                        ed[j - 1]))  // insert a char at the end of s1
                    + 1;
                        
      }
      prevED = lastRound;
    }
  }

  return ed[5];
}
#endif

unsigned edit_distance(string &s1, string &s2) {
  // ed[i][j] means the edit distance bewtween the first i chars of s1
  // and first j chars of s2.
  unsigned ed[6][6] = { 0 };
  
  for (unsigned i = 1; i <= 5; i++) ed[i][0] = i;
  for (unsigned j = 1; j <= 5; j++) ed[0][j] = j;

  for (unsigned i = 1; i <= 5; i++) {
    for (unsigned j = 1; j <= 5; j++) {
      if (s1[j - 1] == s2[i - 1]) {
        ed[i][j] = ed[i - 1][j - 1];
      } else {
        ed[i][j] = 1 + min(ed[i - 1][j - 1],  // replace the ith char of s1 to be
                                          // the same as the jth char of s2
                           min(ed[i][j - 1], // append jth char of s2 to ith char of s1
                               ed[i - 1][j])); // delete the ith char of s1
      }
    }
  }

  return ed[5][5];
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cout << "Please input two strings.\n";
    return -1;
  }

  string s1 = string(argv[1]);
  string s2 = string(argv[2]);
  cout << "Edit Distance between " << s1 << " and " << s2 << " is ";
  cout << edit_distance(s1, s2) << "\n";

  return 0;
}
