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

#include <ctype.h> // toupper()

#include <Inventor/misc/SoBasic.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/errors/SoDebugError.h>

#include <sowindefs.h>
#include <Inventor/Win/devices/SoWinKeyboard.h>
#include <Inventor/Win/devices/SoGuiKeyboardP.h>
#include <Inventor/Win/devices/SoWinDeviceP.h>
#include <Inventor/Win/SoWinBasic.h>
#include <winuser.h> // VK_ defines

#define PRIVATE(obj) ((obj)->pimpl)

// *************************************************************************

#define VK_OEM_SCROLL 0x91
#define VK_OEM_1 0xBA
#define VK_OEM_PLUS 0xBB
#define VK_OEM_COMMA 0xBC
#define VK_OEM_MINUS 0xBD
#define VK_OEM_PERIOD 0xBE
#define VK_OEM_2 0xBF
#define VK_OEM_3 0xC0
#define VK_OEM_4 0xDB
#define VK_OEM_5 0xDC
#define VK_OEM_6 0xDD
#define VK_OEM_7 0xDE
#define VK_OEM_8 0xDF
#define VK_ICO_F17 0xE0
#define VK_ICO_F18 0xE1
#define VK_OEM102 0xE2
#define VK_ICO_HELP 0xE3
#define VK_ICO_00 0xE4
#define VK_ICO_CLEAR 0xE6
#define VK_OEM_RESET 0xE9
#define VK_OEM_JUMP 0xEA
#define VK_OEM_PA1 0xEB
#define VK_OEM_PA2 0xEC
#define VK_OEM_PA3 0xED
#define VK_OEM_WSCTRL 0xEE
#define VK_OEM_CUSEL 0xEF
#define VK_OEM_ATTN 0xF0
#define VK_OEM_FINNISH 0xF1
#define VK_OEM_COPY 0xF2
#define VK_OEM_AUTO 0xF3
#define VK_OEM_ENLW 0xF4
#define VK_OEM_BACKTAB 0xF5
#define VK_ATTN 0xF6
#define VK_CRSEL 0xF7
#define VK_EXSEL 0xF8
#define VK_EREOF 0xF9
#define VK_PLAY 0xFA
#define VK_ZOOM 0xFB
#define VK_NONAME 0xFC
#define VK_PA1 0xFD
#define VK_OEM_CLEAR 0xFE

#define VK_UNKNOWN 0xFF

// *************************************************************************

struct key1map {
  int from;                // Win val
  SoKeyboardEvent::Key to; // So val
};

static struct key1map WinToSoMapping[] = {
  {VK_ESCAPE, SoKeyboardEvent::ESCAPE},
  {VK_TAB, SoKeyboardEvent::TAB},
  {VK_BACK, SoKeyboardEvent::BACKSPACE},
  {VK_RETURN, SoKeyboardEvent::RETURN},
//{VK_RETURN, SoKeyboardEvent::ENTER},
  {VK_INSERT, SoKeyboardEvent::INSERT},
  {VK_DELETE, SoKeyboardEvent::KEY_DELETE},
  {VK_PAUSE, SoKeyboardEvent::PAUSE},
  {VK_SNAPSHOT, SoKeyboardEvent::PRINT},
  {VK_HOME, SoKeyboardEvent::HOME},
  {VK_END, SoKeyboardEvent::END},
  {VK_LEFT, SoKeyboardEvent::LEFT_ARROW},
  {VK_UP, SoKeyboardEvent::UP_ARROW},
  {VK_RIGHT, SoKeyboardEvent::RIGHT_ARROW},
  {VK_DOWN, SoKeyboardEvent::DOWN_ARROW},
//{VK_PRIOR, SoKeyboardEvent::PRIOR},
  {VK_PRIOR, SoKeyboardEvent::PAGE_UP},
//{VK_NEXT, SoKeyboardEvent::NEXT},
  {VK_NEXT, SoKeyboardEvent::PAGE_DOWN},
  {VK_SHIFT, SoKeyboardEvent::LEFT_SHIFT},
  {VK_CONTROL, SoKeyboardEvent::LEFT_CONTROL}, // ANY CTRL
  {VK_MENU, SoKeyboardEvent::LEFT_ALT}, // ANY ALT
  {VK_CAPITAL, SoKeyboardEvent::CAPS_LOCK},
  {VK_NUMLOCK, SoKeyboardEvent::NUM_LOCK},
  {VK_OEM_SCROLL, SoKeyboardEvent::SCROLL_LOCK},
  {VK_F1, SoKeyboardEvent::F1},
  {VK_F2, SoKeyboardEvent::F2},
  {VK_F3, SoKeyboardEvent::F3},
  {VK_F4, SoKeyboardEvent::F4},
  {VK_F5, SoKeyboardEvent::F5},
  {VK_F6, SoKeyboardEvent::F6},
  {VK_F7, SoKeyboardEvent::F7},
  {VK_F8, SoKeyboardEvent::F8},
  {VK_F9, SoKeyboardEvent::F9},
  {VK_F10, SoKeyboardEvent::F10},
  {VK_F11, SoKeyboardEvent::F11},
  {VK_F12, SoKeyboardEvent::F12},
  {VK_SPACE, SoKeyboardEvent::SPACE},
  {VK_OEM_COMMA, SoKeyboardEvent::COMMA},
  {VK_OEM_MINUS, SoKeyboardEvent::MINUS},
  {VK_OEM_PLUS, SoKeyboardEvent::EQUAL},
  {VK_OEM_PERIOD, SoKeyboardEvent::PERIOD},
  {VK_OEM_1, SoKeyboardEvent::SEMICOLON},
  {VK_OEM_2, SoKeyboardEvent::SLASH},
  {VK_OEM_3, SoKeyboardEvent::APOSTROPHE},
  {VK_OEM_4, SoKeyboardEvent::BRACKETLEFT},
  {VK_OEM_5, SoKeyboardEvent::BACKSLASH},
  {VK_OEM_6, SoKeyboardEvent::BRACKETRIGHT},
  {'0', SoKeyboardEvent::NUMBER_0},
  {'1', SoKeyboardEvent::NUMBER_1},
  {'2', SoKeyboardEvent::NUMBER_2},
  {'3', SoKeyboardEvent::NUMBER_3},
  {'4', SoKeyboardEvent::NUMBER_4},
  {'5', SoKeyboardEvent::NUMBER_5},
  {'6', SoKeyboardEvent::NUMBER_6},
  {'7', SoKeyboardEvent::NUMBER_7},
  {'8', SoKeyboardEvent::NUMBER_8},
  {'9', SoKeyboardEvent::NUMBER_9},
  {VK_SUBTRACT, SoKeyboardEvent::PAD_SUBTRACT},
  {VK_ADD, SoKeyboardEvent::PAD_ADD},
  {VK_DIVIDE, SoKeyboardEvent::PAD_DIVIDE},
  {VK_MULTIPLY, SoKeyboardEvent::PAD_MULTIPLY},
  {VK_NUMPAD0, SoKeyboardEvent::PAD_0},
  {VK_NUMPAD1, SoKeyboardEvent::PAD_1},
  {VK_NUMPAD2, SoKeyboardEvent::PAD_2},
  {VK_NUMPAD3, SoKeyboardEvent::PAD_3},
  {VK_NUMPAD4, SoKeyboardEvent::PAD_4},
  {VK_NUMPAD5, SoKeyboardEvent::PAD_5},
  {VK_NUMPAD6, SoKeyboardEvent::PAD_6},
  {VK_NUMPAD7, SoKeyboardEvent::PAD_7},
  {VK_NUMPAD8, SoKeyboardEvent::PAD_8},
  {VK_NUMPAD9, SoKeyboardEvent::PAD_9},
  {'A', SoKeyboardEvent::A},
  {'B', SoKeyboardEvent::B},
  {'C', SoKeyboardEvent::C},
  {'D', SoKeyboardEvent::D},
  {'E', SoKeyboardEvent::E},
  {'F', SoKeyboardEvent::F},
  {'G', SoKeyboardEvent::G},
  {'H', SoKeyboardEvent::H},
  {'I', SoKeyboardEvent::I},
  {'J', SoKeyboardEvent::J},
  {'K', SoKeyboardEvent::K},
  {'L', SoKeyboardEvent::L},
  {'M', SoKeyboardEvent::M},
  {'N', SoKeyboardEvent::N},
  {'O', SoKeyboardEvent::O},
  {'P', SoKeyboardEvent::P},
  {'Q', SoKeyboardEvent::Q},
  {'R', SoKeyboardEvent::R},
  {'S', SoKeyboardEvent::S},
  {'T', SoKeyboardEvent::T},
  {'U', SoKeyboardEvent::U},
  {'V', SoKeyboardEvent::V},
  {'W', SoKeyboardEvent::W},
  {'X', SoKeyboardEvent::X},
  {'Y', SoKeyboardEvent::Y},
  {'Z', SoKeyboardEvent::Z},
//{Key_Backtab, SoKeyboardEvent::}, // FIXME  
//{Key_F13, SoKeyboardEvent::}, // FIXME
//{Key_F14, SoKeyboardEvent::}, // FIXME
//{Key_F15, SoKeyboardEvent::}, // FIXME
//{Key_F16, SoKeyboardEvent::}, // FIXME
//{Key_F17, SoKeyboardEvent::}, // FIXME
//{Key_F18, SoKeyboardEvent::}, // FIXME
//{Key_F19, SoKeyboardEvent::}, // FIXME
//{Key_F20, SoKeyboardEvent::}, // FIXME
//{Key_F21, SoKeyboardEvent::}, // FIXME
//{Key_F22, SoKeyboardEvent::}, // FIXME
//{Key_F23, SoKeyboardEvent::}, // FIXME
//{Key_F24, SoKeyboardEvent::}, // FIXME
//{Key_F25, SoKeyboardEvent::}, // FIXME
//{Key_F26, SoKeyboardEvent::}, // FIXME
//{Key_F27, SoKeyboardEvent::}, // FIXME
//{Key_F28, SoKeyboardEvent::}, // FIXME
//{Key_F29, SoKeyboardEvent::}, // FIXME
//{Key_F30, SoKeyboardEvent::}, // FIXME
//{Key_F31, SoKeyboardEvent::}, // FIXME
//{Key_F32, SoKeyboardEvent::}, // FIXME
//{Key_F33, SoKeyboardEvent::}, // FIXME
//{Key_F34, SoKeyboardEvent::}, // FIXME
//{Key_F35, SoKeyboardEvent::}, // FIXME
//{Key_Super, SoKeyboardEvent::}, // FIXME
//{Key_Super, SoKeyboardEvent::}, // FIXME
//{Key_Menu, SoKeyboardEvent::}, // FIXME
//{Key_Exclam, SoKeyboardEvent::}, // FIXME
//{Key_QuoteDbl, SoKeyboardEvent::}, // FIXME
//{Key_NumberSign, SoKeyboardEvent::}, // FIXME
//{Key_Dollar, SoKeyboardEvent::}, // FIXME
//{Key_Percent, SoKeyboardEvent::}, // FIXME
//{Key_Ampersand, SoKeyboardEvent::}, // FIXME
//{Key_ParenLeft, SoKeyboardEvent::}, // FIXME
//{Key_ParenRight, SoKeyboardEvent::}, // FIXME
//{Key_Asterisk, SoKeyboardEvent::}, // FIXME
//{Key_Plus, SoKeyboardEvent::}, // FIXME
//{Key_Colon, SoKeyboardEvent::}, // FIXME
//{Key_Less, SoKeyboardEvent::}, // FIXME
//{Key_Greater, SoKeyboardEvent::}, // FIXME
//{Key_Question, SoKeyboardEvent::}, // FIXME
//{Key_At, SoKeyboardEvent::}, // FIXME  
//{Key_AsciiCircum, SoKeyboardEvent::}, // FIXME
//{Key_Underscore, SoKeyboardEvent::}, // FIXME
//{Key_QuoteLeft, SoKeyboardEvent::}, // FIXME
//{Key_BraceLeft, SoKeyboardEvent::}, // FIXME
//{Key_Bar, SoKeyboardEvent::}, // FIXME
//{Key_BraceRight, SoKeyboardEvent::}, // FIXME
//Key_AsciiTilde, SoKeyboardEvent::GRAVE}, // FIXME: is this correct?

// Latin-1
//    {Key_nobreakspace, SoKeyboardEvent::}, // FIXME
//    {Key_exclamdown, SoKeyboardEvent::}, // FIXME
//    {Key_cent, SoKeyboardEvent::}, // FIXME
//    {Key_sterling, SoKeyboardEvent::}, // FIXME
//    {Key_currency, SoKeyboardEvent::}, // FIXME
//    {Key_yen, SoKeyboardEvent::}, // FIXME
//    {Key_brokenbar, SoKeyboardEvent::}, // FIXME
//    {Key_section, SoKeyboardEvent::}, // FIXME
//    {Key_diaeresis, SoKeyboardEvent::}, // FIXME
//    {Key_copyright, SoKeyboardEvent::}, // FIXME
//    {Key_ordfeminine, SoKeyboardEvent::}, // FIXME
//    {Key_guillemotleft, SoKeyboardEvent::}, // FIXME
//    {Key_notsign, SoKeyboardEvent::}, // FIXME
//    {Key_hyphen, SoKeyboardEvent::}, // FIXME
//    {Key_registered, SoKeyboardEvent::}, // FIXME
//    {Key_macron, SoKeyboardEvent::}, // FIXME
//    {Key_degree, SoKeyboardEvent::}, // FIXME
//    {Key_plusminus, SoKeyboardEvent::}, // FIXME
//    {Key_twosuperior, SoKeyboardEvent::}, // FIXME
//    {Key_threesuperior, SoKeyboardEvent::}, // FIXME
//    {Key_acute, SoKeyboardEvent::}, // FIXME
//    {Key_mu, SoKeyboardEvent::}, // FIXME
//    {Key_paragraph, SoKeyboardEvent::}, // FIXME
//    {Key_periodcentered, SoKeyboardEvent::}, // FIXME
//    {Key_cedilla, SoKeyboardEvent::}, // FIXME
//    {Key_onesuperior, SoKeyboardEvent::}, // FIXME
//    {Key_masculine, SoKeyboardEvent::}, // FIXME
//    {Key_guillemotright, SoKeyboardEvent::}, // FIXME
//    {Key_onequarter, SoKeyboardEvent::}, // FIXME
//    {Key_onehalf, SoKeyboardEvent::}, // FIXME
//    {Key_threequarters, SoKeyboardEvent::}, // FIXME
//    {Key_questiondown, SoKeyboardEvent::}, // FIXME
//    {Key_Agrave, SoKeyboardEvent::}, // FIXME
//    {Key_Aacute, SoKeyboardEvent::}, // FIXME
//    {Key_Acircumflex, SoKeyboardEvent::}, // FIXME
//    {Key_Atilde, SoKeyboardEvent::}, // FIXME
//    {Key_Adiaeresis, SoKeyboardEvent::}, // FIXME
//    {Key_Aring, SoKeyboardEvent::}, // FIXME
//    {Key_AE, SoKeyboardEvent::}, // FIXME
//    {Key_Ccedilla, SoKeyboardEvent::}, // FIXME
//    {Key_Egrave, SoKeyboardEvent::}, // FIXME
//    {Key_Eacute, SoKeyboardEvent::}, // FIXME
//    {Key_Ecircumflex, SoKeyboardEvent::}, // FIXME
//    {Key_Ediaeresis, SoKeyboardEvent::}, // FIXME
//    {Key_Igrave, SoKeyboardEvent::}, // FIXME
//    {Key_Iacute, SoKeyboardEvent::}, // FIXME
//    {Key_Icircumflex, SoKeyboardEvent::}, // FIXME
//    {Key_Idiaeresis, SoKeyboardEvent::}, // FIXME
//    {Key_ETH, SoKeyboardEvent::}, // FIXME
//    {Key_Ntilde, SoKeyboardEvent::}, // FIXME
//    {Key_Ograve, SoKeyboardEvent::}, // FIXME
//    {Key_Oacute, SoKeyboardEvent::}, // FIXME
//    {Key_Ocircumflex, SoKeyboardEvent::}, // FIXME
//    {Key_Otilde, SoKeyboardEvent::}, // FIXME
//    {Key_Odiaeresis, SoKeyboardEvent::}, // FIXME
//    {Key_multiply, SoKeyboardEvent::}, // FIXME
//    {Key_Ooblique, SoKeyboardEvent::}, // FIXME
//    {Key_Ugrave, SoKeyboardEvent::}, // FIXME
//    {Key_Uacute, SoKeyboardEvent::}, // FIXME
//    {Key_Ucircumflex, SoKeyboardEvent::}, // FIXME
//    {Key_Udiaeresis, SoKeyboardEvent::}, // FIXME
//    {Key_Yacute, SoKeyboardEvent::}, // FIXME
//    {Key_THORN, SoKeyboardEvent::}, // FIXME
//    {Key_ssharp, SoKeyboardEvent::}, // FIXME
//    {Key_agrave, SoKeyboardEvent::}, // FIXME
//    {Key_aacute, SoKeyboardEvent::}, // FIXME
//    {Key_acircumflex, SoKeyboardEvent::}, // FIXME
//    {Key_atilde, SoKeyboardEvent::}, // FIXME
//    {Key_adiaeresis, SoKeyboardEvent::}, // FIXME
//    {Key_aring, SoKeyboardEvent::}, // FIXME
//    {Key_ae, SoKeyboardEvent::}, // FIXME
//    {Key_ccedilla, SoKeyboardEvent::}, // FIXME
//    {Key_egrave, SoKeyboardEvent::}, // FIXME
//    {Key_eacute, SoKeyboardEvent::}, // FIXME
//    {Key_ecircumflex, SoKeyboardEvent::}, // FIXME
//    {Key_ediaeresis, SoKeyboardEvent::}, // FIXME
//    {Key_igrave, SoKeyboardEvent::}, // FIXME
//    {Key_iacute, SoKeyboardEvent::}, // FIXME
//    {Key_icircumflex, SoKeyboardEvent::}, // FIXME
//    {Key_idiaeresis, SoKeyboardEvent::}, // FIXME
//    {Key_eth, SoKeyboardEvent::}, // FIXME
//    {Key_ntilde, SoKeyboardEvent::}, // FIXME
//    {Key_ograve, SoKeyboardEvent::}, // FIXME
//    {Key_oacute, SoKeyboardEvent::}, // FIXME
//    {Key_ocircumflex, SoKeyboardEvent::}, // FIXME
//    {Key_otilde, SoKeyboardEvent::}, // FIXME
//    {Key_odiaeresis, SoKeyboardEvent::}, // FIXME
//    {Key_division, SoKeyboardEvent::}, // FIXME
//    {Key_oslash, SoKeyboardEvent::}, // FIXME
//    {Key_ugrave, SoKeyboardEvent::}, // FIXME
//    {Key_uacute, SoKeyboardEvent::}, // FIXME
//    {Key_ucircumflex, SoKeyboardEvent::}, // FIXME
//    {Key_udiaeresis, SoKeyboardEvent::}, // FIXME
//    {Key_yacute, SoKeyboardEvent::}, // FIXME
//    {Key_thorn, SoKeyboardEvent::}, // FIXME
//    {Key_ydiaeresis, SoKeyboardEvent::}, // FIXME

  {VK_UNKNOWN, SoKeyboardEvent::ANY} // Ends table
};

// *************************************************************************

// *************************************************************************

class SoWinKeyboardP : public SoGuiKeyboardP {
public:
  SoWinKeyboardP();
  ~SoWinKeyboardP();
  static SbDict * translatetable;
  static int nrofkeyboards;
};

SbDict * SoWinKeyboardP::translatetable = NULL;
int SoWinKeyboardP::nrofkeyboards = 0;

SoWinKeyboardP::SoWinKeyboardP()
{
  ++nrofkeyboards;
}

SoWinKeyboardP::~SoWinKeyboardP()
{
  --nrofkeyboards;
  assert(nrofkeyboards >= 0);
  if (nrofkeyboards == 0) {
    delete translatetable;
    translatetable = 0;
  }
}

// *************************************************************************

SoWinKeyboard::SoWinKeyboard(int events)
{
  PRIVATE(this) = new SoWinKeyboardP();
  PRIVATE(this)->eventmask = events;

  if (PRIVATE(this)->translatetable == NULL) {
    PRIVATE(this)->translatetable = new SbDict;

    int i=0;
    while (WinToSoMapping[i].from != VK_UNKNOWN) {
      PRIVATE(this)->translatetable->enter(WinToSoMapping[i].from,(void *)WinToSoMapping[i].to);
      i++;
    }
  }
}

SoWinKeyboard::~SoWinKeyboard()
{
  delete PRIVATE(this);
}

// *************************************************************************

void
SoWinKeyboard::enable(HWND widget, SoWinEventHandler * callbackproc, void * data)
{
  // Win32 has no way of enabling the keyboard. mariusbu 20010823.
  // Do nothing.

  // FIXME: should still try to add some magic here so Win32 events
  // are actually enabled or disabled for the widget handle in
  // question, or the semantics won't really match what the client
  // code expects. 20020625 mortene.
}

void
SoWinKeyboard::disable(HWND widget, SoWinEventHandler * callbackproc, void * data)
{
  // Win32 has no way of disabling the keyboard. mariusbu 20010823.
  // Do nothing.

  // FIXME: should still try to add some magic here so Win32 events
  // are actually enabled or disabled for the widget handle in
  // question, or the semantics won't really match what the client
  // code expects. 20020625 mortene.
}

// *************************************************************************

const SoEvent *
SoWinKeyboard::translateEvent(MSG * msg)
{
  if ((msg->message != WM_KEYDOWN) && (msg->message != WM_KEYUP) &&
      (msg->message != WM_SYSKEYDOWN) && (msg->message != WM_SYSKEYUP)) {
    return NULL;
  }

  // FIXME: looks like we ignore the value of the eventmask? 20020625 mortene.

  SoButtonEvent::State state = 
    ((msg->message == WM_KEYDOWN) || (msg->message == WM_SYSKEYDOWN)) ? 
      SoButtonEvent::DOWN : SoButtonEvent::UP;
  
  PRIVATE(this)->kbdevent->setState(state);

  // FIXME: wrong -- should be the time the event actually
  // happened. 20011210 mortene.
  PRIVATE(this)->kbdevent->setTime(SbTime::getTimeOfDay());

  if (SOWIN_DEBUG && 0) { // debug
    SoDebugError::postInfo("SoWinKeyboard::makeKeyboardEvent",
                           "MapVirtualKey(msg->wParam,0) == %d",
                           MapVirtualKey(msg->wParam, 0));
  }

  void * sokey;
  if (PRIVATE(this)->translatetable->find(msg->wParam, sokey)) {
    PRIVATE(this)->kbdevent->setKey((SoKeyboardEvent::Key)(int)sokey);
  }
  else {
    PRIVATE(this)->kbdevent->setKey(SoKeyboardEvent::ANY);
    return NULL;
  }

  SbVec2s pos = this->getLastEventPosition();
  this->setEventPosition(PRIVATE(this)->kbdevent, pos[0], pos[1]);

  PRIVATE(this)->kbdevent->setShiftDown(::GetKeyState(VK_SHIFT) & 0x8000);
  PRIVATE(this)->kbdevent->setCtrlDown(::GetKeyState(VK_CONTROL) & 0x8000);
  PRIVATE(this)->kbdevent->setAltDown(::GetKeyState(VK_MENU) & 0x8000);
  
  return PRIVATE(this)->kbdevent;
}

// *************************************************************************

#undef PRIVATE
