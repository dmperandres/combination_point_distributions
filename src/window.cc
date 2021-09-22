//LIC

#include "window.h"
#include "glwidget.h"
#include "images_tab.h"

//HEA

_window::_window()
{
  QSizePolicy Q(QSizePolicy::Expanding,QSizePolicy::Expanding);

  QWidget *Central_widget = new QWidget(this);

  // layout for results
  QHBoxLayout *Result_horizontal_frame = new QHBoxLayout;
  Result_horizontal_frame->setContentsMargins(1,1,1,1);

  QGLFormat Format;
  Format.setDoubleBuffer(true);
  Format.setRgba(true);
  Format.setAlpha(true);
  QGLFormat::setDefaultFormat(Format);

  // the result widget
  Result_widget=new QWidget(Central_widget);

  // frame widget for OpenGL windows
  Framed_widget= new QFrame(Result_widget);
  Framed_widget->setSizePolicy(Q);
  Framed_widget->setFrameStyle(QFrame::Panel);
  Framed_widget->setLineWidth(3);

  // OpenGL windows inside the frame widget
  GL_widget=new _gl_widget(this,Framed_widget);// Central_widget
  GL_widget->setSizePolicy(Q);
  GL_widget->setMinimumSize(400,400);

  // the layout of the central widget
  QHBoxLayout *Frame_horizontal_frame = new QHBoxLayout;
  // add the widgets of the result window
  Frame_horizontal_frame->addWidget(GL_widget);

  Framed_widget->setLayout(Frame_horizontal_frame);

  // This is the table of the result window
  Tab_widget = new QTabWidget(Result_widget);

  // and the widgets to the tab
  Tab_widget->addTab(GL_widget->Images_tab, tr("Images"));
  Tab_widget->addTab(GL_widget->Filters_tab, tr("Filters"));

  QFontMetrics Font_metric(Tab_widget->font());
  int Width_text = Font_metric.width("M");

  Tab_widget->setMaximumWidth(Width_text*_window_ns::WIDTH_TAB_CHARS);

  // add the widgets of the result window
  Result_horizontal_frame->addWidget(Framed_widget);
  //Horizontal_frame->addWidget(GL_widget);
  Result_horizontal_frame->addWidget(Tab_widget);

  // the layout of the result widget
  Result_widget->setLayout(Result_horizontal_frame);

  // now the edition window
  Edit_widget=new QWidget(Central_widget);

  Edit_horizontal_frame= new QHBoxLayout;

  // the scene for designing the filter
  Scene = new _graphics_scene(this);

  QFontMetrics fm(Scene->font());
  int w = fm.width("A");
  int h = fm.height();

  Nodes_editor= new _nodes_editor(Edit_widget);
  Nodes_editor->install(Scene);
  Nodes_editor->set_font_sizes(h,w);
  Nodes_editor->add_grid();

  // the view for the scene
  View = new _graphics_view(Nodes_editor);
  View->setSceneRect(-100,-100,5000,5000);
  View->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  View->setRenderHint(QPainter::Antialiasing, true);
  View->resetTransform();

  // widget for the filters
  QWidget *Filters_widget=new QWidget(Edit_widget);
  Filters_widget->setMaximumWidth(Width_text*_window_ns::WIDTH_TAB_CHARS);
  Filters_widget->setMinimumWidth(Width_text*_window_ns::WIDTH_TAB_CHARS);
  QVBoxLayout *Filters_vertical_frame= new QVBoxLayout;

  Filters_list=new _tree_widget(this,Edit_widget);
  Filters_list->setHeaderLabel(tr("Filter"));
  Filters_list->setIndentation(0);

  Filters_list->setContextMenuPolicy(Qt::CustomContextMenu);

  connect(Filters_list, SIGNAL(itemClicked(QTreeWidgetItem *, int)),this,SLOT(filter_selected_slot(QTreeWidgetItem *, int)));

  Filters_list->setDragEnabled(true);
  Filters_list->setSelectionMode(QAbstractItemView::SingleSelection);
  Filters_list->setAcceptDrops(false);
  Filters_list->setDropIndicatorShown(true);
  Filters_list->setDragDropMode(QAbstractItemView::DragOnly);

  create_filter_list();

  Filters_vertical_frame->addWidget(Filters_list);

  Filters_widget->setLayout(Filters_vertical_frame);

  // layout
  Edit_horizontal_frame->addWidget(Filters_widget);
  Edit_horizontal_frame->addWidget(View);
  // set the layout
  Edit_widget->setLayout(Edit_horizontal_frame);

  // the main tab widget that has the editor window and the result window
  Main_tab_widget=new QTabWidget(Central_widget);
  Main_tab_widget->insertTab(0,Edit_widget,tr("Node Editor"));
  Main_tab_widget->insertTab(1,Result_widget,tr("Results"));
  Main_tab_widget->setTabEnabled(0,false);
  Main_tab_widget->setTabEnabled(1,false);

  // the layout of the central widget
  QHBoxLayout *Horizontal_main = new QHBoxLayout(Central_widget);
  Horizontal_main->setSpacing(10);
  Horizontal_main->setMargin(3);
  Horizontal_main->addWidget(Main_tab_widget);

  Central_widget->setLayout(Horizontal_main);

  setCentralWidget(Central_widget);

  // connect the tabs for changing form edition to visualization
  connect(Main_tab_widget,SIGNAL(currentChanged(int)),this,SLOT(tabbar_clicked_slot(int)));

  // actions for file menu
  Open_file = new QAction(QIcon(":/icons/fileopen.png"), tr("&Open input image"), this);
  Open_file->setShortcuts(QKeySequence::Open);
  Open_file->setToolTip(tr("Open a file"));
  connect(Open_file, SIGNAL(triggered()), this, SLOT(open_file_slot()));
  Open_file->setEnabled(false);

  Save_file = new QAction(QIcon(":/icons/filesave.png"), tr("&Save resulting stipple image "), this);
  Save_file->setShortcuts(QKeySequence::Save);
  Save_file->setToolTip(tr("Save a file"));
  connect(Save_file, SIGNAL(triggered()), this, SLOT(save_file_slot()));
  Save_file->setEnabled(false);

  // actions for effect
  New_effect = new QAction(tr("&New effect"), this);
  //Open_effect_file->setShortcuts(QKeySequence(tr("P")));
  New_effect->setToolTip(tr("New effect"));
  connect(New_effect, SIGNAL(triggered()), this, SLOT(new_effect_slot()));

  Open_effect_file = new QAction(QIcon(":/icons/fileopen.png"), tr("&Open effect file"), this);
  //Open_effect_file->setShortcuts(QKeySequence(tr("P")));
  Open_effect_file->setToolTip(tr("Open an effect file"));
  connect(Open_effect_file, SIGNAL(triggered()), this, SLOT(open_effect_file_slot()));

  Save_effect_file = new QAction(QIcon(":/icons/filesave.png"), tr("&Save effect file"), this);
  //Save_effect_file->setShortcuts(QKeySequence(tr("Ctrl+S")));
  Save_effect_file->setToolTip(tr("Save an effect file"));
  connect(Save_effect_file, SIGNAL(triggered()), this, SLOT(save_effect_file_slot()));
  Save_effect_file->setEnabled(false);

  QAction *Exit = new QAction(QIcon(":/icons/exit.png"), tr("&Exit..."), this);
  Exit->setShortcut(tr("Ctrl+Q"));
  Exit->setToolTip(tr("Exit the application"));
  connect(Exit, SIGNAL(triggered()), this, SLOT(close()));

  // menus
  QMenu *File_menu=menuBar()->addMenu(tr("&File"));
  File_menu->addAction(Open_file);
  File_menu->addAction(Save_file);
  File_menu->addSeparator();
  File_menu->addAction(New_effect);
  File_menu->addAction(Open_effect_file);
  File_menu->addAction(Save_effect_file);
  File_menu->addSeparator();
  File_menu->addAction(Exit);
  File_menu->setAttribute(Qt::WA_AlwaysShowToolTips);

  setWindowTitle(tr("StippleShop"));
  //
  Timer=nullptr;

  QRect Screen_size = QApplication::primaryScreen()->geometry();
  resize(Screen_size.width()*_window_ns::SCREEN_WIDTH_FACTOR,Screen_size.height()*_window_ns::SCREEN_HEIGHT_FACTOR);
}

//HEA

void _window::open_file_slot()
{
  QString File_name1 = QFileDialog::getOpenFileName(this, tr("Open File"),"./images",tr("Images (*.png *.jpg *.pgm)"));

  if (!File_name1.isEmpty()){
    QFile File(File_name1);
    if (!File.open(QFile::ReadOnly | QFile::Text)) {
      QMessageBox::warning(this, tr("Application"),tr("Cannot read file %1:\n%2.").arg(File_name1,File.errorString()));
      return;
    }
    Tab_widget->setEnabled(true);
    GL_widget->read_image(File_name1.toStdString());
    // the name is saved
    File_name=File_name1.toStdString();
    Save_file->setEnabled(true);
  }
}

//HEA

bool _window::save_file_slot()
{
  QString File_name;
  QFileDialog File_dialog;
  int File_type=-1;

  do {
    File_name = File_dialog.getSaveFileName(this, tr("Save File"),"./images",tr("Images (*.png *.jpg)"));
    if (File_name.isEmpty()) return false;
    if (File_name.endsWith(".png")) File_type=0;
    if (File_name.endsWith(".jpg")) File_type=1;
    if (File_type==-1){
      QMessageBox::warning(this, tr("Warning"),tr("The file must be of type .png or .jpg"));
    }
  } while (File_type==-1);

  GL_widget->save_image(File_name.toStdString());

  return true;
}

//HEA

void _window::new_effect_slot()
{
  Nodes_editor->clear();
  Scene->clear();
  View->items().clear();

  Nodes_editor->add_grid();
  Nodes_editor->add_initial_blocks();

  View->setScene(Scene);
  View->setSceneRect(-100,-100,5000,5000);
  View->setRenderHint(QPainter::Antialiasing, true);

  View->horizontalScrollBar()->setValue(-1000);
  View->verticalScrollBar()->setValue(-1000);

  Main_tab_widget->setTabEnabled(0,true);
  Main_tab_widget->setTabEnabled(1,true);

  Nodes_editor->changed(true);

  Save_effect_file->setEnabled(false);

//  GL_widget->clear_data();
}

//HEA

void _window::open_effect_file_slot()
{
  QString File_name1 = QFileDialog::getOpenFileName(this, tr("Open efect"),"./effects",tr("Effects (*.json)"));

  if (!File_name1.isEmpty()){
    QFile File(File_name1);
    if (!File.open(QFile::ReadOnly | QFile::Text)) {
      QMessageBox::warning(this, tr("Application"),tr("Cannot read file %1:\n%2.").arg(File_name1,File.errorString()));
      return;
    }

    Nodes_editor->clear();
    Scene->clear();

    GL_widget->clear_data();

    Nodes_editor->read_data_effect(File_name1.toStdString());
    Nodes_editor->create_blocks_from_effect();

    Scene->setSceneRect(-100,-100,5000,5000);
    QRectF Rect=Scene->itemsBoundingRect();

    Nodes_editor->add_grid();

    View->setScene(Scene);
    View->setSceneRect(-100,-100,5000,5000);
    View->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    View->setRenderHint(QPainter::Antialiasing, true);

    View->scaling_factor((View->width()-80)/Rect.width());

    Main_tab_widget->setTabEnabled(0,true);
    Main_tab_widget->setTabEnabled(1,true);

    Nodes_editor->changed(true);
  }
}

//HEA

void _window::save_effect_file_slot()
{
QString File_name1 = QFileDialog::getSaveFileName(this, tr("Save efect"),"./effects",tr("Effects (*.json)"));

if (!File_name1.isEmpty()){
  GL_widget->write_data_effect(File_name1.toStdString());
  }
}

//HEA

void _window::closeEvent(QCloseEvent *event)
{
  event->accept();
}

//HEA

void _window::set_frame_warning(bool Estate)
{
  QPalette Palette;

  if (Estate){
    Palette.setColor(QPalette::WindowText, Qt::red);
    Framed_widget->setPalette(Palette);
  }
  else{
    // change the color of the frame
    Palette.setColor(QPalette::WindowText, Qt::black);
    Framed_widget->setPalette(Palette);
  }
}

//HEA

void _window::tabbar_clicked_slot(int Index1)
{
  // check if the result is correct
  if (Index1==0){
    // from visualization to edition
    Open_file->setEnabled(false);
    Save_file->setEnabled(false);
    New_effect->setEnabled(true);
    Open_effect_file->setEnabled(true);
    if (GL_widget->image_loaded()==true) Save_effect_file->setEnabled(true);

    Nodes_editor->changed(false);
    GL_widget->update_filters_parameters();

    View->horizontalScrollBar()->setValue(-1000);
    View->verticalScrollBar()->setValue(-1000);
  }
  else{
    // from edition to visualization
    // check if the structure is correct (there is not loop checking)
    if (Nodes_editor->create_blocks_from_editor()==false){
      // there are problems, return to edition tab
      Main_tab_widget->setCurrentIndex(0);
    }
    else{
      Open_file->setEnabled(true);
      if (GL_widget->image_loaded()) Save_file->setEnabled(true);
      else Save_file->setEnabled(false);
      New_effect->setEnabled(false);
      Open_effect_file->setEnabled(false);
      Save_effect_file->setEnabled(false);

      if (Nodes_editor->changed()==true){
        GL_widget->create_filters_from_blocks(Nodes_editor->map_data_blocks(),Nodes_editor->vec_order(),Nodes_editor->map_filters_parameters());
      }
      else{
        GL_widget->refresh_image();
      }
    }
  }
}

//HEA

void _window::filter_selected_slot(QTreeWidgetItem * item, int column)
{
  Q_UNUSED(column)
  if (item->type()==ItemType2){
    QVariant Value=item->data(0,1);
    Filter_parameters=_f_filter_ns::Filter_name_parameters[_f_filter_ns::_type_filter(Value.toInt())];
  }
  else item->setSelected(false);
}

//HEA

void _window::add_filter(int Filter_pos)
{
  Filter_parameters=_f_filter_ns::Filter_name_parameters[_f_filter_ns::_type_filter(Filter_pos)];
  Nodes_editor->add_block(Filter_parameters.Type,Filter_parameters.Name,Filter_parameters.Num_parameters, Filter_parameters.Num_input_ports,Filter_parameters.Num_channels);

  // now the filter is deselected
  QList<QTreeWidgetItem *> List_selected=Filters_list->selectedItems();
  for (int i=0;i<List_selected.size();i++){
   List_selected[i]->setSelected(false);
  }
  Filter_parameters.Type="";
}

//HEA

void _window::add_filter(int Filter_pos, float Col, float Row)
{

  Filter_parameters=_f_filter_ns::Filter_name_parameters[_f_filter_ns::_type_filter(Filter_pos)];
  Nodes_editor->add_block(Filter_parameters.Type,Filter_parameters.Name,Filter_parameters.Num_parameters, Filter_parameters.Num_input_ports, Filter_parameters.Num_channels, Col,Row);

  // now the filter is deselected
  QList<QTreeWidgetItem *> List_selected=Filters_list->selectedItems();
  for (int i=0;i<List_selected.size();i++){
   List_selected[i]->setSelected(false);
  }
  Filter_parameters.Type="";
}

//HEA

void _window::description(int Filter_pos)
{
  _f_filter_ns::_filter_text Text=_f_filter_ns::Filter_name_text[_f_filter_ns::_type_filter(Filter_pos)];

  QMessageBox MsgBox;
  MsgBox.setIcon(QMessageBox::Information);
  MsgBox.setFixedSize(500,300);
  MsgBox.setText(QString::fromStdString(Text.Name));
  MsgBox.setInformativeText( QString::fromStdString(Text.Description));
  MsgBox.setStandardButtons(QMessageBox::Ok);

  // trick to change the messagebox size
  QSpacerItem *Horizontal_spacer=new QSpacerItem(500,0,QSizePolicy::Minimum,QSizePolicy::Expanding);
  QGridLayout *Layout=(static_cast<QGridLayout *>(MsgBox.layout()));
  Layout->addItem(Horizontal_spacer,Layout->rowCount(),0,1,Layout->columnCount());
  MsgBox.exec();
}

//HEA

void _window::create_filter_list()
{
  QFont Font;
  Font.setBold(true);

  // Stippling
#ifdef DEFINE_TYPE_STIPPLING
  _tree_widget_item *Stippling_type = new _tree_widget_item(Filters_list,ItemType1);
//  Placement_and_stippling_type->setBackgroundColor(0,_window_ns::Stippling_separator_color);
  Stippling_type->setFont(0,Font);
  Stippling_type->setText(0, tr("Stippling"));
  Stippling_type->setExpanded(true);
#endif

  #ifdef DEFINE_FILTER_STIPPLING_EBG
  QTreeWidgetItem *Stippling_EBG = new QTreeWidgetItem(Stippling_type,ItemType2);
  Stippling_EBG->setData(0,0,tr("Example-Based Grayscale"));
  Stippling_EBG->setData(0,1,_f_filter_ns::FILTER_STIPPLING_EBG);
  Stippling_EBG->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_STIPPLING_EBG].Description));
  #endif

  #ifdef DEFINE_FILTER_STIPPLING_EBG_MASK
  QTreeWidgetItem *Stippling_EBG_mask = new QTreeWidgetItem(Stippling_type,ItemType2);
  Stippling_EBG_mask->setData(0,0,tr("EBG mask"));
  Stippling_EBG_mask->setData(0,1,_f_filter_ns::FILTER_STIPPLING_EBG_MASK);
  Stippling_EBG_mask->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_STIPPLING_EBG_MASK].Description));
  #endif

  #ifdef DEFINE_FILTER_STIPPLING_BY_EXAMPLE
  QTreeWidgetItem *Stippling_SBE = new QTreeWidgetItem(Stippling_type,ItemType2);
  Stippling_SBE->setData(0,0,tr("Stippling By Example"));
  Stippling_SBE->setData(0,1,_f_filter_ns::FILTER_STIPPLING_SBE);
  Stippling_SBE->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_STIPPLING_SBE].Description));
  #endif

  // Placement
  #ifdef DEFINE_TYPE_PLACEMENT
  _tree_widget_item *Placement_type = new _tree_widget_item(Filters_list,ItemType1);
  Placement_type->setFont(0,Font);
  Placement_type->setText(0, tr("Placement"));
  Placement_type->setExpanded(true);
  #endif

  #ifdef DEFINE_FILTER_CCVT
  _tree_widget_item *Placement_ccvt = new _tree_widget_item(Placement_type,ItemType2);
  Placement_ccvt->setData(0,0,tr("Capacity-Constrained Voronoi Tessellation"));
  Placement_ccvt->setData(0,1,_f_filter_ns::FILTER_CCVT);
  Placement_ccvt->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_CCVT].Description));
  Placement_ccvt->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
  #endif

  #ifdef DEFINE_FILTER_RWT
  QTreeWidgetItem *Placement_RWT = new QTreeWidgetItem(Placement_type,ItemType2);
  Placement_RWT->setData(0,0,tr("Recursive Wang Tiling"));
  Placement_RWT->setData(0,1,_f_filter_ns::FILTER_RWT);
  Placement_RWT->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_RWT].Description));
  #endif

  #ifdef DEFINE_FILTER_STRUCTURE_AWARE_STIPPLING
  QTreeWidgetItem *Stippling_SAS = new QTreeWidgetItem(Placement_type,ItemType2);
  Stippling_SAS->setData(0,0,tr("Structure-Aware Stippling"));
  Stippling_SAS->setData(0,1,_f_filter_ns::FILTER_STIPPLING_SAS);
  Stippling_SAS->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_STIPPLING_SAS].Description));
  #endif

  #ifdef DEFINE_FILTER_WCVD
  QTreeWidgetItem *Placement_WCVD = new QTreeWidgetItem(Placement_type,ItemType2);
  Placement_WCVD->setData(0,0,tr("Weigthed Centroidal Voronoi Diagram"));
  Placement_WCVD->setData(0,1,_f_filter_ns::FILTER_WCVD);
  Placement_WCVD->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_WCVD].Description));
  #endif

  // halftoning
  #ifdef DEFINE_TYPE_HALFTONING
  QTreeWidgetItem *Halftoning_type = new QTreeWidgetItem(Filters_list,ItemType1);
//  Halftoning_type->setBackgroundColor(0,_window_ns::Halftoning_separator_color);
  Halftoning_type->setFont(0,Font);
  Halftoning_type->setText(0, tr("Halftoning"));
  Halftoning_type->setExpanded(true);
  #endif

  #ifdef DEFINE_FILTER_HALFTONING_ADAPTIVE_CLUSTERING_SELECTIVE_PRECIPITATION
  QTreeWidgetItem *Halftoning_ACSP = new QTreeWidgetItem(Halftoning_type,ItemType2);
  Halftoning_ACSP->setData(0,0,tr("Adaptive Clustering Selective Precipitation"));
  Halftoning_ACSP->setData(0,1,_f_filter_ns::FILTER_HALFTONING_ACSP);
  Halftoning_ACSP->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_HALFTONING_ACSP].Description));
  #endif

  #ifdef DEFINE_FILTER_CONTRAST_AWARE_HALFTONING
  QTreeWidgetItem *Halftoning_CAH = new QTreeWidgetItem(Halftoning_type,ItemType2);
  Halftoning_CAH->setData(0,0,tr("Contrast-Aware"));
  Halftoning_CAH->setData(0,1,_f_filter_ns::FILTER_HALFTONING_CAH);
  Halftoning_CAH->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_HALFTONING_CAH].Description));
  #endif

  #ifdef DEFINE_FILTER_HALFTONING_OSTROMOUKHOV
  QTreeWidgetItem *Halftoning_ost = new QTreeWidgetItem(Halftoning_type,ItemType2);
  Halftoning_ost->setData(0,0,tr("Ostromoukhov"));
  Halftoning_ost->setData(0,1,_f_filter_ns::FILTER_HALFTONING_OST);
  Halftoning_ost->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_HALFTONING_OST].Description));
  #endif

  #ifdef DEFINE_FILTER_HALFTONING_SPACE_FILLING_CURVE
  QTreeWidgetItem *Halftoning_SFC = new QTreeWidgetItem(Halftoning_type,ItemType2);
  Halftoning_SFC->setData(0,0,tr("Space Filling Curve"));
  Halftoning_SFC->setData(0,1,_f_filter_ns::FILTER_HALFTONING_SFC);
  Halftoning_SFC->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_HALFTONING_SFC].Description));
  #endif

  // Dots types
  #ifdef DEFINE_TYPE_DOT_OUTPUT_CONTROL
  _tree_widget_item *Dots_type = new _tree_widget_item(Filters_list,ItemType1);
//  Dots_type->setBackgroundColor(0,_window_ns::Dot_separator_color);
  Dots_type->setFont(0,Font);
  Dots_type->setText(0, tr("Dot output control"));
  Dots_type->setExpanded(true);
  #endif

  #ifdef DEFINE_FILTER_DOT_EBG
  _tree_widget_item *Dot_type_ebg = new _tree_widget_item(Dots_type,ItemType2);
  Dot_type_ebg->setData(0,0,tr("Dot EBG"));
  Dot_type_ebg->setData(0,1,_f_filter_ns::FILTER_DOT_EBG);
  Dot_type_ebg->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_DOT_EBG].Description));
  #endif

  #ifdef DEFINE_FILTER_DOT_SVG
  _tree_widget_item *Dot_type_svg = new _tree_widget_item(Dots_type,ItemType2);
  Dot_type_svg->setData(0,0,tr("Dot SVG"));
  Dot_type_svg->setData(0,1,_f_filter_ns::FILTER_DOT_SVG);
  Dot_type_svg->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_DOT_SVG].Description));
  #endif

  #ifdef DEFINE_FILTER_DOT_SVG_MASK
  _tree_widget_item *Dot_type_svg_mask = new _tree_widget_item(Dots_type,ItemType2);
  Dot_type_svg_mask->setData(0,0,tr("Dot SVG mask"));
  Dot_type_svg_mask->setData(0,1,_f_filter_ns::FILTER_DOT_SVG_MASK);
  Dot_type_svg_mask->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_DOT_SVG_MASK].Description));
  #endif

  // contrast
  #ifdef DEFINE_TYPE_CONTRAST
  QTreeWidgetItem *Contrast_type = new QTreeWidgetItem(Filters_list,ItemType1);
//  Contrast_type->setBackgroundColor(0,_window_ns::Contrast_separator_color);
  Contrast_type->setFont(0,Font);
  Contrast_type->setText(0, tr("Contrast"));
  Contrast_type->setExpanded(true);
  #endif

  #ifdef DEFINE_FILTER_CONTRAST_BRIGHT
  QTreeWidgetItem *Contrast_contrast = new QTreeWidgetItem(Contrast_type,ItemType2);
  Contrast_contrast->setData(0,0,tr("Contrast Bright"));
  Contrast_contrast->setData(0,1,_f_filter_ns::FILTER_CONTRAST_BRIGHT);
  Contrast_contrast->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_CONTRAST_BRIGHT].Description));
  #endif

  #ifdef DEFINE_FILTER_RETINEX
  QTreeWidgetItem *Contrast_retinex = new QTreeWidgetItem(Contrast_type,ItemType2);
  Contrast_retinex->setData(0,0,tr("Retinex"));
  Contrast_retinex->setData(0,1,_f_filter_ns::FILTER_RETINEX);
  Contrast_retinex->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_RETINEX].Description));
  #endif

  #ifdef DEFINE_FILTER_RETINEX_A
  QTreeWidgetItem *Contrast_retinex_a = new QTreeWidgetItem(Contrast_type,ItemType2);
  Contrast_retinex_a->setData(0,0,tr("Retinex A"));
  Contrast_retinex_a->setData(0,1,_f_filter_ns::FILTER_RETINEX_A);
  Contrast_retinex_a->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_RETINEX_A].Description));
  #endif

  #ifdef DEFINE_FILTER_RETINEX_B
  QTreeWidgetItem *Contrast_retinex_b = new QTreeWidgetItem(Contrast_type,ItemType2);
  Contrast_retinex_b->setData(0,0,tr("Retinex B"));
  Contrast_retinex_b->setData(0,1,_f_filter_ns::FILTER_RETINEX_B);
  Contrast_retinex_b->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_RETINEX_B].Description));
  #endif

  #ifdef DEFINE_FILTER_RETINEX_B_GPU
  QTreeWidgetItem *Contrast_retinex_c = new QTreeWidgetItem(Contrast_type,ItemType2);
  Contrast_retinex_c->setData(0,0,tr("Retinex B GPU"));
  Contrast_retinex_c->setData(0,1,_f_filter_ns::FILTER_RETINEX_B_GPU);
  Contrast_retinex_c->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_RETINEX_B_GPU].Description));
  #endif

  // edges
  #ifdef DEFINE_TYPE_EDGE_DETECTORS
  QTreeWidgetItem *Edges_detector_type = new QTreeWidgetItem(Filters_list,ItemType1);
//  Edges_detector_type->setBackgroundColor(0,_window_ns::Edges_separator_color);
  Edges_detector_type->setFont(0,Font);
  Edges_detector_type->setText(0, tr("Edges detectors"));
  Edges_detector_type->setExpanded(true);
  #endif

  #ifdef DEFINE_FILTER_CANNY
  QTreeWidgetItem *Edges_detector_canny = new QTreeWidgetItem(Edges_detector_type,ItemType2);
  Edges_detector_canny->setData(0,0,tr("Canny"));
  Edges_detector_canny->setData(0,1,_f_filter_ns::FILTER_CANNY);
  Edges_detector_canny->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_CANNY].Description));
  #endif

  #ifdef DEFINE_FILTER_CANNY_AUTO
  QTreeWidgetItem *Edges_detector_canny_auto = new QTreeWidgetItem(Edges_detector_type,ItemType2);
  Edges_detector_canny_auto->setData(0,0,tr("Canny auto"));
  Edges_detector_canny_auto->setData(0,1,_f_filter_ns::FILTER_CANNY_AUTO);
  Edges_detector_canny_auto->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_CANNY_AUTO].Description));
  #endif

  #ifdef DEFINE_FILTER_DOG
  QTreeWidgetItem *Edges_detector_dog = new QTreeWidgetItem(Edges_detector_type,ItemType2);
  Edges_detector_dog->setData(0,0,tr("DoG"));
  Edges_detector_dog->setData(0,1,_f_filter_ns::FILTER_DOG);
  Edges_detector_dog->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_DOG].Description));
  #endif

  #ifdef DEFINE_FILTER_KANG
  QTreeWidgetItem *Edges_detector_kang = new QTreeWidgetItem(Edges_detector_type,ItemType2);
  Edges_detector_kang->setData(0,0,tr("Kang"));
  Edges_detector_kang->setData(0,1,_f_filter_ns::FILTER_KANG);
  Edges_detector_kang->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_KANG].Description));
  #endif

  #ifdef DEFINE_FILTER_LAPLACIAN
  QTreeWidgetItem *Edges_detector_laplacian = new QTreeWidgetItem(Edges_detector_type,ItemType2);
  Edges_detector_laplacian->setData(0,0,tr("Laplacian"));
  Edges_detector_laplacian->setData(0,1,_f_filter_ns::FILTER_LAPLACIAN);
  Edges_detector_laplacian->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_LAPLACIAN].Description));
  #endif

  #ifdef DEFINE_FILTER_SOBEL
  QTreeWidgetItem *Edges_detector_sobel = new QTreeWidgetItem(Edges_detector_type,ItemType2);
  Edges_detector_sobel->setData(0,0,tr("Sobel"));
  Edges_detector_sobel->setData(0,1,_f_filter_ns::FILTER_SOBEL);
  Edges_detector_sobel->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_SOBEL].Description));
  #endif

  // blur
  #ifdef DEFINE_TYPE_BLUR
  QTreeWidgetItem *Blur_type = new QTreeWidgetItem(Filters_list,ItemType1);
//  Blur_type->setBackgroundColor(0,_window_ns::Blur_separator_color);
  Blur_type->setFont(0,Font);
  Blur_type->setText(0, tr("Blur"));
  Blur_type->setExpanded(true);
  #endif

  #ifdef DEFINE_FILTER_GAUSSIAN
  QTreeWidgetItem *Blur_gaussian = new QTreeWidgetItem(Blur_type,ItemType2);
  Blur_gaussian->setData(0,0,tr("Gaussian"));
  Blur_gaussian->setData(0,1,_f_filter_ns::FILTER_GAUSSIAN);
  Blur_gaussian->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_GAUSSIAN].Description));
  #endif

  // effects
  #ifdef DEFINE_TYPE_EFFECT
  QTreeWidgetItem *Effect_type = new QTreeWidgetItem(Filters_list,ItemType1);
//  Effect_type->setBackgroundColor(0,_window_ns::Effect_separator_color);
  Effect_type->setFont(0,Font);
  Effect_type->setText(0, tr("Effect"));
  Effect_type->setExpanded(true);
  #endif

  #ifdef DEFINE_FILTER_BILATERAL
  QTreeWidgetItem *Effect_bilateral = new QTreeWidgetItem(Effect_type,ItemType2);
  Effect_bilateral->setData(0,0,tr("Bilateral"));
  Effect_bilateral->setData(0,1,_f_filter_ns::FILTER_BILATERAL);
  Effect_bilateral->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_BILATERAL].Description));
  #endif

  #ifdef DEFINE_FILTER_BINARY
  QTreeWidgetItem *Effect_binary = new QTreeWidgetItem(Effect_type,ItemType2);
  Effect_binary->setData(0,0,tr("Binary"));
  Effect_binary->setData(0,1,_f_filter_ns::FILTER_BINARY);
  Effect_binary->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_BINARY].Description));
  #endif

  #ifdef DEFINE_FILTER_DILATION
  QTreeWidgetItem *Effect_dilation = new QTreeWidgetItem(Effect_type,ItemType2);
  Effect_dilation->setData(0,0,tr("Dilation"));
  Effect_dilation->setData(0,1,_f_filter_ns::FILTER_DILATION);
  Effect_dilation->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_DILATION].Description));
  #endif

  #ifdef DEFINE_FILTER_DISTANCE_FIELD
  QTreeWidgetItem *Effect_distance_field = new QTreeWidgetItem(Effect_type,ItemType2);
  Effect_distance_field->setData(0,0,tr("Distance field"));
  Effect_distance_field->setData(0,1,_f_filter_ns::FILTER_DISTANCE_FIELD);
  Effect_distance_field->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_DISTANCE_FIELD].Description));
  #endif

  #ifdef DEFINE_FILTER_DISTANCE_FIELD_FLOAT
  QTreeWidgetItem *Effect_distance_field_float = new QTreeWidgetItem(Effect_type,ItemType2);
  Effect_distance_field_float->setData(0,0,tr("Distance field float"));
  Effect_distance_field_float->setData(0,1,_f_filter_ns::FILTER_DISTANCE_FIELD_FLOAT);
  Effect_distance_field_float->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_DISTANCE_FIELD_FLOAT].Description));
  #endif

  #ifdef DEFINE_FILTER_DISTANCE_FIELD_LINES
  QTreeWidgetItem *Effect_distance_field_lines = new QTreeWidgetItem(Effect_type,ItemType2);
  Effect_distance_field_lines->setData(0,0,tr("Distance field lines"));
  Effect_distance_field_lines->setData(0,1,_f_filter_ns::FILTER_DISTANCE_FIELD_LINES);
  Effect_distance_field_lines->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_DISTANCE_FIELD_LINES].Description));
  #endif

  #ifdef DEFINE_FILTER_DOTS_EDGES
  QTreeWidgetItem *Effect__dots_edges = new QTreeWidgetItem(Effect_type,ItemType2);
  Effect__dots_edges->setData(0,0,tr("Dots edges"));
  Effect__dots_edges->setData(0,1,_f_filter_ns::FILTER_DOTS_EDGES);
  Effect__dots_edges->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_DOTS_EDGES].Description));
  #endif

  #ifdef DEFINE_FILTER_EROTION
  QTreeWidgetItem *Effect_erotion = new QTreeWidgetItem(Effect_type,ItemType2);
  Effect_erotion->setData(0,0,tr("Erotion"));
  Effect_erotion->setData(0,1,_f_filter_ns::FILTER_EROTION);
  Effect_erotion->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_EROTION].Description));
  #endif

  #ifdef DEFINE_FILTER_INVERSION
  QTreeWidgetItem *Effect_inversion = new QTreeWidgetItem(Effect_type,ItemType2);
  Effect_inversion->setData(0,0,tr("Inversion"));
  Effect_inversion->setData(0,1,_f_filter_ns::FILTER_INVERSION);
  Effect_inversion->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_INVERSION].Description));
  #endif

  #ifdef DEFINE_FILTER_BORDER_MASK
  QTreeWidgetItem *Effect_border_mask = new QTreeWidgetItem(Effect_type,ItemType2);
  Effect_border_mask->setData(0,0,tr("Border mask"));
  Effect_border_mask->setData(0,1,_f_filter_ns::FILTER_BORDER_MASK);
  Effect_border_mask->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_BORDER_MASK].Description));
  #endif

  // combination
  #ifdef DEFINE_TYPE_COMBINATION
  QTreeWidgetItem *Combination_type = new QTreeWidgetItem(Filters_list,ItemType1);
//  Combination_type->setBackgroundColor(0,_window_ns::Combination_separator_color);
  Combination_type->setFont(0,Font);
  Combination_type->setText(0, tr("Combination"));
  Combination_type->setExpanded(true);
  #endif

  #ifdef DEFINE_FILTER_COMBINATION
  QTreeWidgetItem *Combination_combination = new QTreeWidgetItem(Combination_type,ItemType2);
  Combination_combination->setData(0,0,tr("Combination"));
  Combination_combination->setData(0,1,_f_filter_ns::FILTER_COMBINATION);
  Combination_combination->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_COMBINATION].Description));
  #endif

  #ifdef DEFINE_FILTER_DISTANCE_FIELD_COMBINATION
  QTreeWidgetItem *Combination_df = new QTreeWidgetItem(Combination_type,ItemType2);
  Combination_df->setData(0,0,tr("DF combination"));
  Combination_df->setData(0,1,_f_filter_ns::FILTER_DISTANCE_FIELD_COMBINATION);
  Combination_df->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_DISTANCE_FIELD_COMBINATION].Description));
  #endif

  // segmentation
  #ifdef DEFINE_TYPE_SEGMENTATION
  QTreeWidgetItem *Segmentation_type = new QTreeWidgetItem(Filters_list,ItemType1);
  Segmentation_type->setFont(0,Font);
  Segmentation_type->setText(0, tr("Segmentation"));
  Segmentation_type->setExpanded(true);
  #endif

  #ifdef DEFINE_FILTER_MEAN_SHIFT
  QTreeWidgetItem *Segmentation_mean_shift = new QTreeWidgetItem(Segmentation_type,ItemType2);
  Segmentation_mean_shift->setData(0,0,tr("Mean-shift"));
  Segmentation_mean_shift->setData(0,1,_f_filter_ns::FILTER_MEAN_SHIFT);
  Segmentation_mean_shift->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_MEAN_SHIFT].Description));
  #endif

  // measure
  #ifdef DEFINE_TYPE_MEASURE
  QTreeWidgetItem *Measure_type = new QTreeWidgetItem(Filters_list,ItemType1);
//  Measure_type->setBackgroundColor(0,_window_ns::Measure_separator_color);
  Measure_type->setFont(0,Font);
  Measure_type->setText(0, tr("Measure"));
  Measure_type->setExpanded(true);
  #endif

  #ifdef DEFINE_FILTER_MEASURE_SSIM_PSNR
  QTreeWidgetItem *Measure_ssim_psnr = new QTreeWidgetItem(Measure_type,ItemType2);
  Measure_ssim_psnr->setData(0,0,tr("Measure SSIM PSNR"));
  Measure_ssim_psnr->setData(0,1,_f_filter_ns::FILTER_MEASURE_SSIM_PSNR);
  Measure_ssim_psnr->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_MEASURE_SSIM_PSNR].Description));
  #endif

  // auxiliar
  #ifdef DEFINE_TYPE_AUXILIAR
  QTreeWidgetItem *Auxiliar_type = new QTreeWidgetItem(Filters_list,ItemType1);
//  Measure_type->setBackgroundColor(0,_window_ns::Measure_separator_color);
  Auxiliar_type->setFont(0,Font);
  Auxiliar_type->setText(0, tr("Auxiliar"));
  Auxiliar_type->setExpanded(true);
  #endif

  #ifdef DEFINE_FILTER_COLORIZE_PIXELS
  QTreeWidgetItem *Colorize_pixels = new QTreeWidgetItem(Auxiliar_type,ItemType2);
  Colorize_pixels->setData(0,0,tr("Colorize pixels"));
  Colorize_pixels->setData(0,1,_f_filter_ns::FILTER_COLORIZE_PIXELS_);
  Colorize_pixels->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_COLORIZE_PIXELS_].Description));
  #endif

  #ifdef DEFINE_FILTER_LOAD_IMAGE
  QTreeWidgetItem *Load_image = new QTreeWidgetItem(Auxiliar_type,ItemType2);
  Load_image->setData(0,0,tr("Load image"));
  Load_image->setData(0,1,_f_filter_ns::FILTER_LOAD_IMAGE_);
  Load_image->setToolTip(0,QString::fromStdString(_f_filter_ns::Filter_name_text[_f_filter_ns::FILTER_LOAD_IMAGE_].Description));
  #endif
}
