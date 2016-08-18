#define ADDON_ACTION_NONE                    0
#define ADDON_ACTION_MOVE_LEFT               1
#define ADDON_ACTION_MOVE_RIGHT              2
#define ADDON_ACTION_MOVE_UP                 3
#define ADDON_ACTION_MOVE_DOWN               4
#define ADDON_ACTION_PAGE_UP                 5
#define ADDON_ACTION_PAGE_DOWN               6
#define ADDON_ACTION_SELECT_ITEM             7
#define ADDON_ACTION_HIGHLIGHT_ITEM          8
#define ADDON_ACTION_PARENT_DIR              9
#define ADDON_ACTION_PREVIOUS_MENU          10
#define ADDON_ACTION_SHOW_INFO              11

#define ADDON_ACTION_PAUSE                  12
#define ADDON_ACTION_STOP                   13
#define ADDON_ACTION_NEXT_ITEM              14
#define ADDON_ACTION_PREV_ITEM              15
#define ADDON_ACTION_FORWARD                16 //!< Can be used to specify specific action in a window, Playback control is handled in ADDON_ACTION_PLAYER_*
#define ADDON_ACTION_REWIND                 17 //!< Can be used to specify specific action in a window, Playback control is handled in ADDON_ACTION_PLAYER_*

#define ADDON_ACTION_SHOW_GUI               18 //!< toggle between GUI and movie or GUI and visualisation.
#define ADDON_ACTION_ASPECT_RATIO           19 //!< toggle quick-access zoom modes. Can b used in videoFullScreen.zml window id=2005
#define ADDON_ACTION_STEP_FORWARD           20 //!< seek +1% in the movie. Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_STEP_BACK              21 //!< seek -1% in the movie. Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_BIG_STEP_FORWARD       22 //!< seek +10% in the movie. Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_BIG_STEP_BACK          23 //!< seek -10% in the movie. Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_SHOW_OSD               24 //!< show/hide OSD. Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_SHOW_SUBTITLES         25 //!< turn subtitles on/off. Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_NEXT_SUBTITLE          26 //!< switch to next subtitle of movie. Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_SHOW_CODEC             27 //!< show debug info for VideoPlayer
#define ADDON_ACTION_NEXT_PICTURE           28 //!< show next picture of slideshow. Can b used in slideshow.xml window id=2007
#define ADDON_ACTION_PREV_PICTURE           29 //!< show previous picture of slideshow. Can b used in slideshow.xml window id=2007
#define ADDON_ACTION_ZOOM_OUT               30 //!< zoom in picture during slideshow. Can b used in slideshow.xml window id=2007
#define ADDON_ACTION_ZOOM_IN                31 //!< zoom out picture during slideshow. Can b used in slideshow.xml window id=2007
#define ADDON_ACTION_TOGGLE_SOURCE_DEST     32 //!< used to toggle between source view and destination view. Can be used in myfiles.xml window id=3
#define ADDON_ACTION_SHOW_PLAYLIST          33 //!< used to toggle between current view and playlist view. Can b used in all mymusic xml files
#define ADDON_ACTION_QUEUE_ITEM             34 //!< used to queue a item to the playlist. Can b used in all mymusic xml files
#define ADDON_ACTION_REMOVE_ITEM            35 //!< not used anymore
#define ADDON_ACTION_SHOW_FULLSCREEN        36 //!< not used anymore
#define ADDON_ACTION_ZOOM_LEVEL_NORMAL      37 //!< zoom 1x picture during slideshow. Can b used in slideshow.xml window id=2007
#define ADDON_ACTION_ZOOM_LEVEL_1           38 //!< zoom 2x picture during slideshow. Can b used in slideshow.xml window id=2007
#define ADDON_ACTION_ZOOM_LEVEL_2           39 //!< zoom 3x picture during slideshow. Can b used in slideshow.xml window id=2007
#define ADDON_ACTION_ZOOM_LEVEL_3           40 //!< zoom 4x picture during slideshow. Can b used in slideshow.xml window id=2007
#define ADDON_ACTION_ZOOM_LEVEL_4           41 //!< zoom 5x picture during slideshow. Can b used in slideshow.xml window id=2007
#define ADDON_ACTION_ZOOM_LEVEL_5           42 //!< zoom 6x picture during slideshow. Can b used in slideshow.xml window id=2007
#define ADDON_ACTION_ZOOM_LEVEL_6           43 //!< zoom 7x picture during slideshow. Can b used in slideshow.xml window id=2007
#define ADDON_ACTION_ZOOM_LEVEL_7           44 //!< zoom 8x picture during slideshow. Can b used in slideshow.xml window id=2007
#define ADDON_ACTION_ZOOM_LEVEL_8           45 //!< zoom 9x picture during slideshow. Can b used in slideshow.xml window id=2007
#define ADDON_ACTION_ZOOM_LEVEL_9           46 //!< zoom 10x picture during slideshow. Can b used in slideshow.xml window id=2007

#define ADDON_ACTION_CALIBRATE_SWAP_ARROWS  47 //!< select next arrow. Can b used in: settingsScreenCalibration.xml windowid=11
#define ADDON_ACTION_CALIBRATE_RESET        48 //!< reset calibration to defaults. Can b used in: `settingsScreenCalibration.xml` windowid=11/settingsUICalibration.xml windowid=10
#define ADDON_ACTION_ANALOG_MOVE            49 //!< analog thumbstick move. Can b used in: `slideshow.xml` windowid=2007/settingsScreenCalibration.xml windowid=11/settingsUICalibration.xml windowid=10
                                         //!< @note see also ADDON_ACTION_ANALOG_MOVE_X, ADDON_ACTION_ANALOG_MOVE_Y
#define ADDON_ACTION_ROTATE_PICTURE_CW      50 //!< rotate current picture clockwise during slideshow. Can be used in slideshow.xml window id=2007
#define ADDON_ACTION_ROTATE_PICTURE_CCW     51 //!< rotate current picture counterclockwise during slideshow. Can be used in slideshow.xml window id=2007

#define ADDON_ACTION_SUBTITLE_DELAY_MIN     52 //!< Decrease subtitle/movie Delay.  Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_SUBTITLE_DELAY_PLUS    53 //!< Increase subtitle/movie Delay.  Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_AUDIO_DELAY_MIN        54 //!< Increase avsync delay.  Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_AUDIO_DELAY_PLUS       55 //!< Decrease avsync delay.  Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_AUDIO_NEXT_LANGUAGE    56 //!< Select next language in movie.  Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_CHANGE_RESOLUTION      57 //!< switch 2 next resolution. Can b used during screen calibration settingsScreenCalibration.xml windowid=11

#define REMOTE_0                      58  //!< remote keys 0-9. are used by multiple windows
#define REMOTE_1                      59  //!< for example in videoFullScreen.xml window id=2005 you can
#define REMOTE_2                      60  //!< enter time (mmss) to jump to particular point in the movie
#define REMOTE_3                      61
#define REMOTE_4                      62  //!< with spincontrols you can enter 3digit number to quickly set
#define REMOTE_5                      63  //!< spincontrol to desired value
#define REMOTE_6                      64
#define REMOTE_7                      65
#define REMOTE_8                      66
#define REMOTE_9                      67

#define ADDON_ACTION_PLAY                   68  //!< Unused at the moment
#define ADDON_ACTION_SMALL_STEP_BACK        76  //!< jumps a few seconds back during playback of movie. Can b used in videoFullScreen.xml window id=2005

#define ADDON_ACTION_PLAYER_FORWARD         77  //!< FF in current file played. global action, can be used anywhere
#define ADDON_ACTION_PLAYER_REWIND          78  //!< RW in current file played. global action, can be used anywhere
#define ADDON_ACTION_PLAYER_PLAY            79  //!< Play current song. Unpauses song and sets playspeed to 1x. global action, can be used anywhere

#define ADDON_ACTION_DELETE_ITEM            80  //!< delete current selected item. Can be used in myfiles.xml window id=3 and in myvideoTitle.xml window id=25
#define ADDON_ACTION_COPY_ITEM              81  //!< copy current selected item. Can be used in myfiles.xml window id=3
#define ADDON_ACTION_MOVE_ITEM              82  //!< move current selected item. Can be used in myfiles.xml window id=3
#define ADDON_ACTION_TAKE_SCREENSHOT        85  //!< take a screenshot
#define ADDON_ACTION_RENAME_ITEM            87  //!< rename item

#define ADDON_ACTION_VOLUME_UP            88
#define ADDON_ACTION_VOLUME_DOWN          89
#define ADDON_ACTION_VOLAMP               90
#define ADDON_ACTION_MUTE                 91
#define ADDON_ACTION_NAV_BACK             92
#define ADDON_ACTION_VOLAMP_UP            93
#define ADDON_ACTION_VOLAMP_DOWN          94

#define ADDON_ACTION_CREATE_EPISODE_BOOKMARK 95 //!< Creates an episode bookmark on the currently playing video file containing more than one episode
#define ADDON_ACTION_CREATE_BOOKMARK         96 //!< Creates a bookmark of the currently playing video file

#define ADDON_ACTION_CHAPTER_OR_BIG_STEP_FORWARD       97 //!< Goto the next chapter, if not available perform a big step forward
#define ADDON_ACTION_CHAPTER_OR_BIG_STEP_BACK          98 //!< Goto the previous chapter, if not available perform a big step back

#define ADDON_ACTION_CYCLE_SUBTITLE         99 //!< switch to next subtitle of movie, but will not enable/disable the subtitles. Can be used in videoFullScreen.xml window id=2005

#define ADDON_ACTION_MOUSE_START            100
#define ADDON_ACTION_MOUSE_LEFT_CLICK       100
#define ADDON_ACTION_MOUSE_RIGHT_CLICK      101
#define ADDON_ACTION_MOUSE_MIDDLE_CLICK     102
#define ADDON_ACTION_MOUSE_DOUBLE_CLICK     103
#define ADDON_ACTION_MOUSE_WHEEL_UP         104
#define ADDON_ACTION_MOUSE_WHEEL_DOWN       105
#define ADDON_ACTION_MOUSE_DRAG             106
#define ADDON_ACTION_MOUSE_MOVE             107
#define ADDON_ACTION_MOUSE_LONG_CLICK       108
#define ADDON_ACTION_MOUSE_END              109

#define ADDON_ACTION_BACKSPACE          110
#define ADDON_ACTION_SCROLL_UP          111
#define ADDON_ACTION_SCROLL_DOWN        112
#define ADDON_ACTION_ANALOG_FORWARD     113
#define ADDON_ACTION_ANALOG_REWIND      114

#define ADDON_ACTION_MOVE_ITEM_UP       115  //!< move item up in playlist
#define ADDON_ACTION_MOVE_ITEM_DOWN     116  //!< move item down in playlist
#define ADDON_ACTION_CONTEXT_MENU       117  //!< pops up the context menu

// stuff for virtual keyboard shortcuts
#define ADDON_ACTION_SHIFT              118 //!< stuff for virtual keyboard shortcuts
#define ADDON_ACTION_SYMBOLS            119 //!< stuff for virtual keyboard shortcuts
#define ADDON_ACTION_CURSOR_LEFT        120 //!< stuff for virtual keyboard shortcuts
#define ADDON_ACTION_CURSOR_RIGHT       121 //!< stuff for virtual keyboard shortcuts

#define ADDON_ACTION_BUILT_IN_FUNCTION  122

#define ADDON_ACTION_SHOW_OSD_TIME      123 //!< displays current time, can be used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_ANALOG_SEEK_FORWARD  124 //!< seeks forward, and displays the seek bar.
#define ADDON_ACTION_ANALOG_SEEK_BACK     125 //!< seeks backward, and displays the seek bar.

#define ADDON_ACTION_VIS_PRESET_SHOW        126
#define ADDON_ACTION_VIS_PRESET_NEXT        128
#define ADDON_ACTION_VIS_PRESET_PREV        129
#define ADDON_ACTION_VIS_PRESET_LOCK        130
#define ADDON_ACTION_VIS_PRESET_RANDOM      131
#define ADDON_ACTION_VIS_RATE_PRESET_PLUS   132
#define ADDON_ACTION_VIS_RATE_PRESET_MINUS  133

#define ADDON_ACTION_SHOW_VIDEOMENU         134
#define ADDON_ACTION_ENTER                  135

#define ADDON_ACTION_INCREASE_RATING        136
#define ADDON_ACTION_DECREASE_RATING        137

#define ADDON_ACTION_NEXT_SCENE             138 //!< switch to next scene/cutpoint in movie
#define ADDON_ACTION_PREV_SCENE             139 //!< switch to previous scene/cutpoint in movie

#define ADDON_ACTION_NEXT_LETTER            140 //!< jump through a list or container by letter
#define ADDON_ACTION_PREV_LETTER            141

#define ADDON_ACTION_JUMP_SMS2              142 //!< jump direct to a particular letter using SMS-style input
#define ADDON_ACTION_JUMP_SMS3              143
#define ADDON_ACTION_JUMP_SMS4              144
#define ADDON_ACTION_JUMP_SMS5              145
#define ADDON_ACTION_JUMP_SMS6              146
#define ADDON_ACTION_JUMP_SMS7              147
#define ADDON_ACTION_JUMP_SMS8              148
#define ADDON_ACTION_JUMP_SMS9              149

#define ADDON_ACTION_FILTER_CLEAR           150
#define ADDON_ACTION_FILTER_SMS2            151
#define ADDON_ACTION_FILTER_SMS3            152
#define ADDON_ACTION_FILTER_SMS4            153
#define ADDON_ACTION_FILTER_SMS5            154
#define ADDON_ACTION_FILTER_SMS6            155
#define ADDON_ACTION_FILTER_SMS7            156
#define ADDON_ACTION_FILTER_SMS8            157
#define ADDON_ACTION_FILTER_SMS9            158

#define ADDON_ACTION_FIRST_PAGE             159
#define ADDON_ACTION_LAST_PAGE              160

#define ADDON_ACTION_AUDIO_DELAY            161
#define ADDON_ACTION_SUBTITLE_DELAY         162
#define ADDON_ACTION_MENU                   163

#define ADDON_ACTION_SET_RATING             164

#define ADDON_ACTION_RECORD                 170

#define ADDON_ACTION_PASTE                  180
#define ADDON_ACTION_NEXT_CONTROL           181
#define ADDON_ACTION_PREV_CONTROL           182
#define ADDON_ACTION_CHANNEL_SWITCH         183
#define ADDON_ACTION_CHANNEL_UP             184
#define ADDON_ACTION_CHANNEL_DOWN           185
#define ADDON_ACTION_NEXT_CHANNELGROUP      186
#define ADDON_ACTION_PREVIOUS_CHANNELGROUP  187
#define ADDON_ACTION_PVR_PLAY               188
#define ADDON_ACTION_PVR_PLAY_TV            189
#define ADDON_ACTION_PVR_PLAY_RADIO         190

#define ADDON_ACTION_TOGGLE_FULLSCREEN      199 //!< switch 2 desktop resolution
#define ADDON_ACTION_TOGGLE_WATCHED         200 //!< Toggle watched status (videos)
#define ADDON_ACTION_SCAN_ITEM              201 //!< scan item
#define ADDON_ACTION_TOGGLE_DIGITAL_ANALOG  202 //!< switch digital <-> analog
#define ADDON_ACTION_RELOAD_KEYMAPS         203 //!< reloads CButtonTranslator's keymaps
#define ADDON_ACTION_GUIPROFILE_BEGIN       204 //!< start the GUIControlProfiler running

#define ADDON_ACTION_TELETEXT_RED           215 //!< Teletext Color button <b>Red</b> to control TopText
#define ADDON_ACTION_TELETEXT_GREEN         216 //!< Teletext Color button <b>Green</b> to control TopText
#define ADDON_ACTION_TELETEXT_YELLOW        217 //!< Teletext Color button <b>Yellow</b> to control TopText
#define ADDON_ACTION_TELETEXT_BLUE          218 //!< Teletext Color button <b>Blue</b> to control TopText

#define ADDON_ACTION_INCREASE_PAR           219
#define ADDON_ACTION_DECREASE_PAR           220

#define ADDON_ACTION_VSHIFT_UP              227 //!< shift up video image in VideoPlayer
#define ADDON_ACTION_VSHIFT_DOWN            228 //!< shift down video image in VideoPlayer

#define ADDON_ACTION_PLAYER_PLAYPAUSE       229 //!< Play/pause. If playing it pauses, if paused it plays.

#define ADDON_ACTION_SUBTITLE_VSHIFT_UP     230 //!< shift up subtitles in VideoPlayer
#define ADDON_ACTION_SUBTITLE_VSHIFT_DOWN   231 //!< shift down subtitles in VideoPlayer
#define ADDON_ACTION_SUBTITLE_ALIGN         232 //!< toggle vertical alignment of subtitles

#define ADDON_ACTION_FILTER                 233

#define ADDON_ACTION_SWITCH_PLAYER          234

#define ADDON_ACTION_STEREOMODE_NEXT        235
#define ADDON_ACTION_STEREOMODE_PREVIOUS    236
#define ADDON_ACTION_STEREOMODE_TOGGLE      237 //!< turns 3d mode on/off
#define ADDON_ACTION_STEREOMODE_SELECT      238
#define ADDON_ACTION_STEREOMODE_TOMONO      239
#define ADDON_ACTION_STEREOMODE_SET         240

#define ADDON_ACTION_SETTINGS_RESET         241
#define ADDON_ACTION_SETTINGS_LEVEL_CHANGE  242

#define ADDON_ACTION_TRIGGER_OSD            243 //!< show autoclosing OSD. Can b used in videoFullScreen.xml window id=2005
#define ADDON_ACTION_INPUT_TEXT             244
#define ADDON_ACTION_VOLUME_SET             245
#define ADDON_ACTION_TOGGLE_COMMSKIP        246

#define ADDON_ACTION_TOUCH_TAP              401 //!< touch actions
#define ADDON_ACTION_TOUCH_TAP_TEN          410 //!< touch actions
#define ADDON_ACTION_TOUCH_LONGPRESS        411 //!< touch actions
#define ADDON_ACTION_TOUCH_LONGPRESS_TEN    420 //!< touch actions

#define ADDON_ACTION_GESTURE_NOTIFY         500
#define ADDON_ACTION_GESTURE_BEGIN          501
#define ADDON_ACTION_GESTURE_ZOOM           502 //!< sendaction with point and currentPinchScale (fingers together < 1.0 -> fingers apart > 1.0)
#define ADDON_ACTION_GESTURE_ROTATE         503
#define ADDON_ACTION_GESTURE_PAN            504

#define ADDON_ACTION_GESTURE_SWIPE_LEFT       511
#define ADDON_ACTION_GESTURE_SWIPE_LEFT_TEN   520
#define ADDON_ACTION_GESTURE_SWIPE_RIGHT      521
#define ADDON_ACTION_GESTURE_SWIPE_RIGHT_TEN  530
#define ADDON_ACTION_GESTURE_SWIPE_UP         531
#define ADDON_ACTION_GESTURE_SWIPE_UP_TEN     540
#define ADDON_ACTION_GESTURE_SWIPE_DOWN       541
#define ADDON_ACTION_GESTURE_SWIPE_DOWN_TEN   550
// 5xx is reserved for additional gesture actions
#define ADDON_ACTION_GESTURE_END            599

// other, non-gesture actions
#define ADDON_ACTION_ANALOG_MOVE_X            601 //!< analog thumbstick move, horizontal axis; see ADDON_ACTION_ANALOG_MOVE
#define ADDON_ACTION_ANALOG_MOVE_Y            602 //!< analog thumbstick move, vertical axis; see ADDON_ACTION_ANALOG_MOVE
//@}

// The NOOP action can be specified to disable an input event. This is
// useful in user keyboard.xml etc to disable actions specified in the
// system mappings. ERROR action is used to play an error sound
#define ADDON_ACTION_ERROR                  998
#define ADDON_ACTION_NOOP                   999