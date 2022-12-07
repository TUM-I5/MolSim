/*
 *  ParticleContainer.h
 *
 *  Created on: 5.12.2022
 *      Author: borisov
 */

#pragma once

#include <vector>
#include <array>

/**
 * @brief namespace that includes utils for the particle container
 */
namespace PContainer {
    // Predeclarations
    std::vector<int> getNeighboursSurrounding2D(int index, std::array<int, 3> &numCells);

    std::array<int, 3> convert1DTo3D(int index, std::array<int, 3> &numCells);

    int convert3DTo1D(std::array<int, 3> index, std::array<int, 3> &numCells);



    /**
     * @brief calculates the indices of the neighbours for a given cell, considering Newton's 3rd Law
     * 
     * @param index index of the cell
     * @param numCells dimension of the cell array
     * @return indices of the neighbours
     */
    inline std::vector<int> getNeighboursNewton(int index, std::array<int, 3> &numCells)
    {
        std::vector<int> result;

        // transform index to 3d, so its easier to find the index of the cell right behind
        std::array<int, 3> index3D = convert1DTo3D(index, numCells);

        // all neighbours in the front layer
        std::vector<int> frontLayerAll = getNeighboursSurrounding2D(index, numCells);

        // filter out smaller indices (according to newtons 3rd law)
        for (int i: frontLayerAll) {
            if (i > index)
                result.push_back(i);
        }

        // all neighbours in the back layer
        // z out of bounds
        if (index3D[2] + 1 >= numCells[2])
            return result;
        
        // z + 1
        index3D[2] = index3D[2] + 1;
        
        // get the 1 dimensional index of the cell right behind
        int backLayerIndex = convert3DTo1D(index3D, numCells);
        // add it right away to the result
        result.push_back(backLayerIndex);

        // add all neighbours of the cell right behind to the result
        for (int i: getNeighboursSurrounding2D(backLayerIndex, numCells)) {
            result.push_back(i);
        }
        return result;
    }

    /**
     * @brief calculates the indices of all surrounding neighbours for a given cell
     * 
     * @param index index of the cell
     * @param numCells dimension of the cell array
     * @return indices of the neighbours
     */
    inline std::vector<int> getNeighboursSurrounding2D(int index, std::array<int, 3> &numCells)
    {
        std::vector<int> result;

        // transform index to 3 dim index for easier use
        // but we will only use x and y in this method
        std::array<int, 3> index3D = convert1DTo3D(index, numCells);

        // get all surrounding cells
        // from bottom left to top right 
        std::array<int, 3> tmpIndex3D;
        for (int y = index3D[1] - 1; y <= index3D[1] + 1; y++) {
            // y out of bounds
            if (y < 0 || y >= numCells[1])
                continue;

            for (int x = index3D[0] - 1; x <= index3D[0] + 1; x++) {
                // x out of bounds
                if (x < 0 || x >= numCells[0])
                    continue;

                tmpIndex3D[0] = x;
                tmpIndex3D[1] = y;
                tmpIndex3D[2] = index3D[2];

                int n = convert3DTo1D(tmpIndex3D, numCells);
                // index is not neighbour of itself
                if (n != index)
                    result.push_back(n);
            }
        }
        return result;
    }

    /*
    * Helper Functions
    */

    /**
     * @brief converts a 1 dimensional index to a 3 dimensional index
     * 
     * @param index 1 dimensional index
     * @param numCells dimension of the array
     * @return 3 dimensional index
     */
   inline std::array<int, 3> convert1DTo3D(int index, std::array<int, 3> &numCells) 
    {
        std::array<int, 3> result;
        
        // x
        result[0] = index % numCells[0];
        // y
        result[1] = (index / numCells[0]) % numCells[1];
        //z
        result[2] = index / (numCells[0] * numCells[1]);

        return result;
    }

    /**
     * @brief converts a 3 dimensional index to a 1 dimensional index
     * 
     * @param index 3 dimensional index
     * @param numCells dimension fo the array
     * @return 1 dimensional index
     */
    inline int convert3DTo1D(std::array<int, 3> index, std::array<int, 3> &numCells)
    {
        return index[0] + (index[1] * numCells[0]) + (index[2] * numCells[0] * numCells[1]);
    }
}