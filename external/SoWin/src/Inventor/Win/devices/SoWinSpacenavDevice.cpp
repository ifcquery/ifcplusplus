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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/misc/SoBasic.h>
#include <Inventor/events/SoMotion3Event.h>
#include <Inventor/events/SoSpaceballButtonEvent.h>

#include <Inventor/Win/devices/SoWinSpacenavDevice.h>

#include <cstdio>

#define PRIVATE(obj) ((obj)->pimpl)


SOWIN_OBJECT_SOURCE(SoWinSpacenavDevice);


class SoWinSpacenavDeviceP {
public:
  SoWinSpacenavDeviceP(SoWinSpacenavDevice * master) {
    this->master = master;
    this->motion3event = new SoMotion3Event;
    this->buttonevent = new SoSpaceballButtonEvent;
    this->rotationscale = 0.0001f;
    this->translationscale = 0.00001f;
  } 
  ~SoWinSpacenavDeviceP() {
    delete this->motion3event;
    delete this->buttonevent;
  };

  SbRotation makeRotation(const int rx, const int ry, const int rz) const;
  SbVec3f makeTranslation(const int tx, const int ty, const int tz) const;

  SoMotion3Event * motion3event;
  SoSpaceballButtonEvent * buttonevent;
  float rotationscale;
  float translationscale;
  SoWinSpacenavDevice * master;
};

SbRotation
SoWinSpacenavDeviceP::makeRotation(const int rx, const int ry, const int rz) const
{
  SbRotation xrot(SbVec3f(1, 0, 0), rx * this->rotationscale);
  SbRotation yrot(SbVec3f(0, 1, 0), ry * this->rotationscale);
  SbRotation zrot(SbVec3f(0, 0, 1), rz * this->rotationscale);
  return xrot * yrot * zrot;
}

SbVec3f
SoWinSpacenavDeviceP::makeTranslation(const int tx, const int ty, const int tz) const
{
  return SbVec3f(tx * this->translationscale,
                 ty * this->translationscale,
                 tz * this->translationscale);
}



SoWinSpacenavDevice::SoWinSpacenavDevice()
{
  PRIVATE(this) = new SoWinSpacenavDeviceP(this);
}


SoWinSpacenavDevice::~SoWinSpacenavDevice()
{
  delete PRIVATE(this);
}


void
SoWinSpacenavDevice::enable(HWND w, SoWinEventHandler * handler, void * closure)
{
	// No need.
}


void 
SoWinSpacenavDevice::disable(HWND w, SoWinEventHandler * handler, void * closure)
{
  // No need.
}

  
const SoEvent * 
SoWinSpacenavDevice::translateEvent(MSG * inevent)
{
  SoEvent * ret = NULL;

  if (inevent->message == WM_INPUT) {
    // Read event header
    RAWINPUTHEADER header;
    UINT size = sizeof(header);
    if (GetRawInputData((HRAWINPUT)inevent->lParam, RID_HEADER, 
                        &header, &size, sizeof(RAWINPUTHEADER)) == -1) {
      return NULL; // Could not get header.
    }

    // Set aside enough memory for the full event
    size = header.dwSize;
    LPRAWINPUT event = (LPRAWINPUT)malloc(size);
    if (GetRawInputData((HRAWINPUT)inevent->lParam, RID_INPUT, 
                        event, &size, sizeof(RAWINPUTHEADER)) == -1) {
      free(event);
      return NULL; // Error reading event-data
    }
    
    if (event->header.dwType == RIM_TYPEHID) {
      static int all6DOFs[6] = {0};
      LPRAWHID pRawHid = &event->data.hid;
      
      // Translation or Rotation packet?  They come in two different packets.
      if (pRawHid->bRawData[0] == 1) { // Translation vector        
        all6DOFs[0] = (pRawHid->bRawData[1] & 0x000000ff) | 
          ((signed short)(pRawHid->bRawData[2]<<8) & 0xffffff00); 
        all6DOFs[1] = (pRawHid->bRawData[3] & 0x000000ff) | 
          ((signed short)(pRawHid->bRawData[4]<<8) & 0xffffff00); 
        all6DOFs[2] = (pRawHid->bRawData[5] & 0x000000ff) | 
          ((signed short)(pRawHid->bRawData[6]<<8) & 0xffffff00);

        SbVec3f trans = PRIVATE(this)->makeTranslation(all6DOFs[0], all6DOFs[1], all6DOFs[2]);
        PRIVATE(this)->motion3event->setTranslation(trans);
		ret = PRIVATE(this)->motion3event;
      }
      else if (pRawHid->bRawData[0] == 2) { // Rotation vector        
        all6DOFs[3] = (pRawHid->bRawData[1] & 0x000000ff) | 
          ((signed short)(pRawHid->bRawData[2]<<8) & 0xffffff00); 
        all6DOFs[4] = (pRawHid->bRawData[3] & 0x000000ff) | 
          ((signed short)(pRawHid->bRawData[4]<<8) & 0xffffff00); 
        all6DOFs[5] = (pRawHid->bRawData[5] & 0x000000ff) | 
          ((signed short)(pRawHid->bRawData[6]<<8) & 0xffffff00);
        SbRotation rot = PRIVATE(this)->makeRotation(all6DOFs[3], all6DOFs[4], all6DOFs[5]);
        PRIVATE(this)->motion3event->setRotation(rot);
		ret = PRIVATE(this)->motion3event;
      }
      else if (pRawHid->bRawData[0] == 3) { // Buttons (display most significant byte to least)
        
		PRIVATE(this)->buttonevent->setState(SoButtonEvent::DOWN);
		switch ((unsigned char)pRawHid->bRawData[1]) {
			case 0: PRIVATE(this)->buttonevent->setState(SoButtonEvent::UP); 
					break;
			case 1: PRIVATE(this)->buttonevent->setButton(SoSpaceballButtonEvent::BUTTON1);
					break;
			case 2: PRIVATE(this)->buttonevent->setButton(SoSpaceballButtonEvent::BUTTON2);
					break;
			case 3: PRIVATE(this)->buttonevent->setButton(SoSpaceballButtonEvent::BUTTON3);
					break;
			case 4: PRIVATE(this)->buttonevent->setButton(SoSpaceballButtonEvent::BUTTON4);
					break;
			case 5: PRIVATE(this)->buttonevent->setButton(SoSpaceballButtonEvent::BUTTON5);
					break;
			case 6: PRIVATE(this)->buttonevent->setButton(SoSpaceballButtonEvent::BUTTON6);
					break;
			case 7: PRIVATE(this)->buttonevent->setButton(SoSpaceballButtonEvent::BUTTON7);
					break;
			case 8: PRIVATE(this)->buttonevent->setButton(SoSpaceballButtonEvent::BUTTON8);
					break;
			default:
					break;
			// FIXME: Which button is SoSpaceballButtonEvent::PICK? (handegar)
		}
		
		ret = PRIVATE(this)->buttonevent;
      } 
    }
    
    free(event);
  }

  return ret;
}

#undef PRIVATE



