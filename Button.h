#ifndef _Button_H
#define _Button_H

class SpinTimer;
class Button;

//-----------------------------------------------------------------------------

/**
 * Interface for a Pin Supervisor.
 */
class PinSupervisor
{
private:
  bool m_isButtonNegativeLogic;

protected:
  /**
   * Constructor.
   * Abstract class -> protected
   * @param isButtonNegativeLogic Consider button pressed when buttonPin is LOW (true) or HIGH (false); default: false (button pressed when buttonPin is HIGH)
   */
  PinSupervisor(bool isButtonNegativeLogic = IS_POS_LOGIC)
  : m_isButtonNegativeLogic(isButtonNegativeLogic)
  { }

  virtual ~PinSupervisor() { }

public:
  virtual bool getPinState() = 0;

  bool isButtonPressed()
  {
    bool pressed = getPinState();
    pressed = (m_isButtonNegativeLogic ? !pressed : pressed);
    return pressed;
  }

  void setButtonLogic(bool isNegativeLogic) { m_isButtonNegativeLogic = isNegativeLogic; }
  bool isButtonNegativeLogic() { return m_isButtonNegativeLogic; }

  /**
   * Constant for isButtonNegativeLogic parameter of the constructor (@see PinSupervisor()), to create a button switching the pin to HIGH.
   */
  static const bool IS_POS_LOGIC = false;

  /**
   * Constant for isButtonNegativeLogic parameter of the constructor (@see PinSupervisor()), to create a button switching the pin to LOW.
   */
  static const bool IS_NEG_LOGIC = true;

private:  // forbidden functions
  PinSupervisor(const PinSupervisor& src);              // copy constructor
  PinSupervisor& operator = (const PinSupervisor& src); // assignment operator
};

//-----------------------------------------------------------------------------

class DetectorStrategy;

//-----------------------------------------------------------------------------

/**
 * Abstract class for a Toggle Button Adapter to receive status change notifications.
 */
class ButtonAdapter
{
private:
  Button* m_button;

public:
  /**
   * Notification method to be implemented by specific adapter.
   * Called on any Button status change and also on any button pressed event.
   * @param isActive  Indicates the new status: active (true) or inactive (false).
   */
  virtual void notifyStatusChanged(bool isActive) = 0;

  virtual void attachButton(Button* myButton);

  Button* button();

protected:
  ButtonAdapter() ;
  virtual ~ButtonAdapter() { }

private:  // forbidden functions
  ButtonAdapter(const ButtonAdapter& src);              // copy constructor
  ButtonAdapter& operator = (const ButtonAdapter& src); // assignment operator
};

//-----------------------------------------------------------------------------

/**
 * Button object model.
 * - button signal is debounced by a 50ms timer
 * - button pin supervisor is injectable
 */
class Button
{
//  friend class DetectorStrategy;

public:
  /**
   * Constructor.
   * @param pinSupervisor Inject pointer to PinSupervisor object; NULL pointer: no input pin will be supervised
   * @param detector      Inject pointer to a (chain of) DetectorStrategy object(s)
   * @param adapter       Inject pointer to ButtonAdapter object; NULL pointer: no status change notification will be sent out.
   */
  Button(PinSupervisor* pinSupervisor = 0, DetectorStrategy* detector = 0, ButtonAdapter* adapter = 0);

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
   * Debounce the input signal.
   *
   * Called with regular interval (debounce time)
   */
  void debounce();

  /**
   * Retrieve current button status.
   * @return Button status: pressed (true) or not pressed (false).
   */
  bool isButtonPressed();

  DetectorStrategy* detectorChain();

  void addDetector(DetectorStrategy* detector);
  void removeDetector(DetectorStrategy* detector);

private:
  SpinTimer* m_debounceTimer;
  PinSupervisor* m_pinSupervisor;
  ButtonAdapter* m_adapter;
  DetectorStrategy* m_detectorChain;
  bool m_lastWasButtonPressed;                  /// debouncing helper
  static const int c_defaultKeyPollTimeMillis;  /// debounce poll time
  
private:  // forbidden functions
  Button(const Button& src);              // copy constructor
  Button& operator = (const Button& src); // assignment operator
};

#endif

