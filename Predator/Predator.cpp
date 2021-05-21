#include <urlmon.h>
#include <stdio.h>
#include "Windows.h"
#include <shellapi.h>
#include <tchar.h>
#include <fstream>
#include <wininet.h>
#include <iostream>
#include <string>
#include <random>
#include <locale>
#include <codecvt>
#include <msclr/marshal.h>
#include <WinUser.h>
#include <iomanip>
#include "base64.h"

#using <AuthGG.dll>
#using <Handler.dll>
using namespace System;

#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "shell32.lib")

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

bool IsDLLValid;
void CheckDLLS()
{
	System::String^ AuthGGMD5 = "40182c14facac046a2d4e9141e615c80";
	System::String^ NewtonMD5 = "a907da945d77abd7d7ad83bbcbd67b68";
	Handler::Handler hashcheck;
	System::String^ AuthDLLResult   = (System::String^)hashcheck.CalculateMD5("AuthGG.dll");
	System::String^ NewtonDLLResult = (System::String^)hashcheck.CalculateMD5("Newtonsoft.Json.dll");
	if (AuthDLLResult != AuthGGMD5 || NewtonDLLResult != NewtonMD5)
	{
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << "Hashcheck has failed!" << std::endl;
		IsDLLValid = false;
	}
	else
	{
		IsDLLValid = true;
	}

}

std::string  ChangeToConst(System::String^ oParameter)
{
	msclr::interop::marshal_context oMarshalContext;

	std::string pParameter = oMarshalContext.marshal_as<const char*>(oParameter);

	return pParameter;

}

void KillProcessById(DWORD pid) {
	HANDLE hnd;
	hnd = OpenProcess(SYNCHRONIZE | PROCESS_TERMINATE, TRUE, pid);
	TerminateProcess(hnd, 0);
}

bool FileExists(const std::string& filename) {
	std::ifstream ifile(filename.c_str());
	return (bool)ifile;

}

std::wstring String2LPCWSTR(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

std::string random_string(size_t length)
{
	auto randchar = []() -> char
	{
		const char charset[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		const size_t max_index = (sizeof(charset) - 1);
		return charset[rand() % max_index];
	};
	std::string str(length, 0);
	std::generate_n(str.begin(), length, randchar);
	return str;
}

void randomTitle()
{
	std::string RandomTitle = random_string(15);
	std::wstring temp_title = String2LPCWSTR(RandomTitle);
	LPCWSTR title = temp_title.c_str();

	const LPCWSTR titleReal = title;

	SetConsoleTitle(titleReal);
}

void menu()
{
	randomTitle();
	SetConsoleTextAttribute(hConsole, 12);
	std::wcout << L"+-----------------------------------------------------+" << std::endl;
	std::wcout << L"|	  ____                __      __              |" << std::endl;
	std::wcout << L"|   _  __/ __ \\________  ____/ /___ _/ /_____   ____  |" << std::endl;
	std::wcout << L"|  | |/_/ /_/ / ___/ _ \\/ __  / __ `/ __/ __ \\/ ___/  |" << std::endl;
	std::wcout << L"| _>  </ ____/ /  /  __/ /_/ / /_/ / /_/ /_/ / /      |" << std::endl;
	std::wcout << L"|/_/|_/_/   /_/   \\___/\\__,_/\\__,_/\\__/\\____/_/ 4.3.1 |    " << std::endl;
	std::wcout << L"+-----------------------------------------------------+\n" << std::endl;
	SetConsoleTextAttribute(hConsole, 15);
}

void UpdateAndSpoof()
{
	randomTitle();
	system("cls");
	menu();
	printf("Getting latest version...\n");


	// Clean traces
	if (FileExists("C:\\Windows\\apppatch\\Custom\\Custom64\\vcredist_x86.exe") == 1)
	{
		std::remove("C:\\Windows\\apppatch\\Custom\\Custom64\\vcredist_x86.exe");
	}

	if (FileExists("C:\\Windows\\apppatch\\Custom\\Custom64\\CCleaner64.exe") == 1)
	{
		std::remove("C:\\Windows\\apppatch\\Custom\\Custom64\\CCleaner64.exe");
	}

	if (FileExists("C:\\Windows\\apppatch\\Custom\\Custom64\\RuntimeBroker.exe") == 1)
	{
		std::remove("C:\\Windows\\apppatch\\Custom\\Custom64\\RuntimeBroker.exe");
	}

	if (FileExists("C:\\Windows\\apppatch\\Custom\\Custom64\\VSTXRAlD.sys") == 1)
	{
		std::remove("C:\\Windows\\apppatch\\Custom\\Custom64\\VSTXRAlD.sys");
	}

	// BASE64 "obfuscated" strings
	const std::string o_mpr = "aHR0cDovL3d3dy5kZXYtc3RvcmUuZnIvREp4SkR6aTJoajIzSkREODRqay9BL0xhdGVzdC5leGU=";			// BASE64 - MAPPER
	const std::string o_drv = "aHR0cDovL3d3dy5kZXYtc3RvcmUuZnIvREp4SkR6aTJoajIzSkREODRqay9CL1ZTVFhSQWxELnN5cw==";		// BASE64 - DRIVER
	const std::string o_mob = "aHR0cDovL3d3dy5kZXYtc3RvcmUuZnIvREp4SkR6aTJoajIzSkREODRqay9DL0Jpb3NTcG9vZmVyLmV4ZQ==";	// BASE64 - MOBO

	// Deobfuscate and convert them to LPCWSTR
	std::wstring temp_mpr = String2LPCWSTR(base64_decode(o_mpr));
	std::wstring temp_drv = String2LPCWSTR(base64_decode(o_drv));
	std::wstring temp_mob = String2LPCWSTR(base64_decode(o_mob));

	LPCWSTR u_mpr = temp_mpr.c_str();
	LPCWSTR u_drv = temp_drv.c_str();
	LPCWSTR u_mob = temp_mob.c_str();

	// URLs
	const LPCWSTR url_mpr = u_mpr;
	const LPCWSTR url_drv = u_drv;
	const LPCWSTR url_mob = u_mob;

	// Output paths
	const LPCWSTR out_mpr = L"C:\\Windows\\apppatch\\Custom\\Custom64\\vcredist_x86.exe";
	const LPCWSTR out_drv = L"C:\\Windows\\apppatch\\Custom\\Custom64\\VSTXRAlD.sys";
	const LPCWSTR out_mob = L"C:\\Windows\\apppatch\\Custom\\Custom64\\RuntimeBroker.exe";

	// Delete IE Cache
	DeleteUrlCacheEntry(url_mpr);
	DeleteUrlCacheEntry(url_drv);
	DeleteUrlCacheEntry(url_mob);

	// Download the files
	URLDownloadToFile(NULL, url_mpr, out_mpr, 0, NULL);
	URLDownloadToFile(NULL, url_drv, out_drv, 0, NULL);
	URLDownloadToFile(NULL, url_mob, out_mob, 0, NULL);

	// Run the HWID Spoofer AND the Mobo Spoofer
	system("C:\\Windows\\apppatch\\Custom\\Custom64\\vcredist_x86.exe C:\\Windows\\apppatch\\Custom\\Custom64\\VSTXRAlD.sys");
	system("cls");
	menu();
	//ShellExecute(NULL, L"runas", L"C:\\Windows\\apppatch\\Custom\\Custom64\\vcredist_x86.exe", L"C:\\Windows\\apppatch\\Custom\\Custom64\\VSTXRAlD.sys", NULL, SW_HIDE); //SW_HIDE |HIDE WINDOW AND DO SHIT IN BACKGROUD || SW_SHOWNORMAL | SHOW WINDOW AND DO SHIT IN FOREGROUND
	ShellExecute(NULL, L"runas", L"C:\\Windows\\apppatch\\Custom\\Custom64\\RuntimeBroker.exe", NULL, NULL, SW_HIDE);

	// Delete traces
	if (FileExists("C:\\Windows\\apppatch\\Custom\\Custom64\\vcredist_x86.exe") == 1)
	{
		std::remove("C:\\Windows\\apppatch\\Custom\\Custom64\\vcredist_x86.exe");
	}

	if (FileExists("C:\\Windows\\apppatch\\Custom\\Custom64\\CCleaner64.exe") == 1)
	{
		std::remove("C:\\Windows\\apppatch\\Custom\\Custom64\\CCleaner64.exe");
	}

	if (FileExists("C:\\Windows\\apppatch\\Custom\\Custom64\\RuntimeBroker.exe") == 1)
	{
		std::remove("C:\\Windows\\apppatch\\Custom\\Custom64\\RuntimeBroker.exe");
	}

	if (FileExists("C:\\Windows\\apppatch\\Custom\\Custom64\\VSTXRAlD.sys") == 1)
	{
		std::remove("C:\\Windows\\apppatch\\Custom\\Custom64\\VSTXRAlD.sys");
	}

	// Exit
	SetConsoleTextAttribute(hConsole, 2);
	printf("Spoofed HWID and BIOS. Wait 15s before opening the game.");
	Sleep(3000);

	DWORD pid = GetCurrentProcessId();
	KillProcessById(pid);
}

// Getting latest cleaner
void Clean()
{
	randomTitle();
	system("cls");
	menu();
	printf("Getting latest version...\n");

	// Clean traces
	if (FileExists("C:\\Windows\\apppatch\\Custom\\Custom64\\vcredist_x86.exe") == 1)
	{
		std::remove("C:\\Windows\\apppatch\\Custom\\Custom64\\vcredist_x86.exe");
	}

	if (FileExists("C:\\Windows\\apppatch\\Custom\\Custom64\\CCleaner64.exe") == 1)
	{
		std::remove("C:\\Windows\\apppatch\\Custom\\Custom64\\CCleaner64.exe");
	}

	if (FileExists("C:\\Windows\\apppatch\\Custom\\Custom64\\RuntimeBroker.exe") == 1)
	{
		std::remove("C:\\Windows\\apppatch\\Custom\\Custom64\\RuntimeBroker.exe");
	}

	if (FileExists("C:\\Windows\\apppatch\\Custom\\Custom64\\VSTXRAlD.sys") == 1)
	{
		std::remove("C:\\Windows\\apppatch\\Custom\\Custom64\\VSTXRAlD.sys");
	}

	// Obfuscated string
	const std::string o_cln = "aHR0cDovL3d3dy5kZXYtc3RvcmUuZnIvREp4SkR6aTJoajIzSkREODRqay9EL0NsZWFuZXIuZXhl";	//BASE64 - Cleaner

	// Convert and deobfuscate
	std::wstring temp_cln = String2LPCWSTR(base64_decode(o_cln));

	LPCWSTR u_cln = temp_cln.c_str();

	const LPCWSTR url_cln = u_cln;

	const LPCWSTR out_cln = L"C:\\Windows\\apppatch\\Custom\\Custom64\\CCleaner64.exe";

	// Delete cache
	DeleteUrlCacheEntry(url_cln);

	// Download
	URLDownloadToFile(NULL, url_cln, out_cln, 0, NULL);

	// Execute
	ShellExecute(NULL, L"runas", L"C:\\Windows\\apppatch\\Custom\\Custom64\\CCleaner64.exe", NULL, NULL, SW_HIDE);

	// Remove traces
	if (FileExists("C:\\Windows\\apppatch\\Custom\\Custom64\\vcredist_x86.exe") == 1)
	{
		std::remove("C:\\Windows\\apppatch\\Custom\\Custom64\\vcredist_x86.exe");
	}

	if (FileExists("C:\\Windows\\apppatch\\Custom\\Custom64\\CCleaner64.exe") == 1)
	{
		std::remove("C:\\Windows\\apppatch\\Custom\\Custom64\\CCleaner64.exe");
	}

	if (FileExists("C:\\Windows\\apppatch\\Custom\\Custom64\\RuntimeBroker.exe") == 1)
	{
		std::remove("C:\\Windows\\apppatch\\Custom\\Custom64\\RuntimeBroker.exe");
	}

	if (FileExists("C:\\Windows\\apppatch\\Custom\\Custom64\\VSTXRAlD.sys") == 1)
	{
		std::remove("C:\\Windows\\apppatch\\Custom\\Custom64\\VSTXRAlD.sys");
	}

	// Exit
	SetConsoleTextAttribute(hConsole, 2);
	printf("Cleaned Traces. Wait 15s before spoofing.");
	Sleep(2000);

	DWORD pid = GetCurrentProcessId();
	KillProcessById(pid);
}

void CleanAndSpoof()
{
	randomTitle();
	system("cls");
	menu();
	printf("Getting latest version...\n");


	// Clean traces
	if (FileExists("C:\\Windows\\apppatch\\Custom\\Custom64\\vcredist_x86.exe") == 1)
	{
		std::remove("C:\\Windows\\apppatch\\Custom\\Custom64\\vcredist_x86.exe");
	}

	if (FileExists("C:\\Windows\\apppatch\\Custom\\Custom64\\CCleaner64.exe") == 1)
	{
		std::remove("C:\\Windows\\apppatch\\Custom\\Custom64\\CCleaner64.exe");
	}

	if (FileExists("C:\\Windows\\apppatch\\Custom\\Custom64\\RuntimeBroker.exe") == 1)
	{
		std::remove("C:\\Windows\\apppatch\\Custom\\Custom64\\RuntimeBroker.exe");
	}

	if (FileExists("C:\\Windows\\apppatch\\Custom\\Custom64\\VSTXRAlD.sys") == 1)
	{
		std::remove("C:\\Windows\\apppatch\\Custom\\Custom64\\VSTXRAlD.sys");
	}

	// BASE64 "obfuscated" strings
	const std::string o_mpr = "aHR0cDovL3d3dy5kZXYtc3RvcmUuZnIvREp4SkR6aTJoajIzSkREODRqay9BL0xhdGVzdC5leGU=";			// BASE64 - MAPPER
	const std::string o_cln = "aHR0cDovL3d3dy5kZXYtc3RvcmUuZnIvREp4SkR6aTJoajIzSkREODRqay9EL0NsZWFuZXIuZXhl";			// BASE64 - Cleaner
	const std::string o_drv = "aHR0cDovL3d3dy5kZXYtc3RvcmUuZnIvREp4SkR6aTJoajIzSkREODRqay9CL1ZTVFhSQWxELnN5cw==";		// BASE64 - DRIVER
	const std::string o_mob = "aHR0cDovL3d3dy5kZXYtc3RvcmUuZnIvREp4SkR6aTJoajIzSkREODRqay9DL0Jpb3NTcG9vZmVyLmV4ZQ==";	// BASE64 - MOBO

	// Deobfuscate and convert them to LPCWSTR
	std::wstring temp_mpr = String2LPCWSTR(base64_decode(o_mpr));
	std::wstring temp_cln = String2LPCWSTR(base64_decode(o_cln));
	std::wstring temp_drv = String2LPCWSTR(base64_decode(o_drv));
	std::wstring temp_mob = String2LPCWSTR(base64_decode(o_mob));

	LPCWSTR u_mpr = temp_mpr.c_str();
	LPCWSTR u_cln = temp_cln.c_str();
	LPCWSTR u_drv = temp_drv.c_str();
	LPCWSTR u_mob = temp_mob.c_str();

	// URLs
	const LPCWSTR url_mpr = u_mpr;
	const LPCWSTR url_cln = u_cln;
	const LPCWSTR url_drv = u_drv;
	const LPCWSTR url_mob = u_mob;

	// Output paths
	const LPCWSTR out_mpr = L"C:\\Windows\\apppatch\\Custom\\Custom64\\vcredist_x86.exe";
	const LPCWSTR out_cln = L"C:\\Windows\\apppatch\\Custom\\Custom64\\CCleaner64.exe";
	const LPCWSTR out_drv = L"C:\\Windows\\apppatch\\Custom\\Custom64\\VSTXRAlD.sys";
	const LPCWSTR out_mob = L"C:\\Windows\\apppatch\\Custom\\Custom64\\RuntimeBroker.exe";

	// Delete IE Cache
	DeleteUrlCacheEntry(url_mpr);
	DeleteUrlCacheEntry(url_cln);
	DeleteUrlCacheEntry(url_drv);
	DeleteUrlCacheEntry(url_mob);

	// Download the files
	URLDownloadToFile(NULL, url_mpr, out_mpr, 0, NULL);
	URLDownloadToFile(NULL, url_cln, out_cln, 0, NULL);
	URLDownloadToFile(NULL, url_drv, out_drv, 0, NULL);
	URLDownloadToFile(NULL, url_mob, out_mob, 0, NULL);

	// Run the HWID Spoofer AND the Mobo Spoofer
	ShellExecute(NULL, L"runas", L"C:\\Windows\\apppatch\\Custom\\Custom64\\CCleaner64.exe", NULL, NULL, SW_HIDE);
	system("C:\\Windows\\apppatch\\Custom\\Custom64\\vcredist_x86.exe C:\\Windows\\apppatch\\Custom\\Custom64\\VSTXRAlD.sys");
	system("cls");
	menu();

	ShellExecute(NULL, L"runas", L"C:\\Windows\\apppatch\\Custom\\Custom64\\RuntimeBroker.exe", NULL, NULL, SW_HIDE);
	//ShellExecute(NULL, L"runas", L"C:\\Windows\\apppatch\\Custom\\Custom64\\vcredist_x86.exe", L"C:\\Windows\\apppatch\\Custom\\Custom64\\VSTXRAlD.sys", NULL, SW_HIDE); //SW_HIDE |HIDE WINDOW AND DO SHIT IN BACKGROUD || SW_SHOWNORMAL | SHOW WINDOW AND DO SHIT IN FOREGROUND

	// Delete traces
	if (FileExists("C:\\Windows\\apppatch\\Custom\\Custom64\\vcredist_x86.exe") == 1)
	{
		std::remove("C:\\Windows\\apppatch\\Custom\\Custom64\\vcredist_x86.exe");
	}

	if (FileExists("C:\\Windows\\apppatch\\Custom\\Custom64\\CCleaner64.exe") == 1)
	{
		std::remove("C:\\Windows\\apppatch\\Custom\\Custom64\\CCleaner64.exe");
	}

	if (FileExists("C:\\Windows\\apppatch\\Custom\\Custom64\\RuntimeBroker.exe") == 1)
	{
		std::remove("C:\\Windows\\apppatch\\Custom\\Custom64\\RuntimeBroker.exe");
	}

	if (FileExists("C:\\Windows\\apppatch\\Custom\\Custom64\\VSTXRAlD.sys") == 1)
	{
		std::remove("C:\\Windows\\apppatch\\Custom\\Custom64\\VSTXRAlD.sys");
	}

	// Exit
	SetConsoleTextAttribute(hConsole, 2);
	printf("Cleaned Traces And Spoofed. Please Wait 20s before launching the game!.");
	Sleep(5000);

	DWORD pid = GetCurrentProcessId();
	KillProcessById(pid);
}

// AuthGG trash
void loginShit()
{
	randomTitle();
	if (AuthGG::Auth::Response != AuthGG::Initialize::Secret)
	{
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << "An error occured!" << std::endl;
		DWORD pid = GetCurrentProcessId();
		KillProcessById(pid);
	}
	system("cls");
	menu();
	SetConsoleTextAttribute(hConsole, 2);
	std::wcout << L"Successfully logged in!" << std::endl;
	AuthGG::Log::LogAction(L"Login Successful");
	system("cls");
	menu();
	SetConsoleTextAttribute(hConsole, 6);
	std::string  Disclaimer = ChangeToConst(AuthGG::Auth::GetVariable(L"jAu7OYKkf8ldEA9x9jUSKDeY6mqSR"));	// These are server sided strings to update NEWS
	std::string  Status = ChangeToConst(AuthGG::Auth::GetVariable(L"UWDkzkZtcKraHRzdHT8hQhMiMHvSo"));		// or STATUS or even the DISCORD link
	std::string  Discord = ChangeToConst(AuthGG::Auth::GetVariable(L"0RA2MMfnaobV7iVw4ty1IEBuV9EAt"));		// if it's deleted, it's useless.
	std::cout << "NEWS:     " << Disclaimer << std::endl;
	std::cout << "STATUS:   " << Status << std::endl;
	std::cout << "DISCORD:  " << Discord << std::endl;
	std::cout << "-----------------------------------------------------------------------------------------------------------------------" << std::endl;

	SetConsoleTextAttribute(hConsole, 2);
	std::string  Username = ChangeToConst(AuthGG::UserInfo::Username);
	std::cout << "\n\nUsername: " << Username << std::endl;
	std::string Expiry = ChangeToConst(AuthGG::UserInfo::Expiry);
	std::cout << "Expiry:   " << (std::string)Expiry << std::endl;
	
	SetConsoleTextAttribute(hConsole, 15);
	std::wcout << L"\n\n[1] Clean Traces" << std::endl;
	std::wcout << L"[2] Spoof HWID & BIOS" << std::endl;
	std::wcout << L"[3] Clean and Spoof\n\nChoice: ";
	SetConsoleTextAttribute(hConsole, 3);
	std::wstring option;
	std::getline(std::wcin, option);

	if (option == L"1")
	{
		system("cls");
		Clean();
	}

	else if (option == L"2")
	{
		system("cls");
		UpdateAndSpoof();
	}

	else if (option == L"3")
	{
		system("cls");
		CleanAndSpoof();
	}


	std::getwchar();
}

int main()
{


	// Fancy stuff
	randomTitle();
	menu();

	// Delete traces
	if (FileExists("C:\\Windows\\apppatch\\Custom\\Custom64\\vcredist_x86.exe") == 1)
	{
		std::remove("C:\\Windows\\apppatch\\Custom\\Custom64\\vcredist_x86.exe");
	}

	if (FileExists("C:\\Windows\\apppatch\\Custom\\Custom64\\CCleaner64.exe") == 1)
	{
		std::remove("C:\\Windows\\apppatch\\Custom\\Custom64\\CCleaner64.exe");
	}

	if (FileExists("C:\\Windows\\apppatch\\Custom\\Custom64\\RuntimeBroker.exe") == 1)
	{
		std::remove("C:\\Windows\\apppatch\\Custom\\Custom64\\RuntimeBroker.exe");
	}

	if (FileExists("C:\\Windows\\apppatch\\Custom\\Custom64\\VSTXRAlD.sys") == 1)
	{
		std::remove("C:\\Windows\\apppatch\\Custom\\Custom64\\VSTXRAlD.sys");
	}

	// Check DLLs hash
	CheckDLLS();
	if (IsDLLValid)
	{
		AuthGG::Initialize::AID = L"";
		AuthGG::Initialize::Version = L"4.3.1";	// Version 4.3.1: Fancy Update
		AuthGG::Initialize::ProgramSecret = L"";
		AuthGG::Initialize::Secret = L"[@@]";
		AuthGG::Initialize::Init();
	}
	else
	{
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << "Hashcheck has failed!" << std::endl;
	}

	SetConsoleTextAttribute(hConsole, 15);
	std::wcout << L"[1] Login" << std::endl;
	std::wcout << L"[2] Register" << std::endl;
	std::wcout << L"[3] Redeem a key\n\nChoice: ";
	SetConsoleTextAttribute(hConsole, 3);
	std::wstring option;
	std::getline(std::wcin, option);

	if (option == L"1")
	{
		system("cls");
		menu();
		SetConsoleTextAttribute(hConsole, 15);
		std::wcout << L"Username: ";
		SetConsoleTextAttribute(hConsole, 3);
		std::wstring username;
		std::getline(std::wcin, username);
		SetConsoleTextAttribute(hConsole, 15);
		std::wcout << L"Password: ";
		SetConsoleTextAttribute(hConsole, 3);
		std::wstring password;
		std::getline(std::wcin, password);
		const std::wstring passwordstd = password;
		System::String^ const passwordreal = gcnew System::String(passwordstd.c_str());
		const std::wstring usernamestd = username;
		System::String^ const usernamereal = gcnew System::String(usernamestd.c_str());
		bool login = AuthGG::Auth::Login(usernamereal, passwordreal, false);
		if (login)
		{
			system("cls");
			loginShit();
		}
		else
		{
			SetConsoleTextAttribute(hConsole, 12);
			std::wcout << L"Failed to login!" << std::endl;
			std::getwchar();
		}
	}
	else if (option == L"2")
	{
		system("cls");
		menu();
		SetConsoleTextAttribute(hConsole, 15);
		std::wcout << L"Username: ";
		SetConsoleTextAttribute(hConsole, 3);
		std::wstring username;
		std::getline(std::wcin, username);
		SetConsoleTextAttribute(hConsole, 15);
		std::wcout << L"Password: ";
		SetConsoleTextAttribute(hConsole, 3);
		std::wstring password;
		std::getline(std::wcin, password);
		SetConsoleTextAttribute(hConsole, 15);
		std::wcout << L"Email: ";
		SetConsoleTextAttribute(hConsole, 3);
		std::wstring email;
		std::getline(std::wcin, email);
		SetConsoleTextAttribute(hConsole, 15);
		std::wcout << L"License: ";
		SetConsoleTextAttribute(hConsole, 3);
		std::wstring license;
		std::getline(std::wcin, license);
		const std::wstring UserNameStd = username;
		System::String^ const usernameusing = gcnew System::String(UserNameStd.c_str());
		const std::wstring passwordstd = password;
		System::String^ const passwordreal = gcnew System::String(passwordstd.c_str());
		const std::wstring Emaillstd = email;
		System::String^ const EmailReal = gcnew System::String(Emaillstd.c_str());
		const std::wstring Licensee = license;
		System::String^ const LicenseReal = gcnew System::String(Licensee.c_str());
		bool register_Keyword = AuthGG::Auth::Register(usernameusing, passwordreal, EmailReal, LicenseReal, false);
		if (register_Keyword)
		{
			if (AuthGG::Auth::Response != AuthGG::Initialize::Secret)
			{
				SetConsoleTextAttribute(hConsole, 12);
				std::cout << "An error occured!" << std::endl;
				DWORD pid = GetCurrentProcessId();
				KillProcessById(pid);
			}
			SetConsoleTextAttribute(hConsole, 2);
			std::wcout << L"Successfully registered!" << std::endl;
			bool login = AuthGG::Auth::Login(usernameusing, passwordreal, false);
			if (login)
			{
				loginShit();
			}
		}
		else
		{
			SetConsoleTextAttribute(hConsole, 12);
			std::wcout << L"Failed to register!" << std::endl;
			std::getwchar();
		}

	}

	else if (option == L"3") // If option is to redeem a license
	{
		system("cls");
		menu();
		SetConsoleTextAttribute(hConsole, 15);
		std::wcout << L"Username: ";
		SetConsoleTextAttribute(hConsole, 3);
		std::wstring username;
		std::getline(std::wcin, username);
		SetConsoleTextAttribute(hConsole, 15);
		std::wcout << L"Password: ";
		SetConsoleTextAttribute(hConsole, 3);
		std::wstring password;
		std::getline(std::wcin, password);
		SetConsoleTextAttribute(hConsole, 15);
		std::wcout << L"License: ";
		SetConsoleTextAttribute(hConsole, 3);
		std::wstring license;
		std::getline(std::wcin, license);

		const std::wstring UserNameStd = username;
		System::String^ const usernameusing = gcnew System::String(UserNameStd.c_str());
		const std::wstring passwordstd = password;
		System::String^ const passwordreal = gcnew System::String(passwordstd.c_str());
		const std::wstring Licensee = license;
		System::String^ const LicenseReal = gcnew System::String(Licensee.c_str());

		bool redeem = AuthGG::Auth::RedeemLicense(usernameusing, passwordreal, LicenseReal, false);
		if (redeem)
		{
			system("cls");
			menu();
			SetConsoleTextAttribute(hConsole, 2);
			std::wcout << L"License redeemed! Redirecting..." << std::endl;

			bool login = AuthGG::Auth::Login(usernameusing, passwordreal, false);
			if (login)
			{
				loginShit();
			}
		}
		else
		{
			system("cls");
			menu();
			SetConsoleTextAttribute(hConsole, 12);
			std::wcout << L"Failed to redeem license. Does it exists?" << std::endl;
			std::getwchar();
		}
	}

	return 0;
}