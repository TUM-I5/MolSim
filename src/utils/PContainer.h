/*
 *  ParticleContainer.h
 *
 *  Created on: 5.12.2022
 *      Author: borisov
 */

#pragma once

#include <vector>
#include <array>
#include "ArrayUtils.h"

/**
 * @brief namespace that includes utils for the particle container
 */
namespace PContainer
{
    // Predeclarations
    std::vector<int> getSurroundingZ(int index, std::array<int, 3> &numCells);

    std::vector<int> getHaloX(int index, std::array<int, 3> &numCells);

    std::vector<int> getHaloY(int index, std::array<int, 3> &numCells);

    std::vector<int> getHaloZ(int index, std::array<int, 3> &numCells);

    std::array<int, 3> convert1DTo3D(int index, std::array<int, 3> &numCells);

    int convert3DTo1D(std::array<int, 3> index, std::array<int, 3> &numCells);


    /**
     * @brief calculates indices of neighbouring halo cells of given cell
     * @param index 1D index of cell
     * @param numCells number of cells in each dimension
     * @return 1D indices of halo cells neighbouring given cell
    */
    inline std::vector<int> getHaloNeighbours(int index, std::array<int, 3> &numCells) {
        std::set<int> result;
        std::array<int, 3> index3D = convert1DTo3D(index, numCells);
        
        //cell at front boundary
        if (index3D[2] == 1) {
            int tmp_index = convert3DTo1D({index3D[0], index3D[1], 0}, numCells);
            std::vector<int> halo = getHaloZ(tmp_index, numCells);
            std::copy(halo.begin(), halo.end(), std::inserter(result, result.end()));
        }
        //cell at back boundary
        if (index3D[2] == numCells[2] - 2) {
            int tmp_index = convert3DTo1D({index3D[0], index3D[1], index3D[2]+1}, numCells);
            std::vector<int> halo = getHaloZ(tmp_index, numCells);
            std::copy(halo.begin(), halo.end(), std::inserter(result, result.end()));
        }
        //cell at left boundary
        if (index3D[0] == 1) {
            int tmp_index = convert3DTo1D({0, index3D[1], index3D[2]}, numCells);
            std::vector<int> halo = getHaloX(tmp_index, numCells);
            std::copy(halo.begin(), halo.end(), std::inserter(result, result.end()));
        }
        //cell at right boundary
        if (index3D[0] == numCells[0] - 2) {
            int tmp_index = convert3DTo1D({index3D[0]+1, index3D[1], index3D[2]}, numCells);
            std::vector<int> halo = getHaloX(tmp_index, numCells);
            std::copy(halo.begin(), halo.end(), std::inserter(result, result.end()));
        }
        //cell at bottom boundary
        if (index3D[1] == 1) {
            int tmp_index = convert3DTo1D({index3D[0], 0, index3D[2]}, numCells);
            std::vector<int> halo = getHaloY(tmp_index, numCells);
            std::copy(halo.begin(), halo.end(), std::inserter(result, result.end()));
        }
        //cell at top boundary
        if (index3D[1] == numCells[1] - 2) {
            int tmp_index = convert3DTo1D({index3D[0], index3D[1]+1, index3D[2]}, numCells);
            std::vector<int> halo = getHaloY(tmp_index, numCells);
            std::copy(halo.begin(), halo.end(), std::inserter(result, result.end()));
        }

        std::vector<int> res(result.begin(), result.end());
        return res;
    }

    /**
     * @brief calculates the indices of the neighbours for a given cell, considering Newton's 3rd Law
     *
     * @param index index of the cell
     * @param numCells dimension of the cell array
     * @return indices of the neighbours
     */
    inline std::vector<int> getDomainNeighboursNewton(int index, std::array<int, 3> &numCells)
    {
        std::vector<int> result;

        // transform index to 3d, so its easier to find the index of the cell right behind
        std::array<int, 3> index3D = convert1DTo3D(index, numCells);

        // all neighbours in the front layer
        std::vector<int> frontLayerAll = getSurroundingZ(index, numCells);

        // filter out smaller indices (according to newtons 3rd law)
        for (int i : frontLayerAll)
        {
            if (i > index)
                result.push_back(i);
        }

        // all neighbours in the back layer
        // z out of bounds (pointing to halo cell)
        if (index3D[2] + 1 >= numCells[2] - 1)
            return result;

        // z + 1
        index3D[2] = index3D[2] + 1;

        // get the 1 dimensional index of the cell right behind
        int backLayerIndex = convert3DTo1D(index3D, numCells);
        // add it right away to the result
        result.push_back(backLayerIndex);

        // add all neighbours of the cell right behind to the result
        for (int i : getSurroundingZ(backLayerIndex, numCells))
        {
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
    inline std::vector<int> getSurroundingZ(int index, std::array<int, 3> &numCells)
    {
        std::vector<int> result;

        // transform index to 3 dim index for easier use
        // but we will only use x and y in this method
        std::array<int, 3> index3D = convert1DTo3D(index, numCells);

        // get all surrounding cells
        // from bottom left to top right
        std::array<int, 3> tmpIndex3D;
        for (int y = index3D[1] - 1; y <= index3D[1] + 1; y++)
        {
            // y out of bounds
            if (y < 1 || y > numCells[1] - 2)
                continue;

            for (int x = index3D[0] - 1; x <= index3D[0] + 1; x++)
            {
                // x out of bounds 
                if (x < 1 || x > numCells[0] - 2)
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

    /**
     * @brief calculates indices of neighboring halo cells in x-plane
     * @param index 1D index of cell
     * @param numCells number of cells in each dimension
     * @return 1D indices of neighbouring halo cells in x-plane
    */
    inline std::vector<int> getHaloX(int index, std::array<int, 3> &numCells) {
        std::vector<int> result;
        std::array<int, 3> index3D = convert1DTo3D(index, numCells);

        std::array<int, 3> tmpIndex3D;
        for (int y = index3D[1] - 1; y <= index3D[1] + 1; y++)
        {
            for (int z = index3D[2] - 1; z <= index3D[2] + 1; z++)
            {
                tmpIndex3D[0] = index3D[0];
                tmpIndex3D[1] = y;
                tmpIndex3D[2] = z;
                int n = convert3DTo1D(tmpIndex3D, numCells);
                result.push_back(n);
            }
        }
        return result;
    }

    /**
     * @brief calculates indices of neighboring halo cells in y-plane
     * @param index 1D index of cell
     * @param numCells number of cells in each dimension
     * @return 1D indices of neighbouring halo cells in y-plane
    */
    inline std::vector<int> getHaloY(int index, std::array<int, 3> &numCells) {
        std::vector<int> result;
        std::array<int, 3> index3D = convert1DTo3D(index, numCells);

        std::array<int, 3> tmpIndex3D;
        for (int x = index3D[0] - 1; x <= index3D[0] + 1; x++)
        {
            for (int z = index3D[2] - 1; z <= index3D[2] + 1; z++)
            {
                tmpIndex3D[0] = x;
                tmpIndex3D[1] = index3D[1];
                tmpIndex3D[2] = z;
                int n = convert3DTo1D(tmpIndex3D, numCells);
                result.push_back(n);
            }
        }
        return result;
    }

    /**
     * @brief calculates indices of neighboring halo cells in z-plane
     * @param index 1D index of cell
     * @param numCells number of cells in each dimension
     * @return 1D indices of neighbouring halo cells in z-plane
    */
    inline std::vector<int> getHaloZ(int index, std::array<int, 3> &numCells) {
        std::vector<int> result;
        std::array<int, 3> index3D = convert1DTo3D(index, numCells);

        std::array<int, 3> tmpIndex3D;
        for (int y = index3D[1] - 1; y <= index3D[1] + 1; y++)
        {
            for (int x = index3D[0] - 1; x <= index3D[0] + 1; x++)
            {
                tmpIndex3D[0] = x;
                tmpIndex3D[1] = y;
                tmpIndex3D[2] = index3D[2];
                int n = convert3DTo1D(tmpIndex3D, numCells);
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
        // z
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

    inline int crossedBoundary(int prev_cell, int new_cell, std::array<int, 3> &numCells) {
        std::array<int, 3> prev_cell_3D = convert1DTo3D(prev_cell, numCells);
        std::array<int, 3> new_cell_3D = convert1DTo3D(new_cell, numCells);

        if (new_cell_3D[0] - prev_cell_3D[0] == -1) 
            return 0;
        else if (new_cell_3D[0] - prev_cell_3D[0] == 1)
            return 1;
        else if (new_cell_3D[1] - prev_cell_3D[1] == -1)
            return 2;
        else if (new_cell_3D[1] - prev_cell_3D[1] == 1)
            return 3;
        else if (new_cell_3D[2] - prev_cell_3D[2] == -1)
            return 4;
        else if (new_cell_3D[2] - prev_cell_3D[2] == 1)
            return 5;
        else 
            throw std::invalid_argument("Cells not adjacent, cannot compute crossed boundary");
    }
}