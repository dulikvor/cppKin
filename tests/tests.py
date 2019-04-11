from zipkinStubServer import ServerGuard, ZipkinStubServer
from server import WSGIRefServerStoppable
import cppkin
from cppkin import Trace, span, trace, TracingContext
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
        with TracingContext("Context_Span") as _:
            cppkin.add_annotation("context span event")

        cppkin.add_tag("some key", "some value")
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
        self.assertEqual(len(spans), 4, "The amount of received spans dosn't match expected {0} != {1}". format(len(spans), 4))
        self.assertEqual(spans[0].name, "TestTrace", "Trace name dosn't match {0} != {1}".format(spans[0].name, "TestTrace"))
        self.assertEqual(spans[0].id, spans[0].traceId, "Trace id dosn't match id {0} != {1}".format(spans[0].id, spans[0].traceId))
        self.assertEqual(spans[1].name, "Context_Span", "Span's name dosn't match {0} != {1}".format(spans[1].name, "Context_Span"))
        self.assertEqual(spans[1].traceId, spans[3].id, "Span's id dosn't match id {0} != {1}".format(spans[1].traceId, spans[3].id))
        self.assertEqual(spans[1].parentId, spans[2].id, "Span parent id dosn't match trace's id {0} != {1}".format(spans[1].parentId, spans[2].id))
        self.assertEqual(spans[1].annotations[1].value, "context span event", "Span's annotation dons't match {0} != {1}".format(spans[1].annotations[1].value, "context span event"))
        self.assertEqual(spans[2].name, "Decorator_Span", "Trace name dosn't match {0} != {1}".format(spans[2].name, "Decorator_Span"))
        self.assertEqual(spans[2].traceId, spans[3].id, "Trace id dosn't match id {0} != {1}".format(spans[2].traceId, spans[3].id))
        self.assertEqual(spans[2].parentId, spans[3].id, "Span parent id dosn't match trace's id {0} != {1}".format(spans[2].parentId, spans[3].id))
        tag_state = lambda annotation: (annotation.key, annotation.value)
        self.assertEqual(tag_state(spans[2].binaryAnnotations[0]), ("some key", "some value"), "Span's binary annotation dons't match {0} != {1}".format(tag_state(spans[2].binaryAnnotations[0]), ("some key", "some value")))
        self.assertEqual(spans[3].name, "Decorator_Trace", "Trace name dosn't match {0} != {1}".format(spans[3].name, "Decorator_Trace"))
        self.assertEqual(spans[3].id, spans[3].traceId, "Trace id dosn't match id {0} != {1}".format(spans[3].id, spans[3].traceId))
        self.assertEqual(spans[3].annotations[1].value, "some event", "Span's annotation dons't match {0} != {1}".format(spans[3].annotations[1].value, "some event"))


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
