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

```
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

```
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

```
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
 
```
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

```
STATIC INLINE void Chip_GPIO_SetPinState(LPC_GPIO_T *pGPIO, uint8_t port, uint8_t pin, bool setting)
{
	pGPIO->B[port][pin] = setting;
}
```
Se comprende que el apagado del LEDB funciona exactamente igual, donde la variable "setting" es igual a 0 = "OFF".
Con respecto a la función "delay()", la misma consiste en tomar un valor de tiempo mediante la función "tickRead()" al entrar a la función, para luego comparar dentro de un loop, la diferencia de dicho valor con una variable de tiempo actualizada y la duración deseada normalizada.

```
void delay(tick_t duration){
    tick_t startTime = tickRead();
    while ( (tick_t)(tickRead() - startTime) < duration/tickRateMS );
}
```

### Primer uso con Github

### Migración de arhivos

### Optimización de compilación


## Integrantes

* **Emmanuel Chang**
* **Miguel Perez Andrade**
* **Jesús Calle** -  