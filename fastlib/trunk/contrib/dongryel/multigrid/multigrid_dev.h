/** @file multigrid_dev.h
 *  @brief An implementation of multigrid algorithm for solving linear systems.
 *
 *  @author Dongryeol Lee (dongryel@cc.gatech.edu)
 */

#ifndef MLPACK_MULTIGRID_MULTIGRID_DEV_H
#define MLPACK_MULTIGRID_MULTIGRID_DEV_H

#include <algorithm>
#include "multigrid.h"

namespace fl {
namespace ml {

template<typename MatrixType, typename VectorType>
Multigrid<MatrixType, VectorType>::Multigrid() {
  left_hand_side_ = NULL;
  right_hand_side_ = NULL;
}

template<typename MatrixType, typename VectorType>
Multigrid<MatrixType, VectorType>::~Multigrid() {
  for (unsigned int i = 0; i < levels_.size(); i++) {
    delete levels_[i];
  }
}

template<typename MatrixType, typename VectorType>
void Multigrid<MatrixType, VectorType>::Coarsen_(
  const MultigridLevel &level_in,
  MultigridLevel *coarsened_level_out) {

  const double threshold = 0.2;

  // Make a copy of the fine nodes from the points owned by the
  // previous level.
  const std::vector<int> &fine_point_indices = level_in.point_indices();
  std::vector<int> shuffle_indices(fine_point_indices.size());
  for (unsigned int i = 0; i < shuffle_indices.size(); i++) {
    shuffle_indices[i] = i;
  }
  std::random_shuffle(shuffle_indices.begin(), shuffle_indices.end());

  // The generated coarse points. The first component of the pair is
  // the index of the coarse point (i.e. the physical position in the
  // matrix). The second component is the real label of the coarse
  // point.
  std::vector< std::pair<int, int> > coarse_point_indices;

  for (unsigned int i = 0; i < fine_point_indices.size(); i++) {

    // The index of the fine node point.
    int fine_point_index = shuffle_indices[i];

    // The associated label of the fine node point.
    int fine_point_label = fine_point_indices[ fine_point_index ];

    // Compute the sum of the affinities between the current fine node
    // point and the existing set of coarse points.
    double sum_coarse_affinities = 0;
    for (unsigned int j = 0; j < coarse_point_indices.size(); j++) {

      // The physical index of the coarse node point.
      int coarse_point_index = coarse_point_indices[j].first;
      sum_coarse_affinities +=
        fabs(level_in.get(fine_point_index, coarse_point_index));
    }

    // Compute the sum of the affinities between the current fine node
    // and all of the points.
    double sum_all_affinities = 0;
    for (unsigned int j = 0; j < fine_point_indices.size(); j++) {
      sum_all_affinities += fabs(level_in.get(fine_point_index, j));
    }

    printf("Sum coarse: %g, sum all: %g\n", sum_coarse_affinities,
           sum_all_affinities);
    // Add to the coarse set if the following condition is satisfied.
    if (sum_coarse_affinities < threshold * sum_all_affinities) {
      coarse_point_indices.push_back(
        std::pair<int, int>(fine_point_index, fine_point_label));
    }
  } // end of looping over all fine nodes.

  // Sort the coarse point indices, which will sort by the physical index
  // of each point.
  std::sort(coarse_point_indices.begin(), coarse_point_indices.end());
  coarsened_level_out->set_point_indices(coarse_point_indices);

  printf("Fine point indices:\n");
  for (unsigned int i = 0; i < fine_point_indices.size(); i++) {
    printf("%d ", fine_point_indices[i]);
  }
  printf("\nCoarse point indices:\n");
  for (unsigned int i = 0; i < coarse_point_indices.size(); i++) {
    printf("%d ", coarse_point_indices[i].second);
  }

  // Build the interpolation matrix.
  coarsened_level_out->Build(level_in, coarse_point_indices);
}

template<typename MatrixType, typename VectorType>
void Multigrid<MatrixType, VectorType>::Init(
  MatrixType &left_hand_side_in,
  VectorType &right_hand_side_in,
  int level_threshold_in,
  int max_num_iterations_in) {

  // Set the incoming variables.
  left_hand_side_ = &left_hand_side_in;
  right_hand_side_ = &right_hand_side_in;
  level_threshold_ = level_threshold_in;
  max_num_iterations_ = max_num_iterations_in;

  // Generate the coarse problems.
  MultigridLevel root_level(left_hand_side_in, right_hand_side_in);
  MultigridLevel *previous_level = &root_level;

  // Start coarsening.
  printf("Starting with %d\n", previous_level->num_points());
  while (previous_level->num_points() > level_threshold_) {
    printf("Creating a new level!\n");
    levels_.push_back(new MultigridLevel());

    // The next level to be generated.
    MultigridLevel &next_level = *(levels_[ levels_.size() - 1 ]);
    Coarsen_(*previous_level, &next_level);

    // Change the previous level pointer.
    previous_level = &next_level;
    printf("Created a new level with %d points\n", next_level.num_points());
    next_level.left_hand_side().PrintDebug();
  }

  printf("Created %d levels\n", levels_.size());
}

template<typename MatrixType, typename VectorType>
void Multigrid<MatrixType, VectorType>::Compute(Vector *output) {

  // Allocate space for the output vector.
  output->Init(right_hand_side_->length());
  output->SetZero();


}
};
};

#endif
