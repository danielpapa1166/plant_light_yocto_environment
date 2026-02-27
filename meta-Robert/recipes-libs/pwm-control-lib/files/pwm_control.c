#include <stdio.h>


#ifndef USE_DEFAULT_PWM_FREQUENCY
#define USE_DEFAULT_PWM_FREQUENCY 1
#endif

#ifndef PWM_DEBUG
#define PWM_DEBUG 0
#endif

const static char * pwm_device_name = "pwm0"; 
const static char * pwm_device_path = "/sys/class/pwm/pwmchip0"; 
char file_path[256];
FILE * fp;

// Set default PWM frequency to 1 kHz (1 ms period)
static const int default_period_ns = 1000000; // 1 ms in nanoseconds


static int pwm_control_write_file(const char * path, int value) {
    FILE *fp = fopen(path, "w");
    if (fp == NULL) {
        #if PWM_DEBUG == 1
            printf("Failed to open file for writing: %s\n", path);
        #endif
        return -1;
    }
    fprintf(fp, "%d", value);
    fclose(fp);
    return 0;
}

static int pwm_control_set_enabled(int enabled) {
    snprintf(file_path, 
        sizeof(file_path), 
        "%s/%s/%s", pwm_device_path, pwm_device_name, "enable");

    return pwm_control_write_file(file_path, enabled);
}



int pwm_control_init_device() {
    int retval = 0; 
    #if PWM_DEBUG == 1
        printf("Initializing PWM control device...\n");
    #endif

    // export the pwm device
    snprintf(file_path, sizeof(file_path), "%s/export", pwm_device_path);
    
    // Export pwm0 (assuming pwm0 is at index 0)
    retval = pwm_control_write_file(file_path, 0); 
    if(retval != 0) {
        #if PWM_DEBUG == 1
            printf("Failed to export PWM device. Error code: %d\n", retval);
        #endif
        return -1;
    }

    // set default PWM frequency:
    if(USE_DEFAULT_PWM_FREQUENCY) {
        // set default PWM frequency: 
        retval = pwm_control_set_period(default_period_ns);
        if(retval == -2) {
            retval = 0; // reset retval to 0 
            // since we're treating fixed frequency as a non-error case
        }
    }

    return retval;
}


int pwm_control_set_period(int period_ns) {
    if(USE_DEFAULT_PWM_FREQUENCY) {
        #if PWM_DEBUG == 1
            printf("Using default PWM frequency of %d Hz "
                "(period: %d ns)\n", 
                1000000000 / default_period_ns, 
                default_period_ns);
        #endif

        period_ns = default_period_ns; // Override period with default value
    }

    #if PWM_DEBUG == 1
        printf("Setting PWM period to %d ns...\n", period_ns);
    #endif

    snprintf(file_path, 
        sizeof(file_path), 
        "%s/%s/%s", pwm_device_path, pwm_device_name, "period");

    // write period value to the period file:
    int retval = pwm_control_write_file(file_path, period_ns);

    if(retval == 0 && USE_DEFAULT_PWM_FREQUENCY) {
        // no error but indicate the frequency 
        // is fixed and period cannot be changed:
        retval = -2; 
    }
    return retval;
}

int pwm_control_set_duty_cycle(int duty_cycle_ns) {
    #if PWM_DEBUG == 1
        printf("Setting PWM duty cycle to %d ns...\n", duty_cycle_ns);
    #endif

    snprintf(file_path, 
        sizeof(file_path), 
        "%s/%s/%s", pwm_device_path, pwm_device_name, "duty_cycle");

    int retval = pwm_control_write_file(file_path, duty_cycle_ns);

    // Enable PWM after setting duty cycle
    retval |= pwm_control_set_enabled(1); 

    return retval;
}

int pwm_control_set_duty_percent(int duty_percent) {
    if(duty_percent < 0 || duty_percent > 100) {
        #if PWM_DEBUG == 1
            printf("Invalid duty cycle percentage: %d. "
                "Must be between 0 and 100.\n", 
                duty_percent);
        #endif
        return -1;
    }

    int period_ns = 0;

    if(USE_DEFAULT_PWM_FREQUENCY) {
        #if PWM_DEBUG == 1
            printf("Using default PWM frequency of %d Hz "
                "(period: %d ns)\n", 
                1000000000 / default_period_ns, 
                default_period_ns);
        #endif
        period_ns = default_period_ns; // Assuming default period is used
    }
    else {
        // Read the current period from the device
        period_ns = pwm_control_get_period();
        if (period_ns < 0) {
            #if PWM_DEBUG == 1
                printf("Failed to read PWM period from device.\n");
            #endif
            return -1;
        }

        #if PWM_DEBUG == 1
            printf("Current PWM period read from device: %d ns\n", period_ns);
        #endif
    }

    int duty_cycle_ns = (period_ns * duty_percent) / 100;

    return pwm_control_set_duty_cycle(duty_cycle_ns);
}


int pwm_control_get_period() {
    snprintf(file_path, 
        sizeof(file_path), 
        "%s/%s/%s", pwm_device_path, pwm_device_name, "period");

    FILE *fp = fopen(file_path, "r");
    if (fp == NULL) {
        #if PWM_DEBUG == 1
            printf("Failed to open PWM period path for reading: %s\n", 
                file_path);
        #endif
        return -1;
    }

    int period_ns = 0;
    fscanf(fp, "%d", &period_ns);
    fclose(fp);

    return period_ns;
}


int pwm_control_isenabled() {
    snprintf(file_path, 
        sizeof(file_path), 
        "%s/%s/%s", pwm_device_path, pwm_device_name, "enable");

    FILE *fp = fopen(file_path, "r");
    if (fp == NULL) {
        #if PWM_DEBUG == 1
            printf("Failed to open PWM enable path for reading: %s\n", 
                file_path);
        #endif
        return -1;
    }

    int enabled = 0;
    fscanf(fp, "%d", &enabled);
    fclose(fp);

    return enabled;
}


int pwm_control_deinit_device() {
    #if PWM_DEBUG == 1
        printf("Deinitializing PWM control device...\n");
    #endif

    // disable pwm before unexporting
    int retval = pwm_control_set_enabled(0);

    // unexport the pwm device
    snprintf(file_path, sizeof(file_path), "%s/unexport", pwm_device_path);
    
    // Unexport pwm0 (assuming index 0)
    retval |= pwm_control_write_file(file_path, 0); 
    return retval;
}