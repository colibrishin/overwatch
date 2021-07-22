#ifndef MODELS_H
#define MODELS_H

#include <vector>

#include "data.hpp"
#include "loader.hpp"
#include "../hash/hash.hpp"
#include "../macros/global.hpp"
#include "../macros/utility.hpp"

namespace Models {
	template<class T>
	class Model {
	protected:
		T data;
	public:
		const T& getData() const noexcept { return data; }
		std::unique_ptr<Data::Form::Hashable_Data<T>> getHashable() const noexcept {
			std::unique_ptr<Data::Form::Hashable_Data<T>> ptr;
			ptr = std::make_unique<Data::Form::Hashable_Data<T>>(data);
			return ptr;
		}
	};

	class Game : public Model<Data::Structures::Game> {
	public:
		Game() {};
		Game(const STRING& nameGame, const STRING& nameProc, const STRING& pathGame, const STRING& pathSave, const STRING& ExtensionOfSave);
		void setName(const STRING& nameGame) noexcept;
		void setProcessName(const STRING& nameProc) noexcept;
		void setPathToGame(const STRING& pathGame) noexcept;
		void setPathToSaveFiles(const STRING& pathSave) noexcept;
		void setExtensionOfSave(const STRING& ExtensionOfSave) noexcept;
	};

	class Snapshot : public Model<Data::Structures::Snapshot>{
	public:
		Snapshot() {};
		Snapshot(const STRING& nameSave, const Filesystem::pathT& pathSave);
		void setName(const STRING& nameSave) noexcept;
		void setTime(const std::time_t& dateAdded) noexcept;
		void setPathToFile(const STRING& pathSave) noexcept;
		void setHashGame(const Hash::SHA256 hashGame) noexcept;
		void setHashSnapshot(const Hash::SHA256 hashSnapshot) noexcept;
		void setPathToCopyOfFile(const STRING& pathCopy) noexcept;
	};
}

#endif