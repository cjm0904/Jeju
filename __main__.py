from multiprocessing import Process
import recvData, jejuGateway

msgGw = 'Gateway is start'
msgTime = 'receiving Time'

if __name__ == '__main__':
    p = Process(target=jejuGateway.Gateway, args=(msgGw,))
    q = Process(target=recvData.recvTime, args=(msgTime,))
    p.start()
    q.start()
    p.join()
    q.join()
    pass

