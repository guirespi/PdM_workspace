# Practica 3

## Autor
Guido Ramírez - guidoramirez7@gmail.com

## Descripción

El objetivo de la práctica 3 es realizar una maquina de estados finitos (MEF) cuyo objetivo es implementar el debounce de un butón.

Para cumplir el objetivo se debe implementar una API denominado 'api_debounce'. Dicha API debe implementar la MEF y actualizar si el botón fue previamente presionado.
Por último, se debe cambiar el periodo de encendido de los leds cuando se detecte que el botón haya sido presionado. Los periodos cambian de 100 a 500ms de encendido y viceversa.

Para poder completar esta practica se hace uso de la API led_pwm, delay y debounce realizadas por el autor.

## Post-Práctica

### ¿Es adecuado el control de los parámetros pasados por el usuario que se hace en las funciones implementadas? ¿Se controla que sean valores válidos? ¿Se controla que estén dentro de los rangos correctos?

Sí, API delay y API pwm validan que los estados y parametros de los objetos sean válidos antes de realizar alguna operación.

### ¿Se nota una mejora en la detección de las pulsaciones respecto a la práctica 0? ¿Se pierden pulsaciones? ¿Hay falsos positivos?

En la práctica 0, personalmente usé una IRQ en flanco de baja para el botón por lo que el efecto en comparación a esta práctica es el mismo.

### ¿Es adecuada la temporización con la que se llama a debounceFSM_update()? ¿Y a readKey()? ¿Qué pasaría si se llamara con un tiempo mucho más grande? ¿Y mucho más corto?

Sí es adecuada. Si el tiempo es mucho más grande, pues forzará al usuario a tener el botón presionado por mucho más tiempo antes de detectar el evento de 'presionado' como tal, además, abre paso que menos falsos positivos
se den. Si el tiempo es más corto, da paso a más falsos positivos por interferencia u otro fenomeno pero el beneficio es que la reacción del equipo será mayor cuando un usuario lo manipule.
