/**
 * @file PartitionMeanWeight.cpp
 * @author  Scott Rasmussen (scott.rasmussen@zaita.com)
 * @date 7/01/2014
 * @section LICENSE
 *
 * Copyright NIWA Science �2013 - www.niwa.co.nz
 *
 */

// headers
#include "PartitionMeanWeight.h"

#include "AgeLengths/Manager.h"
#include "Categories/Categories.h"
#include "Model/Model.h"
#include "Partition/Partition.h"
#include "Partition/Accessors/All.h"
#include "LengthWeights/Manager.h"
#include "TimeSteps/Manager.h"

// namespaces
namespace niwa {
namespace reports {
namespace age {

/**
 * Default constructor
 */
PartitionMeanWeight::PartitionMeanWeight(Model* model) : Report(model) {
  run_mode_    = (RunMode::Type)(RunMode::kBasic | RunMode::kProjection | RunMode::kSimulation | RunMode::kEstimation | RunMode::kProfiling);
  model_state_ = (State::Type)(State::kIterationComplete);

  parameters_.Bind<string>(PARAM_TIME_STEP, &time_step_, "The time step label", "", "");
  parameters_.Bind<unsigned>(PARAM_YEARS, &years_, "The years for the report", "", true);
}

/**
 * Build method
 */
void PartitionMeanWeight::DoBuild() {
  if (!parameters_.Get(PARAM_YEARS)->has_been_defined()) {
    years_ = model_->years();
  }
}

/**
 * Execute the report
 */
void PartitionMeanWeight::DoExecute() {
  LOG_TRACE();

//  unsigned year_index      = 0;
  unsigned time_step_index = model_->managers().time_step()->GetTimeStepIndex(time_step_);
  niwa::partition::accessors::All all_view(model_);

  cache_ << "*"<< type_ << "[" << label_ << "]" << "\n";
  cache_ << "time_step: " << time_step_ << "\n";

  for (auto iterator = all_view.Begin(); iterator != all_view.End(); ++iterator) {
    string category = (*iterator)->name_;
    LOG_FINEST() << "printing mean weight-at-age for category " << category;
    cache_ << category << " " << REPORT_R_LIST << "\n";

    cache_ << "mean_weights " << REPORT_R_DATAFRAME << "\n";
    cache_ << "year ";
    for (unsigned i = model_->min_age(); i <= model_->max_age(); ++i)
      cache_ << i << " ";
    cache_ << "\n";

    for (auto year : years_) {
//      year_index = year > model_->start_year() ? year - model_->start_year() : 0;
      cache_ << year << " ";

      for (unsigned age = (*iterator)->min_age_; age <= (*iterator)->max_age_; ++age) {
        Double temp = (*iterator)->mean_weight_by_time_step_age_[time_step_index][age];
        cache_ << AS_VALUE(temp) << " ";
      }

      cache_ << "\n";
      LOG_FINEST() << "cached mean weight";
    }

    cache_ << REPORT_R_LIST_END << "\n";
  }

  ready_for_writing_ = true;
}

} /* namespace age */
} /* namespace reports */
} /* namespace niwa */
