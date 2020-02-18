/*
 * EdgeDetector.h
 *
 *  Created on: 18.02.2020
 *      Author: ERN_DNI
 */

#ifndef PIO_LIBDEPS_NRF52840_DK_BUTTON_EDGEDETECTOR_H_
#define PIO_LIBDEPS_NRF52840_DK_BUTTON_EDGEDETECTOR_H_

#include "DetectorStrategy.h"

class EdgeDetector : public DetectorStrategy
{
public:
  EdgeDetector();
  virtual ~EdgeDetector();

  virtual void onEdge(bool newState) = 0;

protected:
  virtual void onRisingEdge();
  virtual void onFallingEdge();

private:  // forbidden functions
  EdgeDetector(const EdgeDetector& src);              // copy constructor
  EdgeDetector& operator = (const EdgeDetector& src); // assignment operator
};

#endif /* PIO_LIBDEPS_NRF52840_DK_BUTTON_EDGEDETECTOR_H_ */
