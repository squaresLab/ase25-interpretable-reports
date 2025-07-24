//FormAI DATASET v1.0 Category: Elevator Simulation ; Style: asynchronous
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

#define FLOORS 10
#define ELEVATORS 3

int elevator_positions[ELEVATORS] = { 0 };
bool elevator_directions[ELEVATORS] = { true }; //true for up, false for down
bool elevator_doors_open[ELEVATORS] = { false };

int floor_buttons_pressed[FLOORS] = { 0 };

void* elevator_thread_function(void* args)
{
    int elevator_num = *(int*) args; //Extracting the elevator number from the void pointer argument
    while (true) //Elevator's continuous loop
    {
        //Check if the elevator is moving or if its doors are open
        if (!elevator_doors_open[elevator_num])
        {
            //Check if any floor buttons have been pressed in the direction the elevator is moving
            bool pressed_button = false;
            for (int floor = 0; floor < FLOORS; floor++)
            {
                //Check if the button has been pressed and the elevator is going in the right direction
                if (floor_buttons_pressed[floor] > 0 && ((elevator_directions[elevator_num] && floor > elevator_positions[elevator_num])
                     || (!elevator_directions[elevator_num] && floor < elevator_positions[elevator_num])))
                {
                    pressed_button = true;
                    break;
                }
            }
            //If there are no pressed buttons in the elevator's direction, switch directions
            if (!pressed_button)
            {
                elevator_directions[elevator_num] = !elevator_directions[elevator_num];
            }
            //Move the elevator in the proper direction
            if (elevator_directions[elevator_num])
            {
                elevator_positions[elevator_num]++;
            }
            else
            {
                elevator_positions[elevator_num]--;
            }
            //Check if the elevator has arrived at a floor and needs to stop
            bool stop = false;
            for (int floor = 0; floor < FLOORS; floor++)
            {
                if (elevator_positions[elevator_num] == floor && floor_buttons_pressed[floor] > 0)
                {
                    stop = true;
                    break;
                }
            }
            if (stop)
            {
                elevator_doors_open[elevator_num] = true;
                printf("Elevator %d has arrived at floor %d.\n", elevator_num, elevator_positions[elevator_num]);
                sleep(2); //Open the doors for 2 seconds
                elevator_doors_open[elevator_num] = false;
                //Remove the button press for this floor
                floor_buttons_pressed[elevator_positions[elevator_num]] = 0;
            }
        }
        //Sleep the thread for 100 milliseconds (simulate moving between floors)
        usleep(100000);
    }
}

int main()
{
    pthread_t elevator_threads[ELEVATORS];
    int elevator_numbers[ELEVATORS] = { 0, 1, 2 };
    //Create the elevator threads
    for (int i = 0; i < ELEVATORS; i++)
    {
        if (pthread_create(&elevator_threads[i], NULL, elevator_thread_function, (void*) &elevator_numbers[i]))
        {
            printf("Error: Cannot create elevator thread %d.\n", i);
            exit(1);
        }
    }

    while (true) //Simulation loop for user pressing floor buttons
    {
        //Prompt for user input
        printf("Enter floor number (0 to exit): ");
        int floor;
        scanf("%d", &floor);
        if (floor < 0 || floor >= FLOORS) //Invalid floor number
        {
            printf("Invalid floor number.\n");
        }
        else if (floor == 0) //Exit simulation
        {
            break;
        }
        else //Valid floor number, add it to the queue
        {
            int min_queue_len = elevator_positions[0] - floor; //Calculate the minimum queue length for the first elevator
            int min_queue_elevator = 0; //This will store the index of the elevator with the shortest queue
            for (int i = 1; i < ELEVATORS; i++)
            {
                int queue_len = elevator_positions[i] - floor; //Calculate the queue length for this elevator
                if (queue_len * (elevator_directions[i] ? 1 : -1) < min_queue_len * (elevator_directions[min_queue_elevator] ? 1 : -1))
                {
                    min_queue_len = queue_len;
                    min_queue_elevator = i;
                }
            }
            floor_buttons_pressed[floor]++; //Add the button press to the queue
            printf("Button for floor %d pressed.\n", floor);
        }
    }
    return 0;
}