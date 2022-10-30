//
// Created by alex on 29.10.2022.
//

#pragma once

#include "Particle.h"

#include <list>
#include <vector>
#include<iostream>

namespace io {

    /**
     * Generic Input file loader.
     * Basic interface to load particles.
     * @param LOAD is a function that loads raw data from @param LOCATOR into the list.
     * */
    template <typename LOCATOR, void (*LOAD)(LOCATOR, std::list<Particle>&)>
    class InputLoader {
    private:
        std::list<Particle> buffer;
        LOCATOR locator;
    public:
        explicit InputLoader(LOCATOR loc) : locator(loc) {}
        InputLoader(const InputLoader& i) = delete;
        /**
         * Calls the loader again and stores all particles the internal buffer.
         * */
        void reload() {
            LOAD(locator, buffer);
        }

        /**
         * Populates @param buf with the stored particles and afterwards clears the internal buffer.
         * */
        void getParticles(std::vector<Particle> &buf) {
            while(!buffer.empty()) {
                buf.template emplace_back(buffer.front());
                buffer.pop_front();
            }
        }
    };

} // io

