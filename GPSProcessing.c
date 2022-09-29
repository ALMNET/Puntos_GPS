#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "main.h"
#include "GPSProcessing.h"

static unsigned char CombinadoHabilitado = 0;

static void eBUS_Combinado_GPS_Alarm_Callback(GPS_Point* pCurrentGPS, unsigned int PointIndex, unsigned int TripReason)
{
	static unsigned long long PointMask = 0;

	if (PointIndex > 63)
		return;

	if (TripReason == GPS_ALARM_TRIPPED_ON_ENTER)
	{
		if (!(PointMask & (1 << PointIndex)))
		{
			//DBG_STRING(DBG_MODULE_EBUS, "GPS Alarm Trip on Enter");
			PointMask |= ((unsigned long long) (1 << PointIndex));
		}
	}
	else if (TripReason == GPS_ALARM_TRIPPED_ON_EXIT)
	{
		if (PointMask & (1 << PointIndex))
		{
			//DBG_STRING(DBG_MODULE_EBUS, "GPS Alarm Trip on Exit");
			PointMask &= ~((unsigned long long) (1 << PointIndex));
		}
	}

	if (PointMask)
		CombinadoHabilitado = 1;
	else
		CombinadoHabilitado = 0;
}


static GPS_Alarm GPSAlarms[GPS_MAX_ALARMS];

/**
 * @brief 		Recupera los puntos GPS de combinación desde la SD
 *
 * @param[in] 	Ninguno
 *
 * @retval		Ninguno
 */

/*
static void eBUS_Read_GPS_Alarms(void)
{
	tsSdStruct SD_Struct;
	GPS_Alarm* eBUSCombinadoGPSAlarm = NULL;
	tsAlarmPointsDescriptor AlarmPointDescriptor;
	int i =0;
	unsigned int Offset = 0;

	//DBG_STRING(DBG_MODULE_EBUS, "Reading GPS alarms");

	while (!SdWaitForCmdComplete(SdRead(EBUS_GPS_ALARM_CFG_FILENAME, Offset, (unsigned char*) &AlarmPointDescriptor, sizeof(tsAlarmPointsDescriptor), 0), &SD_Struct))
	{
		if (!SD_Struct.bytes)
		{
			break;
		}
		else if (AlarmPointDescriptor.Linea && (var_StatUVP.Linea == AlarmPointDescriptor.Linea))
		{
			if ((eBUSCombinadoGPSAlarm = GPS_AlarmCreate(eBUS_Combinado_GPS_Alarm_Callback, AlarmPointDescriptor.UmbralDistancia, GPS_ALARM_FLAG_ON_ENTER | GPS_ALARM_FLAG_ON_EXIT)))
			{
				for (i=0; i< 63; i++)
				{
					if (AlarmPointDescriptor.PuntoGPS[i].Latitude)
					{
						DBG_STRING(DBG_MODULE_EBUS, "Agregando punto combinado");
						DBG_FLT(DBG_MODULE_EBUS, "Lat: ", AlarmPointDescriptor.PuntoGPS[i].Latitude);
						DBG_FLT(DBG_MODULE_EBUS, "Long: ", AlarmPointDescriptor.PuntoGPS[i].Longitude);
						GPS_AlarmAddPoint(eBUSCombinadoGPSAlarm, AlarmPointDescriptor.PuntoGPS[i].Latitude, AlarmPointDescriptor.PuntoGPS[i].Longitude);
					}
					else
						break;
				}
				GPSAlarmInitialized = 1;
			}
			break;
		}
		else
			Offset += sizeof(tsAlarmPointsDescriptor);
	}
}

*/




GPS_Alarm* GPS_AlarmCreate(void (*pfnCbk)(GPS_Point* , unsigned int, unsigned int ), unsigned int DistThreshold, unsigned int Flags)
{
	int i = 0;

	if (!pfnCbk)
		return 0;

	for (i=0; i <GPS_MAX_ALARMS; i++)
	{
		if (!(GPSAlarms[i].Flags & GPS_ALARM_INITIALIZED))
		{
			GPSAlarms[i].pfnCallback = pfnCbk;
			GPSAlarms[i].pAlarmPoints = 0;
			GPSAlarms[i].DistanceThreshold = DistThreshold;
			GPSAlarms[i].Flags = GPS_ALARM_INITIALIZED | Flags;
			return &GPSAlarms[i];
		}
	}

	return 0;
}




/**
 * @brief 		Recupera los puntos GPS de combinación desde la SD
 *
 * @param[in] 	Alarm       Cadena de puntos GPS
 * 
 * @param[in] 	Latitude    Punto GPS de Latitud a agregar
 * 
 * @param[in] 	Longitude   Punto GPS de Longitud a agregar
 *
 * @retval		Ninguno
 */

void GPS_AlarmAddPoint(GPS_Alarm* Alarm, float Latitude, float Longitude)
{
	Alarm_Point* pAlarmPoint;
	Alarm_Point* pAlarmPointScan;

	if (!(Alarm->Flags & GPS_ALARM_INITIALIZED))
		return;

	if ((pAlarmPoint = (Alarm_Point*) pvPortMalloc(sizeof(Alarm_Point))))
	{
		pAlarmPoint->Lat = Latitude;
		pAlarmPoint->Long = Longitude;
		pAlarmPoint->Status = 0;
		pAlarmPoint->pNextPoint = 0;

		if (!Alarm->pAlarmPoints)               // Si Alarm no tiene punto GPS
			Alarm->pAlarmPoints = pAlarmPoint;  // Guarda en Alarm el punto GPS ingresado
		else
		{
			pAlarmPointScan = Alarm->pAlarmPoints;
			while (pAlarmPointScan->pNextPoint)
				pAlarmPointScan = pAlarmPointScan->pNextPoint;
			pAlarmPointScan->pNextPoint = pAlarmPoint;
		}
	}
}





void GPS_AlarmDestroy(GPS_Alarm* Alarm)
{
	Alarm_Point* pAlarmPointScan;
	Alarm_Point* pAlarmPointRemove;

	pAlarmPointScan = Alarm->pAlarmPoints;

	while (pAlarmPointScan)
	{
		pAlarmPointRemove = pAlarmPointScan;
		pAlarmPointScan = pAlarmPointScan->pNextPoint;
		vPortFree(pAlarmPointRemove);
	}
	memset(Alarm, 0, sizeof(GPS_Alarm));
}