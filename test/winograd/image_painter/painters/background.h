#pragma once

#include "abstract.h"

namespace Graph {

class Background;

class Background : public AbstractPainter {
 public:
  Background(Parameters &params, const QColor &color = {15, 15, 30},
             const QPen &axes_pen = {QColor{184, 134, 11}, 2},
             const QPen &grid_pen = {QColor{200, 200, 200, 32}, 1},
             const QFont &text_font = {"Arial"},
             QImage::Format format = QImage::Format_RGB16);

  Background(const Background &other) = default;
  Background(Background &&other) = default;
  virtual ~Background() = default;

  void Draw() override;

  void SetAxes(bool axes) noexcept;
  void SetColor(const QColor &color) noexcept;
  void SetHorizontalGrid(bool grid) noexcept;
  void SetVerticalGrid(bool grid) noexcept;
  void SetGrid(bool grid) noexcept;
  void SetShowText(bool show) noexcept;

  void SetVerticalGridStep(qreal step) noexcept;
  void SetHorizontalGridStep(qreal step) noexcept;
  void SetBottomTextIndent(qreal indent) noexcept;
  void SetLeftTextIndent(qreal indent) noexcept;
  void SetAxesPen(const QPen &pen) noexcept;
  void SetGridPen(const QPen &pen) noexcept;
  void SetTextFont(const QFont &font) noexcept;
  void SetTextPen(const QPen &pen) noexcept;

  const QFont &GetTextFont() const noexcept;
  const QColor &GetColor() const noexcept;
  const QPen &GetAxesPen() const noexcept;
  const QPen &GetGridPen() const noexcept;
  const QPen &GetTextPen() const noexcept;
  qreal GetHorizontalGridStep() const noexcept;
  qreal GetVerticalGridStep() const noexcept;

  using TextFunc = std::function<QString(double)>;
  void SetXTextFunc(const TextFunc &func) noexcept;
  void SetYTextFunc(const TextFunc &func) noexcept;

  static QString NumberFormat(double n);
  static QString DateFormatSec(double n);
  static QString DateFormatDay(double n);

 protected:
  bool axes_{true};
  bool horizontal_grid_{true};
  bool vertical_grid_{true};
  bool show_text_{false};
  QColor color_;
  qreal vertical_grid_step_{100};
  qreal horizontal_grid_step_{100};
  qreal bottom_text_indent_{20};
  qreal left_text_indent_{5};
  QPen axes_pen_;
  QPen grid_pen_;
  QPen text_pen_;
  QFont text_font_;

  TextFunc x_text_func_{&Background::DateFormatDay};
  TextFunc y_text_func_{&Background::NumberFormat};

 private:
  template <class T>
  void Setter(T &var, T val) noexcept {
    if (var == val) return;

    var = val;
    // Draw();
  }
};

}  // namespace Graph
