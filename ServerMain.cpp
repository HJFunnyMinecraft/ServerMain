//基础头文件
#include <iostream>
#include <cstdio>
#include <cstdlib>
//字符串
#include <cstring>
#include <string>
//时间
#include <ctime>
//用于设置控制台字体颜色
#include "windows.h"
//字符串流：用于编辑字符串
#include <sstream>
//文件流：用于读取配置文件
#include <fstream>
//用于判断文件可用性
#include <shlobj.h>
#include <CommDlg.h>
#include <io.h>
//定义统一颜色
#define COLOR_SUCCESS 10
#define COLOR_PROCESS 14
#define COLOR_ERROR 12
#define COLOR_TIPS 9
#define COLOR_DEFAULT 7
using namespace std;

int RestartCount;
tm* NowTime;
tm* LastRestartTime;
struct LocalConfig {
    string Command;
    int RestartTimeout;
};
LocalConfig MainConfig;

inline void GetNowTime() {
    time_t now = time(NULL);
    NowTime = localtime(&now);
}
string GetTimeString(tm* tm_t) {
    stringstream ret;
    ret << tm_t->tm_hour << ":" << tm_t->tm_min;
    return ret.str();
}
string GetDateString(tm* tm_t) {
    stringstream ret;
    ret << tm_t->tm_mon + 1 << "-" << tm_t->tm_mday;
    return ret.str();
}
void setTitle(string title) {
    stringstream cmd;
    cmd << "title  "<< title << "";
    system(cmd.str().c_str());
}
void timeout(int second) {
    stringstream cmd;
    cmd << "timeout " << second;
    system(cmd.str().c_str());
}
void color(int x) {
    /*
    0 = 黑色       8  = 灰色
    1 = 蓝色       9  = 淡蓝色
    2 = 绿色       10 = 淡绿色
    3 = 浅绿色     11 = 淡浅绿色
    4 = 红色       12 = 淡红色
    5 = 紫色       13 = 淡紫色
    6 = 黄色       14 = 淡黄色
    7 = 白色       15 = 亮白色
    */
    if (x >= 0 && x <= 15)
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);
    else
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}
inline bool fileExits(const char* name) {
    int acsResult = _access(name, 00);
    if (acsResult == 0) return true;
    else return false;
}
inline bool fileRead(const char* name) {
    int acsResult = _access(name, 04);
    if (acsResult == 0) return true;
    else return false;
}
void ReadConfig() {
    MainConfig.Command = "";
    MainConfig.RestartTimeout = 5;
    color(COLOR_PROCESS);
    puts("[PROC] 读取配置文件 config.txt ...");
    if(!fileExits("config.txt")) {
        color(COLOR_ERROR);
        puts("[ERR!] 配置文件不存在！将在10秒后重试 ……");
        timeout(10);
        ReadConfig();
        return;
    }
    if(!fileRead("config.txt")) {
        color(COLOR_ERROR);
        puts("[ERR!] 配置文件无法读取！将在10秒后重试 ……");
        timeout(10);
        ReadConfig();
        return;
    }
    ifstream in("config.txt");
    string reader;
    color(COLOR_SUCCESS);
    puts("[SUCC] 配置文件打开成功！");
    while(reader != "#Config_Start.") {
        in >> reader;
        if(in.eof()) {
            color(COLOR_ERROR);
            puts("[ERR!] 配置文件没有任何配置组！将在10秒后重试 ……");
            timeout(10);
            ReadConfig();
            return;
        }
    }
    while(reader != "#Config_End.") {
        in >> reader;
        if(in.eof()) {
            color(COLOR_ERROR);
            puts("[ERR!] 配置文件没有有效结尾！将在10秒后重试 ……");
            timeout(10);
            ReadConfig();
            return;
        }
        if(reader == "command=") {
            stringstream command;
            in >> reader;
            while(reader != "#Config_End." && reader != "timeout=") {
                command << reader << " ";
                in >> reader;
            }
            MainConfig.Command = command.str();
        }
        if(reader == "timeout=") {
            int tmout;
            in >> tmout;
            MainConfig.RestartTimeout = tmout;
        }
    }
    in.close();
    color(COLOR_SUCCESS);
    puts("[SUCC] 配置文件读取成功：");
    cout << "\tJavaCommand: " << MainConfig.Command << endl;
    cout << "\tRestartTimeout: " << MainConfig.RestartTimeout << endl;
    puts("");
}

int main() {
    system("echo off");
    system("cls");
    setTitle("PowerCode 进程守护器 V1.2");
    color(COLOR_SUCCESS);
    puts("[SUCC] 软件启动成功！");
    puts("");
    color(COLOR_TIPS);
    puts("========== PowerCode 进程守护器 ==========");
    puts("版本：V1.2");
    puts("构建日期：2022-12-01");
    puts("作者：CodeZhangBorui");
    puts("");
    puts("");
    while(1) {
        ReadConfig();
        GetNowTime();
        LastRestartTime = NowTime;
        stringstream title;
        title
                << "PowerCode 进程守护器 V1.2 [ 进程退出次数："
                << RestartCount
                << " 上次重启："
                << GetDateString(LastRestartTime)
                << " "
                << GetTimeString(LastRestartTime)
                << " ]";
        setTitle(title.str());
        color(COLOR_TIPS);
        puts("> 启动进程……");
        puts("");
        color(COLOR_DEFAULT);
        system(MainConfig.Command.c_str());
        puts("");
        color(COLOR_PROCESS);
        puts("检测到进程终止，即将重启 ……");
        puts("若希望停止进程，请直接关闭此窗口。");
        timeout(MainConfig.RestartTimeout);
        RestartCount++;
        puts("");
    }
}
