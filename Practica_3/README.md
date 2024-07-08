# Practica 3

El objetivo de la práctica 3 es modularizar las funciones de 'delay' y de 'led pwm'. 

Para la modularización, se crearon carpetas dentro de un nuevo espacio en 'Drivers', llamado 'API'. En este espacio se crearon dos carpetas para representar los componentes 'api_delay' y 'api_led_pwm'. El primero implementa delays bloqueantes y no bloqueantes; mientras que el segundo, implementa el comportamiento de leds pwm usando los recursos disponibles en la NUCLE F429ZI, de igual forma que el primer componente, de manera bloqueante y no bloqueante.

Posteriormente, se pidió que se pueda configurar una secuencia de ciclos consecutivos en un solo led y que para ello se deba usar una nueva función del componente 'api_delay', la cual retorne el estado de un objeto delay.

## Autor
Guido Ramírez - guidoramirez7@gmail.com
