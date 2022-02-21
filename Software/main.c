#include "io.h"
#include "kernel.h"
#include "user_tasks.h"

int main()
{
    hardware_init();
    
    config_os();

    create_task(1, climate_control);
   
    start_os();
   
    while (1);
   
    return 0;
}
