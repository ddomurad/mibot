#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QWidget>
#include <QToolButton>

#include "Other/JsInput.h"

namespace Ui {
class SettingsForm;
}

class SettingsForm : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsForm(QWidget *parent = 0);
    ~SettingsForm();


private slots:
    void on_pushButton_save_settings_clicked();

    void on_pushButton_cancel_changes_clicked();

    void on_lineEdit_osm_server_textChanged(const QString &text);

    void on_lineEdit_missing_tile_textChanged(const QString &arg1);

    void on_comboBox_tiles_format_currentTextChanged(const QString &arg1);

    void on_lineEdit_cache_dir_textChanged(const QString &arg1);

    void on_pushButton_restore_settings_clicked();

    void on_pushButton_clear_cache_clicked();

    void on_toolButton_missing_tile_pick_directory_clicked();

    void on_toolButton_cache_pick_directory_clicked();

    void on_lineEdit_locations_storage_file_textChanged(const QString &text);

    void on_lineEdit_routes_storage_file_textChanged(const QString &arg1);

    void on_toolButton_select_location_file_clicked();

    void on_toolButton_select_routes_file_clicked();

    void on_doubleSpinBox_route_opacity_valueChanged(double arg1);

    void on_doubleSpinBox_not_route_opacity_valueChanged(double arg1);

    void on_toolButton_select_marker_color_clicked();

    void on_toolButton_select_selected_marker_color_clicked();

    void on_toolButton_select_selected_route_color_clicked();

    void on_lineEdit_start_location_name_textChanged(const QString &arg1);

    void on_spinBox_rover_info_port_valueChanged(const QString &arg1);

    void on_lineEdit_rover_addr_textChanged(const QString &arg1);

    void on_lineEdit_cert_path_textChanged(const QString &arg1);

    void on_pushButton_open_cert_clicked();

    void on_pushButton_connection_test_clicked();

    void on_checkBox_infi_with_ssl_toggled(bool checked);

    void on_lineEdit_private_key_textChanged(const QString &arg1);

    void on_lineEdit_ca_certs_textChanged(const QString &arg1);

    void on_pushButton_open_pk_clicked();

    void on_pushButton_open_ca_cert_clicked();

    void on_pushButton_reset_pass_clicked();

    void on_lineEdit_sensors_service_textChanged(const QString &arg1);

    void on_lineEdit_driver_service_textChanged(const QString &arg1);

    void on_lineEdit_video_service_textChanged(const QString &arg1);

    void on_pushButton_test_device_clicked();

    void on_lineEdit_device_path_textChanged(const QString &arg1);

    void on_spinBox_drive_valueChanged(int arg1);

    void on_spinBox_turn_valueChanged(int arg1);

    void on_spinBox_send_ms_valueChanged(int arg1);

    void on_spinBox_turbo_valueChanged(int arg1);

    void on_spinBox_brake_valueChanged(int arg1);

    void on_spinBox_horn_valueChanged(int arg1);

    void on_checkBox_run_test_toggled(bool checked);

    void on_checkBox_swap_drive_toggled(bool checked);

    void on_checkBox_swap_turn_toggled(bool checked);

    void on_js_update();

    void on_toolButton_get_recotd_Storage_dir_clicked();

    void on_lineEdit_record_Storage_dir_textChanged(const QString &arg1);

    void on_lineEdit_autopilot_textChanged(const QString &arg1);

private:
    Ui::SettingsForm *ui;

    void restore();

    void setBtnColor(QToolButton * btn, QString propName);

    JsInput * _js_input;
    QTimer *_js_update_timer;
};


#endif // SETTINGSFORM_H
