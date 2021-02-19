#pragma once
#include "(2,4)Tree.h"

template<typename V>
void TwoFourTree<V>::Insert(const Entry& entry)
{
	if (all_entries_size == 0) {
		root_->PushEntry(entry);
		all_entries_size++;
		return;
	}


}

template<typename V>
TwoFourTree<V>::EntryPtr TwoFourTree<V>::Search(const int& key) const
{
	if (all_entries_size == 0) { throw std::length_error("There is no entry!"); }

}
