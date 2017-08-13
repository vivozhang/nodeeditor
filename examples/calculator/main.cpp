#include <nodes/NodeData>
#include <nodes/FlowScene>
#include <nodes/FlowView>
#include <nodes/ConnectionStyle>

#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QDockWidget>
#include <QtWidgets>

#include <nodes/DataModelRegistry>

#include "NumberSourceDataModel.hpp"
#include "NumberDisplayDataModel.hpp"
#include "AdditionModel.hpp"
#include "SubtractionModel.hpp"
#include "MultiplicationModel.hpp"
#include "DivisionModel.hpp"
#include "ModuloModel.hpp"
#include "DecimalToIntegerModel.hpp"
#include "IntegerToDecimalModel.hpp"

using QtNodes::DataModelRegistry;
using QtNodes::FlowScene;
using QtNodes::FlowView;
using QtNodes::ConnectionStyle;

static std::shared_ptr<DataModelRegistry>
registerDataModels()
{
  auto ret = std::make_shared<DataModelRegistry>();
  ret->registerModel<NumberSourceDataModel>("Sources");

  ret->registerModel<NumberDisplayDataModel>("Displays");

  ret->registerModel<AdditionModel>("Operators");

  ret->registerModel<SubtractionModel>("Operators");

  ret->registerModel<MultiplicationModel>("Operators");

  ret->registerModel<DivisionModel>("Operators");

  ret->registerModel<ModuloModel>("Operators");

  ret->registerModel<DecimalToIntegerModel, true>("Type converters");

  ret->registerModel<IntegerToDecimalModel, true>("Type converters");

  return ret;
}


static
void
setStyle()
{
  ConnectionStyle::setConnectionStyle(
  R"(
  {
    "ConnectionStyle": {
      "UseDataDefinedColors": true
    }
  }
  )");
}


int
main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  setStyle();

  QWidget mainWidget;
  QWidget libWidget;
  auto skipText = QStringLiteral("skip me");

  auto menuBar    = new QMenuBar();
  auto libMenuBar    = new QMenuBar();

  QMenu *fileMenu;
  QMenu *winMenu;
  QMenu *helpMenu;

    QAction *newModelAct;
    QAction *openAct;
    QAction *openRecentAct;
    QAction *importFromAct;
    QAction *vivoPpAct;
    QAction *quitVpAct;
    QAction *libBrowserAct;
    QAction *helpViewrAct;
    QAction *demoAct;
    QAction *showConsoleAct;
    QAction *showLogAct;
    QAction *aboutVpAct;

    newModelAct = new QAction(("New"),&libWidget);



  fileMenu =libMenuBar->addMenu(("File"));
  fileMenu->addAction(newModelAct);

  auto fileAction = libMenuBar->addAction("File");
  auto windowAction = libMenuBar->addAction("Window");
  auto helpAction = libMenuBar->addAction("Help");

  auto saveAction = menuBar->addAction("Save..");
  auto loadAction = menuBar->addAction("Load..");


  QVBoxLayout *l = new QVBoxLayout(&mainWidget);
  QVBoxLayout *b = new QVBoxLayout(&libWidget);

  l->addWidget(menuBar);
  b->addWidget(libMenuBar);
  auto scene = new FlowScene(registerDataModels());
  //auto dockLib = new QDockWidget;

  l->addWidget(new FlowView(scene));
  //l->addWidget(dockLib);



  //dockLib->setAllowedAreas(false);

  auto *treeView = new QTreeWidget();

  auto *txtBox = new QLineEdit;
  txtBox->setPlaceholderText(QStringLiteral("Filter"));
  txtBox->setClearButtonEnabled(true);

  //auto *treeViewAction = new QWidgetAction;
  //treeViewAction->setDefaultWidget(treeView);

  //QVBoxLayout *m = new QVBoxLayout(dockLib);
  b->addWidget(txtBox);
  b->addWidget(treeView);

  QMap<QString, QTreeWidgetItem*> topLevelItems;
  for (auto const &cat : scene->registry().categories())
  {
    auto item = new QTreeWidgetItem(treeView);
    item->setText(0, cat);
    item->setData(0, Qt::UserRole, skipText);
    topLevelItems[cat] = item;
  }

  for (auto const &assoc : scene->registry().registeredModelsCategoryAssociation())
  {
    auto parent = topLevelItems[assoc.second];
    auto item = new QTreeWidgetItem(parent);
    item->setText(0, assoc.first);
    item->setData(0, Qt::UserRole, assoc.first);
  }

  treeView->expandAll();



  l->setContentsMargins(0, 0, 0, 0);
  l->setSpacing(0);

  b->setContentsMargins(0, 0, 0, 0);
  b->setSpacing(0);

  QObject::connect(saveAction, &QAction::triggered,
                   scene, &FlowScene::save);

  QObject::connect(loadAction, &QAction::triggered,
                   scene, &FlowScene::load);

  mainWidget.setWindowTitle("Dataflow tools: simplest calculator");
  mainWidget.resize(800, 600);
  mainWidget.showNormal();

  libWidget.setWindowTitle("Library browse");
  libWidget.resize(250, 500);
  libWidget.showNormal();

  return app.exec();
}
