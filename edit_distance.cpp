/**
 * Securely compute edit distance between two strings of length 5 held by
 * the two parties where each character is from { A,T,G,C}.
 *
 * */

#include "emp-sh2pc/emp-sh2pc.h"
#include <cmath>
using namespace emp;
using namespace std;

/**
 * printInput(): print out the input of a user.
 *
 * Input:
 *   s - an Integer array representing the input of a user.
 * */
void printInput(Integer s[5]) {
  for (unsigned i = 0; i < 5; i++) {
    switch(s[i].reveal<int>()) {
      case 1:
        cout << "A";
        break;
      case 2:
        cout << "T";
        break;
      case 3:
        cout << "G";
        break;
      case 4:
        cout << "C";
        break;
    }
  }
  cout << endl;
}


/**
 * editDistance(): securely computes the edit distance between two strings
 * whose sizes are both 5 and that consist of only 'A', 'T', 'G', and 'C'.
 * */
void editDistance(int party, string s) {
  // First, convert the string to an integer. We encode 'A', 'T', 'G', 'C'
  // to be 1, 2, 3, 4.  For example, ATTCG -> 12243.
  int s_int_arr[5], s_int = 0;
  for (unsigned i = 0; i < 5; i++) {
    switch (s[i]) {
      case 'A':
        s_int_arr[i] = 1;
        break;
      case 'T':
        s_int_arr[i] = 2;
        break;
      case 'G':
        s_int_arr[i] = 3;
        break;
      case 'C':
        s_int_arr[i] = 4;
        break;
    }
  }
  for (unsigned i = 0; i < 5; i++) {
    s_int += pow(10, (4 - i)) * s_int_arr[i];
  }

  
  // Create secure Integer for the inputs.
  Integer s1(32, s_int, ALICE);
  Integer s2(32, s_int, BOB);

  // Helper Integer.
  Integer ONE(32, 1, PUBLIC);
  Integer TEN(32, 10, PUBLIC);

  // Split the input Integer into 5 individual ones that represent the
  // 5 character because we need compare char by char (Integer by Integer here).
  Integer s1_Int_Arr[5];
  Integer s2_Int_Arr[5];
  for (int i = 4; i >= 0; i--) {
    s1_Int_Arr[i] = s1 % TEN;
    s1 = s1 / TEN;

    s2_Int_Arr[i] = s2 % TEN;
    s2 = s2 / TEN;
  }

  // ed[i][j] means the edit distance bewtween the first i chars of s1
  // and first j chars of s2.
  Integer ed[6][6];
  // Initialize the edit distance Integer; otherwise they would all
  // 0-length nullptr bits.
  for (unsigned i = 0; i < 6; i++) {
    for (unsigned j = 0; j < 6; j++) {
      ed[i][j] = Integer(32, 0, PUBLIC);
    }
  }
  
  for (unsigned i = 1; i <= 5; i++) ed[i][0] = Integer(32, i, PUBLIC);
  for (unsigned i = 1; i <= 5; i++) ed[0][i] = Integer(32, i, PUBLIC);

  for (unsigned i = 1; i <= 5; i++) {
    for (unsigned j = 1; j <= 5; j++) {
      // If s1[i] == s2[j], then ed[i][j] = ed[i - 1][j - 1]
      if (s1_Int_Arr[j - 1].equal(s2_Int_Arr[i - 1]).reveal<bool>()) {
        ed[i][j] = ed[i - 1][j - 1];
      } else {
        // We have three options:
        // 1. replacing the ith char of s1 to be the same as the jth char of s2,
        // 2. appending the jth char of s2 to the end of the ith char of s1.
        // 3. deleting the ith char of s1.

        // Compare option 2 with option 3.
        if (ed[i][j - 1].geq(ed[i - 1][j]).reveal<bool>()) {
          ed[i][j] = ed[i - 1][j] + ONE;
        } else {
          ed[i][j] = ed[i][j - 1] + ONE;
        }
        // Compare the result from the previous step with option 1.
        if (ed[i][j].geq(ed[i - 1][j - 1] + ONE).reveal<bool>()) {
          ed[i][j] = ed[i - 1][j - 1] + ONE;
        }
      }
    }
  }

  cout << "Alice's Input: ";
  printInput(s1_Int_Arr);
  cout << "Bob's Input: ";
  printInput(s2_Int_Arr);

  cout << "The edit distance is " << ed[5][5].reveal<int>() << endl;
}

int main(int argc, char** argv) {
  int port, party;
  parse_party_and_port(argv, &party, &port);
  NetIO * io = new NetIO(party==ALICE ? nullptr : "127.0.0.1", port);

  setup_semi_honest(io, party);

  if (argc != 4) {
    cout << "Please input a string.\n";
    return -1;
  }

  string s = string(argv[3]);
    // Check if the input is in the correct format: a 5-char string
    // with only 'A', 'T', 'G', 'C'
    if (s.size() != 5) {
      cout << "The string's size is not 5. ";
      cout << "Please input a correct string.\n";
      return -1;
    }
    for (char c : s) {
      if (c != 'A' && c != 'T' && c != 'G' && c != 'C') {
        cout << "Illegal character(s) in the string. ";
        cout << "Please input a correct string.\n";
        return -1;
      }
    }

    editDistance(party, s);

	delete io;
}
