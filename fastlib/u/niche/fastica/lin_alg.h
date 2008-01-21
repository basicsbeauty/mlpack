#include "fastlib/fastlib.h"

/**
 * Save the matrix to a file so that rows in the matrix correspond to rows in
 * the file: This just means call data::Save() on the transpose of the matrix
 */ 
void SaveCorrectly(const char *filename, Matrix a) {
  Matrix a_transpose;
  la::TransposeInit(a, &a_transpose);
  data::Save(filename, a_transpose);
}

double ExpArg(double x, double arg) {
  return exp(x * arg);
}

double Inv(double x, double arg) {
  return 1 / x;
}

double Square(double x, double arg) {
  return x * x;
}

double SquareArg(double x, double arg) {
  return arg * x * x;
}


double TanhArg(double x, double arg) {
  return tanh(arg * x);
}

double Times(double x, double arg) {
  return arg * x;
}

double Plus(double x, double arg) {
  return x + arg;
}

double MinusArg(double x, double arg) {
  return x - arg;
}

double ArgMinus(double x, double arg) {
  return arg - x;
}

Matrix* DiagMatrixInit(index_t n, double value, Matrix *diag_matrix) {
  diag_matrix -> Init(n, n);
  diag_matrix -> SetZero();
  for(index_t i = 0; i < n; i++) {
    diag_matrix -> set(i, i, value);
  }

  return diag_matrix;
}

Matrix* ColVector(index_t n, double value, Matrix *col_vector) {
  col_vector -> Init(n, 1);
  col_vector -> SetAll(value);
  
  return col_vector;
}


Matrix* Sum(const Matrix* const A, Matrix *sum_vector) {
  index_t n_rows = A -> n_rows();
  index_t n_cols = A -> n_cols();

  sum_vector -> Init(1, n_cols);

  const double *A_col_j;
  for(index_t j = 0; j < n_cols; j++) {
    A_col_j = A -> GetColumnPtr(j);
    double sum = 0;
    for(index_t i = 0; i < n_rows; i++) {
      sum += A_col_j[i];
    }
    (*sum_vector).set(0, j, sum);
  }

  return sum_vector;
}


double Sum(Vector *v) {
  index_t n = v -> length();

  double sum = 0;
  for(index_t i = 0; i < n; i++) {
    sum += (*v)[i];
  }

  return sum;
}


Matrix* MatrixMapSum(double (*function)(double,double),
		     double arg,
		     const Matrix* const A,
		     Matrix *sum_vector) {
  index_t n_rows = A -> n_rows();
  index_t n_cols = A -> n_cols();

  sum_vector -> Init(1, n_cols);

  const double *A_col_j;
  for(index_t j = 0; j < n_cols; j++) {
    A_col_j = A -> GetColumnPtr(j);
    double sum = 0;
    for(index_t i = 0; i < n_rows; i++) {
      sum += function(A_col_j[i], arg);
    }
    (*sum_vector).set(0, j, sum);
  }

  return sum_vector;
}


double VectorMapSum(double (*function)(double,double),
		    double arg,
		    const Vector* const v) {
  index_t n = v -> length();

  double sum = 0;
  for(index_t i = 0; i < n; i++) {
    sum += function((*v)[i], arg);
  }
  
  return sum;
}


  

// we assume that the dimensions of A and B are equal, if not, oops!!
Matrix* DotMultiplyInit(const Matrix* const A, const Matrix* const B, Matrix* C) {
  index_t n_rows = A -> n_rows();
  index_t n_cols = A -> n_cols();

  C -> Init(n_rows, n_cols);

  const double *A_col_j;
  const double *B_col_j;
  double *C_col_j;
  for(index_t j = 0; j < n_cols; j++) {
    A_col_j = A -> GetColumnPtr(j);
    B_col_j = B -> GetColumnPtr(j);
    C_col_j = C -> GetColumnPtr(j);
    for(index_t i = 0; i < n_rows; i++) {
      C_col_j[i] = A_col_j[i] * B_col_j[i];
    }
  }

  return C;
}


// we assume that the dimensions of A and B are equal, if not, oops!!
Matrix* DotMultiplyOverwrite(const Matrix* const A, Matrix* const B) {
  index_t n_rows = A -> n_rows();
  index_t n_cols = A -> n_cols();

  const double *A_col_j;
  double *B_col_j;
  for(index_t j = 0; j < n_cols; j++) {
    A_col_j = A -> GetColumnPtr(j);
    B_col_j = B -> GetColumnPtr(j);
    for(index_t i = 0; i < n_rows; i++) {
      B_col_j[i] *= A_col_j[i];
    }
  }

  return B;
}


Vector* DotMultiplyInit(const Vector* const u, const Vector* const v,
			Vector* w) {
  index_t n = u -> length();
  
  (*w).Init(n);

  for(index_t i = 0; i < n; i++) {
    (*w)[i] = (*u)[i] * (*v)[i];
  }

  return w;
}


Vector* DotMultiplyOverwrite(const Vector* const u, Vector* const v) {
  index_t n = u -> length();
  
  for(index_t i = 0; i < n; i++) {
    (*v)[i] *= (*u)[i];
  }

  return v;
}



Matrix* DotMultiplySum(const Matrix* const A, const Matrix* const B, Matrix* sum_vector) {
  index_t n_rows = A -> n_rows();
  index_t n_cols = A -> n_cols();

  sum_vector -> Init(1, n_cols);

  const double *A_col_j;
  const double *B_col_j;
  for(index_t j = 0; j < n_cols; j++) {
    A_col_j = A -> GetColumnPtr(j);
    B_col_j = B -> GetColumnPtr(j);
    double sum = 0;
    for(index_t i = 0; i < n_rows; i++) {
      sum += A_col_j[i] * B_col_j[i];
    }
    (*sum_vector).set(0, j, sum);
  }

  return sum_vector;
}
  


Matrix* VectorToDiag(const Matrix* const diag_vector, Matrix* diag_matrix) {

  index_t n = diag_vector -> n_cols();
  diag_matrix -> Init(n, n);
  diag_matrix -> SetZero();

  for(index_t i = 0; i < n; i++) {
    diag_matrix -> set(i, i, diag_vector -> get(0, i));
  }

  return diag_matrix;
}



Matrix* VectorToDiag(const Vector* const diag_vector, Matrix* diag_matrix) {

  diag_matrix -> InitDiagonal(*diag_vector);

  return diag_matrix;
}


// only valid for square matrices; we don't check the square condition, so don't pass in non-square matrices!
Vector* DiagToVector(const Matrix* const diag_matrix, Vector* diag_vector) {

  index_t n = diag_matrix -> n_rows();

  diag_vector -> Init(n);

  for(index_t i = 0; i < n; i++) {
    (*diag_vector)[i] = diag_matrix -> get(i, i);
  }

  return diag_vector;
}



Matrix* Scale(double alpha, Matrix *A) {
  la::Scale(alpha, A);

  return A;
}

Vector* Scale(double alpha, Vector* v) {
  la::Scale(alpha, v);

  return v;
}

Vector* ScaleInit(double alpha, const Vector* const u, Vector* v) {
  la::ScaleInit(alpha, *u, v);

  return v;
}


 

Matrix* MulInit(const Matrix* const A, const Matrix* const B,
		Matrix* const C) {
  la::MulInit(*A, *B, C);

  return C;
}

Vector* MulInit(const Matrix* const A, const Vector* const u,
		Vector* const v) {
  la::MulInit(*A, *u, v);

  return v;
}

Vector* MulInit(const Vector* const u, const Matrix* const A,
		Vector* const v) {
  la::MulInit(*u, *A, v);

  return v;
}



Matrix* MulOverwrite(const Matrix* const A, const Matrix* const B,
		     Matrix* const C) {
  la::MulOverwrite(*A, *B, C);

  return C;
}


Matrix* MulTransAInit(const Matrix* const A, const Matrix* const B,
		      Matrix* C) {
  la::MulTransAInit(*A, *B, C);

  return C;
}


Matrix* MulTransAOverwrite(const Matrix* const A, const Matrix* const B,
			   Matrix* const C) {
  la::MulTransAOverwrite(*A, *B, C);

  return C;
}


Matrix* MulTransBInit(const Matrix* const A, const Matrix* const B,
		      Matrix* C) {
  la::MulTransBInit(*A, *B, C);

  return C;
}

Matrix* MulTransBOverwrite(const Matrix* const A, Matrix* const B,
			   Matrix* const C) {
  la::MulTransBOverwrite(*A, *B, C);

  return C;
}



Matrix* SubInit(const Matrix* const A, const Matrix* const B, Matrix* C) {
  la::SubInit(*B, *A, C);

  return C;
}

Vector* SubInit(const Vector* const u, const Vector* const v, Vector* w) {
  la::SubInit(*v, *u, w);

  return w;
}


Matrix* SubOverwrite(const Matrix* const A, const Matrix* const B, Matrix* const C) {
  la::SubOverwrite(*B, *A, C);

  return C;
}


Matrix* SubFrom(const Matrix* const A, Matrix* const B) {
  la::SubFrom(*A, B);

  return B;
}

Vector* SubFrom(const Vector* const u, Vector* const v) {
  la::SubFrom(*u, v);

  return v;
}



Matrix* AddTo(const Matrix* const A, Matrix* const B) {
  la::AddTo(*A, B);

  return B;
}

Vector* AddTo(const Vector* const u, Vector* const v) {
  la::AddTo(*u, v);

  return v;
}


Matrix* AddExpert(double alpha, const Matrix* const A, Matrix* const B) {
  la::AddExpert(alpha, *A, B);

  return B;
}

Vector* AddExpert(double alpha, const Vector* const u, Vector* const v) {
  la::AddExpert(alpha, *u, v);

  return v;
}




Matrix* MapOverwrite(double (*function)(double,double),
		     double arg,
		     Matrix *A) {
  index_t n_rows = A -> n_rows();
  index_t n_cols = A -> n_cols();

  double *A_col_j;
  for(index_t j = 0; j < n_cols; j++) {
    A_col_j = A -> GetColumnPtr(j);
    for(index_t i = 0; i < n_rows; i++) {
      A_col_j[i] = function(A_col_j[i], arg);
    }
  }

  return A;
}

Vector* MapOverwrite(double (*function)(double,double),
		     double arg,
		     Vector* const v) {
  index_t n = v -> length();

  for(index_t i = 0; i < n; i++) {
    (*v)[i] = function((*v)[i], arg);
  }

  return v;
}


Matrix* MapInit(double (*function)(double,double),
		double arg,
		const Matrix* const A,
		Matrix *B) {
  index_t n_rows = A -> n_rows();
  index_t n_cols = A -> n_cols();

  B -> Init(n_rows, n_cols);

  const double *A_col_j;
  double *B_col_j;
  for(index_t j = 0; j < n_cols; j++) {
    A_col_j = A -> GetColumnPtr(j);
    B_col_j = B -> GetColumnPtr(j);
    for(index_t i = 0; i < n_rows; i++) {
      B_col_j[i] = function(A_col_j[i], arg);
    }
  }

  return B;
}


Vector* MapInit(double (*function)(double,double),
		double arg,
		const Vector* const v,
		Vector *w) {
  index_t n = v -> length();
  w -> Init(n);
  
  for(index_t i = 0; i < n; i++) {
    (*w)[i] = function((*v)[i], arg);
  }

  return w;
}






void RandMatrix(index_t n_rows, index_t n_cols, Matrix *A) {
  A -> Init(n_rows, n_cols);

  for(index_t j = 0; j < n_cols; j++) {
    for(index_t i = 0; i < n_rows; i++) {
      A -> set(i, j, drand48());
    }
  }
}

void MakeSubMatrixByColumns(Vector column_indices, Matrix A, Matrix *A_sub) {
  
  index_t num_selected = column_indices.length();

  A_sub -> Init(A.n_rows(), num_selected);
  
  for(index_t i = 0; i < num_selected; i++) {
    index_t index = (index_t) column_indices[i];
    Vector A_col_index_i, A_sub_col_i;
    A.MakeColumnVector(index, &A_col_index_i);
    A_sub -> MakeColumnVector(i, &A_sub_col_i);
    A_sub_col_i.CopyValues(A_col_index_i);
  }
}



void Center(Matrix X, Matrix* X_centered) {
  Vector col_vector_sum;
  col_vector_sum.Init(X.n_rows());
  col_vector_sum.SetZero();
  
  index_t n = X.n_cols();
 
  for(index_t i = 0; i < n; i++) {
    Vector cur_col_vector;
    X.MakeColumnVector(i, &cur_col_vector);
    la::AddTo(cur_col_vector, &col_vector_sum);
  }

  la::Scale(1/(double) n, &col_vector_sum);

  X_centered -> Copy(X);

  for(index_t i = 0; i < n; i++) {
    Vector cur_col_vector;
    X_centered -> MakeColumnVector(i, &cur_col_vector);
    la::SubFrom(col_vector_sum, &cur_col_vector);
  }

}


void WhitenUsingSVD(Matrix X, Matrix* X_whitened, Matrix* whitening_matrix) {
  
  Matrix cov_X, U, VT, inv_S_matrix, temp1;
  Vector S_vector;
  
  Scale(1 / (double) (X.n_cols() - 1),
	MulTransBInit(&X, &X, &cov_X));
  
  la::SVDInit(cov_X, &S_vector, &U, &VT);
  
  index_t d = S_vector.length();
  inv_S_matrix.Init(d, d);
  inv_S_matrix.SetZero();
  for(index_t i = 0; i < d; i++) {
    double inv_sqrt_val = 1 / sqrt(S_vector[i]);
    inv_S_matrix.set(i, i, inv_sqrt_val);
  }

  cov_X.PrintDebug("cov(X')");
  U.PrintDebug("U");
  VT.PrintDebug("VT");
  inv_S_matrix.PrintDebug("S^-.5");
  
  MulTransBInit(MulTransAInit(&VT, &inv_S_matrix, &temp1),
		&U,
		whitening_matrix);
  
  MulInit(whitening_matrix, &X, X_whitened);
  
}


void WhitenUsingEig(Matrix X, Matrix* X_whitened, Matrix* whitening_matrix, Matrix* dewhitening_matrix) {
  Matrix cov_X, D, D_inv, E;
  Vector D_vector;

  Scale(1 / (double) (X.n_cols() - 1),
	MulTransBInit(&X, &X, &cov_X));
    

  la::EigenvectorsInit(cov_X, &D_vector, &E);

  //E.set(0, 1, -E.get(0, 1));
  //E.set(1, 1, -E.get(1, 1));

    

  index_t d = D_vector.length();
  D.Init(d, d);
  D.SetZero();
  D_inv.Init(d, d);
  D_inv.SetZero();
  for(index_t i = 0; i < d; i++) {
    double sqrt_val = sqrt(D_vector[i]);
    D.set(i, i, sqrt_val);
    D_inv.set(i, i, 1 / sqrt_val);
  }

  la::MulTransBInit(D_inv, E, whitening_matrix);
  la::MulInit(E, D, dewhitening_matrix);
  la::MulInit(*whitening_matrix, X, X_whitened);
}



void RandVector(Vector &v) {
  
  index_t d = v.length();
  v.SetZero();
  
  for(index_t i = 0; i+1 < d; i+=2) {
    double a = drand48();
    double b = drand48();
    double first_term = sqrt(-2 * log(a));
    double second_term = 2 * M_PI * b;
    v[i] =   first_term * cos(second_term);
    v[i+1] = first_term * sin(second_term);
  }
  
  if((d % 2) == 1) {
    v[d - 1] = sqrt(-2 * log(drand48())) * cos(2 * M_PI * drand48());
  }
  
  la::Scale(1/sqrt(la::Dot(v, v)), &v);
  
}


Matrix* RandNormalInit(index_t d, index_t n, Matrix* A) {

  double* A_elements = A -> ptr();

  index_t num_elements = d * n;

  for(index_t i = 0; i+1 < num_elements; i+=2) {
    double a = drand48();
    double b = drand48();
    double first_term = sqrt(-2 * log(a));
    double second_term = 2 * M_PI * b;
    A_elements[i] =   first_term * cos(second_term);
    A_elements[i+1] = first_term * sin(second_term);
  }
  
  if((d % 2) == 1) {
    A_elements[d - 1] = sqrt(-2 * log(drand48())) * cos(2 * M_PI * drand48());
  }

  return A;
}

Matrix* RepeatMatrix(index_t num_row_reps, index_t num_col_reps,
		     Matrix base_matrix, Matrix* new_matrix) {

  index_t num_rows = base_matrix.n_rows();
  index_t num_cols = base_matrix.n_cols();

  new_matrix -> Init(num_rows * num_row_reps, num_cols * num_col_reps);
  
  double* base_elements;
  double* new_elements = new_matrix -> ptr();
    
  for(index_t col_rep = 0; col_rep < num_col_reps; col_rep++) {
    base_elements = base_matrix.ptr();
    for(index_t col_num = 0; col_num < num_cols; col_num++) {
      for(index_t row_rep = 0; row_rep < num_row_reps; row_rep++) {
	memcpy(new_elements, base_elements, num_rows * sizeof(double));
	new_elements += num_rows;
      }
      base_elements += num_rows;
    }
  }

  return new_matrix;
}



/*
  int main(int argc, char *argv[]) {*/
//fx_init(argc, argv);
/*  
    Matrix A, B, C, D, E, F;

    RandMatrix(5, 2, &A);
    RandMatrix(2, 4, &B);
    RandMatrix(5, 4, &C);
    RandMatrix(7, 5, &D);
    RandMatrix(7, 4, &E);
    RandMatrix(7, 4, &F);

    SaveCorrectly("A.dat", A);
    SaveCorrectly("B.dat", B);
    SaveCorrectly("C.dat", C);
    SaveCorrectly("D.dat", D);
    SaveCorrectly("E.dat", E);


    Matrix temp1, temp2, temp3, temp4, temp5;
  
    Vector sum_vector;
    Matrix diag_matrix;

    VectorToDiag(MatrixSum(DotMultiplyInit(MapOverwrite(&TimesTen, Sub(Mul(&D, Sub(Mul(&A, &B, &temp1), &C, &temp2), &temp3), &E, &temp4)), &F, &temp5), &sum_vector), &diag_matrix);

    Vector diag_vector;
    DiagToVector(&diag_matrix, &diag_vector);

    la::Scale(2, Scale(100, &diag_matrix));
						
  
    A.PrintDebug("A");
    B.PrintDebug("B");
    C.PrintDebug("C");
    D.PrintDebug("D");
    E.PrintDebug("E");
    F.PrintDebug("F");

    temp4.PrintDebug("temp4");
    temp5.PrintDebug("temp5");

    sum_vector.PrintDebug("sum(temp5)");

    diag_matrix.PrintDebug("diag_matrix");
    diag_vector.PrintDebug("diag_vector");


    Matrix Z, sub_Z;
    RandMatrix(4, 5, &Z);
    Vector indices;
    indices.Init(3);
    indices[0] = 1;
    indices[1] = 0;
    indices[2] = 3;
    MakeSubMatrixByColumns(indices, Z, &sub_Z);

    Z.PrintDebug("Z");
    sub_Z.PrintDebug("sub_Z");

*/
/*
  index_t n = 10;
  index_t d = 2;

  Matrix X, B;
  RandMatrix(d, n, &X);
  RandMatrix(d, d, &B);

  Matrix hyp_tan;
  MapOverwrite(&TanhArg, A1, MulTransAInit(&X, &B, &hyp_tan));

  Matrix ones;
  ColVector(d, 1, &ones);

  Matrix sum, temp1, temp2;
  AddOverwrite(
  Scale(1 / (double) n, MulInit(&X, &hyp_tan, &temp1)),
  DotMultiplyOverwrite(MulInit(&ones, Scale(A1 / (double) n, MapOverwrite(&MinusArg, A1, Sum(MapOverwrite(&Square, 0, &hyp_tan), &sum))), &temp2), &B));
	     

  B.PrintDebug("B");




  //fx_done();

  return 0;

  }
*/
