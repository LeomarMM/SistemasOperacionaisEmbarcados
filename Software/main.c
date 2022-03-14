#include "kernel.h"
#include "user_tasks.h"

int main()
{
   config_os();
   
   // Cria��o das tarefas de usu�rio
   create_task(2, pisca_led_1);
   create_task(2, pisca_led_2);
   create_task(2, pisca_led_3);
   
   // Coloca o SO em execu��o
   start_os();
   
   while (1);
   
   return 0;
}
