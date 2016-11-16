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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include "json.hpp"
#pragma clang pop

static const int USER_ROLE_USER  = 0x01;
static const int USER_ROLE_ADMIN = 0x02;
/* PSEUDO - ORM */
class User{

public:
    typedef enum UserPermission { USER_JOKER = 4, USER_RD = 2, USER_WR = 1, USER_RDWR = 3 }UserPermission;

private:
    
    /************** MACHINE FILE SYSTEM REFERENCED **********/
    std::string     m_userBaseDir;
    std::string     m_currentWorkingDir;
    nlohmann::json  m_userRegister;
    
    /* From / To Json Values */
    std::string     m_accountname;
    std::string     m_password;
    UserPermission  m_permissions;
    std::string     m_baseDir;
    unsigned int    m_role;
    int             m_quota;
    int             m_leftquota;
    std::string     m_lastDir;

    void            userUpdateLastDir();
public:
    /* Object CTRs and validation */
    bool            m_validUser;
    User()          {m_validUser = false;}
    ~User()         {}
    User(nlohmann::json userEntry, std::string appServerPath);
    
    bool            userIsAuthorized(std::string & password);
    
    /************** ROOT IS THE HOME DIR OF EACH USER **************/
    std::string     userCwd();
    int        		userChdir(std::string & path);
    int             userMkdir(std::string & path);
    int             userRmDir(std::string & dirpath);
    int             userRmFile(std::string & filepath);
    
    /************** BOTH FUNCTIONS UPDATE M_LEFTQUOTA **************/
    unsigned int    userUsedQuota(std::string path = "/");
    unsigned int 	userLeftQuota();
    unsigned int    userLeftQuotaFast() {return m_leftquota;}
    
    
    /* Getters and Setters */
    void            set_accountname(std::string an) {m_accountname = an;}
    std::string     accountname()                   {return m_accountname;}
    
    void            set_password(std::string an) {m_password = an;}
    std::string     password()                   {return m_password;}
    
    void            set_permissions(UserPermission an) {m_permissions = an;}
    UserPermission  permissions()                      {return m_permissions;}
    
    void            set_baseDir(std::string an) {m_baseDir = an;}
    std::string     baseDir()                   {return m_baseDir;}
    
    void            set_role(unsigned int an)   {m_role = an;}
    unsigned int    role()                      {return m_role;}
    
    void            set_quota(int an)   {m_quota = an;}
    int             quota()             {return m_quota;}
    
    
};
/* DATA MODEL */
class Users
{
    
    std::string     m_configFilePath;
    nlohmann::json  m_j;
    nlohmann::json  m_user;
public:
    
    Users (std::string jsonpath);
    ~Users(){}
private:
    int             persistModel            ();
    int             updateUserIntoModel     (nlohmann::json existingAccount);
    int             commitNewUserIntoModel  (nlohmann::json newAccount);
    int             deleteUserFromModel     (int            accountIndex);
public:
    void            userDump();
    int             userCount();
    bool            userExist(std::string);
    int             userIndex(std::string);
    
    /************** CREATE USER **************/
    int             userCreate(std::string userName, std::string password, unsigned int roles, User::UserPermission perm, int quota=10);
    
    /*************   READ USER  **************/
    User            user(std::string user);
    
    /************** UPDATE USER **************/
    int             userUpdate(User user, bool createOnNonExistance = false);
    
    /************** DELETE USER **************/
    int             userDelete(User user);
    
    
    

    
    
};


#endif
