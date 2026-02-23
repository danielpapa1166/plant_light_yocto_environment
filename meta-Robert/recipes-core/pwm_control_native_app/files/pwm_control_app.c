#include <stdio.h>
#include "pwm_control.h"

int main(int argc, char *argv[]) {
    printf("This is a test program for the PWM control library.\n");
    // printf the arguments passed to the program
    printf("Arguments passed to the program:\n");
    for (int i = 0; i < argc; i++) {
        printf("  argv[%d]: %s\n", i, argv[i]);
    }

    hello_world();


    pwm_control_init_device();

    int period_ns = atoi(argv[1]); // 1 ms
    int duty_cycle_ns = atoi(argv[2]); // 0.5 ms

    printf("Setting PWM period to %d ns and duty cycle to %d ns...\n", 
        period_ns, duty_cycle_ns);

    pwm_control_set_period(period_ns); // Set period to 1 ms (1,000,000 ns)
    pwm_control_set_duty_cycle(duty_cycle_ns); // Set duty cycle to 0.5 ms (500,000 ns)

    return 0;
}