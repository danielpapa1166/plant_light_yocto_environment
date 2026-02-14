from datetime import datetime



DATA_FILE_PATH = "/home/pbd2bp/projects/plant_light/log/env_data_log.csv"

def get_data(): 
    dates = []
    values1 = []
    values2 = []
    with open(DATA_FILE_PATH, "r") as f:
        for line in f:
            parts = line.strip().split(",")
            if len(parts) != 3:
                continue

            try:
                dates.append(datetime.strptime(parts[0], "%Y-%m-%d %H:%M:%S"))
                values1.append(float(parts[1]))
                values2.append(float(parts[2]))
            except ValueError:
                continue


    return [dates, values1, values2]
