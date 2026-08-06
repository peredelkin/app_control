/*
 * exti.c
 *
 *  Created on: 5 авг. 2026 г.
 *      Author: ruslan
 */

#include "exti.h"

void(*exti_callback [16])();

#define EXTI_PR_PR9_5 (EXTI_PR_PR5 | EXTI_PR_PR6 | EXTI_PR_PR7 | EXTI_PR_PR8 | EXTI_PR_PR9)
#define EXTI_PR_PR15_10 (EXTI_PR_PR10 | EXTI_PR_PR11 | EXTI_PR_PR12 | EXTI_PR_PR13 | EXTI_PR_PR14 | EXTI_PR_PR15)

void exti15_10_init(uint32_t priority) {

	//GPI_CS_App_MC
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI15_PA;
	EXTI->IMR |= EXTI_IMR_MR15;
	EXTI->RTSR |= EXTI_RTSR_TR15;

	NVIC_SetPriority(EXTI15_10_IRQn, priority);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void EXTI0_IRQHandler() {
	uint32_t PR = EXTI->PR & EXTI_PR_PR0;

#ifdef EXTI0_ENABLED
	if(exti_callback[0] != NULL) {
		exti_callback[0]();
	}
#endif

	EXTI->PR = PR;
}

void EXTI1_IRQHandler() {
	uint32_t PR = EXTI->PR & EXTI_PR_PR1;

#ifdef EXTI1_ENABLED
	if(exti_callback[1] != NULL) {
		exti_callback[1]();
	}
#endif

	EXTI->PR = PR;
}

void EXTI2_IRQHandler() {
	uint32_t PR = EXTI->PR & EXTI_PR_PR2;

#ifdef EXTI2_ENABLED
	if(exti_callback[2] != NULL) {
		exti_callback[2]();
	}
#endif

	EXTI->PR = PR;
}

void EXTI3_IRQHandler() {
	uint32_t PR = EXTI->PR & EXTI_PR_PR3;

#ifdef EXTI3_ENABLED
	if(exti_callback[3] != NULL) {
		exti_callback[3]();
	}
#endif

	EXTI->PR = PR;
}

void EXTI4_IRQHandler() {
	uint32_t PR = EXTI->PR & EXTI_PR_PR4;

#ifdef EXTI4_ENABLED
	if(exti_callback[4] != NULL) {
		exti_callback[4]();
	}
#endif

	EXTI->PR = PR;
}

void EXTI9_5_IRQHandler() {
	uint32_t PR = EXTI->PR & EXTI_PR_PR9_5;

#ifdef EXTI5_ENABLED
	if((PR & EXTI_PR_PR5) && (exti_callback[5] != NULL)) {
		exti_callback[5]();
	}
#endif

#ifdef EXTI6_ENABLED
	if((PR & EXTI_PR_PR6) && (exti_callback[6] != NULL)) {
		exti_callback[6]();
	}
#endif

#ifdef EXTI7_ENABLED
	if((PR & EXTI_PR_PR7) && (exti_callback[7] != NULL)) {
		exti_callback[7]();
	}
#endif

#ifdef EXTI8_ENABLED
	if((PR & EXTI_PR_PR8) && (exti_callback[8] != NULL)) {
		exti_callback[8]();
	}
#endif

#ifdef EXTI9_ENABLED
	if((PR & EXTI_PR_PR9) && (exti_callback[9] != NULL)) {
		exti_callback[9]();
	}
#endif

	EXTI->PR = PR;
}

void EXTI15_10_IRQHandler() {
	uint32_t PR = EXTI->PR & EXTI_PR_PR15_10;

#ifdef EXTI10_ENABLED
	if((PR & EXTI_PR_PR10) && (exti_callback[10] != NULL)) {
		exti_callback[10]();
	}
#endif

#ifdef EXIT11_ENABLED
	if((PR & EXTI_PR_PR11) && (exti_callback[11] != NULL)) {
		exti_callback[11]();
	}
#endif

#ifdef EXTI12_ENABLED
	if((PR & EXTI_PR_PR12) && (exti_callback[12] != NULL)) {
		exti_callback[12]();
	}
#endif

#ifdef EXTI13_ENABLED
	if((PR & EXTI_PR_PR13) && (exti_callback[13] != NULL)) {
		exti_callback[13]();
	}
#endif

#ifdef EXTI14_ENABLED
	if((PR & EXTI_PR_PR14) && (exti_callback[14] != NULL)) {
		exti_callback[14]();
	}
#endif

#ifdef EXTI15_ENABLED
	if((PR & EXTI_PR_PR15) && (exti_callback[15] != NULL)) {
		exti_callback[15]();
	}
#endif

	EXTI->PR = PR;
}
