/*
 * Switch.h
 * Copyright (C) 2012 ARM Limited
 *
 */

#ifndef SWITCH_H_
#define SWITCH_H_

typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;
typedef unsigned       __int64 uint64_t;

#define   __RO     volatile const
#define   __WO     volatile
#define   __RW     volatile

/* LED - Register Layout Typedef */
typedef struct {
  __RW uint32_t SWITCH_SOFT;           /*GPIO Register*/
} Switch_Type;

#define SWITCH_BASE       (0x1C010004u)
#define SWITCH            ((Switch_Type *)SWITCH_BASE)


extern unsigned int Get_Switch(void);
extern void Set_Switch(unsigned int val);

#endif /* LED_H_ */
