//
// Created by alex on 29.10.2022.
//

#pragma once

#include "data/Particle.h"
#include "defaults.h"

#include <list>
#include <vector>
#include <iostream>
#include <unordered_map>

namespace io::input {

    /**
     * Generic Input file loader.
     * Basic interface to load particles.
     * @param LOAD is a function that loads raw data from @param LOCATOR into the list.
     * */
    template<typename LOCATOR, void (*LOAD)(LOCATOR, std::list<Particle> &,
                                            std::unordered_map<std::string, std::string> &)>
    class InputLoader {
    private:
        std::list<Particle> buffer;
        LOCATOR locator;
        std::unordered_map<std::string, std::string> &arg_map;
    public:
        explicit InputLoader(LOCATOR loc, std::unordered_map<std::string, std::string> &map) : locator(loc),
                                                                                               arg_map(map) {}

        InputLoader(const InputLoader &i) = delete;

        /**
         * Calls the loader again and stores all particles the internal buffer.
         * */
        void reload() {
            LOAD(locator, buffer, arg_map);
        }

        /**
         * Populates @param buf with the stored particles and afterwards clears the internal buffer.
         * */
        void getParticles(std::vector<Particle> &buf) {
            while (!buffer.empty()) {
                buf.template emplace_back(buffer.front());
                buffer.pop_front();
            }
        }
    };

} // io::input

