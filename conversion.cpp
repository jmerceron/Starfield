#include "conversion.h"
#include <stdio.h>

namespace Julien_Conversion {

    void convert_to_string(float value, char* buffer)
    {
        sprintf_s(buffer, 10, "%.2f", value);
    }

} // namespace Julien_Conversion