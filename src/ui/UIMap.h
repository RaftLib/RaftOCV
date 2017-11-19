#pragma once

#include <memory>
#include <QMainWindow>
#include <raft>

namespace Ui {
class UIMap;
}

class MainWindow_p;
class UIMap2;

class UIMap : public QMainWindow, public raft::map
{
    Q_OBJECT

public:
    explicit UIMap(QWidget *parent = 0);
    ~UIMap();

    void setItem(QObject& obj);
    void addQObect(QObject* obj);
    void exec();
private slots:
    void on_listView_activated(const QModelIndex &index);

    void on_Start_clicked();

private:
    Ui::UIMap *ui;
    std::unique_ptr<MainWindow_p> p;
    friend class MainWindow_p;
};
