#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>

#define K double
#define V int64_t

int main(int, char**) {
  std::string keys_file_path = "keys.bin";
  int num_records = 50000000;

  // Read keys from file. Keys are in random order (not sorted).
  std::vector<K> keys(num_records);
  std::ifstream is(keys_file_path.c_str(), std::ios::binary | std::ios::in);
  if (!is.is_open()) {
    std::cout << "Run `sh download.sh` to download the keys file" << std::endl;
    return 0;
  }
  is.read(reinterpret_cast<char*>(keys.data()),
          std::streamsize(num_records * sizeof(K)));
  is.close();

  // Combine loaded keys with randomly generated values
  std::vector<std::pair<K, V>> data(num_records);
  std::mt19937_64 gen_payload(std::random_device{}());
  for (int i = 0; i < num_records; i++) {
    data[i].first = keys[i];
    data[i].second = static_cast<V>(gen_payload());
  }

  // Sort data
  std::cout << "Sorting data..." << std::endl;
  std::sort(data.begin(), data.end());

  // Run workload using binary search
  std::cout << "Running query workload..." << std::endl;
  auto workload_start_time = std::chrono::high_resolution_clock::now();
  V sum = 0;
  for (K key : keys) {
    auto it = std::lower_bound(
        data.begin(), data.end(), key,
        [](auto const& pair, K key) { return pair.first < key; });
    sum += it->second;
  }
  double workload_time =
      std::chrono::duration_cast<std::chrono::nanoseconds>(
          std::chrono::high_resolution_clock::now() - workload_start_time)
          .count();

  std::cout << "Workload complete. Binary search workload time: "
            << workload_time / 1e9 << " seconds, proof of work: " << sum
            << std::endl;
}