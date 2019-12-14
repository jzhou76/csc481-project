/**
 * Prime numbers are important in cryptography. We define a new property
 * of a number: primeness. The primeness of a number is the number of
 * its prime factors. For example, 12345 = 3 * 5 * 823; so its primeness
 * is 3. We count the same prime factor accumulately; e.g., number 8's primeness
 * is 3. We say that a number A is better than number B if A's primeness
 * is less than B's.
 *
 * In this file, we implemented a function to securely compute which party's
 * input is a better number by the definition above. Now our implementation
 * only supports input number less than 1,000,000.
 * */

#include "emp-sh2pc/emp-sh2pc.h"
using namespace emp;
using namespace std;

/**
 * printPrimes(): Print out the prime numbers in an array.
 * */
void printPrimes(Integer primes[19]) {
  Integer ZERO(32, 0, PUBLIC);
  cout << "(" << primes[0].reveal<int>();

  for (int i = 1; i < 19; i++) {
    if (!primes[i].equal(ZERO).reveal<bool>()) 
      cout << " * " << primes[i].reveal<int>();
  }

  cout << ").\n";
}


/**
 * Function: betterNum()
 *
 * Description:
 *   This function computes which input number (less than 1,000,000)
 *   is a better number in terms of the number of prime factors.
 *   The algorithm of factorizing an input number is simple.
 *   Because the input is less than 1,000,000, there would be at most
 *   one prime facor over 1,000. So we iterate over all the prime numbers
 *   less than 1,000 and use modulo operation to find how many a certain
 *   prime number appears in the input's factorization.
 *
 *   See the code for more details.
 * */
void betterNum(int party, int num) {
  // Helper Integer.
  Integer ZERO(32, 0, PUBLIC);
  Integer ONE(32, 1, PUBLIC);

  // All the prime numbers less than 1,000.
  int Prime_Nums[168] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151,
    157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233,
    239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317,
    331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419,
    421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503,
    509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607,
    613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701,
    709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811,
    821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911,
    919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997
  };

  // The maximum exponent to which a prime is still less than 1,000,000.
  // For example, for Prime_Nums[2] = 5, Prime_Num_Expo[2] = 8 means
  // 5^8 < 1,000,000 and 5^9 > 1,000,000. This is used to fix the iterations
  // of computing how many a prime number is in the factorization of a number.
  // The reason we need code like this is because we cannot use a variable
  // as the termination condition of a loop.
  int Prime_Num_Expo[168] {
    19, 12, 8, 7, 5, 5, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
  };

  Integer Num_Alice(32, num, ALICE);
  Integer Num_Bob(32, num, BOB);
  cout << "Alice's input = " << Num_Alice.reveal<int>() << endl;
  cout << "Bob's input = " << Num_Bob.reveal<int>() << endl;

  Integer primes_Alice[19], primes_Bob[19];
  for (int i = 0; i < 19; i++) {
    // Initialization
    primes_Alice[i] = Integer(32, 0, ALICE);
    primes_Bob[i] = Integer(32, 0, BOB);
  }

  // Compute the prime factors of Alice's input.
  Integer Counter_Alice(32, 0, ALICE);
  for (int i = 0; i < 168; i++) {
    Integer Prime(32, Prime_Nums[i], PUBLIC);
    for (int j = 0; j < Prime_Num_Expo[i]; j++) {
      if ((Num_Alice % Prime).equal(ZERO).reveal<bool>()) {
        primes_Alice[Counter_Alice.reveal<int>()] = Prime;
        Counter_Alice = Counter_Alice + ONE;
        Num_Alice = Num_Alice / Prime;
      }
    }
  }
  if (!Num_Alice.equal(ONE).reveal<bool>()) Counter_Alice = Counter_Alice + ONE;

  // Compute the prime factors of Bob's input.
  Integer Counter_Bob(32, 0, BOB);
  for (int i = 0; i < 168; i++) {
    Integer Prime(32, Prime_Nums[i], PUBLIC);
    for (int j = 0; j < Prime_Num_Expo[i]; j++) {
      if ((Num_Bob % Prime).equal(ZERO).reveal<bool>()) {
        primes_Bob[Counter_Bob.reveal<int>()] = Prime;
        Counter_Bob = Counter_Bob + ONE;
        Num_Bob = Num_Bob / Prime;
      }
    }
  }
  if (!Num_Bob.equal(ONE).reveal<bool>()) Counter_Bob = Counter_Bob + ONE;

  // Print out the computation results.
  cout << "Alice's input can be factorized to " <<
    Counter_Alice.reveal<int>() << " prime ";
  printPrimes(primes_Alice);
  cout << "Bob's input can be factorized to " <<
    Counter_Bob.reveal<int>() << " prime ";
  printPrimes(primes_Bob);

  if (Counter_Alice.geq(Counter_Bob).reveal<bool>()) {
    cout << "Bob's input is the better number.\n";
  } else {
    cout << "Alice's input is the better number.\n";
  }
}


int main(int argc, char** argv) {
	int port, party;
	parse_party_and_port(argv, &party, &port);
	NetIO * io = new NetIO(party==ALICE ? nullptr : "127.0.0.1", port);

	setup_semi_honest(io, party);

    betterNum(party, atoi(argv[3]));

	delete io;
}
