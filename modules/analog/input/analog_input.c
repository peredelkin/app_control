#include "analog_input.h"
#include "modules/modules.h"

METHOD_INIT_IMPL(M_analog_input, analog_input)
{
	analog_input->status = ANALOG_INPUT_STATUS_NONE;
	analog_input->control = ANALOG_INPUT_CONTROL_NONE;

	INIT(ai_ads8665);

	analog_input->status = ANALOG_INPUT_STATUS_READY;
}

METHOD_DEINIT_IMPL(M_analog_input, analog_input)
{
	DEINIT(ai_ads8665);
}

static void analog_input_control_handler(M_analog_input* analog_input) {
	if(analog_input->control & ANALOG_INPUT_CONTROL_START) {
		analog_input->control &= ~ANALOG_INPUT_CONTROL_START;
		analog_input->status |= ANALOG_INPUT_STATUS_RUN;
	}

	if(analog_input->control & ANALOG_INPUT_CONTROL_STOP) {
		analog_input->control &= ~ANALOG_INPUT_CONTROL_STOP;
		analog_input->status &= ~ANALOG_INPUT_STATUS_RUN;
	}
}

METHOD_CALC_IMPL(M_analog_input, analog_input)
{
	analog_input_control_handler(analog_input);

	if((analog_input->status & (ANALOG_INPUT_STATUS_READY | ANALOG_INPUT_STATUS_RUN)) ==
			(ANALOG_INPUT_STATUS_READY | ANALOG_INPUT_STATUS_RUN)) {
		CALC(ai_ads8665);
	}
}
