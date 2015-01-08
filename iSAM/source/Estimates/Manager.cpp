/**
 * @file Manager.cpp
 * @author  Scott Rasmussen (scott.rasmussen@zaita.com)
 * @version 1.0
 * @date 27/02/2013
 * @section LICENSE
 *
 * Copyright NIWA Science �2013 - www.niwa.co.nz
 *
 * $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
 */

// Headers
#include "Manager.h"

#include "ConfigurationLoader/EstimateValuesLoader.h"
#include "GlobalConfiguration/GlobalConfiguration.h"

// Namespaces
namespace niwa {
namespace estimates {

using std::set;

/**
 *
 */
void Manager::Validate() {
  /**
   * Run over our creators and get them to build the actual
   * estimates the system is going to build.
   */
  for (CreatorPtr creator : creators_)
    creator->CreateEstimates();

  /**
   * Validate the actual estimates now
   */
  for (EstimatePtr estimate : objects_)
    estimate->Validate();

  /**
   * Load any estimate values that have been supplied
   */
  GlobalConfigurationPtr global_config = GlobalConfiguration::Instance();
  if (global_config->estimate_value_file() != "") {
    configuration::EstimateValuesLoader loader;
    loader.LoadValues(global_config->estimate_value_file());

    unsigned values = 0;
    for (auto iter : estimate_values_) {
      if (values == 0)
        values = iter.second.size();
      if (values != iter.second.size())
        LOG_ERROR("Estimate values loaded for estimate " << iter.first << " count is " << iter.second.size() << " when we expected " << values);

      EstimatePtr current_estimate;
      for (EstimatePtr estimate : objects_) {
        if (estimate->label() == iter.first) {
          current_estimate = estimate;
          break;
        }
      }

      if (!current_estimate)
        LOG_ERROR("Could not find the estimate " << iter.first << " when loading estimate values from file");
    }
  }
}


/**
 * Count how many of our estimates are enabled
 * and return the count
 *
 * @return The number of enabled estimates
 */
unsigned Manager::GetEnabledCount() {
  unsigned count = 0;

  for (EstimatePtr estimate : objects_) {
    if (estimate->enabled())
      count++;
  }

  return count;
}

/**
 *
 */
vector<EstimatePtr> Manager::GetEnabled() {
  vector<EstimatePtr> result;

  for (EstimatePtr estimate : objects_) {
    if (estimate->enabled())
      result.push_back(estimate);
  }

  return result;
}

/**
 *
 */
void Manager::RemoveAllObjects() {
  objects_.clear();
  creators_.clear();
}

/**
 * This method will look for estimates that match our parameter.
 *
 * First it will check all of the parameters parent info object
 * (the object that created them) for matches. If it doesn't
 * find a match then it'll re-loop through the estimates
 * looking for a specific parameter there.
 *
 * This is because we want to prioritise matches to what was
 * defined explicitly in the configuration file. As each @estimate
 * block in the file can create multiple Estimate objects we
 * will prioritise this way first before looking at what
 * actual estimates were created.
 *
 * @param parameter The parameter the estimate is targeting
 * @return True if found, false otherwise
 */
bool Manager::HasEstimate(const string& parameter) {

  for (EstimatePtr estimate : objects_) {
    if (estimate->parent_info()->parameters().Get(PARAM_PARAMETER)->values()[0] == parameter)
      return true;
  }

  for (EstimatePtr estimate : objects_) {
    if (estimate->parameter() == parameter)
      return true;
  }

  return false;
}

/**
 * This method will enable all estimates that match either the parameter
 * or the parent info matches the parameter.
 *
 * @param parameter The parameter to match against the estimate and parent info
 */
void Manager::EnableEstimate(const string& parameter) {
  for (EstimatePtr estimate : objects_) {
    if (estimate->parent_info()->parameters().Get(PARAM_PARAMETER)->values()[0] == parameter)
      estimate->set_enabled(true);
    else if (estimate->parameter() == parameter)
      estimate->set_enabled(true);
  }
}

/**
 * This method will disable all estimates that match either the parameter
 * or the parent info matches the parameter.
 *
 * @param parameter The parameter to match against the estimate and parent info
 */
void Manager::DisableEstimate(const string& parameter) {
  for (EstimatePtr estimate : objects_) {
    if (estimate->parent_info()->parameters().Get(PARAM_PARAMETER)->values()[0] == parameter)
      estimate->set_enabled(false);
    else if (estimate->parameter() == parameter)
      estimate->set_enabled(false);
  }
}

/**
 *
 */
EstimatePtr Manager::GetEstimate(const string& parameter) {
  for (EstimatePtr estimate : objects_) {
    if (estimate->parameter() == parameter)
      return estimate;
  }

  for (EstimatePtr estimate : objects_)
    cout << "Estimate: " << estimate->label() << " | " << estimate->parameter() << endl;


  return EstimatePtr();
}

/**
 *
 */
void Manager::AddEstimateValue(const string& estimate_label, Double value) {
  estimate_values_[estimate_label].push_back(value);
}

} /* namespace estimates */
} /* namespace niwa */
