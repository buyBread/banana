#pragma once

#include <cstdlib>

#include "util/types.hh"
#include "util/macros/sanity_assert.hh"

/*
    todo: this will need to be more properly implemented
          when? idk, we'll see
*/

namespace treyarch {
    struct fixed_pool; // i'm not looking forward to this

    class stringx {

public: // why are you public?
        char* data_;
        i32 length_;

protected:
        fixed_pool* source_pool;

public:
        enum      { npos = -1 };
        enum fmtd { fmt };

                 stringx();
                 stringx(const stringx &cp);
                 stringx(const char* str, i32 len = -1);
                 stringx(fmtd, const char* fmtp, ...);
                 stringx(const char* fmt, va_list vlist);
        explicit stringx(f32 f);
        explicit stringx(i32 i);
        explicit stringx(u32 i);

        ~stringx ();

        void init(const char* str, i32 len = -1);
        void term();

        void copy(const char* str, i32 source_count = -1);
        void copy(stringx &cp);
        void copy(char ch );

        void append(const char* str, i32 source_count = -1);
        void append(const stringx &cp);
        void append(char ch);


               stringx &operator= (const stringx &cp);
               stringx &operator= (const char* str);
               stringx &operator+=(const stringx &cp);
               stringx &operator+=(const char* str);
        inline stringx &operator+=(char ch ) { append(ch); return *this; };
        inline stringx &operator= (char ch ) { copy(ch); return *this; };

        inline       char &operator[] (i32 i);
        inline const char  operator[] (i32 i) const;

        stringx &remove_leading(const char *remove);
        stringx &remove_trailing(const char *remove);

        stringx &remove_surrounding_whitespace() {
            remove_leading(" \n\t\r");
            remove_trailing(" \n\t\r");

            return *this;
        }

        f32 to_float() const { 
            return (f32)std::atof(data_);
        }
        i32 to_int() const {
            return std::atoi(data_);
        }

        inline const char* c_str() const;

        inline const char* data() const;

        inline i32 size() const;
        inline i32 length() const;

        inline bool empty() const;
        inline bool is_equal(const char* str) const;
    
        i32 compare (const char* str) const;
    
        stringx &to_upper ();
        stringx &to_lower ();

        stringx substr(i32 i = 0, i32 n = -1) const;

        stringx &truncate(i32 new_lenght);

        inline stringx slice ( i32 start, i32 end ) const;

        i32 find(const char *s, i32 pos = 0) const;
        i32 find(i32 pos, char c) const;
        i32 rfind(char c, i32 pos = -1) const;
        i32 rfind(const char *str) const;

        friend stringx operator+(const stringx &lhs, const stringx &rhs);
        friend stringx operator+(const stringx &lhs, const char*    rhs);
        friend stringx operator+(const char*    lhs, const stringx &rhs);

        friend inline bool operator==(const stringx &lhs, const stringx &rhs);
        friend inline bool operator==(const stringx &lhs, const char*    rhs);

        friend inline bool operator!=(const stringx &lhs, const stringx &rhs);
        friend inline bool operator!=(const stringx &lhs, const char*    rhs);

        friend inline bool operator<(const stringx &lhs, const stringx &rhs);
        friend inline bool operator>(const stringx &lhs, const stringx &rhs);

        friend void validate_stringx(const stringx &x);
    };

    ASSERT_SIZEOF(stringx, 0x0C);
} // treyarch