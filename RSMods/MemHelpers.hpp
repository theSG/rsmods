#pragma once

#include "windows.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <codecvt>
#include <algorithm>
#include <atlbase.h>
#include "Lib/DirectX/d3dx9.h"
#include "Lib/DirectX/d3d9types.h"
#include "Lib/Json/json.hpp"
#include "D3D/D3DHooks.hpp"

#include "MemUtil.hpp"
#include "Offsets.hpp"
#include "Structs.hpp"
#include "Settings.hpp"

namespace MemHelpers {
	byte getLowestStringTuning();
	byte* GetCurrentTuning(bool verbose = false);
	void ToggleLoft();
	float SongTimer();
	bool IsExtendedRangeSong();
	int* GetHighestLowestString();
	int* GetHighestLowestString(Tuning tuningOverride);
	bool IsSongInDrop(Tuning tuning);
	bool IsSongInStandard(Tuning tuning);
	int GetTrueTuning();
	int GetNoteHits();
	int GetNoteMiss();
	Resolution GetWindowSize();
	bool IsInStringArray(std::string stringToCheckIfInsideArray, std::vector<std::string> stringVector = std::vector<std::string>());
	void DX9DrawText(std::string textToDraw, int textColorHex, int topLeftX, int topLeftY, int bottomRightX, int bottomRightY, LPDIRECT3DDEVICE9 pDevice, Resolution setFontSize = { NULL, NULL }, DWORD format = DT_LEFT | DT_NOCLIP);
	void ToggleDrunkMode(bool enable);
	bool IsInSong();
	Tuning GetTuningAtTuner();
	bool IsExtendedRangeTuner();
	bool IsMultiplayer();
	std::string CurrentSelectedUser();
	std::string GetSongKey();
	float GetGreyNoteTimer();
	void SetGreyNoteTimer(float timeInSeconds);
	double GetNonStopPlayTimer();
	void SetNonStopPlayTimer(double NewTimer);

	inline std::string lastSongKey = "";

	std::string GetCurrentMenu(bool GameNotLoaded=false);
	void ToggleCB(bool enabled);

	inline static std::string lastMenu = "";
	inline static bool canGetRealMenu = false;

	inline HRESULT CustomDX9Font = NULL;
	inline ID3DXFont* DX9FontEncapsulation = NULL;
	inline int WindowSizeX = NULL, WindowSizeY = NULL;


	struct FontKey {
		std::string face;
		int height;
		int width;
		int weight;
		bool italic;

		static std::string NormalizeFace(std::string s) {
			auto not_space = [](int c) { return !std::isspace(c); };
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), not_space));
			s.erase(std::find_if(s.rbegin(), s.rend(), not_space).base(), s.end());
			std::transform(s.begin(), s.end(), s.begin(),
				[](unsigned char c) { return char(std::tolower(c)); });
			return s;
		}

		bool operator==(const FontKey& o) const {
			return height == o.height && weight == o.weight && italic == o.italic && face == o.face;
		}

		static FontKey Make(std::string face, int h, int w, int wt, bool it) {
			return FontKey{ NormalizeFace(std::move(face)), h, w, wt, it };
		}
	};

	struct FontKeyHash {
		size_t operator()(const FontKey& k) const {
			size_t h = std::hash<std::string>()(k.face);
			h ^= static_cast<size_t>(k.height) + 0x9e3779b9 + (h << 6) + (h >> 2);
			h ^= static_cast<size_t>(k.weight) + 0x9e3779b9 + (h << 6) + (h >> 2);
			h ^= static_cast<size_t>(k.italic) + 0x9e3779b9 + (h << 6) + (h >> 2);
			return h;
		}
	};

	class FontCache {
	public:
		bool Get(IDirect3DDevice9* dev, const FontKey& key, CComPtr<ID3DXFont>& out) {
			out.Release();
			if (auto it = cache.find(key); it != cache.end() && it->second) {
				out = it->second;
				return true;
			}

			CComPtr<ID3DXFont> font;
			HRESULT hr = D3DXCreateFontA(dev, key.height, key.width, key.weight, 1, key.italic,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
				DEFAULT_PITCH | FF_DONTCARE, key.face.c_str(), &font);
			if (FAILED(hr) || !font) return false;

			auto [iter, inserted] = cache.try_emplace(key, font);
			out = iter->second;

			return true;
		}

		void OnLostDevice() const {
			for (const auto& [key, fontPtr] : cache) if (fontPtr) fontPtr->OnLostDevice();
		}

		void OnResetDevice() const {
			for (const auto& [key, fontPtr] : cache) if (fontPtr) fontPtr->OnResetDevice();
		}
	private:
		std::unordered_map<FontKey, CComPtr<ID3DXFont>, FontKeyHash> cache;
	};

	inline static FontCache fontCache;
};

namespace Util {
	inline void SendKey(unsigned int key) {
		PostMessage(FindWindow(NULL, L"Rocksmith 2014"), WM_KEYDOWN, key, 0);
		Sleep(30);
		PostMessage(FindWindow(NULL, L"Rocksmith 2014"), WM_KEYUP, key, 0);
	}
};
