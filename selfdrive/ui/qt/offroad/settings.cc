#include "settings.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

#ifndef QCOM
#include "selfdrive/ui/qt/offroad/networking.h"
#endif
#include "selfdrive/common/params.h"
#include "selfdrive/common/util.h"
#include "selfdrive/hardware/hw.h"
#include "selfdrive/ui/qt/widgets/controls.h"
#include "selfdrive/ui/qt/widgets/input.h"
#include "selfdrive/ui/qt/widgets/offroad_alerts.h"
#include "selfdrive/ui/qt/widgets/scrollview.h"
#include "selfdrive/ui/qt/widgets/ssh_keys.h"
#include "selfdrive/ui/qt/widgets/toggle.h"
#include "selfdrive/ui/ui.h"

TogglesPanel::TogglesPanel(QWidget *parent) : QWidget(parent) {
  QVBoxLayout *toggles_list = new QVBoxLayout();

  QList<ParamControl*> toggles;

  toggles.append(new ParamControl("OpenpilotEnabledToggle",
                                  "启用 openpilot",
                                  "使用openpilot系统进行自适应巡航控制和车道保持驾驶员辅助。要使用此功能，您需要随时注意.",
                                  "../assets/offroad/icon_openpilot.png",
                                  this));
  toggles.append(new ParamControl("IsLdwEnabled",
                                  "启用车道偏离警告",
                                  "当时速超过50km每小时，车辆压线就会发出报警提示",
                                  "../assets/offroad/icon_warning.png",
                                  this));
  toggles.append(new ParamControl("GasPressNoquit",
                                  "踩油门不退出OP",
                                  "踩油门op也能继续控制转向",
                                  "../assets/offroad/icon_gas.png",
                                  this));
  toggles.append(new ParamControl("Driverlook",
                                  "关闭驾驶员监控提醒",
                                  "请专心驾驶！！",
                                  "../assets/offroad/icon_driver.png",
                                  this));
  toggles.append(new ParamControl("Turn_Lamp",
                                  "打转向灯取消控制",
                                  "打转向灯之后暂时取消op对方向盘的控制，但是这样就不能自动变道啦！",
                                  "../assets/offroad/icon_Turn_Lamp.png",
                                  this));
  toggles.append(new ParamControl("IsRHD",
                                  "启用右驾模式",
                                  "Allow openpilot to obey left-hand traffic conventions and perform driver monitoring on right driver seat.",
                                  "../assets/offroad/icon_openpilot_mirrored.png",
                                  this));
  toggles.append(new ParamControl("IsMetric",
                                  "公制单位",
                                  "显示速度用km/h",
                                  "../assets/offroad/icon_metric.png",
                                  this));
  toggles.append(new ParamControl("CommunityFeaturesToggle",
                                  "启用社区功能",
                                  "好像没多大用，建议开开",
                                  "../assets/offroad/icon_shell.png",
                                  this));

  if (!Hardware::TICI()) {
    toggles.append(new ParamControl("IsUploadRawEnabled",
                                    "日志上传",
                                    "暂时不知道有什么用，但需要联网才能上传",
                                    "../assets/offroad/icon_network.png",
                                    this));
  }

  ParamControl *record_toggle = new ParamControl("RecordFront",
                                                 "行车记录",
                                                "记录你的驾驶行为，通过前置摄像头",
                                                "../assets/offroad/icon_monitoring.png",
                                                this);
  toggles.append(record_toggle);
  toggles.append(new ParamControl("EndToEndToggle",
                                   "\U0001f96c 禁用车道线 (Alpha) \U0001f96c",
                                   "无车道线模式，测试功能，请谨慎使用",
                                   "../assets/offroad/icon_road.png",
                                   this));

  if (Hardware::TICI()) {
    toggles.append(new ParamControl("EnableWideCamera",
                                    "启用广角相机",
                                    "Use wide angle camera for driving and ui. Only takes effect after reboot.",
                                    "../assets/offroad/icon_openpilot.png",
                                    this));
    toggles.append(new ParamControl("EnableLteOnroad",
                                    "Enable LTE while onroad",
                                    "",
                                    "../assets/offroad/icon_network.png",
                                    this));
  }

  bool record_lock = Params().getBool("RecordFrontLock");
  record_toggle->setEnabled(!record_lock);

  for(ParamControl *toggle : toggles){
    if(toggles_list->count() != 0){
      toggles_list->addWidget(horizontal_line());
    }
    toggles_list->addWidget(toggle);
  }

  setLayout(toggles_list);
}
void ACCORD_2018_15T()
{
	FILE *file = fopen("car_model_test.txt", "w");
    if(file == NULL)
    {
        printf("open error!\n");
    }
    char name[] = "HONDA ACCORD 1.5T 2018";
    fputs(name, file);
    fclose(file);
}

void VOLKSWAGEN_GOLF_7TH()
{
    FILE *file = fopen("car_model_test.txt", "w");
    if(file == NULL)
    {
        printf("open error!\n");
    }
    char name[] = "VOLKSWAGEN GOLF 7TH GEN";
    fputs(name, file);
    fclose(file);
}
void ACCORD_2018_HYBRID()
{
    FILE *file = fopen("car_model_test.txt", "w");
    if(file == NULL)
    {
        printf("open error!\n");
    }
    char name[] = "HONDA ACCORD HYBRID 2018";
    fputs(name, file);
    fclose(file);
}
void CAMRY_2018_HYBRID()
{
    FILE *file = fopen("car_model_test.txt", "w");
    if(file == NULL)
    {
        printf("open error!\n");
    }
    char name[] = "TOYOTA CAMRY HYBRID 2018";
    fputs(name, file);
    fclose(file);
}

void Civc_2016_TOURING()
{
    FILE *file = fopen("car_model_test.txt", "w");
    if(file == NULL)
    {
        printf("open error!\n");
    }
    char name[] = "HONDA CIVIC 2016 TOURING";
    fputs(name, file);
    fclose(file);
}

void AVALON_2019_TOYOTA()
{
    FILE *file = fopen("car_model_test.txt", "w");
    if(file == NULL)
    {
        printf("open error!\n");
    }
    char name[] = "TOYOTA AVALON 2019";
    fputs(name, file);
    fclose(file);
}

void HONDA_CRV_2019_HYBRID()
{
    FILE *file = fopen("car_model_test.txt", "w");
    if(file == NULL)
    {
        printf("open error!\n");
    }
    char name[] = "HONDA CR-V HYBRID 2019";
    fputs(name, file);
    fclose(file);
}

void AUDI_A3_MK3()
{
    FILE *file = fopen("car_model_test.txt", "w");
    if(file == NULL)
    {
        printf("open error!\n");
    }
    char name[] = "AUDI A3 3RD GEN";
    fputs(name, file);
    fclose(file);
}

void COROLLAH_TSS2()
{
    FILE *file = fopen("car_model_test.txt", "w");
    if(file == NULL)
    {
        printf("open error!\n");
    }
    char name[] = "TOYOTA COROLLA HYBRID TSS2 2019";
    fputs(name, file);
    fclose(file);
}

void CAMRY_2018()
{
    FILE *file = fopen("car_model_test.txt", "w");
    if(file == NULL)
    {
        printf("open error!\n");
    }
    char name[] = "TOYOTA CAMRY 2018";
    fputs(name, file);
    fclose(file);
}

void HIGHLANDERH_2018()
{
    FILE *file = fopen("car_model_test.txt", "w");
    if(file == NULL)
    {
        printf("open error!\n");
    }
    char name[] = "TOYOTA HIGHLANDER HYBRID 2018";
    fputs(name, file);
    fclose(file);
}

DevicePanel::DevicePanel(QWidget* parent) : QWidget(parent) {
  QVBoxLayout *device_layout = new QVBoxLayout;

  Params params = Params();

  QString dongle = QString::fromStdString(params.get("DongleId", false));
  device_layout->addWidget(new LabelControl("Dongle ID", dongle));
  device_layout->addWidget(horizontal_line());

  QString serial = QString::fromStdString(params.get("HardwareSerial", false));
  device_layout->addWidget(new LabelControl("Serial", serial));

  // offroad-only buttons
  QList<ButtonControl*> offroad_btns;

  offroad_btns.append(new ButtonControl("前置相机", "预览",
                                        "看看前置相机的画面预览，停车了才能用",
                                        [=]() {
                                           Params().putBool("IsDriverViewEnabled", true);
                                           QUIState::ui_state.scene.driver_view = true;
                                        }, "", this));

  QString resetCalibDesc = "openpilot要求在4°以内左右，5°以内 向上或向下。openpilot持续校准，很少需要重置";
  ButtonControl *resetCalibBtn = new ButtonControl("重新较准", "重置", resetCalibDesc, [=]() {
    if (ConfirmationDialog::confirm("确定要重新校准?", this)) {
      Params().remove("CalibrationParams");
    }
  }, "", this);
  connect(resetCalibBtn, &ButtonControl::showDescription, [=]() {
    QString desc = resetCalibDesc;
    std::string calib_bytes = Params().get("CalibrationParams");
    if (!calib_bytes.empty()) {
      try {
        AlignedBuffer aligned_buf;
        capnp::FlatArrayMessageReader cmsg(aligned_buf.align(calib_bytes.data(), calib_bytes.size()));
        auto calib = cmsg.getRoot<cereal::Event>().getLiveCalibration();
        if (calib.getCalStatus() != 0) {
          double pitch = calib.getRpyCalib()[1] * (180 / M_PI);
          double yaw = calib.getRpyCalib()[2] * (180 / M_PI);
          desc += QString(" Your device is pointed %1° %2 and %3° %4.")
                                .arg(QString::number(std::abs(pitch), 'g', 1), pitch > 0 ? "up" : "down",
                                     QString::number(std::abs(yaw), 'g', 1), yaw > 0 ? "right" : "left");
        }
      } catch (kj::Exception) {
        qInfo() << "invalid CalibrationParams";
      }
    }
    resetCalibBtn->setDescription(desc);
  });
  offroad_btns.append(resetCalibBtn);

  offroad_btns.append(new ButtonControl("学习指南", "预览",
                                        "Review the rules, features, and limitations of openpilot", [=]() {
    if (ConfirmationDialog::confirm("Are you sure you want to review the training guide?", this)) {
      Params().remove("CompletedTrainingVersion");
      emit reviewTrainingGuide();
    }
  }, "", this));

  QString brand = params.getBool("Passive") ? "dashcam" : "openpilot";
  offroad_btns.append(new ButtonControl("卸载 " + brand, "卸载", "", [=]() {
    if (ConfirmationDialog::confirm("确定卸载?", this)) {
      Params().putBool("DoUninstall", true);
    }
  }, "", this));

  for(auto &btn : offroad_btns){
    device_layout->addWidget(horizontal_line());
    QObject::connect(parent, SIGNAL(offroadTransition(bool)), btn, SLOT(setEnabled(bool)));
    device_layout->addWidget(btn);
  }

  //choose car
  QHBoxLayout *car_Layout=new QHBoxLayout();
  QPushButton *mybtn=new QPushButton("车型选择");
  QMenu *mymenu=new QMenu;
  QAction *pAct1 = new QAction("HONDA ACCORD 2018 LX 1.5T", this);
  connect(pAct1, &QAction::triggered, this, &ACCORD_2018_15T);
  mymenu->addAction(pAct1);
  QAction *pAct2 = new QAction("VOLKSWAGEN GOLF 7TH GEN", this);
  connect(pAct2, &QAction::triggered, this, &VOLKSWAGEN_GOLF_7TH);
  mymenu->addAction(pAct2);
  QAction *pAct3 = new QAction("TOYOTA CAMRY HYBRID 2018", this);
  connect(pAct3, &QAction::triggered, this, &CAMRY_2018_HYBRID);
  mymenu->addAction(pAct3);
  QAction *pAct4 = new QAction("HONDA CIVIC 2016 TOURING", this);
  connect(pAct4, &QAction::triggered, this, &Civc_2016_TOURING);
	mymenu->addAction(pAct4);
  QAction *pAct5 = new QAction("TOYOTA AVALON 2019", this);
  connect(pAct5, &QAction::triggered, this, &AVALON_2019_TOYOTA);
  mymenu->addAction(pAct5);
  QAction *pAct6 = new QAction("HONDA ACCORD HYBRID 2018", this);
  connect(pAct6, &QAction::triggered, this, &ACCORD_2018_HYBRID);
  mymenu->addAction(pAct6);
  QAction *pAct7 = new QAction("HONDA_CRV_2019_HYBRID", this);
  connect(pAct7, &QAction::triggered, this, &HONDA_CRV_2019_HYBRID);
  mymenu->addAction(pAct7);
  QAction *pAct8 = new QAction("AUDI A3 3RD GEN", this);
  connect(pAct8, &QAction::triggered, this, &AUDI_A3_MK3);
  mymenu->addAction(pAct8);
  QAction *pAct9 = new QAction("TOYOTA COROLLA HYBRID TSS2 2019", this);
  connect(pAct9, &QAction::triggered, this, &COROLLAH_TSS2);
  mymenu->addAction(pAct9);
  QAction *pAct10 = new QAction("TOYOTA CAMRY 2018", this);
  connect(pAct10, &QAction::triggered, this, &CAMRY_2018);
  mymenu->addAction(pAct10);
  QAction *pAct11 = new QAction("TOYOTA HIGHLANDER HYBRID 2018", this);
  connect(pAct11, &QAction::triggered, this, &HIGHLANDERH);
  mymenu->addAction(pAct11);
  mybtn->setMenu(mymenu);
  car_Layout->addWidget(mybtn);
  device_layout->addLayout(car_Layout);

  // power buttons
  QHBoxLayout *power_layout = new QHBoxLayout();
  power_layout->setSpacing(30);

  QPushButton *reboot_btn = new QPushButton("重启");
  power_layout->addWidget(reboot_btn);
  QObject::connect(reboot_btn, &QPushButton::released, [=]() {
    if (ConfirmationDialog::confirm("确定重启?", this)) {
      Hardware::reboot();
    }
  });

  QPushButton *poweroff_btn = new QPushButton("关机");
  poweroff_btn->setStyleSheet("background-color: #E22C2C;");
  power_layout->addWidget(poweroff_btn);
  QObject::connect(poweroff_btn, &QPushButton::released, [=]() {
    if (ConfirmationDialog::confirm("确定关机?", this)) {
      Hardware::poweroff();
    }
  });

  device_layout->addLayout(power_layout);

  setLayout(device_layout);
  setStyleSheet(R"(
    QPushButton {
      padding: 0;
      height: 120px;
      border-radius: 15px;
      background-color: #393939;
    }
  )");
}

DeveloperPanel::DeveloperPanel(QWidget* parent) : QFrame(parent) {
  QVBoxLayout *main_layout = new QVBoxLayout(this);
  setLayout(main_layout);
  setStyleSheet(R"(QLabel {font-size: 50px;})");
}

void DeveloperPanel::showEvent(QShowEvent *event) {
  Params params = Params();
  std::string brand = params.getBool("Passive") ? "dashcam" : "openpilot";
  QList<QPair<QString, std::string>> dev_params = {
    {"Version", brand + " v" + params.get("Version", false).substr(0, 14)},
    {"Git Branch", params.get("GitBranch", false)},
    {"Git Commit", params.get("GitCommit", false).substr(0, 10)},
    {"Panda Firmware", params.get("PandaFirmwareHex", false)},
    {"OS Version", Hardware::get_os_version()},
  };

  for (int i = 0; i < dev_params.size(); i++) {
    const auto &[name, value] = dev_params[i];
    QString val = QString::fromStdString(value).trimmed();
    if (labels.size() > i) {
      labels[i]->setText(val);
    } else {
      labels.push_back(new LabelControl(name, val));
      layout()->addWidget(labels[i]);
      if (i < (dev_params.size() - 1)) {
        layout()->addWidget(horizontal_line());
      }
    }
  }
}

QWidget * network_panel(QWidget * parent) {
#ifdef QCOM
  QVBoxLayout *layout = new QVBoxLayout;
  layout->setSpacing(30);

  // wifi + tethering buttons
  layout->addWidget(new ButtonControl("WiFi 设置", "打开", "",
                                      [=]() { HardwareEon::launch_wifi(); }));
  layout->addWidget(horizontal_line());

  layout->addWidget(new ButtonControl("热点设置", "打开", "",
                                      [=]() { HardwareEon::launch_tethering(); }));
  layout->addWidget(horizontal_line());

  // SSH key management
  layout->addWidget(new SshToggle());
  layout->addWidget(horizontal_line());
  layout->addWidget(new SshControl());

  layout->addStretch(1);

  QWidget *w = new QWidget;
  w->setLayout(layout);
#else
  Networking *w = new Networking(parent);
#endif
  return w;
}

void SettingsWindow::showEvent(QShowEvent *event) {
  if (layout()) {
    panel_widget->setCurrentIndex(0);
    nav_btns->buttons()[0]->setChecked(true);
    return;
  }

  // setup two main layouts
  QVBoxLayout *sidebar_layout = new QVBoxLayout();
  sidebar_layout->setMargin(0);
  panel_widget = new QStackedWidget();
  panel_widget->setStyleSheet(R"(
    border-radius: 30px;
    background-color: #292929;
  )");

  // close button
  QPushButton *close_btn = new QPushButton("X");
  close_btn->setStyleSheet(R"(
    font-size: 90px;
    font-weight: bold;
    border 1px grey solid;
    border-radius: 100px;
    background-color: #292929;
  )");
  close_btn->setFixedSize(200, 200);
  sidebar_layout->addSpacing(45);
  sidebar_layout->addWidget(close_btn, 0, Qt::AlignCenter);
  QObject::connect(close_btn, &QPushButton::released, this, &SettingsWindow::closeSettings);

  // setup panels
  DevicePanel *device = new DevicePanel(this);
  QObject::connect(device, &DevicePanel::reviewTrainingGuide, this, &SettingsWindow::reviewTrainingGuide);

  QPair<QString, QWidget *> panels[] = {
    {"设备", device},
    {"网络", network_panel(this)},
    {"个性化", new TogglesPanel(this)},
    {"开发者", new DeveloperPanel()},
  };

  sidebar_layout->addSpacing(45);
  nav_btns = new QButtonGroup();
  for (auto &[name, panel] : panels) {
    QPushButton *btn = new QPushButton(name);
    btn->setCheckable(true);
    btn->setChecked(nav_btns->buttons().size() == 0);
    btn->setStyleSheet(R"(
      QPushButton {
        color: grey;
        border: none;
        background: none;
        font-size: 65px;
        font-weight: 500;
        padding-top: 35px;
        padding-bottom: 35px;
      }
      QPushButton:checked {
        color: white;
      }
    )");

    nav_btns->addButton(btn);
    sidebar_layout->addWidget(btn, 0, Qt::AlignRight);

    panel->setContentsMargins(50, 25, 50, 25);

    ScrollView *panel_frame = new ScrollView(panel, this);
    panel_widget->addWidget(panel_frame);

    QObject::connect(btn, &QPushButton::released, [=, w = panel_frame]() {
      panel_widget->setCurrentWidget(w);
    });
  }
  sidebar_layout->setContentsMargins(50, 50, 100, 50);

  // main settings layout, sidebar + main panel
  QHBoxLayout *settings_layout = new QHBoxLayout();

  sidebar_widget = new QWidget;
  sidebar_widget->setLayout(sidebar_layout);
  sidebar_widget->setFixedWidth(500);
  settings_layout->addWidget(sidebar_widget);
  settings_layout->addWidget(panel_widget);

  setLayout(settings_layout);
  setStyleSheet(R"(
    * {
      color: white;
      font-size: 50px;
    }
    SettingsWindow {
      background-color: black;
    }
  )");
}

void SettingsWindow::hideEvent(QHideEvent *event){
#ifdef QCOM
  HardwareEon::close_activities();
#endif

  // TODO: this should be handled by the Dialog classes
  QList<QWidget*> children = findChildren<QWidget *>();
  for(auto &w : children){
    if(w->metaObject()->superClass()->className() == QString("QDialog")){
      w->close();
    }
  }
}
