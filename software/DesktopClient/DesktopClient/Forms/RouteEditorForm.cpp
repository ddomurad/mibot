#include "RouteEditorForm.h"
#include "ui_RouteEditorForm.h"
#include "./Settings/AppSettings.h"
#include <QJsonObject>

#define MAP_MENU_SELECT_LOCATION 0x01
#define MAP_MENU_ADD_MARKER_TO_ROUTE 0x02
#define MAP_MENU_ADD_MARKER_TO_ROUTE_BEFORE 0x03
#define MAP_MENU_ADD_MARKER_TO_ROUTE_AFTER 0x04
#define MAP_MENU_REMOVE_MARKER_FROM_ROUTE 0x05
#define MAP_MENU_SELECT_ROUTE 0x06
#define MAP_MENU_MAKE_ROUTE_EDITABLE 0x07

RouteEditorForm::RouteEditorForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RouteEditorForm)
{
    ui->setupUi(this);

    ui->widget->setResources(
                AppSettings::GetKey( AppSettings::OSM_Server_Address ).toString(),
                AppSettings::GetKey( AppSettings::OSM_Cache_Dir ).toString(),
                AppSettings::GetKey( AppSettings::OSM_Tiles_Format ).toString(),
                AppSettings::GetKey( AppSettings::OSM_Missing_Tile ).toString() );

    ui->widget->addMapMenu(MAP_MENU_SELECT_LOCATION ,QWidget::tr("Select location"));
    ui->widget->addMapMenu(MAP_MENU_ADD_MARKER_TO_ROUTE ,QWidget::tr("Add marker to route"));

    ui->widget->addMarkerMenu(MAP_MENU_SELECT_ROUTE, QWidget::tr("Select route"));
    ui->widget->addMarkerMenu(MAP_MENU_MAKE_ROUTE_EDITABLE, QWidget::tr("Edit On/Off"));
    ui->widget->addMarkerMenu(MAP_MENU_ADD_MARKER_TO_ROUTE_BEFORE, QWidget::tr("Add before"));
    ui->widget->addMarkerMenu(MAP_MENU_ADD_MARKER_TO_ROUTE_AFTER, QWidget::tr("Add after"));
    ui->widget->addMarkerMenu(MAP_MENU_REMOVE_MARKER_FROM_ROUTE, QWidget::tr("Remove"));


    connect(ui->widget, SIGNAL(menuAction(OSMMenuItem)), this, SLOT(on_mapMenu(OSMMenuItem)));
    loadLocationsFromFile();
    loadRoutesFromFile();

    refreshLocationList();
    refreshRouteList();

    goToLocation( AppSettings::GetKey( AppSettings::MapEdit_Start_Location_Name ).toString() );
}

RouteEditorForm::~RouteEditorForm()
{
    delete ui;
}

void RouteEditorForm::on_toolButton_add_location_clicked()
{
    MapLocation location;
    location.name = ui->lineEdit_location_name->text();
    QStringList loc = ui->lineEdit_location_pos->text().split(" ");
    if(loc.count() != 2) return;

    bool ok_x = true;
    bool ok_y = true;

    double dx = loc[0].toDouble(&ok_x);
    double dy = loc[1].toDouble(&ok_y);

    if(!ok_x && ok_y) return;

    location.loc.setX(dx);
    location.loc.setY(dy);

    _locations.append( location );
    refreshLocationList();
}

void RouteEditorForm::on_toolButton_remove_location_clicked()
{
    int currentRow = ui->listWidget_locations->currentRow();
    if(currentRow < 0) return;

    _locations.removeAt(currentRow);
    refreshLocationList();
}

void RouteEditorForm::on_mapMenu(OSMMenuItem item)
{
    if(item.id == MAP_MENU_SELECT_LOCATION)
    {
        QString pstr = QString("%1 %2").arg(item.gpsPos.x()).arg(item.gpsPos.y());
        ui->lineEdit_location_pos->setText( pstr );
        ui->widget->repaint();
        return;
    }
    else if(item.id == MAP_MENU_ADD_MARKER_TO_ROUTE)
    {
        int row = ui->listWidget_route_list->currentRow();
        if(row < 0) return;
        OSMRoute * route =  ui->widget->getRoute( row );
        route->addPoint( item.gpsPos  );
        ui->widget->repaint();
        return;
    }
    else if(item.id == MAP_MENU_ADD_MARKER_TO_ROUTE_AFTER)
    {
        for(int i=0;i < item.route->route.count(); i++)
        {
            if(item.route->route[i].name == item.marker->name)
            {
                item.route->addPoint(item.mouePos, i+1);
                ui->widget->setMarkerEdit(&item.route->route[i+1]);
                ui->widget->repaint();
                return;
            }
        }
    }
    else if(item.id == MAP_MENU_ADD_MARKER_TO_ROUTE_BEFORE)
    {
        for(int i=0;i < item.route->route.count(); i++)
        {
            if(item.route->route[i].name == item.marker->name)
            {
                item.route->addPoint(item.mouePos, i);
                ui->widget->setMarkerEdit(&item.route->route[i]);
                ui->widget->repaint();
                return;
            }
        }
    }else if(item.id == MAP_MENU_REMOVE_MARKER_FROM_ROUTE)
    {
        for(int i=0;i<item.route->route.count(); i++)
        {
            if(item.route->route[i].name == item.marker->name)
            {
                item.route->route.removeAt(i);
                ui->widget->repaint();
                return;
            }
        }
    }else if(item.id == MAP_MENU_SELECT_ROUTE)
    {
        for(int i=0; i<ui->listWidget_route_list->count(); i++)
        {
            if( ui->listWidget_route_list->item(i)->text() == item.route->name)
            {
                ui->listWidget_route_list->setCurrentRow(i);
                routeSelected(i);
                return;
            }
        }
    }else if(item.id == MAP_MENU_MAKE_ROUTE_EDITABLE)
    {
        item.route->editable = !item.route->editable;
        ui->widget->repaint();
        return;
    }
}

void RouteEditorForm::saveLocationsToFile()
{
    QJsonArray arr = QJsonArray();
    for(int i=0; i<_locations.count(); i++)
    {
        QJsonObject obj;
        obj.insert("name", QJsonValue(_locations[i].name));
        obj.insert("loc_x", QJsonValue(_locations[i].loc.x()));
        obj.insert("loc_y", QJsonValue(_locations[i].loc.y()));
        arr.append( QJsonValue(obj) );
    }

    QJsonObject finalObj;
    finalObj.insert("locations", QJsonValue(arr));

    QJsonDocument doc(finalObj);

    QFile file("./locations");
    if(!file.open(QIODevice::WriteOnly))
        return;

    file.write(doc.toJson());
    file.close();
}

void RouteEditorForm::loadLocationsFromFile()
{
    _locations.clear();
    QFile file("./locations");
    if(!file.open(QIODevice::ReadOnly))
        return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject locObj = doc.object();

    file.close();
    if(!locObj["locations"].isArray())
        return;

    QJsonArray locArray = locObj["locations"].toArray();

    for(int i=0; i<locArray.count(); i++)
    {
        QJsonObject element = locArray.at(i).toObject();
        MapLocation loc;
        loc.name = element["name"].toString();
        loc.loc.setX( element["loc_x"].toDouble() );
        loc.loc.setY( element["loc_y"].toDouble() );

        _locations.append( loc );
    }

}

void RouteEditorForm::refreshLocationList()
{
    ui->listWidget_locations->clear();

    for(int i=0; i<_locations.count(); i++)
        ui->listWidget_locations->addItem( _locations[i].name );

    saveLocationsToFile();
}

void RouteEditorForm::refreshRouteList()
{
    ui->listWidget_route_list->clear();

    for(int i=0; i<ui->widget->getRouteCount(); i++)
    {
        OSMRoute * route = ui->widget->getRoute(i);
        QListWidgetItem * item = new QListWidgetItem(route->name);
        if(!route->visible) item->setTextColor( Qt::gray );
        ui->listWidget_route_list->addItem(  item );
    }

    saveRoutesToFile();
}

void RouteEditorForm::removeLocation(QString name)
{
    for(int i=0; i<_locations.count(); i++)
    {
        if(_locations[i].name == name)
        {
            _locations.removeAt(i);
            break;
        }
    }

    refreshLocationList();
}

void RouteEditorForm::saveRoutesToFile()
{
    QFile file("./routes");
    if(!file.open(QIODevice::WriteOnly))
        return;

    QJsonObject main = QJsonObject();
    QJsonArray routes = QJsonArray();

    for(int i=0;i< ui->widget->getRouteCount(); i++)
    {
        OSMRoute * r = ui->widget->getRoute(i);
        QJsonArray routeArr = QJsonArray();
        for(int p=0;p< r->route.count(); p++)
        {
            QJsonObject point;
            point.insert("pos_x", QJsonValue(r->route[p].possition.x()));
            point.insert("pos_y", QJsonValue(r->route[p].possition.y()));
            routeArr.append( point );
        }

        QJsonObject routeObj = QJsonObject();
        routeObj.insert("points", QJsonValue(routeArr));
        routeObj.insert("visible", QJsonValue(r->visible));
        routeObj.insert("editable", QJsonValue(r->editable));
        routeObj.insert("closed", QJsonValue(r->closed));
        routeObj.insert("name", QJsonValue(r->name));

        routes.append(routeObj);
    }
    main.insert("routes",routes);

    QJsonDocument doc(main);

    file.write(doc.toJson());
    file.close();
}

void RouteEditorForm::loadRoutesFromFile()
{
    ui->widget->removeAllRoutes();

    QFile file("./routes");
    if(!file.open(QIODevice::ReadOnly))
        return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject routesObj = doc.object();

    file.close();

    if(!routesObj["routes"].isArray()) return;
    QJsonArray routesArray = routesObj["routes"].toArray();

    for(int i=0;i<routesArray.count(); i++)
    {
        QJsonObject routeObj = routesArray.at(i).toObject();
        bool closed = routeObj["closed"].toBool();
        bool visible = routeObj["visible"].toBool();
        bool editable = routeObj["editable"].toBool();
        QString name = routeObj["name"].toString();

        OSMRoute newRoute;
        newRoute.name = name;
        newRoute.closed = closed;
        newRoute.visible = visible;
        newRoute.editable = editable;

        QJsonArray routePoints = routeObj["points"].toArray();
        for(int j=0; j<routePoints.count(); j++)
        {
            QJsonObject pointObj = routePoints.at(j).toObject();
            QPointF point;
            point.setX( pointObj["pos_x"].toDouble());
            point.setY( pointObj["pos_y"].toDouble());

            newRoute.addPoint(point);
        }

        ui->widget->addRoute( newRoute );
    }
}

void RouteEditorForm::routeSelected(int row)
{
    QString txt = "";
    if(row >= 0)
        txt = ui->listWidget_route_list->item(row)->text();

    for(int i = 0; i < ui->widget->getRouteCount(); i++)
    {
        OSMRoute * route = ui->widget->getRoute(i);
        if(route->name == txt)
        {
            route->pen.setColor( AppSettings::GetKey( AppSettings::MapEdit_Selected_Route_Color ).value<QColor>() );
            ui->lineEdit_route_name->setText( route->name );
            ui->checkBox_polygon_closed->setChecked( route->closed );
            ui->checkBox_route_visible->setChecked( route->visible );
            ui->checkBox_route_is_editable->setChecked( route->editable );
        }
        else
            route->pen.setColor(Qt::black);
    }

    ui->widget->repaint();
}

void RouteEditorForm::goToLocation(QString name)
{
    for(MapLocation loc : _locations)
    {
        if(loc.name == name)
        {
            ui->widget->setGPSAndZoom( loc.loc, 17, ui->widget->widgetCenter() );
            break;
        }
    }
}

void RouteEditorForm::on_listWidget_locations_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    QPointF gpsPos = _locations[row].loc;
    ui->widget->setGPSAndZoom( gpsPos, 17, ui->widget->widgetCenter() );
}

void RouteEditorForm::on_toolButton_edit_location_clicked()
{
    int row = ui->listWidget_locations->currentRow();
    if(row < 0) return;

    QStringList loc = ui->lineEdit_location_pos->text().split(" ");
    if(loc.count() != 2) return;

    bool ok_x = true;
    bool ok_y = true;

    double dx = loc[0].toDouble(&ok_x);
    double dy = loc[1].toDouble(&ok_y);

    if(!ok_x && ok_y) return;

    _locations[row].name = ui->lineEdit_location_name->text();
    _locations[row].loc.setX( dx );
    _locations[row].loc.setY( dy );

    refreshLocationList();
}

void RouteEditorForm::on_listWidget_locations_clicked(const QModelIndex &index)
{
    int row = index.row();
    if(row < 0) return;

    ui->lineEdit_location_name->setText( _locations[row].name );
    ui->lineEdit_location_pos->setText( QString("%1 %2").arg(_locations[row].loc.x()).arg(_locations[row].loc.y()) );
}

void RouteEditorForm::on_toolButton_add_route_clicked()
{
    QString route_name = ui->lineEdit_route_name->text();
    if(route_name == "") return;

    OSMRoute route;
    route.name = route_name;
    route.closed = ui->checkBox_polygon_closed->isChecked();
    route.visible = ui->checkBox_route_visible->isChecked();
    route.editable = ui->checkBox_route_is_editable->isChecked();
    ui->widget->addRoute(route);

    refreshRouteList();
}

void RouteEditorForm::on_listWidget_route_list_clicked(const QModelIndex &index)
{
    routeSelected(index.row());
}

void RouteEditorForm::on_toolButton_route_edit_clicked()
{
    int row = ui->listWidget_route_list->currentRow();
    if(row < 0) return;
    QString name = ui->listWidget_route_list->item( row )->text();
    OSMRoute * route = ui->widget->getRoute( name );
    if(route == nullptr) return;

    QString new_name = ui->lineEdit_route_name->text();
    if(new_name != route->name)
    {
        if(ui->widget->getRoute( new_name ) == nullptr)
            route->name = new_name;
        else
          ui->lineEdit_route_name->setText( route->name );
    }

    route->closed = ui->checkBox_polygon_closed->isChecked();
    route->visible = ui->checkBox_route_visible->isChecked();
    route->editable = ui->checkBox_route_is_editable->isChecked();

    refreshRouteList();
}

void RouteEditorForm::on_toolButton_route_delete_clicked()
{
    int row = ui->listWidget_route_list->currentRow();
    if(row < 0) return;
    QString name = ui->listWidget_route_list->item( row )->text();
    ui->widget->removeRoute(name);
    refreshRouteList();
}

void RouteEditorForm::on_toolButton_save_routes_clicked()
{
    saveRoutesToFile();
}

void RouteEditorForm::on_listWidget_route_list_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    if(row < 0) return;

    QString name = ui->listWidget_route_list->item(row)->text();
    OSMRoute *route =  ui->widget->getRoute(name);
    if(route == nullptr) return;

    OSMMarker * firstMarker = route->getMarker(0);
    if(firstMarker == nullptr) return;

    ui->widget->setGPSPossition( firstMarker->possition, ui->widget->widgetCenter() );
}
