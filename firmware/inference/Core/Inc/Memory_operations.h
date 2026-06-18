/*
 * Memory_operations.h
 *
 *  Created on: Mar 27, 2024
 *      Author: alice
 */

#ifndef INC_MEMORY_OPERATIONS_H_
#define INC_MEMORY_OPERATIONS_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "main.h"
#include "SPI.h"
#include "SPI_NAND.h"
#include "as7341.h"

#define BYTES_PER_SAMPLE 4096 // Full page
#define SAMPLES_PER_PAGE 1 // 4096/BYTES_PER_SAMPLE rounded

typedef struct bookmark
{
  uint16_t blocco_scritto;
  uint8_t pagina_scritta;
  int b;

}NAND_info;
typedef struct Time
      {
          uint8_t hh;
          uint8_t mm;
          uint8_t ss;
          uint16_t sss;
      }Time_Struct;

void find_bad_blocks(uint16_t *bad_blocks);
void erase_good_blocks(uint8_t *bad_blocks);
NAND_info read_memory(int b, NAND_info indice, uint16_t *blocco_letto, uint8_t *pagina_letta, uint16_t bad_blocks[2048], uint8_t *data_letto);
void write_info(NAND_info segnalibro, uint16_t bad_blocks[2048]);
NAND_info read_info(uint16_t bad_blocks[2048]);
void write_packet(uint8_t *NAND_packet, uint16_t packet_index, uint8_t new_data_flag, AS7341_SpectralData_t *spec_data, int16_t *audio_buffer);

#endif /* INC_MEMORY_OPERATIONS_H_ */
