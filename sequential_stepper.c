// gcc -o sequential_stepper sequential_stepper.c -lwiringPi

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // For usleep()

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
int reverse = 0;         // Direction flag

void rotateMotor(int motor) {
    int i;
    for (i = 0; i < 4; i++) { // One full cycle (8 steps for full rotation)
        int step = reverse ? (3 - i) : i;
        
        if (motor == 1) {
            digitalWrite(OUT1_1, step % 4 == 0);
            digitalWrite(OUT1_2, step % 4 == 1);
            digitalWrite(OUT1_3, step % 4 == 2);
            digitalWrite(OUT1_4, step % 4 == 3);
        } else {
            digitalWrite(OUT2_1, step % 4 == 0);
            digitalWrite(OUT2_2, step % 4 == 1);
            digitalWrite(OUT2_3, step % 4 == 2);
            digitalWrite(OUT2_4, step % 4 == 3);
        }
        usleep((int)stepSleep);
    }
}

int main() {
    if (wiringPiSetup() == -1) {
        printf("Failed to initialize wiringPi\n");
        return 1;
    }

    pinMode(OUT1_1, OUTPUT);
    pinMode(OUT1_2, OUTPUT);
    pinMode(OUT1_3, OUTPUT);
    pinMode(OUT1_4, OUTPUT);
    pinMode(OUT2_1, OUTPUT);
    pinMode(OUT2_2, OUTPUT);
    pinMode(OUT2_3, OUTPUT);
    pinMode(OUT2_4, OUTPUT);

    char command;
    while (1) {
        printf("Enter command (m: move one cycle, l: faster, s: slower, r: reverse, x: exit): ");
        scanf(" %c", &command);

        switch (command) {
            case 'm':
                rotateMotor(1);
                break;
            case 'l':
                stepSleep = stepSleep > 100.0 ? stepSleep - 100.0 : 100.0;
                printf("Speed increased, stepSleep: %.2f us\n", stepSleep);
                break;
            case 's':
                stepSleep = stepSleep < 100000.0 ? stepSleep + 100.0 : 100000.0;
                printf("Speed decreased, stepSleep: %.2f us\n", stepSleep);
                break;
            case 'r':
                reverse = !reverse;
                printf("Direction toggled %s\n", reverse ? "Reverse" : "Forward");
                break;
            case 'x':
                printf("Exiting...\n");
                exit(0);
                break;
            default:
                printf("Invalid command. Use m, l, s, r, or x.\n");
        }
    }
    return 0;
}
