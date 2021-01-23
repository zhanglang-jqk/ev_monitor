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
/* 包含头文件 ------------------------------------------------------------------*/
// #include "ch/bsp.h"
#include <Arduino.h>
/* 宏定义 ----------------------------------------------------------------------*/
#define SMART_BUCKLE_NUM 1
/* 类型定义 --------------------------------------------------------------------*/
class SmartBuckle
{
public:
    double temperature = 0; //温度
    uint16_t heartbeat = 0; //心跳
    bool finger_f = 0;      //接触

    bool isNew = false; //is new data

    char* toString();
};

extern SmartBuckle smartBuckles[SMART_BUCKLE_NUM];

/* 变量声明 --------------------------------------------------------------------*/
/* 函数声明 --------------------------------------------------------------------*/
#endif // __MAIN_H_
