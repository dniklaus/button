/*
 * DetectorStrategy.h
 *
 *  Created on: 04.12.2017
 *      Author: nid
 */

#ifndef LIB_BUTTON_DETECTORSTRATEGY_H_
#define LIB_BUTTON_DETECTORSTRATEGY_H_

class DetectorStrategy
{
public:
  void doActionFilter(bool currentState, bool formerState);



protected:
  DetectorStrategy();
  virtual ~DetectorStrategy();

  void setNext(DetectorStrategy* next);
  DetectorStrategy* next();

  virtual void onRisingEdge() { }
  virtual void onFallingEdge() { }

private:
  DetectorStrategy* m_next;

private:  // forbidden functions
  DetectorStrategy(const DetectorStrategy& src);              // copy constructor
  DetectorStrategy& operator = (const DetectorStrategy& src); // assignment operator
};

//-----------------------------------------------------------------------------

class EdgeDetector : public DetectorStrategy
{
public:
  EdgeDetector();
  virtual ~EdgeDetector();

private:  // forbidden functions
  EdgeDetector(const EdgeDetector& src);              // copy constructor
  EdgeDetector& operator = (const EdgeDetector& src); // assignment operator
};

#endif /* LIB_BUTTON_DETECTORSTRATEGY_H_ */
