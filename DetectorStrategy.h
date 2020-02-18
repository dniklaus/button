/*
 * DetectorStrategy.h
 *
 *  Created on: 04.12.2017
 *      Author: nid
 */

#ifndef LIB_BUTTON_DETECTORSTRATEGY_H_
#define LIB_BUTTON_DETECTORSTRATEGY_H_

class Button;

class DetectorStrategy
{
public:
  void doActionFilter(bool currentState, bool formerState);

  void attachButton(Button* myButton);
  Button* button();

protected:
  DetectorStrategy();
  virtual ~DetectorStrategy();

public:
  void setNext(DetectorStrategy* next);
  DetectorStrategy* next();

protected:
  virtual void onRisingEdge() { }
  virtual void onFallingEdge() { }

private:
  DetectorStrategy* m_next;
  Button* m_button;

private:  // forbidden functions
  DetectorStrategy(const DetectorStrategy& src);              // copy constructor
  DetectorStrategy& operator = (const DetectorStrategy& src); // assignment operator
};

#endif /* LIB_BUTTON_DETECTORSTRATEGY_H_ */
