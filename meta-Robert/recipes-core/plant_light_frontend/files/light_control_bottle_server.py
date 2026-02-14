from bottle import Bottle, run, template, request, response, static_file, TEMPLATE_PATH
from datetime import datetime
from udp_handler import UdpHandler
import struct 
import os 
import time 

from light_control_utils import *
from light_control_file_handler import * 


UDP_COMMAND_HEADER = b'\x52\x50'
UDP_COMMAND_TPYE_LIGHT = b'\x4C'
UDP_COMMAND_SCHEDULE_SWITCH = b'\x53'
UDP_COMMAND_SET_PWM = b'\x4D'
UDP_COMMAND_GET_ENV_DATA = b'\x61'


app = Bottle()
udpHandler = UdpHandler(
    ip = "0.0.0.0", 
    port = 8888
)

# add base directory: 
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
TEMPLATE_PATH.insert(0, os.path.join(BASE_DIR, "views"))

# Home page
@app.get('/')
def index():
    return template('index', name="Robert", message=None)


@app.route('/static/<filepath:path>')
def static_files(filepath):
    root_path = os.path.join(BASE_DIR, 'static')
    return static_file(filepath, root=root_path)


@app.route('/resources/<filename>')
def serve_resources(filename): 
    return static_file(filename, root=os.path.join(BASE_DIR, 'resources'))

# Handle form submit
@app.post('/submit')
def submit():
    msg = request.forms.get('msg')
    udpHandler.send_command(msg.encode('utf-8'))
    return template('index', name="Robert", message=msg)


# Light control routes
@app.route('/light/<state>')
def light_control(state):
    udp_command = UDP_COMMAND_HEADER + UDP_COMMAND_TPYE_LIGHT 
    if state == 'on':
        # TODO: Replace with GPIO control if needed
        print("Light turned ON")
        udpHandler.send_command(udp_command + b'\x01')
        return "Light is ON"
    elif state == 'off':
        print("Light turned OFF")
        udpHandler.send_command(udp_command + b'\x00')
        return "Light is OFF"
    else:
        return "Unknown command"

@app.get('/server_data_cyclic_update')
def server_data_cyclic_update(): 

    total_tx_payload = UDP_COMMAND_HEADER + \
        UDP_COMMAND_GET_ENV_DATA

    ret = udpHandler.send_command(total_tx_payload)
    temperature = 0 
    rel_hum = 0

    if(len(ret) >= 11): 
        temperature = u8_array_to_float_le(ret[3:7])
        rel_hum = u8_array_to_float_le(ret[7:11])
    

    print(f"Len: {len(ret)}, Temperature: {temperature}, relative humitidy: {rel_hum}")

    data = {
        "temperature": temperature,
        "relative_humidity": rel_hum
    }

    response.content_type = 'application/json'
    return data

@app.post('/submit_datetime')
def submit_datetime():
    data = request.json
    date_str = data.get("date")
    time_str = data.get("time")
    on_or_off = data.get("on_or_off")

    # Combine into a Python datetime
    dt = datetime.strptime(         \
        date_str                    \
        + " "                       \
        + time_str,                 \
        "%Y-%m-%d %H:%M")

    # pach to bytes: 
    datetime_payload_bytes = struct.pack(
        ">HBBBBB",     # big-endian
        dt.year,
        dt.month,
        dt.day,
        dt.hour,
        dt.minute,
        dt.second
    )

    udp_command = UDP_COMMAND_HEADER + \
        UDP_COMMAND_SCHEDULE_SWITCH

    total_tx_payload                                \
        = udp_command                               \
        + datetime_payload_bytes                    \
        + (b'\x01' if on_or_off else b'\x00')

    print(f"Total payload: {total_tx_payload}")

    ret = udpHandler.send_command(total_tx_payload)
    print(f"ret: {ret}")


    return {"message": f"Received {dt}"}


@app.post('/update_slider')
def update_slider():
    data = request.json
    slider_value = int(data.get('value'))

    if(slider_value > 100): 
        slider_value = 100
    if(slider_value < 0): 
        slider_value = 0 

    slider_byte = slider_value.to_bytes(1, byteorder='big') 

    total_tx_payload                                \
        = UDP_COMMAND_HEADER                        \
        + UDP_COMMAND_SET_PWM                       \
        + slider_byte                               \

    ret = udpHandler.send_command(total_tx_payload)

    return {'message': f'Slider value received'}

@app.route("/fetch_sensor_data")
def sensor_data():
    response.content_type = "application/json"

    [dates, temperature, relative_humidity] = get_data()

    data = {
        "timestamps": [date.isoformat() for date in dates],
        "temperature": temperature,
        "relative_humidity": relative_humidity
    }

    
    return data


run(app, host='0.0.0.0', port=8080)
