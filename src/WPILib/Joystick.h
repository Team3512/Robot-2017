/*----------------------------------------------------------------------------*/
/* Copyright (c) 2008-2017 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <vector>

#include <support/deprecated.h>

#include "GenericHID.h"

namespace frc {

/**
 * Handle input from standard Joysticks connected to the Driver Station.
 * This class handles standard input that comes from the Driver Station. Each
 * time a value is requested the most recent value is returned. There is a
 * single class instance for each joystick and the mapping of ports to hardware
 * buttons depends on the code in the Driver Station.
 */
class Joystick : public GenericHID {
 public:
  enum class Axis { kX, kY, kZ, kTwist, kThrottle };
  enum class Button { kTrigger = 1, kTop = 2 };

  enum AxisType { kXAxis, kYAxis, kZAxis, kTwistAxis, kThrottleAxis };
  enum ButtonType { kTriggerButton, kTopButton };

  explicit Joystick(int port);
  virtual ~Joystick() = default;

  Joystick(const Joystick&) = delete;
  Joystick& operator=(const Joystick&) = delete;

  void SetAxisChannel(Axis axis, int channel);

  WPI_DEPRECATED("Use Axis enum values instead of AxisType.")
  void SetAxisChannel(AxisType axis, int channel);

  int GetAxisChannel(Axis axis) const;

  WPI_DEPRECATED("Use Axis enum values instead of AxisType.")
  int GetAxisChannel(AxisType axis) const;

  double GetX(JoystickHand hand = kRightHand) const override;
  double GetY(JoystickHand hand = kRightHand) const override;
  double GetZ() const;
  double GetTwist() const;
  double GetThrottle() const;

  WPI_DEPRECATED("Use the more specific axis getter functions.")
  double GetAxis(AxisType axis) const;

  WPI_DEPRECATED("Use GetButton() instead.")
  bool GetTrigger(JoystickHand hand = kRightHand) const;

  WPI_DEPRECATED("Use GetButton() instead.")
  bool GetTop(JoystickHand hand = kRightHand) const;

  WPI_DEPRECATED("Use Joystick instances instead.")
  static Joystick* GetStickForPort(int port);

  bool GetButton(Button button) const;
  bool GetButtonPressed(Button button);
  bool GetButtonReleased(Button button);

  WPI_DEPRECATED("Use Button enum values instead of ButtonType.")
  bool GetButton(ButtonType button) const;

  double GetMagnitude() const;
  double GetDirectionRadians() const;
  double GetDirectionDegrees() const;

 private:
  static constexpr int kDefaultXAxis = 0;
  static constexpr int kDefaultYAxis = 1;
  static constexpr int kDefaultZAxis = 2;
  static constexpr int kDefaultTwistAxis = 2;
  static constexpr int kDefaultThrottleAxis = 3;
  static constexpr int kMinNumAxes = 4;

  std::vector<int> m_axes;
};

}  // namespace frc