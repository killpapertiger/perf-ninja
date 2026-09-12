#include "solution.hpp"
#include <algorithm>

/*
bool solution(MatrixOfDoubles &in, MatrixOfDoubles &out) {
  int size = in.size();
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      out[i][j] = in[j][i];
    }
  }
  return out[0][size - 1];
}
*/

bool solution(MatrixOfDoubles &in, MatrixOfDoubles &out) {
    int size = in.size();
    int tile = 16;   // 16 * 16 * 8 = 2kb 
    //用的都是这种“逻辑分块 + 改变循环顺序”的方式，而不是真正把数据切成独立的小矩阵。
    for (int i = 0; i < size; i += tile) {
        for (int j = 0; j < size; j += tile) {

            for (int ii = i; ii < std::min(i + tile,size); ii++) {
                for (int jj = j; jj < std::min(j + tile,size); jj++) {
                    out[ii][jj] = in[jj][ii];
                }
            }
        } 
    }
    return out[0][size - 1];
}
