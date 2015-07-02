#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QWidget>
#include <QToolButton>

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

private:
    Ui::SettingsForm *ui;

    void restore();

    void setBtnColor(QToolButton * btn, QString propName);
};

#endif // SETTINGSFORM_H
