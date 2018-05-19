@echo off
echo =====================================
echo = BetterRA 脚本解密程序 欢迎使用... =
echo =====================================
if not exist Decrypt (
	mkdir Decrypt
)
set Script_File=0
for %%i in (*.txt) do (
	set /a Script_File+=1
)
echo [提示] 目录下共有 %Script_File% 个txt文件待解密...
if %Script_File% == 0 (
	echo [提示] 请把加密文件放入本文件同目录中...
	goto end
)
echo;
set/p key=请输入秘钥(必须为8位):
for %%i in (*.txt) do (
	echo [提示] 正在解密 %%i 到 Decrypt\%%~ni.txt
	BetterRA_Script.exe -d %%~ni.txt Decrypt\%%~ni.txt %key%
)

echo;
echo =====================================
echo = 加密完毕！                        =
echo =====================================
:end
pause