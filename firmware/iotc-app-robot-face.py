# Copyright (c) Microsoft. All rights reserved.
# Licensed under the MIT license.

import iotc
from iotc import IOTConnectType, IOTLogLevel
from random import randint
import serial  # 引用pySerial模組
import json
import time
 
COM_PORT = 'COM3'    # 指定通訊埠名稱
BAUD_RATES = 9600    # 設定傳輸速率
ser = serial.Serial(COM_PORT, BAUD_RATES)   # 初始化序列通訊埠

deviceId = "inmoov-01"
scopeId = ""
deviceKey = ""

iotc = iotc.Device(scopeId, deviceKey, deviceId, IOTConnectType.IOTC_CONNECT_SYMM_KEY)
iotc.setLogLevel(IOTLogLevel.IOTC_LOGGING_API_ONLY)

gCanSend = False
gCounter = 0

def onconnect(info):
  global gCanSend
  print("- [onconnect] => status:" + str(info.getStatusCode()))
  if info.getStatusCode() == 0:
     if iotc.isConnected():
       gCanSend = True

def onmessagesent(info):
  print("\t- [onmessagesent] => " + str(info.getPayload()))

def oncommand(info):
  print("- [oncommand] => " + info.getTag() + " => " + str(info.getPayload()))
  d = info.getPayload().strip('\"')
  dstr = bytes(d+'\n', encoding= 'utf-8')
  print(dstr)
  ser.write(dstr) 
  if d=='talk':
    iotc.sendTelemetry('{"mouthAngle": 180}')
    time.sleep(1)
    iotc.sendTelemetry('{"mouthAngle": 90}')
  if 'eyesLRAngle' in d :
    iotc.sendTelemetry('{"eyePosition": '+d[11:]+'}')
    #time.sleep(1)
    

  if 'eyesUpDownAngle' in d :
    iotc.sendTelemetry('{"eyePosition": '+d[15:]+'}')

def onsettingsupdated(info):
  print("- [onsettingsupdated] => " + info.getTag() + " => " + info.getPayload())
  d = json.loads(info.getPayload())
  dstr = bytes('{"Buzzer":'+str(d['value'])+'}\n', encoding= 'utf-8')
  #dstr = b'{"Buzzer":'+d['value']+'}\n'
  
  if info.getTag() == "Buzzer":
    print(dstr)
    ser.write(dstr)
  if info.getTag() == "LCD_Msg":
    dstr = bytes('{"LCD_Msg":["'+str(d['value'])+'"]}\n', encoding= 'utf-8')
    print(dstr)
    ser.write(dstr)


iotc.on("ConnectionStatus", onconnect)
iotc.on("MessageSent", onmessagesent)
iotc.on("Command", oncommand)
iotc.on("SettingsUpdated", onsettingsupdated)


iotc.connect()

while iotc.isConnected():
  iotc.doNext() # do the async work needed to be done for MQTT
  if gCanSend == True:
    if gCounter % 20 == 0:
      gCounter = 0
      #print(iotc.getDeviceSettings())
      #print("Twins Update..")
    gCounter += 1
ser.close()    # 清除序列通訊物件
