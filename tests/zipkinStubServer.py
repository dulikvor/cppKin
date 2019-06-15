from bottle import post, Bottle, request
import json
import socket, struct
import queue
from threading import Thread
import server

class Span:
    def __init__(self):
        self.id = -1
        self.name = ''
        self.timestamp = -1
        self.traceId = ''
        self.duration = ''
        self.parentId = ''
        self.annotations = []
        self.binaryAnnotations = []

class EndPoint:
    def __init__(self):
        self.ipv4 = ''
        self.port = -1
        self.serviceName = ''

class Annotation:
    def __init__(self):
        self.endpoint = EndPoint()
        self.timestamp = -1
        self.value = ""

class BinaryAnnotation:
    def __init__(self):
        self.endpoint = EndPoint()
        self.key = ""
        self.value = ""

class JsonEncoder:
    spanAttributeList = ['name', 'id', 'timestamp', 'traceId', 'duration']
    annotationAttributeList = ['timestamp', 'value']
    binaryAnnotationAttributeList = ['key', 'value']

    @staticmethod
    def fromRawToEndPoint( endPointRawData):
        endpoint = EndPoint()
        endpoint.ipv4 = endPointRawData['ipv4']
        endpoint.port = endPointRawData['port']
        endpoint.serviceName = endPointRawData['serviceName'].encode('ascii', 'ignore')
        return endpoint

    @staticmethod
    def fromRawToAnnotation( annotationRawData ):
        annotation = Annotation()
        for attribute in JsonEncoder.annotationAttributeList:
            setattr(annotation, attribute, annotationRawData[attribute])
        annotation.endpoint = JsonEncoder.fromRawToEndPoint( annotationRawData['endpoint'])
        return annotation

    @staticmethod
    def fromRawToBinaryAnnotation( binaryAnnotationRawData ):
        binaryAnnotation = BinaryAnnotation()
        for attribute in JsonEncoder.binaryAnnotationAttributeList:
            setattr(binaryAnnotation, attribute, binaryAnnotationRawData[attribute])
        binaryAnnotation.endpoint = JsonEncoder.fromRawToEndPoint( binaryAnnotationRawData['endpoint'])
        return binaryAnnotation

    @staticmethod
    def fromRawToSpan( spanRawData ):
        span = Span()
        for attribute in JsonEncoder.spanAttributeList:
            setattr(span, attribute, spanRawData[attribute])

        span.parentId = spanRawData.get('parentId', '')

        for annotationData in spanRawData['annotations']:
            annotation = JsonEncoder.fromRawToAnnotation( annotationData )
            span.annotations.append(annotation)

        for binaryAnnotationData in spanRawData['binaryAnnotations']:
            binaryAnnotation = JsonEncoder.fromRawToBinaryAnnotation( binaryAnnotationData )
            span.binaryAnnotations.append(binaryAnnotation)

        return span

    @staticmethod
    def fromJson( data ):
        spans = []
        rawData = json.JSONDecoder().decode(data.decode('utf-8'))
        for spanRawData in rawData:
            spans.append(JsonEncoder.fromRawToSpan( spanRawData ))

        return spans

class ZipkinStubServer(object):
    def __init__(self, port = 9411):
        self._port = port
        self._messageQueue = queue.Queue()
        self._running = False
        self._thread = None
        self.outQueue = None
        self.startEvent = None
        self._app = Bottle()
        self._app.route(path = '/api/v1/spans', method = 'POST', callback = self.handleSpans)
        self._server = server.WSGIRefServerStoppable(host = 'localhost', port = port)

    def stop(self):
        self._running = False
        self._messageQueue.put(('',''))
        self._thread.join()
        self._server.stop()

    def start(self):
        self._running = True
        self._thread = Thread(None, self.eventLoop, 'event loop', (), {})
        self._thread.start()
        self.startEvent.set()
        self._app.run(server = self._server)

    def handleSpans(self):
        contentType = request.headers.get('Content-Type')
        a = request.body.readlines()
        self._messageQueue.put((contentType, a))

    def eventLoop(self):
        while self._running == True:
            try:
                message = self._messageQueue.get(True)
                if message[0] == 'application/json':
                    spans = JsonEncoder.fromJson(message[1][0])
                    self.outQueue.put( spans )
            except queue.Empty:
                pass

    @staticmethod
    def spawn(server, outQueue, startEvent):
        server.outQueue = outQueue
        server.startEvent = startEvent
        server.start()

class ServerGuard:
    def __init__(self, entryPoint, *args):
        self._thread = None
        self._entryPoint = entryPoint
        self._args = args

    def __enter__(self):
        self._thread = Thread( target = self._entryPoint, args=self._args )
        self._thread.start()

    def __exit__(self, exc_type, exc_val, exc_tb):
        self._thread.join()



