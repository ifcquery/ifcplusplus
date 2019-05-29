// src\Inventor\Win\SoWinGLWidgetCommon.cpp.  Generated from SoGuiGLWidgetCommon.cpp.in by configure.

/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#define PRIVATE(obj) ((SoGuiGLWidgetP *)(obj->pimpl))

// *************************************************************************

/*!
  \class SoWinGLWidget SoWinGLWidget.h Inventor/Win/SoWinGLWidget.h
  \brief The SoWinGLWidget class manages OpenGL contexts.
  \ingroup components

  This is the basic, abstract component class which sets up an OpenGL
  canvas for its subclasses.

  Application programmers will normally not use this class directly,
  but rather through the interface of either its direct descendent;
  SoWinRenderArea, or through one of the "rapid application
  development"-style viewer subclasses.

  \if SOQT_DOC

  An important thing to know about embedding SoWinGLWidget derived
  components into other Win widgets is that you need to set up
  "focus proxying" to have events still be sent to the OpenGL canvas
  widget. This is true for both the SoWinRenderArea aswell as all
  the viewer components (like SoWinExaminerViewer,
  SoWinPlaneViewer etc).

  As an example, if you embed an SoWinExaminerViewer inside a QFrame
  like this:

  \code
  QMainWindow * toplevel = new QMainWindow;
  QFrame * frame = new QFrame(toplevel);
  SoWinExaminerViewer * viewer = new SoWinExaminerViewer(toplevel);
  \endcode

  ...events from the keyboard will not always automatically be
  forwarded to the viewer OpenGL canvas. This is the code you need to
  add in this case:

  \code
  toplevel->setFocusProxy(viewer->getWidget());
  \endcode

  \endif

  \if SOWIN_DOC

  For debugging purposes, SoWinGLWidget provides the
  OIV_FORCE_PIXEL_FORMAT environment variable to make it possible to
  override the pixel format chosen by the internal attribute match
  algorithm.

  \endif
*/

// *************************************************************************

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/Win/common/gl.h>
#include <Inventor/Win/SoWinGLWidget.h>
#include <Inventor/Win/SoWinGLWidgetP.h>

// *************************************************************************

// All shared documentation for functions with specific
// implementations in the individual toolkits.

/*!
  \fn SoWinGLWidget::SoWinGLWidget(HWND const parent, const char * const name, const SbBool embed, const int glmodes, const SbBool build)

  The constructor is protected, as this is an abstract class to only
  be used by it's subclasses.
*/

/*!
  \fn SoWinGLWidget::~SoWinGLWidget()

  Clean up all use of internal resources.

  The destructor is protected, as this is an abstract class to only be
  used by it's subclasses.
*/

/*!
  \fn void SoWinGLWidget::setBorder(const SbBool enable)

  Specify whether or not there should be a border framing the OpenGL
  canvas. The border will be 2 pixels wide.

  The default is to display the OpenGL canvas with no border.

  \sa isBorder()
*/

/*!
  \fn SbBool SoWinGLWidget::isBorder(void) const

  Returns whether or not there's a border framing the OpenGL canvas.

  \sa setBorder()
*/

/*!
  \fn void SoWinGLWidget::setOverlayRender(const SbBool onoff)

  Turn on or off the use of overlay planes.

  \sa isOverlayRender()
*/

/*!
  \fn SbBool SoWinGLWidget::isOverlayRender(void) const

  Returns a flag indicating whether or not overplay planes are
  currently used.

  \sa setOverlayRender()
*/

/*!
  \fn void SoWinGLWidget::setDoubleBuffer(const SbBool enable)

  Switch between single and double buffer mode for the OpenGL canvas.
  The default is to use a single buffer canvas.

  \sa isDoubleBuffer()
*/

/*!
  \fn SbBool SoWinGLWidget::isDoubleBuffer(void) const

  Returns the status of the buffer mode.

  \sa setDoubleBuffer()
*/

/*!
  \fn void SoWinGLWidget::setQuadBufferStereo(const SbBool enable)

  Enables or disables OpenGL quad buffer stereo.
*/

/*!
  \fn SbBool SoWinGLWidget::isQuadBufferStereo(void) const

  Returns \c TRUE if quad buffer stereo is enabled for this widget.
*/

/*!
  \fn void SoWinGLWidget::setAccumulationBuffer(const SbBool enable)

  Enables/disables the OpenGL accumulation buffer.

  For general information about accumulation buffers, confer with your
  OpenGL reference documentation.

  By default, the accumulation buffer will be attempted disabled.
  Whether or not it will \e actually be disabled depends on what
  OpenGL contexts are available on the system. It is perfectly
  possible that the only usable contexts with regard to the other
  settings (like double buffering and RGBA mode versus color index
  mode) causes the context selection to choose an OpenGL format that
  contains an accumulation buffer, even though it was not
  requested.

  Vice versa, one is not guaranteed to get accumulation buffers even
  when requested through this function, either because they are not
  available, or because they are only available in combination with
  other OpenGL context settings that are not wanted (like single
  buffers, non-accellerated rendering, etc).

  If you try to enable accumulation buffers by using this function,
  you should therefore in succession use the
  SoWinGLWidget::getAccumulationBuffer() to query whether or not an
  accumulation buffer could actually be set up.

  \sa SoWinRenderArea::setAntiAliasing()
*/

/*!
  \fn SbBool SoWinGLWidget::getAccumulationBuffer(void) const

  Returns whether the OpenGL accumulation buffer is enabled.

  The returned flag will indicate whether or not accumulation buffers
  are \e actually present, and not just parrot the input value to
  SoWinGLWidget::setAccumulationBuffer(). See documentation of that
  function for more information.
*/

/*!
  \fn void SoWinGLWidget::setStencilBuffer(const SbBool enable)

  Enables/disables the OpenGL stencil buffer.

  For general information about stencil buffers, confer with your
  OpenGL reference documentation.

  By default, the stencil buffer will be attempted disabled, but the
  same notes apply for stencil buffers as for accumulation
  buffers. For an explanation on the issue of stencil and accumulation
  buffer availability, see the function documentation of
  SoWinGLWidget::setAccumulationBuffer().
*/

/*!
  \fn SbBool SoWinGLWidget::getStencilBuffer(void) const

  Returns whether the OpenGL stencil buffer is enabled.

  The returned flag will indicate whether or not stencil buffers are
  \e actually present, in the same manner as for
  SoWinGLWidget::getAccumulationBuffer(). See documentation of
  SoWinGLWidget::setStencilBuffer().
*/

/*!
  \fn void SoWinGLWidget::setAlphaChannel(const SbBool enable)

  Enables/disables the alpha channel for the OpenGL context.
*/

/*!
  \fn SbBool SoWinGLWidget::getAlphaChannel(void) const

  Returns whether the alpha channel is enabled for the OpenGL context.
*/

/*!
  \fn void SoWinGLWidget::setDrawToFrontBufferEnable(const SbBool enable)

  If this is set to \c TRUE, rendering will happen in the front buffer
  even if the current rendering mode is double buffered.
*/

/*!
  \fn SbBool SoWinGLWidget::isDrawToFrontBufferEnable(void) const
  \sa setDrawToFrontBufferEnable()
*/

/*!
  \fn HWND SoWinGLWidget::buildWidget(HWND parent)

  This method builds the component contents in the given \a parent
  widget. For subclasses adding new user interface items, this method
  is typically overridden in the following manner:

  \code
  HWND MyOwnViewer::buildWidget(HWND parent)
  {
     HWND superw = <superclass>::buildWidget(parent);
     // [then move superw within MyOwnViewer framework and add own
     // user interface components]
  }
  \endcode
*/

/*!
  \fn virtual void SoWinGLWidget::redraw(void)

  This method is invoked when the GL buffer needs to be redrawn.
 */

/*!
  \var SbBool SoWinGLWidget::waitForExpose

  If this is \c TRUE, rendering should not be done yet. Upon the first
  expose event of a newly created OpenGL widget, this variable will
  be set to \c FALSE.
*/

/*!
  \var SbBool SoWinGLWidget::drawToFrontBuffer

  If this is \c TRUE, rendering will happen in the front buffer even
  if the current rendering mode is double buffered.
*/

/*!
  \fn HWND SoWinGLWidget::getNormalWidget(void) const

  \if SOQT_DOC
  For SoQt, this returns the same widget pointer as that of
  SoQtGLWidget::getGLWidget().
  \endif
*/

/*!
  \fn HWND SoWinGLWidget::getOverlayWidget(void) const

  Returns widget associated with overlay planes, or \c NULL if no
  overlay planes are available.

  \if SOQT_DOC
  For the Qt toolkit, overlay planes is a feature of the QGLWidget,
  and not seen at a separate widget entity. So this call will just
  return the same widget reference as the SoQt::getGLWidget() call (if
  overlay planes are supported).
  \endif
*/

/*!
  \fn void SoWinGLWidget::setGLSize(const SbVec2s size)
  Sets the size of the GL canvas.
*/

/*!
  \fn SbVec2s SoWinGLWidget::getGLSize(void) const

  Return the dimensions of the OpenGL canvas.
*/

/*!
  \fn float SoWinGLWidget::getGLAspectRatio(void) const

  Return the aspect ratio of the OpenGL canvas.
*/

/*!
  \fn void SoWinGLWidget::setGlxSize(const SbVec2s size)
  This function has been renamed to the more appropriate setGLSize.
  \sa setGLSize
*/

/*!
  \fn SbVec2s SoWinGLWidget::getGlxSize(void) const
  This function has been renamed to the more appropriate getGLSize.
  \sa getGLSize
*/

/*!
  \fn float SoWinGLWidget::getGlxAspectRatio(void) const
  This function has been renamed to the more appropriate getGLAspectRatio.
  \sa getGLAspectRatio
*/

/*!
  \fn HWND SoWinGLWidget::getGLWidget(void) const

  Returns a pointer to the toolkit-native GL widget.
*/

/*!
  \fn void SoWinGLWidget::widgetChanged(HWND w)

  This is the method which gets called whenever we change which OpenGL
  widget is used.

  Should be overridden in subclasses which directly or indirectly
  store the return value from the SoWinGLWidget::getGLWidget()
  method.

  \sa sizeChanged()
*/

/*!
  \fn void SoWinGLWidget::processEvent(MSG * anyevent)

  Any events from the native window system that goes to the OpenGL
  canvas gets piped through this method.

  It is overridden in the subclasses to catch user interaction with
  the render canvas in the viewers, aswell as forwarding relevant
  events to the scenegraph.
*/

/*!
  \fn void SoWinGLWidget::glLockNormal(void)

  This method calls make-current on the correct context and ups the
  lock level.
*/

/*!
  \fn void SoWinGLWidget::glUnlockNormal(void)

  This method drops the lock level.
*/

/*!
  \fn void SoWinGLWidget::glSwapBuffers(void)
  Swap back buffer to front and vice versa.
*/

/*!
  \fn void SoWinGLWidget::glFlushBuffer(void)

  Flush the current GL buffer. Simply calls glFlush().
*/

/*!
  \fn void SoWinGLWidget::glLockOverlay(void)

  This method calls make-current on the correct context and ups the
  lock level.
*/

/*!
  \fn void SoWinGLWidget::glUnlockOverlay(void)

  This method drops the lock level.
*/

/*!
  \fn unsigned long SoWinGLWidget::getOverlayTransparentPixel(void)
  Returns the overlay transparent pixel.
*/

/*!
  \fn SbBool SoWinGLWidget::isRGBMode(void)

  Returns \c TRUE if the normal GL context is in RGBA mode.
  Return \c FALSE if color index mode is used.
*/

/*!
  \fn void SoWinGLWidget::redrawOverlay(void)

  Renders the overlay scene graph. Default method is empty. Subclasses
  should override this method.
*/

/*!
  \fn SbBool SoWinGLWidget::hasNormalGLArea(void) const 

  Will return \c TRUE if a normal GL drawing area exists.
*/

/*!
  \fn SbBool SoWinGLWidget::hasOverlayGLArea(void) const 

  Will return \c TRUE if an overlay GL drawing area exists.
*/

/*!
  \fn void SoWinGLWidget::initGraphic(void)

  Will be called when GL widget should initialize graphic, after
  the widget has been created. Default method enabled GL_DEPTH_TEST.
*/

/*!
  \fn void SoWinGLWidget::initOverlayGraphic(void)

  Will be called after the overlay widget has been created, and subclasses
  should override this to initialize overlay stuff.

  Default method does nothing.
*/

/*!
  \fn SbBool SoWinGLWidget::glScheduleRedraw(void)

  Will be called whenever scene graph needs to be redrawn.  If this
  method return \c FALSE, redraw() will be called immediately.

  Default method simply returns \c FALSE. Override this method to
  schedule a redraw and return \c TRUE if you're trying to do The
  Right Thing.
*/

/*!
  \fn void SoWinGLWidget::setStealFocus(SbBool enable)

  Sets whether the GL widget should steal keyboard focus when the
  mouse is over the GL view.

  Default is \c TRUE.

  \sa isStealFocus
  \since SoWin 1.3.0
*/

/*!
  \fn SbBool SoWinGLWidget::isStealFocus(void) const

  Returns whether focus stealing policy is on (\c TRUE) or off (\c
  FALSE).

  Default is \c TRUE.

  \sa setStealFocus
  \since SoWin 1.3.0
*/


/*! 
  \fn void SoWinGLWidget::setSampleBuffers(const int numsamples)
  
  Set the number of samples use when enabling multisample buffer contexts.
  Multisampling will be enabled when numsamples > 1.
  
*/

/*! 
  \fn int SoWinGLWidget::getSampleBuffers(void) const
  
  Returns the number of samples used for multisampling. Returns 1 if
  multisampling is disabled.
*/

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
void
SoWinGLWidget::setStealFocus(SbBool enable)
{
  PRIVATE(this)->stealFocus = enable;
}

// Documented in common/SoGuiGLWidgetCommon.cpp.in.
SbBool
SoWinGLWidget::isStealFocus(void) const
{
  return PRIVATE(this)->stealFocus;
}

// *************************************************************************

/*!
  This function is provided as a convenience for the application
  programmer to help with acquiring the OpenGL implementation limits
  for rendering points.

  For robust application code, one needs to consider the range limits
  when setting the SoDrawStyle::pointSize field.
*/
void
SoWinGLWidget::getPointSizeLimits(SbVec2f & range, float & granularity)
{
  this->glLockNormal();

  GLfloat vals[2];
  glGetFloatv(GL_POINT_SIZE_RANGE, vals);

  // Matthias Koenig reported on coin-discuss that the OpenGL
  // implementation on SGI Onyx 2 InfiniteReality returns 0 for the
  // lowest pointsize, but it will still set the return value of
  // glGetError() to GL_INVALID_VALUE if this size is attempted
  // used. So the boundary range fix in the next line of code is a
  // workaround for that OpenGL implementation bug.
  //
  // 0.0f and lower values are explicitly disallowed, according to
  // the OpenGL 1.3 specification, Chapter 3.3.

  if (vals[0] <= 0.0f) { vals[0] = SoWinMin(1.0f, vals[1]); }

  range.setValue(vals[0], vals[1]);

  GLfloat gran[1];
  glGetFloatv(GL_POINT_SIZE_GRANULARITY, gran);
  granularity = gran[0];

  this->glUnlockNormal();
}

/*!
  This function is provided as a convenience for the application
  programmer to help with acquiring the OpenGL implementation limits
  for rendering lines.

  For robust application code, one needs to consider the range limits
  when setting the SoDrawStyle::lineWidth field.
*/
void
SoWinGLWidget::getLineWidthLimits(SbVec2f & range, float & granularity)
{
  this->glLockNormal();

  GLfloat vals[2];
  glGetFloatv(GL_LINE_WIDTH_RANGE, vals);

  // Matthias Koenig reported on coin-discuss that the OpenGL
  // implementation on SGI Onyx 2 InfiniteReality returns 0 for the
  // lowest linewidth, but it will still set the return value of
  // glGetError() to GL_INVALID_VALUE if this size is attempted
  // used. This is a workaround for what looks like an OpenGL bug.

  if (vals[0] <= 0.0f) { vals[0] = SoWinMin(1.0f, vals[1]); }

  range.setValue(vals[0], vals[1]);

  GLfloat gran[1];
  glGetFloatv(GL_LINE_WIDTH_GRANULARITY, gran);
  granularity = gran[0];

  this->glUnlockNormal();
}

// FIXME: other implementation specifics to check are
//
//  * maximum stack depths (attribute, modelview matrix, name,
//    projection matrix, texture matrix)
//
//  * max display list nesting
//
//  * max nr of clipplanes
//
//  * max nr of light sources
//
//  * max 3D texture size (needs specific extension?)
//
// 20020802 mortene.

/*!
  Sets whether OpenGL stereo buffers (quad buffer stereo) should be
  used.
*/
void
SoWinGLWidget::setStereoBuffer(SbBool flag)
{
  // FIXME: is this really correct? 20011012 mortene.
  this->setQuadBufferStereo(flag);
}

/*!
  Returns whether OpenGL stereo buffers are being used.
*/
SbBool
SoWinGLWidget::isStereoBuffer(void) const
{
  // FIXME: is this really correct? 20011012 mortene.
  return this->isQuadBufferStereo();
}

// *************************************************************************

#ifndef DOXYGEN_SKIP_THIS

// Remaining code is for the SoGuiGLWidgetP "private implementation"
// class.


SoGuiGLWidgetP::SoGuiGLWidgetP(SoWinGLWidget * publ)
{
  this->pub = publ;
  this->stealFocus = TRUE;
}

SoGuiGLWidgetP::~SoGuiGLWidgetP()
{
}

// Just forward from a static function, so we can find out from
// anywhere within the library -- without exposing the API for the app
// programmer.
SbBool
SoGuiGLWidgetP::isDirectRendering(SoWinGLWidget * w)
{
  return ((SoGuiGLWidgetP *)(w->pimpl))->isDirectRendering();
}

#undef PRIVATE
#endif // DOXYGEN_SKIP_THIS

// *************************************************************************
