#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "main.h"
#include "GPSProcessing.h"


int main () {
   FILE *fp, *puntosAEscribir;

   unsigned char nombreArchivo[32];

   unsigned char buffer[6*512];

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

   tsAlarmPointsDescriptor Puntos[6];

   float latitu = 0x055015C2;

   float longitu = 0x5C956CC2;

   printf("%f\n", (float) latitu);
   printf("%f\n\n", (float) longitu);


   printf("%d\n\n", sizeof(float));



   int i, j, read;

   /* Open file for both reading and writing */
   fp = fopen(EBUS_GPS_ALARM_CFG_FILENAME, "rb");

   /* Read and display data */
   read = fread((unsigned char*) Puntos, 1, sizeof(Puntos), fp);
   
   for(i = 0; i < read; i++){
      printf("%02X ",buffer[i]);
      // if((i % 512) == 0)
      // printf("\n\n");
   }

   // lee todos los puntos disponibles. el valor de i queda en el último punto
   // conciso que leyó
   for (i=0; i< 63; i++)
   {
      if (Puntos[5].PuntoGPS[i].Latitude)
      {
         printf("Lat: %f", Puntos[5].PuntoGPS[i].Latitude);
         printf("Long: %f\n", Puntos[5].PuntoGPS[i].Longitude);
         
      }
      else
         break;
   }

	Puntos[5].PuntoGPS[i].Latitude = -37.32835578011083;
   Puntos[5].PuntoGPS[i].Longitude = -59.145798155200865;

   
   fclose(fp);

   //if (puntosAEscribir = fopen(EBUS_GPS_ALARM_NEW_FILENAME, "w+"))

   if (puntosAEscribir = fopen("ALRMCFG3_NEW.bin", "w+"))
   {
      fwrite(Puntos, sizeof(Puntos), 1, puntosAEscribir);
      fclose(puntosAEscribir);
   }

   
   return(0);
}




// int main()
// {
//     unsigned char Transac[64];
//     unsigned char buffer[32];

//     uint32_t nTrans;
//     uint32_t nTransDif;
//     uint32_t inicioTransac;
    
//     int i;
//     char Input[64];


//     FILE *f;

//     memset(Transac, 0, sizeof(Transac));
//     Transac[63] = 0x01;

//     puts("Ingrese cantidad de transacciones a regenerar:");
//     gets(Input);

//     if ((nTrans = atoi(Input)))
//     {
//         inicioTransac = nTrans / 512;

//         nTransDif = nTrans - (inicioTransac * 512);

//         sprintf(buffer, "%08X.HSH", inicioTransac);

//         if ((f = fopen(buffer, "wb")))
//         {
//             for (i=0; i< nTransDif; i++)
//             {
//                 fwrite(Transac, sizeof(Transac), 1, f);
//             }
//             fclose(f);
//         }
//     }

//     return 0;
// }
