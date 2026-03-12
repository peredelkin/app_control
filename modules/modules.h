#ifndef MODULES_H
#define MODULES_H


#include "conf/conf.h"
#include "sys/main/sys_main.h"
#include "sys/timer/sys_timer.h"
#include "timer/ms/ms_timer.h"
#include "mso/mso.h"

#include "cli/cli.h"
#include "led/rgb/rgb_led.h"
#include "msdi/msdi.h"
#include "digital/input/digital_input.h"
#include "temp/ntc/ntc_temp.h"
#include "digital/output/relay/relay_output.h"
#include "digital/output/digital_output.h"
#include "digital/output/ncv7608/ncv7608.h"
#include "analog/output/dac7562/dac7562.h"
#include "can/timer/can_timer.h"
#include "led/panel/panel_led.h"
#include "modbus_to_can/modbus_to_can.h"
#include "settings/settings.h"
#include "temp/comp/temp_wind_comp.h"
#include "analog/input/ads8665/ads8685.h"

extern M_conf conf;
extern M_sys_main sys;
extern M_sys_timer sys_tim;
extern M_ms_timer ms_tim;
extern M_can_timer can_tim;

extern M_cli cli;
extern M_rgb_led rgb_led;
extern M_msdi msdi;
extern M_ntc_temp ntc_temp;
extern M_relay_output do_relay;
extern M_ncv7608 do_ncv7608;
extern M_digital_input digital_in;				//!< 0x20b0 -> 8368	@{"id": 8368}
extern M_digital_output digital_out;			//!< 0x20c0 -> 8384	@{"id": 8384}
extern M_mso mso;
extern M_dac7562 ao_dac7562;
extern M_panel_led panel_led;
extern M_modbus_to_can modbus_to_can_panel;
extern M_settings settings;
extern M_temp_wind_comp temp_comp;
extern M_ads8685 ai_ads8665;

#endif /* MODULES_H */
