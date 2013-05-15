/**
 * @file Manager.cpp
 * @author  Scott Rasmussen (scott.rasmussen@zaita.com)
 * @date 15/05/2013
 * @section LICENSE
 *
 * Copyright NIWA Science �2013 - www.niwa.co.nz
 *
 */
#include "Manager.h"

#include "Model/Model.h"

namespace isam {
namespace reports {

/**
 * Default Constructor
 */
Manager::Manager() {
}

/**
 * Destructor
 */
Manager::~Manager() noexcept(true) {
}

/**
 * Build our reports then
 * organise the reports stored in our
 * object list into different containers
 * based on their type.
 */
void Manager::Build() {
  for (ReportPtr report : objects_) {
    report->Build();

    if ((RunMode::Type)(report->run_mode() & RunMode::kInvalid) == RunMode::kInvalid)
      LOG_CODE_ERROR("Report: " << report->label() << " has not been properly configured to have a run mode");

    if (report->model_state() != State::kExecute)
      state_reports_[report->model_state()].push_back(report);
    else
      time_step_reports_[report->time_step()].push_back(report);
  }
}

/**
 * Execute any reports that have the model_state
 * specified as their execution state
 *
 * @param model_state The state the model has just finished
 */
void Manager::Execute(State::Type model_state) {
  RunMode::Type run_mode = Model::Instance()->run_mode();
  for(ReportPtr report : state_reports_[model_state]) {
      if ( (RunMode::Type)(report->run_mode() & run_mode) == run_mode)
        report->Execute();
  }
}

/**
 * Execute any reports that have the year and
 * time step label as their execution parameters.
 * Note: All these reports are only in the execute phase.
 *
 * @param year The current year for the model
 * @param time_step_label The last time step to be completed
 */
void Manager::Execute(unsigned year, const string& time_step_label) {
  LOG_TRACE();
  RunMode::Type run_mode = Model::Instance()->run_mode();
  for(ReportPtr report : time_step_reports_[time_step_label]) {
    if ( (RunMode::Type)(report->run_mode() & run_mode) != run_mode)
      continue;
    if (!report->HasYear(year))
      continue;

    report->Execute();
  }
}

} /* namespace reports */
} /* namespace isam */
