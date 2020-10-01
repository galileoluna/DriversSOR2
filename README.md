# Trabajo Practico 0

## Punto 1

Para poder ejecutar el modulo que nos es previsto debemos realizar los siguientes pasos:

 1.  Creo el archivo miModulo.c
 2. Creo el archivo Makefile
 3. Compilo el archivo Makefile mediante el comando en la terminal:
 4. Ejecuto el siguiente comando. 
 

	    make -f Makefile

 5. Cargo el modulo mediante el comando
 

	    sudo insmod ./miModulo.ko

 6. Descargo el modulo 

	    sudo rmmod miModulo.ko

 7. Compruebo que funcione abriendo dos terminales en uno vemos el log del kernel mediante el siguiente comando.
 

		  tail -f /var/log/syslog

 8.  Luego ejecutamos en el otro terminal
 

		  sudo insmod ./miModulo.ko

 9. Y en la pestaña del primer terminal se mostrara el siguiente log 

	     Oct 1 19:06:26 galileo kernel: [ 5187.677119]  UNGS : Driver registrado 

 
## Punto 2

Para realizar este proyecto definimos las funciones :
*init_module*: tiene la funcion de ingresar el modulo en el kernel, genera un registro que nos da el major number
*cleanup_module*: sirve para cuando desregistramos el modulo 
*cifrar*: con esta funcion nosotros realizamos el cifrado CESAR con un corrimiento.

## Pasos
 1. Ejecutamos el comando:

	    make
	Este nos retornara los archivos ejecutables. 
 2. Luego registramos el Device:
		

	      sudo insmod encryptationDev.ko
    	  [sudo] password for galileo: 

 3. Para ver los mensajes del kernel  donde se encuentran las indicaciones de nuestro modulo ,nosotros utilizamos el comando **dmesg**, ya que este nos indica el major number para poder comunicarnos con este.
	 

		[ 5187.677110] I was assigned major number 244. To talk to

		[ 5187.677113] the driver, create a dev file with

		[ 5187.677114] 'mknod /dev/encryptationDev c 244 0'.

		[ 5187.677115] Try various minor numbers. Try to cat and echo to

		[ 5187.677115] the device file.

		[ 5187.677116] Remove the device file and module when done.



 4. Creamos un  archivo en /dev para comunicarnos con el driver:
	 

	    galileo@galileo:~/Escritorio/tp2/punto2$ sudo mknod /dev/encryptationDev c 244 0

 5. Le otorgamos los permisos:
 
		galileo@galileo:~/Escritorio/tp2/punto2$ sudo chmod 777 /dev/encryptationDev
		
 6. Procedemos a probar el device y el cifrado:
 

		galileo@galileo:~/Escritorio/tp2/punto2$ echo "hola" > /dev/encryptationDev

		galileo@galileo:~/Escritorio/tp2/punto2$ cat /dev/encryptationDev

		ipmb
