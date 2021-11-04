#pragma once

#include <assert.h>

#include <vector>

/* A simple linear regression model for predicting the location of a given key
 * in a sorted array: y = m * x + b, where m is the slope and b is the
 * intercept. The key x can be any numeric type (e.g., integer or floating
 * point). The predicted position y is an integer.
 */
template <class K>
class LinearModel {
  static_assert(std::is_arithmetic<K>::value,
                "Linear model feature type must be numeric.");

 public:
  LinearModel() = default;

  // Train the model (i.e., set values for the slope m_ and the intercept b_)
  // to predict the positions from the keys. In other words, `keys` is a vector
  // of scalar model inputs, and `positions` is the vector of the corresponding
  // desired model outputs (e.g., for input `keys[0]`, the desired output is
  // `positions[0]`).
  void train(const std::vector<K>& keys, const std::vector<int>& positions) {
    assert(keys.size() == positions.size());

    /*** Insert your code here (roughly 25 lines of code) ***/
    // Train the model (i.e., set values for m_ and b_) to predict the
    // positions from the keys.
    // We suggest that you train the model using the closed-form formula
    // for minimizing mean squared error:
    // https://en.wikipedia.org/wiki/Ordinary_least_squares#Simple_linear_regression_model
    // You may need to be careful of cases where the denominators in the above
    // closed-form formulas are zero.
  }

  int predict(K key) const {
    return static_cast<int>(m_ * static_cast<double>(key) + b_);
  }

  K inverse_predict(int position) const {
    return static_cast<K>((static_cast<double>(position) - b_) / m_);
  }

  void rescale(double scaling_factor) {
    m_ *= scaling_factor;
    b_ *= scaling_factor;
  }

 private:
  double m_ = 0;  // slope
  double b_ = 0;  // intercept
};
