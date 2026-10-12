#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/keyboard.h>
#include <math.h>

#define SPEED_VAL 5.0  // Fixed speed constant

int main() {
    wb_robot_init();
    int time_step = (int)wb_robot_get_basic_time_step();
    wb_keyboard_enable(time_step);

    // Get motor devices
    WbDeviceTag fl = wb_robot_get_device("mtr_front_left");
    WbDeviceTag fr = wb_robot_get_device("mtr_front_right");
    WbDeviceTag bl = wb_robot_get_device("mtr_back_left");
    WbDeviceTag br = wb_robot_get_device("mtr_back_right");

    // Set all motors to velocity control mode
    WbDeviceTag motors[4] = {fl, fr, bl, br};
    for (int i = 0; i < 4; i++) {
        if (motors[i] != 0) {
            wb_motor_set_position(motors[i], INFINITY);
        }
    }

    // Motion state tracking (0 = Stop, 1 = Forward, 2 = Reverse, 3 = Turn Left, 4 = Turn Right)
    int current_mode = 0;

    while (wb_robot_step(time_step) != -1) {
        // Read any new key presses from the queue
        int key;
        while ((key = wb_keyboard_get_key()) > 0) {
            switch (key) {
                case 'W':
                case 'w':
                    current_mode = 1;
                    break;
                case 'S':
                case 's':
                    current_mode = 2;
                    break;
                case 'A':
                case 'a':
                    current_mode = 3;
                    break;
                case 'D':
                case 'd':
                    current_mode = 4;
                    break;
                case ' ':
                    current_mode = 0; // Stop on Spacebar
                    break;
                default:
                    break;
            }
        }

        // Determine speeds based on the persistent mode
        double left_speed = 0.0;
        double right_speed = 0.0;

        switch (current_mode) {
            case 1: // Forward
                left_speed = SPEED_VAL;
                right_speed = SPEED_VAL;
                break;
            case 2: // Reverse
                left_speed = -SPEED_VAL;
                right_speed = -SPEED_VAL;
                break;
            case 3: // Turn Left (Pivot)
                left_speed = -SPEED_VAL;
                right_speed = SPEED_VAL;
                break;
            case 4: // Turn Right (Pivot)
                left_speed = SPEED_VAL;
                right_speed = -SPEED_VAL;
                break;
            case 0:
            default: // Stop
                left_speed = 0.0;
                right_speed = 0.0;
                break;
        }

        // Apply velocities to the motors
        if (fl) wb_motor_set_velocity(fl, left_speed);
        if (bl) wb_motor_set_velocity(bl, left_speed);
        if (fr) wb_motor_set_velocity(fr, right_speed);
        if (br) wb_motor_set_velocity(br, right_speed);
    }

    wb_robot_cleanup();
    return 0;
}