#include <stdint.h>

#define EBUS_GPS_ALARM_NEW_FILENAME		"ALRMCFG3_NEW.bin"
#define EBUS_GPS_ALARM_CFG_FILENAME		"ALRMCFG3.bin"

// Arreglar
//#define pvPortMalloc malloc
//#define vPortFree    free      


typedef uint8_t  u8;
typedef uint16_t  u16;
typedef uint32_t  u32;

#ifndef	_GPS_PROCESSING_H_
#define	_GPS_PROCESSING_H_

/*
 * R(Lat)= SQRT( (REq^2*cos(Lat))^2 + (RPol^2*sin(Lat))^2 / ((REq*cos(Lat))^2 + (RPol*sin(Lat))^2) )
 */
#define RADIO_ECUATORIAL 			6378135		//mts
#define RADIO_POLAR 				6356750		//mts

#define GPS_POINT_HISTORY			16

#define GPS_MAX_INVALID_POINTS		600		//600 puntos invalidos X 1 seg = 10 minutos
#define GPS_MIN_SATS				3
#define GPS_MAX_BAD_POINTS			10		//Cantidad máxima de paquetes GPS juzgados válidos por RMC pero no confiables por GGA

#define GPS_VEL_SAMPLES				8
#define GPS_VEL_HYSTERESIS			0		//histéresis en km/h

/*********************************************************/
#define GPS_MAX_ALARMS				10

#define GPS_ALARM_FLAG_ON_ENTER			0x00000001
#define GPS_ALARM_FLAG_ON_EXIT			0x00000002
#define GPS_ALARM_INITIALIZED			0x80000000

#define GPS_ALARM_TRIPPED_ON_ENTER		0x00000001
#define GPS_ALARM_TRIPPED_ON_EXIT		0x00000002

typedef enum
{
	POINT_NOT_RELEVANT = 0,
	POINT_NO_PRIOR_REPORT,
	POINT_MAX_INACT_DISTANCE,
	POINT_HAS_STOPPED,
	POINT_HAS_SPEDUP,
	POINT_MAX_ACT_TIME,
	POINT_HAS_TURNED,
	POINT_MAX_INACT_TIME
}GPS_Point_Relevance;

#pragma pack(1)

typedef struct __attribute__((packed))
{
	float Speed;					//Rapidez en km/h
	float Course;					//Curso en tierra. 0º equivale al norte
}Velocity_Vector;

typedef struct __attribute__((packed))
{
	unsigned char UTC_Hour;
	unsigned char UTC_Min;
	unsigned char UTC_Sec;

	unsigned char UTC_Day;
	unsigned char UTC_Month;
	unsigned char UTC_Year;

	unsigned char Satellites;						//Cantidad de satélites trackeados por el módulo en el momento de la toma del punto gps

	unsigned char Flags;		//

	GPS_Point_Relevance Relevance;

	float Lat;							//Latitud
	float Long;							//Longitud

	unsigned int DistanceIncrement;			//Distancia desde el punto GPS relevante anterior
	unsigned int TimeIncrement;				//Segundos desde el punto GPS relevante anterior

	Velocity_Vector CurrentVelocity;
	Velocity_Vector MeanVelocity;
}GPS_Point;

/**
 * @brief 	Esta estructura contiene puntos GPS siemples, solo contienen latitud y longuitud, ambas con signo.
 * 			Lo básico para realizar cálculos de distancia.
 *
 */
typedef struct
{
	float Lat;							//Latitud con signo
	float Long;							//Longitud con signo

}tsGPSSimplePoint;


typedef struct __attribute__((packed)) _Alarm_Point
{
	float Lat;							//Latitud
	float Long;							//Longitud
	unsigned int Status;
	struct _Alarm_Point* pNextPoint;
}Alarm_Point;


typedef struct __attribute__((packed))
{
	void (*pfnCallback)(GPS_Point* pCurrentGPS, unsigned int PointIndex, unsigned int TripReason);
	unsigned int Flags;
	unsigned int DistanceThreshold;
	Alarm_Point* pAlarmPoints;
}GPS_Alarm;


typedef struct __attribute__((packed))
{
	GPS_Point tsGPS_Last_Point;
	unsigned char SatelliteID[12];					//IDs de los satélites actualmente utilizados
	unsigned char SNR[32];							//relaciones señal a ruido
	float PDOP;							//Position dilution of precision
	unsigned char CurrentSatellites;				//Satélites actuales
}GPS_Status_Params;

#pragma pack()

#define GPS_FLAGS_VALID			(1 << 0)
#define GPS_FLAGS_EnW			(1 << 1)
#define GPS_FLAGS_NnS			(1 << 2)

#define NEW_GPS_OUTPUT_QUEUE(x) xQueueCreate(x, sizeof(GPS_Point))

//MTK NMEA checksum calculator:
//http://www.hhhh.org/wiml/proj/nmeaxor.html


//void GPS_Processor_Init( float MinSpeed, float MaxAngle, unsigned int MaxActTime, unsigned int MaxInactTime, unsigned int MaxDistance, QueueHandle_t OutQ );
unsigned char GPS_GetLastPoint(GPS_Point* OutpGPS);
unsigned char GPS_GetLastReportedPoint(GPS_Point* OutpGPS);

unsigned char GPS_Distancia( GPS_Point* Point1, GPS_Point* Point2, unsigned int* Distancia);
unsigned char GPS_Distancia_SimplePoint( tsGPSSimplePoint* Point1, tsGPSSimplePoint* Point2, unsigned int* Distancia);
unsigned char GPS_Rumbo( GPS_Point* Point1, GPS_Point* Point2, float* Rumbo);
void Format_LatLong_To_ddmmmmmm(double LatLong, unsigned char* buffer);
void Format_ddmmmmmm_To_LatLong(unsigned int origen, float* destino);

GPS_Alarm* GPS_AlarmCreate(void (*pfnCbk)(GPS_Point* , unsigned int, unsigned int ), unsigned int DistThreshold, unsigned int Flags);
void GPS_AlarmAddPoint(GPS_Alarm* Alarm, float Latitude, float Longitude);
void GPS_AlarmDestroy(GPS_Alarm* Alarm);
void GPS_AlarmCheckIfTripped(GPS_Point* pGPSPoint);
float GPS_GetPDOP(void);
float GPS_GetMeanSNR(void);
unsigned char GPS_GetSNRs(unsigned char* Out);
unsigned char GPS_GetCurrentSatellites(void);
unsigned char GPS_Get_Status_Params(GPS_Status_Params* pParams);

unsigned char GPS_Report(GPS_Point* NewPoint);
unsigned char GPS_Check_NMEA_Checksum(char* Message);
unsigned char GPS_Parse_RMC_Packet( char *cadena, GPS_Point* NewPoint);
void GPS_Parse_GGA_Packet( char *cadena);
void GPS_Parse_GSA_Packet( char *cadena);
void GPS_Parse_GSV_Packet( char *cadena);

#endif
