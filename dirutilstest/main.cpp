//
//  main.cpp
//  dirutilstest
//
//  Created by Julian Andres Guarin on 11/14/16.
//  Copyright © 2016 Julian Andres Guarin. All rights reserved.
//

#include <iostream>
#include "DirUtils.hpp"

int main(int argc, const char * argv[]) {
    // insert code here...
    std::string base = "/Users/imac/Code/hack/zImembre";
    std::string cwd = "/Users/imac/Code/hack/zImembre/tcpserver/test/ftpserver";
    std::string xpr = "/ftpadminpanel/";
    xpr = DirUtils::expandDirExpression(xpr, base, cwd);
    DirUtils::absoluteChangeDir(xpr,base,cwd);
    
    xpr = "~";
    xpr = DirUtils::expandDirExpression(xpr, base, cwd);
    DirUtils::absoluteChangeDir(xpr, base, cwd);
    
    DirUtils::relativeMakeDir("staff",base,cwd);
    
    int rm = DirUtils::relativeDeleteDir("staff", base, cwd);
    std::cout << rm << std::endl;
    return 0;
    
}
