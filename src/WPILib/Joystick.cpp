/*----------------------------------------------------------------------------*/
/* Copyright (c) 2008-2017 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Joystick.h"

#include <algorithm>
#include <array>
#include <cmath>

#include <HAL/HAL.h>

#include "DriverStation.h"
#include "WPIErrors.h"

using namespace frc;

constexpr int Joystick::kMinNumAxes;

constexpr double kPi = 3.14159265358979323846;

/**
 * Construct an instance of a joystick.
 *
 * The joystick index is the USB port on the Driver Station.
 *
 * @param port The port on the Driver Station that the joystick is plugged into
 *             (0-5).
 */
Joystick::Joystick(int port)
    : GenericHID(port), m_axes(std::max(GetAxisCount(), kMinNumAxes)) {
  m_axes[static_cast<int>(Axis::kX)] = kDefaultXAxis;
  m_axes[static_cast<int>(Axis::kY)] = kDefaultYAxis;
  m_axes[static_cast<int>(Axis::kZ)] = kDefaultZAxis;
  m_axes[static_cast<int>(Axis::kTwist)] = kDefaultTwistAxis;
  m_axes[static_cast<int>(Axis::kThrottle)] = kDefaultThrottleAxis;

  HAL_Report(HALUsageReporting::kResourceType_Joystick, port);
}

/**
 * Set the channel associated with a specified axis.
 *
 * @param axis    The axis to set the channel for.
 * @param channel The channel to set the axis to.
 */
void Joystick::SetAxisChannel(Axis axis, int channel) {
  m_axes[static_cast<int>(axis)] = channel;
}

/**
 * Set the channel associated with a specified axis.
 *
 * @param axis    The axis to set the channel for.
 * @param channel The channel to set the axis to.
 */
void Joystick::SetAxisChannel(AxisType axis, int channel) {
  m_axes[axis] = channel;
}

/**
 * Get the channel currently associated with the specified axis.
 *
 * @param axis The axis to look up the channel for.
 * @return The channel fr the axis.
 */
int Joystick::GetAxisChannel(Axis axis) const {
  return m_axes[static_cast<int>(axis)];
}

/**
 * Get the channel currently associated with the specified axis.
 *
 * @param axis The axis to look up the channel for.
 * @return The channel fr the axis.
 */
int Joystick::GetAxisChannel(AxisType axis) const { return m_axes[axis]; }

/**
 * Get the X value of the joystick.
 *
 * This depends on the mapping of the joystick connected to the current port.
 *
 * @param hand This parameter is ignored for the Joystick class and is only
 *             here to complete the GenericHID interface.
 */
double Joystick::GetX(JoystickHand hand) const {
  return GetRawAxis(m_axes[kDefaultXAxis]);
}

/**
 * Get the Y value of the joystick.
 *
 * This depends on the mapping of the joystick connected to the current port.
 *
 * @param hand This parameter is ignored for the Joystick class and is only
 *             here to complete the GenericHID interface.
 */
double Joystick::GetY(JoystickHand hand) const {
  return GetRawAxis(m_axes[kDefaultYAxis]);
}

/**
 * Get the Z value of the current joystick.
 *
 * This depends on the mapping of the joystick connected to the current port.
 */
double Joystick::GetZ() const { return GetRawAxis(m_axes[kDefaultZAxis]); }

/**
 * Get the twist value of the current joystick.
 *
 * This depends on the mapping of the joystick connected to the current port.
 */
double Joystick::GetTwist() const {
  return GetRawAxis(m_axes[kDefaultTwistAxis]);
}

/**
 * Get the throttle value of the current joystick.
 *
 * This depends on the mapping of the joystick connected to the current port.
 */
double Joystick::GetThrottle() const {
  return GetRawAxis(m_axes[kDefaultThrottleAxis]);
}

/**
 * For the current joystick, return the axis determined by the argument.
 *
 * This is for cases where the joystick axis is returned programatically,
 * otherwise one of the previous functions would be preferable (for example
 * GetX()).
 *
 * @param axis The axis to read.
 * @return The value of the axis.
 */
double Joystick::GetAxis(AxisType axis) const {
  switch (axis) {
    case kXAxis:
      return GetX();
    case kYAxis:
      return GetY();
    case kZAxis:
      return GetZ();
    case kTwistAxis:
      return GetTwist();
    case kThrottleAxis:
      return GetThrottle();
    default:
      wpi_setWPIError(BadJoystickAxis);
      return 0.0;
  }
}

/**
 * Read the state of the trigger on the joystick.
 *
 * Look up which button has been assigned to the trigger and read its state.
 *
 * @param hand This parameter is ignored for the Joystick class and is only
 *             here to complete the GenericHID interface.
 * @return The state of the trigger.
 */
bool Joystick::GetTrigger(JoystickHand hand) const {
  return GetRawButton(static_cast<int>(Button::kTrigger));
}

/**
 * Read the state of the top button on the joystick.
 *
 * Look up which button has been assigned to the top and read its state.
 *
 * @param hand This parameter is ignored for the Joystick class and is only
 *             here to complete the GenericHID interface.
 * @return The state of the top button.
 */
bool Joystick::GetTop(JoystickHand hand) const {
  return GetRawButton(static_cast<int>(Button::kTop));
}

Joystick* Joystick::GetStickForPort(int port) {
  static std::array<Joystick*, DriverStation::kJoystickPorts> joysticks{};
  Joystick* stick = joysticks[port];
  if (stick == nullptr) {
    stick = new Joystick(port);
    joysticks[port] = stick;
  }
  return stick;
}

/**
 * Get buttons based on an enumerated type.
 *
 * The button type will be looked up in the list of buttons and then read.
 *
 * @param button The type of button to read.
 * @return The state of the button.
 */
bool Joystick::GetButton(Button button) const {
  return GetRawButton(static_cast<int>(button));
}

/**
 * Whether the button was pressed since the last check. Button indexes begin at
 * 1. The button type is mapped to a button index first.
 *
 * @param button The button index, beginning at 1.
 * @return Whether the button was pressed since the last check.
 */
bool Joystick::GetButtonPressed(Button button) {
  return GetRawButtonPressed(static_cast<int>(button));
}

/**
 * Whether the button was released since the last check. Button indexes begin at
 * 1. The button type is mapped to a button index first.
 *
 * @param button The button index, beginning at 1.
 * @return Whether the button was released since the last check.
 */
bool Joystick::GetButtonReleased(Button button) {
  return GetRawButtonReleased(static_cast<int>(button));
}

/**
 * Get buttons based on an enumerated type.
 *
 * The button type will be looked up in the list of buttons and then read.
 *
 * @param button The type of button to read.
 * @return The state of the button.
 */
bool Joystick::GetButton(ButtonType button) const {
  int temp = button;
  return GetRawButton(static_cast<int>(static_cast<Button>(temp)));
}

/**
 * Get the magnitude of the direction vector formed by the joystick's
 * current position relative to its origin.
 *
 * @return The magnitude of the direction vector
 */
double Joystick::GetMagnitude() const {
  return std::sqrt(std::pow(GetX(), 2) + std::pow(GetY(), 2));
}

/**
 * Get the direction of the vector formed by the joystick and its origin
 * in radians.
 *
 * @return The direction of the vector in radians
 */
double Joystick::GetDirectionRadians() const {
  return std::atan2(GetX(), -GetY());
}

/**
 * Get the direction of the vector formed by the joystick and its origin
 * in degrees.
 *
 * uses std::acos(-1) to represent Pi due to absence of readily accessible Pi
 * constant in C++
 *
 * @return The direction of the vector in degrees
 */
double Joystick::GetDirectionDegrees() const {
  return (180 / kPi) * GetDirectionRadians();
}
