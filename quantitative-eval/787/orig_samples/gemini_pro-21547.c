//GEMINI-pro DATASET v1.0 Category: Robot movement control ; Style: high level of detail
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define NUM_SERVOS      4
#define NUM_MOTORS      2

enum direction {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    STOP
};

typedef struct {
    int16_t position;
    int16_t velocity;
    int16_t acceleration;
    bool enabled;
} servo_t;

typedef struct {
    int16_t speed;
    bool enabled;
} motor_t;

typedef struct {
    servo_t servos[NUM_SERVOS];
    motor_t motors[NUM_MOTORS];
} robot_t;

void init_robot(robot_t *robot) {
    for (int i = 0; i < NUM_SERVOS; i++) {
        robot->servos[i].position = 0;
        robot->servos[i].velocity = 0;
        robot->servos[i].acceleration = 0;
        robot->servos[i].enabled = false;
    }

    for (int i = 0; i < NUM_MOTORS; i++) {
        robot->motors[i].speed = 0;
        robot->motors[i].enabled = false;
    }
}

void set_servo_position(robot_t *robot, uint8_t servo_id, int16_t position) {
    if (servo_id >= 0 && servo_id < NUM_SERVOS) {
        robot->servos[servo_id].position = position;
    }
}

void set_servo_velocity(robot_t *robot, uint8_t servo_id, int16_t velocity) {
    if (servo_id >= 0 && servo_id < NUM_SERVOS) {
        robot->servos[servo_id].velocity = velocity;
    }
}

void set_servo_acceleration(robot_t *robot, uint8_t servo_id, int16_t acceleration) {
    if (servo_id >= 0 && servo_id < NUM_SERVOS) {
        robot->servos[servo_id].acceleration = acceleration;
    }
}

void enable_servo(robot_t *robot, uint8_t servo_id) {
    if (servo_id >= 0 && servo_id < NUM_SERVOS) {
        robot->servos[servo_id].enabled = true;
    }
}

void disable_servo(robot_t *robot, uint8_t servo_id) {
    if (servo_id >= 0 && servo_id < NUM_SERVOS) {
        robot->servos[servo_id].enabled = false;
    }
}

void set_motor_speed(robot_t *robot, uint8_t motor_id, int16_t speed) {
    if (motor_id >= 0 && motor_id < NUM_MOTORS) {
        robot->motors[motor_id].speed = speed;
    }
}

void enable_motor(robot_t *robot, uint8_t motor_id) {
    if (motor_id >= 0 && motor_id < NUM_MOTORS) {
        robot->motors[motor_id].enabled = true;
    }
}

void disable_motor(robot_t *robot, uint8_t motor_id) {
    if (motor_id >= 0 && motor_id < NUM_MOTORS) {
        robot->motors[motor_id].enabled = false;
    }
}

void move_forward(robot_t *robot) {
    set_motor_speed(robot, 0, 100);
    set_motor_speed(robot, 1, 100);
    enable_motor(robot, 0);
    enable_motor(robot, 1);
}

void move_backward(robot_t *robot) {
    set_motor_speed(robot, 0, -100);
    set_motor_speed(robot, 1, -100);
    enable_motor(robot, 0);
    enable_motor(robot, 1);
}

void turn_left(robot_t *robot) {
    set_motor_speed(robot, 0, 100);
    set_motor_speed(robot, 1, -100);
    enable_motor(robot, 0);
    enable_motor(robot, 1);
}

void turn_right(robot_t *robot) {
    set_motor_speed(robot, 0, -100);
    set_motor_speed(robot, 1, 100);
    enable_motor(robot, 0);
    enable_motor(robot, 1);
}

void stop(robot_t *robot) {
    disable_motor(robot, 0);
    disable_motor(robot, 1);
}

int main(void) {
    robot_t robot;

    init_robot(&robot);

    move_forward(&robot);
    sleep(1);
    stop(&robot);

    turn_left(&robot);
    sleep(1);
    stop(&robot);

    move_backward(&robot);
    sleep(1);
    stop(&robot);

    turn_right(&robot);
    sleep(1);
    stop(&robot);

    return 0;
}