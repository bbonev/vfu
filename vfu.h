/*
 *
 * (c) Vladi Belperchinov-Shabanski "Cade" 1996-2000
 * http://www.biscom.net/~cade  <cade@biscom.net>  <cade@datamax.bg>
 *
 * SEE `README',`LICENSE' OR `COPYING' FILE FOR LICENSE AND OTHER DETAILS!
 *
 * $Id: vfu.h,v 1.2 2001/10/28 13:56:40 cade Exp $
 *
 */

#ifndef _VFU_H_
#define _VFU_H_

/*############################################ INCLUDE's ###############*/

  #include <dirent.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <sys/wait.h>

  #include <stdio.h>
  #include <stdlib.h>
  #include <unistd.h>
  #include <string.h>
  #include <time.h>
  #include <utime.h>
  #include <assert.h>
  #include <ctype.h>
  #include <grp.h>
  #include <pwd.h>
  #include <errno.h>
  #include <signal.h>

  #include <vslib.h>

  #ifdef _TARGET_NETBSD_
    #include <sys/param.h>
    #include <sys/mount.h>
  #else
    #include <sys/vfs.h>
  #endif

  #include "vfusetup.h"

  #include "vfusys.h" // for attrs_t

/*############################################ COMPATIBILITY DEF's #####*/

#ifdef _TARGET_GO32_
  #define S_ISLNK( p ) 0
  #define S_ISSOCK( p ) 0
  #define lstat(n,p) memset(p,0,sizeof(p))
  #define cuserid(x)    0

  #define FNMATCH_FLAGS FNM_CASEFOLD
  #define FNCASE        0
  #define PATH_DELIMITER   ";"
#endif

#ifdef _TARGET_UNIX_
  #define FNMATCH_FLAGS 0
  #define FNCASE        1
  #define PATH_DELIMITER   ":"
#endif

#define FNMATCH(p,s) fnmatch((p),(s),FNMATCH_FLAGS)

#ifdef _TARGET_GO32_
  #define pathcmp strcasecmp
  #define pathncmp strncasecmp
#else
  #define pathcmp strcmp
  #define pathncmp strncmp
#endif

  typedef double fsize_t; /* used as big integer */
  typedef char  max_path_str_t[MAX_PATH]; /* */

/*############################################ GLOBAL DEFINES  #########*/

  /* history id's */
  #define HID_GREP        10
  #define HID_GS_MASK     20
  #define HID_GS_GREP     30
  #define HID_MKPATH      40
  #define HID_FFMASK      50
  #define HID_FFPATH      60
  #define HID_FFGREP      70
  #define HID_EE_TIME     80 // entry edit
  #define HID_EE_OWNER    90 // entry edit
  #define HID_SHELL_PAR  100
  #define HID_FMASK      110
  #define HID_COMMANDS   120
  #define HID_GETDIR     130
  #define HID_CHDIR      140
  #define HID_SEQ_PREFIX 150
  #define HID_SEQ_SUFFIX 160
  #define HID_SEQ_DIGPOS 170
  #define HID_SEQ_START  180

  #define VFU_CHECK_LIST_POS(n) \
          { ASSERT( files_list[n] != NULL ); \
            ASSERT( n >= 0 && n < files_count ); }

/*######################################################################*/

  /* file class type */
  class TF
    {
      char*         _name;  /* name with extension ( ref. _fname ) */
      char*         _name_ext;   /* extension ( ref. _fname ) */
      char*         _ext;   /* extension ( ref. _fname ) */
      struct stat   _st;
      char          _type_str[3];
      int           _is_link;
      int           _is_dir;
      mode_str_t    _mode_str;
      fsize_t       _size;
      char*         _view; /* up to screen width */
      int           _color; /* view colors */

      /* !!! this is used when full name required */ 
      /* and this is not thread-safe :) */
      static char   _full_name[MAX_PATH]; 

      void          reset(); /* reset -- NULL all fields */
      void          refresh_view(); /* this is called by view() only! */

    public:
      TF();
      TF( const char* a_name, const struct stat* a_stat, int a_is_link );
      ~TF();

      const char*   full_name( int fix = 0 );
      const char*   name() { ASSERT(_name); return (const char*)_name; }
      const char*   name_ext() { ASSERT(_name_ext); return (const char*)_name_ext; }
      const char*   ext() { ASSERT(_ext); return (const char*)_ext; }
    
      void          set_name( const char* a_new_name );
      const char*   view();
      void          drop_view();

      void          update_stat( const struct stat* a_new_stat,
                                 int a_is_link );
      const char*   type_str() { return (const char*)_type_str; }
      const char*   mode_str() { return (const char*)_mode_str; }
      const struct  stat* st() { return (const struct stat*)&_st; }
      
      void          set_size( fsize_t a_new_size );
      fsize_t       size() { if ( _is_dir && _size == -1 ) return 0; else return _size; }
      
      int           is_link() { return _is_link; }
      int           is_dir() { return _is_dir; }
      
      int           color() { return _color; }
      
      /* public member variables */
      int           sel; /* this saves set/get_sel() functions :) */
      int           x; /* misc used extra field */
    };

/*######################################################################*/
  
  #define WM_NORMAL    0
  #define WM_ARCHIVE   1

  /* work context */
  extern int    work_mode;
  extern String work_path;
  /* archive context */
  extern String archive_name;
  extern String archive_path;
  extern PSZCluster archive_extensions;

  extern String external_panelizer;
  extern PSZCluster list_panelizer;
  
  extern TF*        files_list[MAX_FILES];
  /* file list statistics */
  extern int        files_count;
  extern fsize_t    files_size;
  extern int        sel_count;
  extern fsize_t    sel_size;
  /* file system statistics */
  extern fsize_t    fs_free;
  extern fsize_t    fs_total;
  extern fsize_t    fs_block_size;
  /* index in the files list */
  /* NOTE: following defines are kept for historical reasons :) */
  extern  TScrollPos file_list_index; 
  #define FLI       (file_list_index.pos)
  #define FLP       (file_list_index.page)
  #define PS        (file_list_index.pagesize)
  #define FGO(n)    (file_list_index.gotopos(n))

  /* some world wide variables */
  extern String startup_path;
  extern String home_path;
  extern String tmp_path;
  extern String rc_path;
  
  /* files masks */
  extern String         files_mask;
  extern StrSplitter    files_mask_array;

/*############################################ GLOBAL STRUCTS  #########*/

  extern PSZCluster dir_tree;
  extern int        dir_tree_changed;

  extern PSZCluster file_find_results; // filefind results

  extern String path_bookmarks[10];


/*######################################################################*/

  extern PSZCluster user_externals;
  extern PSZCluster history;
  extern PSZCluster see_filters;
  extern PSZCluster panelizers;

  extern PSZCluster mb; /* menu boxes */
  
  extern StrSplitter trim_tree;

/*############################################ CONFIG SETTINGS #########*/

  extern String ext_colors[16];

  extern String shell_browser;
  extern String shell_editor;
  extern String shell_prog;

  extern String user_id_str;
  extern String group_id_str;
  extern String host_name_str;

  extern String filename_opt;
  extern String filename_conf;
  extern String filename_history;
  extern String filename_tree;
  extern String filename_size_cache;
  extern String filename_ffr; /* file find results */
  extern String filename_atl; /* bad name I know, it stands for
                                 `archive temp list' */

/*######################################################################*/

  extern int do_draw;
  extern int do_draw_status;

/*######################################################################*/

/*
  Message issues
*/
void say1(const char *a_str, int attr = cMESSAGE );
void say2(const char *a_str, int attr = cMESSAGE );
void say2errno();

void say1center(const char *a_str, int attr = cMESSAGE );
void say2center(const char *a_str, int attr = cMESSAGE );

/*
  Main things
*/

void vfu_help();
void vfu_help_cli();

void vfu_init();
void vfu_run();
void vfu_cli();
void vfu_done();
void vfu_signal( int sig );
void vfu_exit_path( const char *a_path );
int vfu_exit( const char* a_path );

void vfu_options();

void vfu_toggle_view_fields( int ch );
/*
  Support op's
*/

void vfu_shell( const char* a_command, const char* a_options );

void vfu_tools();
void vfu_command();
void vfu_file_find( int menu );
void vfu_file_find_results();
void vfu_directories_sizes( int a_which_one );

void vfu_change_file_mask( const char* a_new_mask );

void bookmark_goto( int a_n );
void bookmark_set( int a_n, const char* a_path );
const char* bookmark_get( int a_n );
void bookmark_hookup();

void update_status();

int vfu_user_external_find( int key, const char* ext, const char* type, String *shell_line );
void vfu_user_external_exec( int a_key );
void vfu_user_menu();

void vfu_inc_search();

/*
  Main files op's
*/

int  vfu_edit_attr( char *attrs );
void vfu_edit_entry( );
void vfu_rename_file_in_place();

void vfu_browse( const char* a_fname, int no_filters = 0 );
void vfu_browse_selected_files();
void vfu_edit( const char* a_fname );

void vfu_action_plus( int );
void vfu_action_minus();
void vfu_global_select();
void vfu_sort_menu();

void vfu_read_files_menu();

void vfu_clipboard( int act );

void vfu_jump_to_mountpoint( int all );


#endif//_VFU_H_
