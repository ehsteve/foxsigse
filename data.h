/*
 *  data.h
 *  FOXSI_GSE
 *
 *  Created by Steven Christe on 10/31/11.
 *  Copyright 2011 NASA GSFC. All rights reserved.
 * 
 *  Description
 *  -----------
 *
 *  Functions to read the data from the telemetry stream.
 *
 */
#ifndef _data_h_     // prevent multiple includes
#define _data_h_

#include "okFrontPanelDLL.h"

void* data_testfunction(void *p);
void* data_watch_buffer(void *p);
void* data_read_data(void *p);
void* data_timer(void *p);
void* data_countrate(void *p);
void data_start_file(void);
void data_simulate_data(void);
void data_start_reading(void);
void data_stop_reading(void);
void data_update_display(unsigned short int *frame);
void data_frame_print_to_file(unsigned short int *frame);
void data_set_datafilename(void);
void data_initialize(void);
void data_close(void);
okCFrontPanel *data_initialize_formatter_FPGA(void);

#endif