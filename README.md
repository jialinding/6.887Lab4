# 6.887 Machine Learning for Systems
## Lab 3: Learned Indexes

###### Due: Monday, Nov 8 at 12:00 PM

In this lab, you will implement a simple version of the Recursive Model Index (RMI), which was the original learned index proposed in the paper [The Case for Learned Index Structures](https://arxiv.org/pdf/1712.01208.pdf), which you read for class.

### Instructions:

### Step 1
Download cmake, at least version 3.12:
- If on Mac, use `brew install cmake` or follow these instructions: https://cmake.org/install/
- If on Linux, use `sudo apt-get install cmake` or follow these instructions: https://cmake.org/install/
- If on Windows, follow these instructions: https://cmake.org/install/

### Step 2

You will be using C++ to implement a two-level RMI that only uses linear regression models.
An outline of the code is already provided in `src/learned_index.h` and `src/linear_model.h`.
Your task is to complete the code in the three locations marked with "Insert your code here" in those two files.
You should not need to create any new files, though you are free to do so if you want.
You are also free to modify code outside the marked locations, but you should not need to.

Each code block is marked with the approximate number of lines of code that were used in the reference solution.
This is just a rough ballpark, and it is perfectly OK if you use more lines of code.

In your implementation, you can assume that the learned index only stores unique keys.

### Step 3
To compile the code, run `sh build.sh`. This will compile the executables to the `build` directory.

We provide three executables:
- Running `./build/sanity_check` (source code in `src/sanity_check.cpp`) will perform a sanity check of the learned index on a simple synthetic dataset. You can use this during implementation to make sure things are working correctly.
- For the other executables, you will first need to run `sh download.sh` to download a 400MB file named `keys.bin`, which contains 50M floating-point keys.
Then you can run `./build/benchmark_learned_index` to benchmark a lookup workload over those 50M keys using the learned index that you implemented.
You can also run `./build/benchmark_binary_search` to benchmark the same workload using binary search instead of the learned index.

You should make sure that all executables run without any error messages. `./build/benchmark_binary_search` should run without any changes, but the other two executables will not run until you finish step 2.


### Step 4
In `src/benchmark_learned_index.cpp`, the default number of second-level models is set to 100.
You may notice that when using this default, the workload completion time using the learned index (as measured by running `./build/benchmark_learned_index`) is around the same as the workload completion time using binary search (as measured by running `./build/benchmark_binary_search`).
Your task is to tune the default to the number that minimizes workload completion time.
To assist you with trying different numbers, you can run the executable with an argument (e.g., `./build/benchmark_learned_index 1000` to try out 1000 second-level models) instead of re-compiling.

### Step 5
Create a zip file with the files `src/benchmark_learned_index.cpp`, `src/learned_index.h`, and `src/linear_model.h`, and upload to Canvas.
You shouldn't need to create any new files other than the ones already provided, but if you do, include the new files in the submission as well.
