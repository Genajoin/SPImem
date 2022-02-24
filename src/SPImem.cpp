#include "SPImem.h"

SPImem::SPImem(uint8_t cs, SPIClass *spi)
{
    _cs = cs;
    _spi = spi;
}

SPImem::SPImem()
{
    SPImem(SS, &SPI);
}

void SPImem::ChipErase(void)
{
    _SendCommand(M25_WREN);
    _SendCommand(M25_BE);
}

void SPImem::SectorErase(uint32_t addr_in_sector)
{
    _SendCommand(M25_WREN);
    M25_Chip_Select_ENABLE;
    _DataSendReceive(M25_SE);
    _AddressSend(addr_in_sector);
    M25_Chip_Select_DISABLE;
}

uint8_t SPImem::ReadSR()
{
    M25_Chip_Select_ENABLE;
    M25_InstructionSend(M25_READ);
    uint8_t result = M25_DataReceive();
    M25_Chip_Select_DISABLE;
    return result;
}

uint8_t SPImem::ReadByte(uint32_t addr)
{
    M25_Chip_Select_ENABLE;
    M25_InstructionSend(M25_READ);
    _AddressSend(addr);
    uint8_t result = M25_DataReceive();
    M25_Chip_Select_DISABLE;
    return result;
}

void SPImem::ReadBytes(uint32_t addr, byte *buf, int len)
{
    M25_Chip_Select_ENABLE;
    M25_InstructionSend(M25_READ);
    _AddressSend(addr);
    for (int i = 0; i < len; i++)
    {
        buf[i] = M25_DataReceive();
    }
    M25_Chip_Select_DISABLE;
}

void SPImem::WriteByte(uint32_t addr, byte DATA)
{
    _SendCommand(M25_WREN);
    M25_Chip_Select_ENABLE;
    M25_InstructionSend(M25_PP);
    _AddressSend(addr);
    M25_DataSend(DATA);
    M25_Chip_Select_DISABLE;
}

//* write up to 256 bytes
void SPImem::WriteBytes(uint32_t addr, byte *buf, int len)
{
    M25_Chip_Select_ENABLE;
    M25_InstructionSend(M25_PP);
    _AddressSend(addr);
    for (int i = 0; i < len; i++)
    {
        M25_DataSend(buf[i]);
    }
    M25_Chip_Select_DISABLE;
}

bool SPImem::isBusy(void)
{
    M25_Chip_Select_ENABLE;
    M25_InstructionSend(M25_RDSR);
    bool result =  M25_DataReceive() & M25_WIP;
    M25_Chip_Select_DISABLE;
    return result;
}

void SPImem::NotBusyWait(void)
{
    M25_Chip_Select_ENABLE;
    M25_InstructionSend(M25_RDSR);
    while (M25_DataReceive() & M25_WIP)
    {
    };
    M25_Chip_Select_DISABLE;
}

inline void SPImem::_AddressSend(uint32_t addr)
{
    M25_DataSend((addr & 0x00FF0000) >> 16);
    M25_DataSend((addr & 0x0000FF00) >> 8);
    M25_DataSend((addr & 0x000000FF));
}

inline void SPImem::_SendCommand(uint8_t com)
{
    M25_Chip_Select_ENABLE;
    M25_InstructionSend(com);
    M25_Chip_Select_DISABLE;
}

inline uint8_t SPImem::_DataSendReceive(uint8_t DATA)
{
    return _spi->transfer(DATA);
}
