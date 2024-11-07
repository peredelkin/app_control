/*
 * fmc_sram1_settings.c
 *
 *  Created on: 20 июл. 2024 г.
 *      Author: ruslan
 */

#include "fmc_settings.h"

#define FMC_SRAM1_READ_CYCLE_CLK 15	//(180*10^6)/(1/((55+25)*10^-9))
#define FMC_SRAM1_DATAST_CLK 10		//(180*10^6)/(1/((30+25)*10^-9))
#define FMC_SRAM1_ADDSET_CLK (FMC_SRAM1_READ_CYCLE_CLK - FMC_SRAM1_DATAST_CLK)
#define FMC_SRAM1_BUSTURN_CLK 5		//(180*10^6)/(1/((25)*10^-9))

const FMC_BCТRn_TypeDef FMC_SRAM1_BCTR_CFG = {
		.CR.bit.MBKEN = 1,		//Corresponding memory bank is enabled
		.CR.bit.MUXEN = 0,		//Address/Data nonmultiplexed
		.CR.bit.MTYP = 0,		//SRAM
		.CR.bit.MWID = 1,		//16 bits
		.CR.bit.FACCEN = 0,		//Corresponding NOR Flash memory access is disabled
		.CR.bit.BURSTEN = 0,	//Burst mode disabled
		.CR.bit.WAITPOL = 0,	//NWAIT active low
		.CR.bit.WRAPMOD = 0,	//Direct wrapped burst is not enabled
		.CR.bit.WAITCFG = 0,	//NWAIT signal is active one data cycle before wait state
		.CR.bit.WREN = 1,		// Write operations are enabled for the bank by the FMC
		.CR.bit.WAITEN = 0,		//NWAIT signal is disabled
		.CR.bit.EXTMOD = 1,		//values inside FMC_BWTR register are taken into account
		.CR.bit.ASYNCWAIT = 0, 	//NWAIT signal is not taken in to account when running an asynchronous protocol
		.CR.bit.CPSIZE = 0,		//No burst split when crossing page boundary
		.CR.bit.CBURSTRW = 0,	//Write operations are always performed in asynchronous mode
		.CR.bit.CCLKEN = 0,		//The FMC_CLK is only generated during the synchronous memory access

		.TR.bit.ADDSET = FMC_SRAM1_ADDSET_CLK,
		.TR.bit.ADDHLD = 15,	//Don't care
		.TR.bit.DATAST = FMC_SRAM1_DATAST_CLK,
		.TR.bit.BUSTURN = FMC_SRAM1_BUSTURN_CLK,
		.TR.bit.CLKDIV = 15,	//Don't care
		.TR.bit.DATLAT = 15,	//Don't care
		.TR.bit.ACCMOD = 0,		//access mode A
};

const FMC_BWTRn_TypeDef FMC_SRAM1_BWTR_CFG = {
		.WTR.bit.ADDSET = FMC_SRAM1_ADDSET_CLK,
		.WTR.bit.ADDHLD = 15,	//Don't care
		.WTR.bit.DATAST = FMC_SRAM1_DATAST_CLK,
		.WTR.bit.BUSTURN = FMC_SRAM1_BUSTURN_CLK,
		.WTR.bit.ACCMOD = 0,	//access mode A
};
