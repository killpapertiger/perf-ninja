#include "solution.hpp"
#include <array>
#include <iostream>

unsigned getSumOfDigits(unsigned n) {
  unsigned sum = 0;
  while (n != 0) {
    sum = sum + n % 10;
    n = n / 10;
  }
  return sum;
}

// Task: lookup all the values from l2 in l1.
// For every found value, find the sum of its digits.
// Return the sum of all digits in every found number.
// Both lists have no duplicates and elements placed in *random* order.
// Do NOT sort any of the lists. Do NOT store elements in a hash_map/sets.

// Hint: Traversing a linked list is a long data dependency chain:
//       to get the node N+1 you need to retrieve the node N first.
//       Think how you can execute multiple dependency chains in parallel.
unsigned solution(List *l1, List *l2) {
/*
  unsigned retVal = 0;

  List *head2 = l2;
  // O(N^2) algorithm:
  while (l1) {
    unsigned v = l1->value;
    l2 = head2;
    while (l2) {
      if (l2->value == v) {
        retVal += getSumOfDigits(v);
        break;
      }
      l2 = l2->next;
    }
    l1 = l1->next;
  }

  return retVal;
*/
/*
  unsigned retVal = 0;
  List *head2 = l2;
  size_t N = 5;  // 可以用模板参数
  std::vector<unsigned> vec.reserve(N);
  while (l1) {
      int i = 0;
      while (l1 && i < N) {
          unsigned v = l1->value;
          vec.push_back(v);
          i++;
          l1 = l1->next;
      }
      bool mask[N] = {false};  // diverge  // 正确答案里也没有考虑进一步优化通道利用率低的问题，我们也先不考虑
      while(l2) {
          int i = 0;
          while (!mask[i]) {
              if (l2->value == vec[i]) {
                retVal += getSumOfDigits(v);
                i++;
                mask[i] = true;
              }
          }
          
      }
  }
  */
  unsigned retVal = 0;
  List *head2 = l2;
  size_t N = 5;
  std::vector<unsigned> vec;
  vec.reserve(N);
  while(l1) {
    int i = 0;
    int NN = 0;
    vec.clear();
    while(l1 && i < N) {
        unsigned v = l1->value;
        vec.push_back(v);
        i++;
        NN++;
        l1 = l1->next;
    }
    l2 = head2;
    while(l2) {
      int i = 0;
      while(i < NN) {
          if (l2->value == vec[i]) {
              retVal += getSumOfDigits(vec[i]);
          }
          i++;
      }
      l2 = l2->next;
    }
    // l1 = l1->next;
  }
  return retVal; 

}
