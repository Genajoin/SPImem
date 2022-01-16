#include "SPImem.h"

void SPImem::ChipErase(void)
{
    _SendCommand(M25_WREN);
    _SendCommand(M25_BE);
}

void SPImem::SectorErase(uint32_t addr_in_sector)
{
    _DataSendReceive(M25_SE);
    _AddressSend(addr_in_sector);
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

void SPImem::WriteByte(uint32_t addr, byte DATA)
{
    _SendCommand(M25_WREN);
    M25_Chip_Select_ENABLE;
    M25_InstructionSend(M25_PP);
    _AddressSend(addr);
    M25_DataSend(DATA);
    M25_Chip_Select_DISABLE;
}

void SPImem::_NotBusy(void)
{
    M25_Chip_Select_ENABLE;
    M25_InstructionSend(M25_RDSR);
    while (M25_DataReceive() & M25_WIP)
    {
    };
    M25_Chip_Select_DISABLE;
}

void SPImem::_AddressSend(uint32_t addr)
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

uint8_t SPImem::_DataSendReceive(uint8_t DATA)
{
    return SPI.transfer(DATA);
}
