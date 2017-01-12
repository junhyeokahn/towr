/**
 @file    motion_factory.cc
 @author  Alexander W. Winkler (winklera@ethz.ch)
 @date    Aug 25, 2016
 @brief   Brief description
 */

#include <xpp/opt/motion_factory.h>
#include <xpp/opt/com_spline4.h>
#include <xpp/opt/com_spline6.h>

namespace xpp {
namespace opt {

MotionFactory::MotionFactory ()
{
  // TODO Auto-generated constructor stub
}

MotionFactory::~MotionFactory ()
{
  // TODO Auto-generated destructor stub
}

MotionFactory::ComMotionPtrS
MotionFactory::CreateComMotion (const PhaseVec& phases, int polynomials_per_phase)
{
  auto com_spline = std::make_shared<ComSpline6>();
  com_spline->Init(phases, polynomials_per_phase);
  return com_spline;
}

MotionFactory::ComMotionPtrS
MotionFactory::CreateComMotion (const PhaseVec& phases,
                                const Vector2d& start_cog_p,
                                const Vector2d& start_cog_v,
                                int polynomials_per_phase)
{
  auto com_spline = std::make_shared<ComSpline4>();
  com_spline->Init(phases, polynomials_per_phase);
  com_spline->SetStartPosVel(start_cog_p, start_cog_v);
  com_spline->SetEndAtStart();
  return com_spline;
}

} /* namespace opt */
} /* namespace xpp */

