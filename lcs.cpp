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
 * lcs(): securely computes the length of longest common sequence
 * of two arrays of chars
 * */
void lcs(int party, string s) {
    // Helper Integer.
    Integer ONE(32, 1, PUBLIC);

    Integer s1_Int_Arr[INPUT_LENGTH];
    Integer s2_Int_Arr[INPUT_LENGTH];

    // Split the input string to corresponding party
    for(unsigned i = 0; i < INPUT_LENGTH; i++) {
        s1_Int_Arr[i] = Integer(32, s[i], ALICE);
        s2_Int_Arr[i] = Integer(32, s[i], BOB);
    }

  
    // ed[i][j] means the length of least common subsequence between the first
    // i chars of s1 and first j chars of s2.
    Integer ed[INPUT_LENGTH + 1][INPUT_LENGTH + 1];
    // Initialize the table elements to be 0
    for (unsigned i = 0; i <= INPUT_LENGTH; i++) {
        for (unsigned j = 0; j <= INPUT_LENGTH; j++) {
            ed[i][j] = Integer(32, 0, PUBLIC);
        }
    }

    for (unsigned i = 1; i <= INPUT_LENGTH; i++) {
        for (unsigned j = 1; j <= INPUT_LENGTH; j++) {
            // if (s1_Int_Arr[i - 1].equal(s2_Int_Arr[j - 1]).reveal<bool>()) {
            //     ed[i][j] = ed[i - 1][j - 1] + ONE;
            // } else {
            //     if (ed[i - 1][j].geq(ed[i][j - 1]).reveal<bool>())
            //         ed[i][j] = ed[i - 1][j];
            //     else
            //         ed[i][j] = ed[i][j - 1];
            // }
            ed[i][j] = ed[i][j - 1].select(ed[i - 1][j].geq(ed[i][j - 1]),
                                                            ed[i - 1][j]);
            ed[i][j] = ed[i][j].select(s1_Int_Arr[i - 1].equal(s2_Int_Arr[j - 1]),
                                       ed[i - 1][j - 1] + ONE);
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
    lcs(party, s);

    delete io;
}

