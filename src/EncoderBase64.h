#pragma once

#include <string>
#include <sstream>
#include "boost/archive/iterators/base64_from_binary.hpp"
#include "boost/archive/iterators/binary_from_base64.hpp"
#include "boost/archive/iterators/transform_width.hpp"

namespace cppkin
{
    static const char* base64_padding[] = {"", "==","="};

    std::string base64EncodeText(const std::string& data) {
        using namespace boost::archive::iterators;
        typedef base64_from_binary<transform_width<std::string::const_iterator, 6, 8> > base64_enc;
        std::stringstream ss;
        std::copy(base64_enc(data.begin()), base64_enc(data.end()), std::ostream_iterator<char>(ss));
        ss << base64_padding[data.size() % 3];
        return ss.str();
    }
}
