#include "analog_input.h"
#include "modules/modules.h"


METHOD_INIT_IMPL(M_analog_input, analog_input)
{
	INIT(ai_ads8665);

	INIT(ntc_temp);
	INIT(temp_comp);
}

METHOD_DEINIT_IMPL(M_analog_input, analog_input)
{
	DEINIT(ai_ads8665);

	DEINIT(ntc_temp);
	DEINIT(temp_comp);
}

METHOD_CALC_IMPL(M_analog_input, analog_input)
{
	CALC(ai_ads8665);

    CALC(ntc_temp);
    CALC(temp_comp);
}
