/*
 * can_reg.h
 *
 *  Created on: 24 сент. 2024 г.
 *      Author: Ruslan
 */

#ifndef CAN_BUS_CAN_REG_H_
#define CAN_BUS_CAN_REG_H_

#include "lib/bits/bits.h"

//CAN master control register (CAN_MCR)
#ifndef CAN_MCR_INRQ
#define CAN_MCR_INRQ	((uint32_t)BIT(0))	//Bit 0 INRQ: Initialization request
#endif

#ifndef CAN_MCR_SLEEP
#define CAN_MCR_SLEEP	((uint32_t)BIT(1))	//Bit 1 SLEEP: Sleep mode request
#endif

#ifndef CAN_MCR_TXFP
#define CAN_MCR_TXFP	((uint32_t)BIT(2))	//Bit 2 TXFP: Transmit FIFO priority
#endif

#ifndef CAN_MCR_RFLM
#define CAN_MCR_RFLM	((uint32_t)BIT(3))	//Bit 3 RFLM: Receive FIFO locked mode
#endif

#ifndef CAN_MCR_NART
#define CAN_MCR_NART	((uint32_t)BIT(4))	//Bit 4 NART: No automatic retransmission
#endif

#ifndef CAN_MCR_AWUM
#define CAN_MCR_AWUM	((uint32_t)BIT(5))	//Bit 5 AWUM: Automatic wakeup mode
#endif

#ifndef CAN_MCR_ABOM
#define CAN_MCR_ABOM	((uint32_t)BIT(6))	//Bit 6 ABOM: Automatic bus-off management
#endif

#ifndef CAN_MCR_TTCM
#define CAN_MCR_TTCM	((uint32_t)BIT(7))	//Bit 7 TTCM: Time triggered communication mode
#endif

#ifndef CAN_MCR_RESET
#define CAN_MCR_RESET	((uint32_t)BIT(15))	//Bit 15 RESET: bxCAN software master reset
#endif

#ifndef CAN_MCR_DBF
#define CAN_MCR_DBF		((uint32_t)BIT(16))	//Bit 16 DBF: Debug freeze
#endif

//CAN master status register (CAN_MSR)
#ifndef CAN_MSR_INAK
#define CAN_MSR_INAK	((uint32_t)BIT(0))	//Bit 0 INAK: Initialization acknowledge
#endif

#ifndef CAN_MSR_SLAK
#define CAN_MSR_SLAK	((uint32_t)BIT(1))	//Bit 1 SLAK: Sleep acknowledge
#endif

#ifndef CAN_MSR_ERRI
#define CAN_MSR_ERRI	((uint32_t)BIT(2))	//Bit 2 ERRI: Error interrupt
#endif

#ifndef CAN_MSR_WKUI
#define CAN_MSR_WKUI	((uint32_t)BIT(3))	//Bit 3 WKUI: Wakeup interrupt
#endif

#ifndef CAN_MSR_SLAKI
#define CAN_MSR_SLAKI	((uint32_t)BIT(4))	//Bit 4 SLAKI: Sleep acknowledge interrupt
#endif

#ifndef CAN_MSR_TXM
#define CAN_MSR_TXM		((uint32_t)BIT(8))	//Bit 8 TXM: Transmit mode
#endif

#ifndef CAN_MSR_RXM
#define CAN_MSR_RXM		((uint32_t)BIT(9))	//Bit 9 RXM: Receive mode
#endif

#ifndef CAN_MSR_SAMP
#define CAN_MSR_SAMP	((uint32_t)BIT(10))	//Bit 10 SAMP: Last sample point
#endif

#ifndef CAN_MSR_RX
#define CAN_MSR_RX		((uint32_t)BIT(11))	//Bit 11 RX: CAN Rx signal
#endif

//CAN transmit status register (CAN_TSR)
#ifndef CAN_TSR_RQCP0
#define CAN_TSR_RQCP0	((uint32_t)BIT(0))	//Bit 0 RQCP0: Request completed mailbox0
#endif

#ifndef CAN_TSR_TXOK0
#define CAN_TSR_TXOK0	((uint32_t)BIT(1))	//Bit 1 TXOK0: Transmission OK of mailbox0
#endif

#ifndef CAN_TSR_ALST0
#define CAN_TSR_ALST0	((uint32_t)BIT(2))	//Bit 2 ALST0: Arbitration lost for mailbox0
#endif

#ifndef CAN_TSR_TERR0
#define CAN_TSR_TERR0	((uint32_t)BIT(3))	//Bit 3 TERR0: Transmission error of mailbox0
#endif

#ifndef CAN_TSR_ABRQ0
#define CAN_TSR_ABRQ0	((uint32_t)BIT(7))	//Bit 7 ABRQ0: Abort request for mailbox0
#endif

#ifndef CAN_TSR_RQCP1
#define CAN_TSR_RQCP1	((uint32_t)BIT(8))	//Bit 8 RQCP1: Request completed mailbox1
#endif

#ifndef CAN_TSR_TXOK1
#define CAN_TSR_TXOK1	((uint32_t)BIT(9))	//Bit 9 TXOK1: Transmission OK of mailbox1
#endif

#ifndef CAN_TSR_ALST1
#define CAN_TSR_ALST1	((uint32_t)BIT(10))	//Bit 10 ALST1: Arbitration lost for mailbox1
#endif

#ifndef CAN_TSR_TERR1
#define CAN_TSR_TERR1	((uint32_t)BIT(11))	//Bit 11 TERR1: Transmission error of mailbox1
#endif

#ifndef CAN_TSR_ABRQ1
#define CAN_TSR_ABRQ1	((uint32_t)BIT(15))	//Bit 15 ABRQ1: Abort request for mailbox 1
#endif

#ifndef CAN_TSR_RQCP2
#define CAN_TSR_RQCP2	((uint32_t)BIT(16))	//Bit 16 RQCP2: Request completed mailbox2
#endif

#ifndef CAN_TSR_TXOK2
#define CAN_TSR_TXOK2	((uint32_t)BIT(17))	//Bit 17 TXOK2: Transmission OK of mailbox 2
#endif

#ifndef CAN_TSR_ALST2
#define CAN_TSR_ALST2	((uint32_t)BIT(18))	//Bit 18 ALST2: Arbitration lost for mailbox 2
#endif

#ifndef CAN_TSR_TERR2
#define CAN_TSR_TERR2	((uint32_t)BIT(19))	//Bit 19 TERR2: Transmission error of mailbox 2
#endif

#ifndef CAN_TSR_ABRQ2
#define CAN_TSR_ABRQ2	((uint32_t)BIT(23))	//Bit 23 ABRQ2: Abort request for mailbox 2
#endif

#define CAN_TIR_CODE_LENGTH 2
#define CAN_TIR_CODE_SHIFT 24
#ifndef CAN_TSR_CODE
#define CAN_TSR_CODE	((uint32_t)BIT_MAKE_MASK(CAN_TIR_CODE_LENGTH,CAN_TIR_CODE_SHIFT))	//Bits 25:24 CODE[1:0]: Mailbox code
#endif

#ifndef CAN_TSR_TME0
#define CAN_TSR_TME0	((uint32_t)BIT(26))	//Bit 26 TME0: Transmit mailbox 0 empty
#endif

#ifndef CAN_TSR_TME1
#define CAN_TSR_TME1	((uint32_t)BIT(27))	//Bit 27 TME1: Transmit mailbox 1 empty
#endif

#ifndef CAN_TSR_TME2
#define CAN_TSR_TME2	((uint32_t)BIT(28))	//Bit 28 TME2: Transmit mailbox 2 empty
#endif

#ifndef CAN_TSR_LOW0
#define CAN_TSR_LOW0	((uint32_t)BIT(29))	//Bit 29 LOW0: Lowest priority flag for mailbox 0
#endif

#ifndef CAN_TSR_LOW1
#define CAN_TSR_LOW1	((uint32_t)BIT(30))	//Bit 30 LOW1: Lowest priority flag for mailbox 1
#endif

#ifndef CAN_TSR_LOW2
#define CAN_TSR_LOW2	((uint32_t)BIT(31))	//Bit 31 LOW2: Lowest priority flag for mailbox 2
#endif

//CAN receive FIFO 0 register (CAN_RFR)
#ifndef CAN_RFR_FMP
#define CAN_RFR_FMP		((uint32_t)BIT_MAKE_MASK(2,0))	//Bits 1:0 FMP[1:0]: FIFO message pending
#endif

#ifndef CAN_RFR_FULL
#define CAN_RFR_FULL	((uint32_t)BIT(3))	//Bit 3 FULL: FIFO full
#endif

#ifndef CAN_RFR_FOVR
#define CAN_RFR_FOVR	((uint32_t)BIT(4))	//Bit 4 FOVR: FIFO overrun
#endif

#ifndef CAN_RFR_RFOM
#define CAN_RFR_RFOM	((uint32_t)BIT(5))	//Bit 5 RFOM: Release FIFO output mailbox
#endif

//CAN interrupt enable register (CAN_IER)
#ifndef CAN_IER_TMEIE
#define CAN_IER_TMEIE	((uint32_t)BIT(0))	//Bit 0 TMEIE: Transmit mailbox empty interrupt enable
#endif

#ifndef CAN_IER_FMPIE0
#define CAN_IER_FMPIE0	((uint32_t)BIT(1))	//Bit 1 FMPIE0: FIFO message pending interrupt enable
#endif

#ifndef CAN_IER_FFIE0
#define CAN_IER_FFIE0	((uint32_t)BIT(2))	//Bit 2 FFIE0: FIFO full interrupt enable
#endif

#ifndef CAN_IER_FOVIE0
#define CAN_IER_FOVIE0	((uint32_t)BIT(3))	//Bit 3 FOVIE0: FIFO overrun interrupt enable
#endif

#ifndef CAN_IER_FMPIE1
#define CAN_IER_FMPIE1	((uint32_t)BIT(4))	//Bit 4 FMPIE1: FIFO message pending interrupt enable
#endif

#ifndef CAN_IER_FFIE1
#define CAN_IER_FFIE1	((uint32_t)BIT(5))	//Bit 5 FFIE1: FIFO full interrupt enable
#endif

#ifndef CAN_IER_FOVIE1
#define CAN_IER_FOVIE1	((uint32_t)BIT(6))	//Bit 6 FOVIE1: FIFO overrun interrupt enable
#endif

#ifndef CAN_IER_EWGIE
#define CAN_IER_EWGIE	((uint32_t)BIT(8))	//Bit 8 EWGIE: Error warning interrupt enable
#endif

#ifndef CAN_IER_EPVIE
#define CAN_IER_EPVIE	((uint32_t)BIT(9))	//Bit 9 EPVIE: Error passive interrupt enable
#endif

#ifndef CAN_IER_BOFIE
#define CAN_IER_BOFIE	((uint32_t)BIT(10))	//Bit 10 BOFIE: Bus-off interrupt enable
#endif

#ifndef CAN_IER_LECIE
#define CAN_IER_LECIE	((uint32_t)BIT(11))	//Bit 11 LECIE: Last error code interrupt enable
#endif

#ifndef CAN_IER_ERRIE
#define CAN_IER_ERRIE	((uint32_t)BIT(15))	//Bit 15 ERRIE: Error interrupt enable
#endif

#ifndef CAN_IER_WKUIE
#define CAN_IER_WKUIE	((uint32_t)BIT(16))	//Bit 16 WKUIE: Wakeup interrupt enable
#endif

#ifndef CAN_IER_SLKIE
#define CAN_IER_SLKIE	((uint32_t)BIT(17))	//Bit 17 SLKIE: Sleep interrupt enable
#endif

//CAN error status register (CAN_ESR)
#ifndef CAN_ESR_EWGF
#define CAN_ESR_EWGF	((uint32_t)BIT(0))	//Bit 0 EWGF: Error warning flag
#endif

#ifndef CAN_ESR_EPVF
#define CAN_ESR_EPVF	((uint32_t)BIT(1))	//Bit 1 EPVF: Error passive flag
#endif

#ifndef CAN_ESR_BOFF
#define CAN_ESR_BOFF	((uint32_t)BIT(2))	//Bit 2 BOFF: Bus-off flag
#endif

#define CAN_ESR_LEC_LENGTH 3
#define CAN_ESR_LEC_SHIFT 4
#ifndef CAN_ESR_LEC
#define CAN_ESR_LEC		((uint32_t)BIT_MAKE_MASK(CAN_ESR_LEC_LENGTH,CAN_ESR_LEC_SHIFT))	//Bits 6:4 LEC[2:0]: Last error code
#endif

#define CAN_ESR_TEC_LENGTH 8
#define CAN_ESR_TEC_SHIFT 16
#ifndef CAN_ESR_TEC
#define CAN_ESR_TEC		((uint32_t)BIT_MAKE_MASK(CAN_ESR_TEC_LENGTH,CAN_ESR_TEC_SHIFT))	//Bits 23:16 TEC[7:0]: Least significant byte of the 9-bit transmit error counter
#endif

#define CAN_ESR_REC_LENGTH 8
#define CAN_ESR_REC_SHIFT 24
#ifndef CAN_ESR_REC
#define CAN_ESR_REC		((uint32_t)BIT_MAKE_MASK(CAN_ESR_REC_LENGTH,CAN_ESR_REC_SHIFT))	//Bits 31:24 REC[7:0]: Receive error counter
#endif

//CAN bit timing register (CAN_BTR)
#ifndef CAN_BTR_BRP
#define CAN_BTR_BRP		((uint32_t)BIT_MAKE_MASK(10,0)) //Bits 9:0 BRP[9:0]: Baud rate prescaler
#endif

#ifndef CAN_BTR_TS1
#define CAN_BTR_TS1		((uint32_t)BIT_MAKE_MASK(4,16)) //Bits 19:16 TS1[3:0]: Time segment 1
#endif

#ifndef CAN_BTR_TS2
#define CAN_BTR_TS2		((uint32_t)BIT_MAKE_MASK(3,20))	//Bits 22:20 TS2[2:0]: Time segment 2
#endif

#ifndef CAN_BTR_SJW
#define CAN_BTR_SJW		((uint32_t)BIT_MAKE_MASK(2,24))	//Bits 25:24 SJW[1:0]: Resynchronization jump width
#endif

#ifndef CAN_BTR_LBKM
#define CAN_BTR_LBKM	((uint32_t)BIT(30))	//Bit 30 LBKM : Loop back mode (debug)
#endif

#ifndef CAN_BTR_SILM
#define CAN_BTR_SILM	((uint32_t)BIT(31))	//Bit 31 SILM: Silent mode (debug)
#endif

//CAN TX mailbox identifier register (CAN_TIR)
#ifndef CAN_TIR_TXRQ
#define CAN_TIR_TXRQ	((uint32_t)BIT(0))	//Bit 0 TXRQ: Transmit mailbox request
#endif

#ifndef CAN_TIR_RTR
#define CAN_TIR_RTR		((uint32_t)BIT(1))	//Bit 1 RTR: Remote transmission request
#endif

#ifndef CAN_TIR_IDE
#define CAN_TIR_IDE		((uint32_t)BIT(2))	//Bit 2 IDE : Identifier extension
#endif

#define CAN_TIR_EXID_LENGTH 29
#define CAN_TIR_EXID_SHIFT 3
#ifndef CAN_TIR_EXID
#define CAN_TIR_EXID	((uint32_t)BIT_MAKE_MASK(CAN_TIR_EXID_LENGTH,CAN_TIR_EXID_SHIFT))	//Bits 31:3 EXID[28:0]: Extended identifier
#endif

#define CAN_TIR_STID_LENGTH 11
#define CAN_TIR_STID_SHIFT 21
#ifndef CAN_TIR_STID
#define CAN_TIR_STID	((uint32_t)BIT_MAKE_MASK(CAN_TIR_STID_LENGTH,CAN_TIR_STID_SHIFT)) //Bits 31:21 STID[10:0]: Standard identifier
#endif

//CAN mailbox data length control and time stamp register (CAN_TDTR)
#ifndef CAN_TDTR_DLC
#define CAN_TDTR_DLC	((uint32_t)BIT_MAKE_MASK(4,0))	//Bits 3:0 DLC[3:0]: Data length code
#endif

#ifndef CAN_TDTR_TGT
#define CAN_TDTR_TGT	((uint32_t)BIT(8)) //Bit 8 TGT: Transmit global time
#endif

#ifndef CAN_TDTR_TIME
#define CAN_TDTR_TIME	((uint32_t)BIT_MAKE_MASK(16,16))	//Bits 31:16 TIME[15:0]: Message time stamp
#endif

//CAN mailbox data low register (CAN_TDLR)
#ifndef CAN_TDLR_DATA0
#define CAN_TDLR_DATA0	((uint32_t)BIT_MAKE_MASK(8,0))	//Bits 7:0 DATA0[7:0]: Data byte 0
#endif

#ifndef CAN_TDLR_DATA1
#define CAN_TDLR_DATA1	((uint32_t)BIT_MAKE_MASK(8,8))	//Bits 15:8 DATA1[7:0]: Data byte 1
#endif

#ifndef CAN_TDLR_DATA2
#define CAN_TDLR_DATA2	((uint32_t)BIT_MAKE_MASK(8,16))	//Bits 23:16 DATA2[7:0]: Data byte 2
#endif

#ifndef CAN_TDLR_DATA3
#define CAN_TDLR_DATA3	((uint32_t)BIT_MAKE_MASK(8,24))	//Bits 31:24 DATA3[7:0]: Data byte 3
#endif

//CAN mailbox data high register (CAN_TDHR)
#ifndef CAN_TDHR_DATA4
#define CAN_TDHR_DATA4	((uint32_t)BIT_MAKE_MASK(8,0))	//Bits 7:0 DATA4[7:0]: Data byte 4
#endif

#ifndef CAN_TDHR_DATA5
#define CAN_TDHR_DATA5	((uint32_t)BIT_MAKE_MASK(8,8))	//Bits 15:8 DATA5[7:0]: Data byte 5
#endif

#ifndef CAN_TDHR_DATA6
#define CAN_TDHR_DATA6	((uint32_t)BIT_MAKE_MASK(8,16))	//Bits 23:16 DATA6[7:0]: Data byte 6
#endif

#ifndef CAN_TDHR_DATA7
#define CAN_TDHR_DATA7	((uint32_t)BIT_MAKE_MASK(8,24))	//Bits 31:24 DATA7[7:0]: Data byte 7
#endif

//CAN receive FIFO mailbox identifier register
#ifndef CAN_RIR_RTR
#define CAN_RIR_RTR		((uint32_t)BIT(1))	//Bit 1 RTR: Remote transmission request
#endif

#ifndef CAN_RIR_IDE
#define CAN_RIR_IDE		((uint32_t)BIT(2))	//Bit 2 IDE: Identifier extension
#endif

#define CAN_RIR_EXID_LENGTH 29
#define CAN_RIR_EXID_SHIFT 3
#ifndef CAN_RIR_EXID
#define CAN_RIR_EXID	((uint32_t)BIT_MAKE_MASK(CAN_RIR_EXID_LENGTH,CAN_RIR_EXID_SHIFT))	//Bits 31:3 EXID[28:0]: Extended identifier
#endif

#define CAN_RIR_STID_LENGTH 11
#define CAN_RIR_STID_SHIFT 21
#ifndef CAN_RIR_STID
#define CAN_RIR_STID	((uint32_t)BIT_MAKE_MASK(CAN_RIR_STID_LENGTH,CAN_RIR_STID_SHIFT))	//Bits 31:21 STID[10:0]: Standard identifier
#endif

//CAN receive FIFO mailbox data length control and time stamp register
#ifndef CAN_RDTR_DLC
#define CAN_RDTR_DLC	((uint32_t)BIT_MAKE_MASK(4,0))	//Bits 3:0 DLC[3:0]: Data length code
#endif

#define CAN_RIR_FMI_LENGTH 8
#define CAN_RIR_FMI_SHIFT 8
#ifndef CAN_RDTR_FMI
#define CAN_RDTR_FMI	((uint32_t)BIT_MAKE_MASK(CAN_RIR_FMI_LENGTH,CAN_RIR_FMI_SHIFT))	//Bits 15:8 FMI[7:0]: Filter match index
#endif

#ifndef CAN_RDTR_TIME
#define CAN_RDTR_TIME	((uint32_t)BIT_MAKE_MASK(16,16))	//Bits 31:16 TIME[15:0]: Message time stamp
#endif

//CAN filter master register
#ifndef CAN_FMR_FINIT
#define CAN_FMR_FINIT	((uint32_t)BIT(0))	//Bit 0 FINIT: Filter init mode
#endif

#define	CAN_FMR_CAN2SB_LENGTH 6
#define	CAN_FMR_CAN2SB_SHIFT 8
#ifndef CAN_FMR_CAN2SB
#define CAN_FMR_CAN2SB	((uint32_t)BIT_MAKE_MASK(CAN_FMR_CAN2SB_LENGTH,CAN_FMR_CAN2SB_SHIFT))	//Bits 13:8CAN2SB[5:0]: CAN2 start bank
#endif

//CAN filter mode register
#define CAN_FM1R_FBMx_LENGTH 28
#define CAN_FM1R_FBMx_SHIFT 0
#ifndef CAN_FM1R_FBMx
#define CAN_FM1R_FBMx	((uint32_t)BIT_MAKE_MASK(CAN_FM1R_FBMx_LENGTH,CAN_FM1R_FBMx_SHIFT))	//Bits 27:0 FBMx: Filter mode
#endif

//CAN filter scale register
#define CAN_FS1R_FSCx_LENGTH 28
#define CAN_FS1R_FSCx_SHIFT 0
#ifndef CAN_FS1R_FSCx
#define CAN_FS1R_FSCx	((uint32_t)BIT_MAKE_MASK(CAN_FS1R_FSCx_LENGTH,CAN_FS1R_FSCx_SHIFT))	//Bits 27:0FSCx: Filter scale configuration
#endif

//CAN filter FIFO assignment register
#define CAN_FFA1R_FFAx_LENGTH 28
#define CAN_FFA1R_FFAx_SHIFT 0
#ifndef CAN_FFA1R_FFAx
#define CAN_FFA1R_FFAx	((uint32_t)BIT_MAKE_MASK(CAN_FFA1R_FFAx_LENGTH,CAN_FFA1R_FFAx_SHIFT))	//Bits 27:0FFAx: Filter FIFO assignment for filter x
#endif

//CAN filter activation register
#define CAN_FA1R_FACTx_LENGTH 28
#define CAN_FA1R_FACTx_SHIFT 0
#ifndef CAN_FA1R_FACTx
#define CAN_FA1R_FACTx	((uint32_t)BIT_MAKE_MASK(CAN_FA1R_FACTx_LENGTH,CAN_FA1R_FACTx_SHIFT))	//Bits 27:0 FACTx: Filter active
#endif

//CAN receive FiRx mailbox identifier register
#ifndef CAN_FIR_RTR
#define CAN_FIR_RTR		((uint32_t)BIT(1))	//Bit 1 RTR: Remote transmission request
#endif

#ifndef CAN_FIR_IDE
#define CAN_FIR_IDE		((uint32_t)BIT(2))	//Bit 2 IDE: Identifier extension
#endif

#ifndef CAN_FIR_EXID
#define CAN_FIR_EXID_LENGTH 29
#define CAN_FIR_EXID_SHIFT 3
#define CAN_FIR_EXID	((uint32_t)BIT_MAKE_MASK(CAN_FIR_EXID_LENGTH,CAN_FIR_EXID_SHIFT))	//Bits 31:3 EXID[28:0]: Extended identifier
#endif

#ifndef CAN_FIR_STID
#define CAN_FIR_STID_LENGTH 11
#define CAN_FIR_STID_SHIFT 21
#define CAN_FIR_STID	((uint32_t)BIT_MAKE_MASK(CAN_FIR_STID_LENGTH,CAN_FIR_STID_SHIFT))	//Bits 31:21 STID[10:0]: Standard identifier
#endif

#endif /* CAN_BUS_CAN_REG_H_ */
