#ifndef C_MAPWINDOW_H
#define C_MAPWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MapWindow;
}

class MapWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MapWindow(QWidget *parent = nullptr);
    ~MapWindow();

private:
    Ui::MapWindow *ui;
};

#endif // C_MAPWINDOW_H
