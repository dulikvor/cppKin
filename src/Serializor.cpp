#include "Serializor.h"
#include <string>
#include "Core/src/TcpSocket.h"
#include "Span.h"
#include "SimpleAnnotation.h"
#include "SerializeContext.h"

using namespace core;
using namespace std;

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


    void Serializor<SerializeType::ByteStream>::Serialize(SerializeContext& context, const Span::SpanHeader& spanHeader)
    {
        SerializeContextByteStream& byteStreamContext = static_cast<SerializeContextByteStream&>(context);
        byteStreamContext.Write(reinterpret_cast<const char*>(&spanHeader.ParentID), sizeof(spanHeader.ParentID));
        byteStreamContext.Write(reinterpret_cast<const char*>(&spanHeader.ID), sizeof(spanHeader.ID));
        int stringSize = spanHeader.Name.size();
        byteStreamContext.Write(reinterpret_cast<const char*>(&stringSize), sizeof(int));
        byteStreamContext.Write(spanHeader.Name.data(), stringSize);
        byteStreamContext.Write(reinterpret_cast<const char*>(&spanHeader.TraceID), sizeof(spanHeader.TraceID));
    }

    Span::SpanHeader Serializor<SerializeType::ByteStream>::DeSerializeSpanHeader(SerializeContext &context) {
        SerializeContextByteStream& byteStreamContext = static_cast<SerializeContextByteStream&>(context);
        Span::SpanHeader spanHeader;
        byteStreamContext.Read(reinterpret_cast<char*>(&spanHeader.ParentID), sizeof(spanHeader.ParentID));
        byteStreamContext.Read(reinterpret_cast<char*>(&spanHeader.ID), sizeof(spanHeader.ID));
        int stringSize;
        byteStreamContext.Read(reinterpret_cast<char*>(&stringSize), sizeof(int));
        string strName(stringSize, 0);
        byteStreamContext.Read(const_cast<char*>(strName.data()), stringSize);
        spanHeader.Name = move(strName);
        byteStreamContext.Read(reinterpret_cast<char*>(&spanHeader.TraceID), sizeof(spanHeader.TraceID));
    }
}
