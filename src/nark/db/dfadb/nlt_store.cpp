#include "nlt_store.hpp"
#include <nark/fsa/nest_trie_dawg.hpp>
#include <nark/io/FileStream.hpp>
#include <nark/io/DataIO.hpp>
#include <nark/util/mmap.hpp>
#include <boost/filesystem.hpp>

namespace nark { namespace db { namespace dfadb {

NestLoudsTrieStore::NestLoudsTrieStore() {
}
NestLoudsTrieStore::~NestLoudsTrieStore() {
}

llong NestLoudsTrieStore::dataStorageSize() const {
	return m_store->mem_size();
}

llong NestLoudsTrieStore::numDataRows() const {
	return m_store->num_records();
}

void NestLoudsTrieStore::getValueAppend(llong id, valvec<byte>* val, DbContext* ctx) const {
	m_store->get_record_append(size_t(id), val);
}

StoreIterator* NestLoudsTrieStore::createStoreIterForward(DbContext*) const {
	return nullptr; // not needed
}

StoreIterator* NestLoudsTrieStore::createStoreIterBackward(DbContext*) const {
	return nullptr; // not needed
}

void NestLoudsTrieStore::build(SortableStrVec& strVec) {
	NestLoudsTrieConfig conf;
//	conf.maxFragLen = 512;
	conf.initFromEnv();
	m_store.reset(new NestLoudsTrieDataStore_SE_512());
	m_store->build_from(strVec, conf);
}

void NestLoudsTrieStore::load(fstring path) {
	std::string fpath = path + ".nlt";
	std::unique_ptr<BaseDFA> dfa(BaseDFA::load_mmap(fpath.c_str()));
	m_store.reset(dynamic_cast<NestLoudsTrieDataStore_SE_512*>(dfa.get()));
	if (m_store) {
		dfa.release();
	}
}

void NestLoudsTrieStore::save(fstring path) const {
	std::string fpath = path + ".nlt";
	m_store->save_mmap(fpath.c_str());
}

}}} // namespace nark::db::dfadb
