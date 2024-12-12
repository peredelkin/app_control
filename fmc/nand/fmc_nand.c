/*
 * fmc_nand.c
 *
 *  Created on: 12 дек. 2023 г.
 *      Author: Ruslan
 */

#include "fmc_nand.h"
#include "yaffs2/yaffs_guts.h"
#include "yaffs2/yaffs_ecc.h"
#include "lib/utils/barrier.h"

//mem to mem -> Source:PAR to Destination:M0AR
//MINC for Read
//PINC for Write

const uint8_t nand_K9F1G08U0E_id[K9F1G08U0E_ID_DATA_COUNT] = {
		NANDFLASH_ID_READ_CYCLE1,
		NANDFLASH_ID_READ_CYCLE2,
		NANDFLASH_ID_READ_CYCLE3,
		NANDFLASH_ID_READ_CYCLE4,
		NANDFLASH_ID_READ_CYCLE5
};

nand_flash_driver_t nand_K9F1G08U0E_drv = {
		.initialized = false
};

err_t nand_K9F1G08U0E_init(nand_flash_driver_t *drv) {
	if(drv == NULL) return E_NULL_POINTER;

	if(drv->initialized == false) {
		dma_stream_struct_init(&(drv->dma), DMA2, DMA2_Stream4, 4);
		dma_stream_deinit(&(drv->dma));
		drv->initialized = true;
	}

	return E_NO_ERROR;
}

err_t nand_K9F1G08U0E_deinit(nand_flash_driver_t *drv) {
	if(drv == NULL) return E_NULL_POINTER;

	dma_stream_deinit(&(drv->dma));
	if(dma_stream_transfer_complete_interrupt_read(&drv->dma)) dma_stream_transfer_complete_interrupt_clear(&drv->dma);
	if(dma_stream_transfer_error_interrupt_read(&drv->dma)) dma_stream_transfer_error_interrupt_clear(&drv->dma);
	dma_stream_struct_deinit(&(drv->dma));

	return E_NO_ERROR;
}

/*command function sets begin*/
void nand_K9F1G08U0E_page_read_cmd_addr(uint16_t column, uint16_t row) {
	//write cmd/addr
	*K9F1G08U0E_COMMAND8_SECTION = NANDFLASH_CMD_PAGE_READ_CYCLE1;
	barrier();
	*K9F1G08U0E_ADDRESS16_SECTION = column;
	barrier();
	*K9F1G08U0E_ADDRESS16_SECTION = row;
	barrier();
	*K9F1G08U0E_COMMAND8_SECTION = NANDFLASH_CMD_PAGE_READ_CYCLE2;
	barrier();
	//wait write fifo
	fmc_fifo_empty_wait(K9F1G08U0E_CONTROL);
}

void nand_K9F1G08U0E_page_read_for_copy_back_cmd_addr(uint16_t column, uint16_t row) {
	//write cmd/addr
	*K9F1G08U0E_COMMAND8_SECTION = NANDFLASH_CMD_PAGE_READ_COPY_BACK_CYCLE1;
	barrier();
	*K9F1G08U0E_ADDRESS16_SECTION = column;
	barrier();
	*K9F1G08U0E_ADDRESS16_SECTION = row;
	barrier();
	*K9F1G08U0E_COMMAND8_SECTION = NANDFLASH_CMD_PAGE_READ_COPY_BACK_CYCLE2;
	barrier();
	//wait write fifo
	fmc_fifo_empty_wait(K9F1G08U0E_CONTROL);
}

void nand_K9F1G08U0E_id_read_cmd_addr(void) {
	//write cmd/addr
	*K9F1G08U0E_COMMAND8_SECTION = NANDFLASH_CMD_ID_READ_CYCLE1;
	barrier();
	*K9F1G08U0E_ADDRESS8_SECTION = 0x00;
	barrier();
	//wait write fifo
	fmc_fifo_empty_wait(K9F1G08U0E_CONTROL);
}

void nand_K9F1G08U0E_reset(void) {
	//write cmd
	*K9F1G08U0E_COMMAND8_SECTION = NANDFLASH_CMD_RESET_CYCLE1;
	barrier();
	//wait write fifo
	fmc_fifo_empty_wait(K9F1G08U0E_CONTROL);
}

void nand_K9F1G08U0E_page_program_cmd_addr(uint16_t column, uint16_t row) {
	//write cmd/addr
	*K9F1G08U0E_COMMAND8_SECTION = NANDFLASH_CMD_PAGE_PROGRAM_CYCLE1;
	barrier();
	*K9F1G08U0E_ADDRESS16_SECTION = column;
	barrier();
	*K9F1G08U0E_ADDRESS16_SECTION = row;
	barrier();
	//wait write fifo
	fmc_fifo_empty_wait(K9F1G08U0E_CONTROL);
}

void nand_K9F1G08U0E_page_program_cmd(void) {
	//write cmd/addr
	*K9F1G08U0E_COMMAND8_SECTION = NANDFLASH_CMD_PAGE_PROGRAM_CYCLE2;
	barrier();
	//wait write fifo
	fmc_fifo_empty_wait(K9F1G08U0E_CONTROL);
}

void nand_K9F1G08U0E_block_erase_cmd(uint16_t row) {
	//write cmd/addr
	*K9F1G08U0E_COMMAND8_SECTION = NANDFLASH_CMD_BLOCK_ERASE_CYCLE1;
	barrier();
	*K9F1G08U0E_ADDRESS16_SECTION = row;
	barrier();
	*K9F1G08U0E_COMMAND8_SECTION = NANDFLASH_CMD_BLOCK_ERASE_CYCLE2;
	barrier();
	//wait write fifo
	fmc_fifo_empty_wait(K9F1G08U0E_CONTROL);
}

uint8_t nand_K9F1G08U0E_status_read_cmd(void) {
	uint8_t status = 0;
	//write cmd
	*K9F1G08U0E_COMMAND8_SECTION = NANDFLASH_CMD_STATUS_READ_CYCLE1;
	barrier();
	//wait write fifo
	fmc_fifo_empty_wait(K9F1G08U0E_CONTROL);
	//read status
	do {
		status = *K9F1G08U0E_DATA8_SECTION;
	} while (!(status & NANDFLASH_STATUS_READ_READY));
	return status;
}
/*command function sets end*/

/*dma function sets start*/
err_t nand_K9F1G08U0E_conf_dma_to_read(nand_flash_driver_t *drv) {
	//stream check
	if (!dma_stream_ready(&(drv->dma))) return E_BUSY;
	//stream deinit
	dma_stream_deinit(&(drv->dma));
	//stream conf
	dma_stream_number_of_data(&(drv->dma), K9F1G08U0E_PAGE_TOTAL_SIZE); //Count
	dma_stream_peripheral_address(&(drv->dma), (uint32_t) K9F1G08U0E_DATA32_SECTION); //Source
	dma_stream_memory_address(&(drv->dma), 0, (uint32_t) (drv->data)); //Destination
	dma_stream_data_transfer_direction(&(drv->dma), 0b10); //Memory-to-memory
	dma_stream_memory_increment_mode(&(drv->dma), true); //Destination increment
	return E_NO_ERROR;
}

err_t nand_K9F1G08U0E_conf_dma_to_write(nand_flash_driver_t *drv) {
	//stream check
	if (!dma_stream_ready(&(drv->dma))) return E_BUSY;
	//stream deinit
	dma_stream_deinit(&(drv->dma));
	//stream conf
	dma_stream_number_of_data(&(drv->dma), K9F1G08U0E_PAGE_TOTAL_SIZE); //Count
	dma_stream_peripheral_address(&(drv->dma), (uint32_t) (drv->data)); //Source
	dma_stream_memory_address(&(drv->dma), 0, (uint32_t) K9F1G08U0E_DATA32_SECTION);//Destination
	dma_stream_data_transfer_direction(&(drv->dma), 0b10); //Memory-to-memory
	dma_stream_peripheral_increment_mode(&(drv->dma), true); //Source increment
	return E_NO_ERROR;
}

err_t nand_K9F1G08U0E_start_dma_transfer(nand_flash_driver_t *drv) {
	dma_stream_enable(&(drv->dma), true); //enable Stream
	while (!dma_stream_transfer_complete_interrupt_read(&drv->dma)) { //wait TC
		if (dma_stream_transfer_error_interrupt_read(&drv->dma)) { //if TE
			dma_stream_transfer_error_interrupt_clear(&drv->dma); //clear TE
			dma_stream_enable(&(drv->dma), false); //disable Stream
			return E_STATE;
		}
	}
	dma_stream_transfer_complete_interrupt_clear(&(drv->dma)); //clear TC
	dma_stream_enable(&(drv->dma), false); //disable Stream
	return E_NO_ERROR;
}
/*dma function sets end*/

/*Read page begin*/
err_t nand_K9F1G08U0E_page_read(nand_flash_driver_t *drv, uint16_t row) {
	//NULL pointers
	if(drv == NULL) return E_NULL_POINTER;
	if(drv->dma.stream == NULL) return E_NULL_POINTER;
	if(drv->dma.dma == NULL) return E_NULL_POINTER;
	//stream check and conf
	if(nand_K9F1G08U0E_conf_dma_to_read(drv)) return E_BUSY;
	//write cmd/addr
	nand_K9F1G08U0E_page_read_cmd_addr(K9F1G08U0E_PAGE_START_ADDRESS, row);
	//read data
	if(nand_K9F1G08U0E_start_dma_transfer(drv)) return E_STATE;
	return E_NO_ERROR;
}

uint8_t nand_K9F1G08U0E_check_bad_page(uint16_t row) {
	//write cmd/addr
	nand_K9F1G08U0E_page_read_cmd_addr(K9F1G08U0E_PAGE_STATE_ADDRESS, row);
	//read data
	return *K9F1G08U0E_DATA8_SECTION;
}

err_t nand_K9F1G08U0E_block_check_bad(uint16_t block) {
	uint16_t row = block * K9F1G08U0E_BLOCK_SIZE;
	for(int i = 0; i < K9F1G08U0E_MARK_SPARE_COUNT; i++) {
		if(nand_K9F1G08U0E_check_bad_page(row + i) != K9F1G08U0E_BLOCK_GOOD_MASK) {
			return E_INVALID_VALUE;
		}
	}
	return E_NO_ERROR;
}
/*Read page end*/

/*Read ID begin*/
err_t nand_K9F1G08U0E_id_check(nand_flash_driver_t *drv) {
	//write cmd/addr
	nand_K9F1G08U0E_id_read_cmd_addr();
	//read data
	fmc_data_8_read(K9F1G08U0E_DATA8_SECTION, drv->id, K9F1G08U0E_ID_DATA_COUNT);
	//check id
	for(int i = 0; i < K9F1G08U0E_ID_DATA_COUNT; i++) {
		if(nand_K9F1G08U0E_id[i] != drv->id[i]) return E_INVALID_VALUE;
	}
	return E_NO_ERROR;
}
/*Read ID end*/

/*Page Program begin*/
err_t nand_K9F1G08U0E_page_program(nand_flash_driver_t *drv, uint16_t row) {
	//NULL pointers
	if(drv == NULL) return E_NULL_POINTER;
	if(drv->dma.stream == NULL) return E_NULL_POINTER;
	if(drv->dma.dma == NULL) return E_NULL_POINTER;
	//check protected/not protected
	if(!(nand_K9F1G08U0E_status_read_cmd() & NANDFLASH_STATUS_READ_NOT_PROTECTED)) return E_CANCELED;
	//stream check and conf
	if(nand_K9F1G08U0E_conf_dma_to_write(drv)) return E_BUSY;
	//page program cmd/addr cycle 1
	nand_K9F1G08U0E_page_program_cmd_addr(K9F1G08U0E_PAGE_START_ADDRESS, row);
	//write data
	if(nand_K9F1G08U0E_start_dma_transfer(drv)) return E_STATE;
	//wait write fifo
	fmc_fifo_empty_wait(K9F1G08U0E_CONTROL);
	//page program cmd cycle 2
	nand_K9F1G08U0E_page_program_cmd();
	//program pass/fail
	if(nand_K9F1G08U0E_status_read_cmd() & NANDFLASH_STATUS_READ_FAIL) return E_STATE;
	return E_NO_ERROR;
}

err_t nand_K9F1G08U0E_mark_bad_page(uint16_t row) {
	//check protected/not protected
	if(!(nand_K9F1G08U0E_status_read_cmd() & NANDFLASH_STATUS_READ_NOT_PROTECTED)) return E_CANCELED;
	//page program cmd/addr cycle 1
	nand_K9F1G08U0E_page_program_cmd_addr(K9F1G08U0E_PAGE_STATE_ADDRESS, row);
	//write data
	*K9F1G08U0E_DATA8_SECTION = K9F1G08U0E_BLOCK_BAD_MASK;
	//wait write fifo
	fmc_fifo_empty_wait(K9F1G08U0E_CONTROL);
	//page program cmd cycle 2
	nand_K9F1G08U0E_page_program_cmd();
	//program pass/fail
	if(nand_K9F1G08U0E_status_read_cmd() & NANDFLASH_STATUS_READ_FAIL) return E_STATE;
	return E_NO_ERROR;
}

err_t nand_K9F1G08U0E_block_mark_bad(uint16_t block) {
	err_t error = E_NO_ERROR;
	uint16_t row = block * K9F1G08U0E_BLOCK_SIZE;
	for(int i = 0; i < K9F1G08U0E_MARK_SPARE_COUNT; i++) {
		error = nand_K9F1G08U0E_mark_bad_page(row + i);
		if(error) return error;
	}
	return error;
}
/*Page Program end*/

/*Block Erase begin*/
err_t nand_K9F1G08U0E_block_erase(uint16_t block) {
	uint16_t row = block * K9F1G08U0E_BLOCK_SIZE;
	//block erase cmd/addr cycle 1 and cycle 2
	nand_K9F1G08U0E_block_erase_cmd(row);
	//erase pass/fail
	if(nand_K9F1G08U0E_status_read_cmd() & NANDFLASH_STATUS_READ_FAIL) return E_STATE;
	return E_NO_ERROR;
}
/*Block Erase end*/

void nand_K9F1G08U0E_page_ecc_check_and_correct(nand_flash_driver_t *drv, enum yaffs_ecc_result *ecc_result)
{
    enum yaffs_ecc_result ecc_res = YAFFS_ECC_RESULT_UNKNOWN;

    //адрес чтения ECC
    uint8_t* page_ecc = &drv->data[K9F1G08U0E_PAGE_ECC_ADDRESS];

    int res;
    for(int i = 0; i < K9F1G08U0E_ECC_RESULT_COUNT; i ++){
        uint8_t* page_data = &drv->data[i * K9F1G08U0E_ECC_PAGE_SIZE];

        yaffs_ecc_calc(page_data, drv->ecc_tmp);
        res = yaffs_ecc_correct(page_data, &(page_ecc[i * K9F1G08U0E_ECC_RESULT_SIZE]), drv->ecc_tmp);

        if(res > ecc_res) ecc_res = res;
    }

    if(ecc_result) *ecc_result = ecc_res;
}

void nand_K9F1G08U0E_page_ecc_calc(nand_flash_driver_t *drv)
{
	//адрес записи ECC
	uint8_t* page_ecc = &drv->data[K9F1G08U0E_PAGE_ECC_ADDRESS];

    for(int i = 0; i < K9F1G08U0E_ECC_RESULT_COUNT; i ++){
    	uint8_t* page_data = &drv->data[i * K9F1G08U0E_ECC_PAGE_SIZE];

        yaffs_ecc_calc(page_data, &(page_ecc[i * K9F1G08U0E_ECC_RESULT_SIZE]));
    }
}

/*YAFFS BEGIN*/
//TODO: проверить!
int drv_K9F1G08U0E_write_chunk_fn(struct yaffs_dev *dev, int nand_chunk, const u8 *data, int data_len, const u8 *oob,
		int oob_len) {

	if(data_len > K9F1G08U0E_PAGE_DATA_SIZE) return YAFFS_FAIL;
	if(oob_len > K9F1G08U0E_PAGE_OOB_SIZE) return YAFFS_FAIL;

	//fill data with 0xFF
	memset(nand_K9F1G08U0E_drv.data, 0xFF, K9F1G08U0E_PAGE_TOTAL_SIZE);

	//copy data
	memcpy(&(nand_K9F1G08U0E_drv.data[K9F1G08U0E_PAGE_START_ADDRESS]), data, data_len);

	//copy oob
	memcpy(&(nand_K9F1G08U0E_drv.data[K9F1G08U0E_PAGE_OOB_ADDRESS]), oob, oob_len);

	//calc ecc
	nand_K9F1G08U0E_page_ecc_calc(&nand_K9F1G08U0E_drv);

	//page program
	if(nand_K9F1G08U0E_page_program(&nand_K9F1G08U0E_drv, (uint16_t) nand_chunk) != E_NO_ERROR)
		return YAFFS_FAIL;

	return YAFFS_OK;
}

//TODO: проверить!
int drv_K9F1G08U0E_read_chunk_fn(struct yaffs_dev *dev, int nand_chunk, u8 *data, int data_len, u8 *oob, int oob_len,
		enum yaffs_ecc_result *ecc_result) {

	if(data_len > K9F1G08U0E_PAGE_DATA_SIZE) return YAFFS_FAIL;
	if(oob_len > K9F1G08U0E_PAGE_OOB_SIZE) return YAFFS_FAIL;

	//reset ecc result
	if(ecc_result) *ecc_result = YAFFS_ECC_RESULT_UNKNOWN;

	//read page
	if(nand_K9F1G08U0E_page_read(&nand_K9F1G08U0E_drv, (uint16_t) nand_chunk) != E_NO_ERROR)
		return YAFFS_FAIL;

	//correct ecc
	nand_K9F1G08U0E_page_ecc_check_and_correct(&nand_K9F1G08U0E_drv, ecc_result);

	//copy data
	memcpy(data, &(nand_K9F1G08U0E_drv.data[K9F1G08U0E_PAGE_START_ADDRESS]), data_len);

	//copy oob
	memcpy(oob, &(nand_K9F1G08U0E_drv.data[K9F1G08U0E_PAGE_OOB_ADDRESS]), oob_len);

	return YAFFS_OK;
}

int drv_K9F1G08U0E_erase_fn(struct yaffs_dev *dev, int block_no) {
	if(block_no > K9F1G08U0E_BLOCK_COUNT) return YAFFS_FAIL;

	if (nand_K9F1G08U0E_block_erase((uint16_t) block_no) != E_NO_ERROR)
		return YAFFS_FAIL;
	return YAFFS_OK;
}

int drv_K9F1G08U0E_mark_bad_fn(struct yaffs_dev *dev, int block_no) {
	if(block_no > K9F1G08U0E_BLOCK_COUNT) return YAFFS_FAIL;

	if (nand_K9F1G08U0E_block_mark_bad((uint16_t) block_no) != E_NO_ERROR)
		return YAFFS_FAIL;
	return YAFFS_OK;
}

int drv_K9F1G08U0E_check_bad_fn(struct yaffs_dev *dev, int block_no) {
	if(block_no > K9F1G08U0E_BLOCK_COUNT) return YAFFS_FAIL;

	if (nand_K9F1G08U0E_block_check_bad((uint16_t)block_no) != E_NO_ERROR)
		return YAFFS_FAIL;
	return YAFFS_OK;
}

int drv_K9F1G08U0E_initialise_fn(struct yaffs_dev *dev) {
	if (nand_K9F1G08U0E_init(&nand_K9F1G08U0E_drv) != E_NO_ERROR)
		return YAFFS_FAIL;
	return YAFFS_OK;
}

int drv_K9F1G08U0E_deinitialise_fn(struct yaffs_dev *dev) {
	if (nand_K9F1G08U0E_deinit(&nand_K9F1G08U0E_drv) != E_NO_ERROR)
		return YAFFS_FAIL;
	return YAFFS_OK;
}

/*
int write_K9F1G08U0E_chunk_tags_fn(struct yaffs_dev *dev, int nand_chunk, const u8 *data,
		const struct yaffs_ext_tags *tags) {
	return YAFFS_OK;
}

int read_K9F1G08U0E_chunk_tags_fn(struct yaffs_dev *dev, int nand_chunk, u8 *data, struct yaffs_ext_tags *tags) {
	return YAFFS_OK;
}

int query_K9F1G08U0E_block_fn(struct yaffs_dev *dev, int block_no, enum yaffs_block_state *state, u32 *seq_number) {
	return YAFFS_OK;
}

int mark_K9F1G08U0E_bad_fn(struct yaffs_dev *dev, int block_no) {
	return YAFFS_OK;
}
*/

static struct yaffs_dev dev_K9F1G08U0E;

//const YCHAR* K9F1G08U0E_name = "K9F1G08U0E";
const YCHAR* K9F1G08U0E_name = "nand";

int yaffs_start_up(void)
{
	struct yaffs_dev *dev = &dev_K9F1G08U0E;

	dev->param.name = K9F1G08U0E_name;
	dev->param.inband_tags = YAFFS_PARAM_INBAND_TAGS;
	dev->param.total_bytes_per_chunk = YAFFS_PARAM_TOTAL_BYTES_PER_CHUNK;
	dev->param.chunks_per_block = YAFFS_PARAM_CHUNKS_PER_BLOCK;
	dev->param.spare_bytes_per_chunk = YAFFS_PARAM_SPARE_BYTES_PER_CHUNK;
	dev->param.start_block = YAFFS_PARAM_START_BLOCK;
	dev->param.end_block = YAFFS_PARAM_END_BLOCK;
	dev->param.n_reserved_blocks = YAFFS_PARAM_N_RESERVED_BLOCKS;
	dev->param.n_caches = YAFFS_PARAM_N_CACHES;
	dev->param.cache_bypass_aligned = YAFFS_PARAM_CACHE_BYPASS_ALIGNED;
	dev->param.use_nand_ecc = YAFFS_PARAM_USE_NAND_ECC;
	dev->param.tags_9bytes = YAFFS_PARAM_TAGS_9BYTES;
	dev->param.no_tags_ecc = YAFFS_PARAM_NO_TAGS_ECC;
	dev->param.is_yaffs2 = YAFFS_PARAM_IS_YAFFS2;
	dev->param.empty_lost_n_found = YAFFS_PARAM_EMPTY_LOST_N_FOUND;
	dev->param.refresh_period =	YAFFS_PARAM_REFRESH_PERIOD;
	dev->param.skip_checkpt_rd = YAFFS_PARAM_SKIP_CHECKPT_RD;
	dev->param.skip_checkpt_wr = YAFFS_PARAM_SKIP_CHECKPT_WR;
	dev->param.enable_xattr = YAFFS_PARAM_ENABLE_XATTR;
	dev->param.max_objects = YAFFS_PARAM_MAX_OBJECTS;
	dev->param.hide_lost_n_found = YAFFS_PARAM_HIDE_LOST_N_FOUND;
	dev->param.stored_endian = YAFFS_PARAM_STORED_ENDIAN;

	dev->drv.drv_write_chunk_fn = &drv_K9F1G08U0E_write_chunk_fn;
	dev->drv.drv_read_chunk_fn = &drv_K9F1G08U0E_read_chunk_fn;
	dev->drv.drv_erase_fn = &drv_K9F1G08U0E_erase_fn;
	dev->drv.drv_mark_bad_fn = &drv_K9F1G08U0E_mark_bad_fn;
	dev->drv.drv_check_bad_fn = &drv_K9F1G08U0E_check_bad_fn;
	dev->drv.drv_initialise_fn = &drv_K9F1G08U0E_initialise_fn;
	dev->drv.drv_deinitialise_fn = &drv_K9F1G08U0E_deinitialise_fn;

    dev->tagger.write_chunk_tags_fn = NULL;//&write_nandfile_chunk_tags_fn;
    dev->tagger.read_chunk_tags_fn = NULL;//&read_nandfile_chunk_tags_fn;
    dev->tagger.query_block_fn = NULL;//&query_nandfile_block_fn;
    dev->tagger.mark_bad_fn = NULL;//&mark_nandfile_bad_fn;

    dev->driver_context = (void*)(&nand_K9F1G08U0E_drv);

    yaffs_add_device(dev);

	return YAFFS_OK;
}

/*YAFFS END*/
