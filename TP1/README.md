# Trabajo Practico 1 de Embebidos

El trabajo practico consiste en la familiarización de la IDE y GPIO
necesarios para el proyecto final.

##  Pre-requisitos 

### Instalación de software

Es necesario que previo a todo, se proceda a realizar
los pasos que se muestran en [Instrucciones](https://campus.fi.uba.ar/pluginfile.php/175917/mod_resource/content/15/Sistemas_Embebidos-2018_1erC-TP1-Cruz.pdf). 
Esto se debe a que el proyecto hace uso de 
la placa CIAA, por lo que tanto software como plug-in son 
necesarios para la compilación y degug del código.

Dado que no se avanzo en gran medida en el trabajo practico, 
aun no se tienen pre-requisitos más precisos.

## Requisitos

## Uso del IDE

### Instalación del OpenOCD 0.10.0

Para ello se procedió a ir al siguiente [Link](https://github.com/gnuarmeclipse/openocd/releases/tag/gae-0.10.0-20160110). 
En caso de poseer Ubuntu o derivados, la instalación completa 
automática se puede realizar por medio de linea de comandos.

### Instalación del GIT

Para ello se procedió a ir al siguiente [Link](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git).
En caso de poseer Ubuntu o derivados, la instalación completa
automática se puede realizar por medio de linea de comandos.

### Instalación de MCUXpresso IDE v10.1.1

Para ello se procedió a ir al siguiente [Link](https://www.nxp.com/support/developer-resources/software-development-tools/mcuxpresso-software-and-tools/mcuxpresso-integrated-development-environment-ide:MCUXpresso-IDE?tab=Design_Tools_Tab).

NOTA: En este caso no se realizó la activación del software.

### Clonación de repositorio

A continuación se procede a realizar la clonación de la carpeta "firmware_v2" del github 
ofrecido por la cátedra. Esto se realiza con los siguientes
comandos:

```
 git clone https://github.com/ciaa/firmware_v2.git
 cd firmware_v2 
 cp proyect.mk.template project.mk
```
En el archivo project.mk se configura la ubicación del proyecto, el procesador y placa a utilizar.

### Primer uso con MCUXpresso

Para este caso se modifica el archivo "project.mk" de la siguiente manera:

```c#
PROJECT = sapi_examples/edu-ciaa-nxp/bare_metal/gpio/gpio_02_blinky
TARGET = lpc4337_m4
BOARD = edu_ciaa_nxp
```

Se procede a crear un workspace y un proyecto en base a un Makefile 
existente en la carpeta de "firmware_v2" que acabamos de clonar. Dicho Makefile corresponde al proyecto que previamente definimos en el "project.mk":

* firmware_v2/sapi_examples/edu-ciaa-nxp/bare_metal/gpio/gpio_02_blinky/Makefile

El contenido del proyecto, parpadeo de un led, se presenta a continuación:

![Cont_Blinky](https://github.com/jesusphilipraiz/Embebidos/blob/master/TP1/Imagenes/Cont_Blinky.png)

En la misma es posible observar 2 carpetas y un archivo Makefile.
Como se mencionó previamente, este ultimo archivo indica
la carpeta donde se ubican los códigos fuentes, ./src, como 
también los correspondientes headers, carpeta./inc.

![Makefile](https://github.com/jesusphilipraiz/Embebidos/blob/master/TP1/Imagenes/Makefile.png)


|src 	 |inc 	  |Makefile|
|------  |------  |------  |
|blinky.c|blinky.h|-----   |


![BlinkyC](https://github.com/jesusphilipraiz/Embebidos/blob/master/TP1/Imagenes/BlinkyC.png)

#### Primera ejecución con MCUXpresso

Tras seleccionar el proyecto a partir del Makefile de la carpeta "blinky_02", se procede a construir el proyecto. Se obsera que el proyecto no presenta fallas dado que el mensaje de consola indica 0 errores. 

![Build](https://github.com/jesusphilipraiz/Embebidos/blob/master/TP1/Imagenes/CaptureBuild.png)

Notar que aunque la consola indica que hay dos warning estos se deben solo a la presencia de variables no utilizadas.

Por ultimo, tras conectar la placa a la PC a travez de entrada "debug", se seleciona la opción debug que previamente personalizamos ([Sección 1.5](https://campus.fi.uba.ar/pluginfile.php/175917/mod_resource/content/15/Sistemas_Embebidos-2018_1erC-TP1-Cruz.pdf)).
 
Tras ejecutar el proyecto previamente mencionado, "blinky_02", se
procede a analizar linea tras linea. Esto se logra agregando 
*breakpoint* como también haciendo uso de los comandos *Step into*
y *step over*.

En principio se omitirán las lineas correspondiente al inicio del codigo, "main()", y inicialización de la placa, "boardConfig()", para enfocarse pricipalmente en el loop. Tras realizar un ciclo es posible observar que la función "gpioWrite()" es el encargado de encender o apagar el "LEDB" segun que variable se pase {ON, OFF}.

```c
while(1) {

      /* Prende el led azul */
      gpioWrite( LEDB, ON );

      delay(500);

      /* Apaga el led azul */
      gpioWrite( LEDB, OFF );

      delay(500);
   }
```

Realizando *step into* con la función "gpioWrite()" se puede comprobar lo mencionado. Se observa que la función llama a otra función, "gpioObtainPinConfig()", que se encarga de relacionar la etiqueca "LEDB" con una dirección del puerto y pin. Finalmente se llama a la función "Chip_GPIO_SetPinState()" que es la encargada de setear un 1 (ON) o un 0 (OFF) en la direccion de puerto y pin elegido para encender o apagar respectivamente el LED.

```c
bool_t gpioWrite( gpioMap_t pin, bool_t value ){

   bool_t ret_val     = 1;
   int8_t pinNamePort = 0;
   int8_t pinNamePin  = 0;
   int8_t func        = 0;
   int8_t gpioPort    = 0;
   int8_t gpioPin     = 0;

//Relaciona la variable "pin = LEDB" con un registro de la placa
   gpioObtainPinConfig( pin, &pinNamePort, &pinNamePin, &func, &gpioPort, &gpioPin );

//Setee el valor de un determinado registro "gpioPort = 0x02" - "gpioPin = 0x04"
   Chip_GPIO_SetPinState( LPC_GPIO_PORT, gpioPort, gpioPin, value);

   return ret_val;
}
```

Para verificar nuevamente lo mencionado, se realiza *step into* en la función "gpioObtainPinConfig()". Se observa que en la misma se hace uso de la configuración de los pines indexado al valor de nuestro pin. Valores de nombre de puerto y pin como también dirección de puerto y pin son copiadas a las variables pasadas por argumento.
 
```c
static void gpioObtainPinConfig( gpioMap_t pin, int8_t *pinNamePort, int8_t *pinNamePin,
                                 int8_t *func, int8_t *gpioPort, int8_t *gpioPin ){

   *pinNamePort = gpioPinsConfig[pin].pinName.port;
   *pinNamePin  = gpioPinsConfig[pin].pinName.pin;
   *func        = gpioPinsConfig[pin].func;
   *gpioPort    = gpioPinsConfig[pin].gpio.port;
   *gpioPin     = gpioPinsConfig[pin].gpio.pin;
}
```
Se realiza *step return* y se ingresa en "Chip_GPIO_SetPinState()". Se observa que las variables port y pin, seteadas en la función anterior, son utilizadas para posicionarse en el registro a atualizar. La variable de actualización es "setting = ON = 1".

```c
STATIC INLINE void Chip_GPIO_SetPinState(LPC_GPIO_T *pGPIO, uint8_t port, uint8_t pin, bool setting)
{
	pGPIO->B[port][pin] = setting;
}
```
Se comprende que el apagado del LEDB funciona exactamente igual, donde la variable "setting" es igual a 0 = "OFF".
Con respecto a la función "delay()", la misma consiste en tomar un valor de tiempo mediante la función "tickRead()" al entrar a la función, para luego comparar dentro de un loop, la diferencia de dicho valor con una variable de tiempo actualizada y la duración deseada normalizada.

```c
void delay(tick_t duration){
    tick_t startTime = tickRead();
    while ( (tick_t)(tickRead() - startTime) < duration/tickRateMS );
}
```

### Migración de arhivos

Dado que se comprobó que el código "parpadeo de led" funciona correctamente, se procede a crear una carpeta en paralelo a la carpeta firmware_v2. En la misma se almacenarán los codigos correspondientes al primer trabajo practico

%Imagen de la ubicación de la nueva carpeta

Recordar que dado que se cambio la ubicación del proyecto se debe actualizar las definiciones de "project.mk"

```c#
PROJECT = ../TP1              # Actualización de la ubicación del proyecto
TARGET = lpc4337_m4
BOARD = edu_ciaa_nxp
```

Para comprobar que todo el proceso se realizó correctamente, se copia el contenido de la carpeta "blinky_02" a la carpeta "TP1" y se renombran todos los archivos, con exepción del Makefile, como "TP1.* " (Siendo .c o .h donde corresponda). Dado que la ejecución del codigo se realiza correctamente, se verifica una migración exitosa.

### Primer uso con Github

Dado que el trabajo practico se realizará en grupo, se crea un repositorio en GitHub y se guardan los cambios en el.

```
 git init                        #consola posicionada de en la carpeta ../TP1
 git remote add origin [url del repositorio]
 git add TP1/                    #no se quiere incluir la carpeta de firmware_v2 en cada actualización  
 git commit –m ”commit inicial” 
 git push -u origin master
```  

### Compilación condicional

Dado que se tiene interes en ejecutar cualquier codigo sin tener que modificar el "project.mk" y/o copiar los nuevos codigos en la carpeta "TP1" para cada actualización, se procede a usar compilación condicional.

```c
#define TP1_1 (1)       // Parpadeo de un LED
#define TP1_2 (2)       // Encendido de luces en base a botones switch
#define TP1_3 (3)       // Secuencia de encendido de luces

#define TEST (TP1_1)    // Defino que código ejecutar

#if (TEST == TP1_1)     // Parpadeo de un LED
   \\Codigo 1
#endif

#if (TEST == TP1_1)     // Encendido de luces en base a botones switch
...
...

``` 

Gracias a la compilación condicional es posible ejecutar cualquiera de los tres proyectos solo modificando la variable "TEST" aunque con la desventaja de tener todos los códigos en un solo archivo.

Para comprobar que el proceso se realizó exitosamente, se cargan tres codigos al archivo "TP1.c" y se ejecuta cada uno modificando la variable TEST:

* "blinky_02.c" 
* "switches_leds.c"
* "tickHook_01"

Se observa que los tres codigos funcionan correctamente y por ende que la compilación condicional se aplicó correctamente.

PROBLEMA: Los tres códigos funcionan perfectamente individualmente y con la compilación condicional, pero tras terminar de usar la placa y desconectar, no es posible usar el mismo puerto USB para la siguiente oportunidad. Agrego que antes de desconectar la placa paro todas las seciones de debug que se esten realizando. Ademas terminó y limpió en la pestaña "debig" todads aquellas seciones restantes. Realizó clean del proyecto y por ultimo close del proyecto, pero el problema persiste. 

#### "switches_leds.c"

Como se mencionó previamente, nos concetraremos en el loop del código. El loop hace uso de la función de escritura usado en el anterior caso, por lo que el análisis será omitido. Observando el funcionamiento del código, se sabe que en caso de no tocar ningún boton, ningún led se encederá. En caso de presionar alguno, el led proximo a ese boton se encendera. Con esto concluimos que la función "gpioRead()" devuelve 1 si el boton no está presionado y 0 en caso contrario. Para verificar lo mencionado, se procede a realzar *step into* en dicha función.

```c
   while(1) {

      valor = !gpioRead( TEC1 );       //Devuelve 1 si no se toca el boton -> !1 -> 0 -> valor = OFF
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

```
Se observa que la función "gpioRead()" posee gran similitud con la función de escritura previamente analizada. La primera función consiste en relacionar una etiqueta con un registro especifico. La segunda función se encarga de retornar el valor del registro. Notar que en este caso se hace lectura de 5 registros, [TEC1={0,4}, TEC2={0,8}, TEC3={0,9}, TEC4={1,9}, GPIO0={3,0}], siendo el primer termino el valor del puerto y el segundo el pin correspondiente.

```c
bool_t gpioRead( gpioMap_t pin ){

   bool_t ret_val     = OFF;
   int8_t pinNamePort = 0;
   int8_t pinNamePin  = 0;
   int8_t func        = 0;
   int8_t gpioPort    = 0;
   int8_t gpioPin     = 0;

   gpioObtainPinConfig( pin, &pinNamePort, &pinNamePin, &func,
                           &gpioPort, &gpioPin );     //Relaciona variable pin con un port = gpioPort y pin = gpioPin

   ret_val = (bool_t) Chip_GPIO_ReadPortBit( LPC_GPIO_PORT, gpioPort, gpioPin );    //Lee el contenido del registro

   return ret_val;
}

```
Realizando nuevamente *step into* en la función "Chip_GPIO_ReadPortBit()", se observa como la función accede al registro correspondiente y devuelve su valor por retorno.

```c
STATIC INLINE bool Chip_GPIO_ReadPortBit(LPC_GPIO_T *pGPIO, uint32_t port, uint8_t pin)
{
   return (bool) pGPIO->B[port][pin];
}

```
#### "tickHook_01"

En este caso se hace uso de interrupciones y por ende tambien de funciones "enlazadas". En este caso se setea una interupcion por timer de desborde. Cuando se activa el flag, se llama a la función "myTickHook()" a la que se le pasa como variable la etiqueta de un led especifico.

```c
void myTickHook( void *ptr ){

   static bool_t ledState = OFF;    //Variable tipo static. Recuerda el contenido del mismo aun si se sale de la función

   gpioMap_t led = (gpioMap_t)ptr;

   if( ledState ){         //Si estaba en ON entonces OFF
      ledState = OFF;
   }
   else{
      ledState = ON;       //Si estaba en OFF entonces ON
   }
   gpioWrite( led, ledState );      //Como se llama a la función cada 50 ms el led parpadea constantamente
}

```
Se observa en el loop una asociación con los diferentes leds a la funcion de interrupción cada 1000 ms. Notar que la función "tickCallbackSet()" no genera que el led parpadee, solo realiza la asociación, los multiples debordes del timer lo generan.

```c
   while(1) {
      tickCallbackSet( myTickHook, (void*)LEDG );     //Se relaciona al LEDG con la funión de interrupción
      delay(1000);
      tickCallbackSet( myTickHook, (void*)LEDB );     //Se relaciona al LEDB y no al LEDG
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

```

## Integrantes

* **Emmanuel Chang**
* **Miguel Perez Andrade**
* **Jesús Calle**  