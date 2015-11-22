/**
 * @file MCMC.h
 * @author  Scott Rasmussen (scott.rasmussen@zaita.com)
 * @date 8/05/2013
 * @section LICENSE
 *
 * Copyright NIWA Science �2013 - www.niwa.co.nz
 *
 * @section DESCRIPTION
 *
 * Markov Chain Monte Carlo
 */

#ifndef MCMC_H_
#define MCMC_H_

// headers
#include <boost/numeric/ublas/matrix.hpp>

#include "BaseClasses/Object.h"

// namespaces
namespace niwa {
namespace ublas = boost::numeric::ublas;

class Minimiser;
class Model;

/**
 * Struct definition for a chain link
 */
namespace mcmc {
struct ChainLink {
  unsigned        iteration_;
  Double          score_;
  Double          likelihood_;
  Double          prior_;
  Double          penalty_;
  Double          additional_priors_;
  Double          acceptance_rate_;
  Double          acceptance_rate_since_adapt_;
  Double          step_size_;
  vector<Double>  values_;
};
}

/**
 * Class definition
 */
class MCMC : public niwa::base::Object {
public:
  // Methods
  MCMC() = delete;
  explicit MCMC(Model* model);
  virtual                     ~MCMC() = default;
  void                        Validate();
  void                        Build();
  void                        Reset() { };
  virtual void                Execute() = 0;

  // accessors/mutators
  vector<mcmc::ChainLink>&    chain() { return chain_; }
  bool                        active() const { return active_; }
  ublas::matrix<Double>&      covariance_matrix() {return covariance_matrix_;}
  void                        set_starting_iteration(unsigned value) { starting_iteration_ = value; }

protected:
  // pure virtual methods
  virtual void                DoValidate() = 0;
  virtual void                DoBuild() = 0;

  // members
  Model*                      model_;
  unsigned                    length_ = 0;
  unsigned                    starting_iteration_ = 0;
  ublas::matrix<Double>       covariance_matrix_;
  vector<mcmc::ChainLink>     chain_;
  bool                        active_;
  bool                        print_default_reports_;
};

} /* namespace niwa */
#endif /* MCMC_H_ */