#include "plant_light_ICC_device_def.hpp"
#include "plant_light_AHT10_sensor.hpp"



AHT10_Sensor::AHT10_Sensor() {
    _icc_bus_instance.single_write(_dev_icc_addr, 0xE1);
}


void AHT10_Sensor::new_measurement() {
	static uint8_t tx_buf[3] = {
		0xac, 
		0x33, 
		0x00
	};
	// trigger new measurement
	//i2c_bus_ctrl_start_write_data(0x38, tx_buf, 1);
	int res = _icc_bus_instance.batch_write(
		_dev_icc_addr, 
		tx_buf, 
		sizeof(tx_buf)); 

}

void AHT10_Sensor::fetch_data() {

	static uint8_t rx_buf[6] = {0};

	// fetch data
	//uint8_t res /*= i2c_bus_ctrl_start_read_data(0x38, rx_buf, 6)*/;  //HAL_I2C_Master_Receive(m_i2c_handle_sp, 0x38 << 1, rx_buf, 6, HAL_MAX_DELAY);

	int res = _icc_bus_instance.batch_read(
		_dev_icc_addr, 
		rx_buf, 
		sizeof(rx_buf)); 

	// decode data from the receive buffer
	uint32_t raw_data_temp = ((rx_buf[3] & 0x0F) << 16) | (rx_buf[4] << 8) | (rx_buf[5]);
	uint32_t raw_data_rel_hum = (rx_buf[1] << 12) | (rx_buf[2] << 4) | ((rx_buf[3] >> 4) & 0x0F);

	// convert the data to floating point:
	_temperature_f = ( (((float) raw_data_temp) / ((float) 0x000FFFFF - 1) ) * 200 ) - 50;
	_rel_hum_f = ( (((float) raw_data_rel_hum) / ((float) 0x000FFFFF - 1) ) * 100 );
}