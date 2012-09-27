
//_________________________________Copyright (c) 2009,Mapabc Co.,Ltd_____________________________
//                                       All rights reserved.
// 
//   �ļ����ƣ�sa_types.h
//   �ļ���ʶ�������ù���ƻ���
//   ��    ����������Ϣ�������Ͷ���
// 
//   ��ǰ�汾��1.0
//   ��    �ߣ���С��
//   ������ڣ�2010.01.31
//_______________________________________________________________________________________________

#ifndef	__SA_TYPES__H  
#define __SA_TYPES__H 

#include <string.h>
#include "ut_types.h"
#include "htl.h"

/////////////////////////////////POI��ѯ/////////////////////////////////

struct MPOISEARCHOPTIONS // POI��ѯ����
{
	MPOISEARCHOPTIONS()
	{
		lRecordsPerPage = 10;    // Ĭ��Ϊ10
		lPageNum        = 1;     // Ĭ��Ϊ1
		lRange          = 3000;  // Ĭ��Ϊ3000
		memset(cSearchType, 0, sizeof(cSearchType));
		strcpy(cSrcType, "POI"); // Ĭ��ΪPOI
		cSr             = 0;     // Ĭ�ϰ��չؼ�������
		cLanguage       = 0;     // Ĭ��Ϊ���� // ��Ӣ��
	};

	long lRecordsPerPage; // ÿҳ��¼��
	long lPageNum;        // ��ҳ��(��ʾ�ڼ�ҳ)
	long lRange;          // �ܱ߲�ѯ��Χ,��λ��
	char cSearchType[64]; // ��ѯ����,�ɲ��� ����ο��û��ֲ�
	char cSrcType[32];    // ����Դ���� Ӣ�Ŀ� mpoi�������� POI�� ��ҵ�ر� DIBIAO������վ̨ BUS��ͨ�ñ༭�� USERPOI��
	                      // ��������Դ���Ի�ϲ�ѯ�����繫������+���������ݲ�ѯBUS:1000%2bPOI,����ǰ1000������Ϊ�������ݣ�����Ϊ����������
	char cSr;             // cSr=0�����չؼ������� cSr=1�����վ�������
	char cLanguage;       // ��ѯ���� 0���� 1Ӣ�� // ��Ӣ��
};

struct MPLATFORM // վ̨��Ϣ
{
	MLONLAT Coor; // վ̨����
};

struct MPOI // POI(��Ȥ��)
{
	unsigned short cAddress[128];  // ��ַ
	unsigned short cName[64];      // ����
	unsigned short cType[64];      // ����
	unsigned short cUrl[256];      // Url��ַ
	unsigned short cCityCode[64];  // ���б���
	unsigned short cPguid[32];     // Ψһ���
	unsigned short cTel[64];       // �绰����
	unsigned short cImageid[64];   // ͼƬID
	double         X;             // X����
	double         Y;             // Y����
	long           lDistance;      // �ܱ߲�ѯʱ,�������ĵ����
	long           lNumOfPlatForm; // վ̨���� xml��չ��Ϣ
	MPLATFORM      *pPlatForms;    // ����վ̨ xml��չ��Ϣ
};

struct MPOISEARCHRESULT // POI��ѯ���
{
	long  lCountNum; // ʵ�ʷ��ص�����������,���û���Ȩ���й�
	long  lTotalNum; // ���ϲ�ѯ������������
	long  lNum;      // ��ҳ���ص���������

	MPOI *pPois;     // �����POI���
};

/////////////////////////////////�ݳ���ѯ/////////////////////////////////

//*****************�ݳ�����*****************

struct REGION // ��������
{
	htl::vector<MLONLAT> lonlats; // ��������������
};

struct MAUTONAVISEARCHOPTIONS // �ݳ�������ѯ����
{
	MAUTONAVISEARCHOPTIONS()
	{
		lPer             = 1; // Ĭ�Ϸ���50����
		lRouteType       = 0; // Ĭ��ʱ������
		lAvoidanceType   = 3; // Ĭ��������ü����Ʊ���
		lSegmentCoorType = 1; // Ĭ��ֻ����·����㡢�յ�

		memset(cName, 0, sizeof(cName));
	};

	long lPer;                     // ��ϡֵ ����·���ܳ���Ϊ1000����,���ǰѳ�ϡֵ��Ϊ150,���ܳ��Ⱦ�ֻ����150����,1����50����
	long lRouteType;               // ·�����㷽ʽ 
	                               // lRouteType=0  ʱ������
	                               // lRouteType=1  ��������(�����շ�·������·)
	                               // lRouteType=2  ��������
	                               // lRouteType=3  ���߿���·(���߿���·,����������·)
	                               // lRouteType=9  �����(ͬʱʹ��ʱ�����ȡ��������ȡ����������������Լ���·��)
	                               // lRouteType=10 ��������
	                               // lRouteType=11 ʡ������
	long lSegmentCoorType;         // ·���������� 
	                               // lSegmentCoorType = 0 ������·������ 
	                               // lSegmentCoorType = 1 ֻ����·����㡢�յ� 
	                               // lSegmentCoorType = 2 ����·����������

	htl::vector<REGION> Regions;   // ��������
	char cName[64];                // ������������
	long lAvoidanceType;           // ·���滮ʱ���õ���Ϣ,Regions��cName��һ����Ϊ��ʱ��,avoidanceType�Ͳ���Ϊ��,�����ѯ�޽��.
	                               // lAvoidanceType=1 �������
	                               // lAvoidanceType=2 ���Ʊ���
	                               // lAvoidanceType=3 ������ü����Ʊ���
	htl::vector<MLONLAT> PassByXY; // ;��������
};

struct MAUTOSEGMENT // ·��
{
	MAUTOSEGMENT()
	{
		lRoadLength = 0;
		memset(cForm, 0, sizeof(cForm));
		memset(cGrade, 0, sizeof(cGrade));
		memset(cDirection, 0, sizeof(cDirection));
		memset(cRoadName, 0, sizeof(cRoadName));
		memset(cDriveTime, 0, sizeof(cDriveTime));
		memset(cAction, 0, sizeof(cAction));
		memset(cAccessorialInfo, 0, sizeof(cAccessorialInfo));
		memset(cTextInfo, 0, sizeof(cTextInfo));
	};
	
	long           lRoadLength;          // ·�γ���
	unsigned short cForm[32];            // ��·����
	unsigned short cGrade[32];           // ��·�ȼ�
	unsigned short cDirection[16];       // ��ʹ����
	unsigned short cRoadName[64];        // ��·����
	unsigned short cDriveTime[32];       // ������ʻʱ��
	unsigned short cAction[16];          // ��������
	unsigned short cAccessorialInfo[32]; // ����
	unsigned short cTextInfo[128];       // ���ε�·��ʹ��������
	htl::vector<MLONLAT> Coor;           // ·������
};

struct MAUTONAVISEARCHRESULT // �ݳ�������ѯ���
{
	MAUTONAVISEARCHRESULT()
	{
		lTotalNum = 0;
		pSegments = NULL;
	};
	
	long                 lTotalNum; // ·�߰�����·������
	MAUTOSEGMENT        *pSegments; // �����·�ν��

	htl::vector<MLONLAT> Bound;     // ·����Ӿ��� ���¡���������
	htl::vector<MLONLAT> Coors;     // ·������
};

//*****************�ݳ���������*****************

struct MAUTODISSEARCHOPTIONS // �ݳ����������ѯ����
{
	MAUTODISSEARCHOPTIONS()
	{
		lRouteType = 0; // Ĭ��ʱ������
	};

	long lRouteType; // ·�����㷽ʽ lRouteType=0 ʱ������; lRouteType=1 ��������
};

struct MAUTOROUTE // ·��
{
	long lRoadLength; // ·�߳���
};

struct MAUTODISSEARCHRESULT // �ݳ����������ѯ���
{
	MAUTOROUTE *pRoutes; // �����·�߽��
};

/////////////////////////////////������ѯ/////////////////////////////////

//*****************��������*****************

struct MBUSNAVISEARCHOPTIONS // ����������ѯ����
{
	MBUSNAVISEARCHOPTIONS()
	{
		lPer       = 1; // Ĭ�Ϸ���50����
		lRouteType = 0; // Ĭ��ʱ������
	};

	long lPer;       // ��ϡֵ ����·���ܳ���Ϊ1000����,���ǰѳ�ϡֵ��Ϊ150,���ܳ��Ⱦ�ֻ����150����,1����50����
	long lRouteType; // ·�����㷽ʽ 0Ĭ�ϡ�1��á�2���ٻ��ˡ�3���ٲ��С�4������                            
};

struct MBUSDEPOTNAME // ����վ������
{
	MBUSDEPOTNAME()
	{
		memset(cName, 0, sizeof(cName));
	};
	
	unsigned short cName[32]; // ����
};

struct MBUSSEGMENT // ·��
{
	MBUSSEGMENT()
	{
		memset(cStartName, 0, sizeof(cStartName));
		memset(cEndName, 0, sizeof(cEndName));
		memset(cBusName, 0, sizeof(cBusName));
		lDriverLength   = 0; 
		lFootLength     = 0; 
		lPassDepotCount = 0;
	};
	
	unsigned short cStartName[32];            // �������
	unsigned short cEndName[32];              // �յ�����
	unsigned short cBusName[64];              // ��������
	htl::vector<MBUSDEPOTNAME> PassDepotName; // ;��վ������
	long lDriverLength;                       // ��ʻ����
	long lFootLength;                         // ���о���
	long lPassDepotCount;                     // ;��վ����
	htl::vector<MLONLAT> CoordinateList;      // ·������
	htl::vector<MLONLAT> PassDepotNameCoor;   // ;��վ������
};

struct MBUSPROJECT // ���˷���
{
	MBUSPROJECT()
	{
		memset(cExpense, 0, sizeof(cExpense));
		lSegmentCount  = 0; 
		pSegments      = NULL; 
		lFootEndLength = 0;
	};
	
	unsigned short cExpense[32];  // ���˷����۸�
	long           lSegmentCount; // ���˴���
	MBUSSEGMENT   *pSegments;     // �����·�ν��

	long lFootEndLength;          // ���˷���������,���е��յ����
	htl::vector<MLONLAT> Bound;   // ·����Ӿ��� ���¡���������
};

struct MBUSNAVISEARCHRESULT // ����������ѯ���
{
	MBUSNAVISEARCHRESULT()
	{
		lTotalNum     = 0; 
		pBussProjects = NULL; 
	};
	
	long         lTotalNum;     // ���˷�������
	MBUSPROJECT *pBussProjects; // ����Ļ��˷������
};

//*****************������Ϣ��ѯ*****************

struct MBUSINFOSEARCHOPTIONS // ������Ϣ��ѯ����
{
	MBUSINFOSEARCHOPTIONS()
	{
		lRecordsPerPage = 10; // Ĭ��Ϊ10
	    lPageNum        = 1;  // Ĭ��Ϊ1
		lResData        = 0;  // Ĭ�ϲ�����xml����չ��Ϣ
	};

	long lRecordsPerPage; // ÿҳ��¼��
	long lPageNum;        // ��ҳ��(��ʾ�ڼ�ҳ)
	long lResData;        // �Ƿ񷵻�xml����չ��Ϣ(xml��չ��ϢĿǰ��;������Ϣ ) 0������ 1����                        
};

struct MSTATION // ��վ
{
	MSTATION()
	{
		memset(cName, 0, sizeof(cName));
		memset(&Coor, 0, sizeof(Coor));
		memset(cSpell, 0, sizeof(cSpell));
		memset(cCode, 0, sizeof(cCode));
		lStationNum = 0; 
	};
	
	unsigned short cName[32];   // ��վ����
	MLONLAT        Coor;        // ��վ����
	unsigned short cSpell[128]; // ��վ����ƴ��
	unsigned short cCode[32];   // ����
	long           lStationNum; // ��վ����
};

struct MBUSINFO // ������Ϣ
{
	MBUSINFO()
	{
		fLength = 0; 
		memset(cName, 0, sizeof(cName));
		memset(cLine_id, 0, sizeof(cLine_id));
		memset(cStartName, 0, sizeof(cStartName));
		memset(cEndName, 0, sizeof(cEndName));
		memset(cStartTime, 0, sizeof(cStartTime));
		memset(cEndTime, 0, sizeof(cEndTime));
		memset(cCompany, 0, sizeof(cCompany));	
		fBasicPrice   = 0;
		fTotakPrice   = 0;
		bIcCard       = false;
		lNumOfStation = 0;
		pStations     = NULL;
	};
	
	float                 fLength;        // ����·�߳���
	unsigned short        cName[32];      // ��������
	unsigned short        cLine_id[64];   // ������·ID
	unsigned short        cStartName[32]; // �������
	unsigned short        cEndName[32];   // �յ�����
	unsigned short        cStartTime[32]; // ����ʱ��
	unsigned short        cEndTime[32];   // �ճ�ʱ��
	unsigned short        cCompany[64];   // ������˾
	float                 fBasicPrice;    // һվƱ��
	float                 fTotakPrice;    // ��Ʊ��
	bool                  bIcCard;        // �Ƿ����ʹ��IC��
	long                  lNumOfStation;  // ;����վ������ xml��չ��Ϣ
	MSTATION             *pStations;      // ;����վ       xml��չ��Ϣ
	htl::vector<MLONLAT>  CoordinateList; // ·������       xml��չ��Ϣ
};

struct MBUSINFOSEARCHRESULT // ������Ϣ��ѯ���
{
	MBUSINFOSEARCHRESULT()
	{
		lTotalNum  = 0; 
		lNum       = 0;
		pBussInfos = NULL;  
	};
	
	long      lTotalNum;  // ���ϲ�ѯ������������
	long      lNum;       // �˴η��ص���������

	MBUSINFO *pBussInfos; // ����Ĺ�����Ϣ
};

/////////////////////////////////�����ѯ/////////////////////////////////

struct MCOORDINATESEARCHOPTIONS // �����ѯ����
{
	MCOORDINATESEARCHOPTIONS()
	{
		lCr = 0; // ?
	};

	long lCr; // ?                           
};

struct MCOORDINATESEARCHRESULT // �����ѯ���
{
	MLONLAT Coor; // ��ѯ���� ��γ��
};

/////////////////////////////////λ�ò�ѯ/////////////////////////////////

struct MREGEOCODESEARCHOPTIONS // λ�ò�ѯ����
{
	MREGEOCODESEARCHOPTIONS()
	{
		lPoiNumber = 10;  // Ĭ�Ϸ���10��POI��
		lRange     = 300; // Ĭ�Ͼ��뷶ΧΪ300��
		lPattern   = 0;   // Ĭ�Ϸ��صر���POI
		lRoadLevel = 0;   // Ĭ�ϵ�·�ȼ�0-11��Χ�ĵ�·
		memset(cExkey, 0, sizeof(cExkey));
	};

	long lPoiNumber; // �����ܱߵ�POI����;
	long lRange;     // �޶��ܱ��ȵ�POI�͵�·�ľ��뷶Χ
	long lPattern;   // �������ݵ�ģʽ,0Ϊ���صر���POI,1Ϊ����ȫ��POI,���xy�ڷ�����������Ϊ0Ч���Ϻ�,���xy��ƫԶ��������1����
	long lRoadLevel;     // ��·�ȼ� lRoadLevel �˲���ĿǰΪ0��3���͹��ұ�׼�ĵ�·�ȼ����ֵĶ�Ӧ��ϵ����:
	// lRoadLevel��0,����ı�׼��·�ȼ�0-11��Χ�ĵ�·
	// lRoadLevel��1,����ı�׼��·�ȼ�2-11��Χ�ĵ�·
	// lRoadLevel��2,����ı�׼��·�ȼ�7-11��Χ�ĵ�·
	// lRoadLevel��3,����ı�׼��·�ȼ�9-11��Χ�ĵ�·
	// ���ұ�׼�ĵ�·�ȼ����ֹ�12��,�ο����£�
	// 0 ͼ���� δ������·
	// 1 �ǵ�����·
	// 2 ��ͨ��·
	// 3 ��Ҫ��·
	// 4 ������ڲ���·
	// 5 �繫·
	// 6 �ص�
	// 7 ��Ҫ��·
	// 8 ��Ҫ��֡����п��ٵ�
	// 9 ʡ��
	// 10 ����
	// 11 ���ٹ�·

	char cExkey[64]; // ���������ų��ؼ���
};

struct MPROVINCE // ʡ
{
	unsigned short cName[32]; // ʡ��
	long           lCode;     // ʡ���
};

struct MCITY // ��
{
	unsigned short cName[32];   // ����
	long           lCode;       // �б��
	unsigned short cTelnum[16]; // �绰����
};

struct MDISTRICT // ����
{
	unsigned short cName[32]; // ������
	long           lCode;     // ������
};

struct MROUNDROAD // �ܱߵ�·
{
	long           lId;            // ��·id
	unsigned short cName[32];      // ��·��
	unsigned short cDirection[32]; // ������ĵ㷽��
	float          fDistance;      // ������ĵ����
	long           lWidth;         // ·��
};

struct MROUNDPOI // �ܱ���Ȥ��
{
	unsigned short cType[64];      // ����
	unsigned short cPguid[32];     // Ψһ���
	unsigned short cName[32];      // ����
	unsigned short cAddress[128];  // ��ַ
	unsigned short cTel[64];       // �绰����
	unsigned short cDirection[32]; // ������ĵ㷽��
	float          fDistance;      // ������ĵ����
	MLONLAT        Coor;           // ����
};

struct MCROSSPOI // ������Ȥ��
{
	unsigned short cName[32]; // ����
	MLONLAT        Coor;      // ����
};

struct MREGEOCODESEARCHRESULT // λ�ò�ѯ���
{
	MPROVINCE   Province;        // ʡ
	MCITY       City;            // ��
	MDISTRICT   District;        // ����
	long        lNumOfRoundRoad; // �ܱߵ�·������
	MROUNDROAD *pRoundRoads;     // �ܱߵ�·
	long        lNumOfRoundPoi;  // �ܱ���Ȥ�������
	MROUNDPOI  *pRoundPois;      // �ܱ���Ȥ��
	long        lNumOfCrossPoi;  // ������Ȥ�������
	MCROSSPOI  *pCrossPois;      // ������Ȥ��
};

struct MADDRESSPOI // ��ַƥ����Ȥ��
{
	unsigned short cName[32]; // ����
	MLONLAT        Coor;      // ����
};

struct MADDRESSMATHINGRESULT // ��ַƥ���ѯ���
{
	long         lNumOfAddressPoi; // ��ַƥ����Ȥ�������
	MADDRESSPOI *pAddressPois;     // ��ַƥ����Ȥ��
};

struct MBSLRESULT // ��վ��λ��ѯ���
{
	unsigned short cCityCode[64]; // ���б���
	long           lRadius;       // ���뾶
	MLONLAT        Coor;          // ����
};

#endif
//______________________________________________�ļ�����__________________________________________
