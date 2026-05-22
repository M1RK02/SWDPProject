/*
 * SPI_NAND.c
 *
 *   * \brief File.c for SPI NAND MT29F4G01ABAFDWB.
 *
	* Each block of the serial NAND Flash device is divided into 64 programmable pages, each page
	* consisting of 4352 bytes. Each page is further divided into a 4096-byte data storage region and a
	* 256-byte spare area. The spare area is typically used for memory and error management functions.
	* With internal ECC enabled as the default after power-on, ECC code is generated internally when a page
	* is written to the memory core. The ECC code is stored in the spare area of each page

   * SPI2 settings
   * Mode: Full-Duplex master
   * Data Size: 8 Bits
   * First Bit: MSB first
   * Prescaler: 8
   * Baude Rate: 2.0 MBits/s
   * CPOL: Low
   * CPHA: 1 Edge
   *
 */

#include "string.h"
#include "stdio.h"
#include "stdbool.h"
#include "main.h"
#include "SPI.h"
#include "SPI_NAND.h"
#include "Memory_operations.h"
#include "../../USB_Device/App/usb_device.h"

//extern SPI_HandleTypeDef hspi3;
extern SPI_HandleTypeDef hspi2;

// extern variables memory
extern uint8_t NAND_packet[4096];
extern uint16_t sample;
extern uint16_t blocco_scritto;
extern uint8_t pagina_scritta;
extern uint16_t b;
extern uint16_t bad_blocks[2048];
extern read_address_t blocco;
extern column_address_t colonna;
extern uint8_t bad_blocks2[2048];
extern uint8_t data_letto[4096];
extern int exit_flag;

static AppState current_state;

// SPI basic functions
void cs_deselect(void);
void cs_select(void);

// SPI NAND basic operations
bool validate_row_address(read_address_t row);
bool validate_column_address(column_address_t address);
int reset(void);
int set_feature(uint8_t reg, uint8_t data, uint32_t timeout);
int get_feature(uint8_t reg, uint32_t timeout);
int unlock_all_blocks(void);
int enable_ecc(void);
int write_enable(uint32_t timeout);
int page_read(read_address_t row, uint32_t timeout);
int read_from_cache(column_address_t column, uint8_t *data_out, size_t read_len, uint32_t timeout);
int program_load(column_address_t column, const uint8_t *data_in, size_t write_len, uint32_t timeout);
int program_load_random_data(column_address_t column, uint8_t *data_in, size_t write_len, uint32_t timeout);
int program_execute(read_address_t row, uint32_t timeout);
int read_id(void);
int block_erase(read_address_t row, uint32_t timeout);
int spi_nand_block_erase(read_address_t row);
feature_reg_status_t poll_for_oip_clear( uint32_t timeout);
int get_ret_from_ecc_status(feature_reg_status_t status);
int spi_nand_page_program2(read_address_t row, column_address_t column, const uint8_t *data_in,
                          size_t write_len);
void spi_nand_page_program_chunk(read_address_t row, column_address_t column,const uint8_t *data_in,size_t chunk );

int spi_nand_init(void)
{
	/** \brief SPI initialization:
	 * Reset timing 1.25 ms for 3.3V
	 * Read ID
	 * Unlock blocks
	 * Enable ECC
	 */
    cs_deselect();
    HAL_Delay(2);
    int ret = reset();
    if (SPI_NAND_RET_OK != ret) return ret;
    HAL_Delay(2);
    ret = read_id();
    if (SPI_NAND_RET_OK != ret) return ret;
    ret = unlock_all_blocks();
    get_feature(FEATURE_REG_BLOCK_LOCK, 1000);

    if (SPI_NAND_RET_OK != ret) return ret;
    ret = enable_ecc();
    get_feature(FEATURE_REG_CONFIGURATION, 1000);
    if (SPI_NAND_RET_OK != ret) return ret;
    return ret;
}

int spi_nand_feature_page(void){

	/** \brief SPI read feature page:
	*	The following flow must be issued by the memory controller to access the parameter page
	*	contained within Micron SPI devices. The parameter information has size of 256 bytes.
	*	You can read this page to check the read operation functions.
	 */
	feature_reg_configuration_t data;
	data.CFG0=0;
	data.CFG1=1;
	data.CFG2=0;
	get_feature(FEATURE_REG_CONFIGURATION, 1000);
	int ret=set_feature(FEATURE_REG_CONFIGURATION, data.whole, SPI_TIMEOUT);
	get_feature(FEATURE_REG_CONFIGURATION, 1000);
	if (SPI_NAND_RET_OK != ret) return SPI_NAND_RET_BAD_SPI;

	// page read
	read_address_t row;
	row.whole=0x01;
	ret = page_read(row, SPI_TIMEOUT);
	if (SPI_NAND_RET_OK != ret) return ret;

	// read form cache
	column_address_t column =0x00;
	uint8_t parameters[2048]={0};
	//uint8_t parameters[4096]={0};
	ret=read_from_cache(column, parameters, sizeof(parameters), SPI_TIMEOUT);

	feature_reg_configuration_t data_2 = {.whole = 0};
	ret=set_feature(FEATURE_REG_CONFIGURATION, data_2.whole, SPI_TIMEOUT);
	if (SPI_NAND_RET_OK != ret) return SPI_NAND_RET_BAD_SPI;

	return ret;

}


int reset(void)
{	/** \brief Reset operation
	 * Reset sequence time tRST = 2ms
	 * There is the PoR at the power-up of the device as well
	 */

    uint8_t tx_data = CMD_RESET; // this is just a one-byte command

    cs_select();
    int ret = spi_write(&tx_data, 1, SPI_TIMEOUT); // write command
    cs_deselect();

    if(ret != SPI_NAND_RET_OK){
		return SPI_NAND_RET_BAD_SPI;
	}

    /*feature_reg_status_t status;
    return poll_for_oip_clear(&status, SPI_TIMEOUT);*/
    return ret;
}

int block_erase(read_address_t row, uint32_t timeout)
{
	/** \brief block erase
	 * This function sets all the columns in a block to FFh.
	 */
    // setup data for block erase command (need to go from LSB -> MSB first on address)
	uint8_t tx_data[4];
	tx_data[0] = CMD_BLOCK_ERASE;
	tx_data[1] = (row.whole >> 16) & 0xFF;
	tx_data[2] = (row.whole >> 8) & 0xFF;
	tx_data[3] = row.whole & 0xFF;

    // perform transaction
    cs_select();
    int ret = spi_write(tx_data, 4, timeout);
    cs_deselect();
    if (SPI_NAND_RET_OK != ret) return SPI_NAND_RET_BAD_SPI;
    //HAL_Delay(2);

    // wait until that operation finishes
    feature_reg_status_t status;
    status = poll_for_oip_clear(SPI_TIMEOUT);

    if (status.E_FAIL) { // otherwise, check for E_FAIL
        return SPI_NAND_RET_E_FAIL;
    }
    else {
        return SPI_NAND_RET_OK;
    }
}

int read_id(void){
	/** \brief Read ID of the SPI NAND flash memory
	 * READ ID reads the 2-byte identifier code programmed into the device, which includes ID and device configuration data
	 * byte 0: Manufacturer ID (Micron) 2Ch
	 * byte 1: 4Gb 3.3V Device ID 34h
	 * tx_data should be command 9Fh
	 * rx_data should be 2-bytes.
	 */
	uint8_t tx_data[2] = {0};
	uint8_t rx_data[2] = {0};
	tx_data[0] = CMD_READ_ID;

	cs_select();
	HAL_SPI_Transmit(&hspi2, tx_data, 2, SPI_TIMEOUT);
	HAL_SPI_Receive(&hspi2, rx_data, 2, SPI_TIMEOUT);
	cs_deselect();

	//if(ret == SPI_NAND_RET_OK){
		/** check manufacturer & device id */
		if ((MFR_ID_MICRON == rx_data[0]) &&
			(DEVICE_ID_4G_3V3 == rx_data[1])) {
			return SPI_NAND_RET_OK;// success
		}
		else {
			return SPI_NAND_RET_DEVICE_ID; // bad manufacturer or device id
		}
	//}
	//else {
		//return SPI_NAND_RET_BAD_SPI;
	//}
}

int unlock_all_blocks(void)
{
	/** \brief unlock blocks command
	 * By issuing this, you will enable the possibility to write all blocks. Do this if you get P_fail error.
	 */

    feature_reg_block_lock_t unlock_all = {.whole = 0};
    return set_feature(FEATURE_REG_BLOCK_LOCK, unlock_all.whole, SPI_TIMEOUT);
}

int enable_ecc(void)
{
	/** \brief enable ecc
	 * By issuing this, you will enable the ECC.
	 */

    feature_reg_configuration_t ecc_enable = {.whole = 0}; // we want to zero the other bits here
    ecc_enable.ECC_EN = 1;
    return set_feature(FEATURE_REG_CONFIGURATION, ecc_enable.whole, SPI_TIMEOUT);
}


int spi_nand_block_is_bad(read_address_t row, bool *is_bad)
{
	/** BLOCK IS BAD
	 * This function identifies bad blocks setting the variable is_bat to true or false
	 * */
    uint8_t bad_block_mark[1];

    // page read
    int ret = page_read(row, SPI_TIMEOUT);
	if (SPI_NAND_RET_OK != ret) return ret;

	column_address_t column = SPI_NAND_PAGE_SIZE;
	ret=read_from_cache(column, bad_block_mark, sizeof(bad_block_mark), SPI_TIMEOUT);
	if (SPI_NAND_RET_OK != ret) return SPI_NAND_RET_BAD_SPI;

    // check marker
    if (BAD_BLOCK_MARK == bad_block_mark[0]) {
        *is_bad = true;
    }
    else {
        *is_bad = false;
    }

    return SPI_NAND_RET_OK;
}

int spi_nand_block_erase(read_address_t row)
{
	/** \brief ERASE OPERATION
	 * This function issue the erase operation on a block.
	 * */
    row.page = 0; // make sure page address is zero
    // input validation
    if (!validate_row_address(row)) {
        return SPI_NAND_RET_BAD_ADDRESS;
    }

    // write enable
    int ret = write_enable(SPI_TIMEOUT); // ignore the time elapsed since start since its negligible
    if (SPI_NAND_RET_OK != ret) return ret;

    // block erase
    return block_erase(row, SPI_TIMEOUT);
}

int spi_nand_page_read(read_address_t row, column_address_t column, uint8_t *data_out,
                       size_t read_len)
{
	/** \brief READ PAGE OPERATION SEQUENCE
	 * This function check the address is valid and operate page read and read from cache operations
	 * row: contains the page and the number
	 * column: contains the position inside the page i want to read
	 * data_out: is the result contained inside the row-column
	 * read_len: is the number of bytes i want to read
	 * */
    if (!validate_row_address(row) || !validate_column_address(column)) {
        return SPI_NAND_RET_BAD_ADDRESS;
    }
    uint16_t max_read_len = (SPI_NAND_PAGE_SIZE + SPI_NAND_SPARE_SIZE) - column;
    if (read_len > max_read_len) return SPI_NAND_RET_INVALID_LEN;

    page_read(row, SPI_TIMEOUT);  // read page into flash's internal cache
    //if (SPI_NAND_RET_OK != ret) return ret;

    return read_from_cache(column, data_out, read_len, SPI_TIMEOUT);  // read from cache
}

int spi_write_read(const uint8_t *write_buff, uint8_t *read_buff, size_t transfer_len,
                   uint32_t timeout_ms)
{
	/** \brief write and read SPI
	 * write_buff: command, address and data
	 * read_buff: received data from SPI
	 */
	//spi_error_hal =HAL_SPI_Transmit(&hspi2, write_buff, transfer_len, timeout_ms);
	//spi_error_hal=HAL_SPI_Receive(&hspi2, read_buff, transfer_len, timeout_ms);
	HAL_SPI_TransmitReceive(&hspi2, write_buff, read_buff, transfer_len, timeout_ms);
	return SPI_NAND_RET_OK;
}

void spi_nand_page_program_chunk(read_address_t row, column_address_t column,const uint8_t *data_in,size_t chunk ){

	spi_nand_page_program(row, column, data_in, chunk);
	column = chunk;
	spi_nand_page_program(row, column, data_in+column, chunk);
	column = column+chunk;
	spi_nand_page_program(row, column, data_in+column, chunk);
	column = column+chunk;;
	spi_nand_page_program(row, column, data_in+column, chunk);
}

int spi_nand_page_program(read_address_t row, column_address_t column, const uint8_t *data_in,
                          size_t write_len)
{
	/** \brief PAGE PROGRAM
	 * Program load and program execute, to load data to cache and then to the memory itself
	 * */
    // input validation

    if (!validate_row_address(row) || !validate_column_address(column)) {
        return SPI_NAND_RET_BAD_ADDRESS;
    }
    uint16_t max_write_len = (SPI_NAND_PAGE_SIZE + SPI_NAND_SPARE_SIZE) - column;
    if (write_len > max_write_len) return SPI_NAND_RET_INVALID_LEN;

    // write enable
    int ret = write_enable(SPI_TIMEOUT);
    if (SPI_NAND_RET_OK != ret) return ret;

    ret = program_load(column, data_in, write_len, SPI_TIMEOUT);
    //if (SPI_NAND_RET_OK != ret) return ret;


    return program_execute(row, SPI_TIMEOUT);
}



int page_read(read_address_t row, uint32_t timeout)
{
	/** \brief Page read operation
	 * time page read with ecc enabled: 80 us
	 * The PAGE READ (13h) command transfers data from the NAND Flash array to the cache register. It
	 * requires a 24-bit address consisting of 7 dummy bits and a 17-bit block/page address. After the
	 * block/page address is registered, the device starts the transfer from the main array to the cache
	 * register. During this data transfer busy time of tRD, the GET FEATURE command can be issued to
	 * monitor the operation.
	 * Following successful completion of PAGE READ, the READ FROM CACHE command must be issued
	 * to read data out of cache.
	 */
    // setup data for page read command (need to go from LSB -> MSB first on address)
	uint8_t tx_data[4];
	tx_data[0] = CMD_PAGE_READ;
	tx_data[1] = (row.whole >> 16) & 0xFF;
	tx_data[2] = (row.whole >> 8) & 0xFF;
	tx_data[3] = row.whole & 0xFF;
	cs_select();
	int ret = spi_write(tx_data, 4, SPI_TIMEOUT);
	cs_deselect();
	if (SPI_NAND_RET_OK != ret) return SPI_NAND_RET_BAD_SPI;

	feature_reg_status_t status;
	status = poll_for_oip_clear(SPI_TIMEOUT);
	if (SPI_NAND_RET_OK != ret) return ret;
    // check ecc
    return get_ret_from_ecc_status(status);

}

int read_from_cache(column_address_t column, uint8_t *data_out, size_t read_len,
                           uint32_t timeout)
{
	/** \brief Read from cache operation
	 * The READ FROM CACHE ×1 command facilitates the sequential reading of
	 * one or more data bytes from the cache buffer. Initiating the command
	 * involves driving CS# LOW, shifting in the command opcode 03h/0Bh,
	 * followed by a 16-bit column address and 8 dummy clocks.
	 * Both commands operate in fast mode. Data is retrieved from the addressed
	 * cache buffer, with the Most Significant Bit (MSB) first, on SO at the
	 * falling edge of SCK. The address automatically increments to the next
	 * higher address after each byte of data is shifted out, allowing for a
	 * continuous stream of data. The command concludes by driving CS# HIGH.
	 * */
    // setup data for read from cache command (need to go from LSB -> MSB first on address)
    uint8_t tx_data[4];
    tx_data[0] = CMD_READ_FROM_CACHE;
    cache_address_t data;
	data.dummy1 = 0;   // 3 dummy bits
	data.address = column; // 13 address bits
	data.dummy2 = 0;   // 8 dummy bits

	tx_data[1] = (data.whole >> 16) & 0xFF;
	tx_data[2] = (data.whole >> 8) & 0xFF;
	tx_data[3] = data.whole & 0xFF;

    // perform transaction
    cs_select();
    int ret = spi_write(tx_data, 4, timeout);

    if (SPI_NAND_RET_OK == ret) {

        ret = spi_read(data_out, read_len, timeout);

    }
    cs_deselect();

    return (SPI_NAND_RET_OK == ret) ? SPI_NAND_RET_OK : SPI_NAND_RET_BAD_SPI;
}

int write_enable(uint32_t timeout)
{
	/** \brief Write enable command
	 * Command 06h, it sets the WEL bit to 1
	 * In order to be written, it need: page program, otp area program, block earse.
	 * */
    uint8_t cmd = CMD_WRITE_ENABLE;

    cs_select();
    int ret = spi_write(&cmd, sizeof(cmd), timeout);
    cs_deselect();

    if(ret == SPI_NAND_RET_OK){
		return SPI_NAND_RET_OK;
	}
	else{
		return SPI_NAND_RET_BAD_SPI;
	}
}


int program_load(column_address_t column, const uint8_t *data_in, size_t write_len,
                        uint32_t timeout)
{
	// \brief Program load operation
	 //With this function you load the data you want to store in the memory into the cache register. Remember, to complete a write operation you must issue a program_execute command.
	//  The data in the cache will be first reset and then written.

    // setup data for program load (need to go from LSB -> MSB first on address)

	uint8_t tx_data[3];
	tx_data[0] = CMD_PROGRAM_LOAD;
	cache_address_t data;
	data.dummy1 = 0;   // 3 dummy bits
	data.address = column; // 13 address bits
	data.dummy2 = 0;   // 8 dummy bits
	tx_data[1] = (data.whole >> 16) & 0xFF;
	tx_data[2] = (data.whole >> 8) & 0xFF;

    // perform transaction
    cs_select();

    int ret = spi_write(tx_data, 3, timeout);
    if (SPI_NAND_RET_OK == ret) {
        ret = spi_write(data_in, write_len, timeout);
    }

    cs_deselect();

    return (SPI_NAND_RET_OK == ret) ? SPI_NAND_RET_OK : SPI_NAND_RET_BAD_SPI;
}




int program_load_random_data(column_address_t column, uint8_t *data_in, size_t write_len,
                                    uint32_t timeout)
{
	/* \brief load random data
	 * With this you will only overwrite the information that you want to change, without a reset of the cache before (see program_load operation).
	 *
	 */
    // setup data for program load (need to go from LSB -> MSB first on address)
	uint8_t tx_data[3];
	tx_data[0] = CMD_PROGRAM_LOAD_RANDOM_DATA;
	cache_address_t data;
	data.dummy1 = 0;   // 3 dummy bits
	data.address = column; // 13 address bits
	data.dummy2 = 0;   // 8 dummy bits
	tx_data[1] = (data.whole >> 16) & 0xFF;
	tx_data[2] = (data.whole >> 8) & 0xFF;

    // perform transaction
    cs_select();
    int ret = spi_write(tx_data, 3, timeout);
    if (SPI_NAND_RET_OK == ret) {
        ret = spi_write(data_in, write_len, timeout);
    }
    cs_deselect();


    return (SPI_NAND_RET_OK == ret) ? SPI_NAND_RET_OK : SPI_NAND_RET_BAD_SPI;
}

int program_execute(read_address_t row, uint32_t timeout)
{
	/* \brief program execute
	 * With this function you will transfer data from the cache register to the memory.
	 *
	 */
    // setup data for program execute (need to go from LSB -> MSB first on address)
	uint8_t tx_data[4];
	tx_data[0] = CMD_PROGRAM_EXECUTE;
	tx_data[1] = (row.whole >> 16) & 0xFF;
	tx_data[2] = (row.whole >> 8) & 0xFF;
	tx_data[3] = row.whole & 0xFF;

    // perform transaction
    cs_select();
    int ret = spi_write(tx_data, 4, timeout);
    cs_deselect();
    if (SPI_NAND_RET_OK != ret) return SPI_NAND_RET_BAD_SPI;

    feature_reg_status_t status;
    status = poll_for_oip_clear(timeout);

    if (SPI_NAND_RET_OK != ret) { // if polling failed, return that status
        return ret;
    }
    else if (status.P_FAIL) { // otherwise, check for P_FAIL
        return SPI_NAND_RET_P_FAIL;
    }
    else {
        return SPI_NAND_RET_OK;
    }
}

int set_feature(uint8_t reg, uint8_t data, uint32_t timeout)
{
	/** \brief Set feature command operation
	 * The set_feature function is designed to alter the device configuration
	 * from its default state at power-on by utilizing the SET FEATURE (1Fh)
	 * command. The function uses a 1-byte feature address to identify which
	 * feature is to be modified. Features that can be managed using the
	 * SET FEATURE command include OTP protection, block locking,
	 * SPI NOR-like protocol configuration, and ECC correction.
	 * Once a feature is set using this function, it remains active until
	 * the device is power-cycled or the feature is explicitly written to.
	 * Even if a RESET (FFh) command is issued, the set feature remains in
	 * effect unless specified otherwise.
	 *
	 * reg: C0h status register
	 * data: information i want to write into the register
	 * timeout: spi timeout
	 * */
    uint8_t tx_data[3] = {0};
    tx_data[0] = CMD_SET_FEATURE;
    tx_data[1] = reg;
    tx_data[2] = data;

    cs_select();
    int ret = spi_write(tx_data, 3, timeout);
    cs_deselect();

    if(ret == SPI_NAND_RET_OK){
    	return SPI_NAND_RET_OK;
    }
    else{
    	return SPI_NAND_RET_BAD_SPI;
    }
}

int get_feature(uint8_t reg, uint32_t timeout)
{
	/** \brief get feature command operation
	 * The get_feature function is designed to monitor the device status
	 * by utilizing the GET FEATURE (0Fh) command. The function uses a 1-byte
	 * feature address to identify which feature is to be read.
	 * Features that can be monitored using the GET FEATURE command include
	 * OTP protection, block locking, SPI NOR-like protocol configuration,
	 * and ECC correction.
	 *
	 * reg: C0h status register
	 * data_out: information read from the register
	 * timeout: spi timeout
	 * */
    uint8_t tx_data[3] = {0};
    uint8_t rx_data[3] = {0};
    tx_data[0] = CMD_GET_FEATURE;
    tx_data[1] = reg;
    uint8_t data_out=0;
    cs_select();
    int ret = spi_write_read(tx_data, rx_data, 3, timeout);
    cs_deselect();
    data_out = rx_data[2];
    if(ret == SPI_NAND_RET_OK){
		return data_out;
	}
	else{
		return SPI_NAND_RET_BAD_SPI;
	}
}

feature_reg_status_t poll_for_oip_clear(uint32_t timeout)
{
	/** \brief polling for OIP bit to be clear
	 *
	 *  */

	uint8_t data=0;
	feature_reg_status_t status_out;
    for (;;) { // INFINITE CYCLE
    	 //int ret = get_feature(FEATURE_REG_STATUS, &status_out->whole, timeout);
    	data = get_feature(FEATURE_REG_STATUS, timeout);

    	status_out.OIP = data & 0b00000001;
    	status_out.WEL = (data & 0b00000010)>>1;
		status_out.E_FAIL = (data & 0b00000100)>>2;
		status_out.P_FAIL = (data & 0b00001000)>>3;
		status_out.ECCS0_3 = (data & 0b01110000)>>4;
		status_out.CRBSY = 	(data & 0b10000000)>>7;
    	 //if (SPI_NAND_RET_OK != ret) {
           // return ret;
        //}
        // check for OIP clear
        if (0 == status_out.OIP) {
            return status_out;
        }
    }
}




int get_ret_from_ecc_status(feature_reg_status_t status)
{
	/** \brief Reading the status it returns different errors on ecc
	 * */
    int ret;
    // map ECC status to return type
    switch (status.ECCS0_3) {
        case ECC_STATUS_NO_ERR:
        	ret = SPI_NAND_RET_OK;
			break;
        case ECC_STATUS_1_3_NO_REFRESH:
            ret = SPI_NAND_RET_OK;
            break;
        case ECC_STATUS_4_6_REFRESH:
        	ret = SPI_NAND_RET_OK;
			break;
        case ECC_STATUS_7_8_REFRESH:
            ret = SPI_NAND_RET_ECC_REFRESH;
            break;
        case ECC_STATUS_NOT_CORRECTED:
            ret = SPI_NAND_RET_ECC_ERR;
            break;
    }
    return ret;
}

void cs_select(void){
	/** \brief Put chip select low to enable SPI communication with NAND */
	HAL_GPIO_WritePin(SPI3_CS_NAND_GPIO_Port, SPI3_CS_NAND_Pin, 0);
}

void cs_deselect(void){
	/** \brief Put chip select high to disable SPI communication with NAND */
	HAL_GPIO_WritePin(SPI3_CS_NAND_GPIO_Port, SPI3_CS_NAND_Pin, 1);
}

bool validate_row_address(read_address_t row)
{
	/** \brief Ensure that the address block - page is valid */
    if ((row.block > SPI_NAND_MAX_BLOCK_ADDRESS) || (row.page > SPI_NAND_SPARE_SIZE)) {
        return false;
    }
    else {
        return true;
    }
}

bool validate_column_address(column_address_t col)
{
	/** \brief Ensure that the address inside the page is valid */
    if (col >= (SPI_NAND_PAGE_SIZE + SPI_NAND_SPARE_SIZE)) {
        return false;
    }
    else {
        return true;
    }
}

int spi_read(uint8_t *read_buff, size_t read_len, uint32_t timeout_ms)
{
	HAL_SPI_Receive(&hspi2, read_buff, read_len, timeout_ms);
    return SPI_NAND_RET_OK;
}

int spi_write(uint8_t *write_buff, size_t write_len, uint32_t timeout_ms)
{
	int r = HAL_SPI_Transmit(&hspi2, write_buff, write_len, timeout_ms);
    return r;
}


void write_memory()
{
	if(sample == SAMPLES_PER_PAGE){ // Arrived at the end of the page

		sample = 0;

		if(pagina_scritta >= 64){ // End of the block, increment block
			pagina_scritta = 0;
			b++;
		}

		if(b==2048){ // memory full
			current_state = STATE_IDLE;
		}

		// write 1 page at the time
		blocco_scritto = bad_blocks[b];
		blocco.block = blocco_scritto;
		blocco.page = pagina_scritta;
		blocco.dummy = 0;
		colonna = 0;

		spi_nand_page_program(blocco, colonna, NAND_packet, 4096);

		pagina_scritta++;

		memset(NAND_packet, 0, sizeof(NAND_packet));
	}

}

void read_memory_and_transmit()
{
		for(int bloc = 0; bloc < 2048; bloc++) { // Cycle on all the memory blocks (2048)
			if(exit_flag == 0){
			blocco.block = bad_blocks[bloc]; // Read only good blocks

		for(int pag = 0; pag < 64; pag++) {// Cycle on all pages in each block (64)
			blocco.page = pag;
			colonna = 0;
			// Save data of the page into data_letto
			spi_nand_page_read(blocco, colonna, data_letto, sizeof(data_letto));

			if(data_letto[0] == 255) {
				// in this case exit condition is if the first element is 255 but can be adapted
				// for example you can store the #block and #page that you have written and read until those #
				current_state = STATE_USB_CONNECTED;

				uint8_t buffer[1] = { 'T' };  // Termination Char compatible with the PC script
				CDC_Transmit_FS(buffer, sizeof(buffer));  // Send 1 byte: 'T'

				exit_flag = 1;
				break; // exit cycle
			}

			CDC_Transmit_FS(data_letto, sizeof(data_letto)); // Send data via USB
			HAL_Delay(10); // wait some time
		}
		}
	}
}

void erase_memory()
{
	erase_good_blocks(bad_blocks2); // Erase bad_blocks (set all memory to 0xFF)
}



