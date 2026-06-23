/*******************************************************************************
 *
 *                  24CWxx Library
 *
 *******************************************************************************
 * FileName:        24CW128x.c
 * Complier:        XC8 v2.20
 * Author:          Pedro Sanchez Ramirez (MrChunckuee)
 * Blog:            http://mrchunckuee.blogspot.com/
 * Email:           mrchunckuee.electronics@gmail.com
 * Description:     Library for 24CW128x use in PIC18
 *******************************************************************************
 *                  MIT License
 * 
 * Copyright (c) 2016 Pedro Sanchez Ramirez
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.         
 ******************************************************************************/

#include "main.h"

/*******************************************************************************
 * Function:        void EEPROMExt_Initialize(void)
 * Description:     Esta función inicializa el puetto I2C utilizado para la EEPROM
 * Precondition:    None
 * Parameters:      None
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
void EEPROMExt_Initialize(void){
    EEPROMExt_WaitReady(); 
}

/*******************************************************************************
 * Function:        static void EEPROMExt_WaitReady(void)
 * Description:     Espera a que la EEPROM termine su ciclo de escritura interno
 * Precondition:    None
 * Parameters:      None
 * Return Values:   None
 * Remarks:         Esta funcion sustituye un retaro fijo.
 *                  Intentamos enviar la dirección con bit de escritura.
 *                  Si el chip responde (ACK = 0), está listo.
 *                  Tiene un timeout para evitar colgarse.
 ******************************************************************************/
static void EEPROMExt_WaitReady(void){
    uint16_t timeout = 200; 
    while(timeout--){
        I2C2_Start();
        if(I2C2_Send(EEPROM_24CW1280_ADDRESS_DEVICE | EEPROM_WRITE_BIT) == 0){
            I2C2_Stop();
            return; 
        }
        I2C2_Stop();
    }
}

/*******************************************************************************
 * Function:        void EEPROMExt_Write(uint16_t address, uint8_t data)
 * Description:     Esta función escribe un byte en la EEPROM
 * Precondition:    None
 * Parameters:      uint16_t address, direccion de memoria
 *                  uint8_t data, dato a guardar en la memoria
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
void EEPROMExt_Write(uint16_t address, uint8_t data){
    EEPROMExt_WaitReady();
    I2C2_Start();
    I2C2_Send(EEPROM_24CW1280_ADDRESS_DEVICE | EEPROM_WRITE_BIT);
    I2C2_Send((uint8_t)(address >> 8));   // MSB
    I2C2_Send((uint8_t)(address & 0xFF)); // LSB
    I2C2_Send(data);
    I2C2_Stop();
}

/*******************************************************************************
 * Function:        void EEPROMExt_writeData(uint16_t address, uint8_t * data, uint16_t len)
 * Description:     Esta función escribe un numero de bytes en la EEPROM
 * Precondition:    None
 * Parameters:      uint16_t address: Direccion donde inicia la escritura
 *                  uint8_t *data: Datos a escribirse
 *                  uint16_t len: Numero de datos que se van a escribir
 * Return Values:   None
 * Remarks:         La funcion divide el buffer en fragmentos que no crucen el 
 *                  limite de tamaño de pagina.
 ******************************************************************************/
void EEPROMExt_WriteData(uint16_t address, uint8_t *data, uint16_t len){
    uint16_t bytesRemaining = len;
    uint16_t currentAddr = address;
    uint8_t *pData = data;

    while (bytesRemaining > 0) {
        // Calcular cuántos bytes podemos escribir en la página actual
        uint16_t bytesInPage = EEPROM_PAGE_SIZE - (currentAddr % EEPROM_PAGE_SIZE);
        uint16_t chunk = (bytesRemaining < bytesInPage) ? bytesRemaining : bytesInPage;

        EEPROMExt_WaitReady();
        I2C2_Start();
        I2C2_Send(EEPROM_24CW1280_ADDRESS_DEVICE | EEPROM_WRITE_BIT);
        I2C2_Send((uint8_t)(currentAddr >> 8));
        I2C2_Send((uint8_t)(currentAddr & 0xFF));

        for (uint16_t i = 0; i < chunk; i++) {
            I2C2_Send(*pData++);
        }
        I2C2_Stop();

        bytesRemaining -= chunk;
        currentAddr += chunk;
    }
}

/*******************************************************************************
 * Function:        uint8_t EEPROMExt_Read(uint16_t address)
 * Description:     Esta función lee un byte en la EEPROM
 * Precondition:    None
 * Parameters:      uint16_t address, direccion de memoria.
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
uint8_t EEPROMExt_Read(uint16_t address){
    uint8_t data = 0;
    EEPROMExt_WaitReady();

    // Dummy Write para establecer la dirección de lectura
    I2C2_Start();
    if(I2C2_Send(EEPROM_24CW1280_ADDRESS_DEVICE | EEPROM_WRITE_BIT) == 0){
        I2C2_Send((uint8_t)(address >> 8));   // Dirección MSB
        I2C2_Send((uint8_t)(address & 0xFF)); // Dirección LSB

        // Cambio a modo lectura
        I2C2_ReStart();
        I2C2_Send(EEPROM_24CW1280_ADDRESS_DEVICE | EEPROM_READ_BIT);
        
        data = I2C2_Read();    // Leer el byte
        I2C2_Send_NACK();      // IMPORTANTE: NACK para indicar que es el único byte
    }
    I2C2_Stop();

    return data;
}


/*******************************************************************************
 * Function:        void EEPROMExt_readData(uint16_t address, uint8_t * data, uint16_t len)
 * Description:     Esta función lee un numero de bytes en la EEPROM
 * Precondition:    None
 * Parameters:      uint16_t address: Direccion donde inicia la lectura
 *                  uint8_t *data: Arreglo en donde se guardaran los datos leidos
 *                  uint16_t len: Numero de datos que se van a leer
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
void EEPROMExt_ReadData(uint16_t address, uint8_t *data, uint16_t len){
    if (len == 0) return;

    EEPROMExt_WaitReady();
    I2C2_Start();
    I2C2_Send(EEPROM_24CW1280_ADDRESS_DEVICE | EEPROM_WRITE_BIT);
    I2C2_Send((uint8_t)(address >> 8));
    I2C2_Send((uint8_t)(address & 0xFF));
    
    I2C2_ReStart();
    I2C2_Send(EEPROM_24CW1280_ADDRESS_DEVICE | EEPROM_READ_BIT);
    
    for(uint16_t i = 0; i < len; i++) {
        data[i] = I2C2_Read();
        // Enviar ACK para todos excepto el último byte
        if (i < (len - 1)) {
            I2C2_Send_ACK();
        } else {
            I2C2_Send_NACK();
        }
    }
    I2C2_Stop();
}

/*******************************************************************************
 * Function:        void EEPROMExt_writeData(uint16_t address, uint8_t * data, uint16_t len)
 * Description:     Esta función escribe un valor en la EEPROM
 * Precondition:    None
 * Parameters:      uint16_t address: Direccion donde inicia la escritura
 *                  uint8_t value: Valor a escribir
 *                  uint16_t len: Numero de veces que se van a escribir
 * Return Values:   None
 * Remarks:         La funcion divide el buffer en fragmentos que no crucen el 
 *                  limite de tamaño de pagina.
 ******************************************************************************/
void EEPROMExt_Fill(uint16_t address, uint8_t value, uint16_t len){
    uint16_t bytesRemaining = len;
    uint16_t currentAddr = address;

    while(bytesRemaining > 0){
        uint16_t bytesInPage = EEPROM_PAGE_SIZE - (currentAddr % EEPROM_PAGE_SIZE);
        uint16_t chunk = (bytesRemaining < bytesInPage) ? bytesRemaining : bytesInPage;

        EEPROMExt_WaitReady();
        I2C2_Start();
        I2C2_Send(EEPROM_24CW1280_ADDRESS_DEVICE | EEPROM_WRITE_BIT);
        I2C2_Send((uint8_t)(currentAddr >> 8));
        I2C2_Send((uint8_t)(currentAddr & 0xFF));
        
        for(uint16_t i = 0; i < chunk; i++){
            I2C2_Send(value); 
        }
        I2C2_Stop();

        bytesRemaining -= chunk;
        currentAddr += chunk;
    }
}

/*******************************************************************************
 * Function:        uint8_t EEPROMExt_Verify(uint16_t address, uint8_t *data, uint16_t len)
 * Description:     Esta función verifica un bloque de datos
 * Precondition:    None
 * Parameters:      uint16_t address, direccion de memoria.
 *                  uint8_t *data, datos a verificar.
 *                  uint16_t len tamaño del bloque
 * Return Values:   0 si son iguales, 1 si hay error.
 * Remarks:         Verifica si un bloque en la EEPROM coincide con un buffer en RAM
 ******************************************************************************/
uint8_t EEPROMExt_Verify(uint16_t address, uint8_t *data, uint16_t len){
    uint8_t tempByte;
    for(uint16_t i = 0; i < len; i++){
        tempByte = EEPROMExt_Read(address + i);
        if (tempByte != data[i]){
            return 1; // Error de verificación
        }
    }
    return 0; // Todo Okay
}

/*******************************************************************************
 * Function:        void EEPROMExt_Update(uint16_t address, uint8_t data)
 * Description:     Esta función actualiza si es diferente.
 * Precondition:    None
 * Parameters:      uint16_t address, direccion de memoria.
 *                  uint8_t data, dato a actualizar
 * Return Values:   None
 * Remarks:         Solo escribe el byte si es diferente al que ya está guardado.
 *                  Ahorra ciclos de escritura y tiempo de CPU.
 ******************************************************************************/
void EEPROMExt_Update(uint16_t address, uint8_t data){
    if(EEPROMExt_Read(address) != data){
        EEPROMExt_Write(address, data);
    }
}

/*
 * Aun pendiente mejorar:
 *  - Si el buffer es muy largo y solo cambia pocos bytes es mejor usarlo asi
 *    por el desgaste de escritura.
 *  - Si el buffer es largo pero cambian muchos valores lo ideal seria:
 *      - Leer buffer y almacenar en RAM
 *      - Compárar el buffer con el nuevo
 *      - Actualizar si son diferentes
 * 
 *  Considerar: Actualmente este metodo es tardado por que lee, compara y escribe 
 *              si son diferentes, el tiempo del proceso dependera del tamaño del buffer.
 */
void EEPROMExt_UpdateBuffer(uint16_t address, uint8_t *data, uint8_t len){
    for (uint8_t i = 0; i < len; i++){
        EEPROMExt_Update(address + i, data[i]);
    }
}
