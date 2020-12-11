/***********************************************************************
 * @file 	:	m702.cpp
 * @author	:	ch
 * @brief	:	
 * @version:	v1.0
 * @Copyright (C)  2020-12-11  .cdWFVCEL. all right reserved
***********************************************************************/

/* 包含头文件 -------------------------------------------------------------------*/
#include "m702.h"
/* 宏定义 -----------------------------------------------------------------------*/
/* 类型定义 ---------------------------------------------------------------------*/
#define BLOCK_TIME 100
/* 私有变量 ---------------------------------------------------------------------*/
u8 revBuf[17] = {0};
/* 扩展变量 ---------------------------------------------------------------------*/
M702 m702;
/* 私有函数声明 -----------------------------------------------------------------*/
/* 函数声明 ---------------------------------------------------------------------*/
/* 函数实现 ---------------------------------------------------------------------*/
bool M702::equels(M702 &m)
{
    if (CO2 == m.CO2 && CH2O == m.CH2O && TVOC == m.TVOC && PM25 == m.PM25 && PM10 == m.PM10 && temperature == m.temperature && humidity == m.humidity)
        return true;
    return false;
}
/********************************************************************************
*  
* @biref： 
* @param：
* @return: 0x00:OK  0xff:faild
********************************************************************************/
u8 M702_Scan()
{
    u32 block_c = 0;
    u8 revByte = SERIAL.read();
    if (revByte == 0X3C)
    {
        revBuf[0] = 0x3C;
        u8 index = 1;
        while (1)
        {
            u8 tmp = SERIAL.read();
            if (tmp != 0xff)
            {
                revBuf[index] = tmp;
                index++;
                block_c = 0;
            }
            block_c++;

            if (block_c > BLOCK_TIME)
            {
                memset(revBuf, 0, sizeof(revBuf));
                return 0xff;
            }
            else if (index == 17)
            {
                u16 checksum = 0;
                for (int i = 0; i < sizeof(revBuf) - 1; i++)
                {
                    checksum += revBuf[i];
                }
                checksum = checksum & 0x00ff;
                if ((u8)checksum == revBuf[sizeof(revBuf) - 1])
                {
                    for (int i = 0; i < sizeof(revBuf); i++)
                    {
                        Serial2.printf("%02x ", revBuf[i]);
                    }
                    Serial2.println();
                    M702 tmp;
                    tmp.CO2 = revBuf[2] << 8 | revBuf[3];
                    tmp.CH2O = revBuf[4] << 8 | revBuf[5];
                    tmp.TVOC = revBuf[6] << 8 | revBuf[7];
                    tmp.PM25 = revBuf[8] << 8 | revBuf[9];
                    tmp.PM10 = revBuf[10] << 8 | revBuf[11];
                    tmp.temperature = (float)(revBuf[12]) + ((float)(revBuf[13]) / 100);
                    tmp.humidity = (float)(revBuf[14]) + ((float)revBuf[15] / 100);

                    if (m702.equels(tmp) == false)
                    {
                        tmp.isnew = true;
                        memcpy(&m702, &tmp, sizeof(M702));
                    }
                }
                else
                {
                    memset(revBuf, 0, sizeof(revBuf));
                    return 0xff;
                }

                return 0;
            }
            delay(1);
        }
    }
    else
    {
        return 0xff;
    }
}
//m702.cpp