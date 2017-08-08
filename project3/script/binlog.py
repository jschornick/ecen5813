#!/usr/bin/python

import sys
import struct
from datetime import datetime

DEBUG=0

if len(sys.argv) < 2:
    print "Usage: dump.py [filename]"
    sys.exit(1)

logfile = open(sys.argv[1], "rb")

magicStr = "Binlog_Start"
magicPos = 0

datastart = 0

class LogType:
  LD_NULL = 0
  LD_DATA = 1
  LD_INT = 2
  LD_STR = 3
  LD_NVAL = 4

LogIds = [
  "LOGGER_INITIALIZED",
  "SYSTEM_INITIALIZED",
  "SYSTEM_HALTED",
  "GPIO_INITIALIZED",
  "SPI_INITIALIZED",
  "INFO",
  "WARNING",
  "ERROR",
  "PROFILING_STARTED",
  "PROFILING_COMPLETED",
  "PROFILING_RESULT",
  "NRF_ADDRESS",
  "DATA_RECEIVED",
  "DATA_ANALYSIS_STARTED ",
  "DATA_ANALYSIS_COMPLETED",
  "DATA_TOTAL_COUNT",
  "DATA_ALPHA_COUNT",
  "DATA_NUMERIC_COUNT",
  "DATA_PUNCTUATION_COUNT",
  "DATA_MISC_COUNT",
  "HEARTBEAT" ]

print "Magic: ",

while True:
    if not datastart:
        byte = logfile.read(1)
        if byte != magicStr[magicPos]:
            if(magicPos > 0):
                print
                print "Magic: ",
            magicPos = 0
            continue
        else:
            magicPos += 1
            print byte,
            if magicPos >= len(magicStr):
                print
                datastart = 1
            continue

    header = struct.unpack('<BBIII', logfile.read(14))
    id = header[0]
    type = header[1]
    time = header[2]
    ms = header[3]
    length = header[4]

    if DEBUG:
        print "Header: ", id
        print "  Id: ", id
        print "  Type: ", type
        print "  Time: ", time
        print "  Millis: ", ms
        print "  Length: ", length

    if id > len(LogIds):
        print "Bad id: ", id
        continue

    data = logfile.read(length)
    print datetime.fromtimestamp(time).strftime('%Y-%m-%d %H:%M:%S'),
    print '[{}]'.format(LogIds[id]),
    if type == LogType.LD_NULL:
        print
    elif type == LogType.LD_DATA:
        print " ".join(["0x{:02x}".format(ord(b)) for b in data])
    elif type == LogType.LD_INT:
        val = struct.unpack('<I', data)[0]
        print val
    elif type == LogType.LD_STR:
        print data.decode()
    elif type == LogType.LD_NVAL:
        val = struct.unpack('<I', data[0:4])[0]
        name = data[4:].decode()
        print name, "=", val




