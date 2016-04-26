/* 
 * File:   config.h
 * Author: Uživatel
 *
 * Created on 15. duben 2014, 17:30
 */

#ifndef CONFIG_H
#define	CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

//==============================================================================
//Config1 FOSC_INTOSC, WDTE_OFF, PWRTE_OFF, MCLRE_ON, CP_OFF, CPD_OFF, BOREN_OFF, CLKOUTEN_OFF,_IESO_OFF, FCMEN_OFF
//Config2 WRT_OFF, PLLEN_OFF, STVREN_OFF, BORV_25, LVP_OFF

#pragma config FOSC = INTOSC
#pragma config WDTE = OFF           // Watchdog Timer Enable bit
#pragma config PWRTE = OFF          // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF          //? MCLR Pin Function Select bit (MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF             // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF            // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF          // Brown Out Detect (BOR enabled)
#pragma config CLKOUTEN = OFF
#pragma config IESO = OFF           // Internal External Switchover bit (Internal External Switchover mode is disabled)
#pragma config FCMEN = OFF          // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)

//==============================================================================

#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_H */

