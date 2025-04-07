#include "stdio.h"
#include "string.h"
#include "stdbool.h"

char car;
char str1[100];
char LED1;
char LED2;
char LED3;
char LED1off;
char LED2off;
char LED3off;
int index = 0;

int main()

if (HAL_UART_Receive(&huart3, &car, 1, 100) == HAL_OK){
        if(car == "\r"){
            str1[index] = '\0';
            if(str_compare(str1,LED1) == true || str_compare(str1,LED1off) == true){
                HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
            }
            else if(str_compare(str1,LED2) == true || str_compare(str1,LED2off) == true){
                HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
            }
            else if(str_compare(str1,LED3) == true || str_compare(str1,LED3off) == true){
                HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
            }
        memset(str1, 0, sizeof(str1));
        index = 0;
        }
        else{
            str1[index] = car;
            index ++;
        }
}