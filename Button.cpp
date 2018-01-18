#include "Timer.h"
#include "DetectorStrategy.h"
#include "Button.h"

const int  Button::s_defaultKeyPollTimeMillis = 50;

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

Button::Button(PinSupervisor* pinSupervisor, DetectorStrategy* detector, ButtonAdapter* adapter)
: m_debounceTimer(new Timer(new DebounceTimerAdatper(this), Timer::IS_RECURRING, s_defaultKeyPollTimeMillis))
, m_pinSupervisor(pinSupervisor)
, m_adapter(adapter)
, m_detectorChain(detector)
, m_lastWasButtonPressed(false)
{ }

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
