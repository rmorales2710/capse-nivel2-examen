#ifndef MEFPUERTAS_H
#define MEFPUERTAS_H

#include "sapi.h"
//#include <stdlib.h>

//#ifdef __cplusplus
//extern "C" {
//#endif



typedef enum{
   STATE_INIT,
   PUERTAS_CERRADAS,
   ABRIENDO_PUERTAS,
   PUERTAS_ABIERTAS,
   INTENTANDO_CERRAR_PUERTAS,
   ALARMA_PUERTAS_ABIERTAS,
   CERRANDO_PUERTAS
} fsmState_t;

// Variable that hold the current state
fsmState_t mefStatePuertas;

extern   bool_t   secuenciaAperturaDePuertas;
extern   bool_t   secuenciaCerradoDePuertas;

// FSM IMPLEMENTATIONS

static uint8_t conteoSegundos=0;


// FSM functions
void mefPuertasError( void );
void mefPuertasInit( void );
void mefPuertasUpdate( void );



void actualizoTimer(void);
void inicializoTimer(void);
void chequeoEstado( void );
void enciendoLed( void );

void myTickHook( void*ptr );

//#ifdef __cplusplus
//}
//#endif

#endif /*MEFPUERTAS_H*/
