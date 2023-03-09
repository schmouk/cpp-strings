#pragma once
/**
    Library cppstrings
    "What if c++ strings where as easy to use as Python strings?"

    Copyright (C) 2023 Philippe Schmouker
    contact - ph (dot) schmouker (at) gmail (dot) com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation,  either version 3  of the License,  or
    (at your option) any later version.

    This program is distributed in the hope  that  it  will  be  useful,
    but  WITHOUT  ANY  WARRANTY;  without  even  the implied warranty of
    MERCHANTABILITY  or  FITNESS  FOR  A  PARTICULAR  PURPOSE.  See  the
    GNU General Public License for more details.

    You should have received a copy  of  the  GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

//=============================================================
#include <algorithm>
#include <cctype>
#include <string>
#include <vector>
//#include <initializer_list>


//=============================================================
// Forward declarations
template<
    class CharT,
    class TraitsT = std::char_traits<CharT>,
    class AllocatorT = std::allocator<CharT>
> class CppStringT;                     //!< The templated base class. Use its belowing specializations instead!

using CppString  = CppStringT<char>;    //!< Specialization of basic class with template argument 'char'
using CppWString = CppStringT<wchar_t>; //!< Specialization of basic class with template argument 'wchar_t'


//=====   CppStringT<>   ======================================
/**
* \brief This is the templated base class for all CppString
* classes.
* 
* Users should instantiate any specialization of this  base  class
* rather than this base class itself:
*   - \see CppString for CppStringT<char>.
*   - \see CppWString for CppStringT<wchar_t>.
* 
* This base class inherits from std::basic_string<CharT>. As such,
* it  gets  direct access to all public methods of its base class.
* \see  https://en.cppreference.com/w/cpp/string/basic_string  for
* a full list of such methods, for instance.
* 
* You may specialize it by your own with any of the next char
* types:
*   - char8_t  (C++20)
*   - char16_t (C++11)
*   - char32_t (C++11)
*/
template<class CharT, class TraitsT, class AllocatorT>
class CppStringT : public std::basic_string<CharT>
{
public:
    //===   Wrappers   ========================================
    using MyBaseClass = std::basic_string<CharT>;

    using traits_type            = MyBaseClass::traits_type;
    using value_type             = MyBaseClass::value_type;
    using allocator_type         = MyBaseClass::allocator_type;
    using size_type              = MyBaseClass::size_type;
    using difference_type        = MyBaseClass::difference_type;
    using reference              = MyBaseClass::reference;
    using const_reference        = MyBaseClass::const_reference;
    using pointer                = MyBaseClass::pointer;
    using const_pointer          = MyBaseClass::const_pointer;
    using iterator               = MyBaseClass::iterator;
    using const_iterator         = MyBaseClass::const_iterator;
    using reverse_iterator       = MyBaseClass::reverse_iterator;
    using const_reverse_iterator = MyBaseClass::const_reverse_iterator;


    //===   Constructors   ====================================
    inline CppStringT() : MyBaseClass() {}
    inline CppStringT(const CppStringT& other) : MyBaseClass(other) {}
    inline CppStringT(const CppStringT& other, const AllocatorT& alloc) : MyBaseClass(other, alloc){}
    inline CppStringT(CppStringT&& other) : MyBaseClass(other) {}
    inline CppStringT(CppStringT&& other, const AllocatorT& alloc) : MyBaseClass(other, alloc) {}
    inline CppStringT(MyBaseClass::size_type count, CharT ch) : MyBaseClass(count, ch) {}
    inline CppStringT(const CppStringT& other, size_type pos) : MyBaseClass(other, pos) {}
    inline CppStringT(const CppStringT& other, size_type pos, size_type count) noexcept : MyBaseClass(other, pos, count) {}
    inline CppStringT(const CharT* s) : MyBaseClass(s) {}
    inline CppStringT(const CharT* s, size_type count) : MyBaseClass(s, count) {}
    inline CppStringT(std::initializer_list<CharT> ilist) : MyBaseClass(ilist) {}

    template<class InputIt>
    inline CppStringT(InputIt first, InputIt last) : MyBaseClass(first, last) {}

    template<class StringViewLike>
    explicit CppStringT(const StringViewLike& svl) : MyBaseClass(svl) {}

    template<class StringViewLike>
    CppStringT(const StringViewLike& svl, size_type pos, size_type n) : MyBaseClass(svl, pos, n) {}


    //===   Methods   =========================================

    //---   capitalize()   ------------------------------------
    /** \brief In-place modifies the string with its first character capitalized and the rest lowercased. Returns a reference to the string*/
    inline CppStringT& capitalize() noexcept
    {
        this->lower();
        (*this)[0] = upper((*this)[0]);
        return *this;
    }


    //---   center()   ----------------------------------------
    /** Returns the string centered in a string of length width.
    * *
    * Padding is done using the specified fillchar (default is an ASCII space). 
    * A copy of the original string is returned if width is less than or  equal 
    * to the length of the string. The original string remains unchanged.
    */
    inline CppStringT center(const size_type width, const value_type fillch = value_type(' ')) const noexcept
    {
        const size_type l = this->length();
        if (l <= width)
            return CppStringT(*this);
        const size_type half = (width - l) / 2;
        return CppStringT(fillch, half) + *this + CppStringT(fillch, width - half - l);
    }


    //---   is_punctuation()   --------------------------------
    /** \brief Returns true if the string contains only one character and if this character belongs to the ASCII punctuation set. */
    inline const bool is_punctuation() const noexcept
    {
        return this->length() == 1 && is_punctuation((*this)[0]);
    }

    /** \brief Returns true if character belongs to the ASCII punctuation set. */
    static inline const bool is_punctuation(const value_type& ch) noexcept
    {
        return _ASCII_PUNCT_DATA.contains(ch);
    }


    //---   is_space()   --------------------------------------
    /** \brief Returns true if there are only whitespace characters in the string and there is at least one character, or false otherwise.
    *
    * Notice for version 2.0 of this library: a character  is  whitespace  if
    * in  the  Unicode character database,  either its general category is Zs 
    * (�Separator, space�), or its bidirectional class is one of WS, B, or S.
    */
    inline const bool is_space() const noexcept
    {
        if (this->length() == 0)
            return false;
        for (auto& c : *this)
            if (!is_space(c))
                return false;
        return true;
    }

    /** \brief Returns true if character belongs to the ASCII spaces set. */
    static inline const bool is_space(const value_type& ch) noexcept
    {
        return _ASCII_SPACES.contains(ch);
    }


    //---   is_words_sep()   ----------------------------------
    /** \brief Returns true if there are only whitespace and punctuation characters in the string and there is at least one character, or false otherwise. */
    inline const bool is_words_sep() const noexcept
    {
        return is_space() || is_punctuation();
    }

    /** \brief Returns true if character belongs to ASCII spaces or punctuation sets. */
    static inline const bool is_words_sep(const value_type& ch) noexcept
    {
        return is_space(ch) || is_punctuation(ch);
    }


    //---   lower ()  -----------------------------------------
    /** \brief In-place replaces all characters of the string with their lowercase conversion. Returns a reference to string.
    *
    * Notice: uses the currently set std::locale, which is the "C" one
    * by default or any other one as previously set by the user.
    */
    inline CppStringT& lower() noexcept
    {
        std::transform(this->begin(), this->end(),
                       this->begin(),
                       [](value_type ch) { return this->lower(ch); });
        return *this;
    }

    /** \brief Returns lowercase conversion of the character.
    *
    * Notice: uses the currently set std::locale, which is the "C" one
    * by default or any other one as previously set by the user.
    */
    static inline const value_type lower(const value_type ch) noexcept
    {
        return value_type(std::tolower(ch));
    }


    //---   upper ()  -----------------------------------------
    /** \brief In-place replaces all characters of the string with their uppercase conversion. Returns a reference to string.
    *
    * Notice: uses the currently set std::locale, which is the "C" one
    * by default or any other one as previously set by the user.
    */
    inline CppStringT& upper() noexcept
    {
        std::transform(this->begin(), this->end(),
            this->begin(),
            [](value_type ch) { return this->upper(ch); });
        return *this;
    }

    /** \brief Returns uppercase conversion of the character.
    *
    * Notice: uses the currently set std::locale, which is the "C" one
    * by default or any other one as previously set by the user.
    */
    static inline const value_type upper(const value_type ch) noexcept
    {
        return value_type(std::toupper(ch));
    }



protected:


private:
    //===   DATA   ============================================
    static inline constexpr std::vector<value_type> _ASCII_PUNCT_DATA{ '!', ',', '.', ':', ';', '?' };
    static inline constexpr std::vector<value_type> _ASCII_SPACES{ ' ', '\t', '\n', 'r', '\f' };

};
