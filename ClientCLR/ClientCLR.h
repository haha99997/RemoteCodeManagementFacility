#pragma once
/////////////////////////////////////////////////////////////////////
// ClientCLR.h    Main UI for Client, written with WPF             //
// Ver 1.0                                                         //
// Application: CSE687 Pr#4, Spring 2015                           //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013     //
// Author:      Chao Zuo                                           //
/////////////////////////////////////////////////////////////////////

/**
*  Package Operations:
*  -------------------
*  This package is a C++\CLI WPF application.  It
*  provides one class, ClientCLR, derived from System::Windows::Window
*  that is compiled with the /clr option to run in the Common Language
*  Runtime, and another class Channel written in native C++ and compiled
*  as a DLL with no Common Language Runtime support.
*
*  Messages are passed with the help of ISendr and IRecvr, created by
*  ChannelFactory.
*
*  Required Files:
*  ---------------
*  ClientCLR.h, ClientCLR.cpp, MsgMgr.cpp
*
* Build Process:
* --------------
*   devenv RemoteCodeManagementFacility.sln /release rebuild /Project ClientCLR
*
* Maintenance History:
* --------------------
* ver 1.0 : 27 Apr 15
*   - first release
*
*/

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Text;
using namespace System::Windows;
using namespace System::Windows::Input;
using namespace System::Windows::Markup;
using namespace System::Windows::Media;                   // TextBlock formatting
using namespace System::Windows::Controls;                // TabControl
using namespace System::Windows::Controls::Primitives;    // StatusBar
using namespace System::Threading;
using namespace System::Threading::Tasks;
using namespace System::Windows::Threading;
using namespace System::ComponentModel;

#include "../Channel/ChannelFactory.h"
#include <string>
#include <unordered_set>

ref class ClientCLR : Window
{
public:
	ClientCLR();
	~ClientCLR();
private:
	ISendr* sender;							//Send messages
	IRecvr* receiver;						//Receive message
	ISendHandler* sendHandler;				//Provide interface for sending concrete messages
	IRecvDispatcher* recvDispatcher;		//Dispatches messages and send it to concrete classes that process them
	
	// Set up view
	void setUpStatusBar();
	void setUpTabControl();
	void setUpUploadView();
	void setUpFileListView();
	void setUpConnectionView();
	void setUpConnectionViewConnect();
	void setUpConnectionViewListen();
	void setUpTextSearchView();
	void setUpTextSearchViewControlProperty();
	void setUpTextSearchResultView();

	// Event Handler for objects on window
	void clear(Object^ sender, RoutedEventArgs^ args);
	void browseForFolder(Object^ sender, RoutedEventArgs^ args);
	void OnLoaded(Object^ sender, RoutedEventArgs^ args);
	void Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args);
	void OnListenClick(Object^ sender, RoutedEventArgs^ args);
	void OnConnectClick(Object^ sender, RoutedEventArgs^ args);
	void OnCloseClick(Object^ sender, RoutedEventArgs^ args);
	void OnUploadClick(Object^ sender, RoutedEventArgs^ args);
	void OnDownloadClick(Object^ sender, RoutedEventArgs^ args);
	void OnListFileClick(Object^ sender, RoutedEventArgs^ args);
	void OnAddClick(Object^ sender, RoutedEventArgs^ args);
	void OnRemoveClick(Object^ sender, RoutedEventArgs^ args);
	void OnClearClick(Object^ sender, RoutedEventArgs^ args);
	void OnTextSearchClick(Object^ sdr, RoutedEventArgs^ args);

	void getMessage();
	void UpdateFileListBox(String^ filelist);
	void UpdateTextSearchResult(String^ filelist);

	std::string toStdString(String^ pStr);
	String^ toSystemString(std::string& str);
	void setButtonsProperties();

	Thread^ recvThread;

	// Controls for Window

	DockPanel^ hDockPanel = gcnew DockPanel();      // support docking statusbar at bottom
	Grid^ hGrid = gcnew Grid();
	TabControl^ hTabControl = gcnew TabControl();
	TabItem^ hUploadTab = gcnew TabItem();
	TabItem^ hFileListTab = gcnew TabItem();
	TabItem^ hConnectTab = gcnew TabItem();
	TabItem^ hTextSearchTab = gcnew TabItem();
	TabItem^ hTextSearchResultTab = gcnew TabItem();
	StatusBar^ hStatusBar = gcnew StatusBar();
	StatusBarItem^ hStatusBarItem = gcnew StatusBarItem();
	TextBlock^ hStatus = gcnew TextBlock();

	// Controls for SendMessage View

	Grid^ hUploadGrid = gcnew Grid();
	Grid^ hConnectGrid = gcnew Grid();
	Grid^ hTextSearchGrid = gcnew Grid();
	Grid^ hTextSearchResultGrid = gcnew Grid();
	Button^ hListenButton = gcnew Button();
	Button^ hConnectButton = gcnew Button();
	Button^ hCloseButton = gcnew Button();
	Button^ hDownloadButton = gcnew Button();
	Button^ hUploadButton = gcnew Button();
	Button^ hClearButton = gcnew Button();
	TextBlock^ hTextSearchTextBlock = gcnew TextBlock();
	TextBox^ txtBoxListenIp = gcnew TextBox();
	TextBox^ txtBoxListenPort = gcnew TextBox();
	TextBox^ txtBoxIp = gcnew TextBox();
	TextBox^ txtBoxPort = gcnew TextBox();
	ScrollViewer^ hScrollViewer1 = gcnew ScrollViewer();
	StackPanel^ hStackPanelConnect = gcnew StackPanel();
	StackPanel^ hStackPanel1 = gcnew StackPanel();

	// Controls for FileListView View

	Grid^ hFileListGrid = gcnew Grid();
	Forms::FolderBrowserDialog^ hFolderBrowserDialog = gcnew Forms::FolderBrowserDialog();
	Forms::OpenFileDialog^ hOpenFileDialog = gcnew Forms::OpenFileDialog();
	ListBox^ hFileListBox = gcnew ListBox();
	Button^ hDirListButton = gcnew Button();
	Grid^ hGrid2 = gcnew Grid();

	TextBox^ txtBoxDirListPath = gcnew TextBox();
	TextBox^ txtBoxDirListPattern = gcnew TextBox();

	// Controls for text search
	TextBox^ txtBoxTextSearchPath = gcnew TextBox();
	TextBox^ txtBoxTextSearchPattern = gcnew TextBox();
	TextBox^ txtBoxTextSearchServerAddress = gcnew TextBox();
	TextBox^ txtBoxTextSearchThreads = gcnew TextBox();
	TextBox^ txtBoxTextSearchFilePattern = gcnew TextBox();
	ListBox^ hListBoxServersTextSearch = gcnew ListBox();
	Button^ hButtonTextSearchServerAdd = gcnew Button();
	Button^ hButtonTextSearchServerRemove = gcnew Button();
	Button^ hButtonTextSearchServerClear = gcnew Button();
	Button^ hButtonTextSearch = gcnew Button();

	Dictionary<int, System::Diagnostics::Stopwatch^>^ textSearchID = gcnew Dictionary<int, System::Diagnostics::Stopwatch^>();
};

