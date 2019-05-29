#ifndef IVF_DOCUMENT_H
#define IVF_DOCUMENT_H

#include <afxwin.h>

class CInlineItem;
class CInlineQueue;
class CAsyncFileQueue;

class SoWWWAnchor;
class SoSwitch;
class SoInfo;

class CIvfAsyncFile;

#include <Inventor/actions/SoCallbackAction.h>

#include <Ivf/IvfBasic.h>
#include <Ivf/IvfComponent.h>

#include <IvfEnterScope.h>

#define IVF_MAX_URL_NAME_LENGTH     (1024)
#define MAX_SCRATCH_FILES              (4)

class IVF_DLL_API CIvfDocument {
  friend class CIvfComponent;

public:
  CIvfDocument(void);
  virtual ~CIvfDocument(void);

  void IvfSetFileHeader(LPCSTR header);

  virtual BOOL IvfOnNewDocument(void);
  virtual BOOL IvfOnOpenDocument(LPCTSTR path);
  virtual BOOL IvfOnSaveDocument(LPCTSTR path);
  virtual BOOL IvfOnOpenDocument(UINT file);
  virtual BOOL IvfOnSaveDocument(UINT file);
  virtual BOOL IvfOnOpenDocument(HANDLE file);
  virtual BOOL IvfOnSaveDocument(HANDLE file);
  virtual BOOL IvfOnOpenDocument(CArchive & archive);
  virtual BOOL IvfOnSaveDocument(CArchive & archive);
  BOOL IvfOnOpenDocumentMessage(LPCTSTR path);
  void IvfImportDocument(char * path);
 
  static enum IVFDOC_OPEN_ENUM {
    IVFDOC_OPEN_NORMAL, 
    IVFDOC_OPEN_LOAD, 
    IVFDOC_OPEN_IMPORT
  };

  IVFDOC_OPEN_ENUM m_eOpenType;

  void IvfInitDocument(void * createargs);
 
  virtual void IvfDeleteContents(void);
  virtual const char * IvfGetUrlName(void);
  virtual BOOL IvfIsEmbedded(void);
  virtual BOOL IvfIsTitleSet(void);
  virtual void  IvfUpdateTitle(void);
  virtual const char * IvfGetTitleString(void);

  virtual BOOL IvfIsInfoSet(void);
  virtual const char * IvfGetDocInfoString(void);

  virtual void OnFileViewInfo(void);

  virtual void IvfSetSceneGraph(SoSeparator * root);
  SoSeparator * IvfGetDocSceneGraph(void);

  SoCamera * IvfGetActiveCamera(int idx = -1);
  int IvfGetActiveCameraIX(void);
  SoGroup * IvfGetActiveCamParent(void);
  SbPList * IvfGetCameraList(void);

  virtual void setCameraType(SoType type);
   
  SoType getCameraType(void);

  SoCamera * IvfReplaceCamera(SoCamera * camera, int idx);

  virtual void IvfPrepareForSave(void);
  virtual void IvfPostSave(void);

  virtual void IvfFindAnchors(SoGroup * root);

  BOOL IvfIsOleSrvr(void);

  void IvfUpdateHeadlightHint(BOOL on);

  static SoSeparator * IvfReadAll(SoInput * in, CIvfDocument * ivfdocument);
  static BOOL IvfSaveSceneGraph(LPCTSTR name, SoSeparator * root, CIvfDocument * ivfdocument);
  static SoSeparator * IvfLoadSceneGraph(LPCTSTR name, CIvfDocument * ivfdocument);
  static BOOL IvfSaveSceneGraph(UINT file, SoSeparator * root, CIvfDocument * ivfdocument);
  static SoSeparator * IvfLoadSceneGraph(UINT file, CIvfDocument * ivfdocument);
  static BOOL IvfSaveSceneGraph(CArchive & archive, SoSeparator * root, CIvfDocument * ivfdocument);
  static SoSeparator * IvfLoadSceneGraph(CArchive & archive, CIvfDocument * ivfdocument);

  virtual void IvfDeleteSceneGraph(void);
  virtual void IvfSceneGraphChanged(void);
  virtual void IvfSceneGraphCompleted(void);
  virtual void IvfScrubInlines(void);
  virtual void IvfOnLocationChanged(const char * url);
  virtual void IvfOnUpdateViewpoints(CCmdUI * cmd);
  virtual void IvfRemoveViewpointMenu(CCmdUI * cmd);
  int IvfGetNumComponents(void);
  virtual BOOL IdleProcessInlines(void);

  static BOOL IvfIsInventorFile(LPCSTR filename);

  static enum IVF_FILETYPE {
    IVF_FILETYPE_IV, 
    IVF_FILETYPE_VRML,
    IVF_FILETYPE_VRML2
  };
 
  void IvfSetFileType(IVF_FILETYPE filetype);
  IVF_FILETYPE IvfGetFileType(void);

  virtual void IvfSetBgColor(const SbColor & color);
  virtual SbColor & IvfGetBgColor(void);

  virtual BOOL IvfPreTranslateMessage(MSG * msg);
  virtual CIvfAsyncFile * IvfCreateAsyncFile(void * data);
  virtual BOOL IvfOnOpenUrl(const char * url);

  void IvfRemoveAsyncFile(CIvfAsyncFile * file);
  void IvfAbortAllAsyncFiles(void);

  BOOL IvfIsAsyncFilesPending(void);

  int IvfIsInlinesPending(void);

  virtual void IvfEnableStatAccumulation(BOOL accumulation);
  virtual void IvfClearStatCounters(BOOL all = FALSE);

  ULONG m_nTriCount;
  ULONG m_nNodeCount;
  ULONG m_nLineCount;
  ULONG m_nPointCount;

  BOOL  m_bHasTouchSensors;
  static BOOL m_queueingOff;

  void SetBaseUrlPath(const char * path);
  const char * GetBaseUrlPath(void);
  static char m_StaticPath[IVF_MAX_URL_NAME_LENGTH];

protected:
  SoSeparator * m_pSceneRoot;

  virtual void IvfRemoveCameras(SoGroup * root);
  virtual void IvfRestoreCameras(SoGroup * root);
  virtual void IvfDeleteCameraList(void);
  virtual BOOL IvfOnLoadUrlMessage(CIvfAsyncFile * asyncfile);
  virtual void IvfSetupUrlFetchCallback(void);
  virtual void IvfRemoveUrlFetchCallback(void);
  virtual void IvfOnFetchUrl(const char * url);
  static void IvfStaticFetchUrlCB(const SbString & url, 
                                  void * data, 
                                  SoWWWAnchor * node);
  static void IvfStaticFetchInlineUrlCB(const SbString & url, 
                                        void * data,
                                        SoNode * node);
  static BOOL IvfStaticFetchTexture2UrlCB(const SbString & url, 
                                          void * data,
                                          void * node);
  BOOL IsPendingAsyncRequest(CIvfAsyncFile * asyncfile, int & status);
  void OpenLocalAsyncDoc(CIvfAsyncFile * asyncfile);

  void OnProcessPendingInline(WPARAM wparam, LPARAM lparam);
  void ProcessPendingTexture2(CInlineItem *, BOOL);

  void PostInlineLocation(SoNode *, char *, void *);
  char * MakeFullUrlPath(char *out, const char * in);

  BOOL IsOKFile(char * filename);
  BOOL MakeScratchName(char * new_path, char * filename);
  void CleanupScratchFile(char * filename);
 
  void CountPrimitives(void);

  SoSwitch * m_pCameraSwitch;
  SoGroup * m_pCameraSwitchParent;
  SoInfo * m_pCameraSwitchInfo;

  SoType  m_cameraType;

  SoGroup * m_pDefaultCameraGroup;
   
  SbPList * m_pCamList;

  SbPList * m_pCamParentList;
 
  int m_nActiveCameraIX;
 
  BOOL IVOpenError;
 
  SbPList * m_pInfoList;
 
  SoPathList * m_pAnchorList;
 
  SoInfo * m_pBackColorInfo;
  SoInfo * m_pHLInfo;

  SoGroup * m_pVPWindowSizePar; 
  SoInfo * m_pVPWindowSize;

  BOOL m_nEnableUrlFetch;
 
  BOOL m_nEnableUrlDisplay;
 
  BOOL m_nEnableViewpoints;

  char m_urlName[IVF_MAX_URL_NAME_LENGTH];

  IVF_FILETYPE m_fileType;
  char m_fileHeader[256];

  void AddInlineChild(CInlineItem *);
  void AddInlineChild(SoNode * node, char * filename, void * cb);
  CInlineQueue * m_pInlineQueue;
  CAsyncFileQueue * m_pAsyncQueue;
  char m_BasePath[IVF_MAX_URL_NAME_LENGTH];
  char m_OriginalPath[IVF_MAX_URL_NAME_LENGTH];
  BOOL m_bSceneGraphComplete;
  BOOL m_bNewGraphPrepped;
  BOOL m_bBasePathPreset;
  BOOL m_bContainsInlines;

  char * lcl_scratch_filename[MAX_SCRATCH_FILES];
  int n_scratch_files;
  static BOOL viewerSpeedHintSet;
  static BOOL autoClipOffHintSet;
  static BOOL titleHintSet;
  static BOOL anyLightsSet;

  BOOL m_bTitleHintSet;
  BOOL m_bDocInfoSet;
  BOOL m_bSaveAsBinary;
  BOOL m_bDefHeadlightVal;
  float motionSpeedHintVal;

  CIvfComponent::IvfViewerType viewerTypeHintSet;

  SbString * sceneTitle;
  SbString * docInfoString;

  void IvfScanForHints(SoNode * root);

  void DetermineViewerType(const SoInfo * info);
  void DetermineViewerSpeed(const SoInfo * info);
  void DetermineBackgroundColor(const SoInfo * info);
  void DetermineTitle(const SoInfo * info);

  static SoCallbackAction::Response InfoHintCB(void * closure, 
                                               SoCallbackAction * action,
                                               const SoNode * node);

  static SoCallbackAction::Response NavInfoCB(void * closure, 
                                              SoCallbackAction * action,
                                              const SoNode * node);

  static SoCallbackAction::Response WorldInfoCB(void * closure, 
                                                SoCallbackAction * action,
                                                const SoNode * node);

  static SoCallbackAction::Response WWWInlineCB(void * closure,
                                                SoCallbackAction * action,
                                                const SoNode * node);

  static SoCallbackAction::Response VRMLInlineCB(void * closure,
                                                 SoCallbackAction * action,
                                                 const SoNode * node);

  static SoCallbackAction::Response CheckForLightsCB(void * closure, 
                                                     SoCallbackAction * action,
                                                     const SoNode * node);


  static SoCallbackAction::Response CountNodesCB(void * closure, 
                                                 SoCallbackAction * action,
                                                 const SoNode * node);

  static void CountTrisCB(void * closure, 
                          SoCallbackAction * action,
                          const SoPrimitiveVertex * v1,
                          const SoPrimitiveVertex * v2,
                          const SoPrimitiveVertex * v3);
 
  static void CountLinesCB(void * closure, 
                           SoCallbackAction * action,
                           const SoPrimitiveVertex * v1,
                           const SoPrimitiveVertex * v2);

  static void CountPointsCB(void * closure, 
                            SoCallbackAction * action,
                            const SoPrimitiveVertex * v);
};

#include <IvfLeaveScope.h>

extern UINT WMIVF_PROCPENDINGINLINE;
extern UINT WMIVF_LOADURL;
 
#ifdef _DEBUG
void DumpSceneGraph(SoNode *scenegraph);
#endif // _DEBUG

#endif // !IVF_DOCUMENT_H
