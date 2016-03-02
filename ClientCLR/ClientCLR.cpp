/////////////////////////////////////////////////////////////////////
// ClientCLR.cpp    Main UI for Client, written with WPF           //
// Ver 1.0                                                         //
// Application: CSE687 Pr#4, Spring 2015                           //
// Platform:    Lenovo Y40-70, Win 8.1 Pro, Visual Studio 2013     //
// Author:      Chao Zuo                                           //
/////////////////////////////////////////////////////////////////////

#include "ClientCLR.h"
#include <iostream>
#include "../MsgMgr/MsgMgr.h"
#include "../MsgMgr/MsgType.h"
#include <chrono>
#include <ctime>

ClientCLR::ClientCLR()
{
	this->Loaded +=
		gcnew System::Windows::RoutedEventHandler(this, &ClientCLR::OnLoaded);
	this->Closing +=
		gcnew CancelEventHandler(this, &ClientCLR::Unloading);

	hClearButton->Click += gcnew RoutedEventHandler(this, &ClientCLR::clear);
	hListenButton->Click += gcnew RoutedEventHandler(this, &ClientCLR::OnListenClick);
	hConnectButton->Click += gcnew RoutedEventHandler(this, &ClientCLR::OnConnectClick);
	hCloseButton->Click += gcnew RoutedEventHandler(this, &ClientCLR::OnCloseClick);
	hUploadButton->Click += gcnew RoutedEventHandler(this, &ClientCLR::OnUploadClick);
	hDownloadButton->Click += gcnew RoutedEventHandler(this, &ClientCLR::OnDownloadClick);
	hDirListButton->Click += gcnew RoutedEventHandler(this, &ClientCLR::OnListFileClick);
	hButtonTextSearchServerAdd->Click += gcnew RoutedEventHandler(this, &ClientCLR::OnAddClick);
	hButtonTextSearchServerRemove->Click += gcnew RoutedEventHandler(this, &ClientCLR::OnRemoveClick);
	hButtonTextSearchServerClear->Click += gcnew RoutedEventHandler(this, &ClientCLR::OnClearClick);
	hButtonTextSearch->Click += gcnew RoutedEventHandler(this, &ClientCLR::OnTextSearchClick);
	// set Window properties

	this->Title = "Remote Code Management Facility";
	this->Width = 800;
	this->Height = 600;

	// attach dock panel to Window

	this->Content = hDockPanel;
	hDockPanel->Children->Add(hStatusBar);
	hDockPanel->SetDock(hStatusBar, Dock::Bottom);
	hDockPanel->Children->Add(hGrid);

	// setup Window controls and views

	setUpTabControl();
	setUpStatusBar();
	setUpUploadView();
	setUpFileListView();
	setUpConnectionView();
	setUpTextSearchView();
	setUpTextSearchResultView();
}

ClientCLR::~ClientCLR()
{
	delete sender;
	delete receiver;
	delete sendHandler;
	delete recvDispatcher;
	delete textSearchID;
}

void ClientCLR::setUpStatusBar()
{
	hStatusBar->Items->Add(hStatusBarItem);
	hStatus->Text = "very important messages will appear here";
	//status->FontWeight = FontWeights::Bold;
	hStatusBarItem->Content = hStatus;
	hStatusBar->Padding = Thickness(10, 2, 10, 2);
}

void ClientCLR::setUpTabControl()
{
	hGrid->Children->Add(hTabControl);
	hUploadTab->Header = "Upload";
	hFileListTab->Header = "File List & Download";
	hConnectTab->Header = "Connect";
	hTextSearchTab->Header = "Text Search";
	hTextSearchResultTab->Header = "Text Search Result";
	hTabControl->Items->Add(hConnectTab);
	hTabControl->Items->Add(hUploadTab);
	hTabControl->Items->Add(hFileListTab);
	hTabControl->Items->Add(hTextSearchTab);
	hTabControl->Items->Add(hTextSearchResultTab);
}

void ClientCLR::setButtonsProperties()
{
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hUploadGrid->RowDefinitions->Add(hRow2Def);
	hUploadButton->Content = "Upload";
	hUploadButton->IsEnabled = false;
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hUploadButton;
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;
	hStackPanel1->Children->Add(hSpacer);
	hStackPanel1->Children->Add(hBorder3);
	hStackPanel1->Orientation = Orientation::Horizontal;
	hStackPanel1->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hUploadGrid->SetRow(hStackPanel1, 1);
	hUploadGrid->Children->Add(hStackPanel1);
}

void ClientCLR::setUpUploadView()
{
	Console::Write("\n  setting up Upload view");
	hUploadGrid->Margin = Thickness(20);
	hUploadTab->Content = hUploadGrid;
	setButtonsProperties();
}

std::string ClientCLR::toStdString(String^ pStr)
{
	std::string dst;
	for (int i = 0; i < pStr->Length; ++i)
		dst += (char)pStr[i];
	return dst;
}

String^ ClientCLR::toSystemString(std::string& str)
{
	StringBuilder^ pStr = gcnew StringBuilder();
	for (size_t i = 0; i < str.size(); ++i)
		pStr->Append((Char)str[i]);
	return pStr->ToString();
}


void ClientCLR::clear(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  cleared message text");
	hStatus->Text = "Cleared message";
	hTextSearchTextBlock->Text = "";
}

void ClientCLR::setUpFileListView()
{
	hFileListGrid->Margin = Thickness(20);
	hFileListTab->Content = hFileListGrid;
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hFileListGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hFileListBox->SelectionMode = SelectionMode::Single;
	hBorder1->Child = hFileListBox;
	hFileListGrid->SetRow(hBorder1, 0);
	hFileListGrid->Children->Add(hBorder1);
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hFileListGrid->RowDefinitions->Add(hRow2Def);
	StackPanel^ stackPanel = gcnew StackPanel();
	stackPanel->Orientation = Orientation::Horizontal;
	TextBlock^ path = gcnew TextBlock();
	path->Text = "Path on\nserver:";
	stackPanel->Children->Add(path);
	txtBoxDirListPath->Height = 30; txtBoxDirListPath->Width = 100;
	stackPanel->Children->Add(txtBoxDirListPath);
	TextBlock^ space = gcnew TextBlock(); space->Width = 30;
	stackPanel->Children->Add(space);
	TextBlock^ pattern = gcnew TextBlock();
	pattern->Text = "File Patterns\nseperated by \"|\" ";
	stackPanel->Children->Add(pattern);
	txtBoxDirListPattern->Height = 30;
	txtBoxDirListPattern->Width = 100;
	stackPanel->Children->Add(txtBoxDirListPattern);
	space = gcnew TextBlock(); space->Width = 30;
	stackPanel->Children->Add(space);
	hDirListButton->Content = "List files";
	hDirListButton->Height = 30; hDirListButton->Width = 100; hDirListButton->IsEnabled = false;
	stackPanel->Children->Add(hDirListButton);
	space = gcnew TextBlock(); space->Width = 30;
	stackPanel->Children->Add(space);
	hDownloadButton->Content = "Download";
	hDownloadButton->Height = 30; hDownloadButton->Width = 100;
	hDownloadButton->IsEnabled = false;
	stackPanel->Children->Add(hDownloadButton);
	hFileListGrid->SetRow(stackPanel, 1);
	hFileListGrid->Children->Add(stackPanel);
}

void ClientCLR::browseForFolder(Object^ sender, RoutedEventArgs^ args)
{
	std::cout << "\n  Browsing for folder";
	hFileListBox->Items->Clear();
	System::Windows::Forms::DialogResult result;
	result = hFolderBrowserDialog->ShowDialog();
	if (result == System::Windows::Forms::DialogResult::OK)
	{
		String^ path = hFolderBrowserDialog->SelectedPath;
		std::cout << "\n  opening folder \"" << toStdString(path) << "\"";
		array<String^>^ files = System::IO::Directory::GetFiles(path, L"*.*");
		for (int i = 0; i < files->Length; ++i)
			hFileListBox->Items->Add(files[i]);
		array<String^>^ dirs = System::IO::Directory::GetDirectories(path);
		for (int i = 0; i < dirs->Length; ++i)
			hFileListBox->Items->Add(L"<> " + dirs[i]);
	}
}

void ClientCLR::setUpConnectionViewConnect()
{
	hConnectGrid->Margin = Thickness(20);
	hConnectTab->Content = hConnectGrid;

	StackPanel^ stkpnl1 = gcnew StackPanel();
	stkpnl1->Orientation = Orientation::Horizontal;
	TextBlock^ tbIp = gcnew TextBlock();
	TextBlock^ tbPort = gcnew TextBlock();
	tbIp->Text = "Destination IP:";
	tbPort->Text = "Destination Port:";
	txtBoxIp->Width = 120;
	txtBoxIp->Height = 30;
	txtBoxPort->Width = 120;
	txtBoxPort->Height = 30;
	stkpnl1->Children->Add(tbIp);
	stkpnl1->Children->Add(txtBoxIp);
	stkpnl1->Children->Add(tbPort);
	stkpnl1->Children->Add(txtBoxPort);

	StackPanel^ stkpnl2 = gcnew StackPanel();
	stkpnl2->Orientation = Orientation::Vertical;
	hConnectButton->Content = "Connect";
	hConnectButton->IsEnabled = false;
	hConnectButton->Width = 120;
	hConnectButton->Height = 30;

	stkpnl2->Children->Add(hConnectButton);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 30;
	stkpnl2->Children->Add(hSpacer);
	hCloseButton->Content = "Close";
	hCloseButton->IsEnabled = false;
	hCloseButton->Width = 120;
	hCloseButton->Height = 30;
	stkpnl2->Children->Add(hCloseButton);
	hSpacer = gcnew TextBlock();
	hSpacer->Width = 30;
	stkpnl1->Children->Add(hSpacer);
	stkpnl1->Children->Add(stkpnl2);
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(75);
	hConnectGrid->RowDefinitions->Add(hRow1Def);
	hConnectGrid->SetRow(stkpnl1, 1);
	hConnectGrid->Children->Add(stkpnl1);
}

void ClientCLR::setUpConnectionViewListen()
{
	StackPanel^ stkpnl1 = gcnew StackPanel();
	stkpnl1->Orientation = Orientation::Horizontal;
	TextBlock^ tbIp = gcnew TextBlock();
	TextBlock^ tbPort = gcnew TextBlock();
	tbIp->Text = "Listen IP:";
	tbPort->Text = "Listen Port:";
	txtBoxListenIp->Width = 120;
	txtBoxListenIp->Height = 30;
	txtBoxListenIp->Text = "127.0.0.1";
	txtBoxListenPort->Width = 120;
	txtBoxListenPort->Height = 30;
	txtBoxListenPort->Text = "9070";

	stkpnl1->Children->Add(tbIp);
	stkpnl1->Children->Add(txtBoxListenIp);
	stkpnl1->Children->Add(tbPort);
	stkpnl1->Children->Add(txtBoxListenPort);

	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 30;
	stkpnl1->Children->Add(hSpacer);

	hListenButton->Width = 120;
	hListenButton->Height = 30;
	hListenButton->Content = "Listen";
	stkpnl1->Children->Add(hListenButton);

	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(75);
	hConnectGrid->RowDefinitions->Add(hRow1Def);
	hConnectGrid->SetRow(stkpnl1, 0);
	hConnectGrid->Children->Add(stkpnl1);
}

void ClientCLR::setUpConnectionView()
{
	Console::Write("\n  setting up Connection view");
	setUpConnectionViewListen();
	setUpConnectionViewConnect();
}

void ClientCLR::setUpTextSearchViewControlProperty()
{
	hTextSearchGrid->Margin = Thickness(20);
	hTextSearchTab->Content = hTextSearchGrid;
	txtBoxTextSearchServerAddress->Width = 200;
	txtBoxTextSearchServerAddress->Height = 20;
	hButtonTextSearchServerAdd->Height = 20;
	hButtonTextSearchServerAdd->Content = "Add";
	hListBoxServersTextSearch->Height = 300;
	hButtonTextSearchServerRemove->Height = 20;
	hButtonTextSearchServerRemove->Content = "Remove";
	hButtonTextSearchServerClear->Height = 20;
	hButtonTextSearchServerClear->Content = "Clear";
	txtBoxTextSearchPath->Width = 200;
	txtBoxTextSearchPath->Height = 30;
	txtBoxTextSearchFilePattern->Width = 200;
	txtBoxTextSearchFilePattern->Height = 30;
	txtBoxTextSearchPattern->Width = 200;
	txtBoxTextSearchPattern->Height = 30;
	txtBoxTextSearchThreads->Width = 200;
	txtBoxTextSearchThreads->Height = 30;
	hButtonTextSearch->Width = 200;
	hButtonTextSearch->Height = 30;
	hButtonTextSearch->Content = "Text Search";
	hButtonTextSearch->IsEnabled = false;
}

void ClientCLR::setUpTextSearchView()
{
	setUpTextSearchViewControlProperty();
	array<TextBlock^>^ space = gcnew array<TextBlock^>(3);
	for (int i = 0; i < 3; ++i) {
		space[i] = gcnew TextBlock();
		space[i]->Width = 20;  space[i]->Height = 20;
	}
	array<StackPanel^>^ stackPanel = gcnew array<StackPanel^>(5);
	for (int i = 0; i < 5; ++i){
		stackPanel[i] = gcnew StackPanel;
		stackPanel[i]->Orientation = Orientation::Horizontal;
	}
	stackPanel[0]->Orientation = Orientation::Vertical;
	TextBlock^ svrAddr = gcnew TextBlock();
	svrAddr->Text = "Server Address (IP:Port)";
	stackPanel[0]->Children->Add(svrAddr);
	stackPanel[2]->Children->Add(txtBoxTextSearchServerAddress);
	stackPanel[2]->Children->Add(space[2]);
	stackPanel[2]->Children->Add(hButtonTextSearchServerAdd);
	stackPanel[0]->Children->Add(stackPanel[2]);
	stackPanel[0]->Children->Add(hListBoxServersTextSearch);
	stackPanel[1]->Children->Add(hButtonTextSearchServerRemove);
	stackPanel[1]->Children->Add(space[1]);
	stackPanel[1]->Children->Add(hButtonTextSearchServerClear);
	stackPanel[0]->Children->Add(stackPanel[1]);
	TextBlock^ path = gcnew TextBlock(); path->Text = "Path";
	TextBlock^ file_pattern = gcnew TextBlock(); file_pattern->Text = "File Pattern\nPatterns seperated by \"|\"";
	TextBlock^ pattern = gcnew TextBlock(); pattern->Text = "Pattern(regex supported)";
	TextBlock^ threads = gcnew TextBlock(); threads->Text = "Thread number of text search";
	stackPanel[3]->Orientation = Orientation::Vertical;
	stackPanel[3]->Children->Add(path);
	stackPanel[3]->Children->Add(txtBoxTextSearchPath);
	stackPanel[3]->Children->Add(file_pattern);
	stackPanel[3]->Children->Add(txtBoxTextSearchFilePattern);
	stackPanel[3]->Children->Add(pattern);
	stackPanel[3]->Children->Add(txtBoxTextSearchPattern);
	stackPanel[3]->Children->Add(threads);
	stackPanel[3]->Children->Add(txtBoxTextSearchThreads);
	stackPanel[3]->Children->Add(space[0]);
	stackPanel[3]->Children->Add(hButtonTextSearch);
	ColumnDefinition^ hRow1Def = gcnew ColumnDefinition();
	hTextSearchGrid->ColumnDefinitions->Add(hRow1Def);
	hTextSearchGrid->SetColumn(stackPanel[0], 0); hTextSearchGrid->Children->Add(stackPanel[0]);
	ColumnDefinition^ hRow2Def = gcnew ColumnDefinition();
	hTextSearchGrid->ColumnDefinitions->Add(hRow2Def);
	hTextSearchGrid->SetColumn(stackPanel[3], 1); hTextSearchGrid->Children->Add(stackPanel[3]);
}

void ClientCLR::setUpTextSearchResultView()
{
	hTextSearchResultGrid->Margin = Thickness(20);
	hTextSearchResultTab->Content = hTextSearchResultGrid;
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hTextSearchTextBlock;
	hTextSearchTextBlock->Padding = Thickness(15);
	hTextSearchTextBlock->Text = "";
	hTextSearchTextBlock->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hTextSearchTextBlock->FontWeight = FontWeights::Bold;
	hTextSearchTextBlock->FontSize = 16;
	hScrollViewer1->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hScrollViewer1->Content = hBorder1;
	RowDefinition^ rd = gcnew RowDefinition();
	hTextSearchResultGrid->RowDefinitions->Add(rd);
	hTextSearchResultGrid->SetRow(hScrollViewer1, 0);
	hTextSearchResultGrid->Children->Add(hScrollViewer1);

	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hClearButton->Content = "Clear";
	hBorder2->Child = hClearButton;
	rd = gcnew RowDefinition();
	rd->Height = GridLength(35);
	hTextSearchResultGrid->RowDefinitions->Add(rd);
	hTextSearchResultGrid->SetRow(hBorder2, 1);
	hTextSearchResultGrid->Children->Add(hBorder2);
}

void ClientCLR::getMessage()
{
	MsgMgr msgmgr;
	while (true)
	{
		std::string msg = recvDispatcher->getMessage();
		System::Console::WriteLine(toSystemString(msg));
		msgmgr.ParseMessage(msg);
		std::string command = msgmgr.GetAttrbVal(ATTR_COMMAND);
		if (command == FEEDBACK_UPLOAD)
		{
			MessageBox::Show(toSystemString("Upload:" + msgmgr.GetAttrbVal(ATTR_FILENAME) + " " + msgmgr.GetAttrbVal(ATTR_UPLOAD_STATE)));
		}
		else if (command == FEEDBACK_FILELIST)
		{
			String^ body = toSystemString(msgmgr.GetBody());
			Action<String^>^ act = gcnew Action<String^>(this, &ClientCLR::UpdateFileListBox);
			Dispatcher->Invoke(act, body);
		}
		else if (command == SERVER_UPLOAD_FILE)
		{
			MessageBox::Show(toSystemString("Download:" + msgmgr.GetAttrbVal(ATTR_FILENAME) + " " + msgmgr.GetAttrbVal(ATTR_UPLOAD_STATE)));
		}
		else if (command == FEEDBACK_TEXTSEARCH)
		{
			int id = std::stoi(msgmgr.GetAttrbVal(ATTR_TEXT_SEARCH_ID));
			textSearchID[id]->Stop();
			MessageBox::Show("Finished Text Search on " + toSystemString(msgmgr.GetAttrbVal(ATTR_SRCIP) + ":" + msgmgr.GetAttrbVal(ATTR_SRCPORT)) +
				"\nElapsed time: " + textSearchID[id]->ElapsedMilliseconds / 1000.0 + "s");
			textSearchID->Remove(id);
			Action<String^>^ act = gcnew Action<String^>(this, &ClientCLR::UpdateTextSearchResult);
			Dispatcher->Invoke(act, toSystemString(msg));
		}
	}
}

void ClientCLR::UpdateTextSearchResult(String^ filelist)
{
	MsgMgr msgmgr;
	msgmgr.ParseMessage(toStdString(filelist));
	String^ ip = toSystemString(msgmgr.GetAttrbVal(ATTR_SRCIP));
	String^ port = toSystemString(msgmgr.GetAttrbVal(ATTR_SRCPORT));
	String^ time = toSystemString(msgmgr.GetAttrbVal(ATTR_SEARCH_TIME));
	hTextSearchTextBlock->Text = hTextSearchTextBlock->Text + "\nResult from server: " + ip + ":" + port + "\nTime spent on server:" + time + "\n";
	std::string filelist2 = toStdString(filelist);
	int start = 0;
	int end = filelist2.find("</File>");
	while (end != -1)
	{
		std::string temp = filelist2.substr(start, end - start);
		int start2 = temp.find("<File>");
		int end2 = temp.length() - 1;
		start2 = start2 + 6;
		while (temp[start2] == 10 || temp[start2] == 32)
			++start2;
		while (temp[end2] == 10 || temp[end2] == 32)
			--end2;
		std::string result = temp.substr(start2, end2 - start2 + 1);
		hTextSearchTextBlock->Text = hTextSearchTextBlock->Text + toSystemString(result) + "\n";
		start = end + 7;
		end = filelist2.find("</File>", start);
	}
	hStatus->Text = "Text Search Result Received";
}

void ClientCLR::UpdateFileListBox(String^ filelist)
{
	hFileListBox->Items->Clear();
	std::string filelist2 = toStdString(filelist);
	int start = 0;
	int end = filelist2.find("</File>");
	while (end != -1)
	{
		std::string temp = filelist2.substr(start, end - start);
		int start2 = temp.find("<File>");
		int end2 = temp.length() - 1;
		start2 = start2 + 6;
		while (temp[start2] == 10 || temp[start2] == 32)
			++start2;
		while (temp[end2] == 10 || temp[end2] == 32)
			--end2;
		std::string result = temp.substr(start2, end2 - start2 + 1);
		hFileListBox->Items->Add(toSystemString(result));
		start = end + 7;
		end = filelist2.find("</File>", start);
	}
	hStatus->Text = "File List Received";
}

void ClientCLR::OnLoaded(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  Window loaded");
}
void ClientCLR::Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args)
{
	Console::Write("\n  Window closing");
	if (this->sender != nullptr)
		this->sender->sendMessage(ATTR_COMMAND + ":" + QUIT);
	Thread::Sleep(200);
}

void ClientCLR::OnListenClick(Object^ sdr, RoutedEventArgs^ args)
{
	hListenButton->IsEnabled = false;
	hConnectButton->IsEnabled = true;
	hButtonTextSearch->IsEnabled = true;
	std::string ip = toStdString(txtBoxListenIp->Text);
	std::string port = toStdString(txtBoxListenPort->Text);
	ChannelFactory* factory = new ChannelFactory;
	sender = factory->CreateSender(ip, std::stoul(port));
	receiver = factory->CreateReceiver(std::stoul(port), ChannelFactory::IPV4);
	sendHandler = factory->CreateSendHandler(*sender);
	recvDispatcher = factory->CreateRecvDispatcher(*sender, *receiver);
	delete factory;

	recvThread = gcnew Thread(gcnew ThreadStart(this, &ClientCLR::getMessage));
	recvThread->Start();
}

void ClientCLR::OnConnectClick(Object^ sender, RoutedEventArgs^ args)
{
	auto IP = toStdString(txtBoxIp->Text);
	auto Port = toStdString(txtBoxPort->Text);
	if (IP == "" || Port == "")
	{
		MessageBox::Show("Invalid arguments");
		return;
	}
	hCloseButton->IsEnabled = true;
	hDirListButton->IsEnabled = true;
	hDownloadButton->IsEnabled = true;
	hUploadButton->IsEnabled = true;
	hConnectButton->IsEnabled = false;
	sendHandler->invoke(CONNECT, IP + ":" + Port);
	hStatus->Text = "Connected to " + txtBoxIp->Text + ":" + txtBoxPort->Text;
}

void ClientCLR::OnCloseClick(Object^ sender, RoutedEventArgs^ args)
{
	hCloseButton->IsEnabled = false;
	hDirListButton->IsEnabled = false;
	hDownloadButton->IsEnabled = false;
	hUploadButton->IsEnabled = false;
	hConnectButton->IsEnabled = true;
	sendHandler->invoke(CLOSE_CONNECTION, "");
	hStatus->Text = "Connection closed";
}

void ClientCLR::OnUploadClick(Object^ sender, RoutedEventArgs^ args)
{
	hOpenFileDialog->Filter = "All Files|*.*";
	if (hOpenFileDialog->ShowDialog() != Forms::DialogResult::OK)
	{
		return;
	}
	sendHandler->invoke(UPLOAD_FILE, toStdString(hOpenFileDialog->FileName));
	hStatus->Text = "Uploading " + hOpenFileDialog->FileName;
}

void ClientCLR::OnListFileClick(Object^ sender, RoutedEventArgs^ args)
{
	std::string path = toStdString(txtBoxDirListPath->Text);
	std::string patterns = toStdString(txtBoxDirListPattern->Text);
	MsgMgr msgmgr;
	msgmgr.AddAttrb(ATTR_PATH, path);
	msgmgr.AddAttrb(ATTR_PATTERN_FILE, patterns);
	sendHandler->invoke(FIND_FILES, msgmgr.CreateMessage());
	hStatus->Text = "Requesting File List";
}

void ClientCLR::OnDownloadClick(Object^ sender, RoutedEventArgs^ args)
{
	auto result = hFileListBox->SelectedItem;
	if (result->ToString() == "")
		MessageBox::Show("Please select files to download.");
	hStatus->Text = "Downloading";
	sendHandler->invoke(DOWNLOAD_FILE, toStdString(result->ToString()));
}

void ClientCLR::OnAddClick(Object^ sender, RoutedEventArgs^ args)
{
	if (txtBoxTextSearchServerAddress->Text == "")
		return;
	if (hListBoxServersTextSearch->Items->Contains(txtBoxTextSearchServerAddress->Text))
		return;
	hListBoxServersTextSearch->Items->Add(txtBoxTextSearchServerAddress->Text);
}

void ClientCLR::OnRemoveClick(Object^ sender, RoutedEventArgs^ args)
{
	hListBoxServersTextSearch->Items->RemoveAt(hListBoxServersTextSearch->SelectedIndex);
}

void ClientCLR::OnClearClick(Object^ sender, RoutedEventArgs^ args)
{
	hListBoxServersTextSearch->Items->Clear();
}

void ClientCLR::OnTextSearchClick(Object^ sdr, RoutedEventArgs^ args)
{
	std::string path = toStdString(txtBoxTextSearchPath->Text);
	std::string pattern = toStdString(txtBoxTextSearchPattern->Text);
	std::string file_pattern = toStdString(txtBoxTextSearchFilePattern->Text);
	std::string thread_cnt = toStdString(txtBoxTextSearchThreads->Text);
	MsgMgr msgmgr;
	msgmgr.AddAttrb(ATTR_COMMAND, SEARCH_TEXT);
	msgmgr.AddAttrb(ATTR_PATH, path);
	msgmgr.AddAttrb(ATTR_PATTERN_FILE, file_pattern);
	msgmgr.AddAttrb(ATTR_PATTERN_TEXT, pattern);
	msgmgr.AddAttrb(ATTR_THREAD_COUNT, thread_cnt);
	auto servers = hListBoxServersTextSearch->Items;
	int id = 0;
	for each(auto item in servers)
	{
		std::string server = toStdString(item->ToString());
		int index = server.find_last_of(":");
		std::string ip = server.substr(0, index);
		std::string port = server.substr(index + 1);
		msgmgr.AddAttrb(ATTR_DESTIP, ip);
		msgmgr.AddAttrb(ATTR_DESTPORT, port);
		msgmgr.AddAttrb(ATTR_TEXT_SEARCH_ID, std::to_string(id));
		System::Diagnostics::Stopwatch^ watch = System::Diagnostics::Stopwatch::StartNew();
		textSearchID[id++] = watch;
		sender->sendMessage(msgmgr.CreateMessage());
		msgmgr.RemoveAttrb(ATTR_DESTIP);
		msgmgr.RemoveAttrb(ATTR_DESTPORT);
		msgmgr.RemoveAttrb(ATTR_TEXT_SEARCH_ID);
	}
	if (hCloseButton->IsEnabled)
		OnCloseClick(sdr, args);
	hStatus->Text = "Doing Text Search";
}

[STAThread]
//int _stdcall WinMain()
int main(array<System::String^>^ args)
{
	Console::WriteLine(L"\n Starting WPFCppCliDemo");

	Application^ app = gcnew Application();
	app->Run(gcnew ClientCLR());
	Console::WriteLine(L"\n\n");
}