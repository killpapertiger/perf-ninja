#include "solution.hpp"

static int getSumOfDigits(int n) {
  int sum = 0;
  while (n != 0) {
    sum = sum + n % 10;
    n = n / 10;
  }
  return sum;
}

int solution(const hash_map_t *hash_map, const std::vector<int> &lookups) {

/*
  int result = 0;


  for (int val : lookups) {
    if (hash_map->find(val))
      result += getSumOfDigits(val);
  }

  return result;
*/

/*
   int result;
   size_t prefetch_distance = 5;
   int i = 0;
   std::vector<int> prefetch_vec.reserve(prefetch_distance);
   for (int val: lookups) {
     // prefetch应该是prefetch的地址，而不是find函数
     int value =  _builtin_prefetch(hash_map->find(val));
     prefech_distance++;
     prefetch.push_back(value);
     if (i >= prefech_distance) {
         break;
     }
   }
   for (int i = prefetch_distance; i < lookups.size(); i++) {
       result += getSumOfDigits(prefetch_vec[i-prefetch_distance);
       prefech_vec[i % prefetch_distance] = hash_map->find(val);
   }
*/
   // prolog + epilog,其实还可以有prolog把最前面几个元素单独写循环提出出来
   int result = 0;
   int lookahead = 5;
   for (int i = 0; i < lookups.size() - lookahead ; i++) {
    if (hash_map->find(lookups[i])) {
        result += getSumOfDigits(lookups[i]);
    }
    hash_map->prefetch(lookups[i+lookahead]);
   }
   for (int i = lookups.size() - lookahead; i < lookups.size(); i++) {
      if (hash_map->find(lookups[i])) {
          result += getSumOfDigits(lookups[i]);
      }
   }
   return result;


}
