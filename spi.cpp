//*****************************************************************************
//
// Имя файла    : 'spi.cpp'
// Заголовок    : Драйвер SPI
// Автор        : Барышников Р. А.
// Контакты     : plexus_bra@rambler.ru
// Дата         : 26.06.2013
//
//*****************************************************************************

#include "spi.h"
#include "gpio.h"
#include <unistd.h>
#include <stdio.h>

// =============================================================================
//                           Глобальные переменные
// =============================================================================

	// extern	CSPI spi;

// =============================================================================
//                                 Класс CSPI
// =============================================================================

// =============================================================================
///
///                                Конструктор
///
// =============================================================================

CSPI::CSPI()
{
	for (int i = 0; i < N_CS; i++)
	{
		m_CS[i] = -1;
	}
}

// =============================================================================
///
///                            Инициализация SPI
///
// =============================================================================
/// \param  miso  Линия MISO
/// \param  mosi  Линия MOSI
/// \param  clk   Линия CLK
// =============================================================================

void CSPI::Init (int miso, int mosi, int clk)
{
	m_MISO = miso;
	m_MOSI = mosi;
	m_CLK  = clk;

	GPIOInit(m_MISO);
	GPIODir(m_MISO, DIR_IN);
	GPIOInit(m_MOSI);
	GPIOInit(m_CLK);

	GPIOSet(m_MOSI);
	GPIOClr(m_CLK);

	m_us = 300;
}

// =============================================================================
///
///                            Установка задержки
///
// =============================================================================
/// \param  us  Значение в мкс.
// =============================================================================

void CSPI::SetDelay(int us)
{
	m_us = us;
}

// =============================================================================
///
///                            Отправка байта
///
// =============================================================================
/// \param  byte  Значение байта
// =============================================================================

void CSPI::WriteByte(char byte)
{
	for (int i = 0; i < 8; i++, byte <<= 1)
	{
		if ((byte & 0x80) >> 7) GPIOSet(m_MOSI);
		else		  GPIOClr(m_MOSI);

		usleep(m_us);
		GPIOSet(m_CLK);
		usleep(m_us);
		GPIOClr(m_CLK);
	}
	GPIOSet(m_MOSI);
}

// =============================================================================
///
///                            Чтение байта
///
// =============================================================================
/// \return Значение байта
// =============================================================================

char CSPI::ReadByte(void)
{
	char byte = 0;
	for (int i = 0; i < 8; i++)
	{
		byte <<=1;
		usleep(m_us);
		GPIOSet(m_CLK);
		usleep(m_us);
		byte |= 0x01 & GPIOGet(m_MISO);
		usleep(m_us);
		GPIOSet(m_CLK);
		usleep(m_us);
		GPIOClr(m_CLK);
	}
	return byte;
}

// =============================================================================
///
///                          Инициализация линии CS
///
// =============================================================================
/// \param  pin  Линия CS
/// \return		 Индекс линии CS
// =============================================================================

int CSPI::InitCS(int pin)
{
	// Поиск свободной линии CS в таблице m_CS
	for (int index = 0; index < N_CS; index++)
	{
		if (m_CS[index] == -1)
		{
			GPIOInit(pin);
			m_CS[index] = pin;
			GPIOSet(pin);
			return index;
		}
	}
	return -1;
}

// =============================================================================
///
///                          Установка линии CS в 1
///
// =============================================================================
/// \param  index  Индекс линии CS
// =============================================================================

void CSPI::SetCS(int index)
{
	usleep(500);
	GPIOSet(m_CS[index]);
	usleep(500);
}

// =============================================================================
///
///                          Установка линии CS в 0
///
// =============================================================================
/// \param  index  Индекс линии CS
// =============================================================================

void CSPI::ClrCS(int index)
{
	usleep(500);
	GPIOClr(m_CS[index]);
	usleep(500);
}