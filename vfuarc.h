/*
 *
 * (c) Vladi Belperchinov-Shabanski "Cade" 1996-2002
 * http://www.biscom.net/~cade  <cade@biscom.net>  <cade@datamax.bg>
 *
 * SEE `README',`LICENSE' OR `COPYING' FILE FOR LICENSE AND OTHER DETAILS!
 *
 * $Id: vfuarc.h,v 1.3 2002/04/14 10:16:28 cade Exp $
 *
 */

#ifndef _VFUARC_H_
#define _VFUARC_H_

#include "vfu.h"

  void vfu_read_archive_files();
  void vfu_browse_archive_file();
  void vfu_extract_files( int one );
  void vfu_user_external_archive_exec( String &shell_line  );

#endif /* _VFUARC_H_ */

/* eof vfuarc.h */
