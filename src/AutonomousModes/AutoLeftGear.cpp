// Copyright (c) FRC Team 3512, Spartatroniks 2016-2017. All Rights Reserved.

#include "../Robot.hpp"
#include "../Subsystems/DriveTrain.hpp"

using namespace std::chrono_literals;

/* Moves forward, rotates, then moves forward again to hang gear on left side of
 * airship as viewed from the Driver Station.
 */
void Robot::AutoLeftGear() {

    shifter.Set(true);  // low gear
    StateMachine leftGear("LeftGear");

    // Idle
    auto state = std::make_unique<State>("Idle");
    state->Entry = [this] {
        robotDrive.ResetGyro();
        robotDrive.ResetEncoders();
    };
    state->CheckTransition = [this](const std::string& event) {
        return "Initial-Forward";
    };
    state->Exit = [this] {
    	std::cout << "Exit Idle" << std::endl;
    	robotDrive.StartClosedLoop(); };

    // Init-Forward
    state = std::make_unique<State>("Initial-Forward");
    state->Entry = [this] {
    	std::cout << "Entry" << std::endl;
    	std::cout << "Reference Before: " << robotDrive.GetPosReference() << std::endl;
        robotDrive.SetPositionReference(114.3 - 39 /*robot length*/);
    	std::cout << "Reference After: " << robotDrive.GetPosReference() << std::endl;
    };
    state->CheckTransition = [this](const std::string& event) {
    	std::cout << "Check transition" << std::endl;
    	std::cout << "Position: " << robotDrive.GetPosition() << std::endl;
    	std::cout << "Reference: " << robotDrive.GetPosReference() << std::endl;
        if (robotDrive.PosAtReference()) {
            return "Rotate";
        } else {
            return "";
        }
    };
    leftGear.AddState(std::move(state));

    // Rotate TODO: Add PID function for rotation
    state = std::make_unique<State>("Rotate");
    state->Entry = [this] {
        robotDrive.SetAngleReference(45.0);

    };
    state->CheckTransition = [this](const std::string& event) {
        if (robotDrive.AngleAtReference()) {
            return "Final-Forward";
        } else {
            return "";
        }
    };
    leftGear.AddState(std::move(state));

    // Final-Forward
    state = std::make_unique<State>("Final-Forward");
    state->Entry = [this] {
        robotDrive.ResetEncoders();
        robotDrive.SetPositionReference(114.3 - 39 /*robot length*/);
    };
    state->CheckTransition = [this](const std::string& event) {
        if (robotDrive.PosAtReference()) {
            return "Idle";
        } else {
            return "";
        }
    };
    leftGear.AddState(std::move(state));
    leftGear.Run();

    while (IsAutonomous() && IsEnabled() &&
           leftGear.StackTrace() != "leftGear > Idle") {
        leftGear.Run();
        leftGear.HandleEvent("");
        DS_PrintOut();

        std::this_thread::sleep_for(10ms);
    }
    robotDrive.StopClosedLoop();
}
