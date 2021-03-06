/*
 * Copyright 2014, 2015 AASHTO/ITE/NEMA.
 * American Association of State Highway and Transportation Officials,
 * Institute of Transportation Engineers and
 * National Electrical Manufacturers Association.
 *  
 * This file is part of the Advanced Transportation Controller (ATC)
 * Application Programming Interface Reference Implementation (APIRI).
 *  
 * The APIRI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 2.1 of the
 * License, or (at your option) any later version.
 *  
 * The APIRI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *  
 * You should have received a copy of the GNU Lesser General Public
 * License along with the APIRI.  If not, see <http://www.gnu.org/licenses/>.
 */

/*****************************************************************************/
/*

This file contains all definitions for the FIOMAN.

*/
/*****************************************************************************/

#ifndef _FIOMAN_H_
#define _FIOMAN_H_

/*  Include section.
-----------------------------------------------------------------------------*/

/* System includes. */
#include <linux/fs.h>
#include	<linux/list.h>		/* List definitions */
#include	<linux/kfifo.h>

/* Local includes. */
#include	"fiodriver.h"			/* FIO Driver Definitions */

/*  Definition section.
-----------------------------------------------------------------------------*/
/*  Module public structure/enum definition.*/


/*  Global section.
-----------------------------------------------------------------------------*/

/* Structure stored in *filp of App information */
struct fioman_priv_data
{
	struct list_head	fiod_list;	/* List of Apps FIODS */
	struct timer_list	hm_timer;	/* Health Monitor timer */
	unsigned int		hm_timeout;	/* Health Monitor timeout period */
	bool			hm_fault;	/* Health Monitor fault/timeout indication */
};
typedef	struct	fioman_priv_data	FIOMAN_PRIV_DATA;

/* Structure of FIODs registered by System */
struct fioman_sys_fiod
{
	spinlock_t		lock;
	struct	list_head	elem;		/* Allow structure in list */

	FIO_DEV_HANDLE		dev_handle;	/* Device Handle associated w/this elem */
	FIO_IOC_FIOD		fiod;		/* Port / FIOD combination */

	u16			app_enb;	/* FIOD enabled by APPS count */
	u16			app_reg;	/* Registered APPS count */
	struct list_head	app_fiod_list;	/* list of associated app_fiods */

	u8	status;
	u8	status_reset;
									/* Reserved output points */
	u8	outputs_reserved[ FIO_OUTPUT_POINTS_BYTES ];
									/* Load Switch Plus for FIOD */
	u8	outputs_plus[ FIO_OUTPUT_POINTS_BYTES ];
									/* Load Switch Minus for FIOD */
	u8	outputs_minus[ FIO_OUTPUT_POINTS_BYTES ];

									/* Unfiltered Input for FIOD */
	u8	inputs_raw[ FIO_INPUT_POINTS_BYTES ];
									/* Filtered Input for FIOD */
	u8	inputs_filtered[ FIO_INPUT_POINTS_BYTES ];
	u8	input_filters_leading[ FIO_INPUT_POINTS_BYTES ];
	u8	input_filters_trailing[ FIO_INPUT_POINTS_BYTES ];
	u8	input_transition_map[ FIO_INPUT_POINTS_BYTES ];		/* Inputs enabled for transition reporting */

	u8	channels_reserved[ FIO_CHANNEL_BYTES ];
									/* Channel mapping of Outputs for FIOD */
	u8	channel_map_red[ FIO_CHANNELS ];
	u8	channel_map_yellow[ FIO_CHANNELS ];
	u8	channel_map_green[ FIO_CHANNELS ];
	int	frame_frequency_table[128][2];				/* Each frame element has [valid?][frequency] */
	FIO_MMU_FLASH_BIT	flash_bit;
	FIO_TS_FM_STATE		fm_state;
	FIO_TS1_VM_STATE	vm_state;
	FIO_CMU_FSA		cmu_fsa;
	FIO_CMU_DC_MASK		cmu_mask;
	u32	cmu_config_change_count;
	int 	watchdog_output;
	bool	watchdog_state;
	u32	success_rx;					/* Cumulative count of successful responses */
	u32	error_rx;					/* Cumulative count of response errors */
};
typedef	struct	fioman_sys_fiod	FIOMAN_SYS_FIOD;

/* Structure of FIODs registered by App */
struct fioman_app_fiod
{
	struct	list_head	elem;		/* Allow structure in private data list */
	struct	list_head	sys_elem;	/* Allow structure in sys_fiod list */

	FIO_DEV_HANDLE		dev_handle;	/* Device Handle associated w/this elem */
	FIO_IOC_FIOD		fiod;		/* Port / FIOD combination */
	bool			enabled;	/* Has comm been enabled */

	FIOMAN_SYS_FIOD		*p_sys_fiod;/* Ptr to corresponding FIOMAN elem */

									/* Reserved output points */
	u8	outputs_reserved[ FIO_OUTPUT_POINTS_BYTES ];
									/* Load Switch Plus for FIOD */
	u8	outputs_plus[ FIO_OUTPUT_POINTS_BYTES ];
									/* Load Switch Minus for FIOD */
	u8	outputs_minus[ FIO_OUTPUT_POINTS_BYTES ];

	u8	input_filters_leading[ FIO_INPUT_POINTS_BYTES ];
	u8	input_filters_trailing[ FIO_INPUT_POINTS_BYTES ];
	u8	input_transition_map[ FIO_INPUT_POINTS_BYTES ];
	struct kfifo	transition_fifo;

	u8	channels_reserved[ FIO_CHANNEL_BYTES ];
	FIO_HZ	frame_frequency_table[128];
	FIO_MMU_FLASH_BIT	flash_bit;
	FIO_TS_FM_STATE		fm_state;
	FIO_TS1_VM_STATE	vm_state;
	FIO_CMU_FSA		cmu_fsa;
	FIO_CMU_DC_MASK		cmu_mask;
	bool			watchdog_reservation;
	bool			watchdog_toggle_pending;
	bool			hm_disabled;
};
typedef	struct	fioman_app_fiod	FIOMAN_APP_FIOD;

/*  Public Interface section.
-----------------------------------------------------------------------------*/
void fioman_init( void );
void fioman_exit( void );
int fioman_open( struct inode*,	struct file* );
int fioman_release( struct inode*, struct file* );
long fioman_ioctl( struct file*, unsigned int,	unsigned long );


#endif /* #ifndef _FIOMAN_H_ */


/*****************************************************************************/
/*

REVISION HISTORY:
$Log$

*/
/*****************************************************************************/
