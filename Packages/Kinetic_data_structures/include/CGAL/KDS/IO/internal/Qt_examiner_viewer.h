#ifndef CGAL_KDS_IO_INTERNAL_COIN_SIMULATOR_GUI_H
#define CGAL_KDS_IO_INTERNAL_COIN_SIMULATOR_GUI_H

#include <CGAL/KDS/basic.h>

//#ifdef HAVE_INVENTOR_QT_SOQT_H
//#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
//#include <Inventor/nodes/SoSeparator.h>
//#else
//#ifdef HAVE_COIN2_INVENTOR_QT_SOQT_H
//#include <Coin2/Inventor/Qt/SoQt.h>
//#include <Coin2/Inventor/Qt/viewers/SoQtExaminerViewer.h>
//#include <Coin2/Inventor/nodes/SoSeparator.h>
//#endif
//#endif


#include <CGAL/KDS/IO/internal/Qt_core.h>
#include <qapplication.h>
//#include <qmainwindow.h>
#include <map>

class QPushButton;
class SoSeparator;


// I think I need these here explicitly for MOC to work
namespace CGAL {
  namespace KDS {
    namespace internal {
      /*  Usage main_window_= SoQt::init(argc, argv, argv[0]);
	viewer_= new Coin_simulator_viewer(main_window_);
	SoQt::show(main_window_); SoQt::mainLoop();

	Note that for some reason QObject must come before
	SoQtExaminerViewer in the inheritence list.
      */
      class Qt_examiner_viewer : public QObject, public SoQtExaminerViewer {
	Q_OBJECT
      public:
	Qt_examiner_viewer(QWidget * parent);

	virtual ~Qt_examiner_viewer(){}
      
	void new_subgraph(SoNode *p);

	void delete_subgraph(SoNode *p);

	typedef Qt_core Button_handler;
	Button_handler *button_handler(){
	  return &core_;
	}
      protected:
      
	virtual void createViewerButtons(QWidget * parent, SbPList * buttonlist);

	SoSeparator* root_;
	QPushButton *play_button_, *pause_button_, *stop_button_;
	QPushButton *play_to_button_, *play_through_button_, *reverse_button_;
	QPushButton *faster_button_, *slower_button_;
	Qt_core core_;
      };




    };
   
  };
};


#endif // guard

