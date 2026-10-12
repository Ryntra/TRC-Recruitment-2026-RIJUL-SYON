from controller import Robot

def run_robot():
    robot = Robot()
    time_step = int(robot.getBasicTimeStep())
    max_speed = 6.28

    # Initialize Motors (Differential drive setup)
    left_motor = robot.getDevice('mtr_left')
    right_motor = robot.getDevice('mtr_right')
    left_motor.setPosition(float('inf'))
    right_motor.setPosition(float('inf'))
    left_motor.setVelocity(0.0)
    right_motor.setVelocity(0.0)

    # Initialize Infrared / Distance Sensors
    # ir0 = right sensor, ir1 = left sensor
    ir_right = robot.getDevice('ir0')
    ir_left = robot.getDevice('ir1')
    
    if ir_right:
        ir_right.enable(time_step)
    if ir_left:
        ir_left.enable(time_step)

    # Main control loop
    while robot.step(time_step) != -1:
        # Read sensor values (adjust threshold based on your surface reflection)
        right_val = ir_right.getValue() if ir_right else 0.0
        left_val = ir_left.getValue() if ir_left else 0.0

        # Define your threshold (e.g., transition between white floor and black line)
        THRESHOLD = 500.0

        left_speed = max_speed
        right_speed = max_speed

        # Line-following steering logic:
        # If the left sensor hits the line, steer left
        if left_val > THRESHOLD and right_val <= THRESHOLD:
            left_speed = max_speed * 0.2
            right_speed = max_speed
        # If the right sensor hits the line, steer right
        elif right_val > THRESHOLD and left_val <= THRESHOLD:
            left_speed = max_speed
            right_speed = max_speed * 0.2

        # Apply velocities to motors
        if left_motor:
            left_motor.setVelocity(left_speed)
        if right_motor:
            right_motor.setVelocity(right_speed)

if __name__ == "__main__":
    run_robot()