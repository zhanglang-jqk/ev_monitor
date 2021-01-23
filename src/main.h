/***********************************************************************
 * @file main.h
 * MAIN
 * @author	:	ch
 * @brief	:	
 * @version:	v1.0
 * @Copyright (C)  2020-12-10  .cdWFVCEL. all right reserved
***********************************************************************/

#ifndef __MAIN_H_
#define __MAIN_H_
/* ����ͷ�ļ� ------------------------------------------------------------------*/
// #include "ch/bsp.h"
#include <Arduino.h>
/* �궨�� ----------------------------------------------------------------------*/
#define SMART_BUCKLE_NUM 1
/* ���Ͷ��� --------------------------------------------------------------------*/
class SmartBuckle
{
public:
    double temperature = 0; //�¶�
    uint16_t heartbeat = 0; //����
    bool finger_f = 0;      //�Ӵ�

    bool isNew = false; //is new data

    char* toString();
};

extern SmartBuckle smartBuckles[SMART_BUCKLE_NUM];

/* �������� --------------------------------------------------------------------*/
/* �������� --------------------------------------------------------------------*/
#endif // __MAIN_H_
