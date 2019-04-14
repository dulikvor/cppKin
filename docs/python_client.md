# Python Client:

## Initialization:
Before we can start we need to initialize our client, should be done once per service.

Lets start by importing cppkin module and invoking the start function:
```python
import cppkin
from cppkin import span, trace, TracingContext
...
cppkin.start(host_address = "127.0.0.1", port = 9411, service_name = "cppkinTest", sample_count = 1)
```
## Instrumenting:

### zipkin's data model key insights:
- Spans and Traces are used to instrument logical operations.
- The relation between Spans is that of parent-child.
- Those relations composed a non binary tree like structure.
- A Trace acts as the root of a span tree.

One easy approach to create Spans and Traces is to use decorators.

First lets create a trace:
```python
@trace("operation_name_we_want_to_display")
def foo():
  goo()
```
Spans and Traces can display time occurences called - **annotations**:
```python
cppkin.add_annotation("some_event")
```
Lets create a child span for our trace:
```python
@span("operation_name_we_want_to_display")
def goo():
```
Spans and Traces can be added with key-value occurences called - **tags**:
```c++
cppkin.add_tag("some_key", "some_value")
```
Spans and Traces are propagated automatically to zipkin's server once the decorated function goes out of scope.

decorators are not the only way to instrument functions, 
cppkin Tracing context manager can also be used:
```python
def foo():
  with TracingContext("operation_name_we_want_to_display"):
    cppkin.add_annotation("some_event")
```

it is also possible to use Span and Trace user types directly.
```python
from cppkin import Trace, Span
...
trace = Trace("some_operation")
trace.add_annotation("some_event")
span = trace.create_span("some_operation")
...
span.add_tag("key", "value")
span.submit()
```

## Stopping:
once done, we can call stop in order to release the client's resources:
```python
cppkin.stop()
```

# Advance usage:

## Span header:
The span header can be retrieved in order to propagate it to a different service.

The header is retrieved as a single unicode object encoded in b3 format:
```python
header = span.get_header_b3_format()
```

## Join:
Spans can be joined (continued in separate services).

This is done by using the header, encoded in b3 format:
```python
span = Span()
span.join(b3_header_format)
```

## Accessing a trace/span directly when using function decorators:
Only supported in python version >=3.5.

Any decorated function can provide direct access to its matching trace or span instance.
in order to do that the user have to **hint** one of its parameters with a matching type:
```python
@span("some_operation")
def foo(span : Span):
    span.add_annotation("some_event")
```

## Passing Trace/Span across multiple stack frames:
It is possible to provide a Trace/Span instance to another stack frame with out involving dependency injection:
```python
import cppkin
from cppkin import Trace, Span

def foo():
    trace = cppkin.top_span(); #our trace from before
    span = trace.create_span("span_operation");
    span.submit();

#main function
trace = Trace("trace_operation")
#Lets use the span stack container in order to reach a certain stack frame.
cppkin.push_span(trace);
foo()
#once done we can pop the span from the stack
cppkin.pop_span();
trace.submit();
```
#### For more information please review the python example:
[python example](https://github.com/Dudi119/cppKin/blob/master/examples/python/example.py)
