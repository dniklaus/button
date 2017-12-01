#include "Button.h"
#include "Timer.h"
#include "Arduino.h"

const bool Button::IS_POS_LOGIC = false;
const bool Button::IS_NEG_LOGIC = true;
const int  Button::BTN_NC       = -1;
const int  Button::IND_NC       = -1;

const int  Button::s_defaultKeyPollTime = 50;

//-----------------------------------------------------------------------------

class MyDebounceTimerAdatper : public TimerAdapter
{
private:
  Button* m_Button;
  bool m_lastWasButtonPressed;
  
public:
  MyDebounceTimerAdatper(Button* Button)
  : m_Button(Button)
  , m_lastWasButtonPressed(false)
  { }
  
  void timeExpired()
  {
    if (0 != m_Button)
    {
      bool currentIsButtonPressed = m_Button->isButtonPressed();
      
      if (m_lastWasButtonPressed != currentIsButtonPressed)
      {
        m_lastWasButtonPressed = currentIsButtonPressed;
        if (currentIsButtonPressed)
        {
          m_Button->toggle();
        }
      } 
    }
  }
};

//-----------------------------------------------------------------------------

Button::Button(int buttonPin, int indicatorPin, bool isButtonNegativeLogic, ButtonAdapter* adapter)
: m_debounceTimer(new Timer(new MyDebounceTimerAdatper(this), Timer::IS_RECURRING, s_defaultKeyPollTime))
, m_adapter(adapter)
, m_isButtonNegativeLogic(isButtonNegativeLogic)
, m_isActive(false)
, m_buttonPin(buttonPin)
, m_indicatorPin(indicatorPin)
{ 
  if (0 <= m_buttonPin)
  {
    pinMode(m_buttonPin, INPUT);
    digitalWrite(m_buttonPin, m_isButtonNegativeLogic ? HIGH : LOW); // pull
  }
  if (0 <= m_indicatorPin)
  {
    pinMode(m_indicatorPin, OUTPUT);
    digitalWrite(m_indicatorPin, m_isActive);
  }
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
}

bool Button::isActive()
{
  return m_isActive;
}

void Button::setIsActive(bool isActive)
{
  bool changed = (isActive != m_isActive);
  m_isActive = isActive;
  if (0 <= m_indicatorPin)
  {
    digitalWrite(m_indicatorPin, m_isActive);
  }
  if ((0 != m_adapter) && (changed))
  {
    m_adapter->notifyStatusChanged(m_isActive);
  }
}

void Button::toggle()
{
  m_isActive = !m_isActive;
  if (0 <= m_indicatorPin)
  {
    digitalWrite(m_indicatorPin, m_isActive);
  }
  if (0 != m_adapter)
  {
    m_adapter->notifyStatusChanged(m_isActive);
  }
}

bool Button::isButtonPressed()
{
  bool pressed = false;
  if (0 <= m_buttonPin)
  {
    pressed = digitalRead(m_buttonPin);
    pressed = (m_isButtonNegativeLogic ? !pressed : pressed);
  }
  return pressed;
}

