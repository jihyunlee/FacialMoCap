#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class MILLXUTILS_API FMillXUtilsModule : public IModuleInterface {

public:
	static inline FMillXUtilsModule& Get() {
		return FModuleManager::LoadModuleChecked<FMillXUtilsModule>("MillXUtils");
	}

	static inline bool IsAvailable() {
		return FModuleManager::Get().IsModuleLoaded("MillXUtils");
	}

public: 
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
