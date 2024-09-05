/**
 * Copyright (c) 2006-2024 LOVE Development Team
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 **/

#include "Data.h"
#include "freertos/FreeRTOS.h" // For FreeRTOS types
#include "freertos/semphr.h"   // For FreeRTOS mutex functions

namespace love
{

love::Type Data::type("Data", &Object::type);

Data::~Data()
{
    if (mutex != nullptr)
    {
        delete mutex;  // Handle destruction of mutex
    }
}

// FreeRTOS mutex for ensuring createMutex is called only once
static SemaphoreHandle_t mutexCreationLock = xSemaphoreCreateMutex();

static void createMutex(love::thread::Mutex **mutexAddress)
{
    // This function creates the mutex (assuming newMutex exists in love::thread)
    *mutexAddress = love::thread::Mutex();
}

love::thread::Mutex *Data::getMutex()
{
    // Use FreeRTOS to control access to mutex creation
    if (xSemaphoreTake(mutexCreationLock, portMAX_DELAY))
    {
        if (mutex == nullptr)
        {
            createMutex(&mutex);
        }
        xSemaphoreGive(mutexCreationLock);
    }
    return mutex;
}

} // namespace love
