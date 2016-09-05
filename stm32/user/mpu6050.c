#include <stdio.h>
#include "mpu6050.h"
#include "eMPL/inv_mpu.h"
#include "eMPL/inv_mpu_dmp_motion_driver.h"

#define DEFAULT_MPU_HZ (20)

struct platform_data_s {
    signed char orientation[9];
};

static struct platform_data_s gyro_pdata = {
    .orientation = { 1, 0, 0,
                     0, 1, 0,
                     0, 0, 1}
};

static unsigned short inv_row_2_scale(const signed char *row)
{
	unsigned short b;

	if (row[0] > 0)
		b = 0;
	else if (row[0] < 0)
		b = 4;
	else if (row[1] > 0)
		b = 1;
	else if (row[1] < 0)
		b = 5;
	else if (row[2] > 0)
		b = 2;
	else if (row[2] < 0)
		b = 6;
	else
		b = 7;		// error
	return b;
}

unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx)
{

    unsigned short scalar;

    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */

    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;

    return scalar;
}

struct rx_s {
	    unsigned char header[3];
	        unsigned char cmd;
};

struct hal_s {
    unsigned char lp_accel_mode;
    unsigned char sensors;
    unsigned char dmp_on;
    unsigned char wait_for_tap;
    volatile unsigned char new_gyro;
    unsigned char motion_int_mode;
    unsigned long no_dmp_hz;
    unsigned long next_pedo_ms;
    unsigned long next_temp_ms;
    unsigned long next_compass_ms;
    unsigned int report;
    unsigned short dmp_features;
    struct rx_s rx;
};
static struct hal_s hal = {0};

void mpu6050_init( void ){
	int ret;
	unsigned short gyro_rate;
	unsigned short gyro_fsr;
	unsigned char accel_fsr;

	printf( "mpu_init...\r\n" );
	ret = mpu_init( NULL );
	if( ret ){
		printf( "Could not initialize mpu6050\r\n" );
	}

	printf( "mpu_set_sensors...\r\n" );
	ret = mpu_set_sensors( INV_XYZ_GYRO | INV_XYZ_ACCEL );
	printf( "mpu_configure_fifo...\r\n" );
	ret = mpu_configure_fifo( INV_XYZ_GYRO | INV_XYZ_ACCEL );
	printf( "mpu_set_sample_rate...%d\r\n", DEFAULT_MPU_HZ );
	ret = mpu_set_sample_rate( DEFAULT_MPU_HZ );

	ret = mpu_get_sample_rate( &gyro_rate );
	ret = mpu_get_gyro_fsr( &gyro_fsr );
	ret = mpu_get_accel_fsr( &accel_fsr );
	printf( "gyro_rate = %u\r\n", gyro_rate );
	printf( "gyro_fst  = %u\r\n", gyro_fsr );
	printf( "accel_fst = %u\r\n", accel_fsr );

	ret = dmp_load_motion_driver_firmware();
	if( ret ){
		printf( "Could not load firmware\r\n" );
	}
	dmp_set_orientation(
			inv_orientation_matrix_to_scalar(gyro_pdata.orientation));
	hal.dmp_features = DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
		DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
		DMP_FEATURE_GYRO_CAL;

	dmp_enable_feature(hal.dmp_features);
	dmp_set_fifo_rate(DEFAULT_MPU_HZ);
	mpu_set_dmp_state(1);
}
