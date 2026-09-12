#include "solution.hpp"
#include <algorithm>
#include <cassert>
#include <type_traits>
/*
// The alignment algorithm which computes the alignment of the given sequence
// pairs.
result_t compute_alignment(std::vector<sequence_t> const &sequences1,
                           std::vector<sequence_t> const &sequences2) {
  result_t result{};

  for (size_t sequence_idx = 0; sequence_idx < sequences1.size();
       ++sequence_idx) {
    using score_t = int16_t;
    using column_t = std::array<score_t, sequence_size_v + 1>;

    sequence_t const &sequence1 = sequences1[sequence_idx];
    sequence_t const &sequence2 = sequences2[sequence_idx];

    /*
     * Initialise score values.
     * /
    score_t gap_open{-11};
    score_t gap_extension{-1};
    score_t match{6};
    score_t mismatch{-4};

    /*
     * Setup the matrix.
     * Note we can compute the entire matrix with just one column in memory,
     * since we are only interested in the last value of the last column in the
     * score matrix.
     * /
    column_t score_column{};
    column_t horizontal_gap_column{};
    score_t last_vertical_gap{};

    /*
     * Initialise the first column of the matrix.
     * /
    horizontal_gap_column[0] = gap_open;
    last_vertical_gap = gap_open;

    for (size_t i = 1; i < score_column.size(); ++i) {
      score_column[i] = last_vertical_gap;
      horizontal_gap_column[i] = last_vertical_gap + gap_open;
      last_vertical_gap += gap_extension;
    }

    /*
     * Compute the main recursion to fill the matrix.
     * /
    for (unsigned col = 1; col <= sequence2.size(); ++col) {
      score_t last_diagonal_score =
          score_column[0]; // Cache last diagonal score to compute this cell.
      score_column[0] = horizontal_gap_column[0];
      last_vertical_gap = horizontal_gap_column[0] + gap_open;
      horizontal_gap_column[0] += gap_extension;

      for (unsigned row = 1; row <= sequence1.size(); ++row) {
        // Compute next score from diagonal direction with match/mismatch.
        score_t best_cell_score =
            last_diagonal_score +
            (sequence1[row - 1] == sequence2[col - 1] ? match : mismatch);
        // Determine best score from diagonal, vertical, or horizontal
        // direction.
        best_cell_score = std::max(best_cell_score, last_vertical_gap);
        best_cell_score = std::max(best_cell_score, horizontal_gap_column[row]);
        // Cache next diagonal value and store optimum in score_column.
        last_diagonal_score = score_column[row];
        score_column[row] = best_cell_score;
        // Compute the next values for vertical and horizontal gap.
        best_cell_score += gap_open;
        last_vertical_gap += gap_extension;
        horizontal_gap_column[row] += gap_extension;
        // Store optimum between gap open and gap extension.
        last_vertical_gap = std::max(last_vertical_gap, best_cell_score);
        horizontal_gap_column[row] =
            std::max(horizontal_gap_column[row], best_cell_score);
      }
    }

    // Report the best score.
    result[sequence_idx] = score_column.back();
  }

  return result;
}
*/




using  simd_score_t = std::array<uint16_t, sequence_count_v>;
using simd_sequence_t = std::array<simd_score_t, sequence_size_v>;
simd_sequence_t transpose(std::vector<sequence_t> const &sequences) {
    simd_sequence_t simd_sequences;
    for (int i = 0; i < simd_sequences.size(); i++) {
        for (int j = 0; j < sequences.size(); j++) {
            simd_sequences[i][j] = sequences[j][i];
        }
    }
    return simd_sequences;
}

// The alignment algorithm which computes the alignment of the given sequence
// pairs.
result_t compute_alignment(std::vector<sequence_t> const &sequences1,
                           std::vector<sequence_t> const &sequences2) {


  simd_sequence_t trans_sequences1 = transpose(sequences1);
  simd_sequence_t trans_sequences2 = transpose(sequences2);
  result_t result{};

  // for (size_t sequence_idx = 0; sequence_idx < sequences1.size();
  //     ++sequence_idx) {
    // using score_t = int16_t;
    // using column_t = std::array<score_t, sequence_size_v + 1>;
    
    // sequence_t const &sequence1 = sequences1[sequence_idx];
    // sequence_t const &sequence2 = sequences2[sequence_idx];
    using score_t = std::array<int16_t, sequence_count_v>;
    using column_t = std::array<score_t, sequence_size_v + 1>;

    /*
     * Initialise score values.
     */
    // score_t gap_open{-11};
    // score_t gap_extension{-1};
    // score_t match{6};
    // score_t mismatch{-4};
    score_t gap_open{};
    gap_open.fill(-11);
    score_t gap_extension{};
    gap_extension.fill(-1);
    score_t match{};
    match.fill(6);
    score_t mismatch{};
    mismatch.fill(-4);



    /*
     * Setup the matrix.
     * Note we can compute the entire matrix with just one column in memory,
     * since we are only interested in the last value of the last column in the
     * score matrix.
     */
    column_t score_column{};
    column_t horizontal_gap_column{};
    score_t last_vertical_gap{};

    /*
     * Initialise the first column of the matrix.
     */
    // horizontal_gap_column[0] = gap_open;
    // last_vertical_gap = gap_open;
    for (int i = 0; i < sequence_count_v; i++) {
        horizontal_gap_column[0][i] = gap_open[i];
        last_vertical_gap[i] = gap_open[i];
    }

    // for (size_t i = 1; i < score_column.size(); ++i) {
    //  score_column[i] = last_vertical_gap;
    //  horizontal_gap_column[i] = last_vertical_gap + gap_open;
    //  last_vertical_gap += gap_extension;
    // }
    for (size_t i = 1; i < score_column.size(); ++i) {
        for (int j = 0; j < sequence_count_v; j++) {
            score_column[i][j] = last_vertical_gap[j];
            horizontal_gap_column[i][j]=last_vertical_gap[j] +gap_open[j];
            last_vertical_gap[j] += gap_extension[j];
        }
    }

    /*
     * Compute the main recursion to fill the matrix.
     */
    for (unsigned col = 1; col <= trans_sequences2.size(); ++col) {
      // score_t last_diagonal_score =
      //    score_column[0]; // Cache last diagonal score to compute this cell.
      // score_column[0] = horizontal_gap_column[0];
      // last_vertical_gap = horizontal_gap_column[0] + gap_open;
      // horizontal_gap_column[0] += gap_extension;


      score_t last_diagonal_score;
      for (int i = 0; i < sequence_count_v; i++) {
          last_diagonal_score[i] =
              score_column[0][i]; // Cache last diagonal score to compute this cell.
          score_column[0][i] = horizontal_gap_column[0][i];
          last_vertical_gap[i] = horizontal_gap_column[0][i] + gap_open[i];
          horizontal_gap_column[0][i] += gap_extension[i];
      }

      for (unsigned row = 1; row <= trans_sequences1.size(); ++row) {
          score_t best_cell_score;
          for (int i = 0; i < sequence_count_v; i++) {
           // Compute next score from diagonal direction with match/mismatch.
            best_cell_score[i] =
                last_diagonal_score[i] +
                (trans_sequences1[row - 1][i] == trans_sequences2[col - 1][i]? match[i] : mismatch[i]);
            // Determine best score from diagonal, vertical, or horizontal
            // direction.
            best_cell_score[i] = std::max(best_cell_score[i], last_vertical_gap[i]);
            best_cell_score[i] = std::max(best_cell_score[i], horizontal_gap_column[row][i]);
            // Cache next diagonal value and store optimum in score_column.
            last_diagonal_score[i] = score_column[row][i];
            score_column[row][i] = best_cell_score[i];
            // Compute the next values for vertical and horizontal gap.
            best_cell_score[i] += gap_open[i];
            last_vertical_gap[i] += gap_extension[i];
            horizontal_gap_column[row][i] += gap_extension[i];
            // Store optimum between gap open and gap extension.
            last_vertical_gap[i] = std::max(last_vertical_gap[i], best_cell_score[i]);
            horizontal_gap_column[row][i] =
                std::max(horizontal_gap_column[row][i], best_cell_score[i]);
         }  // for sequence_count_v
      }  // for trans_sequences1.size()
    }  // for trans_sequences2.size()

    // Report the best score.
    // result[sequence_idx] = score_column.back();
  // }
    for (int i = 0; i < sequence_count_v; i++) {
        result[i] = score_column.back()[i];
    }

  return result;
}
