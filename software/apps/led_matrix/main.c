// LED Matrix app
//
// Display messages on the LED matrix

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


#include "nrf_delay.h"

#include "led_matrix.h"
#include "microbit_v2.h"
#include "lsm303agr.h"
#include "nrf_twi_mngr.h"

NRF_TWI_MNGR_DEF(twi_mngr_instance, 1, 0);

int main(void) {
  printf("Board started!\n");

  //initialize led_matrix and rng peripheral
  led_matrix_init();
  rng_init();

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
  
  // call other functions here


  // loop forever
  while (1) {
    //Acceleration
    accel = lsm303agr_read_accelerometer();
    printf("Accel_X: %f     Accel_Y: %f     Accel_Z: %f\n", accel.x_axis, accel.y_axis, accel.z_axis);
    
    //Tilt
    phi = get_phi(accel);
    printf("Tilt angle phi: %f\n", phi);

    theta = get_theta(accel);
    printf("Tilt angel theta: %f\n", theta);
    nrf_delay_ms(1000);
  }
}

