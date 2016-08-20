#ifndef ROUTEEDITORFORM_H
#define ROUTEEDITORFORM_H

#include <QWidget>
#include "./Forms/OSMWidget.h"

namespace Ui {
class RouteEditorForm;
}

class MapLocation
{
public:
    QString name;
    QPointF loc;
};

class RouteEditorForm : public QWidget
{
    Q_OBJECT

public:
    explicit RouteEditorForm(QWidget *parent = 0);
    ~RouteEditorForm();

private slots:
    void on_toolButton_add_location_clicked();
    void on_toolButton_remove_location_clicked();

    void on_mapMenu(OSMMenuItem item);
    void on_listWidget_locations_doubleClicked(const QModelIndex &index);

    void on_toolButton_edit_location_clicked();

    void on_listWidget_locations_clicked(const QModelIndex &index);

    void on_toolButton_add_route_clicked();

    void on_listWidget_route_list_clicked(const QModelIndex &index);

    void on_toolButton_route_edit_clicked();

    void on_toolButton_route_delete_clicked();

    void on_toolButton_save_routes_clicked();

    void on_listWidget_route_list_doubleClicked(const QModelIndex &index);

private:
    Ui::RouteEditorForm *ui;
    QList<MapLocation> _locations;

    void saveLocationsToFile();
    void loadLocationsFromFile();
    void refreshLocationList();
    void refreshRouteList();
    void removeLocation(QString name);

    void saveRoutesToFile();
    void loadRoutesFromFile();

    void routeSelected(int row);
    void goToLocation(QString name);

    QString getLocationsFilePath();
    QString getRoutestFilePath();
};

#endif // ROUTEEDITORFORM_H
