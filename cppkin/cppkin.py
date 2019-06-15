import sys
try:
    from . import _cppkin
    from . _cppkin import Span, Trace, SERVER_RECEIVE, SERVER_SEND, NOP, add_tag, add_annotation
except ImportError:
    import _cppkin
    from _cppkin import Span, Trace, SERVER_RECEIVE, SERVER_SEND, NOP, add_tag, add_annotation

is_type_hints_supported = sys.version_info.major >= 3 and sys.version_info.minor >= 5

if is_type_hints_supported:
    import typing

def push_span(span):
    _cppkin.push_span(span)

def pop_span():
    _cppkin.pop_span()

def top_span():
    return _cppkin.top_span()

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
        span_arg_name = None
        if is_type_hints_supported:
            hints = typing.get_type_hints(func)
            span_arg_name = next((arg_name for arg_name, hint in hints.items() if hint == Trace), None)

        def trace_wrapper(*args, **kwargs):
            trace = Trace(operation)
            _cppkin.push_span(_cppkin.cast_trace_to_span(trace))
            if span_arg_name:
                kwargs.update({span_arg_name : span})
            result = func(*args, **kwargs)
            _cppkin.pop_span()
            trace.submit()
            return result
        return trace_wrapper
    return trace_decorator

def span(operation):
    def span_decorator(func):
        span_arg_name = None
        if is_type_hints_supported:
            hints = typing.get_type_hints(func)
            span_arg_name = next((arg_name for arg_name, hint in hints.items() if hint == Span ), None)

        def span_wrapper(*args, **kwargs):
            top_span = _cppkin.top_span()
            span = top_span.create_span(operation, SERVER_RECEIVE)
            _cppkin.push_span(span)
            if span_arg_name:
                kwargs.update({span_arg_name : span})
            result = func(*args, **kwargs)
            _cppkin.pop_span()
            span.submit()
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
            self._span = top_span.create_span(self._operation, SERVER_RECEIVE)
        _cppkin.push_span(self._span)

    def __exit__(self, exc_type, exc_val, exc_tb):
        _cppkin.pop_span()
        self._span.submit()


