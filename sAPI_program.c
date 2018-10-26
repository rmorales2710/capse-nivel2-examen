/*=====[Datos del modulo]====================================================
 *
 * Copyright 2018 Eric Nicolas Pernia <ericpernia@gmail.com>
 * All rights reserved.
 * License: BSD-3-Clause <https://opensource.org/licenses/BSD-3-Clause>)
 *
 * Module Name: programa
 * Version: 1.0.0
 * Creation Date: 2018/10/12
 */

/*=====[Inclusiones de dependencias de funciones privadas]===================*/

#include "sapi.h"
#include "mefPuertas.h"

/*=====[Definiciones de Variables globales publicas]=========================*/

DEBUG_PRINT_ENABLE

bool_t   modoConfiguracion = FALSE;
bool_t   secuenciaAperturaDePuertas=FALSE;
bool_t   secuenciaCerradoDePuertas=FALSE;


circularBufferNew( pisosPendientes, 1, 10 );
//   =
//   circularBuffer_t pisosPendientes;
//   uint8_t pisosPendientes_BufferMemory[11];

/*=====[Definiciones de Variables globales privadas]=========================*/

static int32_t varGlobalPrivada = 0;

/*=====[Prototipos de funciones privadas]====================================*/

//static void cuandoBufferLleno(void* unused);

//static void cuandoBufferVacio(void* unused);



/*=====[Implementaciones de funciones publicas]==============================*/

int main(void){
   
   // Inicializar y configurar la plataforma
   boardConfig();
   
   // Inicializar UART_USB como salida Serial de debug
   debugPrintConfigUart( UART_USB, 115200 );
   debugPrintlnString( "DEBUG: UART_USB configurada." );

 
   

   // Buffer circular de 10 elementos de 1 byte de tamaño
  // circularBufferInit( &pisosPendientes, 1, 10 ); 

  
   // Si queremos usar los callbacks:
  // circularBufferEmptyBufferCallbackSet( &pisosPendientes, cuandoBufferVacio );
  // circularBufferFullBufferCallbackSet( &pisosPendientes, cuandoBufferLleno );
   

   //mefIgresoPisoInit();
   mefPuertasInit();
  // mefModoConfiguracionInit();
  // mefAscensorInit();

   // Delay no bloqueante 1s
   delay_t fsmScheduler;
   delayConfig( &fsmScheduler, 1000); 
   
    uint8_t receivedByte=0;//Solo para pruebas
   


   
   while(TRUE){



      //simulo peticiones externas
      if(uartReadByte(UART_USB,&receivedByte)){
           
           switch ( receivedByte){
                case 'a':
                        secuenciaAperturaDePuertas = TRUE; //pidieron abrir puertas
                     break;
                
                case 'c':
                        secuenciaCerradoDePuertas = TRUE; //pidieron cerrar puertas
                     break;
                
                
             }
          }


     if( delayRead(&fsmScheduler) ){
         
         
       //  mefStatePuertas++;
         
         
         // Ejecutar cada 1s
    //     mefIgresoPisoUpdate();
         mefPuertasUpdate();
    //     mefModoConfiguracionUpdate();
    //     mefAscensorUpdate();
      }
      
      
   }
   return 0;
}

/*=====[Implementaciones de funciones del modulo]==============================*/

