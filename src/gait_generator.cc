/**
 @file    gait_generator.cc
 @author  Alexander W. Winkler (winklera@ethz.ch)
 @date    Sep 20, 2017
 @brief   Brief description
 */

#include <xpp/gait_generator.h>

namespace xpp {
namespace opt {

GaitGenerator::GaitGenerator ()
{
  // TODO Auto-generated constructor stub
}

GaitGenerator::VecTimes
GaitGenerator::GetNormalizedContactSchedule (EndeffectorID ee) const
{
  auto v = GetContactSchedule().at(ee); // shorthand
  double total_time = std::accumulate(v.begin(), v.end(), 0.0);
  std::transform(v.begin(), v.end(), v.begin(),
                 [total_time](double t_phase){ return t_phase/total_time;});

  return v;
}

GaitGenerator::VecTimes
GaitGenerator::GetContactSchedule (EndeffectorID ee) const
{
  return GetContactSchedule().at(ee);
}

GaitGenerator::FootDurations
GaitGenerator::GetContactSchedule () const
{
  int n_ee = contacts_.front().GetCount();
  VecTimes d_accumulated(n_ee, 0.0);

  FootDurations foot_durations(n_ee);
  for (int phase=0; phase<contacts_.size()-1; ++phase) {

    ContactState curr = contacts_.at(phase);
    ContactState next = contacts_.at(phase+1);

    for (auto ee : curr.GetEEsOrdered()) {
      d_accumulated.at(ee) += times_.at(phase);

      // if contact will change in next phase, so this phase duration complete
      bool contacts_will_change = curr.At(ee) != next.At(ee);
      if (contacts_will_change)  {
        foot_durations.at(ee).push_back(d_accumulated.at(ee));
        d_accumulated.at(ee) = 0.0;
      }
    }
  }

  // push back last phase
  for (auto ee : contacts_.back().GetEEsOrdered())
    foot_durations.at(ee).push_back(d_accumulated.at(ee) + times_.back());


  return foot_durations;
}

void
GaitGenerator::SetGaits (const std::vector<GaitTypes>& gaits)
{
  for (GaitTypes g : gaits) {
    auto info = GetGait(g);
    std::vector<double>       t = info.first;
    std::vector<ContactState> c = info.second;
    assert(t.size() == c.size()); // make sure every phase has a time

    times_.insert      (times_.end(), t.begin(), t.end());
    contacts_.insert(contacts_.end(), c.begin(), c.end());
  }
}


GaitGenerator::~GaitGenerator ()
{
  // TODO Auto-generated destructor stub
}

} /* namespace opt */
} /* namespace xpp */
