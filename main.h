#include <stdint.h>

typedef uint8_t  u8;
typedef uint16_t  u16;
typedef uint32_t  u32;

typedef struct __attribute__((packed))
{

       u16          Mach_ID;
       u8           IdaVuelta;
       u8           IDRecorrido;
       u8           Num_Seccion;
       u16          Compania;
       u16          Unidad;
       u16          Interno;
       u32          Chofer;
       u16          Linea;
       u16          Tarifa;
       u16          VFW_UVP;
       u8			Imagen_Actual_UVP;
       u16          VFW_UCH;
       u8			Imagen_Actual_UCH;
       u8           EstadoMaquina;
       u8           Contacto;
       u8           Emergencia;
       u16			VTabla_Tarifas;
} StatUVP;

typedef struct __attribute__((packed))
{
	float Latitude;      // 4 Bytes
	float Longitude;     // 4 Bytes
}tsGPSLatLong;              // 8 Bytes

typedef struct __attribute__((packed))
{
	u16 Linea;			// 2 Bytes: Línea para la que se aplicarán las alarmas
	u16 UmbralDistancia;		// 2 Bytes: radio en metros alrededor de cada punto GPS que dispara la alarma +- 10% histéresis
	tsGPSLatLong PuntoGPS[63];	// 63*8 = 504 Bytes: arreglo fijo de hasta 63 puntos GPS
	u32 Padding;			// 4 Bytes: padding para completar 512 bytes
}tsAlarmPointsDescriptor;          // 512 Bytes Total