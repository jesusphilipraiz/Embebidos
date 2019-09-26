//#include "blinky.h"   // <= own header (optional)
#include "sapi.h"       // <= sAPI header

#define TICKRATE_1MS (1)            /* 1000 ticks per second */
#define TICKRATE_10MS   (10)        /* 100 ticks per second */
#define TICKRATE_100MS  (100)       /* 10 ticks per second */
#define TICKRATE_MS     (TICKRATE_1MS) /* ¿? ticks per second */

#define LED_TOGGLE_100MS  (100)
#define LED_TOGGLE_500MS  (500)
#define LED_TOGGLE_1000MS (1000)
#define LED_TOGGLE_MS   (LED_TOGGLE_500MS / TICKRATE_MS)

#define BUTTON_STATUS_10MS (10)
#define BUTTON_STATUS_100MS   (50)
#define BUTTON_STATUS_500MS   (100)
#define BUTTON_STATUS_MS   (BUTTON_STATUS_10MS / TICKRATE_MS)

volatile bool LED_Time_Flag = false;

volatile bool BUTTON_Status_Flag = false;
volatile bool BUTTON_Time_Flag = false;

#define TP1_1 (1)       // Parpadeo de un LED
#define TP1_2 (2)       // Encendido de luces en base a botones switch
#define TP1_3 (3)       // Secuencia de encendido de luces
#define TP1_4 (4)       //Damn it
#define TP1_5 (5)       //Damn it 2
#define TP1_6 (6)       //Really?!

#define TEST (TP1_4)    // Defino que código ejecutar

#if (TEST == TP1_1)     // Parpadeo de un LED

int main(void){

   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();

   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(1) {

      /* Prendo el led azul */
      gpioWrite( LEDB, ON );

      delay(500);

      /* Apago el led azul */
      gpioWrite( LEDB, OFF );

      delay(500);
   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}

#endif

#if (TEST == TP1_2)     // Encendido de luces en base a botones switch

int main(void){

   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();

   gpioConfig( GPIO0, GPIO_INPUT );

   gpioConfig( GPIO1, GPIO_OUTPUT );

   /* Variable para almacenar el valor de tecla leido */
   bool_t valor;

   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(1) {

      valor = !gpioRead( TEC1 );
      gpioWrite( LEDB, valor );

      valor = !gpioRead( TEC2 );
      gpioWrite( LED1, valor );

      valor = !gpioRead( TEC3 );
      gpioWrite( LED2, valor );

      valor = !gpioRead( TEC4 );
      gpioWrite( LED3, valor );

      valor = !gpioRead( GPIO0 );
      gpioWrite( GPIO1, valor );
   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}

#endif

#if (TEST == TP1_3)     // Secuencia de encendido de luces

void myTickHook( void *ptr ){

   static bool_t ledState = OFF;

   gpioMap_t led = (gpioMap_t)ptr;

   if( ledState ){
      ledState = OFF;
   }
   else{
      ledState = ON;
   }
   gpioWrite( led, ledState );
}

int main(void){

   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();

   /* Inicializar el conteo de Ticks con resolucion de 50ms (se ejecuta
      periodicamente una interrupcion cada 50ms que incrementa un contador de
      Ticks obteniendose una base de tiempos). */
   tickConfig( 50 );

   /* Se agrega ademas un "tick hook" nombrado myTickHook. El tick hook es
      simplemente una funcion que se ejecutara peri­odicamente con cada
      interrupcion de Tick, este nombre se refiere a una funcion "enganchada"
      a una interrupcion.
      El segundo parametro es el parametro que recibe la funcion myTickHook
      al ejecutarse. En este ejemplo se utiliza para pasarle el led a titilar.
   */
   tickCallbackSet( myTickHook, (void*)LEDR );
   delay(1000);

   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(1) {
      tickCallbackSet( myTickHook, (void*)LEDG );
      delay(1000);
      tickCallbackSet( myTickHook, (void*)LEDB );
      delay(1000);
      tickCallbackSet( myTickHook, (void*)LED1 );
      delay(1000);
      tickCallbackSet( myTickHook, (void*)LED2 );
      delay(1000);
      tickCallbackSet( myTickHook, (void*)LED3 );
      delay(1000);
      tickCallbackSet( myTickHook, (void*)LEDR );
      delay(1000);
   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}

#endif

#if (TEST == TP1_4)     // Secuencia de encendido de luces por variable TICKRATE_MS
   /* FUNCION que se ejecuta cada vez que ocurre un Tick. */
void myTickHook( void *ptr ) {
   LED_Time_Flag = true;
}


/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void) {

   /* ------------- INICIALIZACIONES ------------- */
   uint32_t LED_Toggle_Counter = 0;

   /* Inicializar la placa */
   boardConfig();

   /* Inicializar el conteo de Ticks con resolucion de 1ms (se ejecuta
       periodicamente una interrupcion cada TICKRATE_MS que incrementa un contador de
       Ticks obteniendose una base de tiempos). */
   tickConfig( TICKRATE_MS );

   /* Se agrega ademas un "tick hook" nombrado myTickHook. El tick hook es
       simplemente una funcion que se ejecutara peri�odicamente con cada
       interrupcion de Tick, este nombre se refiere a una funcion "enganchada"
       a una interrupcion.
       El segundo parametro es el parametro que recibe la funcion myTickHook
       al ejecutarse. En este ejemplo se utiliza para pasarle el led a titilar.
   */
   tickCallbackSet( myTickHook, (void*)NULL );
   gpioMap_t arr_LEDS[6] = {LED3, LED3, LED2, LED2, LED1, LED1};
   int arr_Length = 6;
   int i = 0;

   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(1) {
      //__WFI();

      if (LED_Time_Flag == true) {
         LED_Time_Flag = false;

         if (LED_Toggle_Counter == 0) {
            LED_Toggle_Counter = LED_TOGGLE_MS;

            i = i % arr_Length;
            gpioToggle(arr_LEDS[i]);
            i++;
         }
         else
            LED_Toggle_Counter--;
      }
   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
       por ningun S.O. */
   return 0 ;
}
#endif

#if (TEST == TP1_5)     // Print Debug

#endif

#if (TEST == TP1_6)     // Secuencia de encendido de luces

#endif
