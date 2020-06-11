/*
 * Umbrella Rental System
 * Copyright 2020 a5768549
 *
 * GitHub Project：https://github.com/a5768549/Umbrella-Rental-System
 */

#include <TM1637Display.h>

#define CLK   26
#define DIO   25

TM1637Display display(CLK, DIO);

uint8_t Seg[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x27,0x7F,0x6F};//0123456789

uint8_t Find_Seg(String num)
{
  int tmp = num.toInt();
  return Seg[tmp];
}

uint8_t status_no_wifi[] = {0x7F,0x7F,0x7F,0x7F}; //8888 Esp init
uint8_t status_0[]       = {0x40,0x40,0x40,0x40}; //---- Default
uint8_t status_1[]       = {0x39,0x6D,0x40,0x06}; //CS-1 Write
uint8_t status_S[]       = {0x39,0x6D,0x40,0x6D}; //CS-S Success
uint8_t status_n[]       = {0x39,0x6D,0x40,0x54}; //CS-n not us card
uint8_t status_b[]       = {0x39,0x6D,0x40,0x7C}; //CS-n already borrow
