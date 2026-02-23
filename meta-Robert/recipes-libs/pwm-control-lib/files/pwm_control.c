#include <stdio.h>

void hello_world() {
    printf("Hello, from shared library!\n");
}


const static char * pwm_device_name = "pwm0"; 
const static char * pwm_device_path = "/sys/class/pwm/pwmchip0"; 
char file_path[256];
FILE * fp;

int pwm_control_init_device() {
    printf("Initializing PWM control device...\n");

    // export the pwm device
    snprintf(file_path, sizeof(file_path), "%s/export", pwm_device_path);
    
    // try to open the export file
    fp = fopen(file_path, "w");
    if (fp == NULL) {
        printf("Failed to open PWM export path: %s\n", file_path);
        return -1;
    } else {
        printf("PWM export path opened successfully: %s\n", file_path);
    }

    // write "0" to the export file to export the pwm device
    fprintf(fp, "0");
    fclose(fp);

    snprintf(file_path, 
        sizeof(file_path), 
        "%s/%s/%s", pwm_device_path, pwm_device_name, "enable");

    // try to open the pwm device path
    fp = fopen(file_path, "w");
    if (fp == NULL) {
        printf("Failed to open PWM device path: %s\n", file_path);
        return -1;
    } else {
        printf("PWM device path opened successfully: %s\n", file_path);
    }

    // write "1" to the enable file to enable the pwm device
    fprintf(fp, "1");
    fclose(fp);

    return 0;
}


int pwm_control_set_period(int period_ns) {
    printf("Setting PWM period to %d ns...\n", period_ns);


    snprintf(file_path, 
        sizeof(file_path), 
        "%s/%s/%s", pwm_device_path, pwm_device_name, "period");

    // try to open the period file
    FILE *fp = fopen(file_path, "w");
    if (fp == NULL) {
        printf("Failed to open PWM period path: %s\n", file_path);
        return -1;
    } else {
        printf("PWM period path opened successfully: %s\n", file_path);
    }

    // write the period value to the period file
    fprintf(fp, "%d", period_ns);
    fclose(fp);

    return 0;
}

int pwm_control_set_duty_cycle(int duty_cycle_ns) {
    printf("Setting PWM duty cycle to %d ns...\n", duty_cycle_ns);

    snprintf(file_path, 
        sizeof(file_path), 
        "%s/%s/%s", pwm_device_path, pwm_device_name, "duty_cycle");

    // try to open the duty cycle file
    FILE *fp = fopen(file_path, "w");
    if (fp == NULL) {
        printf("Failed to open PWM duty cycle path: %s\n", file_path);
        return -1;
    } else {
        printf("PWM duty cycle path opened successfully: %s\n", file_path);
    }

    // write the duty cycle value to the duty cycle file
    fprintf(fp, "%d", duty_cycle_ns);
    fclose(fp);

    return 0;
}