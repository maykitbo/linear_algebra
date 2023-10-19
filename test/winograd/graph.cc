#include "image_painter/image_painter.h"

#include "O_big.cc"

#include <parser.h>

#include <iostream>



std::vector<Graph::data_t> ConvertToDataT(const vector_data_2d &values)
{
    std::vector<Graph::data_t> result(values.size());
    for (size_t i = 0; i < values.size(); ++i)
    {
        for (auto &v : values[i])
        {
            result[i].emplace_back(v.first, v.second);
        }
    }
    return result;
}

int GraphBuilder(int argc, char *argv[], std::vector<Graph::data_t> &values, const std::string &name)
{

    // setenv("LC_NUMERIC", "C", 1);
    // QApplication app(argc, argv);
    // Graph::WidgetFrame wf;
    // wf.show();
    // wf.AddGraph(double_values[0], "winograd");
    // wf.AddGraph(double_values[1], "winograd P");
    // wf.AddGraph(double_values[2], "strassen");
    // wf.AddGraph(double_values[3], "strassen P");
    // wf.AddGraph(double_values[4], "blas");
    // wf.AddGraph(double_values[5], "mul2 P");
    // wf.GetBackground()->SetXTextFunc([] (double x) {
    //     return QString::number(x);
    // });
    // return app.exec();

    Graph::ImagePainter painter(QSize(1500, 1500), argc, argv);
    painter.ReserveGraphs(5);
    painter.AddGraph(values[0], "W", QPen(Qt::red, 1), true).SetPointsPen(QPen(Qt::red, 4));
    painter.AddGraph(values[1], "WP", QPen(Qt::blue, 1), true).SetPointsPen(QPen(Qt::blue, 4));
    painter.AddGraph(values[2], "S", QPen(Qt::green, 1), true).SetPointsPen(QPen(Qt::green, 4));
    painter.AddGraph(values[3], "SP", QPen(Qt::yellow, 1), true).SetPointsPen(QPen(Qt::yellow, 4));
    painter.AddGraph(values[4], "blas", QPen(Qt::cyan, 1), true).SetPointsPen(QPen(Qt::cyan, 4));
    painter.AddGraph(values[5], "mul2P", QPen(Qt::magenta, 1), true).SetPointsPen(QPen(Qt::magenta, 4));
    painter.AddGraph(values[6], "W_reg", QPen(Qt::red, 2));
    painter.AddGraph(values[7], "WP_reg", QPen(Qt::blue, 2));
    painter.AddGraph(values[8], "S_reg", QPen(Qt::green, 2));
    painter.AddGraph(values[9], "SP_reg", QPen(Qt::yellow, 2));
    painter.AddGraph(values[10], "blas_reg", QPen(Qt::cyan, 2));
    painter.AddGraph(values[11], "mul2P_reg", QPen(Qt::magenta, 2));
    painter.GetBackground().SetXTextFunc([] (double x) {
        return QString::number(x);
    });
    painter.SavePng(QString::fromStdString(name + ".png"));
    return 0;
}

int main(int argc, char *argv[])
{
    const std::string name = "test8";
    // auto result = Parser(name, 6);
    auto result = Parser::FromFile(name, 6);
    auto reg_res = RegressionToGraphData(Regression(result), result[0]);
    for (auto &v : result) {
        v.insert(v.begin(), std::make_pair(0, 0));
    }
    result.insert(result.end(), reg_res.begin(), reg_res.end());


    // for (int k = 0; k < result[0].size(); ++k)
    // {
    //     std::cout << result[0][k].first << ' ';
    //     for (int h = 0; h < result.size(); ++h)
    //     {
    //         std::cout << result[h][k].second << ' ';
    //     }
    //     std::cout << '\n';
    // }

    return GraphBuilder(argc, argv, result, name);
    return 0;
    
}
