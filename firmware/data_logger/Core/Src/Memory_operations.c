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

void write_packet(uint16_t sample, Time_Struct timestamp, uint8_t *gyroscope, uint8_t *accelerometer, uint8_t *NAND_packet){

	NAND_packet[0 + (sample * BYTES_PER_SAMPLE)] = timestamp.hh;
	NAND_packet[1 + (sample * BYTES_PER_SAMPLE)] = timestamp.mm;
	NAND_packet[2 + (sample * BYTES_PER_SAMPLE)] = timestamp.ss;

	uint16_t milli = timestamp.sss;
	uint8_t m[2];
	m[0] = milli & 0xff;
	m[1] = milli >> 8;

	NAND_packet[3 + (sample * BYTES_PER_SAMPLE)] = m[0];
	NAND_packet[4 + (sample * BYTES_PER_SAMPLE)] = m[1];

	NAND_packet[5 + (sample * BYTES_PER_SAMPLE)] = accelerometer[0];
	NAND_packet[6 + (sample * BYTES_PER_SAMPLE)] = accelerometer[1];
	NAND_packet[7 + (sample * BYTES_PER_SAMPLE)] = accelerometer[2];
	NAND_packet[8 + (sample * BYTES_PER_SAMPLE)] = accelerometer[3];
	NAND_packet[9 + (sample * BYTES_PER_SAMPLE)] = accelerometer[4];
	NAND_packet[10 + (sample * BYTES_PER_SAMPLE)] = accelerometer[5];

	NAND_packet[11 + (sample * BYTES_PER_SAMPLE)] = gyroscope[0];
	NAND_packet[12 + (sample * BYTES_PER_SAMPLE)] = gyroscope[1];
	NAND_packet[13 + (sample * BYTES_PER_SAMPLE)] = gyroscope[2];
	NAND_packet[14 + (sample * BYTES_PER_SAMPLE)] = gyroscope[3];
	NAND_packet[15 + (sample * BYTES_PER_SAMPLE)] = gyroscope[4];
	NAND_packet[16 + (sample * BYTES_PER_SAMPLE)] = gyroscope[5];

}





