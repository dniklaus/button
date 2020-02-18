/*
 * EdgeDetector.cpp
 *
 *  Created on: 18.02.2020
 *      Author: ERN_DNI
 */

#include "EdgeDetector.h"

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
