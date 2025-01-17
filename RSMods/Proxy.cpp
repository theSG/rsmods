#include "Proxy.hpp"

#pragma pack(1)

typedef void(__stdcall* T_XInput_XInputEnable)(BOOL enable);
typedef DWORD(__stdcall* T_XInput_XInputGetBatteryInformation)(DWORD dwUserIndex, BYTE devType, XINPUT_BATTERY_INFORMATION* pBatteryInformation);
typedef DWORD(__stdcall* T_XInput_XInputGetCapabilities)(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES* pCapabilities);
typedef DWORD(__stdcall* T_XInput_XInputGetDSoundAudioDeviceGuids)(DWORD dwUserIndex, GUID* pDSoundRenderGuid, GUID* pDSoundCaptureGuid);
typedef DWORD(__stdcall* T_XInput_XInputGetKeystroke)(DWORD dwUserIndex, DWORD dwReserved, XINPUT_KEYSTROKE* pKeystroke);
typedef DWORD(__stdcall* T_XInput_XInputGetState)(DWORD dwUserIndex, XINPUT_STATE* pState);
typedef DWORD(__stdcall* T_XInput_XInputSetState)(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration);

FARPROC proxy[7] = { 0 };
HINSTANCE originalDLL;

/// <summary>
/// Shutdown Proxy to original DLL
/// </summary>
void Proxy::Shutdown() {
	FreeLibrary(originalDLL);
}

/// <summary>
/// Proxy to original DLL
/// </summary>
bool Proxy::Init() {
	char winpath[MAX_PATH];

	if (originalDLL)
		return true;

	GetSystemDirectoryA(winpath, sizeof(winpath));
	strcat_s(winpath, "\\xinput1_3.dll");

	originalDLL = LoadLibraryA(winpath);

	if (!originalDLL)
		return false;

	proxy[0] = GetProcAddress(originalDLL, "XInputEnable");
	proxy[1] = GetProcAddress(originalDLL, "XInputGetBatteryInformation");
	proxy[2] = GetProcAddress(originalDLL, "XInputGetCapabilities");
	proxy[3] = GetProcAddress(originalDLL, "XInputGetDSoundAudioDeviceGuids");
	proxy[4] = GetProcAddress(originalDLL, "XInputGetKeystroke");
	proxy[5] = GetProcAddress(originalDLL, "XInputGetState");
	proxy[6] = GetProcAddress(originalDLL, "XInputSetState");

	return true;
}


extern "C" {
	enum XInput_Proxy {
		Enable,
		GetBatteryInformation,
		GetCapabilities,
		GetDSoundAudioDeviceGuids,
		GetKeystroke,
		GetState,
		SetState
	};


	// XInputEnable
	void __stdcall XInput_XInputEnable(BOOL enable) {
		return ((T_XInput_XInputEnable)proxy[XInput_Proxy::Enable])(enable);
	}

	// XInputGetBatteryInformation
	DWORD __stdcall XInput_XInputGetBatteryInformation(DWORD dwUserIndex, BYTE devType, XINPUT_BATTERY_INFORMATION* batteryInformation) {
		return ((T_XInput_XInputGetBatteryInformation)proxy[XInput_Proxy::GetBatteryInformation])(dwUserIndex, devType, batteryInformation);
	}

	// XInputGetCapabilities
	DWORD __stdcall XInput_XInputGetCapabilities(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES* capabilities) {
		return ((T_XInput_XInputGetCapabilities)proxy[XInput_Proxy::GetCapabilities])(dwUserIndex, dwFlags, capabilities);
	}

	// XInputGetDSoundAudioDeviceGuids
	DWORD __stdcall XInput_XInputGetDSoundAudioDeviceGuids(DWORD dwUserIndex, GUID* pDSoundRenderGuid, GUID* DSoundCaptureGuid) {
		return ((T_XInput_XInputGetDSoundAudioDeviceGuids)proxy[XInput_Proxy::GetDSoundAudioDeviceGuids])(dwUserIndex, pDSoundRenderGuid, DSoundCaptureGuid);
	}

	// XInputGetKeystroke
	DWORD __stdcall XInput_XInputGetKeystroke(DWORD dwUserIndex, DWORD dwReserved, XINPUT_KEYSTROKE* keystroke) {
		return ((T_XInput_XInputGetKeystroke)proxy[XInput_Proxy::GetKeystroke])(dwUserIndex, dwReserved, keystroke);
	}

	// XInputGetState
	DWORD __stdcall XInput_XInputGetState(DWORD dwUserIndex, XINPUT_STATE* state) {
		return ((T_XInput_XInputGetState)proxy[XInput_Proxy::GetState])(dwUserIndex, state);
	}

	// XInputSetState
	DWORD __stdcall  XInput_XInputSetState(DWORD dwUserIndex, XINPUT_VIBRATION* vibration) {
		return ((T_XInput_XInputSetState)proxy[XInput_Proxy::SetState])(dwUserIndex, vibration);
	}
}