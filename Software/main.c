#include "kernel.h"
#include "user_tasks.h"
int main()
{
    config_os();

    create_task(2, pisca_led_1);
    create_task(2, pisca_led_2);
    create_task(2, adc_temp);
   
    start_os();
   
    while (1);
   
    return 0;
}
