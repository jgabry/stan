#include <stan/prob/distributions/univariate/continuous/inv_gamma.hpp>
#include <gtest/gtest.h>
#include <boost/random/mersenne_twister.hpp>
#include<boost/math/distributions.hpp>

TEST(ProbDistributionsInvGamma, random) {
  boost::random::mt19937 rng;
  EXPECT_NO_THROW(stan::prob::inv_gamma_rng(4.0,3.0,rng));
}

TEST(ProbDistributionsInvGamma, doesFit) {
  boost::random::mt19937 rng;
  int N = 10000;
  int K = 5;
  boost::math::inverse_gamma_distribution<>dist (4.0,3.0);
  boost::math::chi_squared mydist(K-1);

  double loc[4];
  for(int i = 1; i < K; i++)
    loc[i - 1] = quantile(dist, 0.2 * i);

  int count = 0;
  int bin [5] = {0, 0, 0, 0, 0};

  while (count < N) {
    double a = stan::prob::inv_gamma_rng(4.0,3.0,rng);
    int i = 0;
    while (i < K-1 && a > loc[i]) 
	++i;
    ++bin[i];
    count++;
   }

  double chi = 0;
  double expect [5] = {N / K, N / K, N / K, N / K, N / K};

  for(int j = 0; j < K; j++)
    chi += ((bin[j] - expect[j]) * (bin[j] - expect[j]) / expect[j]);

  EXPECT_TRUE(chi < quantile(complement(mydist, 1e-6)));
}
