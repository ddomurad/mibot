#include <QtNetwork>
#include "SettingsForm.h"
#include "ui_SettingsForm.h"
#include "./Settings/AppSettings.h"
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QColorDialog>
#include "./Clients/Socket.h"
#include "./Clients/GlobalPasswordProvider.h"

SettingsForm::SettingsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsForm)
{
    ui->setupUi(this);
    _js_input = new JsInput(this);
    _js_update_timer = new QTimer(this);

    connect(_js_update_timer, SIGNAL(timeout()), this, SLOT(on_js_update()));

    restore();
}

SettingsForm::~SettingsForm()
{
    _js_input->Stop();
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

    ui->lineEdit_rover_addr->setText(AppSettings::GetKey(AppSettings::Connection_Rover_Addr).toString());
    ui->spinBox_rover_info_port->setValue(AppSettings::GetKey(AppSettings::Connection_Rover_Info_Port).toInt());
    ui->lineEdit_cert_path->setText(AppSettings::GetKey(AppSettings::Connection_Cert_Path).toString());
    ui->lineEdit_ca_certs->setText(AppSettings::GetKey(AppSettings::Connection_CA_Path).toString());
    ui->lineEdit_private_key->setText(AppSettings::GetKey(AppSettings::Connection_PK_Path).toString());

    ui->checkBox_infi_with_ssl->setChecked(AppSettings::GetKey(AppSettings::Connection_Rover_Info_SSL).toBool());

    ui->lineEdit_sensors_service->setText(AppSettings::GetKey(AppSettings::Connection_Status_Service).toString());
    ui->lineEdit_driver_service->setText(AppSettings::GetKey(AppSettings::Connection_Driver_Service).toString());
    ui->lineEdit_video_service->setText(AppSettings::GetKey(AppSettings::Connection_Video_Service).toString());
    ui->lineEdit_autopilot->setText(AppSettings::GetKey(AppSettings::Connection_Autopilot_Service).toString());

    ui->lineEdit_device_path->setText(AppSettings::GetKey(AppSettings::JS_Device_Path).toString());
    ui->spinBox_drive->setValue(AppSettings::GetKey(AppSettings::JS_Drive_Axis).toInt());
    ui->spinBox_turn->setValue(AppSettings::GetKey(AppSettings::JS_Turn_Axis).toInt());
    ui->spinBox_turbo->setValue(AppSettings::GetKey(AppSettings::JS_Turbo_Btn).toInt());
    ui->spinBox_brake->setValue(AppSettings::GetKey(AppSettings::JS_Brake_Btn).toInt());
    ui->spinBox_horn->setValue(AppSettings::GetKey(AppSettings::JS_Horn_Btn).toInt());
    ui->spinBox_send_ms->setValue(AppSettings::GetKey(AppSettings::JS_Send_Ms).toInt());
    ui->checkBox_swap_drive->setChecked(AppSettings::GetKey(AppSettings::JS_Swap_Drive).toBool());
    ui->checkBox_swap_turn->setChecked(AppSettings::GetKey(AppSettings::JS_Swap_Trun).toBool());

    ui->lineEdit_record_Storage_dir->setText(AppSettings::GetKey(AppSettings::Recording_Store_Dir).toString());
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

void SettingsForm::on_spinBox_rover_info_port_valueChanged(const QString &val)
{
    AppSettings::SetKey( AppSettings::Connection_Rover_Info_Port, QVariant(val.toInt()) );
}

void SettingsForm::on_lineEdit_rover_addr_textChanged(const QString &text)
{
    AppSettings::SetKey( AppSettings::Connection_Rover_Addr, QVariant(text) );
}

void SettingsForm::on_lineEdit_cert_path_textChanged(const QString &text)
{
    AppSettings::SetKey( AppSettings::Connection_Cert_Path, QVariant(text) );
}

void SettingsForm::on_pushButton_open_cert_clicked()
{
    QString fname = QFileDialog::getOpenFileName(this, QFileDialog::tr("Select Certificate file"), QDir::currentPath());
    if(!fname.isEmpty())
    {
        ui->lineEdit_cert_path->setText( fname );
    }
}

void SettingsForm::on_pushButton_connection_test_clicked()
{
    Socket * socket = new Socket("TestConnection", ui->checkBox_infi_with_ssl->isChecked(),this);

    if(!socket->ConnectToRover(AppSettings::GetKey(AppSettings::Connection_Rover_Info_Port).toInt()))
    {
        QMessageBox::critical(this, "Failure", "Can't connect to host.");
        GlobalPasswordProvider::Clear();
    }
    else
        QMessageBox::information(this,"Success", "Test connection success.");

    socket->close();
    delete socket;
}

void SettingsForm::on_checkBox_infi_with_ssl_toggled(bool checked)
{
    AppSettings::SetKey( AppSettings::Connection_Rover_Info_SSL, QVariant(checked) );
}

void SettingsForm::on_lineEdit_private_key_textChanged(const QString &text)
{
    AppSettings::SetKey( AppSettings::Connection_PK_Path, QVariant(text) );
}

void SettingsForm::on_lineEdit_ca_certs_textChanged(const QString &text)
{
    AppSettings::SetKey( AppSettings::Connection_CA_Path, QVariant(text) );
}

void SettingsForm::on_pushButton_open_pk_clicked()
{
    QString fname = QFileDialog::getOpenFileName(this, QFileDialog::tr("Select Private key file"),
                        AppSettings::GetKey(AppSettings::Connection_Cert_Path).toString());

    if(!fname.isEmpty())
    {
        ui->lineEdit_private_key->setText( fname );
    }
}

void SettingsForm::on_pushButton_open_ca_cert_clicked()
{
    QString fname = QFileDialog::getOpenFileName(this, QFileDialog::tr("Select CA Certificate file"), QDir::currentPath());
    if(!fname.isEmpty())
    {
        ui->lineEdit_ca_certs->setText( fname );
    }
}

void SettingsForm::on_pushButton_reset_pass_clicked()
{
    GlobalPasswordProvider::Clear();
    GlobalPasswordProvider::Get();
}


void SettingsForm::on_lineEdit_sensors_service_textChanged(const QString &arg1)
{
    AppSettings::SetKey(AppSettings::Connection_Status_Service, QVariant(arg1));
}

void SettingsForm::on_lineEdit_driver_service_textChanged(const QString &arg1)
{
    AppSettings::SetKey(AppSettings::Connection_Driver_Service, QVariant(arg1));
}

void SettingsForm::on_lineEdit_video_service_textChanged(const QString &arg1)
{
    AppSettings::SetKey(AppSettings::Connection_Video_Service, QVariant(arg1));
}

void SettingsForm::on_lineEdit_autopilot_textChanged(const QString &arg1)
{
    AppSettings::SetKey(AppSettings::Connection_Autopilot_Service, QVariant(arg1));
}

void SettingsForm::on_pushButton_test_device_clicked()
{
    QString js_file = ui->lineEdit_device_path->text();
    QFile file(js_file);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical( this, "Joystick test.","Joystic input file opening error.");
    }else
    {
        QMessageBox::information( this, "Joystick test.","Joystic input file opening sucess.");
        file.close();
    }
}

void SettingsForm::on_lineEdit_device_path_textChanged(const QString &arg1)
{
    AppSettings::SetKey(AppSettings::JS_Device_Path, arg1);
}

void SettingsForm::on_spinBox_drive_valueChanged(int arg1)
{
    AppSettings::SetKey(AppSettings::JS_Drive_Axis, arg1);
}

void SettingsForm::on_spinBox_turn_valueChanged(int arg1)
{
    AppSettings::SetKey(AppSettings::JS_Turn_Axis, arg1);
}

void SettingsForm::on_spinBox_send_ms_valueChanged(int arg1)
{
    AppSettings::SetKey(AppSettings::JS_Send_Ms, arg1);
}

void SettingsForm::on_spinBox_turbo_valueChanged(int arg1)
{
    AppSettings::SetKey(AppSettings::JS_Turbo_Btn, arg1);
}

void SettingsForm::on_spinBox_brake_valueChanged(int arg1)
{
    AppSettings::SetKey(AppSettings::JS_Brake_Btn, arg1);
}

void SettingsForm::on_spinBox_horn_valueChanged(int arg1)
{
    AppSettings::SetKey(AppSettings::JS_Horn_Btn, arg1);
}

void SettingsForm::on_checkBox_swap_drive_toggled(bool checked)
{
    AppSettings::SetKey(AppSettings::JS_Swap_Drive, checked);
}

void SettingsForm::on_checkBox_swap_turn_toggled(bool checked)
{
    AppSettings::SetKey(AppSettings::JS_Swap_Trun, checked);
}


void SettingsForm::on_checkBox_run_test_toggled(bool checked)
{
    if(checked)
    {
        _js_input->Start(AppSettings::GetKey(AppSettings::JS_Device_Path).toString());
        _js_update_timer->start(100);
    }
    else
    {
        _js_update_timer->stop();
        _js_input->Stop();
    }
}

void SettingsForm::on_js_update()
{
    int da = AppSettings::GetKey(AppSettings::JS_Drive_Axis).toInt();
    int ta = AppSettings::GetKey(AppSettings::JS_Turn_Axis).toInt();

    int tb = AppSettings::GetKey(AppSettings::JS_Turbo_Btn).toInt();
    int bb = AppSettings::GetKey(AppSettings::JS_Brake_Btn).toInt();
    int hb = AppSettings::GetKey(AppSettings::JS_Horn_Btn).toInt();

    bool sd = AppSettings::GetKey(AppSettings::JS_Swap_Drive).toBool();
    bool st = AppSettings::GetKey(AppSettings::JS_Swap_Trun).toBool();

    int dav = _js_input->axis[da];
    int tav = _js_input->axis[ta];

    bool tbv = _js_input->btn[tb];
    bool bbv = _js_input->btn[bb];
    bool hbv = _js_input->btn[hb];


    ui->label_drive_axis->setText(QString::number(dav * (sd ? -1 : 1)));
    ui->label_turn_axis->setText(QString::number(tav * (st ? -1 : 1)));
    ui->label_turbo_btn->setText( tbv ? "1" : "0");
    ui->label_break_btn->setText( bbv ? "1" : "0");
    ui->label_horn_bnt->setText( hbv ? "1" : "0");
}

void SettingsForm::on_toolButton_get_recotd_Storage_dir_clicked()
{
    QString fname = QFileDialog::getExistingDirectory(this, QFileDialog::tr("Get Record Directory"),QDir::currentPath());
    if(!fname.isEmpty())
    {
        ui->lineEdit_record_Storage_dir->setText( fname );
    }
}

void SettingsForm::on_lineEdit_record_Storage_dir_textChanged(const QString &arg1)
{
    AppSettings::SetKey(AppSettings::Recording_Store_Dir, arg1);
}

