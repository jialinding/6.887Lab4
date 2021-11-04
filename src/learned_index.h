#pragma once

#include <assert.h>

#include <iostream>
#include <numeric>
#include <vector>

#include "linear_model.h"

template <class K, class V>
class LearnedIndex {
  static_assert(std::is_arithmetic<K>::value,
                "Learned index key type must be numeric.");

 public:
  typedef std::pair<K, V> record;

  LearnedIndex(std::vector<record> data) : data_(data) {
    std::sort(data_.begin(), data_.end());
  }

  // Build a two-level RMI that only uses linear regression models, with the
  // specified number of second-level models.
  void build(int num_second_level_models) {
    assert(num_second_level_models > 0);
    second_level_models_.clear();

    // Construct the root model over the entire data.
    // Extract keys from key-value records. In practice, you would want to avoid
    // this because it requires making a redundant temporary copy of all the
    // keys, but for simplicity in this lab we will make some redundant copies.
    std::vector<K> keys;
    std::transform(std::begin(data_), std::end(data_), std::back_inserter(keys),
                   [](auto const& pair) { return pair.first; });
    // Build a vector of positions (i.e., indexes) for each key.
    // For the root node over n records, these are simply the integers 0 through
    // n-1.
    std::vector<int> positions(keys.size());
    std::iota(std::begin(positions), std::end(positions), 0);
    root_model_.train(keys, positions);
    // Rescale the root model so that instead of predicting a position, it
    // predicts the index for the second-level model. Feeding a key through
    // the root model will output the index of the second-level model to which
    // the key should be assigned (root model outputs may need to be manually
    // bounded between 0 and num_second_level_models-1).
    root_model_.rescale(static_cast<double>(num_second_level_models) /
                        keys.size());

    /*** Insert your code here (roughly 35 lines of code) ***/
    // Use the trained root model to assign records to each of the second-level
    // models. Then train the second-level models to predict the positions for
    // each of their assigned records and compute the maximum prediction error
    // for each second-level model. Unlike the paper, in which each model
    // stores both a min-error (i.e., a left-error) and a max-error (i.e., a
    // right error), here we will only store a single maximum bi-directional
    // error for each second-level model.
  }

  // If the key exists, return a pointer to the corresponding value in data_.
  // If the key does not exist, return a nullptr.
  const V* get_value(K key) const {
    assert(second_level_models_.size() > 0);
    int root_model_output = root_model_.predict(key);

    /*** Insert your code here (roughly 20 lines of code) ***/
    // Use the root model's output to select a second-level model, then use
    // the second-level model to predict the key's position, then do a
    // last-mile search using the model's error bound to find the true position
    // of the key. If the key exists, return a pointer to the value. If the
    // key does not exist, return a nullptr.
    // NOTE: to receive full credit, the last-mile search should use the
    // `last_mile_search` method provided below.
  }

 private:
  // Do a binary search for the position of a key in the data.
  // Only search in the range between the given start position (inclusive)
  // and end position (exclusive).
  // If the key is not found in the data, return -1.
  int last_mile_search(K key, int start_pos, int end_pos) const {
    int pos = std::lower_bound(
                  data_.begin() + start_pos, data_.begin() + end_pos, key,
                  [](auto const& pair, K key) { return pair.first < key; }) -
              data_.begin();
    if (pos > static_cast<int>(data_.size()) || data_[pos].first != key) {
      return -1;
    } else {
      return pos;
    }
  }

  std::vector<record> data_;
  LinearModel<K> root_model_;
  std::vector<LinearModel<K>> second_level_models_;
  // The maximum prediction error for each second-level model.
  std::vector<int> second_level_error_bounds_;
};
