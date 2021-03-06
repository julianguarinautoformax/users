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

static const int CRUD_NO_ERROR      = 0x0;
static const int CRUD_ERROR         = 0x1;
static const int CRUD_USR_EXIST     = 0x2;
static const int CRUD_USR_DONT_EXIST= 0x3;

class User{

public:
    typedef enum UserPermission { USER_JOKER = 4, USER_RD = 2, USER_WR = 1, USER_RDWR = 3 }UserPermission;

private:
    
    /************** MACHINE FILE SYSTEM REFERENCED **********/
    bool            m_dirtybit;
    std::string     m_originalAccountName;
    
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
    std::string     m_lastIp;
    int             m_lastTcpPort;
    
    
    void            userUpdateLastDir();
public:
    /* Object CTRs and validation */
    bool            m_validUser;
    bool            m_authUser;
    
    User()          {m_validUser = false;}
    ~User()         {}
    User(nlohmann::json userEntry, std::string appServerPath);
    
    bool            userIsAuthorized(std::string & password);
    
    /************** ROOT IS THE HOME DIR OF EACH USER **************/
    std::string     userCwd(bool machineCWD=false);
    int        		userChdir(std::string path);
    int             userMkdir(std::string path);
    int             userRmDir(std::string dirpath);
    int             userRmFile(std::string filepath);
    
    /************** BOTH FUNCTIONS UPDATE M_LEFTQUOTA **************/
    unsigned int    userUsedQuota(std::string path = "/");
    unsigned int 	userLeftQuota();
    unsigned int    userLeftQuotaFast() {return m_leftquota;}
    
    
    /* Getters and Setters */
    void            set_accountname(std::string an)     {m_accountname = an; m_dirtybit = true;}
    std::string     accountname()                       {return m_accountname;}
    
    void            set_password(std::string an)        {m_password = an; m_dirtybit = true;}
    std::string     password()                          {return m_password;}
    
    void            set_permissions(UserPermission an)  {m_permissions = an; m_dirtybit = true;}
    UserPermission  permissions()                       {return m_permissions;}
    
    void            set_baseDir(std::string an)         {m_baseDir = an; m_dirtybit = true;}
    std::string     baseDir()                           {return m_baseDir;}
    
    void            set_role(unsigned int an)           {m_role = an; m_dirtybit = true;}
    unsigned int    role()                              {return m_role;}
    
    void            set_quota(int an)                   {m_quota = an < (m_quota - m_leftquota) ? m_quota - m_leftquota : an ; m_dirtybit = true;}
    int             quota()                             {return m_quota;}
    
    void            set_lastIp(std::string ip)          {m_lastIp = m_lastIp; m_dirtybit = true;}
    std::string     lastIp()                            {return m_lastIp;}
    
    void            set_lastPort(unsigned int port)     {m_lastTcpPort = port; m_dirtybit = true;}
    int             lastTcpPort()                       {return m_lastTcpPort;}
    
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
    int             crudError;
    void            userDump();
    int             userCount();
    bool            userExist(std::string);
    int             userIndex(std::string);
    
    /************** CREATE USER **************/
    int             userCreate(std::string userName, std::string password, unsigned int roles, User::UserPermission perm, int quota=10);
    int             userCreate(nlohmann::json newUser);
    /*************   READ USER  **************/
    User            user(std::string user);
    nlohmann::json  user_j(std::string user);
    /************** UPDATE USER **************/
    int             userUpdate(User user, bool createOnNonExistance = false);
    int             userUpdate(nlohmann::json account);
    /************** DELETE USER **************/
    int             userDelete(User user);
    
    
    
/*** Queries **/
    nlohmann::json  config(){return m_j[0];}
    nlohmann::json  accountUsersByRole(unsigned int roles = USER_ROLE_USER);
    

    
    
};


#endif
