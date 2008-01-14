#include "fastlib/fastlib_int.h"
#include "regression.h"
#include "regression2.h"

int main (int argc, char *argv[]){

  fx_init (argc, argv);

  //const char *algorithm = fx_param_str_req (NULL, "method");
  //bool do_naive = fx_param_exists (NULL, "do_naive");

  FastKde <GaussianKernel> fast_kde;

  //This will hold the results of B^TWY
  ArrayList<Vector> fast_kde_results;

 
  Matrix query_dataset;
  Matrix reference_dataset;
 
  index_t num_query_points;
  index_t num_of_dimensions;

  Vector regression_estimates_naive;
  Vector regression_estimates;
  // Vector nwr_estimates;

  ArrayList <index_t> old_from_new_r;
  ArrayList <index_t> new_from_old_r;
  /* FILE *fp;
  FILE *gp;
  fp=fopen("fast_regression.txt","w+");
  gp=fopen("naive_regression.txt","w+");*/

  if (!strcmp (fx_param_str (NULL, "kernel", "gaussian"), "gaussian")){
    
    //    FastKde <GaussianKernel> fast_kde;
    //First lets get B^TWY
    printf("will start computing the vector..\n");
    fast_kde.Init ();

    fx_timer_start(NULL,"second_matrix");
    fast_kde.Compute (fx_param_double (NULL, "tau", 0.1));
    fx_timer_stop(NULL,"second_matrix");

      //default value is 0.1
    /*if(fx_param_exists(NULL,"fast_kde_output"))
      {
	fast_kde.PrintDebug();
	}*/
	
    
    query_dataset.Alias(fast_kde.get_query_dataset());
    reference_dataset.Alias(fast_kde.get_reference_dataset());  // Hence with this we initialize both the datasets

    //initialize fast_kde_results
    num_query_points=query_dataset.n_cols();
    num_of_dimensions=query_dataset.n_rows();
    
    fast_kde_results.Init(num_query_points);  //this initializes fast_kde results
    
    for(index_t i=0;i<num_query_points;i++)
      { 
	fast_kde_results[i].Init(num_of_dimensions+1); //Each element is now initialized
      }
 
    //get density estimates from fast kde calculations and push it into the array fast_kde_results

    //nwr_estimates.Init(num_query_points);

      for(index_t q=0;q<num_query_points;q++)
	{
	  //for each query point
	  for(index_t d=0;d<num_of_dimensions+1;d++)
	    {
	      //along each dimension
	      fast_kde_results[q][d]=fast_kde.get_density_estimates(q,d);
	    }
	  //  nwr_estimates[q]=fast_kde_results[q][0];
	}
      old_from_new_r.Copy(fast_kde.get_old_from_new_r());
      new_from_old_r.Copy(fast_kde.get_new_from_old_r());
      printf("finished vector computation.......\n");
      printf("WILL NOW START REGRESSION2......................\n");

    //Now lets get (B^TWB)^-1. This can be done by calling routines related the object Regression2 present in the file regression2.h 


      //The dataset retrieved in the previous steps is being used once again
      
      Regression2 <GaussianKernel> reg2;
      printf("going to initialization function...\n");
      reg2.Init(query_dataset,reference_dataset);
      fx_timer_start(NULL,"first_matrix");
      reg2.Compute(fx_param_double (NULL, "tau", 0.1));
      fx_timer_stop(NULL,"first_matrix");
      printf("Initializations done..\n");

      //This will hold the results of regression2 calculations
      ArrayList<Matrix> wfkde_results;
      
      wfkde_results.Copy(reg2.get_results());  //This initializes wfkde_results
      
      
      //We now have to multiply the matrix wfkde_results with fast_kde_results
      
      ArrayList<Vector> temp1;
      temp1.Init(num_query_points);  //This initializes temp1

     
      regression_estimates.Init(num_query_points);
      regression_estimates.SetZero();
     
      for(index_t q=0;q<num_query_points;q++){


	temp1[q].Init(num_of_dimensions+1); //and this initializes each element of temp1
	//printf("fast_kde_results[%d] \n",q);
	//fast_kde_results[q].PrintDebug();
	la::MulInit(wfkde_results[q],fast_kde_results[q],&temp1[q]);
       
	for(index_t i=0;i<num_of_dimensions+1;i++){

	  if(i!=0){

	    //printf("temp1[%d][%d] is %f\n",q,i,temp1[q].get(i));
	    //printf("query_dataset[%d][%d] is %f\n",i-1,q,query_dataset.get(i-1,q));
	    regression_estimates[q]+=temp1[q].get(i)*query_dataset.get(i-1,q);

	  }	

	  else{
	    //printf("temp1[%d][%d] is %f\n",q,i,temp1[q].get(i));
	    regression_estimates[q]+=temp1[q].get(i)*1;
	  }
	}

	/*fprintf(fp,"Point:");

	for(index_t d=0;d<query_dataset.n_rows();d++){

	  fprintf(fp,"%f,",query_dataset.get(d,q));
	}
	fprintf(fp,"  ",regression_estimates[q]);*/
	//printf("Hence regression estimate is %f\n",regression_estimates[q]);
      }
    
      //Fill up the enwr_estimates
      /*Vector temp;
      temp.Copy(reg2.get_denominator_nwr());

      for(index_t i=0;i<query_dataset.n_cols();i++){                 
	nwr_estimates[i]/=temp[i];
	}*/
  }

  printf("ended matrix computation........ and did multiplication too....\n");
  //Lets do naive calculations too.......

  printf("Will start naive regression....\n");

  NaiveKde <GaussianKernel> naive_kde;
  //This will hold the results of B^TWY
  ArrayList<Vector> naive_kde_results;

  if (!strcmp (fx_param_str (NULL, "kernel", "gaussian"), "gaussian")){
    

    //First lets get B^TWY

    //printf("In main function query dataset is..........\n");
    //query_dataset.PrintDebug();

    
    naive_kde.Init (query_dataset,reference_dataset,old_from_new_r);
    fx_timer_start(NULL,"second_matrix_naive");
    naive_kde.Compute ();
    fx_timer_stop(NULL,"second_matrix_naive");

    //initialize naive_kde_results
    num_query_points=query_dataset.n_cols();
    num_of_dimensions=query_dataset.n_rows();
    
    naive_kde_results.Init(num_query_points);  //this initializes naive_kde results
    
    for(index_t i=0;i<num_query_points;i++)
      { 
	naive_kde_results[i].Init(num_of_dimensions+1); //Each element is now initialized
      }
 
    //get density estimates from naive kde calculations and push it into the array naive_kde_results

      for(index_t q=0;q<num_query_points;q++)
	{
	  //for each query point
	  for(index_t d=0;d<num_of_dimensions+1;d++)
	    {
	      //along each dimension
	      naive_kde_results[q][d]=naive_kde.get_density_estimates(q,d);
	    }
	}

      printf("WILL NOW START Naive REGRESSION2......................\n");

    //Now lets get (B^TWB)^-1.


      //The dataset retrieved in the previous steps is being used once again
      
       NaiveRegression2 <GaussianKernel> naive_reg2;
       printf("going to initialization function...\n");

      
       naive_reg2.Init(query_dataset,reference_dataset);
       fx_timer_start(NULL,"first_matrix_naive");
       naive_reg2.Compute();
       fx_timer_stop(NULL,"first_matrix_naive");

       printf("Initializations done..\n");
       
       //This will hold the results of Naive regression2 calculations
       ArrayList<Matrix> naive_reg2_results;
       
       naive_reg2_results.Copy(naive_reg2.get_results());  //This initializes wfkde_results
       
       
       //We now have to multiply the matrix wfkde_results with fast_kde_results
       
       ArrayList<Vector> temp1;
       temp1.Init(num_query_points);  //This initializes temp1
       
      
       regression_estimates_naive.Init(num_query_points);
       regression_estimates_naive.SetZero();
       
       for(index_t q=0;q<num_query_points;q++){
	 
	 
	 temp1[q].Init(num_of_dimensions+1); //and this initializes each element of temp1
	
	 la::MulInit(naive_reg2_results[q],naive_kde_results[q],&temp1[q]);
	 
	 for(index_t i=0;i<num_of_dimensions+1;i++){
	   
	   if(i!=0){
	     
	     // printf("temp1[%d][%d] is %f\n",q,i,temp1[q].get(i));
	     //printf("query_dataset[%d][%d] is %f\n",i-1,q,query_dataset.get(i-1,q));
	     regression_estimates_naive[q]+=temp1[q].get(i)*query_dataset.get(i-1,q);
	     
	   }	
	   
	   else{
	     //printf("temp1[%d][%d] is %f\n",q,i,temp1[q].get(i));
	     regression_estimates_naive[q]+=temp1[q].get(i)*1;
	   }
	 }
	 /*fprintf(gp,"Point:");

	 for(index_t d=0;d<query_dataset.n_rows();d++){

	   fprintf(gp,"%f,",query_dataset.get(d,q));
	 }
	 fprintf(gp,"  ",regression_estimates_naive[q]);*/
	 
       }
       FILE *lp;
       lp=fopen("estimates_astrodataset_bw_0.3.txt","w+");
       double relative_error=0;
       double error;
       double total_error=0;
       double mean_square_error;
       double max_relative_error;
       for(index_t q=0;q<num_query_points;q++){
	 
	 //printf("Naive:%f fast estimate:%f\n",q,regression_estimates_naive[q],regression_estimates);
	 
	 error=(double)fabs(regression_estimates_naive[new_from_old_r[q]]-regression_estimates[new_from_old_r[q]]);

	 relative_error=error/regression_estimates_naive[new_from_old_r[q]];
	 total_error+=pow(error,2);
	 
	 for(index_t d=0;d<num_of_dimensions;d++){
	   fprintf(lp,"%f, ",query_dataset.get(d,new_from_old_r[q]));
	 }

	 fprintf(lp,"naive: %2f, fast:%2f  diff:%2f\n",regression_estimates_naive[new_from_old_r[q]],regression_estimates[new_from_old_r[q]],relative_error);

	 if(relative_error>max_relative_error){
	   max_relative_error=relative_error;
	 }
       }
       //mean_square_error=error/num_query_points;
       fprintf(lp,"number of query points are %d\n",num_query_points);
       fprintf(lp,"total error=%f\n",total_error);
       fprintf(lp,"Max relative error=%f\n",max_relative_error); 
          
  }
  fx_done();
}


