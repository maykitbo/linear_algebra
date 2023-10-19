#include <parser.h>

#include <iostream>
#include <eigen3/Eigen/Dense>
#include <eigen3/unsupported/Eigen/NonLinearOptimization>

using namespace Eigen;

struct LMFunctor
{
    const vector_data &data;

    LMFunctor(const vector_data& input_data) : data(input_data) {}

    int operator()(const VectorXd& params, VectorXd& residuals) const
    {
        double a = params[0];
        double b = params[1];
        double c = params[2];
        for (size_t i = 0; i < data.size(); ++i)
        {
            // residuals[i] = data[i].second - (a * pow(data[i].first, b) + c);
            residuals[i] = data[i].second - (a * pow(data[i].first, b) + c * data[i].first);
            // residuals[i] = data[i].second - a * pow(data[i].first, b);
        }
        return 0;
    }

    int df(const VectorXd& params, MatrixXd& jacobian) const
    {
        double a = params[0];
        double b = params[1];
        double c = params[2];
        for (size_t i = 0; i < data.size(); ++i)
        {
            // jacobian(i, 0) = -pow(data[i].first, b);
            // jacobian(i, 1) = -a * pow(data[i].first, b) * log(data[i].first);
            // jacobian(i, 2) = -1;
            jacobian(i, 0) = -pow(data[i].first, b);
            jacobian(i, 1) = -a * pow(data[i].first, b) * log(data[i].first);
            jacobian(i, 2) = -data[i].first;
        }
		return 0;
    }
    int inputs() const
    {
        return 3;
    }

    int values() const
    {
        return data.size();
    }
};

std::vector<double> PerformNonlinearRegression(const vector_data &data)
{
    VectorXd params(3);
    params[0] = 1;
    params[1] = 2.9;
    params[2] = 0;

    LMFunctor cost_function(data);
    LevenbergMarquardt<LMFunctor, double> lm(cost_function);
    lm.parameters.maxfev = 1e8;
    lm.parameters.xtol = 1.0e-20;
    lm.parameters.ftol = 1.0e-20;
    lm.parameters.gtol = 1.0e-20;
    
    lm.minimize(params);
    for (auto p : params)
        std::cout << p << ' ';

    std::cout << '\n';
    std::vector<double> result = {params[0], params[1], params[2]};
    // std::vector<double> result = {params[0], params[1]};
    return result;
}

std::vector<std::vector<double>> Regression(const vector_data_2d &values)
{
    std::vector<std::vector<double>> result;
    for (size_t i = 0; i < values.size(); ++i)
    {
        result.push_back(PerformNonlinearRegression(values[i]));
    }
    return result;
}

vector_data_2d RegressionToGraphData(const std::vector<std::vector<double>> &values, const vector_data &xy)
{
    vector_data_2d result(values.size());
    for (size_t i = 0; i < values.size(); ++i)
    {
        for (size_t j = 0; j < xy.size(); ++j)
        {
            result[i].emplace_back(xy[j].first, values[i][0] * pow(xy[j].first, values[i][1]) + values[i][2]);
        }
    }
    return result;
}
