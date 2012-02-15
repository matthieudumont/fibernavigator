/////////////////////////////////////////////////////////////////////////////
// Description: mainFrame class. Contains every elements of the GUI, and frame events
/////////////////////////////////////////////////////////////////////////////

#ifndef MAINFRAME_H_
#define MAINFRAME_H_

#include <wx/checkbox.h>
#include <wx/grid.h>
#include <wx/treectrl.h>

#include "MainCanvas.h"
#include "MyListCtrl.h"
#include "ListCtrl.h"

#include "../misc/Algorithms/Helper.h"

class DatasetHelper;
class SelectionObject;
class DatasetInfo;
class ToolBar;
class MenuBar;
class SceneObject;

class MainFrame : public wxFrame
{
    friend class ToolBar;
    friend class MenuBar;
    friend class PropertiesWindow;

public:
    MainFrame( wxWindow *i_parent, const wxWindowID i_id, const wxString &i_title, const wxPoint &i_pos, const wxSize &i_size, const long i_style);
    ~MainFrame();

    void displayPropertiesSheet();
    void deleteSceneObject();
    void deleteListItem();
    void deleteTreeItem();
    void refreshAllGLWidgets();
    void refreshViews();
    void renewAllGLWidgets();
    void screenshot                         ( wxString      i_fileName    );
    void onTreeChange();
    void onMouseEvent                       ( wxMouseEvent&   evt );
    void onLoad                             ( wxCommandEvent& evt );
    long getCurrentListItem() const         { return m_currentListItem; }
	void createNewAnatomy					( DatasetType dataType );
    void updateSliders();
    
private:
    void initOpenGl();
    void initLayout();

    // File menu
    void onNewAnatomyByte                   ( wxCommandEvent& evt );
    void onNewAnatomyRGB                    ( wxCommandEvent& evt );
    void onReloadShaders                    ( wxCommandEvent& evt );
    void onSave                             ( wxCommandEvent& evt );
    void onSaveFibers                       ( wxCommandEvent& evt );
    void onSaveSurface                      ( wxCommandEvent& evt );
    void onSaveDataset                      ( wxCommandEvent& evt );
    void onQuit                             ( wxCommandEvent& evt );
    void onClose                            ( wxCloseEvent&   evt );
    // View menu
    void onMenuViewReset                    ( wxCommandEvent& evt );
    void onMenuViewLeft                     ( wxCommandEvent& evt );
    void onMenuViewRight                    ( wxCommandEvent& evt );
    void onMenuViewTop                      ( wxCommandEvent& evt );
    void onMenuViewBottom                   ( wxCommandEvent& evt );
    void onMenuViewFront                    ( wxCommandEvent& evt );
    void onMenuViewBack                     ( wxCommandEvent& evt );
    void onMenuViewCrosshair                ( wxCommandEvent& evt );
    void onMenuViewAxes                     ( wxCommandEvent& evt );
    void onMenuLock                         ( wxCommandEvent& evt );
    void onSceneLock                        ( wxCommandEvent& evt );
    void onRotateZ                          ( wxCommandEvent& evt );
    void onRotateY                          ( wxCommandEvent& evt );
    void onRotateX                          ( wxCommandEvent& evt );
    void onNavigateSagital                  ( wxCommandEvent& evt );
    void onNavigateAxial                    ( wxCommandEvent& evt );
    void onNavigateCoronal                  ( wxCommandEvent& evt );
    void onToggleShowAxial                  ( wxCommandEvent& evt );
    void onToggleShowCoronal                ( wxCommandEvent& evt );
    void onToggleShowSagittal               ( wxCommandEvent& evt );

    // Voi menu
    void onToggleSelectionObjects           ( wxCommandEvent& evt );
    void onNewSelectionBox                  ( wxCommandEvent& evt );
    void onNewSelectionEllipsoid            ( wxCommandEvent& evt );
    void onHideSelectionObjects             ( wxCommandEvent& evt );
    void onActivateSelectionObjects         ( wxCommandEvent& evt );
    void onUseMorph                         ( wxCommandEvent& evt );    
    // Fibers menu
    void onInvertFibers                     ( wxCommandEvent& evt );
    void onUseFakeTubes                     ( wxCommandEvent& evt );
    void onResetColor                       ( wxCommandEvent& evt );
    void onUseTransparency                  ( wxCommandEvent& evt );
    void onUseGeometryShader                ( wxCommandEvent& evt );
    // surface menu
    void onNewSplineSurface                 ( wxCommandEvent& evt );
    void onMoveBoundaryPointsLeft           ( wxCommandEvent& evt );
    void onMoveBoundaryPointsRight          ( wxCommandEvent& evt );
    void moveBoundaryPoints( int i_value);
    // Options menu
    void onToggleLighting                   ( wxCommandEvent& evt );
	void onClearToBlack                     ( wxCommandEvent& evt );
	void onSelectNormalPointer              ( wxCommandEvent& evt );
    void onSelectRuler                      ( wxCommandEvent& evt );
    void onRulerToolClear                   ( wxCommandEvent& evt );
    void onRulerToolAdd                     ( wxCommandEvent& evt );
    void onRulerToolDel                     ( wxCommandEvent& evt );
    void onToggleTextureFiltering           ( wxCommandEvent& evt );
    void onToggleBlendTexOnMesh             ( wxCommandEvent& evt );
    void onToggleFilterIso                  ( wxCommandEvent& evt );
    void onToggleColorMapLegend             ( wxCommandEvent& evt );    
    void onSetCMap0                         ( wxCommandEvent& evt );
    void onSetCMap1                         ( wxCommandEvent& evt );
    void onSetCMap2                         ( wxCommandEvent& evt );
    void onSetCMap3                         ( wxCommandEvent& evt );
    void onSetCMap4                         ( wxCommandEvent& evt );
    void onSetCMap5                         ( wxCommandEvent& evt );
    void onSetCMapNo                        ( wxCommandEvent& evt );
    void onToggleNormal                     ( wxCommandEvent& evt );
    void onToggleDrawVectors                ( wxCommandEvent& evt );
    void onToggleAlpha                      ( wxCommandEvent& evt );
    void onToggleDrawPointsMode             ( wxCommandEvent& evt );
    // Help menu
    void onAbout                            ( wxCommandEvent& evt );
    void onShortcuts                        ( wxCommandEvent& evt );
    void onScreenshot                       ( wxCommandEvent& evt );
	void onWarningsInformations				( wxCommandEvent& evt );
    void onSlizeMovieSag                    ( wxCommandEvent& evt );
    void onSlizeMovieCor                    ( wxCommandEvent& evt );
    void onSlizeMovieAxi                    ( wxCommandEvent& evt );

    // List widget event functions     
    void onActivateListItem                 ( wxListEvent&    evt );
    void onSelectListItem                   ( wxListEvent&    evt );
    void onActivateListItem2                ( wxListEvent&    evt );
    void onSelectListItem2                  ( wxListEvent&    evt );
    void onDeleteListItem2                  ( wxListEvent&    evt );
    void onDeselectListItem2                ( wxListEvent&    evt );

    // Tree widget event functions
    void onDeleteTreeItem                   ( wxTreeEvent&    evt );
    void onSelectTreeItem                   ( wxTreeEvent&    evt );
    void onRightClickTreeItem               ( wxTreeEvent&    evt );
    void onUnSelectTreeItem                 ( wxTreeEvent&    evt );
    void onActivateTreeItem                 ( wxTreeEvent&    evt );
    void onTreeLabelEdit                    ( wxTreeEvent&    evt );
    int  treeSelected                       ( wxTreeItemId    i_id  ); 
    
    // System functions
    void onSize                             ( wxSizeEvent&    evt );
    void doOnSize();
    void onGLEvent                          ( wxCommandEvent& evt );    
    void onSliderMoved                      ( wxCommandEvent& evt );
    void onKdTreeThreadFinished             ( wxCommandEvent& evt );
    void updateStatusBar();
    void updateMenus();
    void onTimerEvent                       ( wxTimerEvent&   evt );
    void setTimerSpeed();
    void createNewSelectionObject( ObjectType i_newSelectionObjectType );

	void onSelectDrawer                     ( wxCommandEvent& evt );
    void onSwitchDrawer                     ( wxCommandEvent& evt );
	void onToggleDrawRound                  ( wxCommandEvent& evt );
	void onToggleDraw3d                     ( wxCommandEvent& evt );
	void onSelectColorPicker                ( wxCommandEvent& evt );
	void onSelectStroke1                    ( wxCommandEvent& evt );
	void onSelectStroke2                    ( wxCommandEvent& evt );
	void onSelectStroke3                    ( wxCommandEvent& evt );
	void onSelectStroke4                    ( wxCommandEvent& evt );
	void onSelectStroke5                    ( wxCommandEvent& evt );
	void onSelectStroke7                    ( wxCommandEvent& evt );
	void onSelectStroke10                   ( wxCommandEvent& evt );
	void onSelectPen                        ( wxCommandEvent& evt );
	void onSelectEraser                     ( wxCommandEvent& evt );
                        
    // Utility
    void updateDrawerToolbar();

    
private:
    ToolBar             *m_pToolBar;
    MenuBar             *m_pMenuBar;       
    wxBoxSizer          *m_pCurrentSizer;
    SceneObject         *m_pCurrentSceneObject;
    SceneObject         *m_pLastSelectedSceneObject;
    long                m_currentListItem;
    long                m_lastSelectedListItem;

    wxBoxSizer          *m_pMainSizer;
    wxBoxSizer          *m_pListSizer;
    wxBoxSizer          *m_pObjectSizer;
    wxBoxSizer          *m_pLeftMainSizer;
    wxBoxSizer          *m_pNavSizer;

    wxString            m_lastPath;

    wxTimer             *m_pTimer;

public:
    PropertiesWindow    *m_pPropertiesWindow;
    MainCanvas          *m_pMainGL;
    MainCanvas          *m_pGL0;
    MainCanvas          *m_pGL1;
    MainCanvas          *m_pGL2;
//    MyListCtrl          *m_pListCtrl;
    ListCtrl            *m_pListCtrl2;
    MyTreeCtrl          *m_pTreeWidget;
    wxSlider            *m_pXSlider;
    wxSlider            *m_pYSlider;
    wxSlider            *m_pZSlider;
    DatasetHelper       *m_pDatasetHelper;
    wxTreeItemId        m_tRootId;
    wxTreeItemId        m_tPointId;
    wxTreeItemId        m_tSelectionObjectsId;

DECLARE_EVENT_TABLE()
};

/*
 * Defines for interface items and other events
 */
#define KDTREE_EVENT                                270

#define ID_GL_NAV_X                                 271
#define ID_GL_NAV_Y                                 272
#define ID_GL_NAV_Z                                 273
#define ID_GL_MAIN                                  290

//#define ID_LIST_CTRL                                291
#define ID_TREE_CTRL                                300

#define ID_X_SLIDER                                 301
#define ID_Y_SLIDER                                 302
#define ID_Z_SLIDER                                 303
 
#define TREE_CTRL_TOGGLE_ANDNOT                     351
#define TREE_CTRL_DELETE_BOX                        352
#define TREE_CTRL_TOGGLE_BOX_ACTIVE                 353
#define TREE_CTRL_TOGGLE_BOX_SHOW                   370

#endif /*MAINFRAME_H_*/
