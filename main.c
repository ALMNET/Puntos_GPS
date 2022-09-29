#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "main.h"
#include "GPSProcessing.h"

// Config Json VS Code
//"miDebuggerPath": "C:\\MinGW\\bin\\gdb.exe",


int main () {

   unsigned short lineaIndex, puntoIndex;

   FILE *fp, *puntosAEscribir;

   unsigned char nombreArchivo[32];

   unsigned char buffer[6*512];

   tsAlarmPointsDescriptor Puntos[6];

   //tsAlarmPointsDescriptor Puntos[6]; //= {
   //    {
   //       .Linea = 500,
   //       .UmbralDistancia = 60,
   //       .PuntoGPS = 
   //       {
   //          {
   //             .Latitude = 0,
   //             .Longitude = 0
   //          },
   //       }
   //    },
   //    {},
   //    {},
   //    {},
   //    {},
   //    {},
   // };

   // puntos[5] = {
   //    {
   //       .Linea = 500,
   //       .UmbralDistancia = 60,
   //       .PuntoGPS = 
   //       {
   //          {
   //             .Latitude = 0,
   //             .Longitude = 0
   //          },
   //       }
   //    },
   //    {},
   //    {},
   //    {},
   //    {},
   //    {},
   // };




   int i, j, read;

   /* Open file for both reading and writing */
   fp = fopen(EBUS_GPS_ALARM_CFG_FILENAME, "rb");

   /* Read and display data */
   read = fread((unsigned char*) Puntos, 1, sizeof(Puntos), fp);
   
   // for(i = 0; i < read; i++){
   //    printf("%02X ",buffer[i]);
   //    if(((i+1) % 512) == 0)
   //    printf("\n\n");
   // }


   for(lineaIndex = 0; lineaIndex <= 5; lineaIndex++)
   {
      printf("Puntos Linea 50%d:\n", lineaIndex);
      for(puntoIndex = 0; (Puntos[lineaIndex].PuntoGPS[puntoIndex].Latitude); puntoIndex++)
      {
         printf("                   Latitud:  %f\n", Puntos[lineaIndex].PuntoGPS[lineaIndex].Latitude);
         printf("                   Longitud: %f\n\n", Puntos[lineaIndex].PuntoGPS[lineaIndex].Longitude);
      }
      printf("\n\n");
   }







   // lee todos los puntos disponibles. el valor de i queda en el último punto
   // conciso que leyó
   for (i=0; i< 63; i++)
   {
      if (Puntos[5].PuntoGPS[i].Latitude)
      {
         printf("Lat: %f ", Puntos[5].PuntoGPS[i].Latitude);
         printf("Long: %f\n", Puntos[5].PuntoGPS[i].Longitude);
         
      }
      else
         break;
   }

	Puntos[5].PuntoGPS[i].Latitude = -37.32835578011083;
   Puntos[5].PuntoGPS[i].Longitude = -59.145798155200865;

   
   fclose(fp);

   //if (puntosAEscribir = fopen(EBUS_GPS_ALARM_NEW_FILENAME, "w+"))

   if (puntosAEscribir = fopen("ALRMCFG3_NEW.bin", "wb"))
   {
      fwrite(Puntos, sizeof(Puntos), 1, puntosAEscribir);
      fclose(puntosAEscribir);
   }

   
   return(0);
}