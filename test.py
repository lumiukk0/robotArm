#!/usr/bin/python3
import RPi.GPIO as GPIO
import time
import threading

# GPIO Pins
out1_1 = 11
out1_2 = 12
out1_3 = 13
out1_4 = 16

# Variables
step_sleep = 0.002  # Time to wait between steps
step_count = 200    # Number of steps
running = True      # Motor running state
reverse = False     # Direction flag

# GPIO setup
GPIO.setmode(GPIO.BOARD)
GPIO.setup(out1_1, GPIO.OUT)
GPIO.setup(out1_2, GPIO.OUT)
GPIO.setup(out1_3, GPIO.OUT)
GPIO.setup(out1_4, GPIO.OUT)

GPIO.setup(out2_1, GPIO.OUT)
GPIO.setup(out2_2, GPIO.OUT)
GPIO.setup(out2_3, GPIO.OUT)
GPIO.setup(out2_4, GPIO.OUT)


# Initialize
GPIO.output(out1_1, GPIO.LOW)
GPIO.output(out1_2, GPIO.LOW)
GPIO.output(out1_3, GPIO.LOW)
GPIO.output(out1_4, GPIO.LOW)

GPIO.output(out2_1, GPIO.LOW)
GPIO.output(out2_2, GPIO.LOW)
GPIO.output(out2_3, GPIO.LOW)
GPIO.output(out2_4, GPIO.LOW)

def cleanup():
    GPIO.output(out1_1, GPIO.LOW)
    GPIO.output(out1_2, GPIO.LOW)
    GPIO.output(out1_3, GPIO.LOW)
    GPIO.output(out1_4, GPIO.LOW)
    
    GPIO.output(out2_1, GPIO.LOW)
    GPIO.output(out2_2, GPIO.LOW)
    GPIO.output(out2_3, GPIO.LOW)
    GPIO.output(out2_4, GPIO.LOW)
    GPIO.cleanup()

def motor_control1():
    global step_sleep, running, reverse
    i = 0
    try:
        while True:
            if running:
                if reverse:
                    i -= 1
                else:
                    i += 1

                if i % 4 == 0:
                    GPIO.output(out1_4, GPIO.HIGH)
                    GPIO.output(out1_3, GPIO.LOW)
                    GPIO.output(out1_2, GPIO.LOW)
                    GPIO.output(out1_1, GPIO.LOW)
                elif i % 4 == 1:
                    GPIO.output(out1_4, GPIO.LOW)
                    GPIO.output(out1_3, GPIO.LOW)
                    GPIO.output(out1_2, GPIO.HIGH)
                    GPIO.output(out1_1, GPIO.LOW)
                elif i % 4 == 2:
                    GPIO.output(out1_4, GPIO.LOW)
                    GPIO.output(out1_3, GPIO.HIGH)
                    GPIO.output(out1_2, GPIO.LOW)
                    GPIO.output(out1_1, GPIO.LOW)
                elif i % 4 == 3:
                    GPIO.output(out1_4, GPIO.LOW)
                    GPIO.output(out1_3, GPIO.LOW)
                    GPIO.output(out1_2, GPIO.LOW)
                    GPIO.output(out1_1, GPIO.HIGH)

                time.sleep(abs(step_sleep))
            else:
                time.sleep(0.1)  # Small delay when motor is off
    except KeyboardInterrupt:
        cleanup()
        exit(1)

def motor_control2():
    global step_sleep, running, reverse
    i = 0
    try:
        while True:
            if running:
                if reverse:
                    i -= 1
                else:
                    i += 1

                if i % 4 == 0:
                    GPIO.output(out2_4, GPIO.HIGH)
                    GPIO.output(out2_3, GPIO.LOW)
                    GPIO.output(out2_2, GPIO.LOW)
                    GPIO.output(out2_1, GPIO.LOW)
                elif i % 4 == 1:
                    GPIO.output(out2_4, GPIO.LOW)
                    GPIO.output(out2_3, GPIO.LOW)
                    GPIO.output(out2_2, GPIO.HIGH)
                    GPIO.output(out2_1, GPIO.LOW)
                elif i % 4 == 2:
                    GPIO.output(out2_4, GPIO.LOW)
                    GPIO.output(out2_3, GPIO.HIGH)
                    GPIO.output(out2_2, GPIO.LOW)
                    GPIO.output(out2_1, GPIO.LOW)
                elif i % 4 == 3:
                    GPIO.output(out2_4, GPIO.LOW)
                    GPIO.output(out2_3, GPIO.LOW)
                    GPIO.output(out2_2, GPIO.LOW)
                    GPIO.output(out2_1, GPIO.HIGH)

                time.sleep(abs(step_sleep))
            else:
                time.sleep(0.1)  # Small delay when motor is off
    except KeyboardInterrupt:
        cleanup()
        exit(1)


# Start motor control in a separate thread
motor_thread = threading.Thread(target=motor_control)
motor_thread.daemon = True
motor_thread.start()

# Main loop for user input
try:
    while True:
        command = input("Enter command (l: faster, s: slower, h: toggle, x: exit, r: reverse): ").strip().lower()

        if command == "l":
            step_sleep = max(step_sleep /2, 0.0001)  # Decrease step_sleep (faster)
            print(f"Speed increased, step_sleep: {step_sleep:.4f}")
        elif command == "s":
            step_sleep = min(step_sleep * 2 , 0.1)  # Increase step_sleep (slower)
            print(f"Speed decreased, step_sleep: {step_sleep:.4f}")
        elif command == "h":
            running = not running  # Toggle motor state
            print("Motor toggled", "ON" if running else "OFF")
        elif command == "r":
            reverse = not reverse  # Reverse motor direction
            print("Direction toggled", "Reverse" if reverse else "Forward")
        elif command == "x":
            print("Exiting...")
            cleanup()
            break
        else:
            print("Invalid command. Please enter l, s, h, x, or r.")

except KeyboardInterrupt:
    cleanup()
    exit(0)