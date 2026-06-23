# EEPROM 24CW Series Library

FileName:        EEPROM24CW.c and EEPROM24CW.h <br>
Processor:       PICxxxxxx <br>
Complier:        XC8 <br>
Author:          Pedro Sanchez (@mrchunckuee_electronics) <br>
Blog:            http://mrchunckuee.blogspot.com/ <br>
Email:           mrchunckuee.electronics@gmail.com <br>
Description:     Firmware change history, bug fixes, and new version implementation.

## xx/xx/xxxx _vx.x.x_
Author:	Full Name
### Notes
-
### Added
- 
### Changed
- 
### Fixed
- 

<br>
<br>

***

## 20/06/2026 _v0.0.3_
Author:	Pedro Sanchez
### Notes
- Release of the library and its documentation.
### Added
- Add static void EEPROMExt_WaitReady(void)
- Add void EEPROMExt_Fill(uint16_t address, uint8_t value, uint16_t len)
- Add uint8_t EEPROMExt_Verify(uint16_t address, uint8_t *data, uint16_t len)
### Changed
- A page limit was implemented in the writing functions.

<br>
<br>

***

## 02/12/2025 _v0.0.2_
Author:	Pedro Sanchez
### Changed
- The library was renamed.
- The functions for reading and writing n bytes were modified.


<br>
<br>

***

## 24/08/2021 _v0.0.1_
Author:	Pedro Sanchez
### Notes
- Created library
### Added
- Implementation of read and write functions for a specific byte

<br>
<br>
