//
//  ErrorMessage.h
//  Data Structures
//
//  Created by Mickey on 10/11/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#ifndef __Data_Structures__Message__
#define __Data_Structures__Message__

#include <string>
using std::string;

#include <sstream>
using std::stringstream;

namespace has_insertion_operator_impl {
    typedef char no;
    typedef char yes[2];
    
    struct any_t {
        template<typename T> any_t( T const& );
    };
    
    no operator<<( std::ostream const&, any_t const& );
    
    yes& test( std::ostream& );
    no test( no );
    
    template<typename T>
    struct has_insertion_operator {
        static std::ostream &s;
        static T const &t;
        static bool const value = sizeof( test(s << t) ) == sizeof( yes );
    };
}

template<typename T>
struct has_insertion_operator :
has_insertion_operator_impl::has_insertion_operator<T> {
};

class Message {
    
private:
    stringstream ss;

public:
    Message() {
        
    }
    
    ~Message() {
        
    }
    
    template< typename T >
    typename std::enable_if<has_insertion_operator<T>::value, Message&>::type
    operator<<( const T& value ) {
        ss << value;
        return *this;
    }

    template< typename T >
    typename std::enable_if<!has_insertion_operator<T>::value, Message&>::type
    operator<<( const T& value ) {
        ss << &value;
        return *this;
    }
    operator std::string() const {
        return ss.str();
    }
    
    string str() const {
        return ss.str();
    }
};

#endif /* defined(__Data_Structures__ErrorMessage__) */
