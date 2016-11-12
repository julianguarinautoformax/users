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
    
    cout << "El numero de registros que tenemos que tenemos es: " << x.userCount() << endl;
    x.userDump();
    
    return 0;
}
#pragma clang pop
