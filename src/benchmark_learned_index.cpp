#include <algorithm>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>

#include "learned_index.h"

#define K double
#define V int64_t

int main(int argc, char** argv) {
  /*** Your task: change the default from 100 to a tuned number. ***/
  int num_second_level_models = 100;
  if (argc > 1) {
    num_second_level_models = atoi(argv[1]);
  }

  std::string keys_file_path = "keys.bin";
  int num_records = 50000000;

  // Read keys from file. Keys are in random order (not sorted).
  auto keys = new K[num_records];
  std::ifstream is(keys_file_path.c_str(), std::ios::binary | std::ios::in);
  if (!is.is_open()) {
    std::cout << "Run `sh download.sh` to download the keys file" << std::endl;
    return 0;
  }
  is.read(reinterpret_cast<char*>(keys),
          std::streamsize(num_records * sizeof(K)));
  is.close();

  // Combine loaded keys with randomly generated values
  std::vector<std::pair<K, V>> data(num_records);
  std::mt19937_64 gen_payload(std::random_device{}());
  for (int i = 0; i < num_records; i++) {
    data[i].first = keys[i];
    data[i].second = static_cast<V>(gen_payload());
  }
  delete[] keys;

  // Build index index
  std::cout << "Building learned index with " << num_second_level_models
            << " second level models..." << std::endl;
  LearnedIndex<K, V> index(data);
  auto build_start_time = std::chrono::high_resolution_clock::now();
  index.build(num_second_level_models);
  double build_time =
      std::chrono::duration_cast<std::chrono::nanoseconds>(
          std::chrono::high_resolution_clock::now() - build_start_time)
          .count();

  // Run workload using learned index
  std::cout << "Running query workload..." << std::endl;
  auto workload_start_time = std::chrono::high_resolution_clock::now();
  V sum = 0;
  for (const auto& record : data) {
    K key = record.first;
    const V* payload = index.get_value(key);
    if (payload) {
      sum += *payload;
    }
  }
  double workload_time =
      std::chrono::duration_cast<std::chrono::nanoseconds>(
          std::chrono::high_resolution_clock::now() - workload_start_time)
          .count();

  std::cout << "Workload complete. Learned index build time: "
            << build_time / 1e9
            << " seconds, workload time: " << workload_time / 1e9
            << " seconds, proof of work: " << sum << std::endl;
}