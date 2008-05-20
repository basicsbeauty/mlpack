
void BigSdpNmfObjective::Init(datanode *module, 
			ArrayList<index_t> &rows,
			ArrayList<index_t> &columns,
      ArrayList<double>  &values) {
  module_=module;
	rows_.Copy(rows);
	columns_.Copy(columns);
	values_.Copy(values);
	num_of_rows_=std::max_element(rows_.begin(), rows_.end())+1;
	num_of_columns_=std::max_element(columns_.begin(), columns_.end())+1;
	eq_lagrange_mult_.Init(values_.size());
}

void BigSdpNmfObjective::Destruct() {
  rows_.Renew();
	columns_.Renew();
  values_.Renew();
	eq_lagrange_mult_.Destruct();
}

void BigSdpNmfObjective::ComputeGradient(Matrix &coordinates, Matrix *gradient) {
  gradient->CopyValues(coordinates);
	la::Scale(2, gradient);
	for(index_t i=0; i<values_.size(); i++) {
    index_t w =rows_(i)*new_dim_;
    index_t h =offset_h_+cols_(i)*new_dim_;
    double diff=la::Dot(rank_*new_dim_,
				                coordinates.GetColumnPtr(w),
												coordinates.GetColumnPtr(h))-values_(i);
	  la::AddExpert(rank_*new_dim_, -eq_lagrange_mult_[i]+2*sigma_*diff,
				 gradient->GetColumnPtr(w), gradient->GetColumnPtr(h));	
		la::AddExpert(rank_*new_dim_, -eq_lagrange_mult_[i]+2*sigma_*diff, 
				gradient->GetColumnPtr(h), gradient->GetColumnPtr(w));
	}
	
}

void BigSdpNmfObjective::ComputeObjective(Matrix &coordinates, double *objective) {
  *objective=0;	
	for(index_t i=0; i<coordinates.n_cols(); i++) {
	   *objective+=la::Dot(coordinates.n_rows(), 
				 ccoordinates.GetColumnPtr(i), coordinates.GetColumnPtr(i));
	}
}

void BigSdpNmObjectivef::ComputeFeasibilityError(Matrix &coordinates, double *error) {
  *error=0;
	for(index_t i=0; i<values_.size(); i++) {
    index_t w =rows_(i)*new_dim_;
    index_t h =offset_h_+cols_(i)*new_dim_;
    double diff=la::Dot(rank_*new_dim_,
				                coordinates.GetColumnPtr(w),
												coordinates.GetColumnPtr(h))-values_(i);
		*error+=diff*diff;
   		
	}
}

double BigSdpNmfObjective::ComputeLagrangian(Matrix &coordinates) {
  double lagrangian=0;
  ComputeObjective(coordinates, &lagrangian);
  for(index_t i=0; i<values_.size(); i++) {
    index_t w =rows_(i)*new_dim_;
    index_t h =offset_h_+cols_(i)*new_dim_;
    double diff=la::Dot(rank_*new_dim_,
				                coordinates.GetColumnPtr(w),
												coordinates.GetColumnPtr(h))-values_(i);
		lagrangian+=(sigma_*diff-eq_lagrange_mult_[i])*diff;
	}
	return lagrangian;
}

void BigSdpNmfObjective::UpdateLagrangeMult(Matrix &coordinates) {
  for(index_t i=0; i<values_.size(); i++) {
    index_t w =rows_(i)*new_dim_;
    index_t h =offset_h_+cols_(i)*new_dim_;
    double diff=la::Dot(rank_*new_dim_,
				                coordinates.GetColumnPtr(w),
												coordinates.GetColumnPtr(h))-values_(i);
		eq_lagrange_mult_[i]-=sigma_*diff;
	}
}

void BigSdpNmfObjective::Project(Matrix *coordinates) {
  for(index_t i=0; i<coordinates->n_cols(); i++) {
	  if (coordinates->get(0, i)<0) {
		  coordinates->set(0, i)=0;
		}
	}
}

void BigSdpNmfObjective::set_sigma(double sigma) {
  sigma_=sigma;
} 
bool BigSdpNmfObjective::IsDiverging(double objective) {
  return false;
} 

