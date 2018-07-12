from bottle import post, run, request
import json
import socket, struct
import Queue
from threading import Thread
import multiprocessing


class Span:
    def __init__(self):
        self.id = -1
        self.name = ''
        self.timestamp = -1
        self.traceId = -1
        self.duration = -1
        self.parentId = -1
        self.annotations = []

class EndPoint:
    def __init__(self):
        self.ipv4 = -1
        self.port = -1
        self.serviceName = ''

class Annotation:
    def __init__(self):
        self.endpoint = EndPoint()
        self.timestamp = -1
        self.value = ""

class JsonEncoder:
    spanAttributeList = ['name', 'id', 'timestamp', 'traceId', 'duration']
    annotationAttributeList = ['timestamp', 'value']

    @staticmethod
    def fromRawToEndPoint( endPointRawData):
        endpoint = EndPoint()
        endpoint.ipv4 = socket.inet_ntoa(struct.pack('!i', endPointRawData['ipv4']))
        endpoint.port = endPointRawData['port']
        endpoint.serviceName = endPointRawData['serviceName'].encode('ascii', 'ignore')
        return endpoint

    @staticmethod
    def fromRawToAnnotation( annotationRawData ):
        annotation = Annotation()
        for attribute in JsonEncoder.annotationAttributeList:
            value = annotationRawData[attribute].encode('ascii', 'ignore') if isinstance( annotationRawData[attribute], basestring) else annotationRawData[attribute]
            setattr(annotation, attribute, value)
        annotation.endpoint = JsonEncoder.fromRawToEndPoint( annotationRawData['endpoint'])
        return annotation

    @staticmethod
    def fromRawToSpan( spanRawData ):
        span = Span()
        for attribute in JsonEncoder.spanAttributeList:
            value = spanRawData[attribute].encode('ascii', 'ignore') if isinstance( spanRawData[attribute], basestring) else spanRawData[attribute]
            setattr(span, attribute, value)

        span.parentId = spanRawData.get('parentId', -1)

        for annotationData in spanRawData['annotations']:
            annotation = JsonEncoder.fromRawToAnnotation( annotationData )
            span.annotations.append(annotation)

        return span

    @staticmethod
    def fromJson( data ):
        spans = []
        rawData = json.JSONDecoder().decode(data)
        for spanRawData in rawData:
            spans.append(JsonEncoder.fromRawToSpan( spanRawData ))

        return spans

class ZipkinStubServer(object):

    def __init__(self, port = 9411):
        self._port = port
        self._messageQueue = Queue.Queue()
        self._running = False
        self._thread = None
        self.outQueue = None
        self.startEvent = None
        post('/api/v1/spans')(self.handleSpans)

    def __del__(self):
        self._running = False
        self._messageQueue.put(('',''))
        self._thread.join()

    def start(self):
        self._running = True
        self._thread = Thread(None, self.eventLoop, 'event loop', (), {})
        self._thread.start()
        self.startEvent.set()
        run(host = 'localhost', port=self._port)

    def handleSpans(self):
        contentType = request.headers.get('Content-Type')
        self._messageQueue.put((contentType, request.body.buf))

    def eventLoop(self):
        while self._running == True:
            try:
                message = self._messageQueue.get(True, 2)
                if message[0] == 'application/json':
                    spans = JsonEncoder.fromJson(message[1])
                    self.outQueue.put( spans )
            except Queue.Empty:
                pass

    @staticmethod
    def spawn(outQueue, startEvent):
        server = ZipkinStubServer()
        server.outQueue = outQueue
        server.startEvent = startEvent
        server.start()

class ProcessGuard:
    def __init__(self, entryPoint, *args):
        self._serverProcess = None
        self._entryPoint = entryPoint
        self._args = args

    def __enter__(self):
        self._serverProcess = multiprocessing.Process( target = self._entryPoint, args=self._args )
        self._serverProcess.start()

    def __exit__(self, exc_type, exc_val, exc_tb):
        pass



