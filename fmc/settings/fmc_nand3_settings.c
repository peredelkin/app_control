/*
 * fmc_nand3_settings.c
 *
 *  Created on: 20 июл. 2024 г.
 *      Author: ruslan
 */

#include "fmc_settings.h"

#define FMC_NAND3_MEMSET 3	//2		(SET + 1) x t HCLK ≥ max (20nS, 12nS, 12nS, 10nS, 10nS) - 12nS
#define FMC_NAND3_MEMWAIT 4	//3		(WAIT + 1) x tHCLK ≥ max (12nS, 12nS)
#define FMC_NAND3_MEMHOLD 2	//1		(HOLD) x t HCLK ≥ max (5nS, 5nS , 5nS)
#define FMC_NAND3_MEMHIZ 5	//4		(HIZ) x tHCLK ≥ max (20nS, 12nS, 12nS) + (12nS - 12nS)
//((WAIT + 1) + (HOLD) + (SET + 1)) x tHCLK ≥ max (25nS, 25nS)

const FMC_BANK23_TypeDef FMC_NAND3_CFG = {
		.PCR.bit.PWAITEN = 1,	/*Wait feature enabled*/
		.PCR.bit.PBKEN = 1,		/*Corresponding memory bank is enabled*/
		.PCR.bit.PTYP = 1,		/*NAND Flash*/
		.PCR.bit.PWID = 0,		/*8 bits*/
		.PCR.bit.ECCEN = 0,		/*ECC logic is disabled and reset*/
		.PCR.bit.TCLR = 2,		/*tCLR min 10nS (t_clr = (TCLR + SET + 2) × THCLK where THCLK is the HCLK clock period)*/
		.PCR.bit.TAR = 2,		/*tAR min 10nS (t_ar = (TAR + SET + 2) × THCLK where THCLK is the HCLK clock period)*/
		.PCR.bit.ECCPS = 0b000,	/*Defines the page size for the extended ECC: 256 byte*/

		.PMEM.bit.MEMSET = FMC_NAND3_MEMSET,	/* Must be more 0 */
		.PMEM.bit.MEMWAIT = FMC_NAND3_MEMWAIT, /* Must be less 0xFF */
		.PMEM.bit.MEMHOLD = FMC_NAND3_MEMHOLD,
		.PMEM.bit.MEMHIZ = FMC_NAND3_MEMHIZ,

		.PATT.bit.ATTSET = FMC_NAND3_MEMSET,
		.PATT.bit.ATTWAIT = FMC_NAND3_MEMWAIT, /* Must be less 0xFF */
		.PATT.bit.ATTHOLD = FMC_NAND3_MEMHOLD, /* Must be more 1 */
		.PATT.bit.ATTHIZ = FMC_NAND3_MEMHIZ,
};
