#include "stm32f10x.h"     // Device header
#include "Delay.h"			//包含Delay.h头文件
#include "beep.h" 

int main(void)
 {    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
    BEEP_Init(); 

    while(1)
    {                
       play_music();        
    }    
}
