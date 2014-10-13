//
//  ErrorMessage.h
//  Data Structures
//
//  Created by Mickey on 10/11/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#ifndef __Data_Structures__ErrorMessage__
#define __Data_Structures__ErrorMessage__

#include <string>
using std::string;

#include <sstream>
using std::stringstream;

class Message {
    
private:
    stringstream ss;

public:
    Message() {
        
    }
    
    ~Message() {
        
    }
    
    template< typename T >
    Message& operator<<( const T& value ) {
        ss << value;
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
