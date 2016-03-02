#pragma once

/////////////////////////////////////////////////////////////////////
// MsgType.h			Constants								   //
// Ver 1.0                                                         //
// Application: CSE687 Pr#3, Spring 2015                           //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013     //
// Author:      Chao Zuo                                           //
/////////////////////////////////////////////////////////////////////
/*
* Module Description:
* ------------------
*	This module declares constants.

* Required Files:
* ---------------
* None
*
* Build Process:
* --------------
* None
*
* Maintenance History:
* --------------------
* ver 1.0 : 4 Apr 15
*   - first release
*/

#include <string>

const std::string  UPLOAD_FILE = "UPLOAD_FILE";
const std::string  SERVER_UPLOAD_FILE = "SERVER_UPLOAD_FILE";
const std::string  DOWNLOAD_FILE = "DOWNLOAD_FILE";
const std::string  SEARCH_TEXT = "SEARCH_TEXT";
const std::string  FIND_FILES = "FIND_FILES";
const std::string  CONNECT = "CONNECT";
const std::string  CLOSE_CONNECTION = "CLOSE";
const std::string  FEEDBACK_UPLOAD = "FEEDBACK_UPLOAD";
const std::string  FEEDBACK_FILELIST = "FEEDBACK_FILELIST";
const std::string  FEEDBACK_TEXTSEARCH = "FEEDBACK_TEXTSEARCH";
const std::string  QUIT = "QUIT";

const std::string  ATTR_COMMAND = "Command";
const std::string  ATTR_DESTIP = "DestIP";
const std::string  ATTR_DESTPORT = "DestPort";
const std::string  ATTR_SRCIP = "SrcIP";
const std::string  ATTR_SRCPORT = "SrcPort";
const std::string  ATTR_FILENAME = "File";
const std::string  ATTR_BLOCK_SIZE = "BlockSize";
const std::string  ATTR_PATTERN_TEXT = "TextPattern";
const std::string  ATTR_PATTERN_FILE = "FilePattern";
const std::string  ATTR_PATH = "Path";
const std::string  ATTR_UPLOAD_STATE = "UploadState";
const std::string  ATTR_THREAD_COUNT = "Threads";
const std::string  ATTR_SEARCH_TIME = "SearchTime";
const std::string  ATTR_TEXT_SEARCH_ID = "SearchID";

const int CONNECT_TRY = 20;
const int BLOCK_SIZE = 1024;

const std::string  UPLOAD_SUCESS = "Success";
const std::string  UPLOAD_FAIL = "Fail";