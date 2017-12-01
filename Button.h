#ifndef _Button_H
#define _Button_H

class Timer;

//-----------------------------------------------------------------------------

/**
 * Interface for a Toggle Button Adapter to receive status change notifications.
 */
class ButtonAdapter
{
public:
  /**
   * Notification method to be implemented by specific adapter.
   * Called on any Button status change and also on any button pressed event.
   * @param isActive  Indicates the new status: active (true) or inactive (false).
   */
  virtual void notifyStatusChanged(bool isActive) = 0;

protected:
  ButtonAdapter() { }
  virtual ~ButtonAdapter() { }

private:  // forbidden functions
  ButtonAdapter(const ButtonAdapter& src);              // copy constructor
  ButtonAdapter& operator = (const ButtonAdapter& src); // assignment operator
};

//-----------------------------------------------------------------------------

/**
 * Toggle Button object model.
 * - toggles its status on button status change from "not pressed" to "pressed"
 * - button signal is debounced by a 50ms timer
 * - button pin is configurable
 * - status indication pin is configurable
 * - button logic is configurable
 * - status change notifications can be emitted through injectable specific adapter implementation.
 * - status can be changed and toggled by client program
 * - also useful to implement virtual binary status
 * - also useful when just a a button debouncer is needed
 */
class Button
{
public:
  /**
   * Constructor.
   * @param buttonPin             Arduino Pin where the button is connected to; default: BTN_NC (not connected)
   * @param indicatorPin          Arduino Pin where the (LED) status indication is connected to; default: BTN_NC (not connected)
   * @param isButtonNegativeLogic Consider button pressed when buttonPin is LOW (true) or HIGH (false); default: false (button pressed when buttonPin is HIGH)
   * @param adapter               Inject pointer to ButtonAdapter object; NULL pointer: no status change notification will be sent out.
   */
  Button(int buttonPin = BTN_NC, int indicatorPin = IND_NC, bool isButtonNegativeLogic = false, ButtonAdapter* adapter = 0);

  /**
   * Destructor.
   */
  virtual ~Button();

  /**
   * Retrieve pointer to currently injected adapter object.
   * @return Pointer to currently injected ButtonAdapter object; NULL pointer if none is injected.
   */
  ButtonAdapter* adapter();

  /**
   * Attach adapter object.
   * @param adapter Pointer to specific ButtonAdapter object; NULL pointer: no status change notification will be sent out.
   */
  void attachAdapter(ButtonAdapter* adapter);

  /**
   * Retrieve current status.
   * @return  Current status: active (true) or inactive (false).
   */
  bool isActive();

  /**
   * Set new status.
   * @param isActive  New status: active (true) or inactive (false).
   */
  void setIsActive(bool isActive);

  /**
   * Toggle current status.
   * Set new status to true if current was false and vice versa.
   */
  virtual void toggle();

  /**
   * Retrieve current button status.
   * @return Button status: pressed (true) or not pressed (false).
   */
  bool isButtonPressed();

  /**
   * Constant for isButtonNegativeLogic parameter of the constructor (@see Button()), to create a button switching the pin to HIGH.
   */
  static const bool IS_POS_LOGIC;

  /**
   * Constant for isButtonNegativeLogic parameter of the constructor (@see Button()), to create a button switching the pin to LOW.
   */
  static const bool IS_NEG_LOGIC;

  /**
   * Constant for constructor's buttonPin parameter (@see Button()), to create a button object w/o any button connected to a pin, i.e. useful for virtual button.
   */
  static const int BTN_NC;

  /**
   * Constant for constructor's indicatorPin parameter (@see Button()), to create a button object w/o any indicator output connected to a pin.
   */
  static const int IND_NC;

private:
  Timer* m_debounceTimer;
  ButtonAdapter* m_adapter;
  bool m_isButtonNegativeLogic;
  bool m_isActive;
  int m_buttonPin;
  int m_indicatorPin;
  static const int s_defaultKeyPollTime;
  
private:  // forbidden functions
  Button(const Button& src);              // copy constructor
  Button& operator = (const Button& src); // assignment operator
};

#endif

