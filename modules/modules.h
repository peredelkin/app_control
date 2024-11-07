#ifndef MODULES_H
#define MODULES_H


#include "conf/conf.h"
#include "sys_main/sys_main.h"
#include "sys_timer/sys_timer.h"
#include "ms_timer/ms_timer.h"
#include "data_log/data_log.h"

#include "cli/cli.h"
#include "rgb_led/rgb_led.h"
#include "msdi/msdi.h"
#include "digital_input/digital_input.h"
#include "ntc_temp/ntc_temp.h"
#include "digital_output/relay_output.h"
#include "digital_output/digital_output.h"
#include "ncv7608/ncv7608.h"

extern M_conf conf;
extern M_sys_main sys;
extern M_sys_timer sys_tim;
extern M_ms_timer ms_tim;
extern M_data_log dlog;

extern M_cli cli;
extern M_rgb_led rgb_led;
extern M_msdi msdi;
extern M_digital_input din;
extern M_ntc_temp ntc_temp;
extern M_relay_output do_relay;
extern M_digital_output dout;
extern M_ncv7608 do_ncv7608;

#endif /* MODULES_H */
