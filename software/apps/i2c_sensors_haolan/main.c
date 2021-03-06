// I2C sensors app
//
// Read from I2C accelerometer/magnetometer on the Microbit

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "nrf_delay.h"
#include "nrf_twi_mngr.h"

#include "microbit_v2.h"
#include "lsm303agr.h"

// Global variables
NRF_TWI_MNGR_DEF(twi_mngr_instance, 1, 0);

int main(void) {
  printf("Board started!\n");

  // Initialize I2C peripheral and driver
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  i2c_config.scl = I2C_SCL;
  i2c_config.sda = I2C_SDA;
  i2c_config.frequency = NRF_TWIM_FREQ_100K;
  nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);

  // Initialize the LSM303AGR accelerometer/magnetometer sensor
  lsm303agr_init(&twi_mngr_instance);

  //float temp;
  lsm303agr_measurement_t accel;
  //lsm303agr_measurement_t mag;
  float phi;
  float theta;
  
  // Loop forever
  while (1) {
    // Print output
    //TODO: implement me!
    
    //Temperature
    //temp = lsm303agr_read_temperature();
    //printf("Temp: %f\n", temp);

    //Acceleration
    accel = lsm303agr_read_accelerometer();
    printf("Accel_X: %f     Accel_Y: %f     Accel_Z: %f\n", accel.x_axis, accel.y_axis, accel.z_axis);
    
    //Tilt
    phi = get_phi(accel);
    printf("Tilt angle phi: %f\n", phi);

    theta = get_theta(accel);
    printf("Tilt angel theta: %f\n", theta);
    
   
    //magnetometer
    // mag = lsm303agr_read_magnetometer();
    //printf("Mag_X: %f       Mag_Y: %f       Mag_Z: %f\n\n", mag.x_axis, mag.y_axis, mag.z_axis);
    
    
    nrf_delay_ms(1000);
  }
}

