#include <iostream>

#include "learned_index.h"

int main(int, char**) {
  // Generate data consisting of 1000 key-value records.
  // Keys are floating point numbers; values are integers.
  std::vector<std::pair<double, int>> data;
  for (int i = 1; i <= 1000; i++) {
    data.emplace_back(static_cast<double>(std::log(i)), i);
  }

  // Build a learned index with 10 second-level models.
  LearnedIndex<double, int> learned_index(data);
  learned_index.build(10);

  // For each key in the data, look up its value using the learned index.
  for (const auto& record : data) {
    double key = record.first;
    int expected_value = record.second;
    const int* found_value = learned_index.get_value(key);

    if (found_value == nullptr) {
      std::cout << "Error: did not find value for key " << key << std::endl;
    } else if (*found_value != expected_value) {
      std::cout << "Error: incorrect value found for key " << key
                << " (expected " << expected_value << ", found " << *found_value
                << ")" << std::endl;
    }
  }

  // Verify that the learned index returns nothing for non-existent keys.
  std::vector<double> nonexistent_keys = {-1., data.back().first + 1.};
  for (double key : nonexistent_keys) {
    const int* found_value = learned_index.get_value(key);
    if (found_value != nullptr) {
      std::cout << "Error: found value for key " << key
                << ", which does not exist" << std::endl;
    }
  }
}