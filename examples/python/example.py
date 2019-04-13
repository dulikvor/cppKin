import argparse, sys, os, time
sys.path.append(os.environ['PYBINDER'])
import cppkin
from cppkin import trace, span, add_annotation, add_tag, Trace, Span, TracingContext

def boo():
    with TracingContext("span3_operation"):
        cppkin.add_annotation("span3_event")

@span("span2_operation")
def foo(span : Span):
    cppkin.add_annotation("span2_event")
    span.add_annotation("span2_event2")
    boo()

def main():
    parser = argparse.ArgumentParser(description='cppkin py example.')
    parser.add_argument('service_name', type=str, help='service name')
    parser.add_argument('--host',  nargs='?', default='localhost', type=str, help='zipkin server host address')
    parser.add_argument('--port', nargs='?', default=9411, type=int, help='zipkin server port')
    parser.add_argument('--sample_count', nargs='?', default=1, type=int, help='sampling count')

    args = parser.parse_args()

    cppkin.start(args.host, args.port, args.service_name, args.sample_count)

    trace = Trace("trace_operation")
    trace.add_annotation("trace_event")
    span_1 = trace.create_span("span1_operation", cppkin.SERVER_RECEIVE)
    span_1.add_annotation("span1_event")
    span_1.add_tag("span1_key", "span1_value")

    cppkin.push_span(span_1)
    foo()
    cppkin.pop_span()

    span_1.submit(cppkin.SERVER_SEND)
    trace.submit(cppkin.SERVER_SEND)

    time.sleep(3)

    cppkin.stop()

if __name__== "__main__":
    main()

