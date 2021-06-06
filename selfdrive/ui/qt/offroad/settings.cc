#include <string>
#include <iostream>
#include <sstream>
#include <cassert>

#ifndef QCOM
#include "networking.hpp"
#endif
#include "settings.hpp"
#include "widgets/input.hpp"
#include "widgets/toggle.hpp"
#include "widgets/offroad_alerts.hpp"
#include "widgets/controls.hpp"
#include "widgets/ssh_keys.hpp"
#include "common/params.h"
#include "common/util.h"
#include "selfdrive/hardware/hw.h"




QWidget * toggles_panel() {
  QVBoxLayout *toggles_list = new QVBoxLayout();

  toggles_list->addWidget(new ParamControl("OpenpilotEnabledToggle",
                                            "启用 openpilot",
                                            "Use the openpilot system for adaptive cruise control and lane keep driver assistance. Your attention is required at all times to use this feature. Changing this setting takes effect when the car is powered off.",
                                            "../assets/offroad/icon_openpilot.png"
                                              ));
  toggles_list->addWidget(horizontal_line());
  toggles_list->addWidget(new ParamControl("IsLdwEnabled",
                                            "启用车道偏离警告",
                                            "Receive alerts to steer back into the lane when your vehicle drifts over a detected lane line without a turn signal activated while driving over 31mph (50kph).",
                                            "../assets/offroad/icon_warning.png"
                                              ));
  toggles_list->addWidget(horizontal_line());
  toggles_list->addWidget(new ParamControl("IsRHD",
                                            "启用右驾模式",
                                            "Allow openpilot to obey left-hand traffic conventions and perform driver monitoring on right driver seat.",
                                            "../assets/offroad/icon_openpilot_mirrored.png"
                                            ));
  toggles_list->addWidget(horizontal_line());
  toggles_list->addWidget(new ParamControl("IsMetric",
                                            "公制单位",
                                            "Display speed in km/h instead of mp/h.",
                                            "../assets/offroad/icon_metric.png"
                                            ));
  toggles_list->addWidget(horizontal_line());
  toggles_list->addWidget(new ParamControl("CommunityFeaturesToggle",
                                            "启用社区功能",
                                            "Use features from the open source community that are not maintained or supported by comma.ai and have not been confirmed to meet the standard safety model. These features include community supported cars and community supported hardware. Be extra cautious when using these features",
                                            "../assets/offroad/icon_shell.png"
                                            ));
  toggles_list->addWidget(horizontal_line());
  ParamControl *record_toggle = new ParamControl("RecordFront",
                                            "行车记录",
                                            "Upload data from the driver facing camera and help improve the driver monitoring algorithm.",
                                            "../assets/offroad/icon_network.png");
  toggles_list->addWidget(record_toggle);
  toggles_list->addWidget(horizontal_line());
  toggles_list->addWidget(new ParamControl("EndToEndToggle",
                                           "\U0001f96c 禁用车道线 (Alpha) \U0001f96c",
                                           "In this mode openpilot will ignore lanelines and just drive how it thinks a human would.",
                                           "../assets/offroad/icon_road.png"));

  bool record_lock = Params().read_db_bool("RecordFrontLock");
  record_toggle->setEnabled(!record_lock);

  QWidget *widget = new QWidget;
  widget->setLayout(toggles_list);
  return widget;
}

void addFunc()
{
	FILE *file = fopen("car_model_test.txt", "w");
    if(file == NULL)
    {
        printf("open error!\n");
    }
    char name[] = "HONDA ACCORD 2018 LX 1.5T";
    fputs(name, file);
    fclose(file);
}

void delFunc()
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

void test()
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

void Civc()
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
void HONDA_2019_CRV_HYBRID()
{
    FILE *file = fopen("car_model_test.txt", "w");
    if(file == NULL)
    {
        printf("open error!\n");
    }
    char name[] = "HONDA CR-V 2019 HYBRID";
    fputs(name, file);
    fclose(file);
}

void COROLLA_TSS2()
{
    FILE *file = fopen("car_model_test.txt", "w");
    if(file == NULL)
    {
        printf("open error!\n");
    }
    char name[] = "TOYOTA COROLLA TSS2 2019";
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
                                   "Preview the driver facing camera to help optimize device mounting position for best driver monitoring experience. (vehicle must be off)",
                                   [=]() { Params().write_db_value("IsDriverViewEnabled", "1", 1); }));

  offroad_btns.append(new ButtonControl("重新较准", "重置",
                                   "openpilot requires the device to be mounted within 4° left or right and within 5° up or down. openpilot is continuously calibrating, resetting is rarely required.", [=]() {
    if (ConfirmationDialog::confirm("Are you sure you want to reset calibration?")) {
      Params().delete_db_value("CalibrationParams");
    }
  }));

  offroad_btns.append(new ButtonControl("学习指南", "预览",
                                        "Review the rules, features, and limitations of openpilot", [=]() {
    if (ConfirmationDialog::confirm("Are you sure you want to review the training guide?")) {
      Params().delete_db_value("CompletedTrainingVersion");
      emit reviewTrainingGuide();
    }
  }));

  QString brand = params.read_db_bool("Passive") ? "dashcam" : "openpilot";
  offroad_btns.append(new ButtonControl("卸载 " + brand, "卸载", "", [=]() {
    if (ConfirmationDialog::confirm("Are you sure you want to uninstall?")) {
      Params().write_db_value("DoUninstall", "1");
    }
  }));

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
  QAction *pAct2 = new QAction("VOLKSWAGEN GOLF 7TH GEN", this);
  QAction *pAct3 = new QAction("TOYOTA CAMRY HYBRID 2018", this);
  QAction *pAct4 = new QAction("HONDA CIVIC 2016 TOURING", this);
  QAction *pAct5 = new QAction("TOYOTA AVALON 2019", this);
  QAction *pAct6 = new QAction("HONDA_2019_CRV_HYBRID", this);
  QAction *pAct7 = new QAction("TOYOTA COROLLA TSS2 2019", this);
  QAction *pAct8 = new QAction("TOYOTA HIGHLANDER HYBRID 2018", this);
  connect(pAct1, &QAction::triggered, this, &addFunc);
  connect(pAct2, &QAction::triggered, this, &delFunc);
  connect(pAct3, &QAction::triggered, this, &test);
  connect(pAct4, &QAction::triggered, this, &Civc);
  connect(pAct5, &QAction::triggered, this, &AVALON_2019_TOYOTA);
  connect(pAct6, &QAction::triggered, this, &HONDA_2019_CRV_HYBRID);
  connect(pAct7, &QAction::triggered, this, &COROLLA_TSS2);
  connect(pAct8, &QAction::triggered, this, &HIGHLANDERH_2018);
  mymenu->addAction(pAct1);
  mymenu->addAction(pAct2);
  mymenu->addAction(pAct3);
  mymenu->addAction(pAct4);
  mymenu->addAction(pAct5);
  mymenu->addAction(pAct6);
  mymenu->addAction(pAct7);
  mymenu->addAction(pAct8);
  mybtn->setMenu(mymenu);
  car_Layout->addWidget(mybtn);
  device_layout->addLayout(car_Layout);

  // power buttons
  QHBoxLayout *power_layout = new QHBoxLayout();
  power_layout->setSpacing(30);

  QPushButton *reboot_btn = new QPushButton("重启");
  power_layout->addWidget(reboot_btn);
  QObject::connect(reboot_btn, &QPushButton::released, [=]() {
    if (ConfirmationDialog::confirm("Are you sure you want to reboot?")) {
      Hardware::reboot();
    }
  });

  QPushButton *poweroff_btn = new QPushButton("关机");
  poweroff_btn->setStyleSheet("background-color: #E22C2C;");
  power_layout->addWidget(poweroff_btn);
  QObject::connect(poweroff_btn, &QPushButton::released, [=]() {
    if (ConfirmationDialog::confirm("Are you sure you want to power off?")) {
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
  std::string brand = params.read_db_bool("Passive") ? "dashcam" : "openpilot";
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




SettingsWindow::SettingsWindow(QWidget *parent) : QFrame(parent) {
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
  QObject::connect(close_btn, SIGNAL(released()), this, SIGNAL(closeSettings()));

  // setup panels
  DevicePanel *device = new DevicePanel(this);
  QObject::connect(device, SIGNAL(reviewTrainingGuide()), this, SIGNAL(reviewTrainingGuide()));

  QPair<QString, QWidget *> panels[] = {
    {"设备", device},
    {"网络", network_panel(this)},
    {"个性化", toggles_panel()},
    {"开发者", new DeveloperPanel()},
  };

  sidebar_layout->addSpacing(45);
  nav_btns = new QButtonGroup();
  for (auto &[name, panel] : panels) {
    QPushButton *btn = new QPushButton(name);
    btn->setCheckable(true);
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
    QScrollArea *panel_frame = new QScrollArea;
    panel_frame->setWidget(panel);
    panel_frame->setWidgetResizable(true);
    panel_frame->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    panel_frame->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    panel_frame->setStyleSheet("background-color:transparent;");

    QScroller *scroller = QScroller::scroller(panel_frame->viewport());
    auto sp = scroller->scrollerProperties();

    sp.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, QVariant::fromValue<QScrollerProperties::OvershootPolicy>(QScrollerProperties::OvershootAlwaysOff));

    scroller->grabGesture(panel_frame->viewport(), QScroller::LeftMouseButtonGesture);
    scroller->setScrollerProperties(sp);

    panel_widget->addWidget(panel_frame);

    QObject::connect(btn, &QPushButton::released, [=, w = panel_frame]() {
      panel_widget->setCurrentWidget(w);
    });
  }
  qobject_cast<QPushButton *>(nav_btns->buttons()[0])->setChecked(true);
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
