#ifndef MODELS_H
#define MODELS_H

#include <vector>
#include <map>

#include "data.hpp"
#include "../hash/hash.hpp"
#include "../macros/global.hpp"
#include "../macros/utility.hpp"

namespace Models {
	using MiniSnapshotMapT = std::map<Hash::SHA256, Data::Structures::MiniSnapshot>;

	template<class T>
	class ModelBase {
	protected:
		T data;
	private:
		void _COPY(const ModelBase& _ref) {
			memcpy(&this->data, &_ref.data, sizeof(this->data));
		}
		void _CLEAR() {
			memset(&this->data, 0, sizeof(this->data));
		}
	public:
		ModelBase() { _CLEAR(); };
		ModelBase(const ModelBase& _ref) {
			_COPY(_ref);
		}
		const ModelBase& operator=(const ModelBase& _ref) {
			_COPY(_ref);
			return *this;
		}
	};

	template<class T>
	class Model : public ModelBase<T>{ 
	public:
		Model() { };
		Model(const Model& _ref) {
			this->data = _ref.data;
		}
		Model(const T& _ref) {
			this->data = _ref;
		}
		const Model& operator=(const T& _ref) {
			this->data = _ref;
			return *this;
		}
		const T& getData() const noexcept { return this->data; }
		std::unique_ptr<Data::Form::Hashable_Data<T>> getHashable() const noexcept {
			std::unique_ptr<Data::Form::Hashable_Data<T>> ptr;
			ptr = std::make_unique<Data::Form::Hashable_Data<T>>(this->data);
			return ptr;
		}
	};

	class Game : public Model<Data::Structures::Game> {
	public:
		Game() { };
		Game(const Data::Structures::Game& originalData);
		Game(const STRING& nameGame, const STRING& nameProc, const STRING& pathGame, const STRING& pathSave, const STRING& ExtensionOfSave);
		void setName(const STRING& nameGame) noexcept;
		void setProcessName(const STRING& nameProc) noexcept;
		void setPathToGame(const STRING& pathGame) noexcept;
		void setPathToSaveFiles(const STRING& pathSave) noexcept;
		void setExtensionOfSave(const STRING& ExtensionOfSave) noexcept;
	};

	class Snapshot : public Model<Data::Structures::Snapshot>{
	public:
		Snapshot() { };
		Snapshot(const Data::Structures::Snapshot& originalData);
		Snapshot(const STRING& nameSave, const time_t& dateAdded, Hash::SHA256 hashGame, Hash::SHA256 hashSnapshot, const STRING& pathSave, const STRING& pathCopy);
		void setName(const STRING& nameSave) noexcept;
		void setTime(const std::time_t& dateAdded) noexcept;
		void setPathToFile(const STRING& pathSave) noexcept;
		void setHashGame(const Hash::SHA256 hashGame) noexcept;
		void setHashSnapshot(const Hash::SHA256 hashSnapshot) noexcept;
		void setPathToCopyOfFile(const STRING& pathCopy) noexcept;
	};

	class Map : public Model<Data::Structures::Map> {
	public:
		Map() { };
		Map(const Data::Structures::Map& originalData);
		Map(const MiniSnapshotMapT& map);
		Data::T_SNAPSHOT_SIZE_IN_MAP getSize();
		bool isInsertPossible();
		int getInsertPossibleIdx();
		MiniSnapshotMapT getMapAsGeneric();
		void updateMap(const MiniSnapshotMapT map);
		bool insertSnapshot(const Data::Structures::Snapshot& snapshot);
		bool insertSnapshot(const Data::Structures::MiniSnapshot& snapshot);
		void removeSnapshot(unsigned int idx);
	};
}

#endif