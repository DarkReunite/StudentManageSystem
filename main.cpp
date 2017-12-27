//
//  main.cpp
//  StudentManageSystem
//
//  Created by Dragon on 2017/12/17.
//  Copyright © 2017年 Dragon. All rights reserved.
//

#include <iostream>
#include <mysql.h>
using namespace std;

MYSQL mysql;
MYSQL_RES *result;
MYSQL_ROW row;

const char *IP  = "127.0.0.1";
const char *Username = "root";
const char *Password = "123456";
const char *Database = "StudentDatabase";

//连接数据库
void connectDB()
{
   
    /*连接之前，先用mysql_init初始化MYSQL连接句柄*/
    mysql_init(&mysql);
    
    /*使用mysql_real_connect连接服务器,其参数依次为MYSQL句柄，服务器IP地址，
     登录mysql的用户名，密码，要连接的数据库等*/
    
    if(!mysql_real_connect(&mysql, IP, Username, Password, Database, 0, NULL, 0)) {
        printf("connecting to Mysql error:%d from %s\n",mysql_errno(&mysql), mysql_error(&mysql));
    }else {
        printf("Connected Mysql successful!\n");
    }
    
    //将程序默认的Latin1字符集改成utf-8字符集
    if (!mysql_set_character_set(&mysql, "utf8"))
        printf("New client character set: %s\n", mysql_character_set_name(&mysql));
}

//插入数据的函数
void insertInformation()
{
    string no;
    string name;
    string phone;
    
    string sql = "insert into Student values";
    string dou = ",";
    string values;
    
    char *Sql_query;
    
    cout<<"请输入学号："<<endl;
    cin>>no;
    cout<<"请输入姓名："<<endl;
    cin>>name;
    cout<<"请输入手机号："<<endl;
    cin>>phone;
    
    values = "(" + no + dou + "'" + name + "'" + dou + phone + ")";     //语句的拼接
    sql += values;
    cout<<sql<<endl;
    
    Sql_query = strdup(sql.c_str());                    //将string类型转换成char类型并将地址传给Sql_query
    if(!mysql_query(&mysql, Sql_query))                 //执行成功时返回0
    {
        cout<<"插入数据成功"<<endl;
        return;
    }
    else
        cout<<"插入数据失败"<<endl;
    
}

//显示所有通讯信息
void printAll()
{
    const char *sql_query = "select * from Student";
    unsigned int num_fields;
    unsigned int i;
    
    mysql_query(&mysql, sql_query);                   //函数成功时返回0
    result = mysql_store_result(&mysql);
    
    num_fields = mysql_num_fields(result);           //返回结果集中的列数并赋值给num_fields
    
    //查询所有值的操作
    while ((row = mysql_fetch_row(result)))          //返回目前行 执行一次之后自动跳到下一行  当目前行不为空时
    {
        unsigned long *lengths;
        lengths = mysql_fetch_lengths(result);       //将一行的长度
        for(i = 0; i < num_fields; i++)
        {
            printf("[%.*s] ", (int)lengths[i],row[i] ? row[i] : "NULL");
        }
        printf("\n");
    }
    return;
}

//按照学号查找信息
void searchNum()
{
    string sql = "select * from Student where Sno = ";
    string Sno;
    char *sql_search;
    unsigned int num_column;
    unsigned int i;
    
    cout<<"请输入您要查找的学号："<<endl;
    cin>>Sno;
    sql += Sno;
    
    sql_search = strdup(sql.c_str());
    
    mysql_query(&mysql, sql_search);
    result = mysql_store_result(&mysql);
    
    num_column = mysql_num_fields(result);
    
    row = mysql_fetch_row(result);
    if (!row)
    {                                 //查询结果不存在时返回0或者null
        cout<<"您要找的学号不存在！"<<endl;
        return;
    }
    else
        cout<<"您要查找的是："<<endl;
    unsigned long *lengths;
    lengths = mysql_fetch_lengths(result);       //将一行的长度
    
    
    for(i = 0; i < num_column; i++)
    {
        printf("[%.*s] ", (int)lengths[i],row[i] ? row[i] : "NULL");
    }
    printf("\n");
}


//按照姓名查找信息
void searchName()
{
    string sql = "select * from Student where Sname = ";
    string Sname;
    char *sql_search;
    unsigned int num_column;
    unsigned int i;
    
    cout<<"请输入您要查找的姓名："<<endl;
    cin>>Sname;
    
    sql = sql + "'" + Sname + "'";
    sql_search = strdup(sql.c_str());
    
    mysql_query(&mysql, sql_search);
    result = mysql_store_result(&mysql);
    
    num_column = mysql_num_fields(result);
    row = mysql_fetch_row(result);
    if (!row)
    {                                           //查询结果不存在时返回0或者null
        cout<<"您要找的学生不存在！"<<endl;
        return;
    }
    else
        cout<<"您要查找的是："<<endl;
    
    unsigned long *lengths;
    lengths = mysql_fetch_lengths(result);       //将一行的长度
    
    
    for(i = 0; i < num_column; i++)
    {
        printf("[%.*s] ", (int)lengths[i],row[i] ? row[i] : "NULL");
    }
    printf("\n");
}


//通过查找姓名删除通讯录
void deleteName()
{
    string sql_delete = "delete from Student where Sname = ";
    string sql_select = "select * from Student where Sname = ";
    string Sname;
    char *sql_Delete;
    char *sql_Select;
    unsigned int num_column;
    
    cout<<"请输入你要删除的姓名"<<endl;
    cin>>Sname;
    
    sql_delete = sql_delete + "'" + Sname + "'";
    sql_select = sql_select + "'" + Sname + "'";
    
    sql_Delete = strdup(sql_delete.c_str());
    sql_Select = strdup(sql_select.c_str());
    
    mysql_query(&mysql, sql_Select);
    result = mysql_store_result(&mysql);
    num_column = mysql_num_fields(result);
    row = mysql_fetch_row(result);
    
     //查询结果不存在时返回0或者null
    if (!row)
    {
        cout<<"您要找的学生不存在！"<<endl;
        return;
    }
    else
    {
        mysql_query(&mysql, sql_Delete);
        cout<<"删除成功！"<<endl;
    }
    
}

//按照学号删除通讯录
void deleteNum()
{
    string sql_delete = "delete from Student where Sno = ";
    string sql_select = "select * from Student where Sno = ";
    string Sno;
    const char *sql_Select;
    const char *sql_Delete;
    unsigned int num_column;
    
    cout<<"请输入你要删除的学号:"<<endl;
    cin>>Sno;
    sql_select += Sno;
    sql_delete += Sno;

    sql_Select = strdup(sql_select.c_str());
    sql_Delete = strdup(sql_delete.c_str());
    
    mysql_query(&mysql, sql_Select);
    result = mysql_store_result(&mysql);
    
    num_column = mysql_num_fields(result);
    row = mysql_fetch_row(result);
    
    //查询结果不存在时返回0或者null
    if (!row)
    {
        cout<<"您要找的学生不存在！"<<endl;
        return;
    }
    else
    {
        mysql_query(&mysql, sql_Delete);
        cout<<"删除成功！"<<endl;
    }
    
}

void interfacePrint()
{
    cout<<"\n*******************^@^欢迎使用通讯录系统*******************\n";
    cout<<"           *          1 插入通讯录                       \n";
    cout<<"           *          2 查询数据                          \n";
    cout<<"           *          3 删除数据                          \n";
    cout<<"           *          4 显示所有通讯录                     \n";
    cout<<"           *          0 退出管理系统                       \n";
    cout<<"*******************^@^欢迎使用通讯录系统*******************\n";
    
}



int main(int argc, const char * argv[])
{
    int menu = 1;
    int choice = 0;
    connectDB();

   
    
    while (menu != 0)
    {
        interfacePrint();
        cout<<"请选择0-4："<<endl;
        cin>>menu;
        
        switch (menu) {
            case 1:
                insertInformation();
                break;
            case 2:     //查询数据
                while (choice != 1 && choice != 2)
                {        //选择的序号不匹配时重复询问
                    cout<<"请选择查找方式： 1.按学号查找  2.按姓名查找"<<endl;
                    cin>>choice;
                    if (choice == 1)
                    {
                        searchNum();
                    }
                    if (choice == 2)
                    {
                        searchName();
                    }
                }
                break;
            case 3:
                while (choice != 1 && choice != 2)
                {
                    cout<<"请选择删除方式： 1.按学号删除  2.按姓名删除"<<endl;
                    cin>>choice;
                    if (choice == 1)
                    {
                        deleteNum();
                    }
                    if (choice == 2)
                    {
                        deleteName();
                    }
                }
                break;
            
            case 4:
                printAll();
                break;
        }
    }
    
    mysql_free_result(result);
    mysql_close(&mysql); /*关闭连接*/
    return 0;
}

