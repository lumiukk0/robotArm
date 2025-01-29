
// gcc -o stepper_control stepper_control.c -lwiringPi -lpthread
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // For usleep()
#include <pthread.h> // For threading

// GPIO pins for Motor 1
#define OUT1_1 0  // GPIO Pin 11 (wiringPi numbering)
#define OUT1_2 1  // GPIO Pin 12
#define OUT1_3 2  // GPIO Pin 13
#define OUT1_4 3  // GPIO Pin 16

// GPIO pins for Motor 2
#define OUT2_1 4
#define OUT2_2 5
#define OUT2_3 6
#define OUT2_4 7

// Variables
float stepSleep = 2000; // Microseconds to wait between steps (default 2ms)
int running = 1;         // Motor running state
int reverse = 0;         // Direction flag

void cleanup() {
    // Set all motor pins to LOW
    digitalWrite(OUT1_1, LOW);
    digitalWrite(OUT1_2, LOW);
    digitalWrite(OUT1_3, LOW);
    digitalWrite(OUT1_4, LOW);

    digitalWrite(OUT2_1, LOW);
    digitalWrite(OUT2_2, LOW);
    digitalWrite(OUT2_3, LOW);
    digitalWrite(OUT2_4, LOW);
}

void *motorControl1(void *arg) {
    int i = 0;
    while (1) {
        if (running) {
            if (reverse)
                i--;
            else
                i++;

            switch (i % 4) {
                case 0:
                    digitalWrite(OUT1_4, HIGH);
                    digitalWrite(OUT1_3, LOW);
                    digitalWrite(OUT1_2, LOW);
                    digitalWrite(OUT1_1, LOW);
                    break;
                case 1:
                    digitalWrite(OUT1_4, LOW);
                    digitalWrite(OUT1_3, LOW);
                    digitalWrite(OUT1_2, HIGH);
                    digitalWrite(OUT1_1, LOW);
                    break;
                case 2:
                    digitalWrite(OUT1_4, LOW);
                    digitalWrite(OUT1_3, HIGH);
                    digitalWrite(OUT1_2, LOW);
                    digitalWrite(OUT1_1, LOW);
                    break;
                case 3:
                    digitalWrite(OUT1_4, LOW);
                    digitalWrite(OUT1_3, LOW);
                    digitalWrite(OUT1_2, LOW);
                    digitalWrite(OUT1_1, HIGH);
                    break;
            }
            usleep((int)stepSleep);
        } else {
            usleep(100000); // Sleep 100ms when motor is off
        }
    }
    return NULL;
}

void *motorControl2(void *arg) {
    int i = 0;
    while (1) {
        if (running) {
            if (reverse)
                i--;
            else
                i++;

            switch (i % 4) {
                case 0:
                    digitalWrite(OUT2_4, HIGH);
                    digitalWrite(OUT2_3, LOW);
                    digitalWrite(OUT2_2, LOW);
                    digitalWrite(OUT2_1, LOW);
                    break;
                case 1:
                    digitalWrite(OUT2_4, LOW);
                    digitalWrite(OUT2_3, LOW);
                    digitalWrite(OUT2_2, HIGH);
                    digitalWrite(OUT2_1, LOW);
                    break;
                case 2:
                    digitalWrite(OUT2_4, LOW);
                    digitalWrite(OUT2_3, HIGH);
                    digitalWrite(OUT2_2, LOW);
                    digitalWrite(OUT2_1, LOW);
                    break;
                case 3:
                    digitalWrite(OUT2_4, LOW);
                    digitalWrite(OUT2_3, LOW);
                    digitalWrite(OUT2_2, LOW);
                    digitalWrite(OUT2_1, HIGH);
                    break;
            }
            usleep((int)stepSleep);
        } else {
            usleep(100000); // Sleep 100ms when motor is off
        }
    }
    return NULL;
}

int main() {
    // Initialize wiringPi
    if (wiringPiSetup() == -1) {
        printf("Failed to initialize wiringPi\n");
        return 1;
    }

    // Set motor pins to OUTPUT
    pinMode(OUT1_1, OUTPUT);
    pinMode(OUT1_2, OUTPUT);
    pinMode(OUT1_3, OUTPUT);
    pinMode(OUT1_4, OUTPUT);

    pinMode(OUT2_1, OUTPUT);
    pinMode(OUT2_2, OUTPUT);
    pinMode(OUT2_3, OUTPUT);
    pinMode(OUT2_4, OUTPUT);

    cleanup();

    // Start motor control threads
    pthread_t motor1Thread, motor2Thread;
    pthread_create(&motor1Thread, NULL, motorControl1, NULL);
    pthread_create(&motor2Thread, NULL, motorControl2, NULL);

    // Main loop for user input
    char command;
    while (1) {
        printf("Enter command (l: faster, s: slower, h: toggle, x: exit, r: reverse): ");
        scanf(" %c", &command);

        switch (command) {
            case 'l':
                stepSleep = stepSleep > 100.0 ? stepSleep - 100.0 : 100.0; // Minimum 100us
                printf("Speed increased, stepSleep: %.2f us\n", stepSleep);
                break;
            case 's':
                stepSleep = stepSleep < 100000.0 ? stepSleep + 100.0 : 100000.0; // Maximum 100ms
                printf("Speed decreased, stepSleep: %.2f us\n", stepSleep);
                break;
            case 'h':
                running = !running;
                printf("Motor toggled %s\n", running ? "ON" : "OFF");
                break;
            case 'r':
                reverse = !reverse;
                printf("Direction toggled %s\n", reverse ? "Reverse" : "Forward");
                break;
            case 'x':
                printf("Exiting...\n");
                cleanup();
                exit(0);
                break;
            default:
                printf("Invalid command. Please enter l, s, h, x, or r.\n");
        }
    }

    return 0;
}
