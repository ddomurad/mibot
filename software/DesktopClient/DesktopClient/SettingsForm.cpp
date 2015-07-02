#include "SettingsForm.h"
#include "ui_SettingsForm.h"
#include "AppSettings.h"
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QColorDialog>

SettingsForm::SettingsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsForm)
{
    ui->setupUi(this);
    restore();
}

SettingsForm::~SettingsForm()
{
    delete ui;
}

void SettingsForm::restore()
{
    ui->lineEdit_osm_server->setText( AppSettings::GetKey(AppSettings::OSM_Server_Address).toString() );
    ui->lineEdit_missing_tile->setText( AppSettings::GetKey(AppSettings::OSM_Missing_Tile).toString() );
    ui->lineEdit_cache_dir->setText( AppSettings::GetKey(AppSettings::OSM_Cache_Dir).toString() );
    ui->comboBox_tiles_format->setCurrentText( AppSettings::GetKey(AppSettings::OSM_Tiles_Format).toString() );

    ui->lineEdit_locations_storage_file->setText( AppSettings::GetKey(AppSettings::MapEdit_Locations_File).toString() );
    ui->lineEdit_routes_storage_file->setText( AppSettings::GetKey(AppSettings::MapEdit_Routes_File).toString() );
    ui->doubleSpinBox_route_opacity->setValue( AppSettings::GetKey(AppSettings::MapEdit_Enabled_Route_Opacity).toDouble() );
    ui->doubleSpinBox_not_route_opacity->setValue( AppSettings::GetKey(AppSettings::MapEdit_Disabled_Route_Opacity).toDouble() );
    ui->lineEdit_start_location_name->setText( AppSettings::GetKey(AppSettings::MapEdit_Start_Location_Name).toString() );

    setBtnColor(ui->toolButton_select_selected_route_color, AppSettings::MapEdit_Selected_Route_Color);
    setBtnColor(ui->toolButton_select_selected_marker_color, AppSettings::MapEdit_Active_Marker_Color);
    setBtnColor(ui->toolButton_select_marker_color, AppSettings::MapEdit_Marker_Color);
}

void SettingsForm::setBtnColor(QToolButton *btn, QString propName)
{
    QColor color = AppSettings::GetKey( propName ).value<QColor>();
    QString qss = QString("background-color: %1").arg(color.name());
    btn->setStyleSheet(qss);
}

void SettingsForm::on_pushButton_save_settings_clicked()
{
    AppSettings::get()->SaveAllCachedSettings();
}

void SettingsForm::on_pushButton_cancel_changes_clicked()
{
    AppSettings::get()->ClearCachedSettings();
}

void SettingsForm::on_lineEdit_osm_server_textChanged(const QString &text)
{
    AppSettings::SetKey(AppSettings::OSM_Server_Address, QVariant(text));
}

void SettingsForm::on_lineEdit_missing_tile_textChanged(const QString &text)
{
    AppSettings::SetKey(AppSettings::OSM_Missing_Tile, QVariant(text));
}

void SettingsForm::on_comboBox_tiles_format_currentTextChanged(const QString &text)
{
    AppSettings::SetKey( AppSettings::OSM_Tiles_Format, QVariant(text));
}

void SettingsForm::on_lineEdit_cache_dir_textChanged(const QString &text)
{
    AppSettings::SetKey( AppSettings::OSM_Cache_Dir, QVariant(text));
}

void SettingsForm::on_pushButton_restore_settings_clicked()
{
    AppSettings::ClearCachedSettings();
    restore();
}

void SettingsForm::on_pushButton_clear_cache_clicked()
{
    int btn =
            QMessageBox::warning(this, QMessageBox::tr("Clear cache data"),
                         QMessageBox::tr("While clearing cahce data you can lose other files. Continue?"),
                         QMessageBox::Yes, QMessageBox::Cancel);

    if(btn != QMessageBox::Yes) return;

    QDirIterator dirs( AppSettings::GetKey( AppSettings::OSM_Cache_Dir).toString() );

    QString dir;
    while( (dir = dirs.next()) != "")
    {
        if(dir.endsWith("/.") || dir.endsWith("/..")) continue;
        QDir dirFiles = (dir);
        QStringList files = dirFiles.entryList();
        for(QString file : files)
        {
            if(file.startsWith("tile_")
                && (file.endsWith(".jpeg") || file.endsWith(".png")))
            {
                QString file_to_remove = QString("%1/%2").arg(dir).arg(file);
                QFile(file_to_remove).remove();
            }
        }
    }

}

void SettingsForm::on_toolButton_missing_tile_pick_directory_clicked()
{
    QString fname = QFileDialog::getOpenFileName(this, QFileDialog::tr("Select Error Tile"), QDir::currentPath());
    if(!fname.isEmpty())
    {
        ui->lineEdit_missing_tile->setText( fname );
    }
}

void SettingsForm::on_toolButton_cache_pick_directory_clicked()
{
    QString fname = QFileDialog::getExistingDirectory(this, QFileDialog::tr("Get Cache Directory"),QDir::currentPath());
    if(!fname.isEmpty())
    {
        ui->lineEdit_cache_dir->setText( fname );
    }
}

void SettingsForm::on_lineEdit_locations_storage_file_textChanged(const QString &text)
{
    AppSettings::SetKey( AppSettings::MapEdit_Locations_File, QVariant(text) );
}

void SettingsForm::on_lineEdit_routes_storage_file_textChanged(const QString &text)
{
    AppSettings::SetKey( AppSettings::MapEdit_Routes_File, QVariant(text) );
}

void SettingsForm::on_toolButton_select_location_file_clicked()
{
    QString fname = QFileDialog::getOpenFileName(this, QFileDialog::tr("Select Locations File"), QDir::currentPath());
    if(!fname.isEmpty())
    {
        ui->lineEdit_locations_storage_file->setText( fname );
    }
}

void SettingsForm::on_toolButton_select_routes_file_clicked()
{
    QString fname = QFileDialog::getOpenFileName(this, QFileDialog::tr("Select Route File"), QDir::currentPath());
    if(!fname.isEmpty())
    {
        ui->lineEdit_routes_storage_file->setText( fname );
    }
}

void SettingsForm::on_doubleSpinBox_route_opacity_valueChanged(double val)
{
    AppSettings::SetKey( AppSettings::MapEdit_Enabled_Route_Opacity, QVariant(val) );
}

void SettingsForm::on_doubleSpinBox_not_route_opacity_valueChanged(double val)
{
 AppSettings::SetKey( AppSettings::MapEdit_Disabled_Route_Opacity, QVariant(val) );
}

void SettingsForm::on_toolButton_select_marker_color_clicked()
{
    QColor initialColor = AppSettings::GetKey( AppSettings::MapEdit_Marker_Color ).value<QColor>();
    QColor newColor = QColorDialog::getColor(initialColor,this, QColorDialog::tr("Marker Color"));
    if(!newColor.isValid()) return;
    QVariant newColorV = newColor;
    AppSettings::SetKey( AppSettings::MapEdit_Marker_Color, newColorV );
    setBtnColor(ui->toolButton_select_marker_color, AppSettings::MapEdit_Marker_Color);
}

void SettingsForm::on_toolButton_select_selected_marker_color_clicked()
{
    QColor initialColor = AppSettings::GetKey( AppSettings::MapEdit_Active_Marker_Color ).value<QColor>();
    QColor newColor = QColorDialog::getColor(initialColor,this, QColorDialog::tr("Active Marker Color"));
    if(!newColor.isValid()) return;
    QVariant newColorV = newColor;
    AppSettings::SetKey( AppSettings::MapEdit_Active_Marker_Color, newColorV );
    setBtnColor(ui->toolButton_select_selected_marker_color, AppSettings::MapEdit_Active_Marker_Color);
}

void SettingsForm::on_toolButton_select_selected_route_color_clicked()
{
    QColor initialColor = AppSettings::GetKey( AppSettings::MapEdit_Selected_Route_Color ).value<QColor>();
    QColor newColor = QColorDialog::getColor(initialColor,this, QColorDialog::tr("Selected Route Color"));
    if(!newColor.isValid()) return;
    QVariant newColorV = newColor;
    AppSettings::SetKey( AppSettings::MapEdit_Selected_Route_Color, newColorV );
    setBtnColor(ui->toolButton_select_selected_route_color, AppSettings::MapEdit_Selected_Route_Color);
}

void SettingsForm::on_lineEdit_start_location_name_textChanged(const QString &text)
{
    AppSettings::SetKey( AppSettings::MapEdit_Start_Location_Name, QVariant(text) );
}
