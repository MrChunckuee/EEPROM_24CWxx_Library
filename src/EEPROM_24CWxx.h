/* 
 * File:   EEPROM_24CWxx.h
 * Author: MrChunckueee
 *
 * Created on 15 de junio de 2026, 12:37 AM
 */

#ifndef EEPROM_24CWXX_H
#define	EEPROM_24CWXX_H

#ifdef	__cplusplus
extern "C" {
#endif


    #define EEPROM_24CW1280_ADDRESS_DEVICE  0xA0 // 8bits
    
    // Definiciones para 24CW1280
    #define EEPROM_PAGE_SIZE    32      // Tamaño de pagina de la 24CW1280

    #define EEPROM_WRITE_BIT    0x00
    #define EEPROM_READ_BIT     0x01

    void EEPROMExt_Initialize(void);
    static void EEPROMExt_WaitReady(void);
    void EEPROMExt_Write(uint16_t address, uint8_t data); 
    void EEPROMExt_WriteData(uint16_t address,  uint8_t *data, uint16_t len);
    uint8_t EEPROMExt_Read(uint16_t address);
    void EEPROMExt_ReadData(uint16_t address,  uint8_t *data, uint16_t len);
    void EEPROMExt_Fill(uint16_t address, uint8_t value, uint16_t len);
    uint8_t EEPROMExt_Verify(uint16_t address, uint8_t *data, uint16_t len);
    void EEPROMExt_Update(uint16_t address, uint8_t data);
    void EEPROMExt_UpdateBuffer(uint16_t address, uint8_t *data, uint8_t len);


#ifdef	__cplusplus
}
#endif

#endif	/* EEPROM_24CWXX_H */

