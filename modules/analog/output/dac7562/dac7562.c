#include "gpio/init/gpio_init.h"
#include "spi/init/spi_init.h"
#include "spi/settings/spi_settings.h"
#include "dac7562.h"

#define DAC7562_ADDR_MASK	((uint8_t)0b111)
#define DAC7562_CMD_MASK	((uint8_t)(0b111 << 3))

void dac7562_frame_fill(uint8_t* frame, uint8_t cmd, uint8_t addr, uint16_t data) {
	frame[0] = ((uint8_t*)&data)[0];
	frame[1] = ((uint8_t*)&data)[1];
	frame[2] = (DAC7562_CMD_MASK & (cmd << 3)) | (DAC7562_ADDR_MASK & addr);
}

void dac7562_write_input_a(uint8_t* frame, uint16_t data) {
	dac7562_frame_fill(frame, 0b000, 0b000, data);
}
void dac7562_write_input_b(uint8_t* frame, uint16_t data) {
	dac7562_frame_fill(frame, 0b000, 0b001, data);
}
void dac7562_write_input_ab(uint8_t* frame, uint16_t data) {
	dac7562_frame_fill(frame, 0b000, 0b111, data);
}

void dac7562_write_input_a_update_all(uint8_t* frame, uint16_t data) {
	dac7562_frame_fill(frame, 0b010, 0b000, data);
}
void dac7562_write_input_b_update_all(uint8_t* frame, uint16_t data) {
	dac7562_frame_fill(frame, 0b010, 0b001, data);
}
void dac7562_write_input_ab_update_all(uint8_t* frame, uint16_t data) {
	dac7562_frame_fill(frame, 0b010, 0b111, data);
}

void dac7562_write_input_a_update_a(uint8_t* frame, uint16_t data) {
	dac7562_frame_fill(frame, 0b011, 0b000, data);
}
void dac7562_write_input_b_update_b(uint8_t* frame, uint16_t data) {
	dac7562_frame_fill(frame, 0b011, 0b001, data);
}
void dac7562_write_input_ab_update_ab(uint8_t* frame, uint16_t data) {
	dac7562_frame_fill(frame, 0b011, 0b111, data);
}

void dac7562_gain_b2_a2(uint8_t* frame) {
	dac7562_frame_fill(frame, 0b000, 0b010, 0b00);
}
void dac7562_gain_b2_a1(uint8_t* frame) {
	dac7562_frame_fill(frame, 0b000, 0b010, 0b01);
}
void dac7562_gain_b1_a2(uint8_t* frame) {
	dac7562_frame_fill(frame, 0b000, 0b010, 0b10);
}
void dac7562_gain_b1_a1(uint8_t* frame) {
	dac7562_frame_fill(frame, 0b000, 0b010, 0b11);
}

void dac7562_update_a(uint8_t* frame) {
	dac7562_frame_fill(frame, 0b001, 0b000, 0);
}
void dac7562_update_b(uint8_t* frame) {
	dac7562_frame_fill(frame, 0b001, 0b001, 0);
}
void dac7562_update_all(uint8_t* frame) {
	dac7562_frame_fill(frame, 0b001, 0b111, 0);
}

void dac7562_power_up_a(uint8_t* frame) {
	dac7562_frame_fill(frame, 0b100, 0, 0b000001);
}
void dac7562_power_up_b(uint8_t* frame) {
	dac7562_frame_fill(frame, 0b100, 0, 0b000010);
}
void dac7562_power_up_ab(uint8_t* frame) {
	dac7562_frame_fill(frame, 0b100, 0, 0b000011);
}

void dac7562_power_down_a_1k(uint8_t* frame) {
	dac7562_frame_fill(frame, 0b100, 0, 0b010001);
}
void dac7562_power_down_b_1k(uint8_t* frame) {
	dac7562_frame_fill(frame, 0b100, 0, 0b010010);
}
void dac7562_power_down_ab_1k(uint8_t* frame) {
	dac7562_frame_fill(frame, 0b100, 0, 0b010011);
}

void dac7562_power_down_a_100k(uint8_t* frame) {
	dac7562_frame_fill(frame, 0b100, 0, 0b100001);
}
void dac7562_power_down_b_100k(uint8_t* frame) {
	dac7562_frame_fill(frame, 0b100, 0, 0b100010);
}
void dac7562_power_down_ab_100k(uint8_t* frame) {
	dac7562_frame_fill(frame, 0b100, 0, 0b100011);
}

void dac7562_power_down_a_hiz(uint8_t* frame) {
	dac7562_frame_fill(frame, 0b100, 0, 0b110001);
}
void dac7562_power_down_b_hiz(uint8_t* frame) {
	dac7562_frame_fill(frame, 0b100, 0, 0b110010);
}
void dac7562_power_down_ab_hiz(uint8_t* frame) {
	dac7562_frame_fill(frame, 0b100, 0, 0b110011);
}

void dac7562_reset_ab_input_update_all(uint8_t* frame) {
	dac7562_frame_fill(frame, 0b101, 0, 0);
}
void dac7562_reset_all_update_all(uint8_t* frame) {
	dac7562_frame_fill(frame, 0b101, 0, 1);
}

void dac7562_ldac_active_b_active_a(uint8_t* frame) {
	dac7562_frame_fill(frame, 0b110, 0, 0b00);
}
void dac7562_ldac_active_b_inactive_a(uint8_t* frame) {
	dac7562_frame_fill(frame, 0b110, 0, 0b01);
}
void dac7562_ldac_inactive_b_active_a(uint8_t* frame) {
	dac7562_frame_fill(frame, 0b110, 0, 0b10);
}
void dac7562_ldac_inactive_b_inactive_a(uint8_t* frame) {
	dac7562_frame_fill(frame, 0b110, 0, 0b11);
}

void dac7562_internal_ref_disable_gain_1(uint8_t* frame) {
	dac7562_frame_fill(frame, 0b111, 0, 0);
}
void dac7562_internal_ref_enable_gain_2(uint8_t* frame) {
	dac7562_frame_fill(frame, 0b111, 0, 1);
}

void dac7562_init(M_dac7562* dac7562) {
	gpio_output_bit_setup(&GPO_nCLR_App, GPIO_STATE_OFF);
	sys_counter_delay(0, 100);
	gpio_output_bit_setup(&GPO_nCLR_App, GPIO_STATE_ON);
	sys_counter_delay(0, 100);

	spi_bus_open(dac7562->m_spi_bus, &spi_dac7562_cfg);

	dac7562_internal_ref_enable_gain_2(dac7562->m_frame_ctrl);
	spi_bus_transfer(dac7562->m_spi_bus, &dac7562->spi_control[0], 1, NULL, NULL);
	spi_bus_wait(dac7562->m_spi_bus);

	dac7562_gain_b1_a1(dac7562->m_frame_ctrl);
	spi_bus_transfer(dac7562->m_spi_bus, &dac7562->spi_control[0], 1, NULL, NULL);
	spi_bus_wait(dac7562->m_spi_bus);

	spi_bus_close(dac7562->m_spi_bus);
}

METHOD_INIT_IMPL(M_dac7562, dac7562)
{
	dac7562->status = DAC7562_STATUS_NONE;
	dac7562->control = DAC7562_CONTROL_NONE;

	dac7562->m_spi_bus = &SPI2_Bus;

	gpio_dac7562_cfg_setup();

	dac7562->spi_control[0].count = 3;
	dac7562->spi_control[0].rx = NULL;
	dac7562->spi_control[0].tx = dac7562->m_frame_ctrl;
	dac7562->spi_control[0].byte_order = SPI_BYTE_ORDER_REVERSE;
	dac7562->spi_control[0].callback = NULL;
	dac7562->spi_control[0].callback_argument = NULL;

	dac7562->spi_control[1].count = 3;
	dac7562->spi_control[1].rx = NULL;
	dac7562->spi_control[1].tx = dac7562->m_frame_ch_a;
	dac7562->spi_control[1].byte_order = SPI_BYTE_ORDER_REVERSE;
	dac7562->spi_control[1].callback = NULL;
	dac7562->spi_control[1].callback_argument = NULL;

	dac7562->spi_control[2].count = 3;
	dac7562->spi_control[2].rx = NULL;
	dac7562->spi_control[2].tx = dac7562->m_frame_ch_b;
	dac7562->spi_control[2].byte_order = SPI_BYTE_ORDER_REVERSE;
	dac7562->spi_control[2].callback = NULL;
	dac7562->spi_control[2].callback_argument = NULL;

	dac7562_init(dac7562);

	dac7562->status = DAC7562_STATUS_READY;
}

METHOD_DEINIT_IMPL(M_dac7562, dac7562)
{
}

static void dac7562_control_handler(M_dac7562* dac7562) {
	if(dac7562->control & DAC7562_CONTROL_START) {
		dac7562->control &= ~DAC7562_CONTROL_START;
		dac7562->status |= DAC7562_STATUS_RUN;
	}

	if(dac7562->control & DAC7562_CONTROL_STOP) {
		dac7562->control &= ~DAC7562_CONTROL_STOP;
		dac7562->status &= ~DAC7562_STATUS_RUN;
	}
}

void dac7562_calc(M_dac7562* dac7562) {
	dac7562_write_input_a_update_a(dac7562->m_frame_ch_a, dac7562->in_ch_a);
	dac7562_write_input_b_update_b(dac7562->m_frame_ch_b, dac7562->in_ch_b);

	spi_bus_open(dac7562->m_spi_bus, &spi_dac7562_cfg);

	spi_bus_transfer(dac7562->m_spi_bus, &dac7562->spi_control[1], 1, NULL, NULL);
	spi_bus_wait(dac7562->m_spi_bus);

	spi_bus_transfer(dac7562->m_spi_bus, &dac7562->spi_control[2], 1, NULL, NULL);
	spi_bus_wait(dac7562->m_spi_bus);

	spi_bus_close(dac7562->m_spi_bus);
}

METHOD_CALC_IMPL(M_dac7562, dac7562)
{
	dac7562_control_handler(dac7562);

	if((dac7562->status & (DAC7562_STATUS_READY | DAC7562_STATUS_RUN)) ==
			(DAC7562_STATUS_READY | DAC7562_STATUS_RUN)) {
		dac7562_calc(dac7562);
	}
}

