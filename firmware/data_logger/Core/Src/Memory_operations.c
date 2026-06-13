/*
 * Memory_operations.c
 *
 *  This file contains the high-level operations that can be used for managing the SPI NAND
 *  You will need the SPI_NAND.c library.
 *
 */

#include "string.h"
#include "stdio.h"
#include "stdbool.h"
#include "main.h"
#include "SPI.h"
#include "SPI_NAND.h"
#include "Memory_operations.h"
#include "as7341.h"

NAND_info data;
//void write_packet(uint8_t sample, RTC_TimeTypeDef sTime, uint8_t *magnetometer, uint8_t *pressure, uint8_t *temp, uint8_t *gyroscope,uint8_t *accelerometer, uint8_t *gyroscope2,uint8_t *accelerometer2,uint8_t *ppg_sample, uint8_t *NAND_packet);

// todo:
/*
 * All'inizio faccio un spi_nand_init(); che non cancella il contenuto della memoria
 * Il flusso di operazioni sarà: inizializzo, leggo tutto, identifico i bad blocks, erase tutto, scrivo tutto
 * Idea: uso il primo blocco disponibile per salvarmi il blocco e la pagina a cui sono arrivata -> può avere dei problemi.
 */

void find_bad_blocks(uint16_t *bad_blocks){
	// inizializzo bad_blocks con -1
	// metto l'indice del good_block nel vettore
	// i++ e prendo il numero scritto dentro nel vettore, non il suo indice quando devo andare a scrivere

	read_address_t blocco;
	blocco.block=0;
	blocco.page=0;
	blocco.dummy=0;
	bool is_bad_mark=true;
	int j = 0;
	for(int i = 0; i<2048; i++){
		blocco.block=i;
		spi_nand_block_is_bad(blocco, &is_bad_mark);
		/*
		if(is_bad_mark){
		  bad_blocks[i]=1;
		}*/
		if(!is_bad_mark) {
		  bad_blocks[j]=i;
		  j++;
		}

	}
}

// Questo da tenere così
void erase_good_blocks(uint8_t *bad_blocks){
	read_address_t blocco;
	blocco.block=0;
	blocco.page=0;
	blocco.dummy=0;
	bool is_bad_mark=true;
	for(int i = 0; i<2048; i++){
		blocco.block=i;
		spi_nand_block_is_bad(blocco, &is_bad_mark);
		if(is_bad_mark){
		  bad_blocks[i]=1;
		}
		if(!is_bad_mark) {
		  bad_blocks[i]=0;
		  spi_nand_block_erase(blocco);
		}
	}
}

void write_packet(Time_Struct timestamp, AS7341_SpectralData_t *spec_data, int16_t *audio_buffer, uint8_t *NAND_packet){
    // 1. Write the Time_Struct (5 bytes)
    NAND_packet[0] = timestamp.hh;
    NAND_packet[1] = timestamp.mm;
    NAND_packet[2] = timestamp.ss;
    NAND_packet[3] = timestamp.sss & 0xFF;         
    NAND_packet[4] = (timestamp.sss >> 8) & 0xFF;  
    
    // 2. Write the Spectrometer Data (20 bytes)
    // Copies f1..f8, clear, nir directly into the packet
    memcpy(&NAND_packet[5], spec_data, sizeof(AS7341_SpectralData_t));
    
    // 3. Add 3 bytes of padding so the audio starts on an even boundary (Byte 28)
    NAND_packet[25] = 0;
    NAND_packet[26] = 0;
    NAND_packet[27] = 0;
    
    // 4. Copy the audio data into the remaining 4068 bytes
    memcpy(&NAND_packet[28], audio_buffer, BYTES_PER_SAMPLE - 28);
}