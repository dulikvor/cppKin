#include "curl/curl.h"

#include "HttpTransport.h"
#include "ConfigParams.h"
#include "Encoder.h"
#include "EncodingContext.h"

using namespace std;

namespace cppkin
{
    void HttpTransport::Submit(std::vector<std::unique_ptr<span_impl>>& spans) {

        string buffer = EncoderContext(spans).ToString();
        try {

            CURL* curl = curl_easy_init();
            if (!curl) {
                // TODO
            }
            struct curl_slist *headers = nullptr;

            if (ConfigParams::Instance().GetEncodingType() == EncodingType::Thrift)
                headers = curl_slist_append(headers, "Content-Type: application/x-thrift");
            else
                headers = curl_slist_append(headers, "Content-Type: application/json");

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
        catch(...){
        }
    }
}
