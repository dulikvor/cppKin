from zipkinStubServer import ServerGuard, ZipkinStubServer
import _cppkin
from _cppkin import Trace, CppkinParams, Logger
import unittest
from Queue import Queue
from threading import Event

outQueue = None
startEvent = None

class TestCppkin(unittest.TestCase):
    def setUp(self):
        params = CppkinParams()
        params.add_str(_cppkin.HOST_ADDRESS, "127.0.0.1")
        params.add_int(_cppkin.PORT, 9411)
        params.add_str(_cppkin.SERVICE_NAME, "cppkinTest")
        params.add_bool(_cppkin.DEBUG, True)
        params.add_str(_cppkin.TRANSPORT_TYPE, "Http Transport")
        params.add_int(_cppkin.SAMPLE_COUNT, 1)

        _cppkin.init( params )

class TestCppkinTrace(TestCppkin):
    def runTest(self):
        trace = Trace("TestTrace")
        trace.submit()

        global outQueue
        spans = outQueue.get()
        self.assertEqual(len(spans), 1, "The amount of received spans dosn't match expected {0} != {1}". format(len(spans), 1))
        self.assertEqual(spans[0].name, "TestTrace", "Trace name dosn't match {0} != {1}".format(spans[0].name, "TestTrace"))

def main():
    global outQueue
    global startEvent
    server = ZipkinStubServer()
    outQueue = Queue()
    startEvent = Event()
    with ServerGuard(ZipkinStubServer.spawn, server, outQueue, startEvent):
        unittest.main(exit=False)
        server.stop()


if __name__ == "__main__":
    Logger.instance().start(_cppkin.TraceSeverity.Info)
    main()
