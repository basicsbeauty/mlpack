/**
 * @file farfield_expansion.h
 *
 * This file contains a templatized class implementing $O(D^p)$
 * expansion for computing the coefficients for a far-field expansion
 * for an arbitrary kernel function.
 *
 * @author Dongryeol Lee (dongryel)
 * @bug No known bugs.
 */

#ifndef FARFIELD_EXPANSION
#define FARFIELD_EXPANSION

#include "fastlib/fastlib.h"
#include "kernel_aux.h"
#include "series_expansion_aux.h"

template<typename TKernelAux> 
class LocalExpansion;

/** @brief Far field expansion class in \f$O(D^p)\f$ expansion.
 *
 *  \f$O(D^p)\f$ expansion for a kernel is a traditional expansion
 *  generated by the multivariate Taylor expansion.
 *
 *  @code
 *   //Declare a far-field expansion for the Gaussian kernel.
 *   FarFieldExpansion<GaussianKernelAux> fe;
 *
 *  @endcode
 */
template<typename TKernelAux>
class FarFieldExpansion {

 private:
  
  ////////// Private Member Variables //////////

  /** @brief The center of the expansion. */
  Vector center_;
  
  /** @brief The coefficients. */
  Vector coeffs_;
  
  /** @brief The order of the expansion. */
  int order_;
  
  /** @brief The pointer to the auxilirary methods for the kernel
   *         (derivative, truncation error bound).
   */
  const TKernelAux *ka_;

  /** @brief The pointer to the kernel object inside kernel auxiliary
   *         object.
   */
  const typename TKernelAux::TKernel *kernel_;

  /** @brief The pointer to the precomputed constants inside kernel
   *         auxiliary object 
   */
  const typename TKernelAux::TSeriesExpansionAux *sea_;

  OT_DEF(FarFieldExpansion) {
    OT_MY_OBJECT(center_);
    OT_MY_OBJECT(coeffs_);
    OT_MY_OBJECT(order_);
  }

 public:
  
  ////////// Getters/Setters //////////
  
  /** @brief Gets the squared bandwidth value that is being used by
   *         the current far-field expansion object.
   *
   *  @return The squared bandwidth value.
   */
  double bandwidth_sq() const { return kernel_->bandwidth_sq(); }

  /** @brief Gets the center of expansion.
   *
   *  @return The center of expansion for the current far-field expansion.
   */
  Vector* get_center() { return &center_; }

  const Vector* get_center() const { return &center_; }

  /** @brief Gets the set of far-field coefficients.
   *
   *  @return The const reference to the vector containing the
   *          far-field coefficients.
   */
  const Vector& get_coeffs() const { return coeffs_; }
  
  /** @brief Gets the approximation order.
   *
   *  @return The integer representing the current approximation order.
   */
  int get_order() const { return order_; }
  
  /** @brief Gets the maximum possible approximation order.
   *
   *  @return The maximum allowable approximation order.
   */
  int get_max_order() const { return sea_->get_max_order(); }

  /** @brief Sets the approximation order of the far-field expansion.
   *
   *  @param new_order The desired new order of the approximation.
   */
  void set_order(int new_order) { order_ = new_order; }
  
  /** @brief Set the center of the expansion - assumes that the center
   *         has been initialized before...
   *
   *  @param center The center of expansion whose coordinate values
   *                will be copied to the center of the given far-field 
   *                expansion object.
   */
  void set_center(const Vector &center) {
    
    for(index_t i = 0; i < center.length(); i++) {
      center_[i] = center[i];
    }
  }

  ////////// User-level Functions //////////
  
  /** @brief Accumulates the contribution of a single reference point as a 
   *         far-field moment.
   *
   *  For the reference point $r$ and the current expansion centroid
   *  in the reference node \f$R\f$ as \f$R.c\f$, this function
   *  computes \f$\left( \frac{r - R.c}{kh} \right)^{\alpha}\f$ for
   *  \f$0 \leq \alpha \leq p\f$ where \f$\alpha\f$ is a
   *  \f$D\f$-dimensional multi-index and adds to the currently
   *  accumulated far-field moments: \f$ F(R) \leftarrow F(R) + w [
   *  \left( \frac{r - R.c}{kh} \right)^{\alpha}]_{0 \leq p \leq
   *  \alpha}\f$ where \f$F(R)\f$ is the current set of far-field
   *  moments for the reference node \f$R\f$. \f$k\f$ is an
   *  appropriate factor to multiply the bandwidth \f$h\f$ by; for the
   *  Gaussian kernel expansion, it is \f$\sqrt{2}\f$.
   *
   *  @param reference_point The coordinates of the reference point.
   *  @param weight The weight of the reference point v.
   *  @param order The order up to which the far-field moments should be 
   *               accumulated up to.
   */
  void Accumulate(const Vector &reference_point, double weight, int order);

  /** @brief Accumulates the far field moment represented by the given
   *         reference data into the coefficients.
   *
   *  Given the set of reference points \f$r_{j_n} \in R\f$ in the
   *  reference node \f$R\f$, this function computes
   *  \f$\sum\limits_{r_{j_n} \in R} w_{j_n} \left( \frac{r_{j_n} -
   *  R.c}{kh} \right)^{\alpha}\f$ for \f$0 \leq \alpha \leq p\f$
   *  where \f$\alpha\f$ is a \f$D\f$-dimensional multi-index and adds
   *  to the currently accumulated far-field moments: \f$ F(R)
   *  \leftarrow F(R) + \left[ \sum\limits_{r_{j_n} \in R} w_{j_n}
   *  \left( \frac{r_{j_n} - R.c}{kh} \right)^{\alpha} \right]_{0 \leq
   *  p \leq \alpha}\f$ where \f$F(R)\f$ is the current set of
   *  far-field moments for the reference node \f$R\f$. \f$k\f$ is an
   *  appropriate factor to multiply the bandwidth \f$h\f$ by; for the
   *  Gaussian kernel expansion, it is \f$\sqrt{2}\f$.
   *
   *  @param data The entire reference dataset \f$\mathcal{R}\f$.
   *  @param weights The entire reference weights \f$\mathcal{W}\f$.
   *  @param begin The beginning index of the reference points for
   *               which we would like to accumulate the moments for.
   *  @param end The upper limit on the index of the reference points for
   *             which we would like to accumulate the moments for.
   *  @param order The order up to which the far-field moments should be
   *               accumulated up to.
   */
  void AccumulateCoeffs(const Matrix& data, const Vector& weights,
			int begin, int end, int order);

  /** @brief Refine the far field moment that has been computed before
   *         up to a new order.
   */
  void RefineCoeffs(const Matrix& data, const Vector& weights,
		    int begin, int end, int order);
  
  /** @brief Evaluates the far-field coefficients at the given point.
   */
  double EvaluateField(const Matrix& data, int row_num, int order) const;
  double EvaluateField(const Vector &x_q, int order) const;

  /** @brief Evaluates the two-way convolution mixed with exhaustive
   *         computations with two other far field expansions.
   */
  double MixField(const Matrix &data, int node1_begin, int node1_end, 
		  int node2_begin, int node2_end, const FarFieldExpansion &fe2,
		  const FarFieldExpansion &fe3, int order2, int order3) const;

  /** @brief Evaluates the three-way convolution with two other far
   *         field expansions.
   */
  double ConvolveField(const FarFieldExpansion &fe2,
		       const FarFieldExpansion &fe3,
		       int order1, int order2, int order3) const;

  /** @brief Initializes the current far field expansion object with
   *         the given center.
   */
  void Init(const Vector& center, const TKernelAux &ka);
  void Init(const TKernelAux &ka);

  /** @brief Computes the required order for evaluating the far field
   *         expansion for any query point within the specified region
   *         for a given bound.
   */
  int OrderForEvaluating(const DHrectBound<2> &far_field_region,
			 const DHrectBound<2> &local_field_region,
			 double min_dist_sqd_regions,
			 double max_dist_sqd_regions,
			 double max_error, double *actual_error) const;

  /** @brief Computes the required order for converting to the local
   *         expansion inside another region, so that the total error
   *         (truncation error of the far field expansion plus the
   *         conversion error) is bounded above by the given user
   *         bound.
   *
   *  @return the minimum approximation order required for the error,
   *          -1 if approximation up to the maximum order is not possible.
   */
  int OrderForConvertingToLocal(const DHrectBound<2> &far_field_region,
				const DHrectBound<2> &local_field_region, 
				double min_dist_sqd_regions, 
				double max_dist_sqd_regions,
				double required_bound, 
				double *actual_error) const;

  /** @brief Prints out the series expansion represented by this object.
   */
  void PrintDebug(const char *name="", FILE *stream=stderr) const;

  /** @brief Translate from a far field expansion to the expansion
   *         here. The translated coefficients are added up to the
   *         ones here.
   */
  void TranslateFromFarField(const FarFieldExpansion &se);
  
  /** @brief Translate to the given local expansion. The translated
   *         coefficients are added up to the passed-in local
   *         expansion coefficients.
   */
  void TranslateToLocal(LocalExpansion<TKernelAux> &se, int truncation_order);

};

template<typename TKernelAux>
void FarFieldExpansion<TKernelAux>::Accumulate(const Vector &v, double weight,
					       int order) {

  int dim = v.length();
  int total_num_coeffs = sea_->get_total_num_coeffs(order);
  Vector tmp;
  int r, i, j, k, t, tail;
  Vector heads;
  Vector x_r;
  double bandwidth_factor = ka_->BandwidthFactor(kernel_->bandwidth_sq());

  // initialize temporary variables
  tmp.Init(total_num_coeffs);
  heads.Init(dim + 1);
  x_r.Init(dim);
  Vector pos_coeffs;
  Vector neg_coeffs;
  pos_coeffs.Init(total_num_coeffs);
  pos_coeffs.SetZero();
  neg_coeffs.Init(total_num_coeffs);
  neg_coeffs.SetZero();

  // set to new order if greater
  if(order_ < order) {
    order_ = order;
  }
  Vector C_k;
    
  // Calculate the coordinate difference between the ref point and the 
  // centroid.
  for(i = 0; i < dim; i++) {
    x_r[i] = (v[i] - center_[i]) / bandwidth_factor;
  }
  
  // initialize heads
  heads.SetZero();
  heads[dim] = INT_MAX;
  
  tmp[0] = 1.0;
  
  for(k = 1, t = 1, tail = 1; k <= order; k++, tail = t) {
    for(i = 0; i < dim; i++) {
      int head = (int) heads[i];
      heads[i] = t;
      
      for(j = head; j < tail; j++, t++) {
	tmp[t] = tmp[j] * x_r[i];
      }
    }
  }
  
  // Tally up the result in A_k.
  for(i = 0; i < total_num_coeffs; i++) {
    double prod = weight * tmp[i];
    
    if(prod > 0) {
      pos_coeffs[i] += prod;
    }
    else {
      neg_coeffs[i] += prod;
    }
  }

  // get multiindex factors
  C_k.Alias(sea_->get_inv_multiindex_factorials());
  
  for(r = 0; r < total_num_coeffs; r++) {
    coeffs_[r] += (pos_coeffs[r] + neg_coeffs[r]) * C_k[r];
  }
}

template<typename TKernelAux>
void FarFieldExpansion<TKernelAux>::AccumulateCoeffs(const Matrix& data, 
						     const Vector& weights, 
						     int begin, int end, 
						     int order) {
  
  int dim = data.n_rows();
  int total_num_coeffs = sea_->get_total_num_coeffs(order);
  Vector tmp;
  int r, i, j, k, t, tail;
  Vector heads;
  Vector x_r;
  double bandwidth_factor = ka_->BandwidthFactor(kernel_->bandwidth_sq());

  // initialize temporary variables
  tmp.Init(total_num_coeffs);
  heads.Init(dim + 1);
  x_r.Init(dim);
  Vector pos_coeffs;
  Vector neg_coeffs;
  pos_coeffs.Init(total_num_coeffs);
  pos_coeffs.SetZero();
  neg_coeffs.Init(total_num_coeffs);
  neg_coeffs.SetZero();

  // set to new order if greater
  if(order_ < order) {
    order_ = order;
  }
  Vector C_k;
    
  // Repeat for each reference point in this reference node.
  for(r = begin; r < end; r++) {
    
    // Calculate the coordinate difference between the ref point and the 
    // centroid.
    for(i = 0; i < dim; i++) {
      x_r[i] = (data.get(i, r) - center_[i]) / bandwidth_factor;
    }

    // initialize heads
    heads.SetZero();
    heads[dim] = INT_MAX;
    
    tmp[0] = 1.0;
    
    for(k = 1, t = 1, tail = 1; k <= order; k++, tail = t) {
      for(i = 0; i < dim; i++) {
	int head = (int) heads[i];
	heads[i] = t;
	
	for(j = head; j < tail; j++, t++) {
	  tmp[t] = tmp[j] * x_r[i];
	}
      }
    }
    
    // Tally up the result in A_k.
    for(i = 0; i < total_num_coeffs; i++) {
      double prod = weights[r] * tmp[i];
      
      if(prod > 0) {
	pos_coeffs[i] += prod;
      }
      else {
	neg_coeffs[i] += prod;
      }
    }
    
  } // End of looping through each reference point

  // get multiindex factors
  C_k.Alias(sea_->get_inv_multiindex_factorials());

  for(r = 0; r < total_num_coeffs; r++) {
    coeffs_[r] += (pos_coeffs[r] + neg_coeffs[r]) * C_k[r];
  }
}

template<typename TKernelAux>
void FarFieldExpansion<TKernelAux>::RefineCoeffs(const Matrix& data, 
						 const Vector& weights, 
						 int begin, int end, 
						 int order) {
  
  if(order_ < 0) {
    
    AccumulateCoeffs(data, weights, begin, end, order);
    return;
  }

  int dim = data.n_rows();
  int old_total_num_coeffs = sea_->get_total_num_coeffs(order_);
  int total_num_coeffs = sea_->get_total_num_coeffs(order);
  double tmp;
  int r, i, j;
  Vector x_r;
  double bandwidth_factor = ka_->BandwidthFactor(kernel_->bandwidth_sq());

  // initialize temporary variables
  x_r.Init(dim);
  Vector pos_coeffs;
  Vector neg_coeffs;
  pos_coeffs.Init(total_num_coeffs);
  pos_coeffs.SetZero();
  neg_coeffs.Init(total_num_coeffs);
  neg_coeffs.SetZero();

  // if we already have the order of approximation, then return.
  if(order_ >= order) {
    return;
  }
  else {
    order_ = order;
  }

  Vector C_k;

  // Repeat for each reference point in this reference node.
  for(r = begin; r < end; r++) {
    
    // Calculate the coordinate difference between the ref point and the 
    // centroid.
    for(i = 0; i < dim; i++) {
      x_r[i] = (data.get(i, r) - center_[i]) / bandwidth_factor;
    }

    // compute in bruteforce way
    for(i = old_total_num_coeffs; i < total_num_coeffs; i++) {
      ArrayList<int> mapping = sea_->get_multiindex(i);
      tmp = 1;
      
      for(j = 0; j < dim; j++) {
	tmp *= pow(x_r[j], mapping[j]);
      }

      double prod = weights[r] * tmp;
      
      if(prod > 0) {
        pos_coeffs[i] += prod;
      }
      else {
        neg_coeffs[i] += prod;
      }
    }
    
  } // End of looping through each reference point

  // get multiindex factors
  C_k.Alias(sea_->get_inv_multiindex_factorials());

  for(r = old_total_num_coeffs; r < total_num_coeffs; r++) {
    coeffs_[r] = (pos_coeffs[r] + neg_coeffs[r]) * C_k[r];
  }
}

template<typename TKernelAux>
double FarFieldExpansion<TKernelAux>::EvaluateField(const Matrix& data, 
						    int row_num, 
						    int order) const {
  
  // dimension
  int dim = sea_->get_dimension();

  // total number of coefficients
  int total_num_coeffs = sea_->get_total_num_coeffs(order);

  // square root times bandwidth
  double bandwidth_factor = ka_->BandwidthFactor(kernel_->bandwidth_sq());
  
  // the evaluated sum
  double pos_multipole_sum = 0;
  double neg_multipole_sum = 0;
  double multipole_sum = 0;
  
  // computed derivative map
  Matrix derivative_map;
  derivative_map.Init(dim, order + 1);

  // temporary variable
  Vector arrtmp;
  arrtmp.Init(total_num_coeffs);

  // (x_q - x_R) scaled by bandwidth
  Vector x_q_minus_x_R;
  x_q_minus_x_R.Init(dim);

  // compute (x_q - x_R) / (sqrt(2h^2))
  for(index_t d = 0; d < dim; d++) {
    x_q_minus_x_R[d] = (data.get(d, row_num) - center_[d]) / 
      bandwidth_factor;
  }

  // compute deriative maps based on coordinate difference.
  ka_->ComputeDirectionalDerivatives(x_q_minus_x_R, derivative_map);

  // compute h_{\alpha}((x_q - x_R)/sqrt(2h^2)) ((x_r - x_R)/h)^{\alpha}
  for(index_t j = 0; j < total_num_coeffs; j++) {
    ArrayList<int> mapping = sea_->get_multiindex(j);
    double arrtmp = ka_->ComputePartialDerivative(derivative_map, mapping);
    double prod = coeffs_[j] * arrtmp;
    
    if(prod > 0) {
      pos_multipole_sum += prod;
    }
    else {
      neg_multipole_sum += prod;
    }
  }

  multipole_sum = pos_multipole_sum + neg_multipole_sum;
  return multipole_sum;
}

template<typename TKernelAux>
double FarFieldExpansion<TKernelAux>::EvaluateField(const Vector &x_q, 
						    int order) const {
  
  // dimension
  int dim = sea_->get_dimension();

  // total number of coefficients
  int total_num_coeffs = sea_->get_total_num_coeffs(order);

  // square root times bandwidth
  double bandwidth_factor = ka_->BandwidthFactor(kernel_->bandwidth_sq());
  
  // the evaluated sum
  double pos_multipole_sum = 0;
  double neg_multipole_sum = 0;
  double multipole_sum = 0;
  
  // computed derivative map
  Matrix derivative_map;
  derivative_map.Init(dim, order + 1);

  // temporary variable
  Vector arrtmp;
  arrtmp.Init(total_num_coeffs);

  // (x_q - x_R) scaled by bandwidth
  Vector x_q_minus_x_R;
  x_q_minus_x_R.Init(dim);

  // compute (x_q - x_R) / (sqrt(2h^2))
  for(index_t d = 0; d < dim; d++) {
    x_q_minus_x_R[d] = (x_q[d] - center_[d]) / bandwidth_factor;
  }

  // compute deriative maps based on coordinate difference.
  ka_->ComputeDirectionalDerivatives(x_q_minus_x_R, derivative_map);

  // compute h_{\alpha}((x_q - x_R)/sqrt(2h^2)) ((x_r - x_R)/h)^{\alpha}
  for(index_t j = 0; j < total_num_coeffs; j++) {
    ArrayList<int> mapping = sea_->get_multiindex(j);
    double arrtmp = ka_->ComputePartialDerivative(derivative_map, mapping);
    double prod = coeffs_[j] * arrtmp;
    
    if(prod > 0) {
      pos_multipole_sum += prod;
    }
    else {
      neg_multipole_sum += prod;
    }
  }

  multipole_sum = pos_multipole_sum + neg_multipole_sum;
  return multipole_sum;
}

template<typename TKernelAux>
double FarFieldExpansion<TKernelAux>::MixField(const Matrix &data, 
					       int node1_begin, int node1_end,
					       int node2_begin, int node2_end, 
					       const FarFieldExpansion &fe2,
					       const FarFieldExpansion &fe3, 
					       int order2, int order3) const {
  
  // bandwidth factor and multiindex mapping stuffs
  double result;
  double bandwidth_factor = ka_->BandwidthFactor(bandwidth_sq());
  const ArrayList<int> *multiindex_mapping = sea_->get_multiindex_mapping();
  const ArrayList<int> *lower_mapping_index = sea_->get_lower_mapping_index();

  // get the total number of coefficients and coefficients
  int total_num_coeffs2 = sea_->get_total_num_coeffs(order2);
  int total_num_coeffs3 = sea_->get_total_num_coeffs(order3);
  int dim = sea_->get_dimension();
  Vector coeffs2, coeffs3;
  coeffs2.Alias(fe2.get_coeffs());
  coeffs3.Alias(fe3.get_coeffs());

  // actual accumulated sum
  double neg_sum = 0;
  double pos_sum = 0;
  double sum = 0;
  
  // some temporary
  double moment_k;
  double xi_xI, xj_xJ, diff;

  // temporary array
  ArrayList<int> beta_gamma_nu_eta_mapping;
  ArrayList<int> beta_nu_mapping;
  ArrayList<int> gamma_eta_mapping;
  beta_nu_mapping.Init(dim);
  gamma_eta_mapping.Init(dim);
  beta_gamma_nu_eta_mapping.Init(dim);

  // partial derivatives table
  Matrix derivative_map_beta;
  derivative_map_beta.Init(dim, order2 + 1);
  Matrix derivative_map_gamma;
  derivative_map_gamma.Init(dim, order3 + 1);
  
  // compute center differences and complete the table of partial derivatives
  Vector xI_xK, xJ_xK;
  xI_xK.Init(dim);
  xJ_xK.Init(dim);
  Vector xJ_center, xK_center;
  xJ_center.Alias(*(fe2.get_center()));
  xK_center.Alias(*(fe3.get_center()));

  for(index_t d = 0; d < dim; d++) {
    xI_xK[d] = (center_[d] - xK_center[d]) / bandwidth_factor;
    xJ_xK[d] = (xJ_center[d] - xK_center[d]) / bandwidth_factor;
  }
  ka_->ComputeDirectionalDerivatives(xI_xK, derivative_map_beta);
  ka_->ComputeDirectionalDerivatives(xJ_xK, derivative_map_gamma);

  // inverse factorials
  Vector inv_multiindex_factorials;
  inv_multiindex_factorials.Alias(sea_->get_inv_multiindex_factorials());

  // precompute pairwise kernel values between node i and node j
  Matrix exhaustive_ij;
  exhaustive_ij.Init(node1_end - node1_begin, node2_end - node2_begin);
  for(index_t i = node1_begin; i < node1_end; i++) {
    const double *i_col = data.GetColumnPtr(i);
    for(index_t j = node2_begin; j < node2_end; j++) {
      const double *j_col = data.GetColumnPtr(j);
      
      exhaustive_ij.set
	(i - node1_begin, j - node2_begin, 
	 kernel_->EvalUnnormOnSq(la::DistanceSqEuclidean(data.n_rows(), 
							i_col, j_col)));
    }
  }

  // main loop
  for(index_t beta = 0; beta < total_num_coeffs2; beta++) {
    
    ArrayList <int> beta_mapping = multiindex_mapping[beta];
    ArrayList <int> lower_mappings_for_beta = lower_mapping_index[beta];
    double beta_derivative = ka_->ComputePartialDerivative
      (derivative_map_beta, beta_mapping);
    
    for(index_t nu = 0; nu < lower_mappings_for_beta.size(); nu++) {
      
      ArrayList<int> nu_mapping = 
	multiindex_mapping[lower_mappings_for_beta[nu]];
      
      // beta - nu
      for(index_t d = 0; d < dim; d++) {
	beta_nu_mapping[d] = beta_mapping[d] - nu_mapping[d];
      }
      
      for(index_t gamma = 0; gamma < total_num_coeffs3; gamma++) {
	
	ArrayList <int> gamma_mapping = multiindex_mapping[gamma];
	ArrayList <int> lower_mappings_for_gamma = 
	  lower_mapping_index[gamma];
	double gamma_derivative = ka_->ComputePartialDerivative
	  (derivative_map_gamma, gamma_mapping);
	
	for(index_t eta = 0; eta < lower_mappings_for_gamma.size(); 
	    eta++){
	  
	  // add up alpha, mu, eta and beta, gamma, nu, eta
	  int sign = 0;
	  
	  ArrayList<int> eta_mapping =
	    multiindex_mapping[lower_mappings_for_gamma[eta]];
	  
	  for(index_t d = 0; d < dim; d++) {
	    beta_gamma_nu_eta_mapping[d] = beta_mapping[d] +
	      gamma_mapping[d] - nu_mapping[d] - eta_mapping[d];
	    gamma_eta_mapping[d] = gamma_mapping[d] - eta_mapping[d];
	    
	    sign += 2 * (beta_mapping[d] + gamma_mapping[d]) - 
	      (nu_mapping[d] + eta_mapping[d]);
	  }
	  if(sign % 2 == 1) {
	    sign = -1;
	  }
	  else {
	    sign = 1;
	  }
	  
	  // retrieve moments for appropriate multiindex maps
	  moment_k = coeffs3[sea_->ComputeMultiindexPosition
			     (beta_gamma_nu_eta_mapping)];

	  // loop over every pairs of points in node i and node j
	  for(index_t i = node1_begin; i < node1_end; i++) {
	    
	    xi_xI = 
	      inv_multiindex_factorials
	      [sea_->ComputeMultiindexPosition(nu_mapping)];
	    for(index_t d = 0; d < dim; d++) {
	      diff = (data.get(d, i) - center_[d]) / bandwidth_factor;
	      xi_xI *= pow(diff, nu_mapping[d]);
	    }	    

	    for(index_t j = node2_begin; j < node2_end; j++) {

	      xj_xJ = inv_multiindex_factorials
		[sea_->ComputeMultiindexPosition(eta_mapping)];
	      for(index_t d = 0; d < dim; d++) {
		diff = (data.get(d, j) - xJ_center[d]) / bandwidth_factor;
		xj_xJ *= pow(diff, eta_mapping[d]);
	      }

	      result = sign *
		sea_->get_n_multichoose_k_by_pos
                (sea_->ComputeMultiindexPosition(beta_gamma_nu_eta_mapping),
                 sea_->ComputeMultiindexPosition(beta_nu_mapping)) *
		beta_derivative * gamma_derivative * xi_xI * xj_xJ *
		moment_k * exhaustive_ij.get
		(i - node1_begin, j - node2_begin);
	      
	      if(result > 0) {
		pos_sum += result;
	      }
	      else {
		neg_sum += result;
	      }
	    }
	  }
	  
	} // end of eta
      } // end of gamma
    } // end of nu
  } // end of beta
  
  // combine negative and positive sums
  sum = neg_sum + pos_sum;
  return sum;
}

template<typename TKernelAux>
double FarFieldExpansion<TKernelAux>::ConvolveField
(const FarFieldExpansion &fe2, const FarFieldExpansion &fe3,
 int order1, int order2, int order3) const {
  
  // bandwidth factor and multiindex mapping stuffs
  double result;
  double bandwidth_factor = ka_->BandwidthFactor(bandwidth_sq());
  const ArrayList<int> *multiindex_mapping = sea_->get_multiindex_mapping();
  const ArrayList<int> *lower_mapping_index = sea_->get_lower_mapping_index();

  // get the total number of coefficients and coefficients
  int total_num_coeffs1 = sea_->get_total_num_coeffs(order1);
  int total_num_coeffs2 = sea_->get_total_num_coeffs(order2);
  int total_num_coeffs3 = sea_->get_total_num_coeffs(order3);
  int dim = sea_->get_dimension();
  Vector coeffs2, coeffs3;
  coeffs2.Alias(fe2.get_coeffs());
  coeffs3.Alias(fe3.get_coeffs());

  // actual accumulated sum
  double neg_sum = 0;
  double pos_sum = 0;
  double sum = 0;
  
  // some temporary
  double moment_i, moment_j, moment_k;

  // temporary array
  ArrayList<int> mu_nu_mapping;
  ArrayList<int> alpha_mu_eta_mapping;
  ArrayList<int> beta_gamma_nu_eta_mapping;
  ArrayList<int> alpha_mu_mapping;
  ArrayList<int> beta_nu_mapping;
  ArrayList<int> gamma_eta_mapping;
  alpha_mu_mapping.Init(dim);
  beta_nu_mapping.Init(dim);
  gamma_eta_mapping.Init(dim);
  mu_nu_mapping.Init(dim);
  alpha_mu_eta_mapping.Init(dim);
  beta_gamma_nu_eta_mapping.Init(dim);

  // partial derivatives table
  Matrix derivative_map_alpha;
  derivative_map_alpha.Init(dim, order1 + 1);
  Matrix derivative_map_beta;
  derivative_map_beta.Init(dim, order2 + 1);
  Matrix derivative_map_gamma;
  derivative_map_gamma.Init(dim, order3 + 1);
  
  // compute center differences and complete the table of partial derivatives
  Vector xI_xJ, xI_xK, xJ_xK;
  xI_xJ.Init(dim);
  xI_xK.Init(dim);
  xJ_xK.Init(dim);
  Vector xJ_center, xK_center;
  xJ_center.Alias(*(fe2.get_center()));
  xK_center.Alias(*(fe3.get_center()));

  for(index_t d = 0; d < dim; d++) {
    xI_xJ[d] = (center_[d] - xJ_center[d]) / bandwidth_factor;
    xI_xK[d] = (center_[d] - xK_center[d]) / bandwidth_factor;
    xJ_xK[d] = (xJ_center[d] - xK_center[d]) / bandwidth_factor;
  }
  ka_->ComputeDirectionalDerivatives(xI_xJ, derivative_map_alpha);
  ka_->ComputeDirectionalDerivatives(xI_xK, derivative_map_beta);
  ka_->ComputeDirectionalDerivatives(xJ_xK, derivative_map_gamma);

  // inverse factorials
  Vector inv_multiindex_factorials;
  inv_multiindex_factorials.Alias(sea_->get_inv_multiindex_factorials());

  // main loop
  for(index_t alpha = 0; alpha < total_num_coeffs1; alpha++) {

    ArrayList <int> alpha_mapping = multiindex_mapping[alpha];
    ArrayList <int> lower_mappings_for_alpha = lower_mapping_index[alpha];
    double alpha_derivative = ka_->ComputePartialDerivative
      (derivative_map_alpha, alpha_mapping);

    for(index_t mu = 0; mu < lower_mappings_for_alpha.size(); mu++) {

      ArrayList <int> mu_mapping = 
	multiindex_mapping[lower_mappings_for_alpha[mu]];

      // alpha - mu
      for(index_t d = 0; d < dim; d++) {
	alpha_mu_mapping[d] = alpha_mapping[d] - mu_mapping[d];
      }
      
      for(index_t beta = 0; beta < total_num_coeffs2; beta++) {
	
	ArrayList <int> beta_mapping = multiindex_mapping[beta];
	ArrayList <int> lower_mappings_for_beta = lower_mapping_index[beta];
	double beta_derivative = ka_->ComputePartialDerivative
	  (derivative_map_beta, beta_mapping);

	for(index_t nu = 0; nu < lower_mappings_for_beta.size(); nu++) {
	  
	  ArrayList<int> nu_mapping = 
	    multiindex_mapping[lower_mappings_for_beta[nu]];

	  // mu + nu and beta - nu
	  for(index_t d = 0; d < dim; d++) {
	    mu_nu_mapping[d] = mu_mapping[d] + nu_mapping[d];
	    beta_nu_mapping[d] = beta_mapping[d] - nu_mapping[d];
	  }

	  for(index_t gamma = 0; gamma < total_num_coeffs3; gamma++) {
	    
	    ArrayList <int> gamma_mapping = multiindex_mapping[gamma];
	    ArrayList <int> lower_mappings_for_gamma = 
	      lower_mapping_index[gamma];
	    double gamma_derivative = ka_->ComputePartialDerivative
	      (derivative_map_gamma, gamma_mapping);

	    for(index_t eta = 0; eta < lower_mappings_for_gamma.size(); 
		eta++){
	      
	      // add up alpha, mu, eta and beta, gamma, nu, eta
	      int sign = 0;
	      
	      ArrayList<int> eta_mapping =
		multiindex_mapping[lower_mappings_for_gamma[eta]];

	      for(index_t d = 0; d < dim; d++) {
		alpha_mu_eta_mapping[d] = alpha_mapping[d] - mu_mapping[d] +
		  eta_mapping[d];
		beta_gamma_nu_eta_mapping[d] = beta_mapping[d] +
		  gamma_mapping[d] - nu_mapping[d] - eta_mapping[d];
		gamma_eta_mapping[d] = gamma_mapping[d] - eta_mapping[d];
		
		sign += 2 * (alpha_mapping[d] + beta_mapping[d] + 
			     gamma_mapping[d]) - mu_mapping[d] - 
		  nu_mapping[d] - eta_mapping[d];
	      }
	      if(sign % 2 == 1) {
		sign = -1;
	      }
	      else {
		sign = 1;
	      }
	      
	      // retrieve moments for appropriate multiindex maps
	      moment_i = 
		coeffs_[sea_->ComputeMultiindexPosition(mu_nu_mapping)];
	      moment_j = 
		coeffs2[sea_->ComputeMultiindexPosition
			(alpha_mu_eta_mapping)];
	      moment_k = 
		coeffs3[sea_->ComputeMultiindexPosition
			(beta_gamma_nu_eta_mapping)];

	      result = sign * 
		sea_->get_n_multichoose_k_by_pos
		(sea_->ComputeMultiindexPosition(mu_nu_mapping),
		 sea_->ComputeMultiindexPosition(mu_mapping)) *
		sea_->get_n_multichoose_k_by_pos
		(sea_->ComputeMultiindexPosition(alpha_mu_eta_mapping),
		 sea_->ComputeMultiindexPosition(eta_mapping)) *
		sea_->get_n_multichoose_k_by_pos
		(sea_->ComputeMultiindexPosition(beta_gamma_nu_eta_mapping),
		 sea_->ComputeMultiindexPosition(beta_nu_mapping)) *
		alpha_derivative * beta_derivative * gamma_derivative * 
		moment_i * moment_j * moment_k;

	      if(result > 0) {
		pos_sum += result;
	      }
	      else {
		neg_sum += result;
	      }

	    } // end of eta
	  } // end of gamma
	} // end of nu
      } // end of beta
    } // end of mu
  } // end of alpha
  
  // combine negative and positive sums
  sum = neg_sum + pos_sum;
  return sum;
}

template<typename TKernelAux>
void FarFieldExpansion<TKernelAux>::Init(const Vector& center, 
					 const TKernelAux &ka) {
  
  // copy kernel type, center, and bandwidth squared
  kernel_ = &(ka.kernel_);
  center_.Copy(center);
  order_ = -1;
  sea_ = &(ka.sea_);
  ka_ = &ka;

  // initialize coefficient array
  coeffs_.Init(sea_->get_max_total_num_coeffs());
  coeffs_.SetZero();
}

template<typename TKernelAux>
void FarFieldExpansion<TKernelAux>::Init(const TKernelAux &ka) {
  
  // copy kernel type, center, and bandwidth squared
  kernel_ = &(ka.kernel_);  
  order_ = -1;
  sea_ = &(ka.sea_);
  center_.Init(sea_->get_dimension());
  center_.SetZero();
  ka_ = &ka;

  // initialize coefficient array
  coeffs_.Init(sea_->get_max_total_num_coeffs());
  coeffs_.SetZero();
}

template<typename TKernelAux>
int FarFieldExpansion<TKernelAux>::OrderForEvaluating
(const DHrectBound<2> &far_field_region, 
 const DHrectBound<2> &local_field_region, double min_dist_sqd_regions,
 double max_dist_sqd_regions, double max_error, double *actual_error) const {
  
  return ka_->OrderForEvaluatingFarField(far_field_region,
					local_field_region,
					min_dist_sqd_regions, 
					max_dist_sqd_regions, max_error,
					actual_error);
}

template<typename TKernelAux>
int FarFieldExpansion<TKernelAux>::
OrderForConvertingToLocal(const DHrectBound<2> &far_field_region,
			  const DHrectBound<2> &local_field_region, 
			  double min_dist_sqd_regions, 
			  double max_dist_sqd_regions,
			  double max_error, 
			  double *actual_error) const {
  
  return ka_->OrderForConvertingFromFarFieldToLocal(far_field_region,
						    local_field_region,
						    min_dist_sqd_regions,
						    max_dist_sqd_regions,
						    max_error, actual_error);
}

template<typename TKernelAux>
void FarFieldExpansion<TKernelAux>::PrintDebug(const char *name, 
					       FILE *stream) const {
  
    
  int dim = sea_->get_dimension();
  int total_num_coeffs = sea_->get_total_num_coeffs(order_);

  fprintf(stream, "----- SERIESEXPANSION %s ------\n", name);
  fprintf(stream, "Far field expansion\n");
  fprintf(stream, "Center: ");
  
  for (index_t i = 0; i < center_.length(); i++) {
    fprintf(stream, "%g ", center_[i]);
  }
  fprintf(stream, "\n");
  
  fprintf(stream, "f(");
  for(index_t d = 0; d < dim; d++) {
    fprintf(stream, "x_q%d", d);
    if(d < dim - 1)
      fprintf(stream, ",");
  }
  fprintf(stream, ") = \\sum\\limits_{x_r \\in R} K(||x_q - x_r||) = ");
  
  for (index_t i = 0; i < total_num_coeffs; i++) {
    ArrayList<int> mapping = sea_->get_multiindex(i);
    fprintf(stream, "%g ", coeffs_[i]);
    
    fprintf(stream, "(-1)^(");
    for(index_t d = 0; d < dim; d++) {
      fprintf(stream, "%d", mapping[d]);
      if(d < dim - 1)
	fprintf(stream, " + ");
    }
    fprintf(stream, ") D^((");
    for(index_t d = 0; d < dim; d++) {
      fprintf(stream, "%d", mapping[d]);
      
      if(d < dim - 1)
	fprintf(stream, ",");
    }
    fprintf(stream, ")) f(x_q - x_R)");
    if(i < total_num_coeffs - 1) {
      fprintf(stream, " + ");
    }
  }
  fprintf(stream, "\n");
}

template<typename TKernelAux>
void FarFieldExpansion<TKernelAux>::TranslateFromFarField
(const FarFieldExpansion &se) {
  
  double bandwidth_factor = ka_->BandwidthFactor(se.bandwidth_sq());
  int dim = sea_->get_dimension();
  int order = se.get_order();
  int total_num_coeffs = sea_->get_total_num_coeffs(order);
  Vector prev_coeffs;
  Vector prev_center;
  const ArrayList < int > *multiindex_mapping = sea_->get_multiindex_mapping();
  const ArrayList < int > *lower_mapping_index = 
    sea_->get_lower_mapping_index();

  ArrayList <int> tmp_storage;
  Vector center_diff;
  Vector inv_multiindex_factorials;

  center_diff.Init(dim);

  // retrieve coefficients to be translated and helper mappings
  prev_coeffs.Alias(se.get_coeffs());
  prev_center.Alias(*(se.get_center()));
  tmp_storage.Init(sea_->get_dimension());
  inv_multiindex_factorials.Alias(sea_->get_inv_multiindex_factorials());

  // no coefficients can be translated
  if(order == -1)
    return;
  else
    order_ = order;
  
  // compute center difference
  for(index_t j = 0; j < dim; j++) {
    center_diff[j] = prev_center[j] - center_[j];
  }

  for(index_t j = 0; j < total_num_coeffs; j++) {
    
    ArrayList <int> gamma_mapping = multiindex_mapping[j];
    ArrayList <int> lower_mappings_for_gamma = lower_mapping_index[j];
    double pos_coeff = 0;
    double neg_coeff = 0;

    for(index_t k = 0; k < lower_mappings_for_gamma.size(); k++) {

      ArrayList <int> inner_mapping = 
	multiindex_mapping[lower_mappings_for_gamma[k]];

      int flag = 0;
      double diff1;
      
      // compute gamma minus alpha
      for(index_t l = 0; l < dim; l++) {
	tmp_storage[l] = gamma_mapping[l] - inner_mapping[l];

	if(tmp_storage[l] < 0) {
	  flag = 1;
	  break;
	}
      }
      
      if(flag) {
	continue;
      }
      
      diff1 = 1.0;
      
      for(index_t l = 0; l < dim; l++) {
	diff1 *= pow(center_diff[l] / bandwidth_factor, tmp_storage[l]);
      }

      double prod = prev_coeffs[lower_mappings_for_gamma[k]] * diff1 * 
	inv_multiindex_factorials
	[sea_->ComputeMultiindexPosition(tmp_storage)];
      
      if(prod > 0) {
	pos_coeff += prod;
      }
      else {
	neg_coeff += prod;
      }

    } // end of k-loop
    
    coeffs_[j] += pos_coeff + neg_coeff;

  } // end of j-loop
}

template<typename TKernelAux>
void FarFieldExpansion<TKernelAux>::TranslateToLocal
(LocalExpansion<TKernelAux> &se, int truncation_order) {
  
  Vector pos_arrtmp, neg_arrtmp;
  Matrix derivative_map;
  Vector local_center;
  Vector cent_diff;
  Vector local_coeffs;
  int local_order = se.get_order();
  int dimension = sea_->get_dimension();
  int total_num_coeffs = sea_->get_total_num_coeffs(truncation_order);
  int limit;
  double bandwidth_factor = ka_->BandwidthFactor(se.bandwidth_sq());

  // get center and coefficients for local expansion
  local_center.Alias(*(se.get_center()));
  local_coeffs.Alias(se.get_coeffs());
  cent_diff.Init(dimension);

  // if the order of the far field expansion is greater than the
  // local one we are adding onto, then increase the order.
  if(local_order < truncation_order) {
    se.set_order(truncation_order);
  }

  // compute Gaussian derivative
  limit = 2 * truncation_order + 1;
  derivative_map.Init(dimension, limit);
  pos_arrtmp.Init(total_num_coeffs);
  neg_arrtmp.Init(total_num_coeffs);

  // compute center difference divided by bw_times_sqrt_two;
  for(index_t j = 0; j < dimension; j++) {
    cent_diff[j] = (local_center[j] - center_[j]) / bandwidth_factor;
  }

  // compute required partial derivatives
  ka_->ComputeDirectionalDerivatives(cent_diff, derivative_map);
  ArrayList<int> beta_plus_alpha;
  beta_plus_alpha.Init(dimension);

  for(index_t j = 0; j < total_num_coeffs; j++) {

    ArrayList<int> beta_mapping = sea_->get_multiindex(j);
    pos_arrtmp[j] = neg_arrtmp[j] = 0;

    for(index_t k = 0; k < total_num_coeffs; k++) {

      ArrayList<int> alpha_mapping = sea_->get_multiindex(k);
      for(index_t d = 0; d < dimension; d++) {
	beta_plus_alpha[d] = beta_mapping[d] + alpha_mapping[d];
      }
      double derivative_factor =
	ka_->ComputePartialDerivative(derivative_map, beta_plus_alpha);
      
      double prod = coeffs_[k] * derivative_factor;

      if(prod > 0) {
	pos_arrtmp[j] += prod;
      }
      else {
	neg_arrtmp[j] += prod;
      }
    } // end of k-loop
  } // end of j-loop

  Vector C_k_neg = sea_->get_neg_inv_multiindex_factorials();
  for(index_t j = 0; j < total_num_coeffs; j++) {
    local_coeffs[j] += (pos_arrtmp[j] + neg_arrtmp[j]) * C_k_neg[j];
  }
}

#endif
