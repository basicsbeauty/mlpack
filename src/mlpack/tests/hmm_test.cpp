/**
 * @file hmm_test.cpp
 *
 * Test file for HMMs.
 */
#include <mlpack/core.h>
#include <mlpack/methods/hmm/hmm.hpp>
#include <mlpack/methods/hmm/distributions/discrete_distribution.hpp>
#include <boost/test/unit_test.hpp>

using namespace mlpack;
using namespace mlpack::hmm;
using namespace mlpack::distribution;

BOOST_AUTO_TEST_SUITE(HMMTest);

/**
 * We will use the simple case proposed by Russell and Norvig in Artificial
 * Intelligence: A Modern Approach, 2nd Edition, around p.549.
 */
BOOST_AUTO_TEST_CASE(SimpleDiscreteHMMTestViterbi)
{
  // We have two hidden states: rain/dry.  Two emission states: umbrella/no
  // umbrella.
  // In this example, the transition matrix is
  //  rain  dry
  // [[0.7 0.3]  rain
  //  [0.3 0.7]] dry
  // and the emission probability is
  //  rain dry
  // [[0.9 0.2]  umbrella
  //  [0.1 0.8]] no umbrella
  arma::mat transition("0.7 0.3; 0.3 0.7");
  std::vector<DiscreteDistribution> emission(2);
  emission[0] = DiscreteDistribution("0.9 0.2");
  emission[1] = DiscreteDistribution("0.1 0.8");

  HMM<DiscreteDistribution> hmm(transition, emission);

  // Now let's take a sequence and find what the most likely state is.
  // We'll use the sequence [U U N U U] (U = umbrella, N = no umbrella) like on
  // p. 547.
  std::vector<size_t> observation;
  observation.push_back(0);
  observation.push_back(0);
  observation.push_back(1);
  observation.push_back(0);
  observation.push_back(0);

  std::vector<size_t> states;
  hmm.Predict(observation, states);

  // Check each state.
  BOOST_REQUIRE_EQUAL(states[0], 0); // Rain.
  BOOST_REQUIRE_EQUAL(states[1], 0); // Rain.
  BOOST_REQUIRE_EQUAL(states[2], 1); // No rain.
  BOOST_REQUIRE_EQUAL(states[3], 0); // Rain.
  BOOST_REQUIRE_EQUAL(states[4], 0); // Rain.
}

/**
 * This example is from Borodovsky & Ekisheva, p. 80-81.  It is just slightly
 * more complex.
 */
BOOST_AUTO_TEST_CASE(BorodovskyHMMTestViterbi)
{
  // Two hidden states: H (high GC content) and L (low GC content), as well as a
  // start state.
  arma::mat transition("0.0 0.0 0.0;"
                       "0.5 0.5 0.4;"
                       "0.5 0.5 0.6");
  // Four emission states: A, C, G, T.  Start state doesn't emit...
  std::vector<DiscreteDistribution> emission(3);
  emission[0] = DiscreteDistribution("0.25 0.25 0.25 0.25");
  emission[1] = DiscreteDistribution("0.20 0.30 0.30 0.20");
  emission[2] = DiscreteDistribution("0.30 0.20 0.20 0.30");

  HMM<DiscreteDistribution> hmm(transition, emission);

  // GGCACTGAA.
  std::vector<size_t> observation(9);
  observation[0] = 2;
  observation[1] = 2;
  observation[2] = 1;
  observation[3] = 0;
  observation[4] = 1;
  observation[5] = 3;
  observation[6] = 2;
  observation[7] = 0;
  observation[8] = 0;

  std::vector<size_t> states;
  hmm.Predict(observation, states);

  // Most probable path is HHHLLLLLL.
  BOOST_REQUIRE_EQUAL(states[0], 1);
  BOOST_REQUIRE_EQUAL(states[1], 1);
  BOOST_REQUIRE_EQUAL(states[2], 1);
  BOOST_REQUIRE_EQUAL(states[3], 2);
  // This could actually be one of two states (equal probability).
  BOOST_REQUIRE((states[4] == 1) || (states[4] == 2));
  BOOST_REQUIRE_EQUAL(states[5], 2);
  // This could also be one of two states.
  BOOST_REQUIRE((states[6] == 1) || (states[6] == 2));
  BOOST_REQUIRE_EQUAL(states[7], 2);
  BOOST_REQUIRE_EQUAL(states[8], 2);
}

/**
 * Ensure that the forward-backward algorithm is correct.
 */
BOOST_AUTO_TEST_CASE(ForwardBackwardTwoState)
{
  std::vector<size_t> obs(10);
  obs[0] = 3;
  obs[1] = 3;
  obs[2] = 2;
  obs[3] = 1;
  obs[4] = 1;
  obs[5] = 1;
  obs[6] = 1;
  obs[7] = 3;
  obs[8] = 3;
  obs[9] = 1;

  arma::mat transition("0.1 0.9; 0.4 0.6");
  std::vector<DiscreteDistribution> emis(2);
  emis[0] = DiscreteDistribution("0.85 0.15 0.00 0.00");
  emis[1] = DiscreteDistribution("0.00 0.00 0.50 0.50");

  HMM<DiscreteDistribution> hmm(transition, emis);

  // Now check we are getting the same results as MATLAB for this sequence.
  arma::mat stateProb;
  arma::mat forwardProb;
  arma::mat backwardProb;
  arma::vec scales;

  double log = hmm.Estimate(obs, stateProb, forwardProb, backwardProb, scales);

  // All values obtained from MATLAB hmmdecode().
  BOOST_REQUIRE_CLOSE(log, -23.4349, 1e-3);

  BOOST_REQUIRE_SMALL(stateProb(0, 0), 1e-5);
  BOOST_REQUIRE_CLOSE(stateProb(1, 0), 1.0, 1e-5);
  BOOST_REQUIRE_SMALL(stateProb(0, 1), 1e-5);
  BOOST_REQUIRE_CLOSE(stateProb(1, 1), 1.0, 1e-5);
  BOOST_REQUIRE_SMALL(stateProb(0, 2), 1e-5);
  BOOST_REQUIRE_CLOSE(stateProb(1, 2), 1.0, 1e-5);
  BOOST_REQUIRE_CLOSE(stateProb(0, 3), 1.0, 1e-5);
  BOOST_REQUIRE_SMALL(stateProb(1, 3), 1e-5);
  BOOST_REQUIRE_CLOSE(stateProb(0, 4), 1.0, 1e-5);
  BOOST_REQUIRE_SMALL(stateProb(1, 4), 1e-5);
  BOOST_REQUIRE_CLOSE(stateProb(0, 5), 1.0, 1e-5);
  BOOST_REQUIRE_SMALL(stateProb(1, 5), 1e-5);
  BOOST_REQUIRE_CLOSE(stateProb(0, 6), 1.0, 1e-5);
  BOOST_REQUIRE_SMALL(stateProb(1, 6), 1e-5);
  BOOST_REQUIRE_SMALL(stateProb(0, 7), 1e-5);
  BOOST_REQUIRE_CLOSE(stateProb(1, 7), 1.0, 1e-5);
  BOOST_REQUIRE_SMALL(stateProb(0, 8), 1e-5);
  BOOST_REQUIRE_CLOSE(stateProb(1, 8), 1.0, 1e-5);
  BOOST_REQUIRE_CLOSE(stateProb(0, 9), 1.0, 1e-5);
  BOOST_REQUIRE_SMALL(stateProb(1, 9), 1e-5);
}

/**
 * In this example we try to estimate the transmission and emission matrices
 * based on some observations.  We use the simplest possible model.
 */
BOOST_AUTO_TEST_CASE(SimplestBaumWelchDiscreteHMM)
{
  // Don't yet require a useful distribution.  1 state, 1 emission.
  HMM<DiscreteDistribution> hmm(1, DiscreteDistribution(1));

  std::vector<std::vector<size_t> > observations;
  observations.push_back(std::vector<size_t>(8, 0)); // 8 zeros.
  observations.push_back(std::vector<size_t>(7, 0)); // 7 zeros.
  observations.push_back(std::vector<size_t>(12, 0)); // 12 zeros.
  observations.push_back(std::vector<size_t>(10, 0)); // 10 zeros.

  hmm.Train(observations);

  BOOST_REQUIRE_CLOSE(hmm.Emission()[0].Probability(0), 1.0, 1e-5);
  BOOST_REQUIRE_CLOSE(hmm.Transition()(0, 0), 1.0, 1e-5);
}

/**
 * A slightly more complex model to estimate.
 */
BOOST_AUTO_TEST_CASE(SimpleBaumWelchDiscreteHMM)
{
  HMM<DiscreteDistribution> hmm(1, 2); // 1 state, 2 emissions.
  // Randomize the emission matrix.
  hmm.Emission()[0].Probabilities(arma::randu<arma::vec>(2));

  // P(each emission) = 0.5.
  // I've been careful to make P(first emission = 0) = P(first emission = 1).
  std::vector<std::vector<size_t> > observations;
  size_t obs[6][12] = {{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
                       {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
                       {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
                       {1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0},
                       {0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1},
                       {1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0}};

  for (size_t i = 0; i < 18; i++)
    observations.push_back(std::vector<size_t>(obs[i % 6], obs[i % 6] + 12));

  hmm.Train(observations);

  BOOST_REQUIRE_CLOSE(hmm.Emission()[0].Probability(0), 0.5, 1e-5);
  BOOST_REQUIRE_CLOSE(hmm.Emission()[0].Probability(1), 0.5, 1e-5);
  BOOST_REQUIRE_CLOSE(hmm.Transition()(0, 0), 1.0, 1e-5);
}

/**
 * Increasing complexity, but still simple; 4 emissions, 2 states; the state can
 * be determined directly by the emission.
 */
BOOST_AUTO_TEST_CASE(SimpleBaumWelchDiscreteHMM_2)
{
  HMM<DiscreteDistribution> hmm(2, DiscreteDistribution(4));

  // A little bit of obfuscation to the solution.
  hmm.Transition() = arma::mat("0.1 0.4; 0.9 0.6");
  hmm.Emission()[0].Probabilities("0.85 0.15 0.00 0.00");
  hmm.Emission()[1].Probabilities("0.00 0.00 0.50 0.50");

  // True emission matrix:
  //  [[0.4 0  ]
  //   [0.6 0  ]
  //   [0   0.2]
  //   [0   0.8]]

  // True transmission matrix:
  //  [[0.5 0.5]
  //   [0.5 0.5]]

  // Generate observations randomly by hand.  This is kinda ugly, but it works.
  std::vector<std::vector<size_t> > observations;
  size_t obsNum = 250; // Number of observations.
  size_t obsLen = 500; // Number of elements in each observation.
  for (size_t i = 0; i < obsNum; i++)
  {
    std::vector<size_t> observation(obsLen);

    size_t state = 0;
    size_t emission = 0;

    for (size_t obs = 0; obs < obsLen; obs++)
    {
      // See if state changed.
      double r = (double) rand() / (double) RAND_MAX;

      if (r <= 0.5)
        state = 0;
      else
        state = 1;

      // Now set the observation.
      r = (double) rand() / (double) RAND_MAX;

      switch (state)
      {
        // case 0 is not possible.
        case 0:
          if (r <= 0.4)
            emission = 0;
          else
            emission = 1;
          break;
        case 1:
          if (r <= 0.2)
            emission = 2;
          else
            emission = 3;
          break;
      }

      observation[obs] = emission;
    }

    observations.push_back(observation);
  }

  hmm.Train(observations);

  // Only require 2.5% tolerance, because this is a little fuzzier.
  BOOST_REQUIRE_CLOSE(hmm.Transition()(0, 0), 0.5, 2.5);
  BOOST_REQUIRE_CLOSE(hmm.Transition()(1, 0), 0.5, 2.5);
  BOOST_REQUIRE_CLOSE(hmm.Transition()(0, 1), 0.5, 2.5);
  BOOST_REQUIRE_CLOSE(hmm.Transition()(1, 1), 0.5, 2.5);

  BOOST_REQUIRE_CLOSE(hmm.Emission()[0].Probability(0), 0.4, 2.5);
  BOOST_REQUIRE_CLOSE(hmm.Emission()[0].Probability(1), 0.6, 2.5);
  BOOST_REQUIRE_SMALL(hmm.Emission()[0].Probability(2), 2.5);
  BOOST_REQUIRE_SMALL(hmm.Emission()[0].Probability(3), 2.5);
  BOOST_REQUIRE_SMALL(hmm.Emission()[1].Probability(0), 2.5);
  BOOST_REQUIRE_SMALL(hmm.Emission()[1].Probability(1), 2.5);
  BOOST_REQUIRE_CLOSE(hmm.Emission()[1].Probability(2), 0.2, 2.5);
  BOOST_REQUIRE_CLOSE(hmm.Emission()[1].Probability(3), 0.8, 2.5);
}

BOOST_AUTO_TEST_CASE(DiscreteHMMLabeledTrainTest)
{
  // Generate a random Markov model with 3 hidden states and 6 observations.
  arma::mat transition;
  std::vector<DiscreteDistribution> emission(3);

  transition.randu(3, 3);
  emission[0].Probabilities(arma::randu<arma::vec>(6));
  emission[1].Probabilities(arma::randu<arma::vec>(6));
  emission[2].Probabilities(arma::randu<arma::vec>(6));

  // Normalize so they we have a correct transition matrix.
  for (size_t col = 0; col < 3; col++)
    transition.col(col) /= accu(transition.col(col));

  // Now generate sequences.
  size_t obsNum = 250;
  size_t obsLen = 800;

  std::vector<std::vector<size_t> > observations(obsNum);
  std::vector<std::vector<size_t> > states(obsNum);

  for (size_t n = 0; n < obsNum; n++)
  {
    observations[n].resize(obsLen);
    states[n].resize(obsLen);

    // Random starting state.
    states[n][0] = rand() % 3;

    // Random starting observation.
    observations[n][0] = emission[states[n][0]].Random();

    // Now the rest of the observations.
    for (size_t t = 1; t < obsLen; t++)
    {
      // Choose random number for state transition.
      double state = (double) rand() / (double) RAND_MAX;

      // Decide next state.
      double sumProb = 0;
      for (size_t st = 0; st < 3; st++)
      {
        sumProb += transition(st, states[n][t - 1]);
        if (sumProb >= state)
        {
          states[n][t] = st;
          break;
        }
      }

      // Decide observation.
      observations[n][t] = emission[states[n][t]].Random();
    }
  }

  // Now that our data is generated, we give the HMM the labeled data to train
  // on.
  HMM<DiscreteDistribution> hmm(3, 6);

  hmm.Train(observations, states);

  // We can't use % tolerance here because percent error increases as the actual
  // value gets very small.  So, instead, we just ensure that every value is no
  // more than 0.009 away from the actual value.
  for (size_t row = 0; row < hmm.Transition().n_rows; row++)
    for (size_t col = 0; col < hmm.Transition().n_cols; col++)
      BOOST_REQUIRE_SMALL(hmm.Transition()(row, col) - transition(row, col),
          0.009);

  for (size_t col = 0; col < hmm.Emission().size(); col++)
    for (size_t row = 0; row < hmm.Emission()[col].Probabilities().n_elem;
        row++)
      BOOST_REQUIRE_SMALL(hmm.Emission()[col].Probability(row) -
          emission[col].Probability(row), 0.009);
}

/**
 * Make sure the Generate() function works for a uniformly distributed HMM;
 * we'll take many samples just to make sure.
 */
BOOST_AUTO_TEST_CASE(DiscreteHMMSimpleGenerateTest)
{
  // Very simple HMM.  4 emissions with equal probability and 2 states with
  // equal probability.  The default transition and emission matrices satisfy
  // this property.
  HMM<DiscreteDistribution> hmm(2, DiscreteDistribution(4));

  // Now generate a really, really long sequence.
  std::vector<size_t> dataSeq;
  std::vector<size_t> stateSeq;

  hmm.Generate(100000, dataSeq, stateSeq);

  // Now find the empirical probabilities of each state.
  arma::vec emissionProb(4);
  arma::vec stateProb(2);
  for (size_t i = 0; i < 100000; i++)
  {
    emissionProb[dataSeq[i]]++;
    stateProb[stateSeq[i]]++;
  }

  // Normalize so these are probabilities.
  emissionProb /= accu(emissionProb);
  stateProb /= accu(stateProb);

  // Now check that the probabilities are right.  2% tolerance.
  BOOST_REQUIRE_CLOSE(emissionProb[0], 0.25, 2.0);
  BOOST_REQUIRE_CLOSE(emissionProb[1], 0.25, 2.0);
  BOOST_REQUIRE_CLOSE(emissionProb[2], 0.25, 2.0);
  BOOST_REQUIRE_CLOSE(emissionProb[3], 0.25, 2.0);

  BOOST_REQUIRE_CLOSE(stateProb[0], 0.50, 2.0);
  BOOST_REQUIRE_CLOSE(stateProb[1], 0.50, 2.0);
}

/***
 * More complex test for Generate().
 */
BOOST_AUTO_TEST_CASE(DiscreteHMMGenerateTest)
{
  // 6 emissions, 4 states.  Random transition and emission probability.
  arma::mat transition(4, 4);
  std::vector<DiscreteDistribution> emission(4);
  emission[0].Probabilities(arma::randu<arma::vec>(6));
  emission[1].Probabilities(arma::randu<arma::vec>(6));
  emission[2].Probabilities(arma::randu<arma::vec>(6));
  emission[3].Probabilities(arma::randu<arma::vec>(6));

  transition.randu();

  // Normalize matrix.
  for (size_t col = 0; col < 4; col++)
    transition.col(col) /= accu(transition.col(col));

  // Create HMM object.
  HMM<DiscreteDistribution> hmm(transition, emission);

  // We'll create a bunch of sequences.
  int numSeq = 400;
  int numObs = 3000;
  std::vector<std::vector<size_t> > sequences(numSeq);
  std::vector<std::vector<size_t> > states(numSeq);
  for (int i = 0; i < numSeq; i++)
  {
    // Random starting state.
    size_t startState = rand() % 4;

    hmm.Generate(numObs, sequences[i], states[i], startState);
  }

  // Now we will calculate the full probabilities.
  HMM<DiscreteDistribution> hmm2(4, 6);
  hmm2.Train(sequences, states);

  // Check that training gives the same result.  Exact tolerance of 0.005.
  for (size_t row = 0; row < 4; row++)
    for (size_t col = 0; col < 4; col++)
      BOOST_REQUIRE_SMALL(hmm.Transition()(row, col) -
          hmm2.Transition()(row, col), 0.005);

  for (size_t row = 0; row < 6; row++)
    for (size_t col = 0; col < 4; col++)
      BOOST_REQUIRE_SMALL(hmm.Emission()[col].Probability(row) -
          hmm2.Emission()[col].Probability(row), 0.005);
}

BOOST_AUTO_TEST_CASE(DiscreteHMMLogLikelihoodTest)
{
  // Create a simple HMM with three states and four emissions.
  arma::mat transition("0.5 0.0 0.1;"
                       "0.2 0.6 0.2;"
                       "0.3 0.4 0.7");
  std::vector<DiscreteDistribution> emission(3);
  emission[0].Probabilities("0.75 0.25 0.00 0.00");
  emission[1].Probabilities("0.00 0.25 0.25 0.50");
  emission[2].Probabilities("0.10 0.40 0.40 0.10");

  HMM<DiscreteDistribution> hmm(transition, emission);

  // Now generate some sequences and check that the log-likelihood is the same
  // as MATLAB gives for this HMM.
  std::vector<size_t> seq(4);
  seq[0] = 0;
  seq[1] = 1;
  seq[2] = 2;
  seq[3] = 3;
  BOOST_REQUIRE_CLOSE(hmm.LogLikelihood(seq), -4.9887223949, 1e-5);

  seq[0] = 1;
  seq[1] = 2;
  seq[2] = 0;
  seq[3] = 0;
  BOOST_REQUIRE_CLOSE(hmm.LogLikelihood(seq), -6.0288487077, 1e-5);

  seq[0] = 3;
  seq[1] = 3;
  seq[2] = 3;
  seq[3] = 3;
  BOOST_REQUIRE_CLOSE(hmm.LogLikelihood(seq), -5.5544000018, 1e-5);

  seq.resize(17);
  seq[0] = 0;
  seq[1] = 2;
  seq[2] = 2;
  seq[3] = 1;
  seq[4] = 2;
  seq[5] = 3;
  seq[6] = 0;
  seq[7] = 0;
  seq[8] = 1;
  seq[9] = 3;
  seq[10] = 1;
  seq[11] = 0;
  seq[12] = 0;
  seq[13] = 3;
  seq[14] = 1;
  seq[15] = 2;
  seq[16] = 2;
  BOOST_REQUIRE_CLOSE(hmm.LogLikelihood(seq), -24.51556128368, 1e-5);
}

BOOST_AUTO_TEST_SUITE_END();
