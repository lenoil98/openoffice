/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/


#ifndef _SV_SALFRAME_H
#define _SV_SALFRAME_H

// -=-= #includes -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#include <unx/salstd.hxx>
#include <unx/salunx.h>

#include <salframe.hxx>
#include <salwtype.hxx>
#include <salinst.hxx>

#include <vcl/ptrstyle.hxx>
#include <vcl/sysdata.hxx>
#include <vcl/timer.hxx>
#include <vclpluginapi.h>


#include <list>

// -=-= forwards -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
class	SalDisplay;
class	X11SalGraphics;
class SalGraphicsLayout;
class   SalI18N_InputContext;

namespace vcl_sal { class WMAdaptor; class NetWMAdaptor; class GnomeWMAdaptor; }

// -=-= X11SalFrame =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#define SHOWSTATE_UNKNOWN		-1
#define SHOWSTATE_MINIMIZED 	0
#define SHOWSTATE_NORMAL		1
#define SHOWSTATE_HIDDEN		2

class VCLPLUG_GEN_PUBLIC X11SalFrame : public SalFrame
{
    friend class vcl_sal::WMAdaptor;
    friend class vcl_sal::NetWMAdaptor;
    friend class vcl_sal::GnomeWMAdaptor;
    
    static X11SalFrame* s_pSaveYourselfFrame;
                                
    X11SalFrame*	mpParent;            // pointer to parent frame
	                                // which should never obscur this frame
    bool			mbTransientForRoot;
    std::list< X11SalFrame* > maChildren;         // List of child frames
    
    SalDisplay	   *pDisplay_;
    int             m_nScreen;
    XLIB_Window		mhWindow;
    XLIB_Window		mhShellWindow;
    XLIB_Window		mhForeignParent;
    // window to fall back to when no longer in fullscreen mode
    XLIB_Window		mhStackingWindow;
    // window to listen for CirculateNotify events
    
    Pixmap          mhBackgroundPixmap;
    
    XLIB_Cursor		hCursor_;
    int				nCaptured_;			// is captured
    
    X11SalGraphics	*pGraphics_;			// current frame graphics
    X11SalGraphics	*pFreeGraphics_;		// first free frame graphics
    
    XLIB_Time		nReleaseTime_;		// timestamp of last key release
    sal_uInt16			nKeyCode_;			// last key code
    sal_uInt16			nKeyState_;			// last key state
    int				nCompose_;			// compose state
    bool			mbKeyMenu;
    bool			mbSendExtKeyModChange;
    sal_uInt16			mnExtKeyMod;
    
    int				nShowState_;		// show state
    int				nWidth_;			// client width
    int				nHeight_;			// client height
    Rectangle		maRestorePosSize;
    sal_uIntPtr			nStyle_;
    SalExtStyle     mnExtStyle;
    sal_Bool			bAlwaysOnTop_;
    sal_Bool			bViewable_;
    sal_Bool			bMapped_;
    sal_Bool			mbInShow;
    sal_Bool			bDefaultPosition_;	// client is centered initially
    bool            m_bXEmbed;
    int				nVisibility_;
    int             m_nWorkArea;
    bool            m_bSetFocusOnMap;

    
    int				nScreenSaversTimeout_;
    Rectangle		maPaintRegion;

    Timer			maAlwaysOnTopRaiseTimer;
    
    // data for WMAdaptor
    int				meWindowType;
    int				mnDecorationFlags;
    bool			mbMaximizedVert;
    bool			mbMaximizedHorz;
    bool			mbShaded;
    bool			mbFullScreen;
    
    // icon id
    int			    mnIconID;
    
    String          m_aTitle;
    
    SystemChildData maSystemChildData;
    
    SalI18N_InputContext *mpInputContext;
    Bool			mbInputFocus;
    
    XRectangle*     m_pClipRectangles;
    int             m_nCurClipRect;
    int             m_nMaxClipRect;
    
    
    void			GetPosSize( Rectangle &rPosSize );
    void			SetSize   ( const Size      &rSize );
    void            Center();
    void			SetPosSize( const Rectangle &rPosSize );
    void			Minimize();
    void			Maximize();
    void			Restore();
    void            SetWindowGravity (int nGravity ) const;
    
    void			RestackChildren( XLIB_Window* pTopLevelWindows, int nTopLevelWindows );
    void			RestackChildren();
    
    long			HandleKeyEvent		( XKeyEvent			*pEvent );
    long			HandleMouseEvent	( XEvent			*pEvent );
    long			HandleFocusEvent	( XFocusChangeEvent *pEvent );
    long			HandleExposeEvent	( XEvent			*pEvent );
    long			HandleSizeEvent		( XConfigureEvent	*pEvent );
    long			HandleColormapEvent	( XColormapEvent	*pEvent );
    long			HandleMapUnmapEvent	( XEvent			*pEvent );
    long			HandleStateEvent	( XPropertyEvent	*pEvent );
    long			HandleReparentEvent	( XReparentEvent	*pEvent );
    long			HandleClientMessage	( XClientMessageEvent*pEvent );
    
    DECL_LINK( HandleAlwaysOnTopRaise, void* );

    void			passOnSaveYourSelf();
    
    void            createNewWindow( XLIB_Window aParent, int nScreen = -1 );
    void            updateScreenNumber();
    
    void            setXEmbedInfo();
    void            askForXEmbedFocus( sal_Int32 i_nTimeCode );
public:
	X11SalFrame( SalFrame* pParent, sal_uIntPtr nSalFrameStyle, SystemParentData* pSystemParent = NULL );
	virtual ~X11SalFrame();

    long			Dispatch( XEvent *pEvent );
    void			Init( sal_uIntPtr nSalFrameStyle, int nScreen = -1,
                          SystemParentData* pParentData = NULL, bool bUseGeometry = false );

    SalDisplay*				GetDisplay() const { return pDisplay_; }
	Display*				GetXDisplay() const;
	XLIB_Window				GetDrawable() const;
    int                     GetScreenNumber() const { return m_nScreen; }
    XLIB_Window				GetWindow() const { return mhWindow; }
    XLIB_Window				GetShellWindow() const { return mhShellWindow; }
	XLIB_Window				GetForeignParent() const { return mhForeignParent; }
	XLIB_Window				GetStackingWindow() const { return mhStackingWindow; }
	long					ShutDown() const { return CallCallback( SALEVENT_SHUTDOWN, 0 ); }
	long					Close() const { return CallCallback( SALEVENT_CLOSE, 0 ); }
          	sal_uIntPtr           GetStyle() const { return nStyle_; }
	
	inline	XLIB_Cursor		GetCursor() const { return hCursor_; }
	inline	sal_Bool			IsCaptured() const { return nCaptured_ == 1; }
#if !defined(__synchronous_extinput__)
    void 			HandleExtTextEvent (XClientMessageEvent *pEvent);
    void			PostExtTextEvent (sal_uInt16 nExtTextEventType, 
                                      void *pExtTextEvent);
#endif
    bool					IsOverrideRedirect() const;
    bool                    IsChildWindow() const { return (nStyle_ & (SAL_FRAME_STYLE_PLUG|SAL_FRAME_STYLE_SYSTEMCHILD)) != 0; }
    bool                    IsSysChildWindow() const { return (nStyle_ & (SAL_FRAME_STYLE_SYSTEMCHILD)) != 0; }
    bool					IsFloatGrabWindow() const;
    SalI18N_InputContext* getInputContext() const { return mpInputContext; }
    void			        getPosSize( Rectangle& rRect ) { GetPosSize( rRect ); }
    void					setPosSize( const Rectangle& rRect ) { SetPosSize( rRect ); }
    bool					isMapped() const { return bMapped_; }
    bool                    hasFocus() const { return mbInputFocus; }
    
    void                    beginUnicodeSequence();
    bool                    appendUnicodeSequence( sal_Unicode );
    bool                    endUnicodeSequence();
    
    virtual SalGraphics*		GetGraphics();
    virtual void				ReleaseGraphics( SalGraphics* pGraphics );
    
    // call with true to clear graphics (setting None as drawable)
    // call with false to setup graphics with window (GetWindow())
    virtual void                updateGraphics( bool bClear );

    virtual sal_Bool				PostEvent( void* pData );

    virtual void				SetTitle( const XubString& rTitle );
    virtual void				SetIcon( sal_uInt16 nIcon );
    virtual void				SetMenu( SalMenu* pMenu );
    virtual void                                DrawMenuBar();

    virtual void                SetExtendedFrameStyle( SalExtStyle nExtStyle );
    virtual void				Show( sal_Bool bVisible, sal_Bool bNoActivate = sal_False );
    virtual void				Enable( sal_Bool bEnable );
    virtual void              SetMinClientSize( long nWidth, long nHeight );
    virtual void              SetMaxClientSize( long nWidth, long nHeight );
    virtual void				SetPosSize( long nX, long nY, long nWidth, long nHeight, sal_uInt16 nFlags );
    virtual void				GetClientSize( long& rWidth, long& rHeight );
    virtual void				GetWorkArea( Rectangle& rRect );
    virtual SalFrame*			GetParent() const;
    virtual void				SetWindowState( const SalFrameState* pState );
    virtual sal_Bool				GetWindowState( SalFrameState* pState );
    virtual void				ShowFullScreen( sal_Bool bFullScreen, sal_Int32 nMonitor );
    virtual void				StartPresentation( sal_Bool bStart );
    virtual void				SetAlwaysOnTop( sal_Bool bOnTop );
    virtual void				ToTop( sal_uInt16 nFlags );
    virtual void				SetPointer( PointerStyle ePointerStyle );
    virtual void				CaptureMouse( sal_Bool bMouse );
    virtual void				SetPointerPos( long nX, long nY );
    using SalFrame::Flush;
    virtual void				Flush();
    virtual void				Sync();
    virtual void				SetInputContext( SalInputContext* pContext );
    virtual void				EndExtTextInput( sal_uInt16 nFlags );
    virtual String				GetKeyName( sal_uInt16 nKeyCode );
    virtual String				GetSymbolKeyName( const XubString& rFontName, sal_uInt16 nKeyCode );
    virtual sal_Bool                MapUnicodeToKeyCode( sal_Unicode aUnicode, LanguageType aLangType, KeyCode& rKeyCode );
    virtual LanguageType		GetInputLanguage();
    virtual SalBitmap*			SnapShot();
    virtual void				UpdateSettings( AllSettings& rSettings );
    virtual void				Beep( SoundType eSoundType );
    virtual const SystemEnvData*	GetSystemData() const;
    virtual SalPointerState     GetPointerState();
    virtual void				SetParent( SalFrame* pNewParent );
    virtual bool				SetPluginParent( SystemParentData* pNewParent );
    virtual void                SetBackgroundBitmap( SalBitmap* pBitmap );

    virtual void                SetScreenNumber( unsigned int );
    
    // shaped system windows
    // set clip region to none (-> rectangular windows, normal state)
	virtual void					ResetClipRegion();
    // start setting the clipregion consisting of nRects rectangles
	virtual void					BeginSetClipRegion( sal_uIntPtr nRects );
    // add a rectangle to the clip region
	virtual void					UnionClipRegion( long nX, long nY, long nWidth, long nHeight );
    // done setting up the clipregion
	virtual void					EndSetClipRegion();

    static void SaveYourselfDone( SalFrame* );
	static Bool checkKeyReleaseForRepeat( Display*, XEvent*, XPointer pX11SalFrame );
};

#ifdef _SV_SALDISP_HXX

inline Display *X11SalFrame::GetXDisplay() const
{ return pDisplay_->GetDisplay(); }

inline XLIB_Window X11SalFrame::GetDrawable() const
{ return GetWindow(); }

#endif

#endif // _SV_SALFRAME_H

