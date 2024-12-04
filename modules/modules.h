#ifndef MODULES_H
#define MODULES_H


#include "conf/conf.h"
#include "sys/main/sys_main.h"
#include "sys/timer/sys_timer.h"
#include "timer/ms/ms_timer.h"
#include "data_log/data_log.h"

#include "cli/cli.h"
#include "led/rgb/rgb_led.h"
#include "msdi/msdi.h"
#include "digital/input/digital_input.h"
#include "temp/ntc/ntc_temp.h"
#include "digital/output/relay/relay_output.h"
#include "digital/output/digital_output.h"
#include "digital/output/ncv7608/ncv7608.h"

extern M_conf conf;
extern M_sys_main sys;
extern M_sys_timer sys_tim;
extern M_ms_timer ms_tim;
extern M_data_log dlog;

extern M_cli cli;
extern M_rgb_led rgb_led;
extern M_msdi msdi;
extern M_ntc_temp ntc_temp;
extern M_relay_output do_relay;
extern M_ncv7608 do_ncv7608;
extern M_digital_input digital_in;
extern M_digital_output digital_out;

#endif /* MODULES_H */
