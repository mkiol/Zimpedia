/*
 * Copyright (C) 2021 Maneesh P M <manu.pm55@gmail.com>
 * Copyright (C) 2020 Matthieu Gautier <mgautier@kymeria.fr>
 * Copyright (C) 2006 Tommi Maekitalo
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * is provided AS IS, WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, and
 * NON-INFRINGEMENT.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 *
 */

#ifndef ZIM_SUGGESTION_ITERATOR_H
#define ZIM_SUGGESTION_ITERATOR_H

#include "archive.h"
#include <iterator>

namespace zim
{
class SuggestionResultSet;
class SuggestionItem;
class SearchIterator;

class SuggestionIterator : public std::iterator<std::bidirectional_iterator_tag, SuggestionItem>
{
    typedef Archive::iterator<EntryOrder::titleOrder> RangeIterator;
    friend class SuggestionResultSet;
    public:
        SuggestionIterator() = delete;
        SuggestionIterator(const SuggestionIterator& it);
        SuggestionIterator& operator=(const SuggestionIterator& it);
        SuggestionIterator(SuggestionIterator&& it);
        SuggestionIterator& operator=(SuggestionIterator&& it);
        ~SuggestionIterator();

        bool operator== (const SuggestionIterator& it) const;
        bool operator!= (const SuggestionIterator& it) const;

        SuggestionIterator& operator++();
        SuggestionIterator operator++(int);
        SuggestionIterator& operator--();
        SuggestionIterator operator--(int);

        Entry getEntry() const;

        const SuggestionItem& operator*();
        const SuggestionItem* operator->();

    private: // data
        struct SuggestionInternalData;
        std::unique_ptr<RangeIterator> mp_rangeIterator;
        std::unique_ptr<SuggestionItem> m_suggestionItem;

    private: // methods
        SuggestionIterator(RangeIterator rangeIterator);

// Xapian based methods and data
#if defined(LIBZIM_WITH_XAPIAN)
#ifdef ZIM_PRIVATE
    public:
        std::string getDbData() const;
#endif
    private: // xapian based data
        std::unique_ptr<SuggestionInternalData> mp_internal;

    private: // xapian based methods
        std::string getIndexPath() const;
        std::string getIndexTitle() const;
        std::string getIndexSnippet() const;
        SuggestionIterator(SuggestionInternalData* internal_data);
#endif  // LIBZIM_WITH_XAPIAN
};

class SuggestionItem
{
    public: // methods
        std::string getTitle() const { return title; }
        std::string getPath() const { return path; }
        std::string getSnippet() const { return snippet; }

        bool hasSnippet() const { return !snippet.empty(); }

    private: // data
        std::string title;
        std::string path;
        std::string snippet;

    private: // methods
        explicit SuggestionItem(std::string title, std::string path, std::string snippet = "")
        :   title(title),
            path(path),
            snippet(snippet) {}

    friend class SuggestionIterator;
};

} // namespace zim

#endif // ZIM_SUGGESTION_ITERATOR_H
