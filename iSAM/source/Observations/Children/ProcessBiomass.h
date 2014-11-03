/**
 * @file ProcessBiomass.h
 * @author Scott Rasmussen (scott.rasmussen@zaita.com)
 * @github https://github.com/Zaita
 * @date 17/10/2014
 * @section LICENSE
 *
 * Copyright NIWA Science �2014 - www.niwa.co.nz
 *
 * @section DESCRIPTION
 *
 * << Add Description >>
 */
#ifndef OBSERVATIONS_PROCESSBIOMASS_H_
#define OBSERVATIONS_PROCESSBIOMASS_H_

// headers
#include "Observations/Children/Biomass/Biomass.h"

// namespaces
namespace isam {
namespace observations {

/**
 *
 */
class ProcessBiomass : public observations::Biomass {
public:
  ProcessBiomass();
  virtual                     ~ProcessBiomass() = default;
  void                        DoBuild() override final;

private:
  // members
  string                      process_label_;
  Double                      process_proportion_;
};

} /* namespace observations */
} /* namespace isam */

#endif /* PROCESSBIOMASS_H_ */