/***********************************************************************
 * @file computer_int.h
 * COMPUTER_INT
 * @author	:	ch
 * @brief	:	
 * @version:	v1.0
 * @Copyright (C)  2020-12-12  .cdWFVCEL. all right reserved
***********************************************************************/

#ifndef __COMPUTER_INT_H_
#define __COMPUTER_INT_H_
/* ����ͷ�ļ� ------------------------------------------------------------------*/
#include "ch/bsp.h"
/* �궨�� ----------------------------------------------------------------------*/
#define CI_SERIAL   Serial
/* ���Ͷ��� --------------------------------------------------------------------*/
/* �������� --------------------------------------------------------------------*/
/* �������� --------------------------------------------------------------------*/
void CI_Init();
void CI_Scan();
void CI_Transimit();
#endif // __COMPUTER_INT_H_