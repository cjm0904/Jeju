import paho.mqtt.client as mqtt
import myMqtt as my
import pymysql as sql
import controller as ctrlr
import time

brkIp = my.brkIp
brkPort = my.Port
brkKeepAlive = my.brkKeepAlive
tls_arr = my.tls_arr

conn = sql.connect(host = '127.0.0.1', user = 'root', password = 'ziumks', db = 'jeju', charset = 'utf8')

def on_connect(client, userdata, flags, rc):
    print("connected with result code " + str(rc))
    client.subscribe("cdns/T12/#")
    client.subscribe("set/T12/#")


def on_message(client, userdata, msg):
    data = str(msg.payload.decode('utf8'))

    if 'period' in eval(data).keys():
        print("check period")
        qry = 'update response set period = (%s)'
        param = data['period']

        try:
            with conn.cursor() as cursor:
                cursor.execute(qry, param)
                conn.commit()
        except TypeError as e:
            print(e)
    
    if 'abc' not in eval(data).keys():
        pass

def cdnsCallback(client, userdata, msg):
    data = str(msg.payload.deccode('utf8'))
    myTopic = msg.topic
    deviceNo = myTopic.split('/')[2]
    
    if 'Q' in eval(data).keys():
        sendTopic = 'cdnc/' + myTopic[1] + '/' + myTopic[2]

        qry = 'select MAX(time) from startack where deviceNo = %s'
        with conn.cursor(sql.cursors.DictCursor) as cursor:
            cursor.execute(qry, deviceNo)
            rows = curs.fetchone()
            now = time.time()
            result = {'t':now, 'cdn':'ON', 'ackt':rows['time']}
            my.mqClient.publish(topic=sendTopic, payload=json.dumps(result),qos=0)
    pass


def setCallback(client, userdata, msg):
    data = str(msg.payload.decode('utf8'))
    myTopic = msg.topic
    
    deviceNo = myTopic.split('/')[2]
    deviceAddr = '00'

    if 'buzzer' in eval(data).keys():
        ctrlr.ctlIndoorUnit(deviceNo, deviceAddr, json.dumps(data))

    else:
        ctrlr.AhuUnit(deviceNo, deviceAddr, json.dumps(data))
    pass


def recvData():
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message

    client.message_callback_add("cdns/T12/#",cdnsCallback)
    client.message_callback_add("set/T12/#",setCallback)

    client.connect(brkIp, brkPort, brkKeepAlive)
    client.loop_forever()

