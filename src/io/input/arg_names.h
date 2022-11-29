//
// Created by alex on 25.11.2022.
//

#pragma once


namespace io::input {
    /**
    * Defines names in the map for arguments in the input loader
    * */
    enum names {
        sigma,
        epsilon,
        brown,
        dimensions,
        boundCond,
        linkedCell,
        names_count [[maybe_unused]]
    };
} // io::input