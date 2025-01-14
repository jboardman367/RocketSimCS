#pragma once
#include "../BaseInc.h"

#include "SerializeObject.h"

// Basic struct for reading raw data from a file
struct DataStreamIn {
	vector<char> data;
	size_t pos = 0;

	DataStreamIn(std::filesystem::path filePath, bool versionCheck = true) {
		std::ifstream fileStream = std::ifstream(filePath, std::ios::binary);
		if (!fileStream.good())
			RS_ERR_CLOSE("Failed to read file " << filePath << ", cannot open file.");

		data = vector<char>(std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>());

		if (versionCheck) {
			uint32_t versionID = Read<uint32_t>();
			if (versionID != RS_VERSION_ID)
				RS_ERR_CLOSE("Failed to read file " << filePath << ", file is invalid or from a different version of RocketSim.");
		}
	}

	bool IsDone() {
		return pos >= data.size();
	}

	bool IsOverflown() {
		return pos > data.size();
	}

	size_t GetNumBytesLeft() {
		if (IsDone()) {
			return 0;
		} else {
			return data.size() - pos;
		}
	}

	void ReadBytes(void* out, size_t amount) {
		if (GetNumBytesLeft() >= amount) {
			byte* asBytes = (byte*)out;
			memcpy(asBytes, data.data() + pos, amount);

			if (RS_IS_BIG_ENDIAN)
				std::reverse(asBytes, asBytes + sizeof(amount));
		}

		pos += amount;
	}

	template <typename T>
	T Read() {
		byte bytes[sizeof(T)];
		ReadBytes(bytes, sizeof(T));
		return *(T*)bytes;
	}

	template <typename T>
	void Read(T& out) {
		out = Read<T>();
	}

	void ReadMultipleFromList(std::vector<SerializeObject> objs) {
		for (const SerializeObject& obj : objs)
			ReadBytes(obj.ptr, obj.size);
	}

	template <typename... Args>
	void ReadMultiple(Args&... args) {
		ReadMultipleFromList({ args... });
	}
};