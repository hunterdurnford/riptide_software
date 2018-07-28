#include "riptide_autonomy/validators.h"

DetectionValidator::DetectionValidator(int detections, double duration)
{
  durationThresh = duration;
  detsReq = detections;
  Reset();
}

bool DetectionValidator::Validate()
{
  if (++detections == 1)
    startTime = ros::Time::now();

  if (ros::Time::now().toSec() - startTime.toSec() > durationThresh)
  {
    valid = detections >= detsReq;
    detections = 0;
    return valid;
  }
  return false;
}

bool DetectionValidator::IsValid()
{
  return valid;
}

void DetectionValidator::Reset()
{
  valid = false;
  detections = 0;
}

ErrorValidator::ErrorValidator(double errorThresh, double duration)
{
  durationThresh = duration;
  this->errorThresh = errorThresh;
  Reset();
}

bool ErrorValidator::Validate(double error)
{
  if (abs(error) <= errorThresh)
  {
    if (outsideRange)
      startTime = ros::Time::now();

    outsideRange = false;

    return ros::Time::now().toSec() - startTime.toSec() > durationThresh;
  }
  else
    outsideRange = true;

  return false;
}

bool ErrorValidator::IsValid()
{
  return ros::Time::now().toSec() - startTime.toSec() > durationThresh;
}

void ErrorValidator::Reset()
{
  outsideRange = true;
}