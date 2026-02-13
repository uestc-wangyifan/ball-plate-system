#ifndef __KEY_H
#define __KEY_H

#define key0 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)
#define key1 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14)
#define key2 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_15)
void Key_init(void);
void Key_scanf(void);

#endif
