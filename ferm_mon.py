import os
import glob
import time
import sys
from time import gmtime, strftime
import httplib
import urllib
import RPi.GPIO as GPIO

OFF = 0
ON = 1

HEATER_PIN = 16

HEATER_STATE = OFF

SET_POINT = 0

def read_temp_raw(device_file):
    f = open(device_file, 'r')
    lines = f.readlines()
    f.close()
    return lines

def read_temp(device_file):
    lines = read_temp_raw(device_file)

    while lines[0].strip()[-3:] != 'YES':
        time.sleep(0.5)
        lines = read_temp_raw(device_file)

    while "YES" not in lines[0]:
        time.sleep(0.5)
        lines = read_temp_raw(device_file)

    equals_pos = lines[1].find('t=')

    if equals_pos != -1:
        temp_string = lines[1][equals_pos+2:]
        temp_c = float(temp_string) / 1000.0

    return temp_c

def update_thingspeak(wort_temp, air_temp, state):

    params = urllib.urlencode({'field1': wort_temp, 'field2': air_temp, 'field3': state, 'key':'N7F0533YVMWFYCET'})
    headers = {"Content-type": "application/x-www-form-urlencoded","Accept": "text/plain"}
    conn = httplib.HTTPConnection("api.thingspeak.com:80")

    try:
        conn.request("POST", "/update", params, headers)
        response = conn.getresponse()
        # print response.status, response.reason
        data = response.read()
        conn.close()
        return response

    except:
        return "Exception"

def init_heater():

    global HEATER_STATE

    HEATER_STATE = OFF
    GPIO.output(HEATER_PIN, GPIO.LOW)
    print "Heater Initialised"

def heater_check( temp ):

    global HEATER_STATE

    if HEATER_STATE == OFF:
        if ( temp < ( SET_POINT - 0.1 ) ):
            print "switching heater on, temp: "+str(temp)
            switch_heater(ON)
            HEATER_STATE = ON

    elif HEATER_STATE == ON:
        if ( temp > ( SET_POINT + 0.1 ) ):
            print "switching heater off, temp: "+str(temp)
            switch_heater(OFF)
            HEATER_STATE = OFF

def switch_heater( state ):

    if state == ON:
        GPIO.output(HEATER_PIN, GPIO.HIGH)
    elif state == OFF:
        GPIO.output(HEATER_PIN, GPIO.LOW)


def main():

    global SET_POINT
    global HEATER_STATE

    # Get input from user
    input_value = raw_input('Enter a set point in degrees C: ')
    SET_POINT = float(input_value)

    # Setup the IO
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(HEATER_PIN, GPIO.OUT)
    GPIO.setwarnings(False)

    # Init the heater
    init_heater()

    # Load kernel modules for 1 wire
    os.system('modprobe w1-gpio')
    os.system('modprobe w1-therm')

    # Location of sensor outputs
    base_dir = '/sys/bus/w1/devices/'

    air_device_folder = glob.glob(base_dir + '28*')[0]
    wort_device_folder = glob.glob(base_dir + '28*')[1]

    wort_device_file = wort_device_folder + '/w1_slave'
    air_device_file = air_device_folder + '/w1_slave'

    while True:
        try:
            wort_temp = read_temp(wort_device_file)
            air_temp = read_temp(air_device_file)

            heater_check(wort_temp)

            current_time = strftime("%Y-%m-%d %H:%M:%S", gmtime())

            response = update_thingspeak(wort_temp, air_temp, HEATER_STATE)

            if ( response != "Exception" ):
                log_line = current_time+","+str(air_temp)+","+str(wort_temp)+","+str(response.status)+" "+response.reason+"\n"
            else:
                log_line = current_time+","+str(air_temp)+","+str(wort_temp)+",Failed Connection"+"\n"

            f = open('temp_monitor.log', 'a')
            f.write(log_line)
            f.close
            # print log_line
            time.sleep(60)
        except (KeyboardInterrupt, SystemExit):
            print "Exiting..."
            switch_heater( ON )
            GPIO.cleanup()
            sys.exit(0)


if __name__ == "__main__":
            main()
