#include "x.hpp"

maim::XEngine* xengine = new maim::XEngine();

maim::XEngine::XEngine() {
    m_display = NULL;
    m_visual = NULL;
    m_screen = NULL;
    m_good = false;
}

maim::XEngine::~XEngine() {
    if ( !m_good ) {
        return;
    }
    XCloseDisplay( m_display );
}

int maim::XEngine::init( std::string display ) {
    // Initialize display
    m_display = XOpenDisplay( display.c_str() );
    if ( !m_display ) {
        fprintf( stderr, "Error: Failed to open X display %s\n", display.c_str() );
        return 1;
    }
    m_screen    = ScreenOfDisplay( m_display, DefaultScreen( m_display ) );
    m_visual    = DefaultVisual  ( m_display, XScreenNumberOfScreen( m_screen ) );
    m_colormap  = DefaultColormap( m_display, XScreenNumberOfScreen( m_screen ) );
    m_root      = RootWindow     ( m_display, XScreenNumberOfScreen( m_screen ) );
    //m_root      = DefaultRootWindow( m_display );

    m_res = XRRGetScreenResourcesCurrent( m_display, m_root);
    if ( !m_res ) {
        fprintf( stderr, "Warning: failed to get screen resources. Multi-monitor X screens won't have garbage visual data removed.\n" );
    }

    m_good = true;
    return 0;
}

Window maim::XEngine::getWindowByID( int id ) {
    // There's actually no way to check if the id is valid...
    return (Window)id;
    // The only thing we can do is use it and see if we get a BadWindow error later.
}

void maim::XEngine::tick() {
    if ( !m_good ) {
        return;
    }
    XFlush( m_display );
    XEvent event;
    while ( XPending( m_display ) ) {
        XNextEvent( m_display, &event );
        switch ( event.type ) {
            default: break;
        }
    }
}

// This stuff is used to detect which pixels we can actually see with
// the physical monitor positions.
// It's useful for people with multimonitor setups where the monitors
// don't fit together well since we can black out the pixels that are
// generally just garbage.
std::vector<XRRCrtcInfo*> maim::XEngine::getCRTCS() {
    std::vector<XRRCrtcInfo*> monitors;
    if ( !m_res ) {
        return monitors;
    }
    for ( int i=0;i<m_res->ncrtc;i++ ) {
        monitors.push_back( XRRGetCrtcInfo( m_display, m_res, m_res->crtcs[ i ] ) );
    }
    return monitors;
}

void maim::XEngine::freeCRTCS( std::vector<XRRCrtcInfo*> monitors ) {
    for ( int i=0;i<monitors.size();i++ ) {
        XRRFreeCrtcInfo( monitors[ i ] );
    }
}
