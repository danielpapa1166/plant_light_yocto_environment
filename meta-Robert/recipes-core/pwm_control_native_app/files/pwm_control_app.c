#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pwm_control.h"

int main(int argc, char *argv[]) {
    printf("This is a test program for the PWM control library.\n");
    // printf the arguments passed to the program
    printf("Arguments passed to the program:\n");
    for (int i = 0; i < argc; i++) {
        printf("  argv[%d]: %s\n", i, argv[i]);
    }

    // parse input argument: 
    // -p: period in ns
    // -d: duty cycle in ns
    // -P: duty cycle in percent
    // -k: kill device (unexport pwm device)

    int opt; 
    int duty_percent = -1;
    int period_ns = -1;
    int duty_cycle_ns = -1;
    int kill_device = 0;

    while ((opt = getopt(argc, argv, "p:d:P:k")) != -1) {
        switch (opt) {
            case 'p':
                printf("Period argument: %s\n", optarg);
                period_ns = atoi(optarg);
                break;
            case 'd':
                printf("Duty cycle argument: %s\n", optarg);
                duty_cycle_ns = atoi(optarg);
                break;
            case 'P':
                printf("Duty cycle percent argument: %s\n", optarg);
                duty_percent = atoi(optarg);
                break;
            case 'k':
                printf("Kill device argument received.\n");
                kill_device = 1;
                break;
            default:
                printf("Invalid argument: %c\n", opt);
                return -1;
        }
    }

    if(kill_device) {
        printf("Deinitializing PWM control device...\n");
        int deinit_retval = pwm_control_deinit_device();
        if (deinit_retval != 0) {
            printf("Failed to deinitialize PWM control device. "
                "Error code: %d\n", deinit_retval);
            return -1;
        }

        int is_enabled = pwm_control_isenabled();
        if (is_enabled < 0) {
            printf("Failed to read PWM enabled state from device.\n");
        }
        else if (is_enabled == 0) {
            printf("PWM is currently disabled.\n");
        }
        else {
            printf("PWM is currently enabled. There may be an issue with "
                "disabling the device.\n");
        }

        return 0;
    }


    if(duty_percent < 0 && duty_cycle_ns < 0) {
        printf("Error: Please specify either duty cycle in ns " 
            "(-d) or duty cycle in percent (-P).\n");
        return -1;
    }

    // init device: 
    int dev_retval = 0; 
    dev_retval = pwm_control_init_device();
    if (dev_retval != 0) {
        printf("Failed to initialize PWM control device. "
            "Error code: %d\n", dev_retval);
        return -1;
    }

    if(duty_percent >= 0) {
        // set duty cycle in percent: 
        printf("Setting PWM duty cycle to %d%%...\n", duty_percent);
        dev_retval = pwm_control_set_duty_percent(duty_percent);
        if (dev_retval != 0) {
            printf("Failed to set PWM duty cycle percentage. "
                "Error code: %d\n", dev_retval);
            return -1;  
        }
    }
    else if(duty_cycle_ns >= 0 && period_ns >= 0) {
        // setting pwm period and duty cycle in ns:

        printf("Setting PWM period to %d ns "
            "and duty cycle to %d ns...\n", 
            period_ns, duty_cycle_ns);

        dev_retval = pwm_control_set_period(period_ns); 
        if (dev_retval == -2) {
            // fixed frequency mode: 
            // read period for informational purposes:
            int current_period_ns = pwm_control_get_period();
            if(current_period_ns < 0) {
                printf("Failed to read current PWM period from device.\n");
            }
            else {
                int current_freq_hz = 1000000000 / current_period_ns;
                printf("WARNING: Cannot set PWM period. "
                    "The frequency is fixed at %d Hz.\n", current_freq_hz);
            }
        }

        // set duty cycle ns: 
        dev_retval = pwm_control_set_duty_cycle(duty_cycle_ns);
        if (dev_retval != 0) {
            printf("Failed to set PWM duty cycle. Error code: %d\n", 
                dev_retval);
            return -1;  
        }
    }
    else {
        printf("Error: If duty cycle in ns is specified, "
            "period in ns must also be specified.\n");
        return -1;
     
    }

    // Check if PWM is enabled
    int enabled = pwm_control_isenabled();
    if (enabled < 0) {
        printf("Failed to read PWM enabled state from device.\n");
    }
    else if (enabled == 0) {
        printf("PWM is currently disabled.\n");
    }
    else {
        printf("PWM is currently \n\n\nenabled.\n\n\n");
    }

    return 0;
}