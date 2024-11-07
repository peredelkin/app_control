#include "conf/conf.h"



static status_t recalc_values(M_conf* conf)
{
    status_t status = STATUS_READY;

    return status;
}


METHOD_INIT_IMPL(M_conf, conf)
{
    conf->status = STATUS_READY;

    status_t status = recalc_values(conf);

    if(status & STATUS_READY){
        conf->status |= STATUS_VALID;
    }
}

METHOD_DEINIT_IMPL(M_conf, conf)
{
    conf->status = STATUS_NONE;
}

METHOD_IDLE_IMPL(M_conf, conf)
{
    status_t status = recalc_values(conf);

    if(status & STATUS_READY){
        conf->status |= STATUS_VALID;
    }else if(status & STATUS_ERROR){
        conf->status &= ~STATUS_VALID;
    }
}
