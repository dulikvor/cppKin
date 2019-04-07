from zipkinStubServer import ServerGuard, ZipkinStubServer
from server import WSGIRefServerStoppable
import cppkin
from cppkin import Trace, span, trace
import unittest
from queue import Queue
from threading import Event

outQueue = None
startEvent = None

class TestCppkin(unittest.TestCase):
    def setUp(self):
        pass

class TestCppkinTrace(TestCppkin):

    @span("Decorator_Span")
    def span_wrapped_func(self):
        cppkin.add_binary_annotation("some key", "some value")
        pass

    @trace("Decorator_Trace")
    def trace_wrapped_func(self):
        cppkin.add_annotation("some event")
        self.span_wrapped_func()

    def runTest(self):
        trace = Trace("TestTrace")
        trace.submit(cppkin.SERVER_SEND)

        self.trace_wrapped_func()
        global outQueue
        spans = outQueue.get()
        self.assertEqual(len(spans), 3, "The amount of received spans dosn't match expected {0} != {1}". format(len(spans), 1))
        self.assertEqual(spans[0].name, "TestTrace", "Trace name dosn't match {0} != {1}".format(spans[0].name, "TestTrace"))
        self.assertEqual(spans[0].id, spans[0].traceId, "Trace id dosn't match id {0} != {1}".format(spans[0].id, spans[0].traceId))
        self.assertEqual(spans[1].name, "Decorator_Span", "Trace name dosn't match {0} != {1}".format(spans[2].name, "Decorator_Span"))
        self.assertEqual(spans[1].traceId, spans[2].id, "Trace id dosn't match id {0} != {1}".format(spans[1].traceId, spans[2].id))
        self.assertEqual(spans[1].parentId, spans[2].id, "Span parent id dosn't match trace's id {0} != {1}".format(spans[1].parentId, spans[2].id))
        binary_annotation_state = lambda annotation: (annotation.key, annotation.value)
        self.assertEqual(binary_annotation_state(spans[1].binaryAnnotations[0]), ("some key", "some value"), "Span's binary annotation dons't match {0} != {1}".format(binary_annotation_state(spans[1].binaryAnnotations[0]), ("some key", "some value")))
        self.assertEqual(spans[2].name, "Decorator_Trace", "Trace name dosn't match {0} != {1}".format(spans[2].name, "Decorator_Trace"))
        self.assertEqual(spans[2].id, spans[2].traceId, "Trace id dosn't match id {0} != {1}".format(spans[2].id, spans[2].traceId))
        self.assertEqual(spans[2].annotations[1].value, "some event", "Span's annotation dons't match {0} != {1}".format(spans[2].annotations[1].value, "some event"))


class TestCppkinTraceSpanRelation(TestCppkin):
    def runTest(self):
        trace = Trace("TestTrace")
        span = trace.createSpan("TestSpan", cppkin.SERVER_RECEIVE)
        span.submit(cppkin.SERVER_SEND)
        trace.submit(cppkin.SERVER_SEND)

        global outQueue
        spans = []
        while(len(spans) != 2):
            spans.extend(outQueue.get())

        self.assertEqual(len(spans), 2, "The amount of received spans dosn't match expected {0} != {1}". format(len(spans), 1))
        self.assertEqual(spans[0].name, "TestSpan", "Span name dosn't match {0} != {1}".format(spans[0].name, "TestSpan"))
        self.assertEqual(spans[0].traceId, spans[1].id, "Span trace id dosn't match trace's id {0} != {1}".format(spans[0].traceId, spans[1].id))
        self.assertEqual(spans[0].parentId, spans[1].id, "Span parent id dosn't match trace's id {0} != {1}".format(spans[0].parentId, spans[1].id))

def main():
    port = WSGIRefServerStoppable.findFreePort()

    cppkin.start("127.0.0.1", port, "cppkinTest", 1)

    global outQueue
    global startEvent
    server = ZipkinStubServer(port)
    outQueue = Queue()
    startEvent = Event()
    with ServerGuard(ZipkinStubServer.spawn, server, outQueue, startEvent):
        unittest.main(exit=False)
        server.stop()

    cppkin.stop()

if __name__ == "__main__":
    main()
