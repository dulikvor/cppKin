from bottle import Bottle, ServerAdapter
from wsgiref.simple_server import make_server

class WSGIRefServerStoppable(ServerAdapter):
    def __init__(self, host, port, **options):
        self._server = None
        super(WSGIRefServerStoppable, self).__init__(host, port, **options)

    def run(self, handler):
        self._server = make_server(self.host, self.port, handler, **self.options)
        self._server.serve_forever()

    def stop(self):
        self._server.shutdown()