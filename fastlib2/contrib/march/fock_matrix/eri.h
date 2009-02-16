#ifndef ERI_H
#define ERI_H

#include "fastlib/fastlib.h"
#include "basis_shell.h"
#include "shell_pair.h"

namespace eri {

  // The erf-like function
  double F_0_(double z);
  
  double ComputeGPTCenter(Vector& A_vec, double alpha_A, Vector& B_vec, 
                          double alpha_B, Vector* p_vec);
  
  // An ERI between four s-type gaussians with arbitrary bandwidth
  // This function does not currently normalize the gaussians
  double SSSSIntegral(double alpha_A,  Vector& A_vec, double alpha_B, 
                       Vector& B_vec, double alpha_C,  Vector& C_vec, 
                      double alpha_D,  Vector& D_vec);
                      
  // Compute the integral for general basis functions
  // I should add a shell version of this to take advantage of shell symmetry
/*  double ComputeIntegral(const BasisFunction& mu_fun, 
                         const BasisFunction& nu_fun, 
                         const BasisFunction& rho_fun, 
                         const BasisFunction& sigma_fun);
  */
                        
  // These won't really return doubles, they'll return lists of doubles
  double ComputeShellIntegrals(BasisShell& mu_fun, 
                               BasisShell& nu_fun, 
                               BasisShell& rho_fun, 
                               BasisShell& sigma_fun);
                               
  double ComputeShellIntegrals(ShellPair& AB_shell, 
                               ShellPair& CD_shell);
                               
  /**
   * Computes the Schwartz factor Q_{i j} = (i j|i j)^1/2
   */
  double SchwartzBound(BasisShell& i_shell, BasisShell& j_shell);
                            
  /**
   * Compute the list of significant shell pairs.
   *
   * Currently, shell pairs are screened by the size of their Schwartz factor, 
   * but some implementations may use overlap screening.   
   */
  index_t ComputeShellPairs(ArrayList<ShellPair>* shell_pairs, 
                            ArrayList<BasisShell>& shells_in, 
                            double shell_pair_cutoff);

  /**
   * Compute the list of significant shell pairs.  shell_max[i] is the 
   * largest Schwartz prescreening estimate for shell i.  This is used in the 
   * LinK algorithm. 
   *
   * Currently, shell pairs are screened by the size of their Schwartz factor, 
   * but some implementations may use overlap screening.   
   */
  index_t ComputeShellPairs(ArrayList<ShellPair>* shell_pairs, 
                            ArrayList<BasisShell>& shells_in, 
                            double shell_pair_cutoff, Vector* shell_max);
  

}

#endif
