/*
 * File:   oeLib.h
 * Author: yenda
 *
 * Created 2014/10
 */
#define testbit(var,bit) ((var) & (1<<(bit)))
#define setbit(var,bit) ((var) |=(1<<(bit))
#define clrbit(var,bit) ((var) &=~(1<<(bit)))

