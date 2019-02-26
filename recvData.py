import paho.mqtt.client as mqtt
import pymysql as sql

brkIp = '45.120.69.86'
brkPort = 1883
brkKeepAlive = 60

conn = sql.connect(host='192.168.0.49', user='root', password='ziumks', db='jeju', charset='utf8')


def on_connect(client, userdata, flags, rc):
    print("connected with result code " + str(rc))
    client.subscribe("sf/tsite/cntr001")


def on_message(client, userdata, msg):
    data = str(msg.payload.decode('utf8'))
    print(data)
    print(eval(data)['t1'])

    if 't1' in eval(data).keys():
        print("있다")
        qry = 'update response set period = (%s) from '

        param = 250

    if 'abc' not in eval(data).keys():
        print("없다")
# 시간조정, 공조기 조정 등 들어가야함.

    if 'hm' in eval(data).keys(): # 습도 조절
        if not ser.is_open:
            ser.open()
        humidity = eval(data)['hm']
        hmData = [0,0,0,0]
        ser.write(hmData)
        ser.close()


def recvTime(msg):
    print(msg)
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message

    client.connect(brkIp, brkPort, brkKeepAlive)
    client.loop_forever()
