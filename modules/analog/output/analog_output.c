#include "analog_output.h"
#include "modules/modules.h"

METHOD_INIT_IMPL(M_analog_output, analog_output)
{
	analog_output->status = ANALOG_OUTPUT_STATUS_NONE;
	analog_output->control = ANALOG_OUTPUT_CONTROL_NONE;

	INIT(ao_dac7562);

	analog_output->status = ANALOG_OUTPUT_STATUS_READY;
}

METHOD_DEINIT_IMPL(M_analog_output, analog_output)
{
	DEINIT(ao_dac7562);
}

static void analog_output_control_handler(M_analog_output* analog_output) {
	if(analog_output->control & ANALOG_OUTPUT_CONTROL_START) {
		analog_output->control &= ~ANALOG_OUTPUT_CONTROL_START;
		analog_output->status |= ANALOG_OUTPUT_STATUS_RUN;
	}

	if(analog_output->control & ANALOG_OUTPUT_CONTROL_STOP) {
		analog_output->control &= ~ANALOG_OUTPUT_CONTROL_STOP;
		analog_output->status &= ~ANALOG_OUTPUT_STATUS_RUN;
	}
}

METHOD_CALC_IMPL(M_analog_output, analog_output)
{
	analog_output_control_handler(analog_output);

	if((analog_output->status & (ANALOG_OUTPUT_STATUS_READY | ANALOG_OUTPUT_STATUS_RUN)) ==
			(ANALOG_OUTPUT_STATUS_READY | ANALOG_OUTPUT_STATUS_RUN)) {
		CALC(ao_dac7562);
	}
}
