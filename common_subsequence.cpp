/**
 * Securely compute the longest common subsequence of two arrays
 * of characters with length INPUT_LENGTH, and return the length
 * of such common subsequence.
 * */

#include "emp-sh2pc/emp-sh2pc.h"
using namespace emp;
using namespace std;

#define INPUT_LENGTH 5

/**
 * printInput(): print out the input of a user.
 *
 * Input:
 *   s - an Integer array representing the input of a user.
 * */
void printInput(Integer s[INPUT_LENGTH]) {
    for (unsigned i = 0; i < INPUT_LENGTH; i++)
        cout << (char)s[i].reveal<int>();
    cout << endl;
}


/**
 * editDistance(): securely computes the edit distance between two strings
 * whose sizes are both 5 and that consist of only 'A', 'T', 'G', and 'C'.
 * */
void editDistance(int party, string s) {
    // int s_int_arr[INPUT_LENGTH];
    // for (unsigned i = 0; i < INPUT_LENGTH; i++)
    //     s_int_arr[i] = (int)s[i];
    // for (unsigned i = 0; i < 5; i++) {
    //     s_int += pow(10, (4 - i)) * s_int_arr[i];
    // }

  
    // Create secure Integer for the inputs.
    // Integer s1(32, s, ALICE);
    // Integer s2(32, s, BOB);
    // Helper Integer.
    Integer ONE(32, 1, PUBLIC);
    Integer TEN(32, 10, PUBLIC);

    // Split the input Integer into 5 individual ones that represent the
    // 5 character because we need compare char by char (Integer by Integer here).
    Integer s1_Int_Arr[INPUT_LENGTH];
    Integer s2_Int_Arr[INPUT_LENGTH];

    cout << "Checkpoint 1" << endl;
    for(unsigned i = 0; i < INPUT_LENGTH; i++) {
        s1_Int_Arr[i] = Integer(32, s[i], ALICE);
        s2_Int_Arr[i] = Integer(32, s[i], BOB);
    }

  
    cout << "Checkpoint 2" << endl;
    // ed[i][j] means the edit distance bewtween the first i chars of s1
    // and first j chars of s2.
    Integer ed[INPUT_LENGTH + 1][INPUT_LENGTH + 1];
    // Initialize first row and first column to be 0
    for (unsigned i = 0; i <= INPUT_LENGTH; i++) {
        for (unsigned j = 0; j <= INPUT_LENGTH; j++) {
            ed[i][j] = Integer(32, 0, PUBLIC);
        }
    }

    for (unsigned i = 1; i <= INPUT_LENGTH; i++) {
        for (unsigned j = 1; j <= INPUT_LENGTH; j++) {
            if (s1_Int_Arr[j - 1].equal(s2_Int_Arr[i - 1]).reveal<bool>()) {
                ed[i][j] = ed[i - 1][j - 1] + ONE;
            } else {
                if (ed[i - 1][j].geq(ed[i][j - 1]).reveal<bool>())
                    ed[i][j] = ed[i - 1][j];
                else
                    ed[i][j] = ed[i][j - 1];
            }
        }
    }
  
    cout << "Alice's Input: ";
    printInput(s1_Int_Arr);
    cout << "Bob's Input: ";
    printInput(s2_Int_Arr);

    cout << "The length of longest common subsequence is " <<
        ed[INPUT_LENGTH][INPUT_LENGTH].reveal<int>() << endl;
}

int main(int argc, char** argv) {
    int port, party;
    parse_party_and_port(argv, &party, &port);
    NetIO * io = new NetIO(party==ALICE ? nullptr : "127.0.0.1", port);

    setup_semi_honest(io, party);

    if (argc != 4) {
        cout << "Usage: ./common_subsequence 0/1 PORT INPUT";
        return -1;
    }

    string s = string(argv[3]);
    // Check if the input has the correct length
    if (s.size() != INPUT_LENGTH) {
        cout << "Error. The string's size is not " << INPUT_LENGTH << ".";
        return -1;
    }
    editDistance(party, s);

    delete io;
}

