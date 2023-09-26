#include "pch.h"

VariableA& sharedStateA() {
	static VariableA inst;
	return inst;
}


void VariableA::LoadSettings() {
	auto file = fopen("SongControlManager.dat", "rb");
	if (file) {
		fseek(file, 0, SEEK_END);
		auto size = ftell(file);
		if (size == sizeof(sharedStateA())) {
			fseek(file, 0, SEEK_SET);
			fread(&sharedStateA(), sizeof(sharedStateA()), 1, file);
			fclose(file);
		}
	}


}

void VariableA::SaveSettings() {
	auto file = fopen("SongControlManager.dat", "wb");
	if (file) {
		fwrite(&sharedStateA(), sizeof(sharedStateA()), 1, file);
		fclose(file);
	}
}
