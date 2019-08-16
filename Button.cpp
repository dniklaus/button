#include "Timer.h"
#include "DetectorStrategy.h"
#include "Button.h"

const int  Button::c_defaultKeyPollTimeMillis = 50;

//-----------------------------------------------------------------------------

class DebounceTimerAdatper : public TimerAdapter
{
private:
  Button* m_Button;
  
public:
  DebounceTimerAdatper(Button* Button)
  : m_Button(Button)
  { }
  
  void timeExpired()
  {
    if (0 != m_Button)
    {
      m_Button->debounce();
    }
  }
};

//-----------------------------------------------------------------------------

ButtonAdapter::ButtonAdapter()
: m_button(0)
{ }

void ButtonAdapter::attachButton(Button* myButton)
{
  m_button = myButton;
}

Button* ButtonAdapter::button()
{
  return m_button;
}

//-----------------------------------------------------------------------------

Button::Button(PinSupervisor* pinSupervisor, DetectorStrategy* detector, ButtonAdapter* adapter)
: m_debounceTimer(new Timer(new DebounceTimerAdatper(this), Timer::IS_RECURRING, c_defaultKeyPollTimeMillis))
, m_pinSupervisor(pinSupervisor)
, m_adapter(0)
, m_detectorChain(0)
, m_lastWasButtonPressed(false)
{
  attachAdapter(adapter);
  addDetector(detector);
}

Button::~Button()
{
  delete m_debounceTimer->adapter();
  delete m_debounceTimer; m_debounceTimer = 0;
}

ButtonAdapter* Button::adapter()
{
  return m_adapter;
}

void Button::attachAdapter(ButtonAdapter* adapter)
{
  m_adapter = adapter;
  if (0 != m_adapter)
  {
    m_adapter->attachButton(this);
  }
}

void Button::debounce()
{
  bool currentIsButtonPressed = isButtonPressed();

  if (m_lastWasButtonPressed != currentIsButtonPressed)
  {
    DetectorStrategy* detector = m_detectorChain;
    while (0 != detector)
    {
      detector->doActionFilter(currentIsButtonPressed, m_lastWasButtonPressed);
      detector = detector->next();
    }
    m_lastWasButtonPressed = currentIsButtonPressed;
  }
}

bool Button::isButtonPressed()
{
  bool pressed = false;
  if (0 != m_pinSupervisor)
  {
    pressed = m_pinSupervisor->isButtonPressed();
  }
  return pressed;
}

void Button::addDetector(DetectorStrategy* detector)
{
  if (0 != detector)
  {
    detector->attachButton(this);
  }
  if (0 == m_detectorChain)
  {
    m_detectorChain = detector;
  }
  else
  {
    DetectorStrategy* next = m_detectorChain;
    while (next->next() != 0)
    {
      next = next->next();
    }
    next->setNext(detector);
  }
}

void Button::removeDetector(DetectorStrategy* detector)
{
  if (m_detectorChain == detector)
  {
    m_detectorChain = detector->next();
  }
  else
  {
    DetectorStrategy* next = m_detectorChain;
    while ((next != 0) && (next->next() != detector))
    {
      next = next->next();
    }
    if (next != 0)
    {
      next->setNext(detector->next());
    }
  }

}
