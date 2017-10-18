#include "Serializor.h"
#include "Core/src/TcpSocket.h"
#include "Span.h"
#include "SimpleAnnotation.h"
#include "SerializeContext.h"

using namespace core;

namespace cppkin
{
    void Serializor<SerializeType::Thrift>::Serialize(SerializeContext& context, const Span& span){
        SerializeContextThrift& thriftContext = static_cast<SerializeContextThrift&>(context);
        ::Span thriftSpan;
        thriftSpan.trace_id = span.GetHeader().TraceID;
        thriftSpan.name = span.GetHeader().Name;
        thriftSpan.id = span.GetHeader().ID;
        thriftSpan.parent_id = span.GetHeader().ParentID;
        for(auto& annotation : span.GetAnnotations())
            if(annotation->GetType() == AnnotationType::Simple)
                Serialize(thriftSpan, static_cast<const SimpleAnnotation&>(*annotation));
        thriftContext.AddSpan(thriftSpan);
    }
    void Serializor<SerializeType::Thrift>::Serialize(::Span& thriftSpan, const SimpleAnnotation &annotation) {
        ::Annotation thriftAnnotation;
        thriftAnnotation.value = annotation.GetEvent();
        thriftAnnotation.timestamp = annotation.GetTimeStamp();
        const Annotation::EndPoint& endPoint = annotation.GetEndPoint();
        sockaddr_in formattedAddress =  TCPSocket::GetSocketAddress(endPoint.Host, endPoint.Port);
        thriftAnnotation.host.ipv4 = formattedAddress.sin_addr.s_addr;
        thriftAnnotation.host.port = formattedAddress.sin_port;
        thriftSpan.annotations.emplace_back(thriftAnnotation);
    }
}
