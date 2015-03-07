#include "hspi.h"

/*
Pinout:
MISO GPIO12
MOSI GPIO13
CLK GPIO14
CS GPIO0
DC GPIO2
*/

#define __min(a,b) ((a > b) ? (b):(a))

static uint32_t *spi_fifo;

void hspi_init(void)
{
	spi_fifo = (uint32_t*)SPI_FLASH_C0(HSPI);

	WRITE_PERI_REG(PERIPHS_IO_MUX, 0x105); //clear bit9

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, 2); // HSPIQ MISO GPIO12
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, 2); // HSPID MOSI GPIO13
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, 2); // CLK GPIO14
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U, 2); // CLK GPIO15


	// SPI clock = CPU clock / 10 / 4
	// time length HIGHT level = (CPU clock / 10 / 2) ^ -1,
	// time length LOW level = (CPU clock / 10 / 2) ^ -1
	WRITE_PERI_REG(SPI_FLASH_CLOCK(HSPI),
	   ((1 & SPI_CLKDIV_PRE) << SPI_CLKDIV_PRE_S) |
	   ((3 & SPI_CLKCNT_N) << SPI_CLKCNT_N_S) |
	   ((1 & SPI_CLKCNT_H) << SPI_CLKCNT_H_S) |
	   ((3 & SPI_CLKCNT_L) << SPI_CLKCNT_L_S));}

#define SPIFIFOSIZE 16 //16 words length

inline void hspi_wait_ready()
{
	while (READ_PERI_REG(SPI_FLASH_CMD(HSPI))&SPI_FLASH_USR);
}

inline void hspi_prepare_tx(uint32_t bytecount)
{
	uint32_t regvalue = SPI_FLASH_DOUT;
    regvalue &= ~(BIT2 | SPI_FLASH_USR_ADDR | SPI_FLASH_USR_DUMMY | SPI_FLASH_USR_DIN | SPI_USR_COMMAND | SPI_DOUTDIN); //clear bit 2 see example IoT_Demo
	WRITE_PERI_REG(SPI_FLASH_USER(HSPI), regvalue);

	uint32_t bitcount = bytecount * 8 - 1;

//	WRITE_PERI_REG(SPI_FLASH_USER1(HSPI), (bitcount & SPI_USR_OUT_BITLEN) << SPI_USR_OUT_BITLEN_S);
	WRITE_PERI_REG(SPI_FLASH_USER1(HSPI),
			( (bitcount & SPI_USR_OUT_BITLEN) << SPI_USR_OUT_BITLEN_S ) |
			( (bitcount & SPI_USR_DIN_BITLEN) << SPI_USR_DIN_BITLEN_S ) );
}

inline void hspi_start_tx()
{
	SET_PERI_REG_MASK(SPI_FLASH_CMD(HSPI), SPI_FLASH_USR);   // send
}

void hspi_send_uint8(uint8_t data)
{
	hspi_wait_ready();
	hspi_prepare_tx(1);
	*spi_fifo = data;
	hspi_start_tx();
	hspi_wait_ready();
}

void hspi_send_uint16(uint16_t data)
{
	hspi_wait_ready();
	hspi_prepare_tx(2);
	*spi_fifo = data;
	hspi_start_tx();
	hspi_wait_ready();
}

void hspi_send_uint32(uint32_t data)
{
	hspi_wait_ready();
	hspi_prepare_tx(4);
	*spi_fifo = data;
	hspi_start_tx();
	hspi_wait_ready();
}

void hspi_send_uint16_r(uint16_t data, uint32_t repeats)
{
	uint32_t i;
	hspi_wait_ready();
	hspi_prepare_tx(2);
	*spi_fifo = data;
	for (i = 0; i < repeats; i++)
	{
		hspi_wait_ready();
		hspi_start_tx();
	}
	hspi_wait_ready();
}

void hspi_send_data(const uint8_t * data, uint8_t datasize)
{
	uint32_t *_data = (uint32_t*)data;
	uint8_t i;

	uint8_t words_to_send = __min((datasize + 3) / 4, SPIFIFOSIZE);
	hspi_wait_ready();
	hspi_prepare_tx(datasize);
	for(i = 0; i < words_to_send;i++)
		spi_fifo[i] = _data[i];
	hspi_start_tx();
	hspi_wait_ready();
}
