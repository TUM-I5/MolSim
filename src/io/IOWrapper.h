//
// Created by alex on 14.11.2022.
//

#pragma once

#include <type_traits>
#include <list>
#include "data/Particle.h"

namespace io {

    template<typename LOCATOR,
            std::conditional<std::is_same<LOCATOR, >>, void (*LOAD)(LOCATOR, std::list<Particle>&, double&, double&)>
    class IOWrapper {

    };

} // io
