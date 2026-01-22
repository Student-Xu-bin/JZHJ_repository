#ifndef __APP_TASKS_H__
#define __APP_TASKS_H__

void task_modbus(void *arg);
void task_detect(void *arg);
void task_work(void *arg);
void task_motor_current_init(void *arg);
void task_motor_current_detect(void *arg);

unsigned char app_task_get_full_state(unsigned char no);
void full_light_green(void *arg);
void full_light_red(void *arg);
void motor_open(void *arg);
void motor_stop(void *arg);
void motor_close(void *arg);
unsigned char app_task_get_close_state(unsigned char no);
unsigned char app_task_get_open_state(unsigned char no);


#endif /* __APP_TASKS_H__ */
