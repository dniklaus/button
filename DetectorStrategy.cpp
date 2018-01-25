/*
 * DetectorStrategy.cpp
 *
 *  Created on: 04.12.2017
 *      Author: nid
 */

#include "DetectorStrategy.h"

DetectorStrategy::DetectorStrategy()
: m_next(0)
{ }

DetectorStrategy::~DetectorStrategy()
{ }

void DetectorStrategy::setNext(DetectorStrategy* next)
{
  m_next = next;
}

DetectorStrategy* DetectorStrategy::next()
{
  return m_next;
}

void DetectorStrategy::doActionFilter(bool currentState, bool formerState)
{
  if (currentState != formerState)
  {
    // change happened / edge detected
    if (currentState)
    {
      onRisingEdge();
    }
    else
    {
      onFallingEdge();
    }
  }
}

//-----------------------------------------------------------------------------

EdgeDetector::EdgeDetector()
: DetectorStrategy()
{ }

EdgeDetector::~EdgeDetector()
{ }

void EdgeDetector::onRisingEdge()
{
  onEdge(true);
}

void EdgeDetector::onFallingEdge()
{
  onEdge(false);
}

