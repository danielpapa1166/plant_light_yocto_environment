#include <fstream>
#include <iomanip>


void plant_light_write_log(std::string f_datetime, float f_temp, float f_rel_hum) {
    std::ofstream file( 
        PLANT_LIGHT_ENV_MEAS_DATA_LOG_FILE, 
        std::ios::app);

    if (!file) {
        return ;
    }

    file << f_datetime << ","
         << std::fixed << std::setprecision(6)
         << f_temp << "," << f_rel_hum << "\n";

    file.flush();
}