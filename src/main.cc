//LIC

#include <QApplication>
#include <QWizardPage>
#include <QLabel>
#include <QVBoxLayout>
#include <QFontDatabase>
#include <QSurfaceFormat>
#include <fstream>

#include "window.h"

//HEA

QWizardPage *createIntroPage()
{
  QWizardPage *page = new QWizardPage;
  page->setTitle("General information");

  page->setPixmap(QWizard::WatermarkPixmap, QPixmap("./splash.png"));

  QLabel *label = new QLabel("Most parts of this program and its associated data have been developed by Domingo Martín (dmartin@ugr.es)\n\nThe program can be used in non-commercial applications\n\nThis program is the complement of the article \"A survey of Digital Stippling\" by Domingo Martín, Germán Arroyo, Alex Rodríguez and Tobias Isenberg published in Computer & Graphics.\n\nPlease, in case that you use this software, add a reference to it");
  label->setWordWrap(true);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(label);
  page->setLayout(layout);

  return page;
}

//HEA

int main(int argc, char *argv[])
{
  string File_name;


  QApplication Application(argc, argv);
  _window Window;
  //Window.read_settings();

  //QWizard wizard;
  //wizard.setOptions(QWizard::NoCancelButton | QWizard::NoBackButtonOnLastPage);
  //wizard.addPage(createIntroPage());
  //wizard.setWindowTitle("License");
  //wizard.show();
  //wizard.raise();

  #ifdef REDIRECT
  std::ofstream Output_stream("log.txt",std::ofstream::out);
  cout.rdbuf(Output_stream.rdbuf());
  #endif

  #ifdef DEBUG_M
  cout << "DEBUG_Mging" << endl;
  #endif

  setlocale(LC_NUMERIC, "C");

  QSurfaceFormat Format;
  Format.setDepthBufferSize(24);
  Format.setStencilBufferSize(8);
  QSurfaceFormat::setDefaultFormat(Format);

  QFont Font("Arial");
  Font.setPointSize(10);
  Application.setFont(Font);

  //QFontDatabase Database;
  //int Result = Database.addApplicationFont("fonts/Ubuntu-R.ttf");
  //QFont Font=QFont("Ubuntu",10);
  //Application.setFont(Font);

  Window.show();
  return Application.exec();
}
