//
//  main.cpp
//  jsonTest
//
//  Created by Julian Andres Guarin on 11/10/16.
//  Copyright © 2016 Julian Andres Guarin. All rights reserved.
//

#include <iostream>
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"

#include "Users.hpp"

using namespace std;
int main(int argc, const char * argv[]) {
    // insert code here...
    
    
    Users x("/Users/imac/Code/hack/users/Users/Users/zimembreftpserver.json");
    
    User jg = x.user((char*)"julianguarin2010");
    if (x.userExist("someuser")){
        x.userDelete(x.user((char*)"someuser"));
    } else {
        
        x.userCreate("someuser", "1234", USER_ROLE_USER, User::USER_RDWR);
        User xu = x.user((char*) "someuser");
        x.userDump();
        xu.set_password("Amoreseriou$password");
        x.userUpdate(xu);
        
    }
    
    return 0;
}
#pragma clang pop
