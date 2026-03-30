#include "analog_output.h"
#include "modules/modules.h"

METHOD_INIT_IMPL(M_analog_output, analog_output)
{
	INIT(ao_dac7562);
}

METHOD_DEINIT_IMPL(M_analog_output, analog_output)
{
	DEINIT(ao_dac7562);
}

METHOD_CALC_IMPL(M_analog_output, analog_output)
{
	CALC(ao_dac7562);
}
