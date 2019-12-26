#!/usr/bin/env python3
import serial
import time
import subprocess
import re

port = '/dev/ttyACM0'
rate = 9600

s1 = serial.Serial(port, rate)
s1.flushInput()
while True:
    try:
        r = subprocess.run(["ansible", "-i", "~/hosts", "-a", "vcgencmd measure_temp", "pi"], capture_output=True, text=True)
        r_items = [ it.strip() for it in re.split(r'\||\n', r.stdout.strip()) ]
        ips = r_items[0::4]
        temps = [ re.split(r'=|\.', it)[1] for it in r_items[3::4] ]
        tempdict = dict(zip(ips, temps))
        temps = { key:tempdict[key] for key in sorted(tempdict.keys()) }

        print(temps)
        message = 'A' + ','.join(temps.values()) + 'B'

        s1.write(message.encode("ascii", "ignore"))
    except:
        pass

    time.sleep(10)