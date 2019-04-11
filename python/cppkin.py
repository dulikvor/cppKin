import _cppkin
from _cppkin import Span, Trace, SERVER_RECEIVE, SERVER_SEND, add_tag, add_annotation


def start(host_address, port, service_name, sample_count):
    params = _cppkin.CppkinParams()
    params.add_str(_cppkin.HOST_ADDRESS, host_address)
    params.add_int(_cppkin.PORT, port)
    params.add_str(_cppkin.SERVICE_NAME, service_name)
    params.add_bool(_cppkin.DEBUG, False)
    params.add_str(_cppkin.TRANSPORT_TYPE, "Http Transport")
    params.add_int(_cppkin.SAMPLE_COUNT, sample_count)
    _cppkin.init( params )

def stop():
    _cppkin.stop()

def trace(operation):
    def trace_decorator(func):
        def trace_wrapper(*args, **kwargs):
            trace = Trace(operation)
            _cppkin.push_span(_cppkin.cast_trace_to_span(trace))
            result = func(*args, **kwargs)
            _cppkin.pop_span()
            trace.submit(SERVER_SEND)
            return result
        return trace_wrapper
    return trace_decorator

def span(operation):
    def span_decorator(func):
        def span_wrapper(*args, **kwargs):
            top_span = _cppkin.top_span()
            span = top_span.createSpan(operation, SERVER_RECEIVE)
            _cppkin.push_span(span)
            result = func(*args, **kwargs)
            _cppkin.pop_span()
            span.submit(SERVER_SEND)
            return result
        return span_wrapper
    return span_decorator

class TracingContext:
    def __init__(self, operation):
        self._operation = operation
        self._span = None

    def __enter__(self):
        if _cppkin.is_container_empty():
            self._span = Trace(self._operation)
        else:
            top_span = _cppkin.top_span()
            self._span = top_span.createSpan(self._operation, SERVER_RECEIVE)
        _cppkin.push_span(self._span)

    def __exit__(self, exc_type, exc_val, exc_tb):
        _cppkin.pop_span()
        self._span.submit(SERVER_SEND)


