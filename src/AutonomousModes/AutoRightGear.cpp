// Copyright (c) FRC Team 3512, Spartatroniks 2016-2017. All Rights Reserved.

#include "../Robot.hpp"
#include "../Subsystems/DriveTrain.hpp"

using namespace std::chrono_literals;

/* Moves forward, rotates, then moves forward again to hang gear on right side
 * of
 * airship as viewed from the Driver Station.
 */

enum class State { Idle, InitForward, Rotate, FinalForward };

void Robot::AutoRightGear() {
    robotDrive.StopClosedLoop();

    State state = State::Idle;

    shifter.Set(true);  // low gear
    gearPunch.Set(frc::DoubleSolenoid::kForward);

    bool SMHasRun = false;

    while (IsAutonomous() && IsEnabled() && !SMHasRun) {
        // Idle
        switch (state) {
            case State::Idle:
                robotDrive.ResetEncoders();
                robotDrive.ResetGyro();
                robotDrive.StartClosedLoop();
                robotDrive.SetPositionReference(114.3 - 39 /*robot length*/);
                robotDrive.SetAngleReference(0);
                state = State::InitForward;
                break;

            // Initial Forward
            case State::InitForward:
                std::cout << "InitForward: PosRef:"
                          << robotDrive.GetPosReference()
                          << " Pos: " << robotDrive.GetPosition() << std::endl;
                if (robotDrive.PosAtReference()) {
                    // Angle references are all scaled by 7 (don't ask why)
                    robotDrive.SetAngleReference(-45 / 7);

                    state = State::Rotate;
                }
                break;

            // Rotate
            case State::Rotate:
                std::cout << "Rotate: AngleRef:"
                          << robotDrive.GetAngleReference()
                          << " Angle: " << robotDrive.GetAngle() << std::endl;
                if (robotDrive.AngleAtReference()) {
                    state = State::FinalForward;
                    // Angle set to prevent overshoot
                    robotDrive.SetAngleReference(robotDrive.GetAngle());

                    // There is a race condition between resetting the encoders
                    // and setting the new position reference, but it's OK
                    // because the controller will drive in the correct
                    // direction during that time anyway.
                    robotDrive.ResetEncoders();
                    robotDrive.SetPositionReference((114.3 - 39) /
                                                    2 /*robot length*/);
                }
                break;

            // FinalForward
            case State::FinalForward:
                // If robot is at position reference or is driving backward
                // (could break robot by running into field wall), disable
                // closed loop control
                if (robotDrive.PosAtReference() ||
                    robotDrive.GetLeftDisplacement() < -5.0 ||
                    robotDrive.GetRightDisplacement() < -5.0) {
                    robotDrive.StopClosedLoop();
                    SMHasRun = true;
                }
                break;
        }
        DS_PrintOut();
        std::this_thread::sleep_for(10ms);
    }

    robotDrive.StopClosedLoop();
}
