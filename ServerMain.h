// 基础头文件
#include <iostream>
#include <cstdio>
#include <cstdlib>
// 字符串
#include <cstring>
#include <string>
// 时间
#include <ctime>
// 用于设置控制台字体颜色
#include "windows.h"
// 字符串流：用于编辑字符串
#include <sstream>
// 文件流：用于读取配置文件
#include <fstream>
// 用于判断文件可用性
#include <shlobj.h>
#include <CommDlg.h>
#include <io.h>
// JSONCPP 库，用于读取 JSON 配置
#include "lib/jsoncpp/json/json.h"
#include "lib/jsoncpp/jsoncpp.cpp"
// 定义统一颜色
#define COLOR_SUCCESS 10
#define COLOR_PROCESS 14
#define COLOR_ERROR 12
#define COLOR_TIPS 9
#define COLOR_DEFAULT 7
using namespace std;

struct LocalConfig {
    // 用于存储配置的结构体
    string Command;
    int RestartTimeout;
};

inline tm* GetNowTime() {
    // 获取当前时间的 time_t 对象并赋值到 NowTime
    time_t now = time(NULL);
    return localtime(&now);
}
string GetTimeString(tm* tm_t) {
    // 对时间格式化
    stringstream ret;
    ret << tm_t->tm_hour << ":" << tm_t->tm_min;
    return ret.str();
}
string GetDateString(tm* tm_t) {
    // 对日期格式化
    stringstream ret;
    ret << tm_t->tm_mon + 1 << "-" << tm_t->tm_mday;
    return ret.str();
}
void setTitle(string title) {
    // 设置控制台标题
    // 原通过控制台执行 title：已废弃
    SetConsoleTitle(title.c_str());
}
void color(int x) {
    // 设置控制台文字颜色
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
void timeout(int second) {
    // 控制台等待
    // 原通过控制台执行 timeout：已废弃
    if(second > 0) {
        for(int i = 0; i < second; i++) {
            printf("\r");
            int pb = (int)(20.0 * (i * 1.0 / second));
            color(1);
            for(int j = 1; j <= pb; j++) {
                printf("━");
            }
            color(8);
            for(int j = 1; j <= 20 - pb; j++) {
                printf("━");
            }
            color(7);
            printf(" 等待 %d 秒 ……   ", second - i);
            _sleep(1000);
        }
        color(10);
        printf("\r━━━━━━━━━━━━━━━━━━━━");
        color(7);
        printf(" 已等待 %d 秒     \n", second);
    } else if(timeout < 0) {
        puts("请按任意键继续……");
        system("pause > nul");
    }
}
inline bool fileExits(const char* name) {
    // 文件是否存在
    int acsResult = _access(name, 00);
    if (acsResult == 0) return true;
    else return false;
}
inline bool fileRead(const char* name) {
    // 文件是否可读取
    int acsResult = _access(name, 04);
    if (acsResult == 0) return true;
    else return false;
}
