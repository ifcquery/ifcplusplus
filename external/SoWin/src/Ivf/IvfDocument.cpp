#include <afxwin.h>
#include <assert.h>

#include <Inventor/SbColor.h>

#include <Ivf/IvfDocument.h>

BOOL CIvfDocument::m_queueingOff = FALSE;
char CIvfDocument::m_StaticPath[IVF_MAX_URL_NAME_LENGTH] = "";

// *************************************************************************

CIvfDocument::CIvfDocument(void)
{
}

CIvfDocument::~CIvfDocument(void)
{
}

BOOL
CIvfDocument::IvfIsInventorFile(LPCSTR filename)
{
  return FALSE;
}

void
CIvfDocument::IvfSetFileType(IVF_FILETYPE filetype)
{
}

CIvfDocument::IVF_FILETYPE
CIvfDocument::IvfGetFileType(void)
{
  return IVF_FILETYPE_IV;
}

void
CIvfDocument::IvfSetupUrlFetchCallback(void) // protected
{
}

BOOL
CIvfDocument::IvfOnNewDocument(void) // virtual
{
  return FALSE;
}

BOOL
CIvfDocument::IvfOnOpenDocument(HANDLE file) // virtual
{
  return FALSE;
}

BOOL
CIvfDocument::IvfOnSaveDocument(HANDLE file) // virtual
{
  return FALSE;
}

BOOL
CIvfDocument::IvfOnOpenDocumentMessage(LPCTSTR path)
{
  return FALSE;
}

BOOL
CIvfDocument::IvfPreTranslateMessage(MSG * msg) // virtual
{
  return FALSE;
}

void
CIvfDocument::IvfSetBgColor(const SbColor & color) // virtual
{
}

SbColor &
CIvfDocument::IvfGetBgColor(void) // virtual
{
  static SbColor col = SbColor(0.0f, 0.0f, 0.0f);
  return col;
}

void
CIvfDocument::IvfSetSceneGraph(SoSeparator * root) // virtual
{
}

SoSeparator *
CIvfDocument::IvfGetDocSceneGraph(void)
{
  return NULL;
}

// *************************************************************************
// the rest will just be stubbed for now

void
CIvfDocument::IvfSetFileHeader(LPCSTR header)
{
}

BOOL
CIvfDocument::IvfOnOpenDocument(LPCTSTR path) // virtual
{
  return FALSE;
}

BOOL
CIvfDocument::IvfOnSaveDocument(LPCTSTR path) // virtual
{
  return FALSE;
}

BOOL
CIvfDocument::IvfOnOpenDocument(UINT file) // virtual
{
  return FALSE;
}

BOOL
CIvfDocument::IvfOnSaveDocument(UINT file) // virtual
{
  return FALSE;
}

BOOL
CIvfDocument::IvfOnOpenDocument(CArchive & archive) // virtual
{
  return FALSE;
}

BOOL
CIvfDocument::IvfOnSaveDocument(CArchive & archive) // virtual
{
  return FALSE;
}

void
CIvfDocument::IvfImportDocument(char * path)
{
}
 
void
CIvfDocument::IvfInitDocument(void * createargs)
{
}
 
void
CIvfDocument::IvfDeleteContents(void) // virtual
{
}

const char *
CIvfDocument::IvfGetUrlName(void) // virtual
{
  return NULL;
}

BOOL
CIvfDocument::IvfIsEmbedded(void) // virtual
{
  return FALSE;
}

BOOL
CIvfDocument::IvfIsTitleSet(void) // virtual
{
  return FALSE;
}

void
CIvfDocument::IvfUpdateTitle(void) // virtual
{
}

const char *
CIvfDocument::IvfGetTitleString(void) // virtual
{
  return NULL;
}

BOOL
CIvfDocument::IvfIsInfoSet(void) // virtual
{
  return FALSE;
}

const char *
CIvfDocument::IvfGetDocInfoString(void) // virtual
{
  return NULL;
}

void
CIvfDocument::OnFileViewInfo(void) // virtual
{
}

SoCamera *
CIvfDocument::IvfGetActiveCamera(int idx)
{
  return NULL;
}

int
CIvfDocument::IvfGetActiveCameraIX(void)
{
  return -1;
}

SoGroup *
CIvfDocument::IvfGetActiveCamParent(void)
{
  return NULL;
}

SbPList *
CIvfDocument::IvfGetCameraList(void)
{
  return NULL;
}

void
CIvfDocument::setCameraType(SoType type) // virtual
{
}
   
SoType
CIvfDocument::getCameraType(void)
{
  return SoType::badType();
}

SoCamera *
CIvfDocument::IvfReplaceCamera(SoCamera * camera, int idx)
{
  return NULL;
}

void
CIvfDocument::IvfPrepareForSave(void) // virtual
{
}

void
CIvfDocument::IvfPostSave(void) // virtual
{
}

void
CIvfDocument::IvfFindAnchors(SoGroup * root) // virtual
{
}

BOOL
CIvfDocument::IvfIsOleSrvr(void)
{
  return FALSE;
}

void
CIvfDocument::IvfUpdateHeadlightHint(BOOL on)
{
}

SoSeparator *
CIvfDocument::IvfReadAll(SoInput * in, CIvfDocument * ivfdocument) // static
{
  return NULL;
}

BOOL
CIvfDocument::IvfSaveSceneGraph(LPCTSTR name, SoSeparator * root, CIvfDocument * ivfdocument) // static
{
  return FALSE;
}

SoSeparator *
CIvfDocument::IvfLoadSceneGraph(LPCTSTR name, CIvfDocument * ivfdocument) // static
{
  return NULL;
}

BOOL
CIvfDocument::IvfSaveSceneGraph(UINT file, SoSeparator * root, CIvfDocument * ivfdocument) // static
{
  return FALSE;
}

SoSeparator *
CIvfDocument::IvfLoadSceneGraph(UINT file, CIvfDocument * ivfdocument) // static
{
  return NULL;
}

BOOL
CIvfDocument::IvfSaveSceneGraph(CArchive & archive, SoSeparator * root, CIvfDocument * ivfdocument) // static
{
return FALSE;
}

SoSeparator *
CIvfDocument::IvfLoadSceneGraph(CArchive & archive, CIvfDocument * ivfdocument) // static
{
  return NULL;
}

void
CIvfDocument::IvfDeleteSceneGraph(void) // virtual
{
}

void
CIvfDocument::IvfSceneGraphChanged(void) // virtual
{
}

void
CIvfDocument::IvfSceneGraphCompleted(void) // virtual
{
}

void
CIvfDocument::IvfScrubInlines(void) // virtual
{
}

void
CIvfDocument::IvfOnLocationChanged(const char * url) // virtual
{
}

void
CIvfDocument::IvfOnUpdateViewpoints(CCmdUI * cmd) // virtual
{
}

void
CIvfDocument::IvfRemoveViewpointMenu(CCmdUI * cmd) // virtual
{
}

int
CIvfDocument::IvfGetNumComponents(void)
{
  return -1;
}

BOOL
CIvfDocument::IdleProcessInlines(void) // virtual
{
  return FALSE;
}

CIvfAsyncFile *
CIvfDocument::IvfCreateAsyncFile(void * data) // virtual
{
  return NULL;
}

BOOL
CIvfDocument::IvfOnOpenUrl(const char * url) // virtual
{
  return FALSE;
}

void
CIvfDocument::IvfRemoveAsyncFile(CIvfAsyncFile * file)
{
}

void
CIvfDocument::IvfAbortAllAsyncFiles(void)
{
}

BOOL
CIvfDocument::IvfIsAsyncFilesPending(void)
{
  return FALSE;
}

int
CIvfDocument::IvfIsInlinesPending(void)
{
  return -1;
}

void
CIvfDocument::IvfEnableStatAccumulation(BOOL accumulation) // virtual
{
}

void
CIvfDocument::IvfClearStatCounters(BOOL all) // virtual
{
}

void
CIvfDocument::SetBaseUrlPath(const char * path)
{
}

const char *
CIvfDocument::GetBaseUrlPath(void)
{
  return NULL;
}

void
CIvfDocument::IvfRemoveCameras(SoGroup * root) // virtual
{
}

void
CIvfDocument::IvfRestoreCameras(SoGroup * root) // virtual
{
}

void
CIvfDocument::IvfDeleteCameraList(void) // virtual
{
}

BOOL
CIvfDocument::IvfOnLoadUrlMessage(CIvfAsyncFile * asyncfile) // virtual
{
  return FALSE;
}

void
CIvfDocument::IvfRemoveUrlFetchCallback(void) // virtual
{
}

void
CIvfDocument::IvfOnFetchUrl(const char * url) // virtual
{
}

void
CIvfDocument::IvfStaticFetchUrlCB(const SbString & url, void * data, SoWWWAnchor * node) // static
{
}

void
CIvfDocument::IvfStaticFetchInlineUrlCB(const SbString & url, void * data, SoNode * node) // static
{
}

BOOL
CIvfDocument::IvfStaticFetchTexture2UrlCB(const SbString & url, void * data, void * node) // static
{
  return FALSE;
}

BOOL
CIvfDocument::IsPendingAsyncRequest(CIvfAsyncFile * asyncfile, int & status)
{
  return FALSE;
}

void
CIvfDocument::OpenLocalAsyncDoc(CIvfAsyncFile * asyncfile)
{
}

void
CIvfDocument::OnProcessPendingInline(WPARAM wparam, LPARAM lparam)
{
}

void
CIvfDocument::ProcessPendingTexture2(CInlineItem *, BOOL)
{
}

void
CIvfDocument::PostInlineLocation(SoNode *, char *, void *)
{
}

char *
CIvfDocument::MakeFullUrlPath(char *out, const char * in)
{
  return NULL;
}

BOOL
CIvfDocument::IsOKFile(char * filename)
{
  return FALSE;
}

BOOL
CIvfDocument::MakeScratchName(char * new_path, char * filename)
{
  return FALSE;
}

void
CIvfDocument::CleanupScratchFile(char * filename)
{
}
 
void
CIvfDocument::CountPrimitives(void)
{
}

void
CIvfDocument::AddInlineChild(CInlineItem *)
{
}

void
CIvfDocument::AddInlineChild(SoNode * node, char * filename, void * cb)
{
}

void
CIvfDocument::IvfScanForHints(SoNode * root)
{
}

void
CIvfDocument::DetermineViewerType(const SoInfo * info)
{
}

void
CIvfDocument::DetermineViewerSpeed(const SoInfo * info)
{
}

void
CIvfDocument::DetermineBackgroundColor(const SoInfo * info)
{
}

void
CIvfDocument::DetermineTitle(const SoInfo * info)
{
}

SoCallbackAction::Response
CIvfDocument::InfoHintCB(void * closure, SoCallbackAction * action, const SoNode * node) // static
{
  return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response
CIvfDocument::NavInfoCB(void * closure, SoCallbackAction * action, const SoNode * node) // static
{
  return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response
CIvfDocument::WorldInfoCB(void * closure, SoCallbackAction * action, const SoNode * node) // static
{
  return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response
CIvfDocument::WWWInlineCB(void * closure, SoCallbackAction * action, const SoNode * node) // static
{
  return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response
CIvfDocument::VRMLInlineCB(void * closure, SoCallbackAction * action, const SoNode * node) // static
{
  return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response
CIvfDocument::CheckForLightsCB(void * closure, SoCallbackAction * action, const SoNode * node) // static
{
  return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response
CIvfDocument::CountNodesCB(void * closure, SoCallbackAction * action, const SoNode * node) // static
{
  return SoCallbackAction::CONTINUE;
}

void
CIvfDocument::CountTrisCB(void * closure, SoCallbackAction * action, const SoPrimitiveVertex * v1, const SoPrimitiveVertex * v2, const SoPrimitiveVertex * v3) // static
{
}
 
void
CIvfDocument::CountLinesCB(void * closure, SoCallbackAction * action, const SoPrimitiveVertex * v1, const SoPrimitiveVertex * v2) // static
{
}

void
CIvfDocument::CountPointsCB(void * closure, SoCallbackAction * action, const SoPrimitiveVertex * v) // static
{
}

// *************************************************************************
