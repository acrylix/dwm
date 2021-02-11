/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const int gappx     			= 10;                 /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     	/* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char slopstyle[]       = "-t 0 -c 0.92,0.85,0.69,0.3"; /* do NOT define -f (format) here */
static const int user_bh            = 40;       /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static		 int attachdirection 	= 0;    	/* 0 default, 1 above, 2 aside, 3 below, 4 bottom, 5 top */

static const char *fonts[]          = { "Terminess Powerline:size=13:style=Bold:antialias=true" };
static const char dmenufont[]       = "Terminess Powerline:size=11:style=Bold:antialias=true";

static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_yellow[]		= "#ffcd44";
static const char col_olive[]		= "#b4c248";
static const char col_purple[]		= "#4c48c2";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_purple, col_olive,  col_olive  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const char *tagsalt[] = { "•", "•", "•", "•", "•", "•", "•", "•", "•" };

/* grid of tags */
#define DRAWCLASSICTAGS             1 << 1
#define DRAWTAGGRID                 1 << 1

#define SWITCHTAG_UP                1 << 0
#define SWITCHTAG_DOWN              1 << 1
#define SWITCHTAG_LEFT              1 << 2
#define SWITCHTAG_RIGHT             1 << 3
#define SWITCHTAG_TOGGLETAG         1 << 4
#define SWITCHTAG_TAG               1 << 5
#define SWITCHTAG_VIEW              1 << 6
#define SWITCHTAG_TOGGLEVIEW        1 << 7

static const unsigned int drawtagmask = DRAWTAGGRID; /* | DRAWCLASSICTAGS to show classic row of tags */
static const int tagrows = 2;


static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) 		= instance, class
	 *	WM_NAME(STRING) 		= title
	 *  WM_WINDOW_ROLE(STRING) 	= role
	 */	
	/* class      			role		instance    title       tags mask     	isfloating   	monitor */
	{ "Gimp",     			NULL,		NULL,       NULL,       0,           	1,           	-1	},
	{ "Org.gnome.Nautilus", NULL,		NULL, 		NULL, 		0, 				1, 				-1	},
	{ "SpeedCrunch", 		NULL,		NULL, 		NULL, 		0, 				1, 				-1	},	
	{ "Google-chrome",		"pop-up",	NULL,		NULL,		0,				1,				-1	},
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "[D]",      deck },
};

/* key definitions */
#define MODKEY 		Mod1Mask
#define CTRLKEY 	ControlMask
#define SUPERKEY	Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \
	{ MOD, XK_q,     ACTION##stack, {.i = 0 } }, \
	{ MOD, XK_a,     ACTION##stack, {.i = 1 } }, \
	{ MOD, XK_z,     ACTION##stack, {.i = 2 } }, \
	{ MOD, XK_x,     ACTION##stack, {.i = -1 } },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
// static const char *roficmd[] = { "rofi", "-show", "run" };
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "alacritty", "-t", scratchpadname, "-d", "100", "30", NULL };
static const char *firefox[] = { "firefox", NULL };
static const char *chrome[] = { "google-chrome-stable", NULL };
static const char *printScreen[] = { "flameshot", "gui", NULL };
static const char *fileExplorer[] = { "nautilus", NULL };
 
static Key keys[] = {
	/* modifier                     key        function        argument */
	// { MODKEY,                       XK_space,  spawn,      	   {.v = roficmd} },
	// { MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ CTRLKEY,						XK_Return, spawn,          {.v = termcmd } },
	{ CTRLKEY|ShiftMask,			XK_Return, spawn,		   {.v = firefox } },
	{ MODKEY|ShiftMask,				XK_Return, spawn,		   {.v = chrome } },
	{ MODKEY,                       XK_u,  togglescratch,  	   {.v = scratchpadcmd } },
	{ 0,							XK_Print,  spawn,		   {.v = printScreen } },
	{ MODKEY,						XK_e,	   spawn,		   {.v = fileExplorer } },
	{ CTRLKEY,						XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY|ShiftMask,             XK_b,      togglealttag,   {0} },
	STACKKEYS(MODKEY,                          focus)
	STACKKEYS(MODKEY|ShiftMask,                push)
	{ SUPERKEY,                     XK_u,      focusurgent,    {0} },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ SUPERKEY,						XK_Tab,	   riodraw,		   {0} },
	{ MODKEY,             			XK_f,      fullscreen,     {0} },
	{ SUPERKEY|MODKEY,           	XK_f,      setlayout,      {.v = &layouts[1]} },
	{ SUPERKEY|MODKEY,           	XK_t,      setlayout,      {.v = &layouts[0]} },
	{ SUPERKEY|MODKEY,           	XK_m,      setlayout,      {.v = &layouts[2]} },
	{ SUPERKEY|MODKEY,           	XK_u,      setlayout,      {.v = &layouts[3]} },
	{ SUPERKEY|MODKEY,           	XK_o,      setlayout,      {.v = &layouts[4]} },
	{ SUPERKEY|MODKEY,           	XK_d,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY,             			XK_t,  	   togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },

	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

	{ MODKEY|ShiftMask,             XK_minus,  setgaps,        {.i = -5 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = +5 } },

	{ MODKEY,                       XK_equal,  incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_minus,  incnmaster,     {.i = -1 } },

	{ MODKEY|SUPERKEY,             		XK_1,  	changeAttachDirection,  { .i = 1 } },
	{ MODKEY|SUPERKEY,             		XK_2,  	changeAttachDirection,  { .i = 2 } },
	{ MODKEY|SUPERKEY,             		XK_3,  	changeAttachDirection,  { .i = 3 } },
	{ MODKEY|SUPERKEY,             		XK_4,  	changeAttachDirection,  { .i = 4 } },
	{ MODKEY|SUPERKEY,             		XK_5,  	changeAttachDirection,  { .i = 5 } },
	{ MODKEY|SUPERKEY,             		XK_0,  	changeAttachDirection,  { .i = 0 } },
 
	{ MODKEY,              			XK_bracketright,           	view_adjacent,  { .i = +1 } },
	{ MODKEY,              			XK_bracketleft,           	view_adjacent,  { .i = -1 } },
	{ CTRLKEY,              		XK_bracketright,           	view_adjacent,  { .i = +1 } },
	{ CTRLKEY,              		XK_bracketleft,           	view_adjacent,  { .i = -1 } },
	{ MODKEY|ShiftMask,				XK_bracketleft, 			focusmon,       { .i = -1 } },
	{ MODKEY|ShiftMask,             XK_bracketright, 			focusmon,       { .i = +1 } },

	{ MODKEY|CTRLKEY,               XK_q,      moveplace,      {.ui = WIN_NW }},
	{ MODKEY|CTRLKEY,               XK_w,      moveplace,      {.ui = WIN_N  }},
	{ MODKEY|CTRLKEY,               XK_e,      moveplace,      {.ui = WIN_NE }},
	{ MODKEY|CTRLKEY,               XK_a,      moveplace,      {.ui = WIN_W  }},
	{ MODKEY|CTRLKEY,               XK_s,      moveplace,      {.ui = WIN_C  }},
	{ MODKEY|CTRLKEY,               XK_d,      moveplace,      {.ui = WIN_E  }},
	{ MODKEY|CTRLKEY,               XK_z,      moveplace,      {.ui = WIN_SW }},
	{ MODKEY|CTRLKEY,               XK_x,      moveplace,      {.ui = WIN_S  }},
	{ MODKEY|CTRLKEY,               XK_c,      moveplace,      {.ui = WIN_SE }},

    { SUPERKEY|MODKEY,           XK_Up,     switchtag,      { .ui = SWITCHTAG_UP     | SWITCHTAG_VIEW } },
    { SUPERKEY|MODKEY,           XK_Down,   switchtag,      { .ui = SWITCHTAG_DOWN   | SWITCHTAG_VIEW } },
    { SUPERKEY|MODKEY,           XK_Right,  switchtag,      { .ui = SWITCHTAG_RIGHT  | SWITCHTAG_VIEW } },
    { SUPERKEY|MODKEY,           XK_Left,   switchtag,      { .ui = SWITCHTAG_LEFT   | SWITCHTAG_VIEW } },

    { SUPERKEY|MODKEY|ShiftMask,              XK_Up,     switchtag,      { .ui = SWITCHTAG_UP     | SWITCHTAG_TAG | SWITCHTAG_VIEW } },
    { SUPERKEY|MODKEY|ShiftMask,              XK_Down,   switchtag,      { .ui = SWITCHTAG_DOWN   | SWITCHTAG_TAG | SWITCHTAG_VIEW } },
    { SUPERKEY|MODKEY|ShiftMask,              XK_Right,  switchtag,      { .ui = SWITCHTAG_RIGHT  | SWITCHTAG_TAG | SWITCHTAG_VIEW } },
    { SUPERKEY|MODKEY|ShiftMask,              XK_Left,   switchtag,      { .ui = SWITCHTAG_LEFT   | SWITCHTAG_TAG | SWITCHTAG_VIEW } },

	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|SUPERKEY|ShiftMask,	XK_BackSpace, quit,        {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      		button          function        argument */
	{ ClkLtSymbol,          0,              		Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              		Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              		Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              		Button2,        spawn,          {.v = termcmd } },
	/* placemouse options, choose which feels more natural:
	 *    0 - tiled position is relative to mouse cursor
	 *    1 - tiled postiion is relative to window center
	 *    2 - mouse pointer warps to window center
	 */
	{ ClkClientWin,         SUPERKEY|ControlMask, 	Button1,    placemouse,     	{.i = 2} },
	{ ClkClientWin,         SUPERKEY,         		Button1,        movemouse,      {0} },
	{ ClkClientWin,         SUPERKEY,         		Button2,        togglefloating, {0} },
	{ ClkClientWin,         SUPERKEY,         		Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              		Button1,        view,           {0} },
	{ ClkTagBar,            0,              		Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         		Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         		Button3,        toggletag,      {0} },
};

