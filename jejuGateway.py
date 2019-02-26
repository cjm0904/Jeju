import serial, time, json, mqtt
import pymysql as sql


co2 = 0
temperature = 0
humidity = 0
i = 0
ser = serial.Serial("/dev/ttyUSB1", 9600, timeout=1)
conn = sql.connect(host='192.168.0.49', user='root', password='ziumks', db='jeju', charset='utf8')


def Gateway(msg):
    print(msg)
    while True:
        try:
            if not ser.is_open:
                ser.open()
            data = ser.readline().decode('utf-8')
            if data.__len__() != 34:
                pass
            else:
                rawCO2 = (data[0:8])
                rawTemp = (data[10:20])
                rawHumid = (data[22:30])
                co2 = (int(rawCO2[0:2]) - 0x30) * 1000 + (int(rawCO2[2:4]) - 0x30) * 100 + (int(
                    rawCO2[4:6]) - 0x30) * 10 + (int(rawCO2[6:8]) - 0x30)
                # temperature =  (int(rawTemp[0:2])-0x30)*100 + (int(rawTemp[2:4])-0x30)*10 + (int(rawTemp[4:6])-0x30) * 1 + (int(rawTemp[8:10])-0x30) * 0.1
                temperature = (int(rawTemp[2:4]) - 0x30) * 10 + (int(rawTemp[4:6]) - 0x30) * 1 + (int(rawTemp[8:10]) - 0x30) * 0.1
                humidity = (int(rawHumid[0:2]) - 0x30) * 10 + (int(rawHumid[2:4]) - 0x30) * 1 + (int(rawHumid[6:8]) - 0x30) * 0.1

                #            print("co2 : " + str(co2))
                #            print("temperature : " + str(temperature))
                #            print("humid : " + str(humidity))

                result = {'id': i, 't': round(time.time() * 1000), 't1': temperature, 't2': temperature, 'hm': humidity,
                          'co': co2, 'c': 0000}
                # print(result)

                try:
                    mqtt.mqClient.publish(topic='sf/tsite/cntr001', payload=json.dumps(result), qos=1)
                    i = i + 1
                except ConnectionError as e:
                    print("error : " + str(e))
                    pass
            ser.close()
        except KeyboardInterrupt:
            ser.close()
            pass

        # 조회 시작
        with conn.cursor() as cursor:
            qry = 'select ' # 시간 조회문
            cursor.execute(qry)
            rstTime = cursor.fetchone()
            time.sleep(rstTime)