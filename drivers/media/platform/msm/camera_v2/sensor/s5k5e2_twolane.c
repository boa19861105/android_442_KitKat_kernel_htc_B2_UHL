/* Copyright (c) 2012-2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include "msm_sensor.h"
#include <linux/regulator/consumer.h>

#define S5K5E2_TWOLANE_SENSOR_NAME "s5k5e2_twolane"
DEFINE_MSM_MUTEX(s5k5e2_twolane_mut);

static struct msm_sensor_ctrl_t s5k5e2_twolane_s_ctrl;

struct msm_sensor_power_setting s5k5e2_twolane_power_setting[] = {
	{
		.seq_type = SENSOR_VREG,
		.seq_val = CAM_VDIG,
		.config_val = 1,
		.delay = 4,
	},
	{
		.seq_type = SENSOR_VREG,
		.seq_val = CAM_VANA,
		.config_val = 1,
		.delay = 4,
	},
	{
		.seq_type = SENSOR_GPIO,
		.seq_val = SENSOR_GPIO_VIO,
		.config_val = GPIO_OUT_HIGH,
		.delay = 4,
	},
	{
		.seq_type = SENSOR_CLK,
		.seq_val = SENSOR_CAM_MCLK,
		.config_val = 0,
		.delay = 8,
	},
	{
		.seq_type = SENSOR_GPIO,
		.seq_val = SENSOR_GPIO_RESET,
		.config_val = GPIO_OUT_HIGH,
		.delay = 2,
	},
	{
		.seq_type = SENSOR_I2C_MUX,
		.seq_val = 0,
		.config_val = 0,
		.delay = 0,
	},

};

struct msm_sensor_power_setting s5k5e2_twolane_power_down_setting[] = {
	{
		.seq_type = SENSOR_VREG,
		.seq_val = CAM_VDIG,
		.config_val = 1,
		.delay = 4,
	},
	{
		.seq_type = SENSOR_VREG,
		.seq_val = CAM_VANA,
		.config_val = 1,
		.delay = 4,
	},
	{
		.seq_type = SENSOR_GPIO,
		.seq_val = SENSOR_GPIO_VIO,
		.config_val = GPIO_OUT_LOW,
		.delay = 4,
	},

	{
		.seq_type = SENSOR_CLK,
		.seq_val = SENSOR_CAM_MCLK,
		.config_val = 0,
		.delay = 8,
	},
	{
		.seq_type = SENSOR_GPIO,
		.seq_val = SENSOR_GPIO_RESET,
		.config_val = GPIO_OUT_LOW,
		.delay = 2,
	},
	{
		.seq_type = SENSOR_I2C_MUX,
		.seq_val = 0,
		.config_val = 0,
		.delay = 0,
	},


};

static struct v4l2_subdev_info s5k5e2_twolane_subdev_info[] = {
	{
		.code   = V4L2_MBUS_FMT_SBGGR10_1X10,
		.colorspace = V4L2_COLORSPACE_JPEG,
		.fmt    = 1,
		.order    = 0,
	},
};

static const struct i2c_device_id s5k5e2_twolane_i2c_id[] = {
	{S5K5E2_TWOLANE_SENSOR_NAME, (kernel_ulong_t)&s5k5e2_twolane_s_ctrl},
	{ }
};

static int32_t msm_s5k5e2_twolane_i2c_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	return msm_sensor_i2c_probe(client, id, &s5k5e2_twolane_s_ctrl);
}

static struct i2c_driver s5k5e2_twolane_i2c_driver = {
	.id_table = s5k5e2_twolane_i2c_id,
	.probe  = msm_s5k5e2_twolane_i2c_probe,
	.driver = {
		.name = S5K5E2_TWOLANE_SENSOR_NAME,
	},
};

static struct msm_camera_i2c_client s5k5e2_twolane_sensor_i2c_client = {
	.addr_type = MSM_CAMERA_I2C_WORD_ADDR,
};

static const struct of_device_id s5k5e2_twolane_dt_match[] = {
	{.compatible = "htc,s5k5e2_twolane", .data = &s5k5e2_twolane_s_ctrl},
	{}
};

MODULE_DEVICE_TABLE(of, s5k5e2_twolane_dt_match);

static struct platform_driver s5k5e2_twolane_platform_driver = {
	.driver = {
		.name = "htc,s5k5e2_twolane",
		.owner = THIS_MODULE,
		.of_match_table = s5k5e2_twolane_dt_match,
	},
};

static const char *s5k5e2_twolaneVendor = "Samsung";
static const char *s5k5e2_twolaneNAME = "s5k5e2_twolane";
static const char *s5k5e2_twolaneSize = "5.0M";

static ssize_t sensor_vendor_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	ssize_t ret = 0;

	sprintf(buf, "%s %s %s\n", s5k5e2_twolaneVendor, s5k5e2_twolaneNAME, s5k5e2_twolaneSize);
	ret = strlen(buf) + 1;

	return ret;
}

static DEVICE_ATTR(sensor, 0444, sensor_vendor_show, NULL);

static struct kobject *android_s5k5e2_twolane;

static int s5k5e2_twolane_sysfs_init(void)
{
	int ret ;
	pr_info("s5k5e2_twolane:kobject creat and add\n");
	android_s5k5e2_twolane = kobject_create_and_add("android_camera1", NULL);
	if (android_s5k5e2_twolane == NULL) {
		pr_info("s5k5e2_twolane_sysfs_init: subsystem_register " \
		"failed\n");
		ret = -ENOMEM;
		return ret ;
	}
	pr_info("s5k5e2_twolane:sysfs_create_file\n");
	ret = sysfs_create_file(android_s5k5e2_twolane, &dev_attr_sensor.attr);
	if (ret) {
		pr_info("s5k5e2_twolane_sysfs_init: sysfs_create_file " \
		"failed\n");
		kobject_del(android_s5k5e2_twolane);
	}

	return 0 ;
}

static int32_t s5k5e2_twolane_platform_probe(struct platform_device *pdev)
{
	int32_t rc = 0;
	const struct of_device_id *match;
	match = of_match_device(s5k5e2_twolane_dt_match, &pdev->dev);
	rc = msm_sensor_platform_probe(pdev, match->data);
	return rc;
}

static int __init s5k5e2_twolane_init_module(void)
{
	int32_t rc = 0;
	pr_info("%s:%d\n", __func__, __LINE__);
	pr_info("s5k5e2_twolane_init_module");
	rc = platform_driver_probe(&s5k5e2_twolane_platform_driver,
		s5k5e2_twolane_platform_probe);
	if (!rc) {
		s5k5e2_twolane_sysfs_init();
		return rc;
	}
	pr_err("%s:%d rc %d\n", __func__, __LINE__, rc);
	return i2c_add_driver(&s5k5e2_twolane_i2c_driver);
}

static void __exit s5k5e2_twolane_exit_module(void)
{
	pr_info("%s:%d\n", __func__, __LINE__);
	if (s5k5e2_twolane_s_ctrl.pdev) {
		msm_sensor_free_sensor_data(&s5k5e2_twolane_s_ctrl);
		platform_driver_unregister(&s5k5e2_twolane_platform_driver);
	} else
		i2c_del_driver(&s5k5e2_twolane_i2c_driver);
	return;
}

int32_t s5k5e2_twolane_sensor_power_up(struct msm_sensor_ctrl_t *s_ctrl)
{
    int32_t status;
	
    pr_info("%s: +\n", __func__);

    s_ctrl->power_setting_array.power_setting = s5k5e2_twolane_power_setting;
    s_ctrl->power_setting_array.size = ARRAY_SIZE(s5k5e2_twolane_power_setting);
    status = msm_sensor_power_up(s_ctrl);
    pr_info("%s: -\n", __func__);
    return status;
}

int32_t s5k5e2_twolane_sensor_power_down(struct msm_sensor_ctrl_t *s_ctrl)
{
    int32_t status;
    int i = 0;
    int j = 0;
    int data_size;
    pr_info("%s: +\n", __func__);
    s_ctrl->power_setting_array.power_setting = s5k5e2_twolane_power_down_setting;
    s_ctrl->power_setting_array.size = ARRAY_SIZE(s5k5e2_twolane_power_down_setting);

    
    for(i = 0; i < s_ctrl->power_setting_array.size;  i++)
    {
        data_size = sizeof(s5k5e2_twolane_power_setting[i].data)/sizeof(void *);
        for (j =0; j < data_size; j++ )
        s5k5e2_twolane_power_down_setting[i].data[j] = s5k5e2_twolane_power_setting[i].data[j];
    }
    status = msm_sensor_power_down(s_ctrl);
    pr_info("%s: -\n", __func__);
    return status;
}

static int s5k5e2_twolane_read_fuseid(struct sensorb_cfg_data *cdata,
	struct msm_sensor_ctrl_t *s_ctrl)
{
	int32_t rc = 0;
	int i,j;
	uint16_t read_data = 0;
	uint16_t id_addr[6] = {0x0A04,0x0A05,0x0A06,0x0A07,0x0A08,0x0A09};
	static uint16_t id_data[6] = {0,0,0,0,0,0};
	static int first = true;
	static int32_t valid_page=-1;

	pr_info("%s called\n", __func__);
	if(first){
		first = false;
		for(j=4;j>=2;j--){
				rc = s_ctrl->sensor_i2c_client->i2c_func_tbl->i2c_write(s_ctrl->sensor_i2c_client, 0x0A00, 0x04, MSM_CAMERA_I2C_BYTE_DATA);
				if (rc < 0)
					pr_err("%s: i2c_write failed\n", __func__);
				rc = s_ctrl->sensor_i2c_client->i2c_func_tbl->i2c_write(s_ctrl->sensor_i2c_client, 0x0A02, j, MSM_CAMERA_I2C_BYTE_DATA);
				if (rc < 0)
					pr_info("%s: i2c_write failed\n", __func__);
				rc = s_ctrl->sensor_i2c_client->i2c_func_tbl->i2c_write(s_ctrl->sensor_i2c_client, 0x0A00, 0x01, MSM_CAMERA_I2C_BYTE_DATA);
				if (rc < 0)
					pr_info("%s: i2c_write failed\n", __func__);
				msleep(10);
				for (i=0; i<6; i++) {
					rc = s_ctrl->sensor_i2c_client->i2c_func_tbl->i2c_read(s_ctrl->sensor_i2c_client, id_addr[i], &read_data, MSM_CAMERA_I2C_BYTE_DATA);

					if (rc < 0)
						pr_err("%s: i2c_read 0x%x failed\n", __func__, i);

					id_data[i] = read_data & 0xff;
				}
				if (read_data)
					valid_page = j;
				if (valid_page!=-1)
					break;
		}
	}
	rc = s_ctrl->sensor_i2c_client->i2c_func_tbl->i2c_write(s_ctrl->sensor_i2c_client, 0x0A00, 0x04, MSM_CAMERA_I2C_BYTE_DATA);
	if (rc < 0)
		pr_err("%s: i2c_write failed\n", __func__);
	rc = s_ctrl->sensor_i2c_client->i2c_func_tbl->i2c_write(s_ctrl->sensor_i2c_client, 0x0A00, 0x00, MSM_CAMERA_I2C_BYTE_DATA);
	if (rc < 0)
		pr_err("%s: i2c_write failed\n", __func__);

	cdata->cfg.fuse.fuse_id_word1 = 0;
	cdata->cfg.fuse.fuse_id_word2 = id_data[3];
	cdata->cfg.fuse.fuse_id_word3 = id_data[4];
	cdata->cfg.fuse.fuse_id_word4 = id_data[5];

	pr_info("s5k5e2_twolane: fuse->fuse_id : 0x%x 0x%x 0x%x 0x%x\n",
		cdata->cfg.fuse.fuse_id_word1,
		cdata->cfg.fuse.fuse_id_word2,
		cdata->cfg.fuse.fuse_id_word3,
		cdata->cfg.fuse.fuse_id_word4);
	pr_info("%s: OTP Module vendor = 0x%x\n", __func__,id_data[0]);
	pr_info("%s: OTP LENS = 0x%x\n",          __func__,id_data[1]);
	pr_info("%s: OTP Sensor Version = 0x%x\n",__func__,id_data[2]);

	return rc;
}

static struct msm_sensor_fn_t s5k5e2_twolane_sensor_func_tbl = {
	.sensor_config = msm_sensor_config,
	.sensor_power_up = s5k5e2_twolane_sensor_power_up,
	.sensor_power_down = s5k5e2_twolane_sensor_power_down,
	.sensor_match_id = msm_sensor_match_id,
	.sensor_i2c_read_fuseid = s5k5e2_twolane_read_fuseid,
};

static struct msm_sensor_ctrl_t s5k5e2_twolane_s_ctrl = {
	.sensor_i2c_client = &s5k5e2_twolane_sensor_i2c_client,
	.power_setting_array.power_setting = s5k5e2_twolane_power_setting,
	.power_setting_array.size = ARRAY_SIZE(s5k5e2_twolane_power_setting),
	.msm_sensor_mutex = &s5k5e2_twolane_mut,
	.sensor_v4l2_subdev_info = s5k5e2_twolane_subdev_info,
	.sensor_v4l2_subdev_info_size = ARRAY_SIZE(s5k5e2_twolane_subdev_info),
	.func_tbl = &s5k5e2_twolane_sensor_func_tbl,
};

module_init(s5k5e2_twolane_init_module);
module_exit(s5k5e2_twolane_exit_module);
MODULE_DESCRIPTION("s5k5e2_twolane");
MODULE_LICENSE("GPL v2");
