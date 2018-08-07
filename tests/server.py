from bottle import Bottle, ServerAdapter
from wsgiref.simple_server import make_server
import socket
from contextlib import closing

class WSGIRefServerStoppable(ServerAdapter):
    def __init__(self, host, port, **options):
        self._server = None
        super(WSGIRefServerStoppable, self).__init__(host, port, **options)

    def run(self, handler):
        self._server = make_server(self.host, self.port, handler, **self.options)
        self._server.serve_forever()

    def stop(self):
        self._server.shutdown()

    @staticmethod
    def findFreePort():
        with closing(socket.socket(socket.AF_INET, socket.SOCK_STREAM)) as s:
            s.bind(('', 0))
            return s.getsockname()[1]