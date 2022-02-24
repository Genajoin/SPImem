#ifndef SPIMEM_H
#define SPIMEM_H

#include <Arduino.h>
#include <SPI.h>

/* M25 instruction definition */

#define M25_WREN 0x06     /*  Write Enable instruction */
#define M25_WRDI 0x04     /*  Write Disable instrction */
#define M25_RDSR 0x05     /*  Read Status Register instruction */
#define M25_WRSR 0x01     /*  Write Status Register instruction */
#define M25_PP 0x02       /*  Page Program instruction */
#define M25_SE 0xD8       /*  Sector Erase instruction */
#define M25_BE 0xC7       /*  Bulk Erase instruction */
#define M25_DP 0xB9       /*  Deep Power-down instruction */
#define M25_RES 0xAB      /*  Release from Deep Power-down, and Read */
                          /*  Electronic Signature */
#define M25_READ 0x03     /*  Read Data Byte instruction */
#define M25_FASTREAD 0x0B /*  Read Data Byte at Higher Speed */
/* Status Register Bit definition */

#define M25_WIP 0x01 /*  Write In Progress bit, polling it to establish */
                     /*  when the previous write cycle or erase cycle
                                     is complete. */
#define M25_WEL 0x02 /*  Write Enable Latch bit indicates the status \
                      of the internal write enable latch. */
#define M52_BP0 0x04 /*  Block Protect bits, they define the area to \
                      be software */
#define M25_BP1 0x08 /* protected against program and erase \
                     instructions. */

#define M25_SRWD 0x80 /* The Status Register Write Protect, */

#define Dummy 0x00

#define M25_Chip_Select_ENABLE digitalWrite(_cs, LOW)
#define M25_Chip_Select_DISABLE digitalWrite(_cs, HIGH)
#define M25_InstructionSend(Instruction) _DataSendReceive(Instruction)
#define M25_DataSend(Data) _DataSendReceive(Data)
#define M25_DataReceive() _DataSendReceive(Dummy)

class SPImem
{
public:
    //------------------------------------ Constructor ------------------------------------//

    /// init flash with CS on SS pin
    SPImem();

    /// init flash on cs pin
    SPImem(uint8_t cs, SPIClass *spi);
    //------------------------------- Public functions -----------------------------------//

    /// Erase all data on flash memory
    void ChipErase(void);

    /// Erase data in Sector area (512 bytes)
    /// addr_in_sector - any memory address inside sector area
    void SectorErase(uint32_t addr_in_sector);
    uint8_t ReadSR();
    uint8_t ReadByte(uint32_t addr);
    void ReadBytes(uint32_t addr, byte *buf, int len);

    void WriteByte(uint32_t addr, byte DATA);
    void WriteBytes(uint32_t addr, byte *buf, int len);

    /// wait to finish all process like erase/write...
    void NotBusyWait(void);
    bool isBusy(void);
    //------------------------------- Public variables -----------------------------------//
private:
    uint8_t _DataSendReceive(uint8_t DATA);
    void _SendCommand(uint8_t com);
    void _AddressSend(uint32_t addr);
    uint8_t _cs;
    SPIClass *_spi;
};

#endif