// Copyright (c) FRC Team 3512, Spartatroniks 2017. All Rights Reserved.

#include "Robot.hpp"

using namespace std::chrono_literals;

Robot::Robot() {
    dsDisplay.AddAutoMethod("No-op", &Robot::AutoNoop, this);
    dsDisplay.AddAutoMethod("LeftGear", &Robot::AutoLeftGear, this);
    dsDisplay.AddAutoMethod("CenterGear", &Robot::AutoCenterGear, this);
}

void Robot::OperatorControl() {
    while (IsEnabled() && IsOperatorControl()) {
        if (driveStick1.GetTrigger()) {
            robotDrive.Drive(driveStick1.GetY() * 0.5, driveStick2.GetX() * 0.5,
                             driveStick2.GetRawButton(2));
        } else {
            robotDrive.Drive(driveStick1.GetY(), driveStick2.GetX(),
                             driveStick2.GetRawButton(2));
        }
        if (grabberStick.GetPOV() == 0) {
            robotWinch.Set(1.0);
        } else if (grabberStick.GetPOV() == 180) {
            robotWinch.Set(-1.0);
        }
        GPU.Pickup();
    }
}

void Robot::Autonomous() {
    autoTimer.Reset();
    autoTimer.Start();

    robotDrive.ResetEncoders();
    dsDisplay.ExecAutonomous();
}

void Robot::Disabled() {
    while (IsDisabled()) {
        DS_PrintOut();
        std::this_thread::sleep_for(10ms);
    }
}

void Robot::Test() {
    while (IsEnabled() && IsTest()) {
        DS_PrintOut();
        std::this_thread::sleep_for(10ms);
    }
}

void Robot::DS_PrintOut() {}

START_ROBOT_CLASS(Robot)
