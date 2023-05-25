/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actStartCamera;
    QAction *actStopCamera;
    QAction *actVideoRecord;
    QAction *actCapture;
    QAction *actQuit;
    QAction *actVideoStop;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_3;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox_Camera;
    QGridLayout *gridLayout_3;
    QLabel *label;
    QComboBox *comboCam_List;
    QLabel *label_3;
    QComboBox *comboCam_PhotoRes;
    QLabel *label_2;
    QComboBox *comboCam_Position;
    QLabel *label_4;
    QComboBox *comboCam_VideoRes;
    QLabel *label_5;
    QComboBox *comboCam_FrameRate;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QCheckBox *chkBoxCam_Color;
    QCheckBox *chkBoxCam_Manual;
    QCheckBox *chkBoxCam_Exposure;
    QCheckBox *chkBoxCam_Custom;
    QCheckBox *chkBoxCam_Iso;
    QCheckBox *chkBoxCam_Focus;
    QSplitter *splitter;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QVideoWidget *videoPreview;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_2;
    QLabel *label_6;
    QComboBox *comboImage_Quality;
    QCheckBox *chkBox_Sound;
    QLabel *label_11;
    QComboBox *comboImage_Resolution;
    QCheckBox *chkBox_SaveToFile;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_5;
    QLabel *labImage;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btnVideoFile;
    QLineEdit *editVideo_OutputFile;
    QGroupBox *groupBox_5;
    QFormLayout *formLayout;
    QLabel *label_8;
    QComboBox *comboVideo_Codec;
    QLabel *label_9;
    QComboBox *comboVideo_FileFormat;
    QLabel *label_10;
    QComboBox *comboViedo_Quality;
    QLabel *label_7;
    QComboBox *comboVideo_Resolution;
    QSpacerItem *verticalSpacer;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(913, 538);
        QFont font;
        font.setPointSize(10);
        MainWindow->setFont(font);
        actStartCamera = new QAction(MainWindow);
        actStartCamera->setObjectName(QString::fromUtf8("actStartCamera"));
        actStartCamera->setEnabled(false);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/images/video_chat.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actStartCamera->setIcon(icon);
        actStopCamera = new QAction(MainWindow);
        actStopCamera->setObjectName(QString::fromUtf8("actStopCamera"));
        actStopCamera->setEnabled(false);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/images/remove_user.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actStopCamera->setIcon(icon1);
        actVideoRecord = new QAction(MainWindow);
        actVideoRecord->setObjectName(QString::fromUtf8("actVideoRecord"));
        actVideoRecord->setEnabled(false);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/images/5.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actVideoRecord->setIcon(icon2);
        actCapture = new QAction(MainWindow);
        actCapture->setObjectName(QString::fromUtf8("actCapture"));
        actCapture->setEnabled(false);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/images/file_pictures.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actCapture->setIcon(icon3);
        actQuit = new QAction(MainWindow);
        actQuit->setObjectName(QString::fromUtf8("actQuit"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/images/exit_24.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actQuit->setIcon(icon4);
        actVideoStop = new QAction(MainWindow);
        actVideoStop->setObjectName(QString::fromUtf8("actVideoStop"));
        actVideoStop->setEnabled(false);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/images/Shut Down.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actVideoStop->setIcon(icon5);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_3 = new QVBoxLayout(centralWidget);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(2, 2, 2, 2);
        frame = new QFrame(centralWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::Panel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(5, 5, 5, 5);
        groupBox_Camera = new QGroupBox(frame);
        groupBox_Camera->setObjectName(QString::fromUtf8("groupBox_Camera"));
        gridLayout_3 = new QGridLayout(groupBox_Camera);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label = new QLabel(groupBox_Camera);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMaximumSize(QSize(80, 16777215));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label, 0, 0, 1, 1);

        comboCam_List = new QComboBox(groupBox_Camera);
        comboCam_List->setObjectName(QString::fromUtf8("comboCam_List"));

        gridLayout_3->addWidget(comboCam_List, 0, 1, 1, 1);

        label_3 = new QLabel(groupBox_Camera);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_3, 0, 2, 1, 1);

        comboCam_PhotoRes = new QComboBox(groupBox_Camera);
        comboCam_PhotoRes->setObjectName(QString::fromUtf8("comboCam_PhotoRes"));

        gridLayout_3->addWidget(comboCam_PhotoRes, 0, 3, 1, 1);

        label_2 = new QLabel(groupBox_Camera);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_2, 1, 0, 1, 1);

        comboCam_Position = new QComboBox(groupBox_Camera);
        comboCam_Position->addItem(QString());
        comboCam_Position->addItem(QString());
        comboCam_Position->addItem(QString());
        comboCam_Position->setObjectName(QString::fromUtf8("comboCam_Position"));

        gridLayout_3->addWidget(comboCam_Position, 1, 1, 1, 1);

        label_4 = new QLabel(groupBox_Camera);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_4, 1, 2, 1, 1);

        comboCam_VideoRes = new QComboBox(groupBox_Camera);
        comboCam_VideoRes->setObjectName(QString::fromUtf8("comboCam_VideoRes"));

        gridLayout_3->addWidget(comboCam_VideoRes, 1, 3, 1, 1);

        label_5 = new QLabel(groupBox_Camera);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_5, 2, 2, 1, 1);

        comboCam_FrameRate = new QComboBox(groupBox_Camera);
        comboCam_FrameRate->setObjectName(QString::fromUtf8("comboCam_FrameRate"));

        gridLayout_3->addWidget(comboCam_FrameRate, 2, 3, 1, 1);


        horizontalLayout->addWidget(groupBox_Camera);

        groupBox_2 = new QGroupBox(frame);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        chkBoxCam_Color = new QCheckBox(groupBox_2);
        chkBoxCam_Color->setObjectName(QString::fromUtf8("chkBoxCam_Color"));

        gridLayout->addWidget(chkBoxCam_Color, 0, 0, 1, 1);

        chkBoxCam_Manual = new QCheckBox(groupBox_2);
        chkBoxCam_Manual->setObjectName(QString::fromUtf8("chkBoxCam_Manual"));

        gridLayout->addWidget(chkBoxCam_Manual, 0, 1, 1, 1);

        chkBoxCam_Exposure = new QCheckBox(groupBox_2);
        chkBoxCam_Exposure->setObjectName(QString::fromUtf8("chkBoxCam_Exposure"));

        gridLayout->addWidget(chkBoxCam_Exposure, 1, 0, 1, 1);

        chkBoxCam_Custom = new QCheckBox(groupBox_2);
        chkBoxCam_Custom->setObjectName(QString::fromUtf8("chkBoxCam_Custom"));

        gridLayout->addWidget(chkBoxCam_Custom, 1, 1, 1, 1);

        chkBoxCam_Iso = new QCheckBox(groupBox_2);
        chkBoxCam_Iso->setObjectName(QString::fromUtf8("chkBoxCam_Iso"));

        gridLayout->addWidget(chkBoxCam_Iso, 2, 0, 1, 1);

        chkBoxCam_Focus = new QCheckBox(groupBox_2);
        chkBoxCam_Focus->setObjectName(QString::fromUtf8("chkBoxCam_Focus"));

        gridLayout->addWidget(chkBoxCam_Focus, 2, 1, 1, 1);


        horizontalLayout->addWidget(groupBox_2);

        horizontalLayout->setStretch(0, 1);

        verticalLayout_3->addWidget(frame);

        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        groupBox = new QGroupBox(splitter);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        videoPreview = new QVideoWidget(groupBox);
        videoPreview->setObjectName(QString::fromUtf8("videoPreview"));
        videoPreview->setMinimumSize(QSize(150, 0));

        verticalLayout->addWidget(videoPreview);

        splitter->addWidget(groupBox);
        tabWidget = new QTabWidget(splitter);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setDocumentMode(true);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout_2 = new QVBoxLayout(tab);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(-1, -1, -1, 0);
        groupBox_3 = new QGroupBox(tab);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_2 = new QGridLayout(groupBox_3);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(-1, 5, -1, 5);
        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 0, 0, 1, 1);

        comboImage_Quality = new QComboBox(groupBox_3);
        comboImage_Quality->addItem(QString());
        comboImage_Quality->addItem(QString());
        comboImage_Quality->addItem(QString());
        comboImage_Quality->addItem(QString());
        comboImage_Quality->addItem(QString());
        comboImage_Quality->setObjectName(QString::fromUtf8("comboImage_Quality"));

        gridLayout_2->addWidget(comboImage_Quality, 0, 1, 1, 1);

        chkBox_Sound = new QCheckBox(groupBox_3);
        chkBox_Sound->setObjectName(QString::fromUtf8("chkBox_Sound"));
        chkBox_Sound->setChecked(true);

        gridLayout_2->addWidget(chkBox_Sound, 0, 2, 1, 1);

        label_11 = new QLabel(groupBox_3);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_2->addWidget(label_11, 1, 0, 1, 1);

        comboImage_Resolution = new QComboBox(groupBox_3);
        comboImage_Resolution->setObjectName(QString::fromUtf8("comboImage_Resolution"));

        gridLayout_2->addWidget(comboImage_Resolution, 1, 1, 1, 1);

        chkBox_SaveToFile = new QCheckBox(groupBox_3);
        chkBox_SaveToFile->setObjectName(QString::fromUtf8("chkBox_SaveToFile"));
        chkBox_SaveToFile->setChecked(true);

        gridLayout_2->addWidget(chkBox_SaveToFile, 1, 2, 1, 1);

        gridLayout_2->setColumnStretch(1, 1);

        verticalLayout_2->addWidget(groupBox_3);

        scrollArea = new QScrollArea(tab);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 621, 146));
        verticalLayout_5 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        labImage = new QLabel(scrollAreaWidgetContents);
        labImage->setObjectName(QString::fromUtf8("labImage"));
        labImage->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(labImage);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_2->addWidget(scrollArea);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_4 = new QVBoxLayout(tab_2);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        btnVideoFile = new QPushButton(tab_2);
        btnVideoFile->setObjectName(QString::fromUtf8("btnVideoFile"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/images/104.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        btnVideoFile->setIcon(icon6);
        btnVideoFile->setFlat(false);

        horizontalLayout_3->addWidget(btnVideoFile);

        editVideo_OutputFile = new QLineEdit(tab_2);
        editVideo_OutputFile->setObjectName(QString::fromUtf8("editVideo_OutputFile"));

        horizontalLayout_3->addWidget(editVideo_OutputFile);


        verticalLayout_4->addLayout(horizontalLayout_3);

        groupBox_5 = new QGroupBox(tab_2);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        formLayout = new QFormLayout(groupBox_5);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setVerticalSpacing(9);
        label_8 = new QLabel(groupBox_5);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_8);

        comboVideo_Codec = new QComboBox(groupBox_5);
        comboVideo_Codec->setObjectName(QString::fromUtf8("comboVideo_Codec"));

        formLayout->setWidget(0, QFormLayout::FieldRole, comboVideo_Codec);

        label_9 = new QLabel(groupBox_5);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_9);

        comboVideo_FileFormat = new QComboBox(groupBox_5);
        comboVideo_FileFormat->setObjectName(QString::fromUtf8("comboVideo_FileFormat"));

        formLayout->setWidget(1, QFormLayout::FieldRole, comboVideo_FileFormat);

        label_10 = new QLabel(groupBox_5);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_10);

        comboViedo_Quality = new QComboBox(groupBox_5);
        comboViedo_Quality->addItem(QString());
        comboViedo_Quality->addItem(QString());
        comboViedo_Quality->addItem(QString());
        comboViedo_Quality->addItem(QString());
        comboViedo_Quality->addItem(QString());
        comboViedo_Quality->setObjectName(QString::fromUtf8("comboViedo_Quality"));

        formLayout->setWidget(2, QFormLayout::FieldRole, comboViedo_Quality);

        label_7 = new QLabel(groupBox_5);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_7);

        comboVideo_Resolution = new QComboBox(groupBox_5);
        comboVideo_Resolution->setObjectName(QString::fromUtf8("comboVideo_Resolution"));

        formLayout->setWidget(3, QFormLayout::FieldRole, comboVideo_Resolution);


        verticalLayout_4->addWidget(groupBox_5);

        verticalSpacer = new QSpacerItem(20, 101, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer);

        tabWidget->addTab(tab_2, QString());
        splitter->addWidget(tabWidget);

        verticalLayout_3->addWidget(splitter);

        verticalLayout_3->setStretch(1, 1);
        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        mainToolBar->addAction(actStartCamera);
        mainToolBar->addAction(actStopCamera);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actCapture);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actVideoRecord);
        mainToolBar->addAction(actVideoStop);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actQuit);

        retranslateUi(MainWindow);
        QObject::connect(actQuit, &QAction::triggered, MainWindow, qOverload<>(&QMainWindow::close));

        tabWidget->setCurrentIndex(1);
        comboImage_Quality->setCurrentIndex(3);
        comboViedo_Quality->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\346\221\204\345\203\217\345\244\264\346\213\215\347\205\247\344\270\216\345\275\225\345\203\217", nullptr));
        actStartCamera->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\220\257\346\221\204\345\203\217\345\244\264", nullptr));
#if QT_CONFIG(tooltip)
        actStartCamera->setToolTip(QCoreApplication::translate("MainWindow", "\345\274\200\345\220\257\346\221\204\345\203\217\345\244\264", nullptr));
#endif // QT_CONFIG(tooltip)
        actStopCamera->setText(QCoreApplication::translate("MainWindow", "\345\205\263\351\227\255\346\221\204\345\203\217\345\244\264", nullptr));
#if QT_CONFIG(tooltip)
        actStopCamera->setToolTip(QCoreApplication::translate("MainWindow", "\345\205\263\351\227\255\346\221\204\345\203\217\345\244\264", nullptr));
#endif // QT_CONFIG(tooltip)
        actVideoRecord->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213\345\275\225\345\203\217", nullptr));
#if QT_CONFIG(tooltip)
        actVideoRecord->setToolTip(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213\345\275\225\345\203\217", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actVideoRecord->setShortcut(QCoreApplication::translate("MainWindow", "F8", nullptr));
#endif // QT_CONFIG(shortcut)
        actCapture->setText(QCoreApplication::translate("MainWindow", "\346\213\215\347\205\247", nullptr));
#if QT_CONFIG(tooltip)
        actCapture->setToolTip(QCoreApplication::translate("MainWindow", "\346\213\215\347\205\247", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actCapture->setShortcut(QCoreApplication::translate("MainWindow", "F10", nullptr));
#endif // QT_CONFIG(shortcut)
        actQuit->setText(QCoreApplication::translate("MainWindow", "\351\200\200\345\207\272", nullptr));
#if QT_CONFIG(tooltip)
        actQuit->setToolTip(QCoreApplication::translate("MainWindow", "\351\200\200\345\207\272", nullptr));
#endif // QT_CONFIG(tooltip)
        actVideoStop->setText(QCoreApplication::translate("MainWindow", "\345\201\234\346\255\242\345\275\225\345\203\217", nullptr));
#if QT_CONFIG(tooltip)
        actVideoStop->setToolTip(QCoreApplication::translate("MainWindow", "\345\201\234\346\255\242\345\275\225\345\203\217", nullptr));
#endif // QT_CONFIG(tooltip)
        groupBox_Camera->setTitle(QCoreApplication::translate("MainWindow", "\346\221\204\345\203\217\345\244\264\350\256\276\345\244\207", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\346\221\204\345\203\217\345\244\264", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\346\224\257\346\214\201\347\232\204\346\213\215\347\205\247\345\210\206\350\276\250\347\216\207", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\346\221\204\345\203\217\345\244\264\344\275\215\347\275\256", nullptr));
        comboCam_Position->setItemText(0, QCoreApplication::translate("MainWindow", "\346\234\252\350\256\276\347\275\256", nullptr));
        comboCam_Position->setItemText(1, QCoreApplication::translate("MainWindow", "\345\220\216\347\275\256", nullptr));
        comboCam_Position->setItemText(2, QCoreApplication::translate("MainWindow", "\345\211\215\347\275\256", nullptr));

        label_4->setText(QCoreApplication::translate("MainWindow", "\346\224\257\346\214\201\347\232\204\350\247\206\351\242\221\345\210\206\350\276\250\347\216\207", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\350\247\206\351\242\221\345\270\247\347\216\207\350\214\203\345\233\264", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "\346\221\204\345\203\217\345\244\264\346\216\247\345\210\266\347\211\271\346\200\247", nullptr));
        chkBoxCam_Color->setText(QCoreApplication::translate("MainWindow", "ColorTemperature", nullptr));
        chkBoxCam_Manual->setText(QCoreApplication::translate("MainWindow", "ManualExposureTime", nullptr));
        chkBoxCam_Exposure->setText(QCoreApplication::translate("MainWindow", "ExposureCompensation", nullptr));
        chkBoxCam_Custom->setText(QCoreApplication::translate("MainWindow", "CustomFocusPoint", nullptr));
        chkBoxCam_Iso->setText(QCoreApplication::translate("MainWindow", "IsoSensitivity", nullptr));
        chkBoxCam_Focus->setText(QCoreApplication::translate("MainWindow", "FocusDistance", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "\346\221\204\345\203\217\345\244\264\351\242\204\350\247\210", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "\346\213\215\347\205\247\350\256\276\347\275\256", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "\345\233\276\347\211\207\347\274\226\347\240\201\345\223\201\350\264\250", nullptr));
        comboImage_Quality->setItemText(0, QCoreApplication::translate("MainWindow", "VeryLowQuality", nullptr));
        comboImage_Quality->setItemText(1, QCoreApplication::translate("MainWindow", "LowQuality", nullptr));
        comboImage_Quality->setItemText(2, QCoreApplication::translate("MainWindow", "NormalQuality", nullptr));
        comboImage_Quality->setItemText(3, QCoreApplication::translate("MainWindow", "HighQuality", nullptr));
        comboImage_Quality->setItemText(4, QCoreApplication::translate("MainWindow", "VeryHighQuality", nullptr));

        chkBox_Sound->setText(QCoreApplication::translate("MainWindow", "\346\222\255\346\224\276\351\237\263\346\225\210", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "\345\233\276\347\211\207\345\210\206\350\276\250\347\216\207", nullptr));
        chkBox_SaveToFile->setText(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230\344\270\272\346\226\207\344\273\266", nullptr));
        labImage->setText(QCoreApplication::translate("MainWindow", "\346\213\215\347\205\247\347\232\204\345\233\276\347\211\207", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "\346\213\215\346\221\204\347\232\204\347\205\247\347\211\207", nullptr));
        btnVideoFile->setText(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230\346\226\207\344\273\266", nullptr));
        editVideo_OutputFile->setText(QString());
        groupBox_5->setTitle(QCoreApplication::translate("MainWindow", "\345\275\225\345\203\217\350\256\276\347\275\256", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "\350\247\206\351\242\221\347\274\226\347\240\201", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266\346\240\274\345\274\217", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "\350\247\206\351\242\221\347\274\226\347\240\201\345\223\201\350\264\250", nullptr));
        comboViedo_Quality->setItemText(0, QCoreApplication::translate("MainWindow", "VeryLowQuality", nullptr));
        comboViedo_Quality->setItemText(1, QCoreApplication::translate("MainWindow", "LowQuality", nullptr));
        comboViedo_Quality->setItemText(2, QCoreApplication::translate("MainWindow", "NormalQuality", nullptr));
        comboViedo_Quality->setItemText(3, QCoreApplication::translate("MainWindow", "HighQuality", nullptr));
        comboViedo_Quality->setItemText(4, QCoreApplication::translate("MainWindow", "VeryHighQuality", nullptr));

        label_7->setText(QCoreApplication::translate("MainWindow", "\350\247\206\351\242\221\345\210\206\350\276\250\347\216\207", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "\345\275\225\345\203\217\350\256\276\347\275\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
