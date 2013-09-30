/**
 * @file cf.cpp
 * @author Mudit Raj Gupta
 *
 * Collaborative Filtering.
 *
 * Implementation of CF class to perform Collaborative Filtering on the
 * specified data set.
 *
 */
#include "cf.hpp"
#include <mlpack/methods/nmf/nmf.hpp>
#include <mlpack/methods/nmf/als_update_rules.hpp>

using namespace mlpack::nmf;
using namespace std;

namespace mlpack {
namespace cf {

/**
 * Construct the CF object.
 */
CF::CF(arma::mat& data) :
     data(data)
{
  Log::Info<<"Constructor (param: input data, default: numRecs;neighbourhood)"<<endl;
  this->numRecs = 5;
  this->numUsersForSimilarity = 5;

  CleanData();
}

CF::CF(const size_t numRecs,arma::mat& data) :
     data(data)
{
  // Validate number of recommendation factor.
  if (numRecs < 1)
  {
    Log::Warn << "CF::CF(): number of recommendations shoud be > 0("
        << numRecs << " given). Setting value to 5.\n";
    //Setting Default Value of 5
    this->numRecs = 5;
  }
  else
    this->numRecs = numRecs;
  this->numUsersForSimilarity = 5;

  CleanData();
}

CF::CF(const size_t numRecs, const size_t numUsersForSimilarity,
     arma::mat& data) :
     data(data)
{
  // Validate number of recommendation factor.
  if (numRecs < 1)
  {
    Log::Warn << "CF::CF(): number of recommendations shoud be > 0("
        << numRecs << " given). Setting value to 5.\n";
    //Setting Default Value of 5
    this->numRecs = 5;
  }
  else
    this->numRecs = numRecs;
  // Validate neighbourhood size.
  if (numUsersForSimilarity < 1)
  {
    Log::Warn << "CF::CF(): neighbourhood size shoud be > 0("
        << numUsersForSimilarity << " given). Setting value to 5.\n";
    //Setting Default Value of 5
    this->numUsersForSimilarity = 5;
  }
  else
    this->numUsersForSimilarity = numUsersForSimilarity;

  CleanData();
}

void CF::GetRecommendations(arma::Mat<size_t>& recommendations)
{
  // Used to save user IDs.
  arma::Col<size_t> users =
    arma::zeros<arma::Col<size_t> >(cleanedData.n_cols, 1);
  // Getting all user IDs.
  for (size_t i = 0; i < cleanedData.n_cols; i++)
    users(i) = i + 1;

  // Calling base function for recommendations.
  GetRecommendations(recommendations, users);
}

void CF::GetRecommendations(arma::Mat<size_t>& recommendations,
                            arma::Col<size_t>& users)
{
  // Base function for calculating recommendations.

  // Operations independent of the query:
  // Decompose the sparse data matrix to user and data matrices.
  // Should this rank be parameterizable?
  size_t rank = 2;

  // Presently only ALS (via NMF) is supported as an optimizer.  This should be
  // converted to a template when more optimizers are available.
  NMF<RandomInitialization, WAlternatingLeastSquaresRule,
      HAlternatingLeastSquaresRule> als(10000, 1e-5);
  als.Apply(cleanedData, rank, w, h);

  // Generate new table by multiplying approximate values.
  rating = w * h;

  // Query-dependent operations.
  Query(recommendations,users);
}

void CF::GetRecommendations(arma::Mat<size_t>& recommendations,
                            arma::Col<size_t>& users,size_t num)
{
  //Setting Number of Recommendations
  NumRecs(num);
  //Calling Base Function for Recommendations
  GetRecommendations(recommendations,users);
}

void CF::GetRecommendations(arma::Mat<size_t>& recommendations,
                            arma::Col<size_t>& users,size_t num,size_t s)
{
  //Setting number of users that should be used for calculating
  //neighbours
  NumUsersForSimilarity(s);
  //Setting Number of Recommendations
  NumRecs(num);
  //Calling Base Function for Recommendations
  GetRecommendations(recommendations,users,num);
}

void CF::CleanData()
{
  // Generate list of locations for batch insert constructor for sparse
  // matrices.
  arma::umat locations(2, data.n_cols);
  arma::vec values(data.n_cols);
  for (size_t i = 0; i < data.n_cols; ++i)
  {
    // We have to transpose it because items are rows, and users are columns.
    locations(1, i) = ((arma::uword) data(0, i)) - 1;
    locations(0, i) = ((arma::uword) data(1, i)) - 1;
    values(i) = data(2, i);
  }

  // Find maximum user and item IDs.
  const size_t maxItemID = (size_t) max(locations.row(0)) + 1;
  const size_t maxUserID = (size_t) max(locations.row(1)) + 1;

  // Fill sparse matrix and mask matrix.
  cleanedData = arma::sp_mat(locations, values, maxItemID, maxUserID);
  mask = arma::ones<arma::mat>((size_t) maxItemID,
                               (size_t) maxUserID);
  // Populate mask.
  for (size_t i = 0; i < data.n_cols; i++)
    mask(data(1, i) - 1, data(0, i) - 1) = -1.0;
}

void CF::Query(arma::Mat<size_t>& recommendations,
               arma::Col<size_t>& users)
{
  // Temporarily store feature vector of queried users.
  arma::mat query(rating.n_rows, users.n_elem);

  // Select feature vectors of queried users.
  for (size_t i = 0; i < users.n_elem; i++)
    query.col(i) = rating.col(users(i) - 1);

  // Temporary storage for neighborhood of the queried users.
  arma::Mat<size_t> neighborhood;

  // Calculate the neighborhood of the queried users.
  // This should be a templatized option.
  AllkNN a(rating, query);
  arma::mat resultingDistances; // Temporary storage.
  a.Search(numUsersForSimilarity, neighborhood, resultingDistances);

  // Temporary storage for storing the average rating for each user in their
  // neighborhood.
  arma::mat averages = arma::zeros<arma::mat>(rating.n_rows, query.n_cols);

  // Calculate the average values.
  CalculateAverage(neighborhood, averages);

  // Calculate the top recommendations.
  CalculateTopRecommendations(recommendations, averages, users);
}

void CF::CalculateAverage(arma::Mat<size_t>& neighbourhood,
                      arma::mat& averages) const
{
  Log::Info<<"CalculateAverage"<<endl;
  //Temprorary Storage for calculating sum
  arma::Col<double> tmp = arma::zeros<arma::Col<double> >(rating.n_rows,1);
  size_t j;
  //Iterating over all users
  for(size_t i=0;i<neighbourhood.n_cols;i++)
  {
    tmp = arma::zeros<arma::Col<double> >(rating.n_rows,1);
    //Iterating over all neighbours
    for(j=0;j<neighbourhood.n_rows;j++)
      tmp += rating.col(neighbourhood(j,i));
    //Calculating averages
    averages.col(i) = tmp/j;
  }
  //data::Save("averages.csv",averages);
}

void CF::CalculateTopRecommendations(arma::Mat<size_t>& recommendations,
                                 arma::mat& averages,
                                 arma::Col<size_t>& users) const
{
  Log::Info<<"CalculateTopRecommendations"<<endl;
  int recos = numRecs;
  if(averages.n_cols<numRecs)
    recos = averages.n_rows;
  //Stores recommendations
  arma::Mat<size_t> rec = arma::zeros<arma::Mat<size_t> >(recos,users.n_rows);
  //Stores valid ratings for items by a user
  arma::Col<double> tmp = arma::zeros<arma::Col<double> >(rating.n_cols,1);
  //Maps the items to their ratings
  std::map<double,size_t> tmpMap;
  //Iterator for the Item-Rating Map
  std::map<double,size_t>::reverse_iterator iter;
  std::map<double,size_t>::iterator it;
  //Keeps count of number of recommendations provided for a user
  size_t count;
  //Iterate for all users
  for(size_t i=0;i<users.n_rows;i++)
  {
    count=0;
    //Dot product between average rating and mask to dilute the ratings
    // of the items that user i has already rated
    tmp = averages.col(users(i)-1) % mask.col(users(i)-1);
    //Mapping Rating to Items
    for(size_t j=0;j<tmp.n_rows;j++)
      if(tmp(j)>=0)
        tmpMap.insert(std::pair<double,size_t>(tmp(j),j+1));
    //Iterating over Item-Rating Map
    for(iter=tmpMap.rbegin();iter!=tmpMap.rend();++iter)
    {
       //Saving recommendations to the recommendations table
      rec(count,i) = (size_t)iter->second;
      count++;
      //break is desired number of recommendations are saved
      if(count==numRecs)
        break;
    }
    //Removing the items from the map
    //note: Item 0 is just to maintain the consistency and it
    //represents not recommendations were available
    for(it=tmpMap.begin();it!=tmpMap.end();++it)
      tmpMap.erase(it);
  }
  //Saving to recommendations
  recommendations = rec;
}

}; // namespace mlpack
}; // namespace cf
