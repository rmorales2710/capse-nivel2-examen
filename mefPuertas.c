#include "sapi.h"
#include "mefPuertas.h"


// FSM Error Handler Function
void mefPuertasError( void ){ mefPuertasInit(); }


// FSM Initialize Function
void mefPuertasInit( void )
{
  // debugPrintlnString( "Inicializacion" );
   tickConfig( 50 );
   
   tickCallbackSet( myTickHook, (void*)LEDB );
   
   mefStatePuertas = STATE_INIT;   // Set initial state
 
}

// FSM Update Sate Function
void mefPuertasUpdate( void )
{
   static bool_t flagStateInit = FALSE;
   static bool_t flagStatePUERTAS_CERRADAS = FALSE;
   static bool_t flagStateABRIENDO_PUERTAS = FALSE;
   static bool_t flagStatePUERTAS_ABIERTAS = FALSE;
   static bool_t flagStateINTENTANDO_CERRAR_PUERTAS = FALSE;
   static bool_t flagStateALARMA_PUERTAS_ABIERTAS = FALSE;
   static bool_t flagStateCERRANDO_PUERTAS = FALSE;
   
   const uint8_t tiempoCerrandoPuertas=5;
   const uint8_t tiempoAbriendoPuertas=5;
   const uint8_t tiempoPuertasAbiertas=5;
   const uint8_t tiempoIntentandoCerrarPuertas=3;
   
   
   chequeoEstado();
   enciendoLed();
         
   switch( mefStatePuertas ){

      case STATE_INIT://Contemplamos la posibilidad de estado inicial de maquina previo al estado inicial fisico
      
         /* ENTRY */
         if( flagStateInit  == FALSE ){
          flagStateInit  = TRUE;
          }
      
          mefStatePuertas= PUERTAS_CERRADAS;
                
         /* LEAVE */
         if( mefStatePuertas != STATE_INIT ){
            flagStateInit  = FALSE;
        
            }
      break;

      
      case PUERTAS_CERRADAS:
      
         /* ENTRY */
         if( flagStatePUERTAS_CERRADAS == FALSE ){
            flagStatePUERTAS_CERRADAS = TRUE;
            // Code for entry...    
         }
         
         
         //RecibÃ¬ peticion de apertura...?
         if (secuenciaAperturaDePuertas == TRUE){mefStatePuertas= ABRIENDO_PUERTAS;}

         if( mefStatePuertas != PUERTAS_CERRADAS ){
            flagStatePUERTAS_CERRADAS = FALSE;
            // Code for leave...  
         }
      break;

      case ABRIENDO_PUERTAS:
      
         /* ENTRY */
         if( flagStateABRIENDO_PUERTAS == FALSE ){
            flagStateABRIENDO_PUERTAS = TRUE;
             
            inicializoTimer();
            
            secuenciaAperturaDePuertas = FALSE; //fuerzo a nueva peticion de apertura
            
         }
         
         actualizoTimer();      
         
         //CaducÃ² el tiempo de abriendo puertas...?
         if (conteoSegundos == tiempoAbriendoPuertas ){mefStatePuertas= PUERTAS_ABIERTAS;}
         
         /* LEAVE */
         if( mefStatePuertas != ABRIENDO_PUERTAS ){
            flagStateABRIENDO_PUERTAS = FALSE;
            // Code for leave...  
         }
      break;
         
      case PUERTAS_ABIERTAS:
      
         /* ENTRY */
         if( flagStatePUERTAS_ABIERTAS == FALSE ){
            flagStatePUERTAS_ABIERTAS = TRUE;
            
            inicializoTimer();
            
            secuenciaCerradoDePuertas = FALSE;//Fuerzo proxima soliucitud
         }
         
                  
         //CaducÃ² el tiempo de puertas abiertas ...?
         if (conteoSegundos == tiempoPuertasAbiertas )
               {if (secuenciaCerradoDePuertas == TRUE) {mefStatePuertas= INTENTANDO_CERRAR_PUERTAS;} }
             else  { actualizoTimer();}
             
        
             
         
         /* LEAVE */
         if( mefStatePuertas != PUERTAS_ABIERTAS ){
            flagStatePUERTAS_ABIERTAS = FALSE;
            // Code for leave...  
         }
      break;  
         
      
       case INTENTANDO_CERRAR_PUERTAS:
  
      
         /* ENTRY */
         if( flagStateINTENTANDO_CERRAR_PUERTAS == FALSE ){
            flagStateINTENTANDO_CERRAR_PUERTAS = TRUE;
            
            inicializoTimer();
            
            secuenciaCerradoDePuertas = FALSE; //Fuerzo nueva peticion de cerrado de puertas
            
         }
         
         actualizoTimer();       
         
         //CaducÃ² el tiempo de intentando cerrar puertas..?
         if (conteoSegundos == tiempoIntentandoCerrarPuertas )
               {if( gpioRead( TEC1 )){ mefStatePuertas= CERRANDO_PUERTAS;}//no hay gente
                     else  { mefStatePuertas= ALARMA_PUERTAS_ABIERTAS; }
                  }      
             

         /* LEAVE */
         if( mefStatePuertas != INTENTANDO_CERRAR_PUERTAS ){
            flagStateINTENTANDO_CERRAR_PUERTAS = FALSE;
            // Code for leave...  
         }
      break;  
         

       case ALARMA_PUERTAS_ABIERTAS:
        
         /* ENTRY */
         if( flagStateALARMA_PUERTAS_ABIERTAS == FALSE ){
            flagStateALARMA_PUERTAS_ABIERTAS = TRUE;
              
         }
         
         
        if( gpioRead( TEC1 )){ mefStatePuertas= INTENTANDO_CERRAR_PUERTAS;}//no hay gente
         
                  

         /* LEAVE */
         if( mefStatePuertas != ALARMA_PUERTAS_ABIERTAS ){
            flagStateALARMA_PUERTAS_ABIERTAS = FALSE;
            
            // Code for leave...  
         }
      break;  

      case CERRANDO_PUERTAS:
        
         /* ENTRY */
         if( flagStateCERRANDO_PUERTAS == FALSE ){
            flagStateCERRANDO_PUERTAS = TRUE;
            
            inicializoTimer();
            
         }
       
         actualizoTimer();
         
         //CaducÃ² el tiempo de cerrando puertas..?
         if (conteoSegundos == tiempoCerrandoPuertas )
            { mefStatePuertas= PUERTAS_CERRADAS;}
         
         /* LEAVE */
         if( mefStatePuertas != CERRANDO_PUERTAS ){
            flagStateCERRANDO_PUERTAS = FALSE;
            // Code for leave...  
         }
      break;  

      default:
         mefPuertasError();
      break;
   }
}
   
   
void actualizoTimer(void){ conteoSegundos++;   }

void inicializoTimer(void){ conteoSegundos=0; }
   

void chequeoEstado( void )
{  
    
   switch( mefStatePuertas ){

      case STATE_INIT:
         //debugPrintlnString( "Estado Inicial" );
      break;
     
      case PUERTAS_CERRADAS:
        // debugPrintlnString( "Puertas Cerradas" );
      break;    
   
      case ABRIENDO_PUERTAS:
         //debugPrintlnString( "Abriendo Puertas" );
     
      break;  
      
      case PUERTAS_ABIERTAS:
         //debugPrintlnString( "Puertas Abiertas" );
      
      break;  
      
      case INTENTANDO_CERRAR_PUERTAS:
         //debugPrintlnString( "Intentando Cerrar Puertas" );
      break;  
      
      case ALARMA_PUERTAS_ABIERTAS:
         //debugPrintlnString( "Alarma Puertas Abiertas" );
      break;  
      
      case CERRANDO_PUERTAS:
         //debugPrintlnString( "Cerrando Puertas" );
   
      break;  
      
   }
      
      
}  
   
   
   
void enciendoLed( void )
{  
   gpioToggle( LED3 ); 
   
   switch( mefStatePuertas ){
      
      case ABRIENDO_PUERTAS:
         gpioWrite( LED1, TRUE );
         gpioWrite( LEDG, FALSE );
         gpioWrite( LED2, FALSE );
         gpioWrite( LEDB, FALSE );
   
      break;  
      
      case PUERTAS_ABIERTAS:
         gpioWrite( LED1, FALSE );
         gpioWrite( LEDG, TRUE );
         gpioWrite( LED2, FALSE );
         gpioWrite( LEDB, FALSE );
      break;  
      
      case CERRANDO_PUERTAS:
         gpioWrite( LED1, FALSE );
         gpioWrite( LEDG, FALSE );
         gpioWrite( LED2, TRUE );
      break;  
      
      case ALARMA_PUERTAS_ABIERTAS:
        
      break;  
      
      default:
         gpioWrite( LED1, FALSE );
         gpioWrite( LEDG, FALSE );
         gpioWrite( LED2, FALSE );
         gpioWrite( LEDB, FALSE );
      break;
      
   }
}

/* FUNCION que se ejecuta cada vez que ocurre un Tick. */
void myTickHook(  void*ptr )
{
   static uint8_t tiempoAlarmaPuertas = 0;
  
  if (mefStatePuertas ==ALARMA_PUERTAS_ABIERTAS){
   if (tiempoAlarmaPuertas >= 10){
      tiempoAlarmaPuertas=0;
      gpioToggle( LEDB );
   }
   else {tiempoAlarmaPuertas++;}
   
}
    
}
  

   
