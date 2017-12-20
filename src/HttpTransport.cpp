#include <sstream>
#include "HttpTransport.h"
#include "ConfigParams.h"
#include "Encoder.h"
#include "EncodingContext.h"
#include "EncoderBase64.h"

using namespace std;
using namespace apache::thrift;

namespace cppkin
{
	HttpTransport::HttpTransport() {
    }

	HttpTransport::~HttpTransport() {
    }

    void HttpTransport::Submit(const std::vector<Span*> &spans) {
		EncoderContextThrift context;

		for (auto &span : spans)
		{
			Encoder<EncodingTypes::Thrift>::Serialize(context, *span);
		}

    	string buffer = context.ToString();
        try {

        	CURL* curl = curl_easy_init();
        	if (!curl) {
        		// TODO
			}
        	struct curl_slist *headers = nullptr;
        	headers = curl_slist_append(headers, "Content-Type: application/x-thrift");
			headers = curl_slist_append(headers, "Expect:");
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

			std::stringstream url;
			url << "http://" << ConfigParams::Instance().GetHostAddress() << ":" << ConfigParams::Instance().GetPort() << "/api/v1/spans";

			curl_easy_setopt(curl, CURLOPT_URL, url.str().c_str() );
			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, buffer.length());
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buffer.c_str());

			CURLcode res = curl_easy_perform(curl);
			/* Check for errors */
			if(res != CURLE_OK)
			  fprintf(stderr, "curl_easy_perform() failed: %s\n",
					  curl_easy_strerror(res));

			curl_slist_free_all(headers);
			curl_easy_cleanup(curl);
        }
        catch(apache::thrift::transport::TTransportException& exc){
        }
    }
}
