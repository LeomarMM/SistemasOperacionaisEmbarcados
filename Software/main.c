#include "kernel.h"
#include "user_tasks.h"

int main()
{
   config_os();
   
   // Criação das tarefas de usuário
   create_task(2, pisca_led_1);
   create_task(2, pisca_led_2);
   create_task(2, adc_temp);
   
   // Coloca o SO em execução
   start_os();
   
   while (1);
   
   return 0;
}
