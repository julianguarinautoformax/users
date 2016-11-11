/*
 *  Users.hpp
 *  Users
 *
 *  Created by Julian Andres Guarin on 11/10/16.
 *  Copyright © 2016 Julian Andres Guarin. All rights reserved.
 *
 */

#ifndef Users_
#define Users_

#include <fstream>
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include "json.hpp"
#pragma clang pop
class Users
{
    
    std::ifstream m_f;
    nlohmann::json m_j;
    




public:
    
    Users (const char * jsonpath);
    ~Users(){}
    
    
    void userDump();
    int userCount();
    bool userExist(char * user);
    
    
};

class User{
    typedef enum UserRole{ ADMIN, USER }UserRole;
    typedef enum UserPermission { USER_JOKER = 4, USER_RD = 2, USER_WR = 1, USER_RDWR = 3}UserPermission;

    std::string m_baseDir;
    std::string m_currentWorkingDir;
    
    
    nlohmann::json m_userRegister;
    
    
};


#endif
