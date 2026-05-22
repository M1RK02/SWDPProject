/*
 * SPI_NAND.h
 *
 *  Created on: Feb 16, 2024
 *      Author: alice
 *      Library to define all parameters and functions for SPI NAND MT29F4G01ABAFDWB
 */

#ifndef INC_SPI_NAND_H_
#define INC_SPI_NAND_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define SPI_NAND_BLOCKS_PER_PLANE 2048
#define SPI_NAND_PAGES_PER_BLOCK 64
#define SPI_NAND_PAGE_SIZE 4096
#define SPI_NAND_SPARE_SIZE 256
#define SPI_NAND_CACHE_SIZE 4096 /*\define cache register size */
#define SPI_NAND_DATA_SIZE 4096 /*\define data register size */

#define SPI_NAND_MAX_PAGE_ADDRESS  (SPI_NAND_PAGES_PER_BLOCK - 1)
#define SPI_NAND_MAX_BLOCK_ADDRESS (SPI_NAND_BLOCKS_PER_PLANE- 1)

#define ECC_STATUS_NO_ERR         0b000 /* Table 12 ECC status register */
#define ECC_STATUS_1_3_NO_REFRESH 0b001
#define ECC_STATUS_4_6_REFRESH    0b011
#define ECC_STATUS_7_8_REFRESH    0b101
#define ECC_STATUS_NOT_CORRECTED  0b010

#define RESET_DELAY 1250 /* tRST max 1.25 ms */
#define MFR_ID_MICRON 0x2C
#define DEVICE_ID_4G_3V3 0x34
#define BAD_BLOCK_MARK 0

#define CMD_RESET                    0xFF
#define CMD_READ_ID                  0x9F
#define CMD_SET_FEATURE              0x1F
#define CMD_GET_FEATURE              0x0F
#define CMD_PAGE_READ                0x13
#define CMD_READ_FROM_CACHE          0x03
#define CMD_WRITE_ENABLE             0x06
#define CMD_PROGRAM_LOAD             0x02
#define CMD_PROGRAM_LOAD_RANDOM_DATA 0x84
#define CMD_PROGRAM_EXECUTE          0x10
#define CMD_BLOCK_ERASE              0xD8

#define FEATURE_REG_STATUS        0xC0
#define FEATURE_REG_BLOCK_LOCK    0xA0
#define FEATURE_REG_CONFIGURATION 0xB0
#define FEATURE_REG_DIE_SELECT    0xD0

typedef uint16_t column_address_t; //from 0 to 4351

typedef union {
    uint32_t whole;
    struct {
        /// valid range 0-63
        uint32_t page : 6;
        /// valid range 0-2047
        uint32_t block : 26;
    };
} row_address_t;

typedef union {
    uint32_t whole;
    struct {
    	/// valid range 0-63
		uint32_t page : 6;
    	/// valid range 0-2047
    	uint32_t block : 11;
    	/// dummy bits
    	uint32_t dummy : 7;
    };
} read_address_t;

typedef union {
    uint32_t whole;
    struct {
        uint32_t dummy2 : 8;
        uint32_t address : 13;
        uint32_t dummy1 : 3;
    };
} cache_address_t;

/**
 *\brief spi nand return statuses
 */
enum {
    SPI_NAND_RET_OK = 0,
    SPI_NAND_RET_BAD_SPI = -1,
    SPI_NAND_RET_TIMEOUT = -2,
    SPI_NAND_RET_DEVICE_ID = -3,
    SPI_NAND_RET_BAD_ADDRESS = -4,
    SPI_NAND_RET_INVALID_LEN = -5,
    SPI_NAND_RET_ECC_REFRESH = -6,
    SPI_NAND_RET_ECC_ERR = -7,
    SPI_NAND_RET_P_FAIL = -8,
    SPI_NAND_RET_E_FAIL = -9,
};

typedef struct {
        uint8_t OIP : 1;
        uint8_t WEL : 1;
        uint8_t E_FAIL : 1;
        uint8_t P_FAIL : 1;
        uint8_t ECCS0_3 : 3;
        uint8_t CRBSY : 1;
} feature_reg_status_t;

typedef union {
    uint8_t whole;
    struct {
        uint8_t : 6;
        uint8_t DS0 : 1;
        uint8_t : 1;
    };
} feature_reg_die_select_t;

typedef union {
    uint8_t whole;
    struct {
        uint8_t CONTI_RD : 1;
        uint8_t CFG0 : 1;
        uint8_t DS_S0_1: 2;
        uint8_t ECC_EN : 1;
        uint8_t LOT_EN : 1;
        uint8_t CFG1 : 1;
        uint8_t CFG2 : 1;
    };
} feature_reg_configuration_t;

typedef union {
    uint8_t whole;
    struct {
        uint8_t : 1;
        uint8_t WP_HOLD_DISABLE : 1;
        uint8_t TB : 1;
        uint8_t BP0 : 1;
        uint8_t BP1 : 1;
        uint8_t BP2 : 1;
        uint8_t BP3 : 1;
        uint8_t BRWD : 1;
    };
} feature_reg_block_lock_t;

int spi_nand_init(void);
int spi_nand_page_read(read_address_t row, column_address_t column, uint8_t *data_out, size_t read_len);

int spi_nand_page_program(read_address_t row, column_address_t column, const uint8_t *data_in, size_t write_len);
int spi_nand_page_copy(row_address_t src, row_address_t dest);
int spi_nand_block_erase(read_address_t row);
int spi_nand_block_is_bad(read_address_t row, bool *is_bad);
int spi_nand_block_mark_bad(row_address_t row);
int spi_nand_page_is_free(row_address_t row, bool *is_free);
int spi_nand_clear(void);

int reset(void);
int read_id(void);
int set_feature(uint8_t reg, uint8_t data, uint32_t timeout);
int get_feature(uint8_t reg,  uint32_t timeout);
int write_enable(uint32_t timeout);
int page_read(read_address_t row, uint32_t timeout);
int read_from_cache(column_address_t column, uint8_t *data_out, size_t read_len, uint32_t timeout);
int program_load(column_address_t column, const uint8_t *data_in, size_t write_len, uint32_t timeout);
int program_load_random_data(column_address_t column, uint8_t *data_in, size_t write_len, uint32_t timeout);
int program_execute(read_address_t row, uint32_t timeout);
int block_erase(read_address_t row, uint32_t timeout);
void write_memory();
void read_memory_and_transmit();
void erase_memory();

void cs_select(void);
void cs_deselect(void);
int spi_write_read(const uint8_t *write_buff, uint8_t *read_buff, size_t transfer_len, uint32_t timeout_ms);
int spi_read(uint8_t *read_buff, size_t read_len, uint32_t timeout_ms);
int spi_write(uint8_t *write_buff, size_t write_len, uint32_t timeout_ms);
int unlock_all_blocks(void);
int enable_ecc(void);
bool validate_row_address(read_address_t row);
bool validate_column_address(column_address_t address);
feature_reg_status_t poll_for_oip_clear( uint32_t timeout);
int get_ret_from_ecc_status(feature_reg_status_t status);


#endif /* INC_SPI_NAND_H_ */
