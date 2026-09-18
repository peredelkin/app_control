#ifndef MODULES_H
#define MODULES_H


#include "sys/main/sys_main.h"
#include "sys/main/sys_main_timer.h"

#include "sys/secondary/sys_secondary.h"
#include "sys/secondary/sys_secondary_timer.h"

#include "can/timer/can_timer.h"

#include "led/rgb/rgb_led.h"
#include "led/panel/panel_led.h"
#include "modbus_to_can/modbus_to_can.h"
#include "cli/cli.h"

#include "msdi/msdi.h"
#include "digital/output/ncv7608/ncv7608.h"
#include "digital/output/relay/relay_output.h"
#include "analog/input/ads8665/ads8685.h"
#include "analog/output/dac7562/dac7562.h"

#include "digital/input/digital_input.h"
#include "digital/output/digital_output.h"
#include "analog/input/analog_input.h"
#include "analog/output/analog_output.h"
#include "temp/ntc/ntc_temp.h"
#include "temp/comp/temp_wind_comp.h"
#include "settings/settings.h"

//основные модули
extern M_sys_main sys_main;						//!< 0x2000 -> 8192	@{"id": 8192}
extern M_sys_timer sys_main_tim;				//!< 0x2010 -> 8208	@{"id": 8208}

//второстепенные модули
extern M_sys_secondary sys_secondary;			//!< 0x2100 -> 8448	@{"id": 8448}
extern M_ms_timer sys_secondary_tim;			//!< 0x2110 -> 8464	@{"id": 8464}

//таймер CANopen
extern M_can_timer can_tim;						//!< 0x2200 -> 8704	@{"id": 8704}

extern M_rgb_led rgb_led;						//!< 0x2300 -> 8960	@{"id": 8960}
extern M_panel_led panel_led;					//!< 0x2310 -> 8976	@{"id": 8976}
extern M_modbus_to_can modbus_to_can_panel;		//!< 0x2320 -> 8992	@{"id": 8992}
extern M_cli cli;								//!< 0x2330 -> 9008	@{"id": 9008}

extern M_msdi msdi;								//!< 0x2400 -> 9216	@{"id": 9216}
extern M_ncv7608 do_ncv7608;					//!< 0x2410 -> 9232	@{"id": 9232}
extern M_relay_output do_relay;					//!< 0x2420 -> 9248	@{"id": 9248}
extern M_ads8685 ai_ads8665;					//!< 0x2430 -> 9264	@{"id": 9264}
extern M_dac7562 ao_dac7562;					//!< 0x2440 -> 9280	@{"id": 9280}

extern M_digital_input digital_in;				//!< 0x2500 -> 9472	@{"id": 9472}
extern M_digital_output digital_out;			//!< 0x2510 -> 9488	@{"id": 9488}
extern M_analog_input analog_in;				//!< 0x2520 -> 9504	@{"id": 9504}
extern M_analog_output analog_out;				//!< 0x2530 -> 9520	@{"id": 9520}
extern M_ntc_temp ntc_temp;						//!< 0x2540 -> 9536	@{"id": 9536}
extern M_temp_wind_comp temp_comp;				//!< 0x2550 -> 9552	@{"id": 9552}
extern M_settings settings;						//!< 0x2560 -> 9568	@{"id": 9568}

#endif /* MODULES_H */
