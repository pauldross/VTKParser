//
// Created by paul on 1/24/24.
//

#ifndef VTKPARSER_PARSE_H
#define VTKPARSER_PARSE_H
#include <string>
#include <utility>
#include <memory>
#include <set>
#include <vector>

typedef struct parse_cell {
    int cell_id;
    int cell_length;
    std::shared_ptr<std::vector<int>> point_ids;
} parse_cell;

typedef struct parse_point {
    int pid;
    double xyz[3];
    double vec[3];
    std::set<int> cells;
} parse_point;

class VTKparser {
    private:
        const std::string file_path;
        int num_cells = 0;
        int cell_list_size = 0;
        int num_points = 0;
        int num_vecs = 0;
        std::shared_ptr<std::vector<parse_cell>> cell_array;
        std::shared_ptr<std::vector<parse_point>> point_array;
        void add_cell_to_point(int pid, int cid);
    public:
        explicit VTKparser(std::string file_path) : file_path(std::move(file_path)) {}
        void print_stats() const;
        void parse();
        std::shared_ptr<std::vector<int>> getPointIDsFromCellId(int cell_id, int &numPoints);
        std::shared_ptr<std::vector<double>> getVec3FromPID(int pid);
        std::shared_ptr<std::vector<int>> getCellIDsFromPointIDs(int pid, int &numpts);
        std::shared_ptr<std::vector<parse_point>> getPointArray();
        std::shared_ptr<std::vector<parse_cell>> getCellArray();
        [[nodiscard]] int getNumCells() const;
        [[nodiscard]] int getNumPoints() const;

};


#endif //VTKPARSER_PARSE_H
