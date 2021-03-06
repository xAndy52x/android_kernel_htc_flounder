/*
 *
 * Code to extract Bluetooth bd_address information
 * from device_tree set up by the bootloader.
 *
 * Copyright (C) 2010 HTC Corporation
 * Author:Yomin Lin <yomin_lin@htc.com>
 * Author:Allen Ou <allen_ou@htc.com>
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <asm/setup.h>
#include <mach/flounder-bdaddress.h>
#include <linux/of.h>

/*configuration tags specific to Bluetooth*/
#define MAX_BT_SIZE 0x6U

#define CALIBRATION_DATA_PATH "/calibration_data"
#define BT_FLASH_DATA "bt_flash"

static unsigned char bt_bd_ram[MAX_BT_SIZE];
static char bdaddress[20];

static unsigned char *get_bt_bd_ram(void)
{
	struct device_node *offset = of_find_node_by_path(CALIBRATION_DATA_PATH);
	int p_size;
	unsigned char *p_data;

	p_size = 0;
	p_data = NULL;
	if (offset) {
		p_data = (unsigned char*) of_get_property(offset, BT_FLASH_DATA, &p_size);
	}
	if (p_data != NULL && p_size <= 20)
		memcpy(bt_bd_ram, p_data, p_size);

	return (bt_bd_ram);
}

void bt_export_bd_address(void)
{
	unsigned char cTemp[MAX_BT_SIZE];

	memcpy(cTemp, get_bt_bd_ram(), sizeof(cTemp));
	sprintf(bdaddress, "%02x:%02x:%02x:%02x:%02x:%02x",
			cTemp[0], cTemp[1], cTemp[2],
			cTemp[3], cTemp[4], cTemp[5]);

	printk(KERN_INFO "SET BD_ADDRESS=%s\n", bdaddress);
}
module_param_string(bdaddress, bdaddress, sizeof(bdaddress), S_IWUSR | S_IRUGO);
MODULE_PARM_DESC(bdaddress, "BT MAC ADDRESS");

