# Simulación de servidor web de alta disponibilidad en ns3

Planificación y Simulación de Redes 2022/2023.

## Autores.
    - Manuel Domínguez Montero.
    - Juan Manuel Alcalá Palomo.
    - Ismael García Mayorga.
    - Julio Navarro Vázquez. 

## Tabla de contenidos.

## Introducción.

En este proyecto, se pretende modelar un servidor web con capacidad de Proxy inverso y balanceo de carga, funcionalidades
que encontramos en los servidores más utilizados en el mercado como Apache2 y NGINX. 

## Dependencias
Para ejecutar el código necesita al menos una instalación de ns3 en el directorio
```shell
${HOME}/.local/bin/ns3/ns3/ns-3.36.1/
```

Además de una instalación de Firecracker, con el ejecutable accesible desde PATH

## Ejecución.

```shell
NS_LOG='Ipv4Router:Main' ./ns3 run "main.cc"
```


