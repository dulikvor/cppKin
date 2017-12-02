#pragma once

#include <thread>
#include <memory>
#include "Core/src/Export.h"
#include "Span.h"

namespace cppkin
{
	class A_EXPORT SpanContainer
    {
    public:
		static SpanContainer& Instance();
		~SpanContainer();
		Span& GetSpan() const;
        void SetSpan(Span* span);
        std::unique_ptr<Span> ReleaseSpan();
    private:
		SpanContainer();
    };
}

