# Trabajo Practico 1 de Embebidos

El trabajo practico consite en la familiarización de la IDE y GPIO
necesarios para el proyecto final.

##  Pre-requisitos 

### Instalacion de software

Es necesario que previo a todo, se proceda a realizar
los pasos que muestran en [Instrucciones](https://campus.fi.uba.ar/pluginfile.php/175917/mod_resource/content/15/Sistemas_Embebidos-2018_1erC-TP1-Cruz.pdf). 
Esto se debe porque el projecto hace uso de 
la placa CIAA, tanto software como plug-in son 
necesarios para la compilación y degug del codigo.

Dado que no se avanzo en gran medida en el trabajo practico, 
aun no se tienen pre-requisitos más precisos.

## Requisitos

## Uso del IDE

### Instalación del OpenOCD 0.10.0

Para ello se procedió a ir al siguiente [Link](https://github.com/gnuarmeclipse/openocd/releases/tag/gae-0.10.0-20160110). 
En caso de poseer Ubuntu o derivados, la instalación completa 
automatica se puede realizar por medio de linea de comandos.

### Instalación del GIT

Para ello se procedió a ir al siguiente [Link](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git).
En caso de poseer Ubuntu o derivados, la instalación completa
automatica se puede realizar por medio de linea de comandos.

### Instalación de MCUXpresso IDE v10.1.1

Para ello se procedió a ir al siguiente [Link](https://www.nxp.com/support/developer-resources/software-development-tools/mcuxpresso-software-and-tools/mcuxpresso-integrated-development-environment-ide:MCUXpresso-IDE?tab=Design_Tools_Tab).

NOTA: En este caso no se realizó la actvación del software.

### Clonación de repositorio

A continuación se procede a realizar la clonación del github 
ofrecido por la catedra. Esto se realiza con los siguientes
comandos:

* git clone URL.git
* cp proyect.mk.template project.mk

En el archivo project.mk se configura el proyecto, el procesador y la placa.

### Primer uso con MCUXpresso

Se procede a crear un workspace y un proyecto en base a un archivo 
existente en la carpete de firware que acabamos de clonar:

* sapi_examples/edu-ciaa-nxp/bare_metal/gpio/gpio_02_blinky

El contenido de la carpeta se presenta acontinuacion:

![Cont_Blinky](https://github.com/jesusphilipraiz/Embebidos/blob/master/TP1/Imagenes/Cont_Blinky.png)

En la misma es posible observar 2 carpetas y un archivo Makefile.
Como se mencionó en secciones previas, este ultimo archivo indica
la carpeta donde se ubican los codigos fuentes, ./src, como 
tambien los correspondientes headers en la carpeta ./inc.

![Makefile](https://github.com/jesusphilipraiz/Embebidos/blob/master/TP1/Imagenes/Makefile.png)


|src 	 |inc 	  |Makefile|
|------  |------  |------  |
|blinky.c|blinky.h|-----   |

![BlinkyC](https://github.com/jesusphilipraiz/Embebidos/blob/master/TP1/Imagenes/BlinkyC.png)

#### Primera ejecución con MCUXpresso

Tras ejecutar el proyecto previamente mencionado, "blinky_02", se
procede a analizar linea tras linea. Esto se logra agregando 
*breakpoint* como tambien haciendo uso de los comandos *Step into*
y *step over*.



### Primer uso con Github

### Migración de arhivos

### Optimización de compilación


## Integrantes

* **Emmanuel Chang**
* **Miguel Perez Andrade**
* **Jesús Calle** -  