/*!
 * @file       mcp41.h
 * @brief      mcp41数字电位器函数实现(带OLED版本)
 * @author     llm
 */
#ifndef __SELFBUILD_MCP41_H
#define __SELFBUILD_MCP41_H

#include "headfile.h"

#define MCP41_SCK		A21
#define MCP41_SDA		A20

#define SPI_MCP41_SCKL	gpio_set(MCP41_SCK, 0)
#define SPI_MCP41_SCKH	gpio_set(MCP41_SCK, 1)
#define SPI_MCP41_SDAL	gpio_set(MCP41_SDA, 0)
#define SPI_MCP41_SDAH	gpio_set(MCP41_SDA, 1)


void MCP41_Init(void);
void MCP41_SetCmd(uint8 Set_Num ,uint8 data);
uint8 MCP41_SetValue(uint8 Set_Num, uint8 Ask_Value);

#endif
