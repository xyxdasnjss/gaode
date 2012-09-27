
//_________________________________Copyright (c) 2009,Mapabc Co.,Ltd_____________________________
//                                       All rights reserved.
// 
//   �ļ����ƣ�sa_api.h
//   �ļ���ʶ�������ù���ƻ���
//   ��    ����������Ϣ����ӿ�(Search Api)
// 
//   ��ǰ�汾��1.0
//   ��    �ߣ���С��
//   ������ڣ�2010.01.31
//_______________________________________________________________________________________________

#ifndef	__SA_API__H  
#define __SA_API__H 

#include "sa_types.h"

// POI
MPOISEARCHRESULT *SAHttpPoiSearchByKeywords(char *cKeywords, char *cCity, char *cUserKey, const MPOISEARCHOPTIONS &Options);
MPOISEARCHRESULT *SAHttpPoiSearchByCenPoi(MLONLAT centerPoiXY, char *cKeywords, char *cCity, char *cUserKey, const MPOISEARCHOPTIONS &Options);
MPOISEARCHRESULT *SAHttpPoiSearchByCenKeywords(char *cCenterKeywords, char *cKeywords, char *cCity, char *cUserKey, const MPOISEARCHOPTIONS &Options);

// ROUTE
MAUTONAVISEARCHRESULT *SAHttpRouteSearchByTwoPoi(MLONLAT startPoiXY, MLONLAT endPoiXY, char *cUserKey, const MAUTONAVISEARCHOPTIONS &Options);
MAUTODISSEARCHRESULT  *SAHttpCalculaNaviDistance(char *cXs, char *cYs, char *cUserKey, const MAUTODISSEARCHOPTIONS &Options);

// BUS
MBUSNAVISEARCHRESULT  *SAHttpBusSearchByTwoPoi(MLONLAT startPoiXY, MLONLAT endPoiXY, char *cCity, char *cUserKey, const MBUSNAVISEARCHOPTIONS &Options);
MBUSINFOSEARCHRESULT  *SAHttpBusSearchByBusId(char *cBusId, char *cCity, char *cUserKey, const MBUSINFOSEARCHOPTIONS &Options);
MBUSINFOSEARCHRESULT  *SAHttpBusSearchByBusStation(char *cStationName, char *cCity, char *cUserKey, const MBUSINFOSEARCHOPTIONS &Options);
MBUSINFOSEARCHRESULT  *SAHttpBusSearchByBusName(char *cBusName, char *cCity, char *cUserKey, const MBUSINFOSEARCHOPTIONS &Options);

// COORDINATE
bool SAHttpGPSToOffset(MLONLAT poiXY, MLONLAT &offsetPoiXY, char *cUserKey, const MCOORDINATESEARCHOPTIONS &Options);

// REGEOCODE
MREGEOCODESEARCHRESULT *SAHttpPoiToAddress(MLONLAT poiXY, char *cUserKey, const MREGEOCODESEARCHOPTIONS &Options);
MADDRESSMATHINGRESULT  *SAHttpAddressToPoi(char *cAddress, char *cUserKey);
MBSLRESULT             *SAHttpBaseStationLocation(char *cUserKey);

#endif
//______________________________________________�ļ�����__________________________________________