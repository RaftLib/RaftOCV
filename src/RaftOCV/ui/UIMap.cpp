#include <iostream>
#include <thread>
#include <QtWidgets/QApplication>
#include "UIMap.h"
#include "ui_UIMap.h"

struct MainWindow_p  : public QAbstractListModel {
    std::vector<QObject*> objects;

    QModelIndex parent(const QModelIndex &child) const override {
        return QModelIndex();
    }

    int rowCount(const QModelIndex &parent) const override {
        return objects.size();
    }

    int columnCount(const QModelIndex &parent) const override {
        return 1;
    }

    QVariant data(const QModelIndex &index, int role) const override {
        if(role == Qt::DisplayRole) {
            auto name = objects[index.row()]->objectName();
            if (name.isEmpty()) {
                name = typeid(*objects[index.row()]).name();
            }
            return name;
        }
        return QVariant();
    }
};

UIMap::UIMap(QWidget *parent) :
    QMainWindow(parent),
    p(new MainWindow_p),
    ui(new Ui::UIMap)
{
    ui->setupUi(this);
    this->ui->listView->setModel(p.get());
}

UIMap::~UIMap()
{
    delete ui;
}

void UIMap::setItem(QObject &obj) {
    this->ui->propertiesView->setObject(&obj);
}

void UIMap::addQObect(QObject *obj) {
    p->objects.push_back(obj);
}

void UIMap::on_listView_activated(const QModelIndex &index)
{

    this->ui->propertiesView->setObject(p->objects[index.row()]);
}

void UIMap::on_Start_clicked()
{
    this->ui->Start->setEnabled(false);
    std::thread([&] {
        this->exe();
    }).detach();
}

void UIMap::exec() {
        auto& kernels = this->all_kernels.acquire();
        for(auto& k : kernels) {
            if(auto qobj = dynamic_cast<QObject*>(k)) {
                addQObect(qobj);
            }
        }
        this->all_kernels.release();
    this->show();
    QApplication::exec();
}
