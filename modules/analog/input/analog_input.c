#include "analog_input.h"
#include "modules/modules.h"


METHOD_INIT_IMPL(M_analog_input, analog_input)
{
	INIT(ai_ads8665);
}

METHOD_DEINIT_IMPL(M_analog_input, analog_input)
{
	DEINIT(ai_ads8665);
}

METHOD_CALC_IMPL(M_analog_input, analog_input)
{
	CALC(ai_ads8665);
}
