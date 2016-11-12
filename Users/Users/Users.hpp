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


class User{
    typedef enum UserRole{ ADMIN=1, USER=0 }UserRole;
    typedef enum UserPermission { USER_JOKER = 4, USER_RD = 2, USER_WR = 1, USER_RDWR = 3 }UserPermission;
    typedef enum SpecialDir{ HOMEDIR=0,ROOTDIR=1,PARENTDIR=2,SAMEDIR=3}SpecialDir;
    std::string m_userBaseDir;
    std::string m_currentWorkingDir;
    nlohmann::json m_userRegister;
    
    /* From / To Json Values */
    std::string             m_accountname;
    std::string             m_password;
    UserPermission          m_permissions;
    std::string             m_baseDir;
    unsigned int            m_role;
    int                     m_quota;
    std::string             m_lastDir;
public:
    bool                    m_validUser;
    User(nlohmann::json userEntry);
    bool userAuthorized(std::string & password);
    std::string getUserCwd();
    bool userChdir(std::string & path);
};

class Users
{
    
    std::string     m_configFilePath;
    nlohmann::json  m_j;
    nlohmann::json  m_user;
    
    
    
    



public:
    
    Users (const char * jsonpath);
    ~Users(){}
    
    
    void userDump();
    int userCount();
    bool userExist(char * user);

    User user(char * user);
    
};


#endif
