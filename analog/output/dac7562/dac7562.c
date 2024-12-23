#include "gpio/init/gpio_init.h"
#include "dac7562.h"

#define DAC7562_ADDR_MASK	((uint8_t)0b111)
#define DAC7562_CMD_MASK	((uint8_t)(0b111 << 3))

void dac7562_frame_fill(M_dac7562 *dac7562, uint8_t cmd, uint8_t addr, uint16_t data) {
	dac7562->m_frame[0] = ((uint8_t*)&data)[0];
	dac7562->m_frame[1] = ((uint8_t*)&data)[1];
	dac7562->m_frame[2] = (DAC7562_CMD_MASK & (cmd << 3)) | (DAC7562_ADDR_MASK & addr);
}

void dac7562_write_input_a(M_dac7562 *dac7562, uint16_t data) {
	dac7562_frame_fill(dac7562, 0b000, 0b000, data);
}
void dac7562_write_input_b(M_dac7562 *dac7562, uint16_t data) {
	dac7562_frame_fill(dac7562, 0b000, 0b001, data);
}
void dac7562_write_input_ab(M_dac7562 *dac7562, uint16_t data) {
	dac7562_frame_fill(dac7562, 0b000, 0b111, data);
}

void dac7562_write_input_a_update_all(M_dac7562 *dac7562, uint16_t data) {
	dac7562_frame_fill(dac7562, 0b010, 0b000, data);
}
void dac7562_write_input_b_update_all(M_dac7562 *dac7562, uint16_t data) {
	dac7562_frame_fill(dac7562, 0b010, 0b001, data);
}
void dac7562_write_input_ab_update_all(M_dac7562 *dac7562, uint16_t data) {
	dac7562_frame_fill(dac7562, 0b010, 0b111, data);
}

void dac7562_write_input_a_update_a(M_dac7562 *dac7562, uint16_t data) {
	dac7562_frame_fill(dac7562, 0b011, 0b000, data);
}
void dac7562_write_input_b_update_b(M_dac7562 *dac7562, uint16_t data) {
	dac7562_frame_fill(dac7562, 0b011, 0b001, data);
}
void dac7562_write_input_ab_update_ab(M_dac7562 *dac7562, uint16_t data) {
	dac7562_frame_fill(dac7562, 0b011, 0b111, data);
}

void dac7562_gain_b2_a2(M_dac7562 *dac7562) {
	dac7562_frame_fill(dac7562, 0b000, 0b010, 0b00);
}
void dac7562_gain_b2_a1(M_dac7562 *dac7562) {
	dac7562_frame_fill(dac7562, 0b000, 0b010, 0b01);
}
void dac7562_gain_b1_a2(M_dac7562 *dac7562) {
	dac7562_frame_fill(dac7562, 0b000, 0b010, 0b10);
}
void dac7562_gain_b1_a1(M_dac7562 *dac7562) {
	dac7562_frame_fill(dac7562, 0b000, 0b010, 0b11);
}

void dac7562_update_a(M_dac7562 *dac7562) {
	dac7562_frame_fill(dac7562, 0b001, 0b000, 0);
}
void dac7562_update_b(M_dac7562 *dac7562) {
	dac7562_frame_fill(dac7562, 0b001, 0b001, 0);
}
void dac7562_update_all(M_dac7562 *dac7562) {
	dac7562_frame_fill(dac7562, 0b001, 0b111, 0);
}

void dac7562_power_up_a(M_dac7562 *dac7562) {
	dac7562_frame_fill(dac7562, 0b100, 0, 0b000001);
}
void dac7562_power_up_b(M_dac7562 *dac7562) {
	dac7562_frame_fill(dac7562, 0b100, 0, 0b000010);
}
void dac7562_power_up_ab(M_dac7562 *dac7562) {
	dac7562_frame_fill(dac7562, 0b100, 0, 0b000011);
}

void dac7562_power_down_a_1k(M_dac7562 *dac7562) {
	dac7562_frame_fill(dac7562, 0b100, 0, 0b010001);
}
void dac7562_power_down_b_1k(M_dac7562 *dac7562) {
	dac7562_frame_fill(dac7562, 0b100, 0, 0b010010);
}
void dac7562_power_down_ab_1k(M_dac7562 *dac7562) {
	dac7562_frame_fill(dac7562, 0b100, 0, 0b010011);
}

void dac7562_power_down_a_100k(M_dac7562 *dac7562) {
	dac7562_frame_fill(dac7562, 0b100, 0, 0b100001);
}
void dac7562_power_down_b_100k(M_dac7562 *dac7562) {
	dac7562_frame_fill(dac7562, 0b100, 0, 0b100010);
}
void dac7562_power_down_ab_100k(M_dac7562 *dac7562) {
	dac7562_frame_fill(dac7562, 0b100, 0, 0b100011);
}

void dac7562_power_down_a_hiz(M_dac7562 *dac7562) {
	dac7562_frame_fill(dac7562, 0b100, 0, 0b110001);
}
void dac7562_power_down_b_hiz(M_dac7562 *dac7562) {
	dac7562_frame_fill(dac7562, 0b100, 0, 0b110010);
}
void dac7562_power_down_ab_hiz(M_dac7562 *dac7562) {
	dac7562_frame_fill(dac7562, 0b100, 0, 0b110011);
}

void dac7562_reset_ab_input_update_all(M_dac7562 *dac7562) {
	dac7562_frame_fill(dac7562, 0b101, 0, 0);
}
void dac7562_reset_all_update_all(M_dac7562 *dac7562) {
	dac7562_frame_fill(dac7562, 0b101, 0, 1);
}

void dac7562_ldac_active_b_active_a(M_dac7562 *dac7562) {
	dac7562_frame_fill(dac7562, 0b110, 0, 0b00);
}
void dac7562_ldac_active_b_inactive_a(M_dac7562 *dac7562) {
	dac7562_frame_fill(dac7562, 0b110, 0, 0b01);
}
void dac7562_ldac_inactive_b_active_a(M_dac7562 *dac7562) {
	dac7562_frame_fill(dac7562, 0b110, 0, 0b10);
}
void dac7562_ldac_inactive_b_inactive_a(M_dac7562 *dac7562) {
	dac7562_frame_fill(dac7562, 0b110, 0, 0b11);
}

void dac7562_internal_ref_disable_gain_1(M_dac7562 *dac7562) {
	dac7562_frame_fill(dac7562, 0b111, 0, 0);
}
void dac7562_internal_ref_enable_gain_2(M_dac7562 *dac7562) {
	dac7562_frame_fill(dac7562, 0b111, 0, 1);
}

METHOD_INIT_IMPL(M_dac7562, dac7562)
{
	gpio_dac7562_cfg_setup();
}

METHOD_DEINIT_IMPL(M_dac7562, dac7562)
{
}

METHOD_CALC_IMPL(M_dac7562, dac7562)
{
}
