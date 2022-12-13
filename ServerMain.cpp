#include "ServerMain.h"
using namespace std;

int RestartCount;
tm* NowTime;
tm* LastRestartTime;
LocalConfig MainConfig;

void ReadConfig() {
    // 读取配置文件 V1
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
    cout << "\tCommand: " << MainConfig.Command << endl;
    cout << "\tRestartTimeout: " << MainConfig.RestartTimeout << endl;
    puts("");
}

int main() {
    system("chcp 65001"); // 使用 UTF-8 格式化控制台
    system("echo off");
    system("cls");
    setTitle("SM服务器启动器 V1.3");
    color(COLOR_SUCCESS);
    puts("[SUCC] 软件启动成功！");
    puts("");
    color(COLOR_TIPS);
    puts("========== SM服务器启动器 ==========");
    puts("版本：V1.3");
    puts("构建日期：2022-12-13");
    puts("作者：CodeZhangBorui");
    puts("");
    puts("");
    while(1) {
        ReadConfig();
        NowTime = GetNowTime();
        LastRestartTime = NowTime;
        stringstream title;
        title
                << "SM服务器启动器 V1.3 [ 进程退出次数："
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
