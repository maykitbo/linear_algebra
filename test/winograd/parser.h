#pragma once

#include <fstream>
#include <vector>

typedef std::vector<std::vector<int64_t>> input_data;
typedef std::vector<std::pair<double, double>> vector_data;
typedef std::vector<vector_data> vector_data_2d;
// typedef std::vector<std::vector<double>> double_data;


struct Parser
{
    static void ToFile(const std::string &file_name, const input_data &data)
    {
        std::ofstream file(file_name + ".txt");
        for (unsigned k = 0; k < data[0].size(); ++k)
        {
            for (unsigned i = 0; i < data.size(); ++i)
            {
                file << data[i][k] << ' ';
            }
            file << '\n';
        }
        file.close();
    }
    static vector_data_2d FromFile(const std::string &file_name, unsigned k)
    {
        std::ifstream file(file_name + ".txt");
        vector_data_2d result(k);
        while (true)
        {
            int64_t x = 0;
            file >> x;
            if (x == 0) break;
            for (auto &v : result) {
                int64_t value = 0;
                file >> value;
                v.emplace_back(x, value);
            }
        }
        file.close();
        return result;
    }
};
