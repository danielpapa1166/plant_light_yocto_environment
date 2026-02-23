#ifndef PWM_CONTROL_H
#define PWM_CONTROL_H

void hello_world();

int pwm_control_init_device();
int pwm_control_set_period(int period_ns);
int pwm_control_set_duty_cycle(int duty_cycle_ns);

#endif // PWM_CONTROL_H
