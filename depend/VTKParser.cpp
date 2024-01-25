//
// Created by paul on 1/24/24.
//

#include "VTKParser.h"
#include <iostream>
#include <fstream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>

void VTKparser::print_stats() const {
    std::cout << "num cells: " <<  num_cells << std::endl;
    std::cout << "num points: " << num_points << std::endl;
    std::cout << "num vecs: " << num_vecs << std::endl;
    std::cout << "num cell list size: " << cell_list_size << std::endl;
}

void VTKparser::parse() {
    std::ifstream file(file_path);
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string keyword;
        iss >> keyword;

        if (keyword == "POINTS") {
            iss >> num_points;
            std::string type;
            iss >> type;
            point_array = std::make_unique<parse_point []>(num_points);
            for (int i = 0; i < num_points; i++){
                std::getline(file, line);
                std::istringstream issL(line);
                issL >> point_array[i].xyz[0] >> point_array[i].xyz[1] >> point_array[i].xyz[2];
            }
        } else if (keyword == "CELLS"){
            iss >> num_cells >> cell_list_size;
            cell_array = std::make_unique<parse_cell []>(num_cells);

            for (int i = 0; i < num_cells; i++) {
                std::getline(file, line);
                std::istringstream issL(line);
                std::string keywordL;
                int cellType;
                issL >> cellType;
                cell_array[i].cell_id = i;
                cell_array[i].cell_length = cellType;
                cell_array[i].point_ids = std::make_unique<int []>(cellType);
                for (int j = 0; j < cellType; ++j) {
                    int pointIndex;
                    issL >> pointIndex;
                    cell_array[i].point_ids[j] = pointIndex;
                }
            }

        } else if (keyword == "POINT_DATA") {
            std::getline(file, line);
            std::istringstream iss_meta(line);
            std::string type, name, dtype;
            iss_meta >> type >> name >> dtype;
            point_array = std::make_unique<parse_point []>(num_points);
            for (int i = 0; i < num_points; i++){
                std::getline(file, line);
                std::istringstream issL(line);
                issL >> point_array[i].vec[0] >> point_array[i].vec[1] >> point_array[i].vec[2];
            }
        }
    }
    // linking ie find all the cell ids for a point.
    for (int i = 0; i < num_cells; i++) {
        int numPoints;
        std::unique_ptr<int[]> points_in_cell = getPointIDsFromCellId(i, numPoints);
        for (int j = 0; j < numPoints; j++){
            add_cell_to_point(points_in_cell[j], i);
        }
    }
}

std::unique_ptr<int[]> VTKparser::getPointIDsFromCellId(int cell_id, int &numPoints) {
    if (!cell_array) {
        std::cerr << "Parser not initialized" << std::endl;
        return nullptr;
    }

    if (cell_id < 0 || cell_id >= num_cells) {
        std::cerr << "Invalid cell_id" << std::endl;
        return nullptr;
    }

    numPoints = cell_array[cell_id].cell_length;
    std::unique_ptr<int []> result_arr = std::make_unique<int []>(numPoints);

    for (int i = 0; i < numPoints; i++) {
        result_arr[i] = cell_array[cell_id].point_ids[i];
    }

    return result_arr;
}

std::unique_ptr<double[]> VTKparser::getVec3FromPID(int pid) {
    std::unique_ptr<double []> res_arr = std::make_unique<double []>(3);
    res_arr[0] = point_array[pid].xyz[0];
    res_arr[1] = point_array[pid].xyz[1];
    res_arr[2] = point_array[pid].xyz[2];
    return res_arr;
}

void VTKparser::add_cell_to_point(int pid, int cid) {
    point_array[pid].cells.insert(cid);
}

std::unique_ptr<int[]> VTKparser::getCellIDsFromPointIDs(int pid, int &numpts) {
    numpts = (int) point_array[pid].cells.size();
    std::unique_ptr<int []> res_array = std::make_unique<int []>(numpts);
    int i = 0;
    for (auto x : point_array[pid].cells) {
        res_array[i] = x;
        i++;
    }
    return res_array;
}

int VTKparser::getNumCells() const {
    return num_cells;
}

int VTKparser::getNumPoints() const {
    return num_points;
}
