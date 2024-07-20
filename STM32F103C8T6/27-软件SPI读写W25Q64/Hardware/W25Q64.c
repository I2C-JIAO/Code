/*
W25Q64简介：
	W25Qxx系列是一种低成本、小型化、使用简单的非易失性存储器，
常应用于数据存储、字库存储、固件程序存储等场景
存储介质：Nor Flash（闪存）
时钟频率：80MHz / 160MHz (Dual SPI) / 320MHz (Quad SPI)
存储容量（24位地址）：
	W25Q40：	  4Mbit / 512KByte
	W25Q80：	  8Mbit / 1MByte
	W25Q16：	  16Mbit / 2MByte
	W25Q32：	  32Mbit / 4MByte
	W25Q64：	  64Mbit / 8MByte
	W25Q128：  128Mbit / 16MByte
	W25Q256：  256Mbit / 32MByte
*/

#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "W25Q64_Ins.h"
//W25Q64_Init函数初始化包括SPI初始化
void W25Q64_Init(void)
{
	MySPI_Init();
}
/*
读取W25Q64ID函数：
	先调用SPI起始条件函数
	再调用SPI交换字节函数写入W25Q64_JEDEC_ID指令获取设备ID
共24位地址，前8位为厂商ID，后16位为设备ID,因交换字节函数每次只能接收或发送一个字节（8位）
所以16位地址变量接收到的第一个字节为高8位（从高到低），需要左移8位后，再接收第2个字节（低8位）
然后进行 |= 操作将低8位数据与高八位数据组成完整的16位字节放入地址变量中。
	最后调用SPI终止条件函数
*/
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID)
{
	MySPI_Start();
	MySPI_SwapByte(W25Q64_JEDEC_ID);
	*MID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	*DID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	*DID <<= 8;
	*DID |= MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	MySPI_Stop();
}    
/*
写使能函数：
	调用SPI起始条件函数
	再调用SPI交换一个字节函数，写入写使能指令
	最后调用SPI终止条件函数
*/
void W25Q64_WriteEnable(void)
{
	MySPI_Start();
	MySPI_SwapByte(W25Q64_WRITE_ENABLE);
	MySPI_Stop();
}
/*
等待忙状态函数：
	调用SPI起始条件函数
	再调用SPI交换一个字节函数，写入读取忙状态寄存器指令
	设置超时时间为100000次计数，用while循环等待非忙状态
	((MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01)：
因8位数据中最后一位代表忙状态，1为忙，0为不忙
将接收到的忙状态 & 0x01判断最后一位是否为1，直到为0跳出等待
等待时间超出设置时长100000则超时
	最后调用SPI终止条件函数
*/
void W25Q64_WaitBusy(void)
{	
	uint32_t Timeout = 100000;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);
	while ((MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01)
	{
		Timeout --;
		if (Timeout == 0)
		{
			break;
		}
	}
	MySPI_Stop();
}
/*
FLASH操作注意事项：
	写入操作时：
		写入操作前，必须先进行写使能
		每个数据位只能由1改写为0，不能由0改写为1
		写入数据前必须先擦除，擦除后，所有数据位变为1
		擦除必须按最小擦除单元进行
		连续写入多字节时，最多写入一页的数据，超过页尾位置的数据，会回到页首覆盖写入
		写入操作结束后，芯片进入忙状态，不响应新的读写操作
	读取操作时：
		直接调用读取时序，无需使能，无需额外操作，没有页的限制，
		读取操作结束后不会进入忙状态，但不能在忙状态时读取
*/
/*
页编程函数：
根据FLASH写入操作特性，
	首先调用写使能函数
	再调用SPI起始条件函数
	再调用SPI交换一个字节函数，写入页编程指令，页编程地址为24位，数据类型定义中没有24位字节的定义，
我们定义函数第一个参数输入为32位地址，第二个参数输入为一个指向数组的指针，第三个参数输入为写入多少个数据
因SPI交换字节函数只能一次交换8位数据（8位有效，多余位舍弃），
24位地址需分开发送，
首先发送高八位（SPI高位先行特性），将地址右移16位后高八位变低八位，
如：0x123456 >> 16 = 0x12; MySPI_SwapByte(0x123456 >> 16); 实际发送12
然后发送中八位,将地址右移8位后低八位被移出舍弃，中8位变低八位，高八位无效，
如：0x123456 >> 8 = 0x1234；MySPI_SwapByte(0x123456 >> 8); 实际发送34
最后发送低八位，因高位无效，低八位可直接发送24位地址本身，如：MySPI_SwapByte(0x123456); 实际发送56
这样就完成了用SPI交换字节函数连续发送24位地址的操作
	之后用for循环来发送数组中的数据
	最后调用SPI终止条件函数
	终止发送后进入忙状态等待，不忙后可进行下一步操作
*/
void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
	uint16_t i;
	W25Q64_WriteEnable();
	MySPI_Start();
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	for (i = 0; i < Count; i ++)
	{
		MySPI_SwapByte(DataArray[i]);
	}
	MySPI_Stop();
	
	W25Q64_WaitBusy();
}
/*
扇区擦除函数：
	首先调用写使能函数
	再调用SPI起始条件函数
	再调用SPI交换一个字节函数，写入扇区擦除（4KB）指令
	函数输入参数为32位地址变量，实际地址为24位，地址写入操作与上面页编程函数一致，不再赘述
	最后调用SPI终止条件函数
	终止发送后进入忙状态等待，不忙后可进行下一步操作
*/
void W25Q64_SectorErase(uint32_t Address)
{
	W25Q64_WriteEnable();
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	MySPI_Stop();
	
	W25Q64_WaitBusy();
}
/*
读取操作时：
		直接调用读取时序，无需使能，无需额外操作，没有页的限制，
		读取操作结束后不会进入忙状态，但不能在忙状态时读取
读取数据函数：
	调用SPI起始条件函数
	再调用SPI交换一个字节函数，写入读取数据指令
	接着发送24位数据指定要读取的地址
	for循环中调用SPI交换一个字节函数
	发送空字节0xFF将指定地址的数据置换过来并传入数组
	最后调用SPI终止条件函数
*/
void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count)
{
	uint32_t i;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_DATA);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	for(i = 0; i < Count; i ++)
	{
		DataArray[i] = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	}
	MySPI_Stop();
	
}
