/*
 * DetectorStrategy.cpp
 *
 *  Created on: 04.12.2017
 *      Author: nid
 */

#include "Button.h"
#include "DetectorStrategy.h"

DetectorStrategy::DetectorStrategy()
: m_next(0)
, m_button(0)
{ }

DetectorStrategy::~DetectorStrategy()
{ }

void DetectorStrategy::attachButton(Button* myButton)
{
  m_button = myButton;
}

Button* DetectorStrategy::button()
{
  return m_button;
}

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


